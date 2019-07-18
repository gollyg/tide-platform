<?php

/**
 * @file
 * Settings file for local environment.
 */

// Show all error messages on the site
$config['system.logging']['error_level'] = 'all';

// Disable Google Analytics from sending dev GA data.
$config['google_analytics.settings']['account'] = 'UA-XXXXXXXX-YY';

// Expiration of cached pages to 0
$config['system.performance']['cache']['page']['max_age'] = 0;

// Aggregate CSS files off
$config['system.performance']['css']['preprocess'] = 0;

// Aggregate JavaScript files off
$config['system.performance']['js']['preprocess'] = 0;

// Disable Shield.
$config['shield.settings']['credentials']['shield']['user'] = '';
$config['shield.settings']['credentials']['shield']['pass'] = '';

// Enable local split.
$config['config_split.config_split.local']['status'] = TRUE;

// Environment indicator color override.
$config['environment_indicator.indicator']['name'] = 'LOCAL';

// Skip permissions hardening.
$settings['skip_permissions_hardening'] = TRUE;

$settings['container_yamls'][] = DRUPAL_ROOT . '/sites/default/envs/local/services.local.yml';
$settings['cache']['bins']['render'] = 'cache.backend.null';
$settings['cache']['bins']['dynamic_page_cache'] = 'cache.backend.null';

// Disable index updates on insert/update.
$config['search_api.index.node']['options']['index_directly'] = FALSE;
