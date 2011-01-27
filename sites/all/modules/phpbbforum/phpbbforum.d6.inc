<?php
// $Id
/**
 * @file
 * Drupal 6 compatible functions
 * @copyright  Copyright (C) 2007-2009 Vadim G.B. (http://vgb.org.ru)
 * @version    $Id: phpbbforum.d6.inc,v 1.4 2010/11/24 20:53:34 vb Exp $
 */

function phpbbforum_help($path, $arg) {
  $output = '';
  switch ($path) {
    case 'admin/help#phpbbforum':
      break;
    case 'admin/modules#description':
      $output .= t('This module provides integration with phpBB3 Forum.');
      break;
  }
  return $output;
}

/*
function phpbbforum_access($op, $node, $account) {

  if ($op == 'create') {
    return user_access('create phpBB forum topics', $account);
  }

  if ($op == 'update' || $op == 'delete') {
    if (user_access('edit own forum topics', $account) && ($account->uid == $node->uid)) {
      return TRUE;
    }
  }

  if ($op == 'view') {
    return user_access('access content');
  }
}

*/

function phpbbforum_menu() { // 6.0

  $items = array();

  if (variable_get("phpbbforum_page_frame", 0) == '1' || variable_get("phpbbforum_page_frame", 0) == '2') {
    $items['phpbbforum'] = array(
      //'title' => 'phpBB Forum',
      'page callback' => 'phpbbforum_page',
      'access callback' => 'user_access',
      'access arguments' => array('access phpBB forum'),
      'type' => MENU_SUGGESTED_ITEM,
      //'type' => MENU_NORMAL_ITEM,
    );
  }

  $items['admin/settings/phpbbforum'] = array(
    'title' => t('phpBBforum settings'),
    'description' => t('Change phpBBforum integration settings'),
    'page callback' => 'drupal_get_form',
    'page arguments' => array('phpbbforum_admin_settings'),
    'access arguments' => array('administer site configuration'),
    'file' => 'phpbbforum.admin.inc',
    'type' => MENU_NORMAL_ITEM
  );

  $items['phpbbforum/discuss/new/%'] = array(
    'page callback' => 'phpbbforum_discuss_new',
    'page arguments' => array(3),
    'access arguments' => array('create phpBB forum topics'),
    'type' => MENU_CALLBACK,
  );
  $items['phpbbforum/discuss/create/%'] = array(
    'page callback' => 'drupal_get_form',
    'page arguments' => array('phpbbforum_discuss_create_form', 3),
    'access arguments' => array('create phpBB forum topics'),
    'type' => MENU_CALLBACK,
  );
  /*
  $items['phpbbforum/discuss/autocomplete'] = array(
    'title' => 'Autocomplete forum topics',
    'page callback' => 'phpbbforum_discuss_autocomplete',
    'access arguments' => array('access content'),
    'type' => MENU_CALLBACK,
    'file' => 'phpbbforum_discuss.autocomplete.inc',
  );
   *
   */
  return $items;
}

/**
 * Implementation of hook_form_alter().
 */
