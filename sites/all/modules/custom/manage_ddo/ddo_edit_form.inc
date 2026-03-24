<?php
// Done by Subham dt.05-06-2025
function ddo_edit_form($form, &$form_state, $ddo_id_enc = '') {
    $ddo_id = decrypt_url($ddo_id_enc);
    $query = db_select('housing_ddo', 'hd')
        ->fields('hd', array('ddo_id', 'ddo_designation', 'ddo_address', 'ddo_code', 'is_active', 'treasury_id', 'district_code'))
        ->condition('hd.ddo_id', $ddo_id);

    $query->innerJoin('housing_treasury', 'ht', 'ht.treasury_id = hd.treasury_id');
    $query->innerJoin('housing_district', 'hds', 'hds.district_code = hd.district_code');

    $query->addField('ht', 'treasury_name');
    $query->addField('hds', 'district_name');

    $result = $query->execute()->fetchAssoc();

    if (!empty($result)) {
    // print_r($result);die;
        $optns1 = [];
        $district_query = db_select('housing_district', 'd')
            ->fields('d', ['district_code', 'district_name'])
            ->execute();
        foreach ($district_query as $row) {
            $optns1[$row->district_code] = $row->district_name;
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
            '#default_value' => $result['ddo_code'],
            '#attributes' => ['oninput' => "this.value=this.value.toUpperCase()", 'class' => ['form-control']],
            '#required' => TRUE,
        );

        $form['ddo_info']['ddo_designation'] = array(
            '#title' => t('DDO Designation'),
            '#type' => 'textfield',
            '#prefix' => '<div class="col-md-6"><div class="form-floating">',
            '#suffix' => '</div></div>',
            '#default_value' => $result['ddo_designation'],
            '#attributes' => ['class' => ['form-control']],
            '#required' => TRUE,
        );

        $form['ddo_info']['select_district'] = array(
            '#title' => t('Select District'),
            '#type' => 'select',
            '#options' => $optns1,
            '#default_value' => $result['district_code'],
            '#attributes' => array('class' => array('form-select')),
            '#required' => TRUE,
            '#prefix' => '<div class="col-md-6"><div class="form-floating">',
            '#suffix' => '</div></div>',
            '#ajax' => array(
                'event' => 'change',
                'callback' => '_reload_edited_treasury_info_callback',
            ),
        );

        $district =  isset($form_state['values']['select_district']) && !empty($form_state['values']['select_district']) ? $form_state['values']['select_district'] : $result['district_code'];

        $treasury_options = array();

        if ($district != 0) {
            $treasury_options = fetch_respective_treasury($district);
        } else {
            $treasury_options = array('' => t('- Select district first -'));
        }

        $form['ddo_info']['select_respective_treasury'] = array(
            '#title' => t('Respective Treasury'),
            '#type' => 'select',
            '#default_value' => $result['treasury_id'],
            '#options' => $treasury_options,
            '#required' => TRUE,
            '#prefix' => '<div class="col-md-6" id="treasury_replace"><div class="form-floating">',
            '#suffix' => '</div></div>',
            '#attributes' => array('class' => array('form-select')),
            '#ajax' => array(
                'event' => 'change',
                'callback' => '_reload_edited_treasury_info_callback',
                'wrapper' => 'treasury_replace',
            ),
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
            '#default_value' => $result['is_active'],
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

        $form['hidden_textbx_ddo_id'] = array(
            '#type' => 'hidden',
            '#default_value' => $result['ddo_id']
        );

        $form['submit_cen'] = array(
            '#prefix' => '<div class="row"><div class="col-md-12 text-center">',
            '#suffix' => '</div></div>'
        );

        $form['submit_cen']['submit'] = array(
            '#type' => 'submit',
            '#value' => t('Save'),
            '#attributes' => array('class' => array('btn bg-primary btn-sm px-5 rounded-pill text-white fw-bolder')),
            '#submit' => array('ddo_edit_submit')
        );

        return $form;
    }
}

function ddo_edit_submit($from, &$form_state) {
    $values = $form_state['values'];
    $form_data = array();

    $form_data['ddo_designation']           =  trim($values['ddo_designation']);
    $form_data['ddo_address']               =  trim($values['ddo_address']) !== '' ? trim($values['ddo_address']) : NULL;
    $form_data['ddo_code']                  =  trim($values['ddo_code']);
    $form_data['is_active']                 =  trim($values['is_active_radio']);
    $form_data['treasury_id']               =  trim($values['select_respective_treasury']);
    $form_data['district_code']             =  trim($values['select_district']);

    // print_r($values);die;

    db_update('housing_ddo')
        ->fields($form_data)
        ->condition('ddo_id', $values['hidden_textbx_ddo_id'])
        ->execute();
    drupal_set_message(t('Record edited successfully.'));
    drupal_goto('ddo-list');
}

function _reload_edited_treasury_info_callback($form, &$form_state) {
    $commands = array();
    
    $commands[] = ajax_command_replace("#treasury_replace", drupal_render($form['ddo_info']['select_respective_treasury']));

    return array('#type' => 'ajax', '#commands' => $commands);
}