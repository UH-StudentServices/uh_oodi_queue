<?php
/**
 * @file
 * Drush integration for "University of Helsinki, Oodi Queue" module.
 *
 * @license GPL, or GNU General Public License, version 3
 * @license http://opensource.org/licenses/GPL-3.0
 * @see README.md how to contribute to this project
 */

/**
 * Implements hook_drush_command().
 */
function uh_oodi_queue_drush_command() {
  $items = array();

  $items['uh-oodi-queue-add-items'] = array(
    'callback' => 'uh_oodi_queue_drush_add_items',
    'drupal dependencies' => array('uh_oodi_queue'),
    'description' => 'Adds items to queue',
    'required-arguments' => TRUE,
    'arguments' => array(
      'action' => 'Action that is performed to the content. Example: update',
      'entity-type' => 'Type of entities that given action is performed. Example: courseunitrealisation',
      'entity-ids' => 'The IDs of the content that can contain letters too. Example: 123456789',
    ),
    'options' => array(
      'priority' => 'One of the priorities: hi, me, lo. If ommitted, then default will be used.',
    ),
    'examples' => array(
      'drush uoq-add update courseunitrealisation 822' => 'Adds an update action for courseunitrealisation id 822',
      'drush uoq-add update courseunitrealisation 123,456' => 'Adds an update action for courseunitrealisation ids 123 and 456',
      'drush uoq-add delete learningopportunity A12345' => 'Adds an delete action for learningopportunity id A12345',
      'drush uoq-add update learningopportunity A12345 --priority=hi' => 'Adds an update action for learningopportunity id A12345 to high priority',
    ),
    'aliases' => array('uoq-add'),
  );

  return $items;
}

/**
 * Callback function for adding items to given priority queue.
 * @param string $action Action.
 * @param string $entity_type Entity type.
 * @param string $entity_ids One or more comma-separated entity IDs.
 */
function uh_oodi_queue_drush_add_items($action, $entity_type, $entity_ids) {
  $priority = drush_get_option('priority', variable_get('uh_oodi_queue_default_queue', UHOodiQueue::PRIORITY_MEDIUM));
  try {
    $queue = new UHOodiQueue($priority);
    $entity_ids = _uh_oodi_queue_drush_parse_entity_ids($entity_ids);
    foreach ($entity_ids as $id) {
      $item = array(
        'action' => $action,
        'entityType' => $entity_type,
        'entityId' => $id,
      );
      // Call validate already now, so we can pass the exception messages to drush
      $queue->validateItem($item);
      $queue->createItem($item);
    }
  } catch (\LogicException $e) {
    drush_set_error($e->getMessage());
  }
}

/**
 * @param string $entity_ids One or more comma-separated entity IDs.
 * @return array Entity IDs.
 */
function _uh_oodi_queue_drush_parse_entity_ids($entity_ids) {
  $entity_ids = explode(',', $entity_ids);
  return array_map('trim', $entity_ids);
}
