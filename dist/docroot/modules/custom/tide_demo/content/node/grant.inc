<?php

/**
 * @file
 * Create demo grant.
 */

use Drupal\node\Entity\Node;
use Drupal\paragraphs\Entity\Paragraph;
use Drupal\tide_demo\TideDemoHelper;
use Drupal\tide_demo\TideDemoRepository;

/**
 * Implements hook_tide_demo_create_ENTITY_TYPE_BUNDLE_weight().
 */
function tide_demo_create_node_grant_weight() {
  return 11;
}

/**
 * Implements hook_tide_demo_create_ENTITY_TYPE_BUNDLE().
 */
function tide_demo_create_node_grant() {
  $nodes = [];
  $repository = TideDemoRepository::getInstance();

  for ($i = 1; $i <= 10; $i++) {
    $uid = TideDemoHelper::randomUid();

    $site_sections = TideDemoHelper::randomSiteSections();
    $primary_site_id = TideDemoHelper::randomSite($site_sections);

    $status = Node::PUBLISHED;
    $node_data = [
      'type' => 'grant',
      'title' => TideDemoHelper::randomSentence(),
      'uid' => $uid,
      'status' => $status,
      'field_landing_page_intro_text' => [
        'value' => TideDemoHelper::randomPlainParagraph(),
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
      'field_node_department' => [
        ['target_id' => TideDemoHelper::randomDepartment()],
      ],
      'field_overview_title' => [
        'value' => TideDemoHelper::randomSentence(),
      ],
      'field_description' => [
        'value' => TideDemoHelper::randomSentence(),
      ],
      'field_call_to_action' => [TideDemoHelper::randomLinkFieldValue()],
      'field_featured_image' => [
        ['target_id' => TideDemoHelper::randomImage()],
      ],
      'field_landing_page_summary' => [
        'value' => TideDemoHelper::randomSentence(10, 20),
      ],
      'field_node_dates' => [
        [
          'value' => TideDemoHelper::randomDate(),
          'end_value' => TideDemoHelper::randomDate(),
        ],
      ],
      'field_node_funding_level' => [
        [
          'from' => mt_rand(10000, 16000),
          'to' => mt_rand(16000, 32000),
        ],
      ],
    ];

    // Add tags.
    $tags = TideDemoHelper::randomTags(3);
    if (!empty($tags)) {
      foreach ($tags as $tag_id => $tag) {
        $node_data['field_tags'][] = ['target_id' => $tag_id];
      }
    }

    // Add guidelines content.
    $accordion_data = [
      'type' => 'accordion',
      'field_paragraph_title' => TideDemoHelper::randomSentence(3, 5),
      'field_paragraph_accordion_style' => [
        'value' => 'basic',
      ],
      'field_paragraph_accordion' => [],
    ];

    // Add 4 Guidelines accordion content data.
    $accordion_content = TideDemoHelper::randomAccordionContent();
    $accordion_data['field_paragraph_accordion'][] = [
      'target_id' => $accordion_content->id(),
      'target_revision_id' => $accordion_content->getRevisionId(),
    ];

    $accordion_content = TideDemoHelper::randomAccordionContent();
    $accordion_data['field_paragraph_accordion'][] = [
      'target_id' => $accordion_content->id(),
      'target_revision_id' => $accordion_content->getRevisionId(),
    ];

    $accordion_content = TideDemoHelper::randomAccordionContent();
    $accordion_data['field_paragraph_accordion'][] = [
      'target_id' => $accordion_content->id(),
      'target_revision_id' => $accordion_content->getRevisionId(),
    ];

    $accordion_content = TideDemoHelper::randomAccordionContent();
    $accordion_data['field_paragraph_accordion'][] = [
      'target_id' => $accordion_content->id(),
      'target_revision_id' => $accordion_content->getRevisionId(),
    ];

    // Create host paragraph.
    $accordion = Paragraph::create($accordion_data);
    $accordion->save();
    $repository->trackEntity($accordion);

    $node_data['field_node_guidelines'] = [
      'target_id' => $accordion->id(),
      'target_revision_id' => $accordion->getRevisionId(),
    ];

    // Add Timelines.
    $timelines_data = [
      'type' => 'timelines',
      'field_paragraph_title' => TideDemoHelper::randomSentence(3, 5),
      'field_timeline' => [],
    ];

    $timeline = Paragraph::create([
      'type' => 'timeline',
      'field_paragraph_title' => TideDemoHelper::randomSentence(3, 5),
      'field_paragraph_summary' => TideDemoHelper::randomSentence(10, 20),
      'field_paragraph_link' => [TideDemoHelper::randomLinkFieldValue()],
      'field_paragraph_cta_text' => TideDemoHelper::randomSentence(3, 5),
      'field_paragraph_department_name' => TideDemoHelper::randomSentence(3, 5),
      'field_paragraph_date_range' => [
        [
          'value' => TideDemoHelper::randomDate(),
          'end_value' => TideDemoHelper::randomDate(),
        ],
      ],
    ]);
    $timeline->save();
    $repository = TideDemoRepository::getInstance();
    $repository->trackEntity($timeline);

    $timelines_data['field_timeline'][] = [
      'target_id' => $timeline->id(),
      'target_revision_id' => $timeline->getRevisionId(),
    ];

    // Create host paragraph.
    $timelines = Paragraph::create($timelines_data);
    $timelines->save();
    $repository->trackEntity($timelines);

    $node_data['field_node_timeline'] = [
      'target_id' => $timelines->id(),
      'target_revision_id' => $timelines->getRevisionId(),
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

    $node = Node::create($node_data);

    $boolean_fields = [
      'field_landing_page_show_contact',
      'field_node_on_going',
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
