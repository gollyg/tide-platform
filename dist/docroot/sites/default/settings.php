<?php

/**
 * @file
 * VICGOVAU Drupal 8 configuration file.
 */

// Environment settings. Each environment is expected to override this.
$settings['environment'] = 'local';

// Trusted Host Patterns.
// @see https://www.drupal.org/node/2410395 for more information.
$settings['trusted_host_patterns'] = [
  '^.+$',
];

/**
 * Subscribe to NotFoundHttpException event.
 *
 * The Fast404 Event subscriber can listen to the NotFoundHttpException event
 * to completely replace the Drupal 404 page.
 *
 * By default, Fast404 only listens to KernelRequest event. If a user hits a
 * valid path, but another module intervenes and returns a NotFoundHttpException
 * exception (eg. m4032404 module), the native Drupal 404 page is returned
 * instead of the Fast404 page.
 */
$settings['fast404_not_found_exception'] = TRUE;

// Shield config.
$config['shield.settings']['credentials']['shield']['user'] = 'dpc';
$config['shield.settings']['credentials']['shield']['pass'] = 'sdp';

// Disable local split.
$config['config_split.config_split.local']['status'] = FALSE;

// Stage file proxy
$config['stage_file_proxy.settings']['origin'] = 'https://%%DOMAIN%%';

// Installation profile.
$settings['install_profile'] = 'tide';

// Private file system.
$settings['file_private_path'] = 'sites/default/files/private';

$settings['elastic_search.site_hash'] = '%%SEARCH_HASH%%';

// Include the redis services yml to ensure that the site can install
// without the redis module being enabled.
if (file_exists(DRUPAL_ROOT . '/modules/contrib/redis/redis.services.yml')) {
  $settings['container_yamls'][] = DRUPAL_ROOT . '/modules/contrib/redis/redis.services.yml';
}

// Include Bay settings.
// Please note, that a lot of configuration is provided in Bay settings by
// default. To override this configuration, use per-environment settings files.
if (file_exists('/bay/settings.php')) {
  require '/bay/settings.php';
}

// Include local settings and services files.
// Those files should be located outside of `docroot` instead of `sites/default`
// because `ahoy rebuild` removes them.
if (file_exists(dirname(DRUPAL_ROOT) . '/local/settings.local.php')) {
  include dirname(DRUPAL_ROOT) . '/local/settings.local.php';
}
if (file_exists(dirname(DRUPAL_ROOT) . '/local/services.local.yml')) {
  $settings['container_yamls'][] = dirname(DRUPAL_ROOT) . '/local/services.local.yml';
}
