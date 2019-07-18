<?php

/**
 * @file
 * Settings file for stage environment.
 */

$settings['environment'] = 'stage';

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

// Shield config.
$config['shield.settings']['credentials']['shield']['user'] = 'dpc';
$config['shield.settings']['credentials']['shield']['pass'] = 'sdp';

$config['config_split.config_split.stage']['status'] = TRUE;

// Environment indicator color override.
$config['environment_indicator.indicator']['name'] = 'STAGE';

// Disable index updates on insert/update.
$config['search_api.index.node']['options']['index_directly'] = FALSE;
