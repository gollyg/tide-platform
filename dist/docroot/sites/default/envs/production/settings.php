<?php

/**
 * @file
 * Settings file for production environment.
 */

$settings['environment'] = 'production';

// Don't show any error messages on the site (will still be shown in watchdog).
$config['system.logging']['error_level'] = 'hide';

// Expiration of cached pages on Varnish to 15 min
$config['system.performance']['cache']['page']['max_age'] = 900;

// Aggregate CSS files on
$config['system.performance']['css']['preprocess'] = 1;

// Aggregate JavaScript files on
$config['system.performance']['js']['preprocess'] = 1;

// Disabling stage file proxy on production.
$config['stage_file_proxy.settings']['origin'] = FALSE;

// Disable shield config.
$config['shield.settings']['credentials']['shield']['user'] = '';
$config['shield.settings']['credentials']['shield']['pass'] = '';

// Environment indicator color override.
$config['environment_indicator.indicator']['bg_color'] = 'red';
$config['environment_indicator.indicator']['name'] = 'PROD';
