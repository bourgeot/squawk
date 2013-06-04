function squawk_format_user($uid) {
	// returns an array with the main key is the uid and has some formatted stuff for
	// list items
	$u=user_load($uid);
	if (!$u->field_first_name || !$u->field_last_name || !$u->field_department) {
		$display_name = $u->name;
		$dept = 'Student Affairs';
	}
	else {
		$fname=$u->field_first_name['und'][0]['value'];
		$lname=$u->field_last_name['und'][0]['value'];
		$dept=$u->field_department['und'][0]['value'];
		$display_name = $fname . ' ' . $lname;
	}
	$pic = theme('user_picture', array('account' => $u));
	$attributes = array( 
		'attributes' => array( 
			'id' => 'user-' . $u->uid,
			'class'=>array('user-link',),
			)
	);
	
	//$output['data'] = $pic . l($display_name,'user/' . $u->uid, $attributes)
	//	. "<span class='ui-li-count department' >" . $dept . "</span>";
	//dsm($output);
	$department = "<span class='ui-li-count department' >" . $dept . "</span>";
	$html_blob = $pic . $display_name . $department;
	$output['data'] = array(
		'#theme' => 'link',
		'#text' => $html_blob,
		'#path' => $u->uid,
		'#options' => array(
			'attributes' => array(
				'id' => 'user-' . $u->uid,
				'class' => array('user-link',),
			)
			'html' => TRUE,
		)
	);
	
	return drupal_render($output);
}