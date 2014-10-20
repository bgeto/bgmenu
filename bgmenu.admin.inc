<?php

/**
 * @file
 * Administrative page callbacks for the bgmenu module.
 */

/**
 * bgmenu administration form.
 */


function bgmenu_admin_settings($form, &$form_state) {
  
  drupal_set_title('Bg Menu configuration');
  // List roles
  $liste_roles = user_roles();

  $form['roles'] = array(
    '#type' => 'fieldset',
    '#title' => t('Roles'),
    '#weight' => 0,
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['roles']['bgmenu_roles'] = array(
    '#type' => 'checkboxes',
    '#title' => t('List of roles can see the menu'),
    '#options' => drupal_map_assoc($liste_roles),
    '#default_value' => variable_get('bgmenu_roles', array()),
    '#description' => t('List of roles can see the menu.'),
  );

  $contenttypes_array = array();
  foreach (node_type_get_types() as $key => $value) {
    $contenttypes_array[] = $value->type;
  }

  $form['cts'] = array(
    '#type' => 'fieldset',
    '#title' => t('Content types'),
    '#weight' => 0,
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['cts']['bgmenu_cts'] = array(
    '#type' => 'checkboxes',
    '#title' => t('List of creatable content types to display'),
    '#options' => drupal_map_assoc($contenttypes_array),
    '#default_value' => variable_get('bgmenu_cts', array()),
    '#description' => t('List of creatable content types to display.'),
  );

  function get_list_taxos_vocabularies() {
    $vocabulary = taxonomy_get_vocabularies();
    $checklist_vocab_array = array();
    foreach ($vocabulary as $item) {
      $checklist_vocab_array[] = $item->machine_name;
    }
    return $checklist_vocab_array;
  }

  $form['taxos'] = array(
    '#type' => 'fieldset',
    '#title' => t('Taxonomies'),
    '#weight' => 1,
    '#collapsible' => TRUE, 
    '#collapsed' => TRUE,
  );
  $form['taxos']['bgmenu_taxos'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Display a list of taxonomies'),
    '#options' => drupal_map_assoc(get_list_taxos_vocabularies()),
    '#default_value' => variable_get('bgmenu_taxos', array()),
    '#description' => t('Display a list of taxonomies.'),
  );

  $form['menus'] = array(
    '#type' => 'fieldset',
    '#title' => t('List menus'),
    '#weight' => 2,
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['menus']['bgmenu_menus'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Display a list of menu'),
    '#options' => drupal_map_assoc(menu_get_names()),
    '#default_value' => variable_get('bgmenu_menus', array()),
    '#description' => t('Display a list of menu.'),
  );

  $form['menu1_links'] = array(
    '#markup' => l(
      t('Go to the first menu'), 
      'admin/structure/menu/manage/bgmenu-menu1' ,
      array(
        'attributes' => array(
          'class' => 'bgmenu-link',
        ),
        'absolute' => TRUE,
      )
    ),
    '#weight' => 5,
  );

  $form['menu2_links'] = array(
    '#markup' => l(
      t('Go to the second menu'), 
      'admin/structure/menu/manage/bgmenu-menu2' ,
      array(
        'attributes' => array(
          'class' => 'bgmenu-link',
        ),
        'absolute' => TRUE,
      )
    ),
    '#weight' => 6,
  );

  $form['bgmenu_logo'] = array(
    '#type' => 'managed_file',
    '#title' => t('Logo customs'),
    '#title_display' => 'invisible',
    '#default_value' => variable_get('bgmenu_logo'),
    '#size' => 60,
    '#theme_wrappers' => array(),
    '#description' => t('Upload a file, allowed extensions: jpg jpeg gif png'),
    '#weight' => 7,
  );

  $form['bgmenu_css'] = array(
    '#type' => 'textarea',
    '#title' => t('Customs css'),
    '#default_value' => variable_get('bgmenu_css', array()),
    '#weight' => 8,
  );

  $form['#submit'][] = 'bgmenu_submit';

  return system_settings_form($form);
}

function bgmenu_submit($form ,&$form_state){
  global $user;
  /*
  $save = file_save_upload('bgmenu_logo', $validators = array());
  if($save) {
    $save = file_move($save, 'public://', FILE_EXISTS_RENAME);
    if($save) {
      variable_set('bgmenu_logo', $save->filename);
      drupal_set_message('Le fichier: <strong>"' . $save->filename . '"</strong> est bien sur le serveur');
    }
  }
  */
  
  // Load the file via file.fid.
  $file = file_load($form_state['values']['bgmenu_logo']);
  // Change status to permanent.
  $file->status = FILE_STATUS_PERMANENT;
  $file = file_move($file, 'public://', FILE_EXISTS_RENAME);
  if($save) {
    file_save($file);
    variable_set('bgmenu_logo', $file->filename);
    // Record that the module (in this example, user module) is using the file. 
    file_usage_add($file, 'user', 'user', $user->uid);
  }
}