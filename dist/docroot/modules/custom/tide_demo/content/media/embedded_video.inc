<?php

/**
 * @file
 * Create demo embedded videos.
 */

use Drupal\media\Entity\Media;
use Drupal\tide_demo\TideDemoHelper;

/**
 * Implements hook_tide_demo_create_ENTITY_TYPE_BUNDLE().
 */
function tide_demo_create_media_embedded_video() {
  $media = [];

  $videos = [
    'https://www.youtube.com/watch?v=pZRrtGYx7EM' => 'Wodonga Local Aboriginal Network - Bringing People Together',
    'https://www.youtube.com/watch?v=bSlnfyGTiss' => 'Getting Things Done in the Loddon Campaspe Region',
    'https://www.youtube.com/watch?v=yHHr3r_1kr0' => 'Getting Things Done | Victorian Budget 2018/19',
    'https://www.youtube.com/watch?v=Qvtu08Tea34' => 'Getting Things Done in the Northern Metro Region',
    'https://www.youtube.com/watch?v=jaC9bBPQIlg' => 'Getting Things Done in Gippsland',
    'https://www.youtube.com/watch?v=risb5PQDjLQ' => 'AW Bell: 2016 winner of the GOVEA for Manufacturing',
    'https://www.youtube.com/watch?v=xWuI0ZkdCPA' => 'Ego Pharmaceuticals: 2016 GOVEA Exporter of the year',
    'https://www.youtube.com/watch?v=w8NtVbUzA-8' => 'Victorian Public Healthcare Awards Judges',
    'https://www.youtube.com/watch?v=fNlQ4gsKrLU' => 'Victoria in Bloom 2018',
    'https://www.youtube.com/watch?v=AqqvbKSakHQ' => 'Putting Aboriginal communities in charge',
  ];

  foreach ($videos as $url => $title) {
    try {
      $site_section = TideDemoHelper::randomSiteSections();
      $video_data = [
        'bundle' => 'embedded_video',
        'uid' => TideDemoHelper::randomUid(),
        'status' => 1,
        'name' => $title,
        'field_media_video_embed_field' => [['value' => $url]],
        'field_media_license' => [['value' => 'cc4']],
        'field_media_restricted' => [['value' => TideDemoHelper::randomBool()]],
        'field_media_site' => TideDemoHelper::getFieldValueForSiteSections($site_section),
        'field_media_link' => [['value' => 'View transcript']],
      ];

      $video = Media::create($video_data);
      $video->save();
      $media[] = $video;
    }
    catch (Exception $exception) {
      watchdog_exception('tide_demo', $exception);
    }
  }

  return $media;
}
