<?php
// $Id: admin_menu.map.inc,v 1.1 2010/03/17 20:49:40 sun Exp $

/**
 * @file
 * Implements hook_admin_menu_map() on behalf of core modules.
 *
 * @todo Replace all/most of those API functions with direct DB queries;
 *   we only need the menu arguments (keys), not fully loaded objects.
 */

/**
 * Implements hook_admin_menu_map() on behalf of Filter module.
 */
function filter_admin_menu_map() {
  $map['admin/config/content/formats/%filter_format'] = array(
    'parent' => 'admin/config/content/formats',
    'hide' => 'admin/config/content/formats/list',
    'arguments' => array(
      array('%filter_format' => array_keys(filter_formats())),
    ),
  );
  return $map;
}

/**
 * Implements hook_admin_menu_map() on behalf of Menu module.
 */
function menu_admin_menu_map() {
  $map['admin/structure/menu/manage/%menu'] = array(
    'parent' => 'admin/structure/menu',
    'hide' => 'admin/structure/menu/list',
    'arguments' => array(
      array('%menu' => array_keys(menu_get_menus())),
    ),
  );
  return $map;
}

/**
 * Implements hook_admin_menu_map() on behalf of Node module.
 */
function node_admin_menu_map() {
  $map['admin/structure/types/manage/%node_type'] = array(
    'parent' => 'admin/structure/types',
    'hide' => 'admin/structure/types/list',
    'arguments' => array(
      array('%node_type' => array_keys(node_type_get_types())),
    ),
  );
  return $map;
}

/**
 * Implements hook_admin_menu_map() on behalf of Field UI module.
 */
function field_ui_admin_menu_map() {
  // @todo D7: Taxonomy still uses vid instead of machine name in administrative
  //   paths.
  if (module_exists('taxonomy')) {
    $vocabularies = taxonomy_vocabulary_get_names();
  }

  $map = array();
  foreach (entity_get_info() as $obj_type => $info) {
    foreach ($info['bundles'] as $bundle_name => $bundle_info) {
      if (isset($bundle_info['admin'])) {
        $arguments = array();
        switch ($obj_type) {
          case 'node':
            $fields = array();
            foreach (field_info_instances($obj_type, $bundle_name) as $field) {
              $fields[] = $field['field_name'];
            }
            $arguments = array(
              '%node_type' => array($bundle_name),
              '%field_ui_menu' => $fields,
            );
            break;

          case 'taxonomy_term':
            $fields = array();
            foreach (field_info_instances($obj_type, $bundle_name) as $field) {
              $fields[] = $field['field_name'];
            }
            // Map machine_name to vid.
            $vid = $vocabularies[$bundle_name]->vid;
            $arguments = array(
              '%taxonomy_vocabulary' => array($vid),
              '%field_ui_menu' => $fields,
            );
            break;

          case 'user':
            $arguments = array(
              '%field_ui_menu' => array_keys(field_info_fields('user')),
            );
            break;
        }
        if (!empty($arguments)) {
          $path = $bundle_info['admin']['path'];
          $map["$path/fields/%field_ui_menu"]['parent'] = "$path/fields";
          $map["$path/fields/%field_ui_menu"]['arguments'][] = $arguments;
        }
      }
    }
  }
  return $map;
}

/**
 * Implements hook_admin_menu_map() on behalf of Taxonomy module.
 */
function taxonomy_admin_menu_map() {
  $map['admin/structure/taxonomy/%taxonomy_vocabulary'] = array(
    'parent' => 'admin/structure/taxonomy',
    'hide' => 'admin/structure/taxonomy/list',
    'arguments' => array(
      array('%taxonomy_vocabulary' => array_keys(taxonomy_get_vocabularies())),
    ),
  );
  return $map;
}

/**
 * Implements hook_admin_menu_map() on behalf of Views UI module.
 */
function views_ui_admin_menu_map() {
  // @todo Requires patch to views_ui.
  $map['admin/structure/views/edit/%views_ui_cache'] = array(
    'parent' => 'admin/structure/views',
    'hide' => 'admin/structure/views/list',
    'arguments' => array(
      array('%views_ui_cache' => array_keys(views_get_all_views())),
    ),
  );
  return $map;
}

