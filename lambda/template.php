<?php
/**
 * Contains theme override functions and preprocess functions for the UA-specific theme.
 **/

/**
 * Implements hook_theme().
 */
function lambda_theme(&$existing, $type, $theme, $path) {
  $hooks = array();
  return $hooks;
}
 
//Adds class to body classes from theme setting 'uabackground'
function lambda_preprocess_html(&$variables) {   
    $variables['attributes_array']['class'][] = theme_get_setting('uabackground');
} 
 
// Sets the UA logo color
function lambda_ualogo() {
  $ualogo = theme_get_setting('ualogo_color');
  switch($ualogo) {
    case ("blue"):
      $class = 'logo_blue';
      break;
    case ("red"):
      $class = 'logo_red';
      break;
    case ("white"):
      $class = 'logo_white';
      break;
  }
  if (isset($class)) {
    return ' class="'. $class .'"';
  }
  return $ualogo;
}

// Sets the UA banner background color
function lambda_uabanner() {
  $uabanner = theme_get_setting('uabanner');
    if (empty($uabanner)) {
      $uabanner = 'none';
    }
    return $uabanner;
}
     
// Sets the variables for the two above functions
function lambda_preprocess_region(&$variables) {
  if ($variables['region'] == 'branding') {
    $variables['lambda_ualogo'] = lambda_ualogo();
    $variables['lambda_uabanner'] = lambda_uabanner();
  }
}
 
// Return a themed Search Form *** Adapted from UA Zen theme used for new admissions.arizona.edu ***
function lambda_form_alter(&$form, &$form_state, $form_id) {
  if ($form_id == 'search_block_form') {
    $form['search_block_form']['#title'] = t('Enter the terms you wish to search for.'); // Change the text on the label element
    $form['search_block_form']['#title_display'] = 'invisible'; // Toggle label visibilty
    $form['search_block_form']['#size'] = 15;  // define size of the textfield
    $form['search_block_form']['#default_value'] = t(''); // Set a default value for the textfield
    $form['actions']['submit'] = array('#type' => 'image_button', '#src' => base_path() . path_to_theme() . '/images/search.png');
        
   // Add extra attributes to the text box
   // $form['search_block_form']['#attributes']['onblur'] = "if (this.value == '') {this.value = 'Search';}";
   // $form['search_block_form']['#attributes']['onfocus'] = "if (this.value == 'Search') {this.value = '';}";
  }    
}

/**
 * Return a header image based on the URL alias, else return a default image
 **/
/** not in use
function lambda_section_header() {
  $path = drupal_get_path_alias($_GET['q']);
  list($sections, ) = explode('/', $path, 2);
  $section = safe_string($sections);
  $filepath = path_to_theme() . '/images/header-' . $section .'.jpg';
  if (file_exists($filepath)) {
    $output = $filepath;
  }
  else {
    $output = path_to_theme() . '/images/header-default.jpg';
  }
  return $output;
}
//Make a string safe for header
function safe_string($string) {
  $string = strtolower(preg_replace('/[^a-zA-Z0-9_-]+/', '-', $string));
  return $string;
}
**/
