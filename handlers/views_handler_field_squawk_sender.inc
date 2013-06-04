<?php

/**
 * @file
 * Squawk Views integration.
 * Link to user profile of status sender
 */
class views_handler_field_squawk_sender extends views_handler_field_user_link {
  function construct() {
    parent::construct();
    $this->additional_fields['uid'] = 'sender';
  }

  function render($values) {
    $uid = $values->{$this->aliases['sender']};
    $sender = user_load($uid);
    return theme('username', array('account' => $sender));
  }

}
