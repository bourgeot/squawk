<?php
/*
 *
 */

function display_update_form($form, &$form_state, $node) {
  //update the last viewed time stamp for this user.
  node_tag_new($node);
  $form = array();
  $form_state['node'] = $node;
  global $user;
  //dsm($form_state);
  //dsm($node);
  if ($user->uid != $node->field_recipient['und'][0]['value']) {
    $other_channel = TRUE;
  }
  else {
    $other_channel = FALSE;
  }
//  $form_state['archived'] = ($node->field_archived['und'][0]['value'] == 1) ? 1 : 0;
//  $form_state['private'] = ($node->field_private['und'][0]['value'] == 1) ? 1 : 0;
  $form_state['archived'] = FALSE;
  $form_state['private'] = FALSE;
  $back_context_label = 'Return (New Msgs)';
  if ($node->field_archived['und'][0]['value'] == TRUE) {
    $form_state['archived'] = TRUE;
    $back_context_label = 'Return (Archived Msgs)';
  }
  if ($node->field_private['und'][0]['value'] == TRUE) {
    $form_state['private'] = TRUE;
    $back_context_label = 'Return (Private Msgs)';
  }
  $view = node_view($node);
  $button_label = ($form_state['archived'] == 1) ? 'Restore' : 'Send to Archive';
  $form = array(
    'actions' => array(
      '#type' => 'container',
      '#attributes' => array(
        'class' => array('actions'),
      ),
      'edit_or_forward' => array(
        '#type' => 'button',
        '#prefix' => '<span class="edit-button">',
        '#suffix' => '</span>',
        '#value' => 'Edit/Forward',
        '#disabled' => $other_channel,
        '#access' => !($other_channel),
        '#executes_submit_callback' => true,
        '#submit' => array('context_edit_callback'),
        '#attributes' => array(
          'class' => array('action-button'),
        ),
      ),
      'send_to_archive' => array(
        '#type' => 'button',
        '#prefix' => '<span class="archive-button">',
        '#suffix' => '</span>',
        '#disabled' => $other_channel,
        '#access' => !($other_channel),
        '#value' => $button_label,
        '#executes_submit_callback' => true,
        '#attributes' => array(
          'id' => 'archive-button',
          'class' => array('action-button'),
        ),
        '#submit' => array('display_update_form_submit'),
        '#ajax' => array( 'callback' => 'relabel_button',
        ),
      ),
      'back' => array(
        '#type' => 'button',
        '#prefix' => '<span class="back-button">',
        '#suffix' => '</span>',
        '#value' => $back_context_label,
        '#attributes' => array(
          'class' => array('action-button'),
        ),
        '#limit_validation_errors' => array(),
        '#executes_submit_callback' => true,
        '#submit' => array('context_back_callback'),
      ),
    ),
    'node' => array(
      '#prefix' => '<div id="node-wrapper">',
      '#suffix' => '</div>',
      '#markup' => drupal_render($view),
    ),
  );
  return $form;
}
function relabel_button(&$form, $form_state) {
  $node = $form_state['node'];
  $element = $form['actions']['send_to_archive'];
    if ($node->field_archived['und'][0]['value'] == 1) {
      $element['#value'] = 'Restore';
      $form['actions']['send_to_archive']['#value'] = 'Restore';
    }
    else if ($node->field_archived['und'][0]['value'] == 0) {
      $element['#value'] = 'Send to Archive';
      $form['actions']['send_to_archive']['#value'] = 'Send to Archive';
    }
    else {
      $element['#value'] = 'WTF?';
    }
    $form['node']['#markup'] = drupal_render(node_view($node));
    return array(
      '#type' => 'ajax',
      '#commands' => array(
      ajax_command_replace('#archive-button', render($form['actions']['send_to_archive'])),
      ajax_command_replace('#node-wrapper', render($form['node'])),
      ),
    );
  //return $element;
}
function display_update_form_validate($form, &$form_state) {
  //change the state of the form(_state)
  $form_state['archived'] = ($form_state['archived'] == 1) ? 0 : 1;
}
function display_update_form_submit($form, &$form_state) {
  $node = $form_state['node'];
  if ($node->field_archived[0]['value'] != $form_state['archived']) {
    //synchronize them
    $node->field_archived[0]['value'] = $form_state['archived'];
    node_save($node);
  }
  //$a = ($node->field_archived['und'][0]['value'] == 1) ? 'Sent to the Archive.' : 'Retrieved from the Archive.';
  //drupal_set_message('status: '. $a);
  $form_state['rebuild'] = true;
  return;
}
function context_edit_callback($form, &$form_state) {
  $node = $form_state['node'];
  drupal_goto('channels/node/' . $node->nid . '/edit');
  return;
}
function context_back_callback($form, &$form_state) {
  $node = $form_state['node'];
  //dsm($node);
  if ($node->field_private['und'][0]['value'] == true) {
    $return_action = '/private';
  }
  elseif ($node->field_archived['und'][0]['value'] == true) {
    $return_action = '/archive';
  }
  else {
    $return_action = '/news';
  }
  drupal_goto('channels/' . $node->field_recipient['und'][0]['value'] . $return_action);
  return;
}
function manage_relationships_form($form, $form_state) {
  $form_state['users'] = squawk_fetch_users();
  foreach($form_state['users'] as $uid=>$account) {
    $form_state['user_names'][$uid]= format_username($account);
  }
  //set some defaults
  $title = 'A User\'s Relationships';
  $options = array();
  $peers = array();
  $leads = array();
  $user_set = $form_state['user_names'];
  // select a user and to manage his/her relationships
  $form['user_select'] = array(
    '#type' => 'select',
    '#title' => t('Select a user to manage his/her relationships:'),
    '#options' => $form_state['user_names'],
    '#ajax' => array(
      'callback' => 'fetch_results',
      'wrapper' => 'results-container',
      'method' => 'replace',
    ),
  );
  if (! empty($form_state['values']['user_select'])) {
    $title = $form_state['user_names'][$form_state['values']['user_select']] . '\'s Relationships';
    $report_set = squawk_relationships($form_state['values']['user_select'],'reports','human');
    //dsm($report_set);
    $peer_set = squawk_relationships($form_state['values']['user_select'],'peers','human');
    $lead_set = squawk_relationships($form_state['values']['user_select'],'leads','human');
    //remove selected user from user set
    $user_set = array_diff(
      $user_set,
      array($form_state['values']['user_select'] => $form_state['user_names'][$form_state['values']['user_select']])
    );
    if(!empty($report_set)) {
      foreach($report_set as $uid=>$value) {
        //$options[$uid] = $form_state['user_names'][$uid];
        $options[$value['rid']] = $form_state['user_names'][$uid];
        $report_names[$uid] = $form_state['user_names'][$uid];
      }
      //remove reports from user set
      $user_set = array_diff($user_set, $report_names);
    }

    if(!empty($peer_set)) {
      foreach($peer_set as $uid=>$value) {
        $peers[$uid] = $form_state['user_names'][$uid];
      }
    }
    if(!empty($lead_set)) {
      foreach($lead_set as $uid=>$value) {
        $leads[$uid] = $form_state['user_names'][$uid];
      }
    }

  }
  else {
    $title = 'A User\'s Relationships';
    $options = array('- None -');
    $peers = array('- None -');
    $leads = array('- None -');
    $user_set = $form_state['user_names'];
  }
    $form['results'] = array(
      '#type' => 'container',
      '#attributes' => array(
        'id' => 'results-container',
      ),
    );
    $form['results']['title-item'] = array(
      '#prefix' => '<div id="title-item">',
      '#suffix' => '</div>',
      '#type' => 'item',
      '#title' => $title,
    );
    $form['results']['manage'] = array(
      '#type' => 'fieldset',
      '#title' => 'Manage Reports (Relationships)',
      '#prefix' => '<div id="manage-relationships-div">',
      '#suffix' => '</div>',
    );
    $form['results']['manage']['report_set']= array(
      '#prefix' => '<div id="report-set-div">',
      '#suffix' => '</div>',
      '#type' => 'select',
      '#multiple' => TRUE,
      '#size' => min(10, count($options)),
      '#title' => t('Current Reports'),
      '#empty_option' =>  t('- None -'),
      '#options' => $options,
    );
    $form['results']['manage']['actions'] = array(
      '#type' => 'container',
      '#attributes' => array(
        'id' => 'actions-div',
        'class' => array('user-actions', 'manage-users'),
      ),
    );
    $form['results']['manage']['actions']['add_report'] = array(
      '#prefix' => '<div id="add-report-div">',
      '#suffix' => '</div>',
      '#type' => 'button',
      '#value' => t('Add User(s)'),
      '#ajax' =>  array(
        'callback' => 'add_report',
        'wrapper' => 'manage-relationships-div',
        'method' => 'replace',
      ),
    );
    $form['results']['manage']['actions']['remove_report'] = array(
      '#prefix' => '<div id="remove-report-div">',
      '#suffix' => '</div>',
      '#type' => 'button',
      '#value' => t('Remove User(s)'),
      '#ajax' =>  array(
        'callback' => 'add_report',
        'wrapper' => 'manage-relationships-div',
        'method' => 'replace',
      ),
    );

    $form['results']['manage']['user_set']= array(
      '#prefix' => '<div id="user-set-div">',
      '#suffix' => '</div>',
      '#type' => 'select',
      '#multiple' => TRUE,
      '#attributes' => array(
        'class' => array('chosen-select', 'multi-select'),
      ),
      '#size' => min(10, count($user_set)),
      '#title' => t('Available Users'),
      '#empty_option' =>  t('- None -'),
      '#options' => $user_set,
    );

    $form['results']['peer_set']= array(
      '#prefix' => '<div id="peer-set-div">',
      '#suffix' => '</div>',
      '#type' => 'select',
      '#multiple' => TRUE,
      '#size' => min(10, count($peers)),
      '#title' => t('Peers'),
      '#empty_option' =>  t('- None -'),
      '#options' => $peers,
    );

    $form['results']['lead_set']= array(
      '#prefix' => '<div id="lead-set-div">',
      '#suffix' => '</div>',
      '#type' => 'select',
      '#multiple' => TRUE,
      '#size' => min(10, count($leads)),
      '#title' => t('Leads'),
      '#empty_option' =>  t('- None -'),
      '#options' => $leads,
    );
    $list = array(
      '#theme' => 'item_list',
      '#prefix' => '<div id="list-set-div">',
      '#suffix' => '</div>',
      '#type' => 'ul',
      '#attributes' => array(
        'class' => array( 'relationships', 'leads'),
      ),
      //'#multiple' => TRUE,
      //'#size' => min(10, count($leads)),
      '#title' => t('Leads'),
      //'#empty_option' =>  t('- None -'),
      '#items' => $leads,
    );
    $form['results']['test'] = array('#markup'=>drupal_render($list),);

  return $form;
}
function squawk_fetch_users() {
  $users = array();
  $result = db_query("SELECT uid FROM {users}");
  foreach ($result as $record) {
    $list[] = $record->uid;
  }
  $accounts = user_load_multiple($list);
  return $accounts;
}
function fetch_results($form, $form_state) {
  //return $form['report_set'];
  return $form['results'];
}
function add_report(&$form, $form_state) {
  if (($form_state['values']['user_select'] == 0) || empty($form_state['values']['user_select'])) {
    return $form['results']['manage'];
  }
  //dsm('here');
  //dsm($form_state);
  //triggering element is in $form_state['triggering_element'];
  switch($form_state['triggering_element']['#value']) {
    case 'Add User(s)':
      if (!empty($form_state['values']['user_set']) && ($form_state['values']['user_select'] > 0)) {
        //add the relationship to the table.
        foreach($form_state['values']['user_set'] as $value) {
          $relationship = new stdClass;
          //$requestee = $form_state['values']['user_set'];
          $relationship->requester_id = $form_state['values']['user_select'];
          $relationship->requestee_id = $value;
          $relationship->rtid = 1;
          //dsm($relationship);
          $result=user_relationships_save_relationship($relationship,'approve');
        }
      }
      else {
        return $form['results']['manage'];
      }
    break;
    case 'Remove User(s)':
      if (!empty($form_state['values']['report_set']) && ($form_state['values']['user_select'] > 0)) {
        //remove the relationship from the table.
        global $user;
        foreach($form_state['values']['report_set'] as $value) {
          $relationship = new stdClass;
          //$requestee = $form_state['values']['user_set'];
          $relationship->requester_id = $form_state['values']['user_select'];
          $relationship->requestee_id = $form_state['values']['user_select']; //I don't think this is important.
          $relationship->rtid = 1;
          $relationship->rid = $value; //this is the critical piece
          //dsm($form_state);
          //delete relationship
          user_relationships_delete_relationship($relationship, $user, 'remove');
        }
      }
      else {
        return $form['results']['manage'];
      }
    break;
  }
  //requery the boxes
  $report_set = squawk_relationships($form_state['values']['user_select'],'reports','human');
  if(!empty($report_set)) {
    foreach($report_set as $uid=>$value) {
      //$options[$uid] = $form_state['user_names'][$uid];
      $options[$value['rid']] = $form_state['user_names'][$uid];
      $report_names[$uid] = $form_state['user_names'][$uid];
    }
    //remove reports from user set
    $user_set = array_diff($form_state['user_names'], $report_names);
    //remove selected user from user set
    $user_set = array_diff(
      $user_set,
      array($form_state['values']['user_select'] => $form_state['user_names'][$form_state['values']['user_select']])
    );
    $form['results']['manage']['report_set']['#options']=$options;
    $form['results']['manage']['report_set']['#size'] = min(10, count($options));
  }
  else {
    //update the empty select box
    $form['results']['manage']['report_set']['#options']=array();
    $form['results']['manage']['report_set']['#size'] = 1;
  }
  //update option sets

  $form['results']['manage']['user_set']['#options']=$user_set;
  return $form['results']['manage'];
}
function _query_orgs() {
  return;
}
//*
//* form to add a new channel
function new_channel_form($form, &$form_state) {
  //name*, description, field_created_by*, field_members,
  //field_graphic, field_type*, field_status*, rdf_mapping??
  //$default_graphic = image_load('public://no_photo.png');
  //by default channels are current and created by is the current user.
  //go fetch the fid of the default picture
  $query = new EntityFieldQuery();
  $query
    ->entityCondition('entity_type', 'file', '=')
    ->propertyCondition('uri', 'public://default_images/no_photo.png', '=');
  $result = $query->execute();

  $fids = array();
  foreach($result['file'] as $record) {
  //should be 1
    $fids[] = $record->fid;
  }

  drupal_set_title('New Channel');
  if(!isset($form_state['storage']['attachments'])) {
    $form_state['storage']['attachments'] = 1;
  }
  $form = array(
    'name' => array(
      '#type' => 'textfield',
      '#title' => t('Channel Name'),
      '#required' => TRUE,
      '#weight' => 0,
    ),
    'description' => array(
      '#type' => 'textarea',
      '#title' => t('Description'),
      '#required' => FALSE,
      '#weight' => 1,
    ),
    'display_graphic' => array(
      '#type' => 'item',
      '#theme' => 'image_style',
      '#style_name' => 'thumbnail',
      // '#path' => 'public://no_photo.png',
      '#path' => 'public://no_photo.png',
      '#prefix' => '<div id="graphic-wrapper" class="channel-list-item-picture user-picture">',
      '#suffix' => '</div>',
      '#alt' => t('wildcat silhouette'),
      //'#title' => t('default image'),
      '#weight' => 2,
    ),
    'graphic' => array(
      '#title' => t('Channel Graphic'),
      '#type' => 'managed_file',
      '#description' => t('Add a graphic to help members identify this channel'),
      '#default_value' => $fids[0],
      '#upload_location' => 'public://',
      '#prefix' => '<div id="graphic-upload-wrapper">',
      '#suffix' => '</div>',
      '#weight' => 3,
    ),
    'channel_type' => array(
      '#title' => t('Channel Type'),
      '#type' => 'radios',
      '#options' => array(
        'broadcast' => t('Broadcast: Only the person who created the channel can add updates, everyone can view.'),
        'open' => t('Open: Other people can join the channel and post to it without permission from the creator.'),
        'closed' => t('Closed: The person who created the channel controls who can see and add to the channel.'),
      ),
      '#default_value' => 'closed',
      '#required' => TRUE,
      '#multiple' => FALSE,
      '#weight' => 4,
    ),
    'member_list' => array(
      '#title' => t('Members'),
      '#description' => t('Enter the net ids, separated by commas, of the additional people who can read and contribute to this channel.'),
      '#type' => 'textarea',
      '#rows' => 1,
      '#required' => FALSE,
      '#weight' => 5,
      '#states' => array(
        'enabled' => array(
          ':input[name="channel_type"]' => array('value' => 'closed'),
        ),
        'visible' => array(
          ':input[name="channel_type"]' => array('value' => 'closed'),
        ),
        'disabled' => array(
          ':input[name="channel_type"]' => array(array('value' => 'open'), array('value' => 'broadcast')),
        ),
        'invisible' => array(
          ':input[name="channel_type"]' => array(array('value' => 'open'), array('value' => 'broadcast')),
        ),
      ),
    ),
/*    'attachments' => array(
      '#type' => 'fieldset',
      '#tree' => TRUE,
      '#title' => t('Attachments'),
      '#description' => t('You can add attachments now or later.'),
      '#prefix' => '<div id="attachments">',
      '#suffix' => '</div>',
      '#weight' => 6,
    ),*/
    'actions' => array(
      '#type' => 'actions',
      '#weight' => 8,
      'submit' => array(
        '#type' => 'submit',
        '#value' => t('Save'),
        '#weight' => 1,
        '#submit' => array('new_channel_form_submit'),
      ),
      'cancel' => array(
        '#type' => 'submit',
        '#value' => t('Cancel'),
        '#weight' => 2,
        '#submit' => array('new_channel_form_cancel'),
        '#limit_validation_errors' => array(),
      ),
    ),
  );
  if(!isset($form['#parents'])) {
    $form['#parents'] = array();
  }
  $vocabulary = taxonomy_vocabulary_machine_name_load('channels');
  $term = new StdClass();
  $term->vid = $vocabulary->vid;
  $term->tid = 0;
  $term->name = '';
  $term->description = '';
  $field_name = 'field_attachments';
  //this code works. testing a separate method...
  $entity_type = 'taxonomy_term';

  $field = field_info_field('field_attachments');
  $instance = field_info_instance('taxonomy_term', $field_name, 'channels');
  //$items = field_get_items($entity_type, $term, $field_name, LANGUAGE_NONE);
  $form['attachments'] = field_default_form('taxonomy_term', NULL, $field, $instance, LANGUAGE_NONE, NULL, $form, $form_state);
  $form['attachments']['#weight'] = 6;
  $form['attachments']['#default_value']=0;
  field_default_form('taxonomy_term', NULL, $field, $instance, LANGUAGE_NONE, NULL, $form, $form_state);
  //$form[]=field_attach_view('taxonomy_term', $term, 'attachments');
 //$content[] = $form['field_attachments'];

  $form_state['redirect'] = 'ext/channels';
  //field_attach_form('taxonomy_term', NULL, $form, $form_state);
  return $form;
}
/*
function new_channel_form_add_file($form, &$form_state) {
  return $form['attachments'];
}
* */
function new_channel_form_submit($form, $form_state) {

  //save a new term to the channels vocabulary.
  global $user;

  $vocabulary = taxonomy_vocabulary_machine_name_load('channels');
  $term = new stdClass();
  $term->name = check_plain(ucwords($form_state['values']['name']));
  $term->description = check_plain($form_state['values']['description']);
  //member processing
  $members=explode(',',$form_state['values']['member_list']);
    if(!empty($members)) {
    foreach($members as $counter=>$string) {
      $member = check_plain(trim($string));
      if($member != '') {
        $term->field_members['und'][]['value'] = $member;
      }
    }
  }
  //if no graphic is loaded the graphic value is 0
  if ($form_state['values']['graphic'] != 0) {
    // assign the fid to the graphic fid and the taxonomy module will take care of the
    //low level file goop.
    $term->field_graphic['und'][0]['fid'] = $form_state['values']['graphic'];
  }
  else {
    dsm('still bad');
    //$term->field_graphic['und'][0]['fid'] = $form_state['values']['default_graphic'];
  }
  //same with attachments
  //dsm($form_state);
  if ($form_state['values']['field_attachments'][LANGUAGE_NONE][0]['fid'] > 0) {
    $term->field_attachments = $form_state['values']['field_attachments'];
  }
  $term->field_status['und'][0]['value'] = 'current';
  $term->vid = $vocabulary->vid;
  $term->field_created_by['und'][0]['value'] = $user->uid;
  taxonomy_term_save($term);
  $form_state['redirect'] = 'ext/channels';
  drupal_goto($form_state['redirect']);
  return;
}
function new_channel_form_cancel($form, $form_state) {
  drupal_goto('ext/channels');
  return;
}
function channel_update_add_form($form, &$form_state, $context) {
  //title*,body*,field_private,field_recipient,field_tags, field_channels,
  //field_archived,field_previous_content,field_previous_title,
  //field_previous_tags,field_previous_author,field_author_surname
  global $user;
  $account = user_load($user->uid);
  //change. the recipient should be the channel owner. not the author.
  $form = array(
    'title' => array(
      '#type' => 'textfield',
      '#title' => t('Headline'),
      '#default_value' => t('New Update'),
      '#required' => TRUE,
      '#attributes' => array(
        'class' => array( 'default-value'),
      ),
      '#weight' => 1,
      '#attached' => array(
        'js' => array(drupal_get_path('module', 'squawk') . '/scripts/default-value.js'),
      ),
    ),
    'body' => array(
      '#type' => 'text_format',
      '#title' => t('Content'),
      '#required' => FALSE,
      '#format' => 'full_html',
      '#weight' => 2,
    ),
    'author_surname' => array(
      '#type' => 'value',
      '#value' => $account->field_last_name['und'][0]['value'],
    ),
    'recipient' => array(
      '#type' => 'value',
      '#value' => $context->field_created_by['und'][0]['value'],
    ),
    'archived' => array(
      '#type' => 'value',
      '#value' => 0,
    ),
    'private' => array(
      '#type' => 'value',
      '#value' => 0,
    ),
    'mentions' => array(
      '#type' => 'value',
      'value' => array(),
    ),
    'target_channel' => array(
      '#type' => 'value',
      '#value' => $context->tid,
    ),
    'actions' => array(
      '#type' => 'actions',
      '#weight' => 5,
      'submit' => array(
        '#type' => 'submit',
        '#value' => t('Tell It!'),
        '#weight' => 1,
        '#submit' => array('channel_update_add_submit'),
        '#validate'=>array('channel_update_add_validate'),
      ),
      'cancel' => array(
        '#type' => 'submit',
        '#value' => t('Cancel'),
        '#weight' => 2,
        '#submit' => array('channel_update_add_cancel'),
        '#limit_validation_errors' => array(),
      ),
    ),
  );
  $form['tagset'] = array(
    '#type' => 'container',
    '#weight' => 3,
    '#attributes' => array(
      'id' => 'tag-set',
    ),
  );
  $terms = squawk_fetch_user_tags($user->uid);
  //make sure it has a value. It should have.
  if(!empty($terms)) {
    foreach($terms['tag_set'] as $term) {
      if($term->field_active['und'][0]['value'] == TRUE) {
        //build the options array for the checkboxes control
        $options[$term->tid] = $term->name;
      }
    }
  }
  if ($options) {
      $form['field_tags']['und']['#options'] = $options;
  }
  $form['tagset']['add_tag_subform'] = array(
    '#type' => 'container',
    '#attributes' => array(
      'id' => 'add-tag-subform',
    ),
  );
  $form['tagset']['tags'] = array(
    '#type' => 'checkboxes',
    '#title' => 'Tags',
    '#validated' => TRUE, //required because this can have content updated via ajax
    '#options' => _fetch_active_tag_options($user->uid),
    '#prefix' =>'<div id="active-user-tags">',
    '#suffix' => '</div>',
  );

  if (!isset($form_state['show_subform']) || $form_state['show_subform'] == FALSE) {
    $form['tagset']['add_tag_subform']['add_tag'] = array(
    '#type' => 'button',
    '#value' => t('Add Tag'),
    '#limit_validation_errors' => array(),
      '#ajax' => array(
        'callback' => '_show_tag_subform',
        //'wrapper' => 'add-tag-subform',
        'wrapper' => 'tag-set',
        'method' => 'replace',
      ),
    );

    $form_state['show_subform'] = TRUE;
  }
  else {
    $form['tagset']['add_tag_subform']['new_tag'] = array(
      '#type' => 'textfield',
      //'#prefix' => '<div id="new-tag-field" >',
      //'#suffix' => '</div>',
      '#title' => t('Tag Name'),
      '#title_display' => 'before',
      '#size' => 35,
    );
    $form['tagset']['add_tag_subform']['cancel'] = array(
    '#type' => 'button',
    '#value' => t('Cancel'),
    '#limit_validation_errors' => array(),
      '#ajax' => array(
        'callback' => '_cancel_tag_subform',
        //'wrapper' => 'add-tag-subform',
        'wrapper' => 'tag-set',
        'method' => 'replace',
      ),
    );
    $form['tagset']['add_tag_subform']['save'] = array(
    '#type' => 'button',
    '#value' => t('Save'),
    '#limit_validation_errors' => array(),
      '#ajax' => array(
        'callback' => '_save_tag_subform',
        'wrapper' => 'tag-set',
        'method' => 'replace',
      ),
    );
    $form_state['show_subform'] = FALSE;
  }

  //Add the crosspost fieldset
  $other_channels = member_channels($user->uid, array($context));
  $options = array();
  foreach($other_channels['#items'] as $item) {
    $options[$item['#tid']] = $item['#name'];
  }
  if(!empty($options)) {
    $form['crosspost'] = array(
      '#type' => 'fieldset',
      '#title' => t('Crosspost this to my other channels'),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
      '#weight' => 4,
      '#prefix' => '<div id="crosspost-wrapper">',
      '#suffix' => '</div>',
      'other_channels' => array(
        '#type' => 'checkboxes',
        '#multiple' => TRUE,
        //'#title' => 'Channels',
        '#options' => $options,
        '#prefix' =>'<div id="other-channels">',
        '#suffix' => '</div>',
      ),
    );
  }
  //dsm($other_channels);
  $form_state['context'] = 'taxonomy/term/' . $context->tid;
  return $form;
}
function channel_update_add_validate($form, &$form_state) {
  dsm('here');
  //transform the values of the active tags control back to the field tags for saving
  //the data for the tags that the user has selected are in $form_state['values']['tags'] and they have to be translated to
  //$form_state['values']['field_tags'] one for one.
  foreach ($form_state['values']['tags'] as $key=>$value) {
    if ($value > 0) {
      $form_state['values']['field_tags']['und'][$key]['tid'] = $value;
    }
  }
  //process mentions.
  $mentions = extract_mentions($form_state['values']['body']['value']);
  dsm($mentions);
  $form_state['values']['mentions']=array();
  foreach($mentions as $key=>$mention) {
    $form_state['values']['mentions']['und'][$key]['tid'] = $mention['tid'];
  }
  //dsm($form_state);
  //$form_state['values']['mentions'] = array('und' => array_merge(array('tid'), $mentions));
  //$form_state['values']['mentions']=$mentions;
  dsm($form_state);

}
function channel_update_add_submit($form, &$form_state) {
  //return to the taxonomy/term channel page.
  global $user;
  dsm($form);
  dsm($form_state);

  //dsm($form);
  //glean stuff
  //$receiver_1 = $form_state['values']['field_recipient'][LANGUAGE_NONE][0]['value'];
  //process everything.
  //$new_nid = squawk_node_clone($form_state['nid']);
  //dsm(node_load($new_nid));
  $node = new stdClass();
  $node->type = 'update';
  $node->is_new = TRUE;
  node_object_prepare($node);
  $node->language = LANGUAGE_NONE;
  $node->comment = 0;
  //$node->format = 2;
  $node->published = TRUE;
  $node->title = $form_state['values']['title'];
  $node->body[$node->language][0]['value'] = $form_state['values']['body']['value'];
  $node->body[$node->language][0]['format'] = $form_state['values']['body']['format'];
  $node->field_channels[$node->language][0]['tid'] = $form_state['values']['target_channel'];
  $node->field_recipient[$node->language][0]['value'] = $form_state['values']['recipient'];
  $node->field_author_surname[$node->language][0]['value'] = $form_state['values']['author_surname'];
  $node->field_archived[$node->language][0]['value'] = $form_state['values']['archived'];
  $node->field_private[$node->language][0]['value'] = $form_state['values']['private'];
  //transform the values of the active tags control back to the field tags for saving
  //the data for the tags that the user has selected are in $form_state['values']['tags'] and they have to be translated to
  //values for $node->field_tags one for one.
  foreach ($form_state['values']['tags'] as $key=>$value) {
    if ($value > 0) {
      $node->field_tags['und'][$key]['tid'] = $value;
    }
  }
  $node->field_mentions = $form_state['values']['mentions'];
  //$node->uid = $user->uid;
  //update the creation date/time

  //$node->created = REQUEST_TIME;
  node_save($node);
  //$node = $form_state['node'];
  //explicitly set the author to the current user
  //dsm($node);
  //$node->uid = $user->uid;
  //$node->name = $user->name;
  //node_save($node);
  if (isset($node->nid) ) {
    //hello to the folks at home
    //dsm('bark');
    drupal_set_message('Saved.', 'status');
    $crossposts = array();
    $other_channels = array();
    //dsm($node->nid . ' and ...process the crossposts');
    //dsm($form_state);
    //dsm($bonus_receivers);
    //$form_state['complete form']['crosspost']['other_channels']['#options'] as $tid=>$channel_name)

    foreach ($form_state['values']['other_channels'] as $key=>$value) {
      if ($value > 0) {
        $crossposts[] = $value;
        $other_channels[$key] = $form_state['complete form']['crosspost']['other_channels']['#options'][$key];
      }
    }
    if (!empty($crossposts)) {
      foreach ($crossposts as $counter=>$channel) {
        //dsm('attempting to post to ' . $recipient . ' with ' . $node->nid);
        //trying some stuff here.
        $node_copy = new stdClass();
        $node_copy = clone $node;
        $node_copy->nid = NULL;
        $node_copy->vid = NULL;
        $node_copy->tnid = NULL;
        $node_copy->created = NULL;
        $node_copy->path = NULL;
        $node_copy->field_channels['und'][0]['tid'] = $channel;
        node_save($node_copy);
        $success = $node_copy->nid;
        if ($success > 0) {
          drupal_set_message('Successfully crossposted to channel ' . $other_channels[$channel] . '.', 'status');
        }
      }
    }
  }

  //dsm($form_state);
  //return to the main update page the user was on. that should provide feedback that the save was successful.
  drupal_goto($form_state['context']);
  return;
}
function channel_update_add_cancel($form, &$form_state) {
  //return to the taxonomy/term channel page.
  //dsm($form_state);
  //dsm($form);
  drupal_goto($form_state['context']);
  return;
}
function channel_members_form($form, &$form_state, $term) {
  drupal_add_js('sites/all/modules/squawk/scripts/show-members-dialog.js');
  drupal_set_message('Coming Soon--channel owners can use this tab to add and remove members', 'status');
  if($term->field_type['und'][0]['value'] != 'broadcast') {
    $items=array(array('data'=> 'None'));
    if (isset($term->field_members['und'])) {
      for($i=0;$i<count($term->field_members['und']); $i++) {
        $items[$i]['data']=$term->field_members['und'][$i]['value'];
      }
    }
    $content['members_dialog'] = array(
      '#prefix' => '<div id="members-dialog" title="' . $term->name . ' Channel Members">',
      '#theme' =>'item_list',
      '#type' => 'ul',
      '#items' => $items,
      '#suffix' => '</div>',
    );
  }
return $content;
}
function channel_files_form($form, &$form_state, $term) {
  drupal_add_js('sites/all/modules/squawk/scripts/show-members-dialog.js');
  //drupal_set_message('Channel members can use this tab to add and remove files', 'status');
  drupal_set_title('');
  if($term->field_type['und'][0]['value'] != 'broadcast') {
    $items=array(array('data'=> 'None'));
    if (isset($term->field_members['und'])) {
      for($i=0;$i<count($term->field_members['und']); $i++) {
        $items[$i]['data']=$term->field_members['und'][$i]['value'];
      }
    }
    $form['members_dialog'] = array(
      '#prefix' => '<div id="members-dialog" title="' . $term->name . ' Channel Members">',
      '#theme' =>'item_list',
      '#type' => 'ul',
      '#items' => $items,
      '#suffix' => '</div>',
    );
  }
  $form[]=field_attach_view('taxonomy_term', $term, 'attachments');
  field_attach_form('taxonomy_term', $term, $form, $form_state);
  //$content[] = $form['field_attachments'];

  $form['actions'] = array(
    '#type' => 'actions',
    '#weight' => 10,
    'submit' => array(
      '#type' => 'submit',
      '#value' => t('Save Changes'),
      '#weight' => 1,
      '#submit' => array('channel_files_form_submit'),
    ),
    'cancel' => array(
      '#type' => 'submit',
      '#value' => t('Cancel'),
      '#weight' => 2,
      '#submit' => array('channel_files_form_cancel'),
      '#limit_validation_errors' => array(),
    ),
  );
  unset($form['field_created_by']);
  unset($form['field_graphic']);
  unset($form['field_members']);
  unset($form['field_status']);
  unset($form['field_type']);
  //dsm($form);
  //dsm($form_state);
  //$content[] = $form['actions'];

  return $form;
}
function channel_files_form_submit($form, $form_state) {
  $term = $form_state['build_info']['args'][0];
  $term->original = $term;
  field_attach_submit('taxonomy_term', $term, $form, $form_state);
  field_attach_update('taxonomy_term', $term);
return;
}
function edit_channel_form($form, &$form_state, $term) {
  drupal_add_js('sites/all/modules/squawk/scripts/show-members-dialog.js');
  //dsm($term);
  //name*, description, field_created_by*, field_members,
  //field_graphic, field_type*, field_status*, rdf_mapping??
  //$default_graphic = image_load('public://no_photo.png');
  //by default channels are current and created by is the current user.
  if($term->field_type['und'][0]['value'] != 'broadcast') {
    $items=array();
    if (isset($term->field_members['und'])) {
      for($i=0;$i<count($term->field_members['und']); $i++) {
        $items[$i]=$term->field_members['und'][$i]['value'];
      }
    }
  }
  drupal_set_title('Edit Channel');
  $form = array(
    'name' => array(
      '#type' => 'textfield',
      '#title' => t('Channel Name'),
      '#required' => TRUE,
      '#weight' => 0,
      '#default_value' => $term->name,
    ),
    'description' => array(
      '#type' => 'textarea',
      '#title' => t('Description'),
      '#required' => FALSE,
      '#default_value' => $term->description,
      '#weight' => 1,
    ),
    'default_graphic' => array(
      '#type' => 'item',
      '#theme' => 'image_style',
      '#style_name' => 'thumbnail',
      '#path' => $term->field_graphic['und'][0]['uri'],
      '#prefix' => '<div id="graphic-wrapper" class="channel-list-item-picture user-picture">',
      '#suffix' => '</div>',
      //'#alt' => t('wildcat silhouette'),
      //'#title' => t('default image'),
      '#weight' => 2,
    ),
    'graphic' => array(
      '#title' => t('Channel Graphic'),
      '#type' => 'managed_file',
      '#description' => t('Change the graphic identifying this channel'),
      '#upload_location' => 'public://',
      '#prefix' => '<div id="graphic-upload-wrapper">',
      '#suffix' => '</div>',
      '#default_value' => $term->field_graphic['und'][0]['fid'],
      '#weight' => 3,
    ),
    'channel_type' => array(
      '#title' => t('Channel Type'),
      '#type' => 'radios',
      '#options' => array(
        'broadcast' => t('Broadcast: Only the person who created the channel can add updates, everyone can view.'),
        'open' => t('Open: Other people can join the channel and post to it without permission from the creator.'),
        'closed' => t('Closed: The person who created the channel controls who can see and add to the channel.'),
      ),
      '#default_value' => $term->field_type['und'][0]['value'],
      '#required' => TRUE,
      '#multiple' => FALSE,
      '#weight' => 5,
    ),
    'member_list' => array(
      '#type' => 'textarea',
      '#title' => t('Current Members'),
      '#required' => FALSE,
      '#default_value' => implode(', ', $items),
      '#weight' => 4,
    ),
    'actions' => array(
      '#type' => 'actions',
      '#weight' => 5,
      'submit' => array(
        '#type' => 'submit',
        '#value' => t('Save'),
        '#weight' => 1,
        '#submit' => array('edit_channel_form_submit'),
      ),
      'cancel' => array(
        '#type' => 'submit',
        '#value' => t('Cancel'),
        '#weight' => 2,
        '#submit' => array('edit_channel_form_cancel'),
        '#limit_validation_errors' => array(),
      ),
    ),
  );
  $form_state['context'] = 'taxonomy/term/' . $term->tid;
  return $form;
}

