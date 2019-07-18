<?php

/**
 * @file
 * Create demo documents.
 */

use Drupal\file\Entity\File;
use Drupal\media\Entity\Media;
use Drupal\tide_demo\TideDemoHelper;
use Drupal\tide_demo\TideDemoRepository;

/**
 * Implements hook_tide_demo_create_ENTITY_TYPE_BUNDLE().
 */
function tide_demo_create_media_document() {
  $media = [];

  $repository = TideDemoRepository::getInstance();

  $destination = 'public://tide_demo/documents';
  file_prepare_directory($destination, FILE_CREATE_DIRECTORY | FILE_MODIFY_PERMISSIONS);

  $directory = drupal_get_path('module', 'tide_demo') . '/content/media/documents';
  $source_file = File::create([
    'uri' => $directory . '/sample.docx',
    'filename' => 'sample.docx',
    'uid' => TideDemoHelper::randomUid(),
  ]);

  for ($i = 1; $i <= 10; $i++) {
    try {
      $site_section = TideDemoHelper::randomSiteSections();

      $destination_file = 'public://tide_demo/documents/demo_' . $i . '_sample.docx';
      $document_file = file_copy($source_file, $destination_file, FILE_EXISTS_REPLACE);
      if ($document_file) {
        $document_file->setPermanent();
        $document_file->save();
        $repository->addDemoEntity($document_file, 'file', 'file');

        $document = Media::create([
          'bundle' => 'document',
          'uid' => $document_file->get('uid'),
          'status' => 1,
          'name' => 'Demo: ' . TideDemoHelper::randomSentence(2, 5),
          'field_media_file' => [
            [
              'target_id' => $document_file->id(),
            ],
          ],
          'field_media_license' => [['value' => 'cc4']],
          'field_media_restricted' => [['value' => TideDemoHelper::randomBool()]],
          'field_media_site' => TideDemoHelper::getFieldValueForSiteSections($site_section),
        ]);
        $document->save();
        $media[] = $document;
      }
    }
    catch (Exception $exception) {
      watchdog_exception('tide_demo', $exception);
    }
  }

  return $media;
}
