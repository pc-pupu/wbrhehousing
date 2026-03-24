<?php
function flat_wise_applicant_details_form($form, &$form_state){
    $rhe_name =  isset($form_state['values']['rhe_name']) && !empty($form_state['values']['rhe_name']) ? $form_state['values']['rhe_name'] : 0;
    $flat_type =  isset($form_state['values']['flat_type']) && !empty($form_state['values']['flat_type']) ? $form_state['values']['flat_type'] : 0;
    $block_name =  isset($form_state['values']['block_name']) && !empty($form_state['values']['block_name']) ? $form_state['values']['block_name'] : 0;

    $form['list_info'] = array(
        '#prefix' => '<div class="row">',
        '#suffix' => '</div>'
    );

    $form['list_info']['rhe_name'] = array(
        '#title' => t('Name of the RHE'),
        '#type' => 'select',
        '#options' => rhe_list_fetch(),
        '#prefix' => '<div class="row"><div class="col-md-4"><div class="form-floating">',
        '#suffix' => '</div></div>',
        '#attributes' => array('class'=>array('form-select')),
        '#ajax' => array(
            'event' => 'change',
            'callback' => '_reload_rhewise_list_view',
            'method' => 'replace',
        ),
    );

    $form['list_info']['flat_type'] = array(
        '#prefix' => '<div class="three" id="flat_type_replace">',
        '#suffix' => '</div>'
    );
        
    if($rhe_name != 0) {
        $form['list_info']['flat_type'] = array(
            '#title' => t('Flat Type'),
            '#type' => 'select',
            '#options' => flat_type_under_rhe($rhe_name),
            '#prefix' => '<div class="col-md-4" id="flat_type_replace"><div class="form-floating">',
            '#suffix' => '</div></div></div>',
            '#attributes' => array('class'=>array('form-select')),
            '#ajax' => array(
                'event' => 'change',
                'callback' => '_reload_rhewise_list_view',
                'method' => 'replace',
            ),
        );
    }

    $form['list_info']['block_name'] = array(
        '#prefix' => '<div class="three" id="block_name_replace">',
        '#suffix' => '</div>'
    );
        
    if($rhe_name != 0 && $flat_type != 0) {
        $form['list_info']['block_name'] = array(
            '#title' => t('Name of the Block'),
            '#type' => 'select',
            '#options' => block_name_under_rhe($rhe_name, $flat_type),
            '#prefix' => '<div class="col-md-4" id="block_name_replace"><div class="form-floating">',
            '#suffix' => '</div></div>',
            '#attributes' => array('class'=>array('form-select')),
            '#ajax' => array(
                'event' => 'change',
                'callback' => '_reload_rhewise_list_view',
                'method' => 'replace',
            ),
        );
    }

    $form['fetch_list'] = array(
        '#prefix' => '<div class="col-md-12" id="flat_list_replace">',
        '#suffix' => '</div>'
    );

    if($rhe_name != 0 && $flat_type != 0 && $block_name != 0) {
        $form['fetch_list'] = array(
            '#type' => 'markup',
            '#markup' => view_flat_occupant_list($rhe_name, $flat_type, $block_name),
            '#weight' => 50,
            '#prefix' => '<div class="col-md-12" id="flat_list_replace">',
            '#suffix' => '</div>'
        );
    }

    return $form;
}

function _reload_rhewise_list_view($form, &$form_state) {
    $commands = array();
    
    $commands[] = ajax_command_replace("#flat_type_replace", drupal_render($form['list_info']['flat_type']));
    $commands[] = ajax_command_replace("#block_name_replace", drupal_render($form['list_info']['block_name']));
    $commands[] = ajax_command_replace("#flat_list_replace", drupal_render($form['fetch_list']));

    return array('#type' => 'ajax', '#commands' => $commands);
}

