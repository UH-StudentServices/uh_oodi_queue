# University of Helsinki, Oodi Queue

This project synchronises tracked changes made in Oodi by reading them into
queue. This way other Drupal modules can read the queue and handle them.

## Getting started

The main module itself is the framework around queue prioritation. It allows you
to read the queue according to priorities.

To populate content you may enable **UH Oodi Queue Poll** sub module and
configure it from /admin/config/services/doo-oodi. This module relies on
"doo-oodi" service.

You will need to contact tike-integraatiopalvelut@helsinki.fi for getting access
to the endpoint. After that you'll be able to configure endpoint URLs.

## Example usage

```php

// Moves item from default queue to hight priority queue
$default_queue = UHOodiQueue();
$hi_queue = UHOodiQueue(UHOodiQueue::PRIORITY_HIGH);
$item = $default_queue->claimItem();
if ($item !== FALSE) {
  $hi_queue->createItem($item);
}

// Creates low priority item, then high priority item, then prints their IDs
// in right priority order.
$priority_queue = new UHOodiPrioritisedQueue();
$priority_queue->createItem(array('action' => 'update', 'entityType' => 'learningopportunity', 'entityId' => 123), UHOodiQueue::PRIORITY_LOW);
$priority_queue->createItem(array('action' => 'update', 'entityType' => 'learningopportunity', 'entityId' => 321), UHOodiQueue::PRIORITY_HIGH);
while ($item = $priority_queue->claimItem()) {

  // Prints:
  //   321
  //   123
  print $item->getId();

  // Deletes item from queue if priority is low, or else it releases the item
  if ($item->getPriority() == UHOodiQueue::PRIORITY_LOW) {
    $priority_queue->deleteItem($item);
  }
  else {
    $priority_queue->releaseItem($item);
  }

  // Print "opportunity" if type matches with "learningopportunity"
  if ($item->isType('learningopportunity')) {
    print "opportunity";
  }

  // Print "opportunity deleted" if action "delete" matches
  if ($item->isAction('delete')) {
    print "opportunity deleted";
  }

}


```

```bash

# Adds an item to high priority queue
[you@machine]$ drush uoq-add update courseunitrealisation 12345 --priority=hi
 
```

## Questions
Please post your question to doo-projekti@helsinki.fi

## License
This software is developed under [GPL v3](LICENSE.txt).
