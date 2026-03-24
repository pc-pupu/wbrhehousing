<?php
// Done by Subham dt.05-06-2025
function ddo_add_form($form, &$form_state) {
    $query1 = db_select('housing_district', 'hds')->fields('hds', array('district_code', 'district_name'))->orderBy('district_code','asc')->execute();

    $optns1 = array();
    foreach ($query1 as $data1) {
        $optns1[$data1->district_code] = $data1->district_name;
    }

    $form['ddo_info'] = array(
        '#prefix' => '<div class="row">',
        '#suffix' => '</div>',
    );

    $form['ddo_info']['ddo_code'] = array(
        '#title' => t('DDO Code'),
        '#type' => 'textfield',
        '#prefix' => '<div class="col-md-6"><div class="form-floating">',
        '#suffix' => '</div></div>',
        '#default_value' => '',
        '#attributes' => array('oninput'=>"this.value=this.value.toUpperCase()", 'class'=>array('form-control')),
        '#required' => TRUE
    );

    $form['ddo_info']['ddo_designation'] = array(
        '#title' => t('DDO Designation'),
        '#type' => 'textfield',
        '#prefix' => '<div class="col-md-6"><div class="form-floating">',
        '#suffix' => '</div></div>',
        '#default_value' => '',
        '#attributes' => array('class'=>array('form-control')),
        '#required' => TRUE
    );

    $form['ddo_info']['select_district'] = array(
        '#title' => t('Select District'),
        '#type' => 'select',
        '#options' => $optns1,
        '#attributes' => array('class' => array('form-select')),
        '#required' => TRUE,
        '#prefix' => '<div class="col-md-6"><div class="form-floating">',
        '#suffix' => '</div></div>',
        '#ajax' => array(
            'event' => 'change',
            'callback' => '_reload_treasury_info_callback',
        ),
    );

    $district =  isset($form_state['values']['select_district']) && !empty($form_state['values']['select_district']) ? $form_state['values']['select_district'] : 0;

    $treasury_options = array();

    if ($district != 0) {
        $treasury_options = fetch_respective_treasury($district);
        $treasury_options = array('' => '- Select -') + $treasury_options;
    } else {
        $treasury_options = array('' => t('- Select district first -'));
    }

    $form['ddo_info']['select_respective_treasury'] = array(
        '#title' => t('Respective Treasury'),
        '#type' => 'select',
        '#options' => $treasury_options,
        '#required' => TRUE,
        '#prefix' => '<div class="col-md-6" id="treasury_replace"><div class="form-floating">',
        '#suffix' => '</div></div>',
        '#attributes' => array('class' => array('form-select')),
        '#ajax' => array(
            'event' => 'change',
            'callback' => '_reload_treasury_info_callback',
            'wrapper' => 'treasury_replace',
        ),
        '#default_value' => isset($form_state['values']['select_respective_treasury']) ? $form_state['values']['select_respective_treasury'] : '',
    );

    $form['ddo_info']['is_active_radio'] = array(
        '#title' => t('DDO Activation Status'),
        '#type' => 'radios',
        '#options' => array(
            'Y' => t('Active'),
            'N' => t('Inactive')
        ),
        '#prefix' => '<div class="col-md-4"><div class="form-floating">',
        '#suffix' => '</div></div>',
        '#required' => TRUE,
        '#default_value' => 'Y'
    );

    $form['ddo_info']['ddo_address'] = array(
        '#title' => t('DDO Address'),
        '#type' => 'textarea',
        '#prefix' => '<div class="col-md-12"><div class="form-floating">',
        '#suffix' => '</div></div></div>',
        '#default_value' => '',
        '#attributes' => array('class'=>array('form-control'), 'style' => 'height:80px;'),
        // '#required' => TRUE
    );

    $form['submit_cen'] = array(
        '#prefix' => '<div class="row"><div class="col-md-12 text-center">',
        '#suffix' => '</div></div>'
    );

    $form['submit_cen']['submit'] = array(
        '#type' => 'submit',
        '#value' => t('Save'),
        '#attributes' => array('class' => array('btn bg-primary btn-sm px-5 rounded-pill text-white fw-bolder')),
        '#submit' => array('ddo_add_submit')
    );

    return $form;
}

function ddo_add_submit($form, &$form_state) {
    $values = $form_state['values'];
    $form_data = array();
    
    $form_data['ddo_designation']           =   trim($values['ddo_designation']);
    $form_data['ddo_address']               =   trim($values['ddo_address']) !== '' ? trim($values['ddo_address']) : NULL;
    $form_data['ddo_code']                  =   trim($values['ddo_code']);
    $form_data['is_active']                 =   trim($values['is_active_radio']);
    $form_data['treasury_id']               =   trim($values['select_respective_treasury']);
    $form_data['district_code']             =   trim($values['select_district']);

    // print_r($form_data);die;
    $duplicate_ddo_exists = db_select('housing_ddo', 'hd')
        ->fields('hd')
        ->condition('ddo_code', $form_data['ddo_code'])
        ->execute()
        ->fetchObject();

    if ($duplicate_ddo_exists) {
        drupal_set_message(t('Duplicate DDO Code. Please change DDO Code.'), 'error');
    } else {
        db_insert('housing_ddo')
        ->fields($form_data)
        ->execute();
        drupal_set_message(t('Record added successfully.'));
    }
    drupal_goto('ddo-add');
}

function _reload_treasury_info_callback($form, &$form_state) {
    $commands = array();
    
    $commands[] = ajax_command_replace("#treasury_replace", drupal_render($form['ddo_info']['select_respective_treasury']));

    return array('#type' => 'ajax', '#commands' => $commands);
}