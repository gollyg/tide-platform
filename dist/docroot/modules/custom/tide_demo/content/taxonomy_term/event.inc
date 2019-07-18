<?php

/**
 * @file
 * Load demo Event terms.
 */

/**
 * Implements hook_tide_demo_create_ENTITY_TYPE_BUNDLE().
 */
function tide_demo_create_taxonomy_term_event() {
  $events = [];
  try {
    $storage = \Drupal::entityTypeManager()->getStorage('taxonomy_term');
    $events = $storage->loadByProperties(['vid' => 'event']);
  }
  catch (Exception $exception) {
    watchdog_exception('tide_demo', $exception);
  }

  return $events;
}

/**
 * Implements hook_tide_demo_create_ENTITY_TYPE_BUNDLE_tracking().
 */
function tide_demo_create_taxonomy_term_event_tracking() {
  return FALSE;
}
