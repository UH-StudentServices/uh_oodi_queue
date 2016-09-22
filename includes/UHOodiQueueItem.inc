<?php
/**
 * @file
 * Implementation for UHOodiQueueItem that represents an prioritised item in the
 * queue.
 *
 * @license GPL, or GNU General Public License, version 3
 * @license http://opensource.org/licenses/GPL-3.0
 * @see README.md how to contribute to this project
 */

class UHOodiQueueItem {

  protected $item;
  protected $priority;

  /**
   * @param $item
   * @param $priority
   */
  public function __construct($item, $priority) {
    $this->item = $item;
    $this->priority = $priority;
  }

  /**
   * @return mixed
   */
  public function getAction() {
    return $this->getData('action');
  }

  /**
   * @return mixed
   */
  public function getType() {
    return $this->getData('entityType');
  }

  /**
   * @return mixed
   */
  public function getId() {
    return $this->getData('entityId');
  }

  /**
   * @param $property
   * @return mixed
   */
  public function getData($property = NULL) {
    return is_null($property) ? $this->item->data : $this->item->data[$property];
  }

  /**
   * @return mixed
   */
  public function getPriority() {
    return $this->priority;
  }

  /**
   * @param $action
   * @return bool
   */
  public function isAction($action) {
    return $this->getAction() == $action;
  }

  /**
   * @param $type
   * @return bool
   */
  public function isType($type) {
    return $this->getType() == $type;
  }

  public function exportToQueueItem() {
    return $this->item;
  }

}
