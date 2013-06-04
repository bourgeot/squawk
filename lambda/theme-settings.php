<?php
function lambda_form_system_theme_settings_alter(&$form, $form_state) { /** alter theme settings form **/
    
 /*
  * Create the UA Logo form using Forms API *** Adapted from Drupal 6 UA Theme ***
  */
  $form['uabrand'] = array(
    '#type'          => 'fieldset',
    '#title'         => t('UA Branding settings'),
  );
  // define 'Choose UA Logo Color': 
  $form['uabrand']['ualogo_color'] = array(
    '#type'          => 'select',
    '#options'       => array(
      'blue'         => 'Blue',
      'red'          => 'Red',  
      'white'        => 'White'),
    '#default_value' => theme_get_setting('ualogo_color'),
    '#title'         => t('Choose UA Logo Color'),   
   );
  // define 'Choose a UA Banner Color':
  $form['uabrand']['uabanner'] = array(
    '#type' => 'textfield',
    '#title' => t('UA Logo Background Color'),
    '#default_value' => theme_get_setting('uabanner'),
    '#size' => 7,
    '#maxlength' => 7,
    '#required' => FALSE,
    '#description' => t('<strong>Please use CSS-compliant and hexidecimal colors ONLY</strong> Defaults to transparent.')
  );
  // define 'Choose a Main Background Class':
  $form['uabrand']['uabackground'] = array(
    '#type' => 'textfield',
    '#title' => t('The Class for the Main Background Color or Gradient'),
    '#default_value' => theme_get_setting('uabackground'),
    '#size' => 12,
    '#maxlength' => 60,
    '#required' => FALSE,
    '#description' => t('<p><strong>Class must be defined in the <span style="text-decoration:underline; cursor:pointer;" title="located on server in sites/-siteurl-/themes/lambda/css/global.css">global.css</span> file or in <a href="/admin/config/development/css-injector" title="Add styles to CSS">CSS Injector settings</a>.</strong> Defaults to <strong>blue-to-tan</strong>.</p><p><strong>Already available classes include:</strong></p><p>Gradients:</p><ul><li>blue-to-tan</li><li>tan-to-blue</li><li>red-to-tan</li><li>tan-to-red</li><li>tan-to-white</li><li>white-to-tan</li><li>lighter-to-tan</li><li>tan-to-lighter</li></ul><p>Solid colors:</p><ul><li>white-bkg</li><li>uablue-bkg</li><li>uared-bkg</li><li>charcoal-bkg</li><li>tucson-tan1-bkg</li><li>tucson-tan2-bkg</li><li>golden-sun-bkg</li><li>town-brown-bkg</li><li>sunset-violet-bkg</li><li>rose-violet-bkg</li><li>squash-blossom-bkg</li><li>succulent-green-bkg</li><li>tan-bkg</li><li>lighter-tan-bkg</li><li>no-bkg</li></ul>')
  );
  
}