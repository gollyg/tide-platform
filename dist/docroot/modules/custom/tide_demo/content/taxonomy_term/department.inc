<?php

/**
 * @file
 * Load demo Event terms.
 */

/**
 * Implements hook_tide_demo_create_ENTITY_TYPE_BUNDLE().
 */
function tide_demo_create_taxonomy_term_department() {
  $departments = [];
  try {
    $storage = \Drupal::entityTypeManager()->getStorage('taxonomy_term');
    $departments = $storage->loadByProperties(['vid' => 'department']);
  }
  catch (Exception $exception) {
    watchdog_exception('tide_demo', $exception);
  }

  return $departments;
}

/**
 * Implements hook_tide_demo_create_ENTITY_TYPE_BUNDLE_tracking().
 */
function tide_demo_create_taxonomy_term_department_tracking() {
  return FALSE;
}
