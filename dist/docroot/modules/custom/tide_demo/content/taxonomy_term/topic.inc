<?php

/**
 * @file
 * Load demo Topic terms.
 */

/**
 * Implements hook_tide_demo_create_ENTITY_TYPE_BUNDLE().
 */
function tide_demo_create_taxonomy_term_topic() {
  $topics = [];
  try {
    $storage = \Drupal::entityTypeManager()->getStorage('taxonomy_term');
    $topics = $storage->loadByProperties(['vid' => 'topic']);
  }
  catch (Exception $exception) {
    watchdog_exception('tide_demo', $exception);
  }

  return $topics;
}

/**
 * Implements hook_tide_demo_create_ENTITY_TYPE_BUNDLE_tracking().
 */
function tide_demo_create_taxonomy_term_topic_tracking() {
  return FALSE;
}
