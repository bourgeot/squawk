<?php

/**
 * @file
 * Squawk Views integration.
 * Link to user profile of status recipient
 */
class views_handler_field_squawk_sender extends views_handler_field_user_link {
  function construct() {
    parent::construct();
    $this->additional_fields['uid'] = 'recipient';
  }

  function render($values) {
    $uid = $values->{$this->aliases['recipient']};
    $recipient = user_load($uid);
    return theme('username', array('account' => $recipient));
  }

}
