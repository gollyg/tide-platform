<?php

/**
 * @file
 * Settings file for CI environment.
 */

$settings['environment'] = 'ci';

// Disable shield config.
$config['shield.settings']['credentials']['shield']['user'] = '';
$config['shield.settings']['credentials']['shield']['pass'] = '';

$config['config_split.config_split.ci']['status'] = TRUE;
