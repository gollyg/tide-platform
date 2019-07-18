<?php

/**
 * @file
 * Create demo alert.
 */

use Drupal\node\Entity\Node;
use Drupal\tide_demo\TideDemoHelper;

/**
 * Implements hook_tide_demo_create_ENTITY_TYPE_BUNDLE_weight().
 */
function tide_demo_create_node_alert_weight() {
  return 11;
}

/**
 * Implements hook_tide_demo_create_ENTITY_TYPE_BUNDLE().
 */
function tide_demo_create_node_alert() {
  $nodes = [];

  for ($i = 1; $i <= 10; $i++) {
    $uid = TideDemoHelper::randomUid();

    $site_sections = TideDemoHelper::randomSiteSections();
    $primary_site_id = TideDemoHelper::randomSite($site_sections);

    $status = Node::PUBLISHED;
    $node_data = [
      'type' => 'alert',
      'title' => TideDemoHelper::randomSentence(),
      'uid' => $uid,
      'status' => $status,
      'field_node_site' => TideDemoHelper::getFieldValueForSiteSections($site_sections),
      'field_node_primary_site' => [
        ['target_id' => $primary_site_id],
      ],
      'field_alert_type' => [
        ['target_id' => TideDemoHelper::randomAlertType()],
      ],
      ['field_call_to_action'] => [
        [],
      ],
    ];

    $node = Node::create($node_data);

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
