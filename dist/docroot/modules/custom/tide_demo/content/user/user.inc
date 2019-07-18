<?php

/**
 * @file
 * Create demo users.
 */

use Drupal\user\Entity\User;
use Drupal\user\Entity\Role;

/**
 * Implements hook_tide_demo_create_ENTITY_TYPE_BUNDLE_weight().
 */
function tide_demo_create_user_user_weight() {
  return -100;
}

/**
 * Implements hook_tide_demo_create_ENTITY_TYPE_BUNDLE().
 */
function tide_demo_create_user_user() {
  $roles = [
    'administrator',
    'site_admin',
    'approver',
    'editor',
    'previewer',
  ];

  $users = [];

  foreach ($roles as $role) {
    for ($i = 1; $i <= 2; $i++) {
      $name = $role . $i . '.test@example.com';

      $existing_user = user_load_by_name($name);
      if ($existing_user) {
        user_delete($existing_user->id());
      }
      if (Role::load($role)) {
        $user = User::create();
        $user->setUsername($name);
        $user->setPassword('DpcFakePass');
        $user->setEmail($name);
        $user->addRole($role);
        $user->activate();
        $user->enforceIsNew();
        try {
          $user->save();
          $users[] = $user;
        }
        catch (Exception $exception) {
          watchdog_exception('tide_demo', $exception);
        }
      }
    }
  }

  return $users;
}
