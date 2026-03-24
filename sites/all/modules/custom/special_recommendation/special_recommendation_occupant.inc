<?php

function special_recommendation_occupant($form, &$form_state, $online_application_id = '') {

    module_load_include('inc', 'user_tagging', 'user_tagging_form.inc');

    // Check if the user has permission to access this page
    if (!user_access('access content')) {
        drupal_access_denied();
        return;
    }

    // Get the online application ID from the URL
    $online_application_id = decrypt_url($online_application_id); 

    if (!$online_application_id) {
        drupal_set_message(t('Invalid online application ID.'), 'error');
        return;
    }

    $form['online_application_id'] = array(
        '#type' => 'hidden',
        '#value' => $online_application_id,
    );

    // RHE info form
    $form['flat_info'] = array(
        '#prefix' => '<div class="row">',
        '#suffix' => '</div>',
    );

    
    $form['personal_info']['allotment_process_date'] = array(
		'#title' => t('Date of Allotment'),
		'#type' => 'textfield',
		'#attributes' => array('id' => 'edit-date-of-birth', 'class'=>array('form-control')),
		'#required' => TRUE,
		'#prefix' => '<div class="col-md-4"><div class="form-floating">',
		'#suffix' => '</div></div>',
	);

    $form['flat_info']['updated_rhe_name'] = array(
        '#title' => t('Name of the RHE'),
        '#type' => 'select',
        '#options' => rhe_list_fetch(),
        '#attributes' => array('class' => array('form-select')),
        '#required' => TRUE,
        '#prefix' => '<div class="col-md-4"><div class="form-floating">',
        '#suffix' => '</div></div>',
        '#ajax' => array(
            'event' => 'change',
            'callback' => '_reload_rhewise_flat_info_callback',
            'method' => 'replace',
        ),
    );

    $rhe_name =  isset($form_state['values']['updated_rhe_name']) && !empty($form_state['values']['updated_rhe_name']) ? $form_state['values']['updated_rhe_name'] : 0;
     
    $form['flat_info']['updated_flat_type'] = array(
        '#prefix' => '<div class="col-md-4" id="flat_type_replace_updated">',
        '#suffix' => '</div>',
    );

    $form['flat_info']['updated_block_name'] = array(
        '#prefix' => '<div class="col-md-6" id="block_name_replace_updated"><div class="form-floating">',
        '#suffix' => '</div></div>',
    );

    if($rhe_name != 0) {
        $form['flat_info']['updated_flat_type'] = array(
            '#title' => t('Flat Type'),
            '#type' => 'select',
            '#options' => flat_type_under_rhe($rhe_name),
            '#required' => TRUE,
            '#prefix' => '<div class="col-md-4" id="flat_type_replace_updated"><div class="form-floating">',
            '#suffix' => '</div></div>',
            '#attributes' => array('class'	=> array('form-select')), 
            '#ajax' => array(
                'event' => 'change',
                'callback' => '_reload_rhewise_flat_info_callback',
                'method' => 'replace',
            ),
        );
    }
    
    $flat_type =  isset($form_state['values']['updated_flat_type']) &&!empty($form_state['values']['updated_flat_type']) ? $form_state['values']['updated_flat_type'] : 0;
     
    if($rhe_name != 0 && $flat_type != 0) {
        $form['flat_info']['updated_block_name'] = array(
            '#title' => t('Name of the Block'),
            '#type' => 'select',
            '#options' => block_name_under_rhe($rhe_name, $flat_type),
            '#required' => TRUE,
            '#prefix' => '<div class="col-md-4" id="block_name_replace_updated"><div class="form-floating">',
            '#suffix' => '</div></div>',
            '#attributes' => array('class'	=> array('form-select')), 
            '#ajax' => array(
                'event' => 'change',
                'callback' => '_reload_rhewise_flat_info_callback',
                'method' => 'replace',
            ),
        );
    }

    $block_name =  isset($form_state['values']['updated_block_name']) && !empty($form_state['values']['updated_block_name']) ? $form_state['values']['updated_block_name'] : 0;

    $form['flat_info']['updated_floor_no'] = array(
        '#prefix' => '<div class="col-md-4" id="floor_no_replace_updated"><div class="form-floating">',
        '#suffix' => '</div></div>',
    );

    if($rhe_name != 0 && $flat_type != 0 && $block_name != 0) {
       $form['flat_info']['updated_floor_no'] = array(
            '#title' => t('Floor No.'),
            '#type' => 'select',
            '#options' => fetch_floor_no($rhe_name, $flat_type, $block_name),
            '#required' => TRUE,
            '#prefix' => '<div class="col-md-4" id="floor_no_replace_updated"><div class="form-floating">',
            '#suffix' => '</div></div>',
            '#attributes' => array('class'	=> array('form-select')), 
            '#ajax' => array(
                'event' => 'change',
                'callback' => '_reload_rhewise_flat_info_callback',
                'method' => 'replace',
            ),
        ); 
    }

    $floor_no =  isset($form_state['values']['updated_floor_no']) && !empty($form_state['values']['updated_floor_no']) ? $form_state['values']['updated_floor_no'] : 0;
    
    $form['flat_info']['updated_flat_no'] = array(
        '#prefix' => '<div class="col-md-4" id="flat_no_replace_updated"><div class="form-floating">',
        '#suffix' => '</div></div>',
    );

    if($rhe_name != 0 && $flat_type != 0 && $block_name != 0 && $floor_no != 0) {
       $form['flat_info']['updated_flat_no'] = array(
            '#title' => t('Flat No.'),
            '#type' => 'select',
            '#options' => fetch_flat_no($rhe_name, $flat_type, $block_name, $floor_no, 1),
            '#required' => TRUE,
            '#prefix' => '<div class="col-md-4" id="flat_no_replace_updated"><div class="form-floating">',
            '#suffix' => '</div></div>',
            '#attributes' => array('class'	=> array('form-select')),
            '#ajax' => array(
                'event' => 'change',
                'callback' => '_reload_rhewise_flat_info_callback',
                'method' => 'replace',
            ),
        ); 
    }
     
    $form['submit_row'] = array(
        '#prefix' => '<div class="row"><div class="col-md-12 text-center">',
        '#suffix' => '</div></div>',
    );

    $form['submit_row']['submit'] = array(
        '#type' => 'submit',
        '#value' => t('Submit'),
        '#attributes' => array('class' => array('btn bg-primary btn-sm px-5 rounded-pill text-white fw-bolder')),
    );

    
    return $form;  

}

