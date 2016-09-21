# University of Helsinki, Oodi Queue

This project synchronises tracked changes made in Oodi by reading them into
queue. This way other Drupal modules can read the queue and handle them.

This module relies on "doo-oodi" service. For gaining access you need to contact
tike-integraatiopalvelut@helsinki.fi.

## Example usage

```php

// Moves item from default queue to hight priority queue
$default_queue = UHOodiQueue();
$hi_queue = UHOodiQueue(UHOodiQueue::PRIORITY_HIGH);
$item = $default_queue->claimItem();
if ($item !== FALSE) {
  $hi_queue->createItem($item);
}


```

## Questions
Please post your question to doo-projekti@helsinki.fi

## License
This software is developed under [GPL v3](LICENSE.txt).
