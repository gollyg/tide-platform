<?php

/**
 * @file
 * Create demo event.
 */

use Drupal\node\Entity\Node;
use Drupal\paragraphs\Entity\Paragraph;
use Drupal\tide_demo\TideDemoHelper;
use Drupal\tide_demo\TideDemoRepository;

/**
 * Implements hook_tide_demo_create_ENTITY_TYPE_BUNDLE_weight().
 */
function tide_demo_create_node_event_weight() {
  return 10;
}

/**
 * Implements hook_tide_demo_create_ENTITY_TYPE_BUNDLE().
 */
function tide_demo_create_node_event() {
  $nodes = [];
  $repository = TideDemoRepository::getInstance();

  for ($i = 1; $i <= 10; $i++) {
    $uid = TideDemoHelper::randomUid();

    $site_sections = TideDemoHelper::randomSiteSections();
    $primary_site_id = TideDemoHelper::randomSite($site_sections);

    $status = Node::PUBLISHED;
    $node_data = [
      'type' => 'event',
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
      'field_audience' => [
        ['target_id' => TideDemoHelper::randomAudience()],
      ],
      'field_event_category' => [
        ['target_id' => TideDemoHelper::randomEventCategory()],
      ],
      'field_event_description' => [
        'value' => TideDemoHelper::randomPlainParagraph(),
        'format' => 'rich_text',
      ],
      'field_featured_image' => [
        ['target_id' => TideDemoHelper::randomImage()],
      ],
      'field_landing_page_summary' => [
        'value' => TideDemoHelper::randomSentence(10, 20),
      ],
    ];

    // Add tags.
    $tags = TideDemoHelper::randomTags(3);
    if (!empty($tags)) {
      foreach ($tags as $tag_id => $tag) {
        $node_data['field_tags'][] = ['target_id' => $tag_id];
      }
    }

    // Add Event Details.
    $event_details_data = [
      'type' => 'event_details',
      'field_paragraph_link' => [TideDemoHelper::randomCtaLinkFieldValue()],
      'field_paragraph_date_range' => [['value' => TideDemoHelper::randomDate(), 'end_value' => TideDemoHelper::randomDate()]],
      'field_event_requirements' => [
        ['target_id' => TideDemoHelper::randomEventRequirements()],
      ],
      'field_paragraph_location' => [
        [
          'langcode' => '',
          'country_code' => 'AU',
          'administrative_area' => 'VIC',
          'locality' => 'Melbourne',
          'postal_code' => 3001,
          'address_line1' => 'Department of Premier and Cabinet',
          'address_line2' => 'GPO Box 4509',
        ],
      ],
      'field_paragraph_event_price_from' => [
        'value' => mt_rand(80, 120),
      ],
      'field_paragraph_event_price_to' => [
        'value' => mt_rand(120, 160),
      ],

    ];

    $event_details = Paragraph::create($event_details_data);
    $event_details->save();
    $repository->trackEntity($event_details);

    $node_data['field_event_details'] = [
      [
        'target_id' => $event_details->id(),
        'target_revision_id' => $event_details->getRevisionId(),
      ],
    ];

    // Add Contact Us.
    $contact_us_data = [
      'type' => 'contact_us',
      'field_paragraph_title' => TideDemoHelper::randomSentence(3, 5),
      'field_paragraph_name' => TideDemoHelper::randomSentence(3, 5),
      'field_paragraph_email' => 'no-reply@vic.gov.au',
      'field_paragraph_cta' => [TideDemoHelper::randomCtaLinkFieldValue()],
      'field_paragraph_department_name' => TideDemoHelper::randomSentence(3, 5),
      'field_paragraph_location' => [
        [
          'langcode' => '',
          'country_code' => 'AU',
          'administrative_area' => 'VIC',
          'locality' => 'Melbourne',
          'postal_code' => 3001,
          'address_line1' => 'Department of Premier and Cabinet',
          'address_line2' => 'GPO Box 4509',
        ],
      ],
      'field_paragraph_postal_address' => [
        [
          'langcode' => '',
          'country_code' => 'AU',
          'administrative_area' => 'VIC',
          'locality' => 'Melbourne',
          'postal_code' => 3001,
          'address_line1' => 'Department of Premier and Cabinet',
          'address_line2' => 'GPO Box 4509',
        ],
      ],
    ];
    $contact_us = Paragraph::create($contact_us_data);
    $contact_us->save();
    $repository->trackEntity($contact_us);
    $node_data['field_landing_page_contact'] = [
      [
        'target_id' => $contact_us->id(),
        'target_revision_id' => $contact_us->getRevisionId(),
      ],
    ];

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
      'field_landing_page_show_contact',
      'field_show_content_rating',
      'field_show_social_sharing',
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
