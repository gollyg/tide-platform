<?php

/**
 * @file
 * Create demo Image Gallery blocks.
 */

use Drupal\block_content\Entity\BlockContent;
use Drupal\tide_demo\TideDemoHelper;

/**
 * Implements hook_tide_demo_create_ENTITY_TYPE_BUNDLE().
 */
function tide_demo_create_block_content_media_gallery() {
  $galleries = [];

  for ($i = 1; $i <= 10; $i++) {
    try {
      $block_data = [
        'type' => 'media_gallery',
        'info' => 'Gallery: ' . TideDemoHelper::randomSentence(3, 5),
        'field_topic' => [
          ['target_id' => TideDemoHelper::randomTopic()],
        ],
        'field_gallery_featured_image' => [
          ['target_id' => TideDemoHelper::randomImage()],
        ],
      ];

      // Add tags.
      $tags = TideDemoHelper::randomTags(3);
      if (!empty($tags)) {
        foreach ($tags as $tag_id => $tag) {
          $block_data['field_tags'][] = ['target_id' => $tag_id];
        }
      }

      // Add up to 10 images.
      for ($j = 1; $j <= mt_rand(1, 10); $j++) {
        $block_data['field_gallery_media'][] = ['target_id' => TideDemoHelper::randomImage()];
      }

      $gallery = BlockContent::create($block_data);
      $gallery->save();
      $galleries[] = $gallery;
    }
    catch (Exception $exception) {
      watchdog_exception('tide_demo', $exception);
    }
  }

  return $galleries;
}
