<?php

/**
 * @file
 *   Recipient Arg is current user's lead. 
 */

/**
 * Argument handler to select statuses relevant to a user and that user's friends.
 */
class views_handler_argument_squawk_is_our_lead extends views_handler_argument {
  function query($group_by = FALSE) {
    $this->ensure_my_table();
    $alias = $this->query->ensure_table('users');
	$ur = $this->query->ensure_table('user_relationships');
	$rtid = 1; 
    $argument = $this->argument;
	//recipient is arg. Arg is lead of current user. Arg is lead of other users statuses.
	/* condition: arg is requester_id and rtid '1' and approved is '1'  sender is current user or
	
	*/
    $this->query->add_where(isset($this->options['group']) ? $this->options['group'] : 0, db_or()
      ->condition(db_and()
        ->condition(db_and()
          ->condition('requester_id', $argument)
          ->condition("$ur.rtid",1))
        ->condition('approved', 1))
      ->condition("$alias.uid", $argument));
  }
}
