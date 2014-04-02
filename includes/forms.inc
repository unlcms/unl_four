<?php

/**
 * Replacement for theme_webform_element().
 */
function unl_four_webform_element($variables)
{
  // Ensure defaults.
  $variables['element'] += array(
    '#title_display' => 'before',
  );

  $element = $variables['element'];

  // All elements using this for display only are given the "display" type.
  if (isset($element['#format']) && $element['#format'] == 'html') {
    $type = 'display';
  } else {
    $type = (isset($element['#type']) && !in_array($element['#type'], array('markup', 'textfield'))) ? $element['#type'] : $element['#webform_component']['type'];
  }
  $parents = str_replace('_', '-', implode('--', array_slice($element['#parents'], 1)));

  $wrapper_classes = array(
    'form-item',
    'webform-component',
    'webform-component-' . $type,
  );
  if (isset($element['#title_display']) && $element['#title_display'] == 'inline') {
    $wrapper_classes[] = 'webform-container-inline';
  }
  $output = '<div class="' . implode(' ', $wrapper_classes) . '" id="webform-component-' . $parents . '">' . "\n";

  // Composite elements consist of more than one HTML form control. These must be grouped by a fieldset.
  // Developers can mark any fieldtype as composite with #composite.
  // radios, checkboxes, and date are marked as composite. In D8, they are so marked in their respective form_process_*().
  if (isset($element['#type']) && in_array($element['#type'], array('radios', 'checkboxes', 'date', 'webform_grid', 'webform_time'), true)) {
    $element['#composite'] = true;
  }

  $composite = false;
  if (isset($element['#composite']) && $element['#composite'] === true) {
    $composite = true;
  }

  if ($composite) {
    $output .= '<fieldset' . drupal_attributes(array('class' => 'fieldset-invisible')) . '>';
  }

  // If #title is not set, we don't display any label or required marker.
  if (!isset($element['#title'])) {
    $element['#title_display'] = 'none';
  }
  if (!isset($element['#children'])) {
    $element['#children'] = '';
  }
  $prefix   = isset($element['#field_prefix']) ? '<span class="field-prefix">' . _webform_filter_xss($element['#field_prefix']) . '</span> ' : '';
  $suffix   = isset($element['#field_suffix']) ? ' <span class="field-suffix">' . _webform_filter_xss($element['#field_suffix']) . '</span>' : '';
  $children = ' ' . $prefix . $element['#children'] . $suffix;

  // Generate the title, either a legend or a label, only if the title will be used
  if (!in_array($element['#title_display'], array('none', 'attribute'))) {
    if ($composite) {
      $attributes = array();
      if ($element['#title_display'] == 'invisible') {
        $attributes['class'] = 'element-invisible';
      }
      $title = '<legend' . drupal_attributes($attributes) . '>' . $element['#title'];
      if (!empty($element['#required'])) {
        $title .= theme('form_required_marker', array('element' => $element));
      }
      $title .= '</legend>';
    } else {
      $title = ' ' . theme('form_element_label', $variables);
    }
  }

  switch ($element['#title_display']) {
    case 'inline':
    case 'before':
    case 'invisible':
      $output .= $title . $children;
      break;

    case 'after':
      $output .= $children . $title;
      break;

    case 'none':
    case 'attribute':
      // Output no label and no required marker, only the children.
      $output .= $children;
      break;
  }
  $output .= "\n";

  if (!empty($element['#description'])) {
    $output .= '<div class="description">' . $element['#description'] . "</div>\n";
  }

  if ($composite) {
    $output .= '</fieldset>';
  }

  $output .= "</div>\n";

  return $output;
}