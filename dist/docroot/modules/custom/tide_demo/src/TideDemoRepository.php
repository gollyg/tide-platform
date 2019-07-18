<?php

namespace Drupal\tide_demo;

use Drupal\Core\Database\Database;
use Drupal\Core\Entity\EntityInterface;

/**
 * Class TideDemoRepository.
 *
 * @package Drupal\tide_demo
 */
class TideDemoRepository {

  const SITE_ID_VICGOVAU = 4;

  /**
   * The repository singleton.
   *
   * @var \Drupal\tide_demo\TideDemoRepository
   */
  protected static $repository = NULL;

  /**
   * The entities.
   *
   * @var array
   */
  protected $entities = [];

  /**
   * TideDemoRepository constructor.
   */
  protected function __construct() {
  }

  /**
   * Get the repository instance.
   *
   * @return \Drupal\tide_demo\TideDemoRepository
   *   The repository.
   */
  public static function getInstance() {
    if (!self::$repository) {
      self::$repository = new self();
    }

    return self::$repository;
  }

  /**
   * Add a demo entity to the repository.
   *
   * @param \Drupal\Core\Entity\EntityInterface $entity
   *   The entity.
   * @param string $entity_type_id
   *   Override entity type with a custom value.
   * @param string $bundle
   *   Override bundle with a custom value.
   * @param bool $tracking
   *   Whether to track the entities.
   */
  public function addDemoEntity(EntityInterface $entity, $entity_type_id = NULL, $bundle = NULL, $tracking = TRUE) {
    $entity_type_id = $entity_type_id ?: $entity->getEntityTypeId();
    $bundle = $bundle ?: $entity->bundle();

    $this->entities[$entity_type_id][$bundle][$entity->id()] = $entity;
    if ($tracking) {
      $this->trackEntity($entity);
    }
  }

  /**
   * Add multiple demo entities to the repository.
   *
   * @param array $entities
   *   The array of entities.
   * @param bool $tracking
   *   Whether to track the entities.
   */
  public function addDemoEntities(array $entities, $tracking = TRUE) {
    /** @var \Drupal\Core\Entity\EntityInterface $entity */
    foreach ($entities as $entity) {
      $this->addDemoEntity($entity, NULL, NULL, $tracking);
    }
  }

  /**
   * Retrieve demo entities created in the same session.
   *
   * @param string $entity_type_id
   *   Entity type ID, eg. node or taxonomy_term.
   * @param string $bundle
   *   Bundle, eg. sites, page, lading_page.
   *
   * @return array
   *   The list of entities.
   */
  public function getDemoEntities($entity_type_id = NULL, $bundle = NULL) {
    if ($entity_type_id) {
      if (isset($this->entities[$entity_type_id])) {
        if ($bundle) {
          if (isset($this->entities[$entity_type_id][$bundle])) {
            return $this->entities[$entity_type_id][$bundle];
          }
          return [];
        }
        return $this->entities[$entity_type_id];
      }
      return [];
    }

    return $this->entities;
  }

  /**
   * Track the entity permanently in the demo table.
   *
   * @param \Drupal\Core\Entity\EntityInterface $entity
   *   The entity.
   */
  public function trackEntity(EntityInterface $entity) {
    try {
      $data = [
        'entity_type' => $entity->getEntityTypeId(),
        'bundle' => $entity->bundle(),
        'entity_id' => $entity->id(),
      ];
      Database::getConnection()->merge('tide_demo')
        ->keys($data)
        ->updateFields($data)
        ->execute();
    }
    catch (\Exception $exception) {
      watchdog_exception('tide_demo', $exception);
    }
  }

  /**
   * Remove all tracked entities.
   */
  public function removeTrackedEntities() {
    try {
      if (!Database::getConnection()->schema()->tableExists('tide_demo')) {
        return;
      }

      $query = Database::getConnection()->select('tide_demo', 'demo')
        ->fields('demo')
        ->execute();

      $results = $query->fetchAll(\PDO::FETCH_ASSOC);
      foreach ($results as $result) {
        try {
          $entity = \Drupal::entityTypeManager()->getStorage($result['entity_type'])
            ->load($result['entity_id']);
          if ($entity) {
            $entity->delete();
          }
        }
        catch (\Exception $exception) {
          watchdog_exception('tide_demo', $exception);
        }
      }
    }
    catch (\Exception $exception) {
      watchdog_exception('tide_demo', $exception);
    }
  }

}
