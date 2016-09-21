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
  $form['queues'] = array(
    '#type' => 'fieldset',
    '#title' => t('Queues'),
    '#description' => t('If queue has items, you wont be able to change the queue name until you have cleared the queue first.'),
    'uh_oodi_queue_name_hi' => array(
      '#type' => 'textfield',
      '#title' => t('High priority queue name'),
      '#required' => TRUE,
      '#default_value' => variable_get('uh_oodi_queue_name_hi', 'uh_oodi_queue_hi'),
      '#disabled' => _uh_oodi_queue_has_items(UHOodiQueue::PRIORITY_HIGH),
    ),
    'uh_oodi_queue_name_me' => array(
      '#type' => 'textfield',
      '#title' => t('Medium priority queue name'),
      '#required' => TRUE,
      '#default_value' => variable_get('uh_oodi_queue_name_me', 'uh_oodi_queue_me'),
      '#disabled' => _uh_oodi_queue_has_items(UHOodiQueue::PRIORITY_MEDIUM),
    ),
    'uh_oodi_queue_name_lo' => array(
      '#type' => 'textfield',
      '#title' => t('Low priority queue name'),
      '#required' => TRUE,
      '#default_value' => variable_get('uh_oodi_queue_name_lo', 'uh_oodi_queue_lo'),
      '#disabled' => _uh_oodi_queue_has_items(UHOodiQueue::PRIORITY_LOW),
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
 * @param $priority
 * @return null|bool
 */
function _uh_oodi_queue_has_items($priority) {
  /** @var UHOodiQueue $queue */
  $queue = new UHOodiQueue($priority);
  $item = $queue->claimItem();
  if ($item === FALSE) {
    return FALSE;
  }
  $queue->releaseItem($item);
  return TRUE;
}
