<?php

/**
 * @file
 * Create demo profile content.
 */

use Drupal\node\Entity\Node;
use Drupal\paragraphs\Entity\Paragraph;
use Drupal\tide_demo\TideDemoHelper;
use Drupal\tide_demo\TideDemoRepository;

/**
 * Implements hook_tide_demo_create_ENTITY_TYPE_BUNDLE().
 */
function tide_demo_create_node_profile() {
  $nodes = [];
  $repository = TideDemoRepository::getInstance();

  for ($i = 1; $i <= 10; $i++) {
    $uid = TideDemoHelper::randomUid();

    $site_sections = TideDemoHelper::randomSiteSections();
    $primary_site_id = TideDemoHelper::randomSite($site_sections);

    $status = Node::PUBLISHED;
    $node_data = [
      'type' => 'profile',
      'title' => TideDemoHelper::randomSentence(),
      'uid' => $uid,
      'status' => $status,
      'body' => [
        'value' => TideDemoHelper::randomRichTextWithMedia(),
        'summary' => TideDemoHelper::randomSentence(10, 30),
        'format' => 'rich_text',
      ],
      'field_profile_intro_text' => [
        'value' => TideDemoHelper::randomPlainParagraph(),
      ],
      'field_landing_page_summary' => [
        'value' => TideDemoHelper::randomSentence(10, 20),
      ],
      'field_life_span' => [
        'value' => TideDemoHelper::randomSentence(),
      ],
      'field_node_site' => TideDemoHelper::getFieldValueForSiteSections($site_sections),
      'field_node_primary_site' => [
        ['target_id' => $primary_site_id],
      ],
      'field_profile_category' => [
        ['target_id' => TideDemoHelper::randomProfileCategory()],
      ],
      'field_topic' => [
        ['target_id' => TideDemoHelper::randomTopic()],
      ],
      'field_year' => [
        'value' => array_rand(range(date('Y'), 2001), 1),
      ],
      'field_location' => [
        ['target_id' => TideDemoHelper::randomLocation()],
      ],
      'field_featured_image' => [
        ['target_id' => TideDemoHelper::randomImage()],
      ],

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
        'field_paragraph_summary' => ['value' => TideDemoHelper::randomPlainParagraph()],
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

    $secondary_campaign = TideDemoHelper::randomCampaign();
    if ($secondary_campaign) {
      $node_data['field_landing_page_c_secondary'] = [
        'target_id' => $secondary_campaign->id(),
      ];
    }

    $node_data['field_expertise'] = [
      'target_id' => TideDemoHelper::randomExpertise(),
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
      'field_show_related_content',
      'field_landing_page_show_contact',
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
