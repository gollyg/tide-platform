<?php

/**
 * @file
 * Create demo site terms.
 */

use Drupal\taxonomy\Entity\Term;
use Drupal\tide_demo\TideDemoRepository;
use Drupal\tide_demo\TideDemoHelper;

/**
 * Implements hook_tide_demo_create_ENTITY_TYPE_BUNDLE().
 */
function tide_demo_create_taxonomy_term_sites() {
  $terms = [];

  $sites = [
    'demo.vic.gov.au' => [
      'Demo 1',
      'Demo 2',
      'Demo 3',
    ],
    'demo2.vic.gov.au' => [
      'Demo 2.1',
      'Demo 2.2',
      'Demo 2.3',
    ],
    'another.demo.vic.gov.au' => [
      'Another Demo 1',
      'Another Demo 2',
      'Another Demo 3',
    ],
  ];

  $site_menus = [
    'demo.vic.gov.au' => [
      'main' => 'main-dhhs-demo',
      'footer' => 'footer-dhhs-demo',
    ],
    'demo2.vic.gov.au' => [
      'main' => 'main-dhhs-demo2',
      'footer' => 'footer-dhhs-demo2',
    ],
    'another.demo.vic.gov.au' => [
      'main' => 'main-dhhs-demo-another',
      'footer' => 'footer-dhhs-demo-another',
    ],
  ];

  $repository = TideDemoRepository::getInstance();
  $menus = $repository->getDemoEntities('menu', 'menu');

  $weight = 100;
  foreach ($sites as $site => $sections) {
    $term_site = Term::create([
      'name' => $site,
      'vid' => 'sites',
      'weight' => $weight,
      'field_site_main_menu' => [
        ['target_id' => $site_menus[$site]['main']],
      ],
      'field_site_footer_menu' => [
        ['target_id' => $site_menus[$site]['footer']],
      ],
      'field_site_domains' => [
        ['value' => $site],
      ],
      'field_acknowledgement_to_country' => [
        ['value' => "The Victorian Government acknowledges Aboriginal and Torres Strait Islander people as the Traditional Custodians of the land and acknowledges and pays respect to their Elders, past and present."],
      ],
      'field_site_show_exit_site' => [
        'value' => TideDemoHelper::randomBool(),
      ],
    ]);

    try {
      $term_site->save();
      $terms[] = $term_site;
      $repository->addDemoEntity($term_site, 'site', 'site', FALSE);

      $repository->addDemoEntity($menus[$site_menus[$site]['main']], 'site_menu', 'main:site:' . $term_site->id(), FALSE);
      $repository->addDemoEntity($menus[$site_menus[$site]['footer']], 'site_menu', 'footer:site:' . $term_site->id(), FALSE);

      foreach ($sections as $section) {
        $term_section = Term::create([
          'name' => $section,
          'vid' => 'sites',
          'parent' => [$term_site->id()],
          'field_site_domains' => [
            ['value' => $site],
          ],
        ]);
        try {
          $term_section->save();
          $terms[] = $term_section;
          $repository->addDemoEntity($term_section, 'site_section', 'site:' . $term_site->id(), FALSE);
        }
        catch (Exception $exception) {
          watchdog_exception('tide_demo', $exception);
        }
      }
    }
    catch (Exception $exception) {
      watchdog_exception('tide_demo', $exception);
    }

    $weight++;
  }

  return $terms;
}
