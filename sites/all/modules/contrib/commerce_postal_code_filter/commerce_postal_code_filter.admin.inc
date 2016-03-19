<?php

/**
 * @file
 * Contains form callback and submit function for the postal code admin form
 */

/**
 * Form callback for the postal code filter admin form.
 */
function commerce_postal_code_filter_configure($form, &$form_state) {

  // Necessary for country_get_list().
  require_once DRUPAL_ROOT . '/includes/locale.inc';
  $countries = country_get_list();

  $config = variable_get('commerce_postal_code_filter_config', array());

  // If the form is being rebuilt with an existing $form_state use that instead.
  if (!empty($form_state['values']['countries'])) {
    $enabled_countries = $form_state['values']['countries'];
  }
  elseif (isset($config['countries'])) {
    $enabled_countries = $config['countries'];
  }
  else {
    $enabled_countries = array();
  }

  $form['countries'] = array(
    '#title' => t('Enable postcode filtering for these countries'),
    '#type' => 'select',
    '#multiple' => TRUE,
    '#options' => $countries,
    '#default_value' => $enabled_countries,
    '#ajax' => array(
      'callback' => 'commerce_postal_code_filter_configure_ajax_callback',
      'wrapper' => 'country-filters',
      'method' => 'replace',
      'effect' => 'fade',
    ),
  );

  // Per country filter settings.
  $form['country_filters'] = array(
    '#prefix' => '<div id="country-filters"><h2>' . t('Filter settings') . '</h2>',
    '#suffix' => '</div>',
    '#type' => 'vertical_tabs',
    '#tree' => TRUE,
  );


  foreach ($enabled_countries as $country_code) {

    $form['country_filters'][$country_code] = array(
      '#type' => 'fieldset',
      '#title' => $countries[$country_code],
      '#tree' => TRUE,
    );

    $form['country_filters'][$country_code]['postal_codes'] = array(
      '#type' => 'textarea',
      '#title' => t('Postal code filter'),
      '#description' => t('Postal codes to be filtered, comma seperated. Partial postal codes are supported, e.g. PL, EX5'),
      // Add spaces between the commas when displayed in the form.
      // These are removed again on submit.
      '#default_value' => empty($config['country_filters'][$country_code]['postal_codes']) ? '' : str_replace(',', ', ', $config['country_filters'][$country_code]['postal_codes']),
    );

    $form['country_filters'][$country_code]['filter_mode'] = array(
      '#type' => 'radios',
      '#title' => t('Filter mode'),
      '#required' => TRUE,
      '#description' => t('Whether to "blacklist" or "whitelist" postcodes. If blacklisted, postcodes in the above filter will be prevented from completing checkout. If whitelisted, <em>only</em> postcodes that are in the above filter will be allowed to checkout'),
      '#default_value' => empty($config['country_filters'][$country_code]['filter_mode']) ? 'blacklist' : $config['country_filters'][$country_code]['filter_mode'],
      '#options' => array(
        'blacklist' => t('Blacklist'),
        'whitelist' => t('Whitelist'),
      ),
    );

    $form['country_filters'][$country_code]['filter_addresses'] = array(
      '#type' => 'checkboxes',
      '#title' => t('Addresses to filter'),
      '#description' => t('Which addresses should be filtered at checkout. Shipping requires commerce_shipping-2.x'),
      '#default_value' => empty($config['country_filters'][$country_code]['filter_addresses']) ? array() : $config['country_filters'][$country_code]['filter_addresses'],
      '#options' => array(
        'customer_profile_billing' => t('Billing'),
        'customer_profile_shipping' => t('Shipping'),
      ),
    );

    // Disable shipping checkbox if the shipping module isn't enabled.
    $form['country_filters'][$country_code]['filter_addresses']['customer_profile_shipping']['#disabled'] = !module_exists('commerce_shipping');

  }

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save postal code filter options'),
  );

  return $form;

}

/**
 * Ajax callback for admin form.
 */
function commerce_postal_code_filter_configure_ajax_callback($form, $form_state) {
  return $form['country_filters'];
}

/**
 * Submit callback for the postal code filter admin form.
 */
function commerce_postal_code_filter_configure_submit($form, &$form_state) {

  // Remove "active tab" value from form state.
  unset($form_state['values']['country_filters']['country_filters__active_tab']);

  foreach ($form_state['values']['country_filters'] as &$country_filter) {

    // Fetch the filter list from the form state and force it to be uppercase.
    $country_filter['postal_codes'] = drupal_strtoupper($country_filter['postal_codes']);

    // Remove all whitespace.
    $country_filter['postal_codes'] = preg_replace('/\s+/', '', $country_filter['postal_codes']);

    // Trim any trailing commas from the list of postcodes
    $country_filter['postal_codes'] = trim($country_filter['postal_codes'], ',');

  }

  form_state_values_clean($form_state);
  variable_set('commerce_postal_code_filter_config', $form_state['values']);

  drupal_set_message(t('The configuration options have been saved.'));

}
