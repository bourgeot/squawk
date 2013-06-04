<?php

/**
 * @file
 *   Views handler for a "squawk_up" link.
 */

/**
 * Formats the link.
 */
class squawk_views_handler_field_squawk_up extends views_handler_field {
  function option_definition() {
    $options = parent::option_definition();
    $options['link_text'] = array(
      'default' => t('Send to Leads'),
      'translatable' => TRUE,
    );
    return $options;
  }
  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    $form['link_text'] = array(
      '#title' => t('Link text'),
      '#type' => 'textfield',
      '#description' => t('The text that will display for the "Squawk this" link.'),
      '#default_value' => $this->options['link_text'],
    );
  }
  function construct() {
    parent::construct();
    $this->additional_fields['sender'] = 'sender';
  }
  function render($values) {
    $sender_uid = $values->{$this->aliases['sender']};
    if (statuses_user_access('add')) {
      drupal_add_css(drupal_get_path('module', 'statuses') . '/resources/statuses.css');
      $options = array(
        'attributes' => array('class' => array('statuses-repost', 'squawk-hello',)),
        'query' => array(
          'sid' => $values->{$this->field_alias},
          'destination' => $_GET['q'],
        ),
      );
      return l($this->options['link_text'], 'statuses/share', $options);
    }
  }
}