function edit_channel_form_submit($form, &$form_state) {
  //dsm($form);
  //dsm($form_state);
  //save the edited term to the channels vocabulary.
  global $user;
  //$vocabulary = taxonomy_vocabulary_machine_name_load('channels');
  $term = new stdClass();
  $term = $form_state['build_info']['args'][0];
  //clear out the members value
  unset($term->field_members);
  $term->name = check_plain(ucwords($form_state['values']['name']));
  $term->description = check_plain($form_state['values']['description']);
  //member processing
  $members=explode(',',$form_state['values']['member_list']);
  if(!empty($members)) {
    foreach($members as $counter=>$string) {
      $member = check_plain(trim($string));
      if($member != '') {
        $term->field_members['und'][$counter]['value'] = $member;
      }
    }
  }
  //if no graphic is loaded the graphic value is 0
  if ($form_state['values']['graphic'] != 0) {
    // assign the fid to the graphic fid and the taxonomy module will take care of the
    //low level file goop.
    $term->field_graphic['und'][0]['fid'] = $form_state['values']['graphic'];
  }
  else {
    $term->field_graphic['und'][0]['fid'] = $form_state['values']['default_graphic'];
  }
  //
  $term->field_type['und'][0]['value'] = $form_state['values']['channel_type'];
  //$term->vid = $vocabulary->vid;
  $term->field_status['und'][0]['value'] = 'current';
  taxonomy_term_save($term);
  drupal_goto($form_state['context']);
  return;
}
function edit_channel_form_cancel($form, &$form_state) {
  drupal_goto($form_state['context']);
  return;
}
