<?php
function squawk_user_channel($arg_user) {
	//build a page of user updates. This will be the default front page when a user logs in.
	//Returns a render array
	$updates = array();
	global $user;
	//dsm($arg_user);
	drupal_set_title('');
	//load the destination user's picture.<--I believe $arg_user has been fully loaded by the menu choice I used.
	//update: yep.
	
	$updates['destination_title_area'] = array(
		'#type' => 'container',
      		'#attributes' => array(
        		'class' => array('container-tom',),
		),
	);
	$updates['destination_title_area']['destination_title']=array(
		'#markup' => format_username($user) . '\'s Update Channel',
		'#prefix' => '<div class="page-title" ><h1>',
		'#suffix' => '</h1></div>',
	);
	$updates['destination_title_area']['destination_picture']=array(
		'#theme' => 'user_picture',
		'#account' => $arg_user,
		'#prefix' => '<div id="user-picture-' . $arg_user->uid . '" class="user-picture" >',
		'#suffix' => '</div>',
	);
	$output = squawk_fetch_updates($user->uid, $arg_user->uid);
	//dsm(request_path());  //returns nothing b/c /user is the default front page (aliased path)
	//dsm(current_path());  //returns user/4 b/c this is the unaliased path
	$updates['main'] = array(
		'#prefix' => '<div class="squawk-updates" id="squawk-updates">',
		'#suffix' => '</div>',
		'#markup' => drupal_render($output),
		'#weight' => 2,
	);
	return $updates;
}

/**
 * Access callback for path /channels.
 *
 * Displays the channel page if user is logged in, or login form for anonymous
 * users.
 */
function squawk_channels_page() {
  global $user;
  if ($user->uid) {
    menu_set_active_item('channels/' . $user->uid);
    return menu_execute_active_handler(NULL, FALSE);
  }
  else {
    return drupal_get_form('user_login');
  }
}
function add_new_tag_form($form, &$form_state, $user) {
	drupal_set_title('Add Filters/Tags');
	$form['#tree'] = TRUE;
//	$form['description'] = array(
//		'#type' => 'item',
//		'#title' => t('A form with dynamically added new fields'),
//	);
	$form['user'] = array(
		'#type' => 'value',
		'#value' => $user,
	);		
	if (empty($form_state['num_tags'])) {
		$form_state['num_tags'] = 1;
	}
	// Build the number of name fieldsets indicated by $form_state['num_tags']
	for ($i = 1; $i <= $form_state['num_tags']; $i++) {
		$form['add_form'][$i] = array(
			'#type' => 'fieldset',
		);
		$form['add_form'][$i]['new_tag'] = array(
			'#type' => 'textfield',
			'#title' => t('Tag Name'),
			'#size' => 35,
		);
	}
	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Save All'),
		'#submit' => array('add_new_tag_save'),
	);
	 // Adds "Add another name" button
	$form['add_another'] = array(
		'#type' => 'submit',
		'#value' => t('Add Another'),
		'#submit' => array('add_new_tag_add_another'),
	);

	// If we have more than one tag, this button allows removal of the
	// last one.
	if ($form_state['num_tags'] > 1) {
		$form['remove_tag'] = array(
		'#type' => 'submit',
		'#value' => t('Cancel Last Tag'),
		'#submit' => array('add_new_tag_remove_last'),
		);
	}
	return $form;
}
function add_new_tag_add_another($form, &$form_state) {
  // Everything in $form_state is persistent, so we'll just use
  // $form_state['add_name']
  $form_state['num_tags']++;
  $form_state['rebuild'] = TRUE;
}


function add_new_tag_remove_last($form, &$form_state) {
  if ($form_state['num_tags'] > 1) {
    $form_state['num_tags']--;
  }
  $form_state['rebuild'] = TRUE;
}
function add_new_tag_save($form, &$form_state) {
	//dsm($form_state);
	$vocabulary = taxonomy_vocabulary_machine_name_load('squawk_tags');
	$user_tid_array = taxonomy_get_term_by_name($form_state['values']['user']->name, 'squawk_tags');
	//dsm($user_tid);
	foreach($form_state['values']['add_form'] as $instance) {
		$term = new stdClass();
		$term->name = check_plain(ucwords($instance['new_tag']));
		$term->vid = $vocabulary->vid;
		$term->parent = $user_tid_array[1]->tid;
		$term->field_active['und'][0]['value'] = TRUE;
		taxonomy_term_save($term);
	}
	$form_state['redirect'] = 'channels/' . $form_state['values']['user']->uid . '/tags';
	return;

}