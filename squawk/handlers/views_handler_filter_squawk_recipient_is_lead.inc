<?php

/**
 * @file
 *   Filters to statuses where a given user is a participant.
 */

/**
 * Filter handler to select statuses where a given user is a participant.
 */
class views_handler_filter_squawk_recipient_is_lead extends views_handler_filter {
  function value_form(&$form, &$form_state) {
    $form['value'] = array(
      '#type' => 'textfield',
      '#title' => t('recipient user ID'),
      '#default_value' => isset($this->value) ? $this->value : '',
      '#required' => TRUE,
    );
  }
  function value_validate($form, &$form_state) {
    if (!is_numeric($form_state['values']['options']['value'])) {
      form_set_error('options][value', t('The specified user ID must be numeric.'));
    }
  }

}
