<?php

/**
 * @file
 * Create demo landing pages.
 */

use Drupal\menu_link_content\Entity\MenuLinkContent;
use Drupal\node\Entity\Node;
use Drupal\paragraphs\Entity\Paragraph;
use Drupal\tide_demo\TideDemoHelper;
use Drupal\tide_demo\TideDemoRepository;

/**
 * Implements hook_tide_demo_create_ENTITY_TYPE_BUNDLE_weight().
 */
function tide_demo_create_node_landing_page_weight() {
  return 100;
}

/**
 * Implements hook_tide_demo_create_ENTITY_TYPE_BUNDLE().
 */
function tide_demo_create_node_landing_page() {
  $nodes = [];
  $repository = TideDemoRepository::getInstance();
  $site_menus = $repository->getDemoEntities('site_menu');

  $total = 10;
  for ($i = 1; $i <= $total; $i++) {
    // Turn off Random mode for the last landing page.
    // It will be generated with all components.
    $random = ($i < $total) ? TRUE : FALSE;
    $uid = TideDemoHelper::randomUid();

    $site_sections = TideDemoHelper::randomSiteSections();
    $primary_site_id = TideDemoHelper::randomSite($site_sections);
    /** @var \Drupal\taxonomy\Entity\Term $primary_site */
    $primary_site = \Drupal::service('tide_site.helper')->getSiteById($primary_site_id);

    $status = Node::PUBLISHED;
    $node_data = [
      'type' => 'landing_page',
      'title' => $random ? TideDemoHelper::randomSentence() : 'Demo Landing Page',
      'uid' => $uid,
      'status' => $status,
      'field_landing_page_intro_text' => [
        'value' => TideDemoHelper::randomPlainParagraph(),
      ],
      'field_landing_page_summary' => [
        'value' => TideDemoHelper::randomSentence(10, 20),
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
      'field_landing_page_hero_image' => [
        ['target_id' => TideDemoHelper::randomImage()],
      ],
      'field_graphical_image' => [
        ['target_id' => TideDemoHelper::randomImage()],
      ],
      'field_landing_page_hero_theme' => [
        'value' => TideDemoHelper::randomBool() ? 'dark' : 'light',
      ],
      'field_landing_page_bg_colour' => [
        'value' => TideDemoHelper::randomBool() ? 'grey' : 'white',
      ],
      'field_landing_page_nav_title' => ['value' => 'Site-section Navigation'],
    ];

    if (!$random) {
      $node_data['field_node_site'][] = ['target_id' => TideDemoRepository::SITE_ID_VICGOVAU];

      // Set path alias in the absence of Pathauto.
      if (!\Drupal::moduleHandler()->moduleExists('pathauto')) {
        $node_data['path'] = ['alias' => '/demo-landing-page'];
      }
    }

    // Add tags.
    $tags = TideDemoHelper::randomTags(3);
    if (!empty($tags)) {
      foreach ($tags as $tag_id => $tag) {
        $node_data['field_tags'][] = ['target_id' => $tag_id];
      }
    }

    // Add Hero Banner.
    $hero_banner_with_cta_data = [
      'type' => 'hero_banner_with_cta',
      'field_paragraph_cta' => [TideDemoHelper::randomCtaLinkFieldValue()],
      'field_paragraph_freetext' => TideDemoHelper::randomSentence(3, 5),
      'field_paragraph_link_text' => [TideDemoHelper::randomCtaLinkFieldValue()],
      'field_paragraph_background_image' => ['target_id' => TideDemoHelper::randomImage()],
      'field_paragraph_theme' => [
        'value' => TideDemoHelper::randomBool() ? 'dark' : 'light',
      ],
    ];

    $hero_banner_with_cta = Paragraph::create($hero_banner_with_cta_data);
    $hero_banner_with_cta->save();
    $repository->trackEntity($hero_banner_with_cta);
    $node_data['field_landing_page_hero_banner'] = [
      [
        'target_id' => $hero_banner_with_cta->id(),
        'target_revision_id' => $hero_banner_with_cta->getRevisionId(),
      ],
    ];

    // Add Key Journeys.
    $key_journeys_data = [
      'type' => 'key_journeys',
      'field_paragraph_title' => 'Want to know more about...',
      'field_paragraph_cta' => [TideDemoHelper::randomCtaLinkFieldValue()],
    ];
    for ($j = 1; $j <= 2; $j++) {
      $key_journeys_data['field_paragraph_links'][] = TideDemoHelper::randomLinkFieldValue();
    }
    $key_journeys = Paragraph::create($key_journeys_data);
    $key_journeys->save();
    $repository->trackEntity($key_journeys);
    $node_data['field_landing_page_key_journeys'] = [
      [
        'target_id' => $key_journeys->id(),
        'target_revision_id' => $key_journeys->getRevisionId(),
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

    // Header components.
    try {
      // Add Intro Banner.
      $banner = TideDemoHelper::randomIntroductionBanner();
      $node_data['field_landing_page_header'] = [
        [
          'target_id' => $banner->id(),
          'target_revision_id' => $banner->getRevisionId(),
        ],
      ];

      $embedded_search_form = TideDemoHelper::randomEmbeddedSearchForm();
      $node_data['embedded_search_form'] = [
        [
          'target_id' => $embedded_search_form->id(),
          'target_revision_id' => $embedded_search_form->getRevisionId(),
        ],
      ];

    }
    catch (Exception $e) {
      watchdog_exception('tide_demo', $e);
    }

    // Add components.
    $components = TideDemoHelper::randomLandingPageComponents(mt_rand(5, 20), $random);
    foreach ($components as $component) {
      /** @var \Drupal\paragraphs\Entity\Paragraph $component */
      $node_data['field_landing_page_component'][] = [
        'target_id' => $component->id(),
        'target_revision_id' => $component->getRevisionId(),
      ];
    }

    // Add campaigns.
    if ($random ? TideDemoHelper::randomBool() : TRUE) {
      $primary_campaign = TideDemoHelper::randomCampaign();
      if ($primary_campaign) {
        $node_data['field_landing_page_c_primary'] = [
          'target_id' => $primary_campaign->id(),
        ];
      }
    }
    if ($random ? TideDemoHelper::randomBool() : TRUE) {
      $secondary_campaign = TideDemoHelper::randomCampaign();
      if ($secondary_campaign) {
        $node_data['field_landing_page_c_secondary'] = [
          'target_id' => $secondary_campaign->id(),
        ];
      }
    }

    $node = Node::create($node_data);

    $boolean_fields = [
      'field_show_related_content',
      'field_show_social_sharing',
      'field_show_whats_next',
      'field_show_content_rating',
      'field_landing_page_show_contact',
      'field_show_table_of_content',
      'field_show_site_section_nav',
    ];
    foreach ($boolean_fields as $boolean_field) {
      if ($node->hasField($boolean_field)) {
        $node->set($boolean_field, $random ? TideDemoHelper::randomBool() : TRUE);
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

      // Set the page as its primary site homepage if necessary.
      if ($primary_site && $primary_site->hasField('field_site_homepage') && $primary_site->get('field_site_homepage')->isEmpty()) {
        $primary_site->set('field_site_homepage', [$node->id()]);
        $primary_site->save();
      }

      // Create menu links.
      foreach ($site_sections as $site_id) {
        foreach (['main', 'footer'] as $menu_prefix) {
          if (isset($site_menus[$menu_prefix . ':site:' . $site_id])) {
            /** @var \Drupal\system\Entity\Menu $menu */
            $menu = reset($site_menus[$menu_prefix . ':site:' . $site_id]);
            $menu_item = MenuLinkContent::create([
              'title' => $node->getTitle(),
              'link' => 'entity:node/' . $node->id(),
              'menu_name' => $menu->id(),
              'expanded' => TRUE,
            ]);
            $menu_item->save();
            $repository->trackEntity($menu_item);
          }
        }
      }
    }
    catch (Exception $exception) {
      watchdog_exception('tide_demo', $exception);
    }
  }

  return $nodes;
}
