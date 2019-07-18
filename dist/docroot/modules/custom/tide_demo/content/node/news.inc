<?php

/**
 * @file
 * Create demo news.
 */

use Drupal\node\Entity\Node;
use Drupal\paragraphs\Entity\Paragraph;
use Drupal\tide_demo\TideDemoHelper;
use Drupal\tide_demo\TideDemoRepository;

/**
 * Implements hook_tide_demo_create_ENTITY_TYPE_BUNDLE_weight().
 */
function tide_demo_create_node_news_weight() {
  return 10;
}

/**
 * Implements hook_tide_demo_create_ENTITY_TYPE_BUNDLE().
 */
function tide_demo_create_node_news() {
  $nodes = [];
  $repository = TideDemoRepository::getInstance();

  for ($i = 1; $i <= 10; $i++) {
    $uid = TideDemoHelper::randomUid();

    $site_sections = TideDemoHelper::randomSiteSections();
    $primary_site_id = TideDemoHelper::randomSite($site_sections);

    $status = Node::PUBLISHED;
    $node_data = [
      'type' => 'news',
      'title' => TideDemoHelper::randomSentence(),
      'uid' => $uid,
      'status' => $status,
      'field_news_intro_text' => [
        'value' => TideDemoHelper::randomPlainParagraph(),
      ],
      'body' => [
        'value' => TideDemoHelper::randomRichTextWithMedia(),
        'summary' => TideDemoHelper::randomSentence(10, 30),
        'format' => 'rich_text',
      ],
      'field_node_site' => TideDemoHelper::getFieldValueForSiteSections($site_sections),
      'field_node_primary_site' => [
        ['target_id' => $primary_site_id],
      ],
      'field_topic' => [
        ['target_id' => TideDemoHelper::randomTopic()],
      ],
      'field_featured_image' => [
        ['target_id' => TideDemoHelper::randomImage()],
      ],
      'field_node_department' => [
        ['target_id' => TideDemoHelper::randomDepartment()],
      ],
      'field_location' => [
        ['target_id' => TideDemoHelper::randomLocation()],
      ],
      'field_news_date' => [['value' => TideDemoHelper::randomDate()]],
    ];

    // Add tags.
    $tags = TideDemoHelper::randomTags(3);
    if (!empty($tags)) {
      foreach ($tags as $tag_id => $tag) {
        $node_data['field_tags'][] = ['target_id' => $tag_id];
      }
    }

    // Add Related links.
    for ($j = 1; $j <= mt_rand(1, 5); $j++) {
      $link_field_value = TideDemoHelper::randomLinkFieldValue();
      if (!count($link_field_value)) {
        break;
      }
      $related_link_data = [
        'type' => 'related_links',
        'field_paragraph_link' => [$link_field_value],
      ];
      $related_link = Paragraph::create($related_link_data);
      $related_link->save();
      $repository->trackEntity($related_link);
      $node_data['field_related_links'][] = [
        'target_id' => $related_link->id(),
        'target_revision_id' => $related_link->getRevisionId(),
      ];
    }

    $node = Node::create($node_data);

    $boolean_fields = [
      'field_show_related_content',
      'field_show_social_sharing',
      'field_show_content_rating',
    ];
    foreach ($boolean_fields as $boolean_field) {
      if ($node->hasField($boolean_field)) {
        $node->set($boolean_field, TideDemoHelper::randomBool());
      }
    }

    if ($node->hasField('moderation_state')) {
      if ($status) {
        $node->set('moderation_state', 'published');
      }
    }

    try {
      $node->save();
      $nodes[] = $node;
    }
    catch (Exception $exception) {
      watchdog_exception('tide_demo', $exception);
    }
  }

  return $nodes;
}
