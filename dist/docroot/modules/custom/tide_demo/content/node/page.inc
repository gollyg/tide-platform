<?php

/**
 * @file
 * Create demo pages.
 */

use Drupal\node\Entity\Node;
use Drupal\paragraphs\Entity\Paragraph;
use Drupal\tide_demo\TideDemoHelper;
use Drupal\tide_demo\TideDemoRepository;

/**
 * Implements hook_tide_demo_create_ENTITY_TYPE_BUNDLE().
 */
function tide_demo_create_node_page() {
  $nodes = [];
  $repository = TideDemoRepository::getInstance();

  for ($i = 1; $i <= 10; $i++) {
    // Turn off Random mode for the last page.
    // It will be generated with all components.
    $random = ($i < 10) ? TRUE : FALSE;

    $uid = TideDemoHelper::randomUid();

    $site_sections = TideDemoHelper::randomSiteSections();
    $primary_site_id = TideDemoHelper::randomSite($site_sections);

    $status = Node::PUBLISHED;
    $node_data = [
      'type' => 'page',
      'title' => $random ? TideDemoHelper::randomSentence() : 'Demo Page',
      'uid' => $uid,
      'status' => $status,
      'field_page_intro_text' => [
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
      'field_graphical_image' => [
        ['target_id' => TideDemoHelper::randomImage()],
      ],
    ];

    if (!$random) {
      $node_data['field_node_site'][] = ['target_id' => TideDemoRepository::SITE_ID_VICGOVAU];

      // Set path alias in the absence of Pathauto.
      if (!\Drupal::moduleHandler()->moduleExists('pathauto')) {
        $node_data['path'] = ['alias' => '/demo-page'];
      }
    }

    // Add tags.
    $tags = TideDemoHelper::randomTags(3);
    if (!empty($tags)) {
      foreach ($tags as $tag_id => $tag) {
        $node_data['field_tags'][] = ['target_id' => $tag_id];
      }
    }

    // Add What's Next.
    for ($j = 1; $j <= mt_rand(1, 3); $j++) {
      $link_field_value = TideDemoHelper::randomLinkFieldValue();
      if (!count($link_field_value)) {
        break;
      }
      $whats_next_data = [
        'type' => 'links',
        'field_paragraph_link' => [$link_field_value],
      ];
      $whats_next = Paragraph::create($whats_next_data);
      $whats_next->save();
      $repository->trackEntity($whats_next);
      $node_data['field_whats_next'][] = [
        'target_id' => $whats_next->id(),
        'target_revision_id' => $whats_next->getRevisionId(),
      ];
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

    $booleanFields = [
      'field_show_related_content',
      'field_show_social_sharing',
      'field_show_whats_next',
      'field_show_content_rating',
    ];
    foreach ($booleanFields as $booleanField) {
      if ($node->hasField($booleanField)) {
        $node->set($booleanField, $random ? TideDemoHelper::randomBool() : TRUE);
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
