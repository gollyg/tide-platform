<?php

/**
 * @file
 * Create demo images.
 */

use Drupal\file\Entity\File;
use Drupal\media\Entity\Media;
use Drupal\tide_demo\TideDemoHelper;
use Drupal\tide_demo\TideDemoRepository;

/**
 * Implements hook_tide_demo_create_ENTITY_TYPE_BUNDLE().
 */
function tide_demo_create_media_image() {
  $media = [];

  $repository = TideDemoRepository::getInstance();

  $destination = 'public://tide_demo/images';
  file_prepare_directory($destination, FILE_CREATE_DIRECTORY | FILE_MODIFY_PERMISSIONS);

  $directory = drupal_get_path('module', 'tide_demo') . '/content/media/images';
  $files = file_scan_directory($directory, '/.*\.jpg$/i');
  foreach ($files as $file) {
    try {
      $site_section = TideDemoHelper::randomSiteSections();

      $source_file = File::create([
        'uri' => $file->uri,
        'filename' => $file->filename,
        'uid' => TideDemoHelper::randomUid(),
      ]);
      $image_file = file_copy($source_file, 'public://tide_demo/images', FILE_EXISTS_REPLACE);
      if ($image_file) {
        $image_file->setPermanent();
        $image_file->save();
        $repository->addDemoEntity($image_file, 'file', 'file');

        $image = Media::create([
          'bundle' => 'image',
          'uid' => $image_file->get('uid'),
          'status' => 1,
          'name' => 'Demo: ' . $file->name,
          'field_media_image' => [
            [
              'target_id' => $image_file->id(),
              'title' => $file->name,
              'alt' => $file->name,
            ],
          ],
          'field_media_license' => [['value' => 'cc4']],
          'field_media_restricted' => [['value' => TideDemoHelper::randomBool()]],
          'field_media_site' => TideDemoHelper::getFieldValueForSiteSections($site_section),
        ]);
        $image->save();
        $media[] = $image;
      }
    }
    catch (Exception $exception) {
      watchdog_exception('tide_demo', $exception);
    }
  }

  return $media;
}
