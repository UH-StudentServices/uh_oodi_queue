<?php
/**
 * @file
 * Implementation for class that provides constructed prioritised items from
 * queues.
 *
 * @license GPL, or GNU General Public License, version 3
 * @license http://opensource.org/licenses/GPL-3.0
 * @see README.md how to contribute to this project
 */

class UHOodiPrioritisedQueue implements DrupalQueueInterface {

  /**
   * @var UHOodiQueue|null
   */
  protected $high_queue = NULL;

  /**
   * @var UHOodiQueue|null
   */
  protected $medium_queue = NULL;

  /**
   * @var UHOodiQueue|null
   */
  protected $low_queue = NULL;

  public function __construct() {
    $this->high_queue = new UHOodiQueue(UHOodiQueue::PRIORITY_HIGH);
    $this->medium_queue = new UHOodiQueue(UHOodiQueue::PRIORITY_MEDIUM);
    $this->low_queue = new UHOodiQueue(UHOodiQueue::PRIORITY_LOW);
  }

  /**
   * Create item with regular $data item. It requires the priority as second
   * parameter which is set to medium by default.
   * @param $data
   * @param string $priority
   * @return bool
   */
  public function createItem($data, $priority = UHOodiQueue::PRIORITY_MEDIUM) {
    switch ($priority) {
      case UHOodiQueue::PRIORITY_HIGH:
        return $this->high_queue->createItem($data);
        break;
      case UHOodiQueue::PRIORITY_MEDIUM:
        return $this->medium_queue->createItem($data);
        break;
      case UHOodiQueue::PRIORITY_LOW:
        return $this->low_queue->createItem($data);
        break;
    }
    return FALSE;
  }

  /**
   * @param int $lease_time
   * @return UHOodiQueueItem
   */
  public function claimItem($lease_time = 3600) {
    // Claims item in priority order and returns in as UHOodiQueueItem
    foreach (array($this->high_queue, $this->medium_queue, $this->low_queue) as $queue) {
      /** @var UHOodiQueue $queue */
      while ($item = $queue->claimItem($lease_time)) {
        return new UHOodiQueueItem($item, $queue->getPriority());
      }
    }
    return FALSE;
  }

  /**
   * Returns total number of items in all priority queues.
   * @return int
   */
  public function numberOfItems() {
    return (int) $this->high_queue->numberOfItems() + (int) $this->medium_queue->numberOfItems() + (int) $this->low_queue->numberOfItems();
  }

  /**
   * @param UHOodiQueueItem $item
   */
  public function deleteItem(UHOodiQueueItem $item) {
    switch ($item->getPriority()) {
      case UHOodiQueue::PRIORITY_HIGH:
        $this->high_queue->deleteItem($item->exportToQueueItem());
        break;
      case UHOodiQueue::PRIORITY_MEDIUM:
        $this->medium_queue->deleteItem($item->exportToQueueItem());
        break;
      case UHOodiQueue::PRIORITY_LOW:
        $this->low_queue->deleteItem($item->exportToQueueItem());
        break;
    }
  }

  /**
   * @param UHOodiQueueItem $item
   * @return bool|DatabaseStatementInterface
   */
  public function releaseItem(UHOodiQueueItem $item) {
    switch ($item->getPriority()) {
      case UHOodiQueue::PRIORITY_HIGH:
        return $this->high_queue->releaseItem($item->exportToQueueItem());
        break;
      case UHOodiQueue::PRIORITY_MEDIUM:
        return $this->medium_queue->releaseItem($item->exportToQueueItem());
        break;
      case UHOodiQueue::PRIORITY_LOW:
        return $this->low_queue->releaseItem($item->exportToQueueItem());
        break;
    }
    return FALSE;
  }

  /**
   * Creates queues for all priorities. At the time of implementation we already
   * know they don't do anything so far, but in case they would, we call that
   * for all priorities.
   */
  public function createQueue() {
    $this->high_queue->createQueue();
    $this->medium_queue->createQueue();
    $this->low_queue->createQueue();
  }

  /**
   * Deletes all priority queues.
   */
  public function deleteQueue() {
    $this->high_queue->deleteQueue();
    $this->medium_queue->deleteQueue();
    $this->low_queue->deleteQueue();
  }

}