function phpbbforum_form_alter(&$form, $form_state, $form_id) { // 6.x
  
  if ($form_id == 'user_login_block' || $form_id == 'user_login') {
    if (!_phpbbforum_settings())
      return;
    $form['#validate'] = phpbbforum_login_validators($form);
    phpbb_api_session_begin();
    // Get forum URL
    global $phpbb_config;
    $phpbb_url = $phpbb_config['forum_url'];
    $phpbb_master = variable_get('phpbbforum_master', 1);
    if ($phpbb_master && !empty($phpbb_url)) {
      global $site_base_url, $site_forum_url, $phpEx, $_phpbb_integration_mode;
      if ($_phpbb_integration_mode == 1 || $_phpbb_integration_mode == 2) {
        $phpbb_url = $site_forum_url;
      }
      $new_account = append_sid("{$phpbb_url}/ucp.$phpEx", 'mode=register');
      $new_password = append_sid("{$phpbb_url}/ucp.$phpEx", 'mode=sendpassword');
      $items = array();
      if (variable_get('user_register', 1)) {
        $items[] = l(t('Create new account'), $new_account, array('title' => t('Create a new user account.')));
      }
      $items[] = l(t('Request new password'), $new_password, array('title' => t('Request new password via e-mail.')));
      $form['links'] = array('#value' => theme('item_list', $items));
    }
  }
  elseif ($form_id == 'node_type_form' && isset($form['identity']['type'])) {
    // Alter the node type's configuration form to add our setting. We don't
    // need to worry about saving this value back to the variable, the form
    // we're altering will do it for us.
    if (variable_get('phpbbforum_submission', true)) {

      $form['phpbbforum'] = array(
        '#type'  => 'fieldset',
        '#title' => t('phpBBforum submission settings'),
        '#collapsible' => TRUE,
        '#collapsed' => TRUE,
      );

      $form['phpbbforum']['phpbbforum_submission'] = array(
        '#type' => 'radios',
        '#title' => t('Drupal to phpBB submission'),
        '#default_value' => variable_get('phpbbforum_submission_'. $form['#node_type']->type, 0),
        '#options' => array(0 => t('Disabled'), 1 => t('Enabled')),
        '#description' => t('Should this node be able to submit a post to the phpBB forum?'),
      );

      $form['phpbbforum']['phpbbforum_submission_teaser'] = array(
        '#type' => 'radios',
        '#title' => t('Create a post from teaser or full node content'),
        '#default_value' => variable_get('phpbbforum_submission_teaser_'. $form['#node_type']->type, 1),
        '#options' => array(0 => t('Full node content'), 1 => t('Teaser'), 2 => t('No node content. Back link only, if enabled.')),
        '#description' => t('Create a post to the phpBB forum from teaser or full node content?'),
      );

      $form['phpbbforum']['phpbbforum_backlink_default'] = array(
        '#type' => 'checkbox',
        '#title' => t("Add back link"),
        '#default_value' => variable_get('phpbbforum_backlink_default_'. $form['#node_type']->type, 1),
        '#description' => t("Default back link setting")
      );

      $form['phpbbforum']['phpbbforum_commentlink_default'] = array(
        '#type' => 'checkbox',
        '#title' => t("Add comment link"),
        '#default_value' => variable_get('phpbbforum_commentlink_default_'. $form['#node_type']->type, 1),
        '#description' => t("Default comment link setting")
      );
   
      $form['phpbbforum']['phpbbforum_commentlink_text_nocomments'] = array(
        '#type' => 'textfield',
        '#title' => t("Link text when no comments are posted"),
        '#default_value' => variable_get('phpbbforum_commentlink_text_nocomments_'. $form['#node_type']->type, variable_get('phpbbforum_commentlink_text_nocomments', t('Discuss'))),
      );

      $form['phpbbforum']['phpbbforum_commentlink_text_comments'] = array(
        '#type' => 'textfield',
        '#title' => t("Link text when one or more comments are posted"),
        '#default_value' => variable_get('phpbbforum_commentlink_text_comments_'. $form['#node_type']->type, variable_get('phpbbforum_commentlink_text_comments', t('Discuss'))),
      );

      $form['phpbbforum']['phpbbforum_topic_backlink_text'] = array(
        '#type' => 'textfield',
        '#title' => t("Drupal node backlink text in phpBB topic first post"),
        '#default_value' => variable_get('phpbbforum_topic_backlink_text_'. $form['#node_type']->type, variable_get('phpbbforum_topic_backlink_text', t('Read'))),
      );

      $form['phpbbforum']['phpbbforum_submission_forum_ids_exclude'] = array(
        '#type' => 'radios',
        '#title' => t('Include or exclude phpBB forum ids in the list'),
        '#default_value' => variable_get('phpbbforum_submission_forum_ids_exclude_'. $form['#node_type']->type, 1),
        '#options' => array(0 => t('Include'), 1 => t('Exclude')),
        //'#description' => t('Include or exclude phpBB forum ids in the list'),
      );

      $form['phpbbforum']['phpbbforum_submission_forum_ids_list'] = array(
        '#type' => 'textfield',
        '#title' => t('phpBB forum id list'),
        '#default_value' => variable_get('phpbbforum_submission_forum_ids_list_'. $form['#node_type']->type, ''),
        '#description' => t('Enter phpBB forum id list, for example, 2,5,6,12'),
      );

      $form['phpbbforum']['phpbbforum_comments'] = array(
        '#type'  => 'fieldset',
        '#title' => t('Display phpBBforum comments'),
        '#collapsible' => TRUE,
        '#collapsed' => FALSE,
      );

      $form['phpbbforum']['phpbbforum_comments']['phpbbforum_comment_location'] = array(
        '#type' => 'checkbox',
        '#title' => t("Display phpBB topic comments below the post"),
        '#default_value' => variable_get('phpbbforum_comment_location_'. $form['#node_type']->type, 0),
        //'#description' => t("Default setting")variable_get('phpbbforum_comment_drupal_'. $node->type, 0)
      );

      $form['phpbbforum']['phpbbforum_comments']['phpbbforum_comment_drupal'] = array(
        '#type' => 'checkbox',
        '#title' => t("Display both Drupal and phpBB comments"),
        '#default_value' => variable_get('phpbbforum_comment_drupal_'. $form['#node_type']->type, 0),
      );

      $form['phpbbforum']['phpbbforum_comments']['phpbbforum_submission_view_teaser'] = array(
        '#type' => 'checkbox',
        '#title' => t('View phpBB comments in teaser'),
        '#default_value' => variable_get('phpbbforum_submission_view_teaser_'. $form['#node_type']->type, 0),
        //'#description' => t('View phpBB comments in teaser or in full node content?'),
      );
      $form['phpbbforum']['phpbbforum_comments']['phpbbforum_submission_view_body'] = array(
        '#type' => 'checkbox',
        '#title' => t('View phpBB comments in full node'),
        '#default_value' => variable_get('phpbbforum_submission_view_body_'. $form['#node_type']->type, 1),
      );
      $form['phpbbforum']['phpbbforum_comments']['phpbbforum_submission_append_to_content'] = array(
        '#type' => 'checkbox',
        '#title' => t('Append phpBB comments to node'),
        '#default_value' => variable_get('phpbbforum_submission_append_to_content_'. $form['#node_type']->type, 0),
        '#description' => t('Advanced. Direct append of phpBB comments to node instead of using variable $phpbbforum_comments in') .' node-'. $form['#node_type']->type .'.tpl.php',
      );

    }
  }
  // If the type and node field are set this may be a node edit form.
  elseif (isset($form['type']) && isset($form['#node']) && $form['type']['#value'] .'_node_form' == $form_id) {

    if (variable_get('phpbbforum_submission', true) && variable_get('phpbbforum_submission_'. $form['type']['#value'], 0) && user_access('create phpBB forum topics')) {

      if (!_phpbbforum_settings())
        return;
      
      $node = $form['#node'];

      phpbb_api_session_begin();

      global $phpbb_user;

      $defaults = _phpbbforum_get_defaults($node->nid, $node->type);

  	  $form['phpbbforum'] = array(
  	    '#type' => 'fieldset',
  	    '#title' => t('phpBBforum submission settings'),
  	    '#collapsible' => TRUE,
  	    '#collapsed' => TRUE,
  	  );

      $form['phpbbforum']['phpbbforum_forumid'] = array(
        '#type' => 'select',
        '#title' => t('Submit this node to'),
        '#default_value' => $defaults['forum'],
        '#description' => t('If you choose a forum, a phpbb topic with the node\'s content will be created in the specified forum.'),
      	'#options' => _phpbbforum_get_forums($phpbb_user->data['user_id'],
                        variable_get('phpbbforum_submission_forum_ids_list_'. $node->type, ''),
                        variable_get('phpbbforum_submission_forum_ids_exclude_'. $node->type, 1)
                      ),
      );

      $form['phpbbforum']['phpbbforum_submission_teaser'] = array(
        '#type' => 'radios',
        '#title' => t('Create a post from teaser or full node content'),
        '#default_value' => variable_get('phpbbforum_submission_teaser_'. $node->type, 1),
        '#options' => array(0 => t('Full node content'), 1 => t('Teaser'), 2 => t('No node content. Back link only, if enabled.')),
        '#description' => t('Create a post to the phpBB forum from teaser or full node content?'),
      );

      $form['phpbbforum']['phpbbforum_commentlink'] = array(
        '#type' => 'checkbox',
        '#title' => t('Add comment link'),
        '#default_value' => $defaults['linked'],
      );

    }
  }
  
  return $form;
}


