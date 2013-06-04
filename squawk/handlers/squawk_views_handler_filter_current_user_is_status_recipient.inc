<?php

/**
 * @file
 * Filter handler for the current user
 * @author pedrofaria http://drupal.org/user/27805
 */
class squawk_views_handler_filter_current_user_is_status_recipient extends views_handler_filter_boolean_operator {
  function construct() {
    parent::construct();
    $this->value_value = t('Is the logged in user');
  }

    function query() {
    if ($this->value) {
      $this->ensure_my_table();
      $this->query->add_where(0, db_and()
        ->condition('type', 'user')
		->where('***CURRENT_USER*** = recipient'));
     // ->where('sender = recipient'));
    }
  }
  /* From The User Relationships curent user is requestee or requester handler
  function query() {
    $this->ensure_my_table();
    $op = empty($this->value) ? '<>' : '=';
    $where = "($this->table_alias.requester_id $op ***CURRENT_USER*** OR $this->table_alias.requestee_id $op ***CURRENT_USER***)";
    $this->query->add_where_expression($this->options['group'], $where);
  }
  */
}

