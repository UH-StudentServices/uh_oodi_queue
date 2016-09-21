<?php
/**
 * @file
 * Implementation for "University of Helsinki, Oodi Queue" module´s
 * administration.
 *
 * @license GPL, or GNU General Public License, version 3
 * @license http://opensource.org/licenses/GPL-3.0
 * @see README.md how to contribute to this project
 */

/**
 * Form callback for configuring queues.
 */
function uh_oodi_queue_settings() {
  $form = array();
  $uh_oodi_queue_name_hi = variable_get('uh_oodi_queue_name_hi', 'uh_oodi_queue_hi');
  $uh_oodi_queue_name_me = variable_get('uh_oodi_queue_name_me', 'uh_oodi_queue_me');
  $uh_oodi_queue_name_lo = variable_get('uh_oodi_queue_name_lo', 'uh_oodi_queue_lo');
  $form['queues'] = array(
    '#type' => 'fieldset',
    '#title' => t('Queues'),
    '#description' => t('If queue has items, you wont be able to change the queue name until you have cleared the queue first.'),
    'uh_oodi_queue_name_hi' => array(
      '#type' => 'textfield',
      '#title' => t('High priority queue name'),
      '#required' => TRUE,
      '#default_value' => $uh_oodi_queue_name_hi,
      '#disabled' => _uh_oodi_queue_has_items($uh_oodi_queue_name_hi),
    ),
    'uh_oodi_queue_name_me' => array(
      '#type' => 'textfield',
      '#title' => t('Medium priority queue name'),
      '#required' => TRUE,
      '#default_value' => $uh_oodi_queue_name_me,
      '#disabled' => _uh_oodi_queue_has_items($uh_oodi_queue_name_me),
    ),
    'uh_oodi_queue_name_lo' => array(
      '#type' => 'textfield',
      '#title' => t('Low priority queue name'),
      '#required' => TRUE,
      '#default_value' => $uh_oodi_queue_name_lo,
      '#disabled' => _uh_oodi_queue_has_items($uh_oodi_queue_name_lo),
    ),
    'uh_oodi_queue_default_queue' => array(
      '#type' => 'select',
      '#title' => t('Default priority'),
      '#required' => TRUE,
      '#options' => array(
        'hi' => t('High'),
        'me' => t('Medium'),
        'lo' => t('Low'),
      ),
      '#default_value' => variable_get('uh_oodi_queue_default_queue', 'me'),
    ),
  );
  return system_settings_form($form);
}

/**
 * @param string $queue_name
 * @return null|bool
 */
function _uh_oodi_queue_has_items($queue_name) {
  if (!is_string($queue_name)) {
    return NULL;
  }
  /** @var DrupalReliableQueueInterface $queue */
  $queue = DrupalQueue::get($queue_name, TRUE);
  $item = $queue->claimItem();
  if ($item === FALSE) {
    return FALSE;
  }
  $queue->releaseItem($item);
  return TRUE;
}
