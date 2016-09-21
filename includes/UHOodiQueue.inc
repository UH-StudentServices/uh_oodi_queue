<?php
/**
 * @file
 * Implementation for "University of Helsinki, Oodi Queue" DrupalReliable.
 *
 * @license GPL, or GNU General Public License, version 3
 * @license http://opensource.org/licenses/GPL-3.0
 * @see README.md how to contribute to this project
 */

class UHOodiQueue extends SystemQueue implements DrupalReliableQueueInterface {

  const PRIORITY_HIGH = 'hi';
  const PRIORITY_MEDIUM = 'me';
  const PRIORITY_LOW = 'lo';

  protected $priority = self::PRIORITY_MEDIUM;

  protected $allowed_actions = array('delete', 'update');
  protected $allowed_entity_types = array('learningopportunity', 'courseunitrealisation');

  public function __construct($priority = NULL) {
    if (is_null($priority)) {
      $priority = variable_get('uh_oodi_queue_default_queue', self::PRIORITY_MEDIUM);
    }
    // Validate priority
    if ($priority !== self::PRIORITY_HIGH &&
      $priority !== self::PRIORITY_MEDIUM &&
      $priority !== self::PRIORITY_LOW) {
      throw new InvalidArgumentException('Invalid priority given');
    }

    // Based on priority, we can deduce the queue name
    $name = variable_get('uh_oodi_queue_name_' . $priority, 'uh_oodi_queue_' . $priority);

    return parent::__construct($name);
  }

  /**
   * Extends SystemQueue::createItem() by adds validation. Returns FALSE if
   * validation fails (and leaves log entry).
   * @param $data
   * @return bool
   */
  public function createItem($data) {
    try {
      if ($this->validateItem($data)) {
        return parent::createItem($data);
      }
    } catch(\LogicException $e) {
      watchdog('uh_oodi_queue', 'Failed to create item to queue due to data validation, reason: @reason.', array('@reason' => $e->getMessage()), WATCHDOG_ERROR);
    }
    return FALSE;
  }

  /**
   * Validates item data. Throws Exception if validation errors occurs.
   * @param array $data
   * @return bool
   * @throws LogicException
   */
  public function validateItem(array $data) {
    // Ensure that we have action defined
    if (empty($data['action'])) {
      throw new \LogicException('Undefined action');
    }
    // Ensure that we have entity type defined
    if (empty($data['entityType'])) {
      throw new \LogicException('Undefined entityType');
    }
    // Ensure that we have entity id defined
    if (empty($data['entityId'])) {
      throw new \LogicException('Undefined entityId');
    }
    // Ensure we have certain allowed actions
    if (!in_array($data['action'], $this->allowed_actions)) {
      throw new \LogicException('Not allowed action "' . $data['action'] . '"');
    }
    // Ensure we have certain allowed entity types
    if (!in_array($data['entityType'], $this->allowed_entity_types)) {
      throw new \LogicException('Not allowed entity type "' . $data['entityType'] . '"');
    }
    return TRUE;
  }

}