function _reload_rhewise_flat_info_callback($form, &$form_state) {
    $commands = array();
    
    $commands[] = ajax_command_replace("#flat_type_replace_updated", drupal_render($form['flat_info']['updated_flat_type']));
    $commands[] = ajax_command_replace("#block_name_replace_updated", drupal_render($form['flat_info']['updated_block_name']));
    $commands[] = ajax_command_replace("#floor_no_replace_updated", drupal_render($form['flat_info']['updated_floor_no']));
    $commands[] = ajax_command_replace("#flat_no_replace_updated", drupal_render($form['flat_info']['updated_flat_no']));

    return array('#type' => 'ajax', '#commands' => $commands);
}

function special_recommendation_occupant_submit($form, &$form_state) {
    global $user;
    $val = $form_state['values'];
    // echo "<pre>";print_r($val); die;

    $online_application_id = $val['online_application_id'];

    // create allotment process number
    $allotment_process_data = db_query("SELECT MAX(allotment_process_no) as allotment_process_no FROM housing_allotment_process WHERE allotment_process_type = 'MANUAL' OR allotment_process_type = 'ALOT'")->fetchObject();
<<<<<<< HEAD

    if(!empty($allotment_process_data)){
       $allotment_process_no = 'ALOT-' . str_pad((int) preg_replace('/\D/', '', $allotment_process_data->allotment_process_no) + 1, 2, '0', STR_PAD_LEFT); 
    }else{
        $allotment_process_no = 'ALOT-01';
    }
=======
    // echo "<pre>";print_r($allotment_process_data);
    if(!empty($allotment_process_data)){
        $allotment_process_no = 'ALOT-' . str_pad((int) preg_replace('/\D/', '', $allotment_process_data->allotment_process_no) + 1, 2, '0', STR_PAD_LEFT); 
    }else{
        $allotment_process_no = 'ALOT-01';
    } 
>>>>>>> 2502ab393f49b61f25516e6bb0502acbb6d447a0
    
    // Allotment Process insert into housing_allotment_process
    $allotment_process_arr = array(
        'allotment_date'    => DateTime::createFromFormat('d/m/Y', $val['allotment_process_date'])->format('Y-m-d'), //by dg 19-09-2025
        'allotment_process_no' => $allotment_process_no,
        'allotment_process_type' => 'MANUAL', // Added by Moumita on 07-08-2025
        'allotment_flat_type' => $val['updated_flat_type'],
    );
    // echo "<pre>";print_r($allotment_process_arr); die;
    db_insert('housing_allotment_process')->fields($allotment_process_arr)->execute();

<<<<<<< HEAD

=======
>>>>>>> 2502ab393f49b61f25516e6bb0502acbb6d447a0
   /* update new alltment application*/  
    $new_allotment_app_arr = array();
    $new_allotment_app_arr['flat_type_id'] = $val['updated_flat_type']; 

    
    if($online_application_id != 0) {
        db_update('housing_new_allotment_application')
        ->fields($new_allotment_app_arr)
        ->condition('online_application_id', $online_application_id, '=')
        ->execute();
    }

    // housing_flat_occupant
    $housing_flat_occupantArr = array(
        'online_application_id' => $online_application_id,
        'allotment_date' => DateTime::createFromFormat('d/m/Y', $val['allotment_process_date'])->format('Y-m-d'), //by dg 19-09-2025
        'flat_id' => $val['updated_flat_no'],
        'allotment_no' => 'MSR-' . $online_application_id.'-'.time(),
        'allotment_process_no' => $allotment_process_no,
        'allotment_approve_or_reject_date' => date('Y-m-d H:i:s'),
    );
    // check for duplicate entry
    $r4 = db_query("select 1 from housing_flat_occupant where online_application_id=:online_application_id and flat_id=:flat_id",array(':online_application_id'=>$online_application_id,':flat_id'=>$val['updated_flat_no']))->fetchObject();
    if(empty($r4)){
        $flat_occupant_id = db_insert('housing_flat_occupant')->fields($housing_flat_occupantArr)->execute();
    }

    // update housing_online_application
    db_update('housing_online_application')->fields(array('status' => 'allotted'))->condition('online_application_id', $online_application_id)->execute();

    // update housing_flat by flat_id 
    db_update('housing_flat')->fields(array('flat_status_id' => 2))->condition('flat_id', $val['updated_flat_no'])->execute();

    // insert housing_process_flow
    $process_flow_arr = array(
        'online_application_id' => $online_application_id,
        'status_id' => get_status_id('allotted'), // status_id of housing_official_approved // Updated by Subham 22/11/2024
        'created_at' => date('Y-m-d H:i:s'),
        'uid' => $user->uid,
        'short_code' =>'allotted'
    );
    db_insert('housing_process_flow')->fields($process_flow_arr)->execute();
    special_recommendation_remove(encrypt_url($online_application_id) , $action = 'manual');


    
    // drupal_goto('special-recommendation-list-view');
}