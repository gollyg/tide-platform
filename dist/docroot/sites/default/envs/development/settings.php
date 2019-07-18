<?php

/**
 * @file
 * Settings file for development environment.
 */

$settings['environment'] = 'development';

// Shield config.
$config['shield.settings']['credentials']['shield']['user'] = 'dpc';
$config['shield.settings']['credentials']['shield']['pass'] = 'sdp';

$config['config_split.config_split.dev']['status'] = TRUE;

// Environment indicator color override.
$config['environment_indicator.indicator']['name'] = 'DEV';

// Disable index updates on insert/update.
$config['search_api.index.node']['options']['index_directly'] = FALSE;
