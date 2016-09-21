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


```

```bash

# Adds an item to high priority queue
[you@machine]$ drush uoq-add update courseunitrealisation 12345 --priority=hi
 
```

## Questions
Please post your question to doo-projekti@helsinki.fi

## License
This software is developed under [GPL v3](LICENSE.txt).
