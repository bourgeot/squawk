<?php
/* 
 * Implements hook_views_data_alter(&$data)
 *
 */
function squawk_views_data_alter(&$data) {
	//dsm('hello world');
	$data['statuses']['recipient']['title'] = t('Receiver');
	$data['statuses']['recipient_is_current'] = array(
      'real field' => 'recipient',
      'title' => t('Recipient is current user'),
      'help' => t('Current user is the one who received the status'),
      'filter' => array(
        'handler' => 'views_handler_filter_user_current'
      ),
	);

  // Set up relationships so that the sender and recipient fields of the status table are related to the
  // requester_id and requestee_id of the user relationships table. 
  
	$data['statuses']['sender-requestee'] = array(
		'title' => t('sender-requestee'),
		'help' => t('Relates status sender to the UR requestee'),
		'field' => array(
			'field' => 'sender',
		),
		'relationship' => array(
			'base' => 'user_relationships',
			'base field' => 'requestee_id',
			'field' => 'sender',
		),
		
	);
	$data['statuses']['recipient-requestee'] = array(
		'title' => t('recipient-requestee'),
		'help' => t('Relates status recipient to the UR requestee'),
		'field' => array(
			'field' => 'recipient',
		),
		'relationship' => array(
			'base' => 'user_relationships',
			'base field' => 'requestee_id',
			'field' => 'recipient',
		),
		
	);

	$data['statuses']['sender-requester'] = array(
		'title' => t('sender-requester'),
		'help' => t('Relates status sender to the UR requester'),
		'field' => array(
			'field' => 'sender',
		),
		'relationship' => array(
			'base' => 'user_relationships',
			'base field' => 'requester_id',
			'field' => 'sender',
		),
		
	);
	$data['statuses']['recipient-requester'] = array(
		'title' => t('recipient-requester'),
		'help' => t('Relates status recipient to the UR requester'),
		'field' => array(
			'field' => 'recipient',
		),
		'relationship' => array(
			'base' => 'user_relationships',
			'base field' => 'requester_id',
			'field' => 'recipient',
		),
		
	);
  	//user relationships additions
	// tcb adding this for other fields 8/21/2012
	$data['user_relationships_types'] = array(
		'reverse_name' =>  array(
		  'title' => t('Reverse Name'),
		  'help' => t('Reverse Name of the relationship (singular)'),
		  'argument' => array(
			'handler' => 'views_handler_argument_string'
		  ),
		  'filter' => array(
			'handler' => 'views_handler_filter_string',
		  ),
		  'field' => array(
			'field' => 'reverse_name',
			'handler' => 'views_handler_field',
			'click sortable' => TRUE
		  ),
		  'sort' => array(
			'handler' => 'views_handler_sort',
		  ),
		),
		'reverse_plural_name' => array(
		  'title' => t('Reverse Plural name'),
		  'help' => t('Reverse Name of the relationship (plural)'),
		  'argument' => array(
			'handler' => 'views_handler_argument_string'
		  ),
		  'field' => array(
			'field' => 'reverse_plural_name',
			'handler' => 'views_handler_field',
			'click sortable' => TRUE
		  ),
		  'sort' => array(
			'handler' => 'views_handler_sort',
		  ),
		),
	);

}