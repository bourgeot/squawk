<?php

/**
 * @file
 *   Filters to statuses where a given user is a participant.
 */

/**
 * Filter handler to select statuses where a given user is a participant.
 */
class views_handler_filter_squawk_st_sender_ur_requestee extends views_handler_filter {
  function admin_summary() { }
  function operator_form() { }
  function query() {
    if ($this->value) {
      $this->ensure_my_table();
	  $ur = 'user_relationships';
	  $fld = 'requestee_id';
	  $requestee = $this->add_field($ur,$fld);
	  //$wclause = $requestee . ' = sender';
	  $this->query->add_where(0,"user_relationships.requestee_id","sender","=");
    }
  }
}