function phpbbforum_login_validators($form) {

  //if (PHPBB_MODULE_DEBUG)
  //return $form['#validate'];
  if (is_array($form['#validate'])) {
    $key = array_search('user_login_authenticate_validate', $form['#validate']);
    if ($key !== false)
      unset($form['#validate'][$key]);
    $key = array_search('user_login_name_validate', $form['#validate']);
    if ($key !== false)
      unset($form['#validate'][$key]);
    $key = array_search('user_login_final_validate', $form['#validate']);
    if ($key !== false)
      unset($form['#validate'][$key]);
    $validators = $form['#validate'];
    $form_validate = true;
  }
  else {
    $validators = array();
    $form_validate = false;
  }

  if (!$form_validate || (!module_exists('ldapauth') && !module_exists('logintoboggan'))) {
    $validators[] = 'user_login_name_validate';
  }
  if (!$form_validate || !module_exists('ldapauth')) {
    $validators[] = 'user_login_authenticate_validate';
  }
  $validators[] = 'phpbbforum_login_authenticate_validate';
  $validators[] = 'user_login_final_validate';

  return $validators;
}

/**
 * A validate handler on the login form. Check supplied username/password
 * against local users table. If successful, sets the global $user object.
 */
function phpbbforum_login_authenticate_validate($form, &$form_state) {
  global $user;
  if (!$user->uid) {
    $pass = trim($form_state['values']['pass']);
    if (!empty($pass)) {
      phpbbforum_external_login_register($form_state['values']['name'], $pass);
      if ($user->uid) {
        user_authenticate_finalize($form_state['values']);
      }
    }
  }
}

if (module_exists('ldapauth')) {
  function phpbbforum_ldapauth_login_validate($form, &$form_state) {
    global $user;
    if (!$user->uid) {
      $pass = trim($form_state['values']['pass']);
      if (!empty($pass)) {
        ldapauth_login_validate($form, $form_state);
      }
    }
  }
}
