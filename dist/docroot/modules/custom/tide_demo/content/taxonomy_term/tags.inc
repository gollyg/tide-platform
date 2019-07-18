<?php

/**
 * @file
 * Load demo Tags terms.
 */

/**
 * Implements hook_tide_demo_create_ENTITY_TYPE_BUNDLE().
 */
function tide_demo_create_taxonomy_term_tags() {
  $tags = [];
  try {
    $storage = \Drupal::entityTypeManager()->getStorage('taxonomy_term');
    $tags = $storage->loadByProperties(['vid' => 'tags']);
  }
  catch (Exception $exception) {
    watchdog_exception('tide_demo', $exception);
  }

  return $tags;
}

/**
 * Implements hook_tide_demo_create_ENTITY_TYPE_BUNDLE_tracking().
 */
function tide_demo_create_taxonomy_term_tags_tracking() {
  return FALSE;
}
