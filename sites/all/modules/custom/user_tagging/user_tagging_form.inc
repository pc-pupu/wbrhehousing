<?php
// Done by Subham dt.28-05-2025
function user_tagging_form($form, &$form_state) {
    global $user;

    // data fetch from hrms log table as read only mode
    $query = db_select('housing_hrms_applicant_login_log','hlog')->fields('hlog',array('json_decrypted_data'))->condition('hrms_id',$user->name)->execute();
    $result = $query->fetchObject(); 
    if(!empty($result))
    $data = json_decode($result->json_decrypted_data); 
    
    if(!empty($data)){
        $name = $data->name;
        $mobile = $data->mobile;
    }else{
        $name = '';
        $mobile = '';
    }

    $form['personal_info'] = array(
        '#prefix' => '<div class="row">',
        '#suffix' => '</div>',
    );
                        
    $form['personal_info']['applicant_name'] = array(
        '#title' => t('Applicant\'s Name'),
        '#type' => 'textfield',
        '#prefix' => '<div class="col-md-4"><div class="form-floating">',
        '#suffix' => '</div></div>',
        '#default_value' => $name,
        '#attributes' => array('oninput'=>"this.value=this.value.toUpperCase()", 'class'=>array('form-control')),
        '#required' => TRUE,
    );

    $form['personal_info']['mobile_no'] = array(
        '#title' => t('Mobile no'),
        '#type' => 'textfield',
        '#maxlength' => 10,
        '#required' => TRUE,
        '#default_value' => $mobile,
        '#prefix' => '<div class="col-md-4"><div class="form-floating">',
        '#suffix' => '</div></div>',
        '#element_validate' => array('element_validate_mobile'),
        '#attributes' => array('id' => 'mobile_no', 'class'=>array('form-control')),
    );

    $form['personal_info']['email'] = array(
        '#title' => t('Email ID'),
        '#type' => 'textfield',
        '#prefix' => '<div class="col-md-4"><div class="form-floating">',
        '#suffix' => '</div></div>',
        '#element_validate' => array('element_validate_email'),
        '#attributes' => array('oninput'=>"this.value=this.value.toLowerCase()", 'class'=>array('form-control')),
        '#default_value' => '',
        '#required' => TRUE,
    );

    /*$form['personal_info']['identification_no'] = array(                 // Turned off 24-07-2025 by Subham
        '#title' => t('Any Identification No. (HRMS/Aadhaar/PAN)'),
        '#type' => 'textfield',
        '#attributes' => array('oninput'=>"this.value=this.value.toUpperCase()",'class'=>array('form-control')),
        '#prefix' => '<div class="col-md-4"><div class="form-floating">',
        '#suffix' => '</div></div>',
        '#required' => TRUE,
    );*/
    /*
    $form['personal_info']['dob'] = array(
        '#title' => t('Date of Birth'),
        '#type' => 'textfield',
        '#attributes' => array('id' => 'edit-date-of-birth', 'class'=>array('form-control')),
        '#required' => TRUE,
        '#prefix' => '<div class="col-md-4"><div class="form-floating">',
        '#suffix' => '</div></div>',
        '#element_validate' => array('element_validate_date', 'element_validate_dob_before_license_issue')
    );*/

    $form['personal_info']['license_no'] = array(
        '#title' => t('Licence No.'),
        '#type' => 'textfield',
        '#attributes' => array('oninput'=>"this.value=this.value.toUpperCase()",'class'=>array('form-control')),
        '#prefix' => '<div class="col-md-4"><div class="form-floating">',
        '#suffix' => '</div></div>',
        '#required' => TRUE,
    );

    $form['personal_info']['license_issue_date'] = array(
        '#title' => t('License Issue Date'),
        '#type' => 'textfield',
        '#attributes' => array('id' => 'edit-issue-date', 'class'=>array('form-control')),
        '#required' => TRUE,
        '#prefix' => '<div class="col-md-4"><div class="form-floating">',
        '#suffix' => '</div></div>',
        '#element_validate' => array('element_validate_date')
    );

    $form['personal_info']['license_expiry_date'] = array(
        '#title' => t('License Expiry Date'),
        '#type' => 'textfield',
        '#attributes' => array('id' => 'edit-expiry-date', 'class'=>array('form-control')),
        '#prefix' => '<div class="col-md-4"><div class="form-floating">',
        '#suffix' => '</div></div>',
        '#element_validate' => array('element_validate_date', 'element_validate_expiry_date')
    );

    $form['personal_info']['physical_application_vs_cs'] = array(
      '#type' => 'radios',
      '#title' => t('Select "Yes" if you have previously submitted a physical application for Vertical Shifting or Category Shifting.'),
      '#options' => array(
        'yes' => t('Yes'),
        'no'  => t('No'),
      ),
      '#default_value' => 'no',
      '#title_display' => 'before', // show title above radios
      '#prefix' => '<div style="color:red; font-weight:bold;">',
      '#suffix' => '</div>',
      '#required' => TRUE,
    );

    $form['personal_info']['physical_application_no'] = array(   // added by dg 18-08-2025
        '#title' => t('Physical Application No.'),
        '#type' => 'textfield',
        
        '#prefix' => '<div class="col-md-4"><div class="form-floating">',
        '#suffix' => '</div></div>',
        '#attributes' => array(
            'id' => 'edit-physical-application-no',
            'class' => array('form-control'),
        ),
        '#states' => array(
            'visible' => array(
            ':input[name="physical_application_vs_cs"]' => array('value' => 'yes'),
            ),
            'required' => array(
            ':input[name="physical_application_vs_cs"]' => array('value' => 'yes'),
            ),
        ),
    );

    $form['personal_info']['application_type'] = array(
        '#title' => t('Application Type'),
        '#type' => 'select',
        '#options' => array(
            '' => t('-Select-'),
            'VS' => t('Floor Shifting'),
            'CS' => t('Category Shifting'),
        ),
        '#prefix' => '<div class="col-md-4"><div class="form-floating">',
        '#suffix' => '</div></div>',
        '#attributes' => array('id' => 'edit-application-type','class' => array('form-select')),
        '#states' => array(
            'visible' => array(
            ':input[name="physical_application_vs_cs"]' => array('value' => 'yes'),
            ),
            'required' => array(
            ':input[name="physical_application_vs_cs"]' => array('value' => 'yes'),
            ),
        ),
    );



    $form['flat_info'] = array(
        '#prefix' => '<div class="row">',
        '#suffix' => '</div>',
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
            '#attributes' => array('class'  => array('form-select')), 
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
            '#attributes' => array('class'  => array('form-select')), 
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
            '#attributes' => array('class'  => array('form-select')), 
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

    if($rhe_name != 0 && $flat_type != 0 && $block_name != 0 && !empty($floor_no)) {
       $form['flat_info']['updated_flat_no'] = array(
            '#title' => t('Flat No.'),
            '#type' => 'select',
            '#options' => fetch_flat_no($rhe_name, $flat_type, $block_name, $floor_no),
            '#required' => TRUE,
            '#prefix' => '<div class="col-md-4" id="flat_no_replace_updated"><div class="form-floating">',
            '#suffix' => '</div></div>',
            '#attributes' => array('class'  => array('form-select')),
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
        '#submit' => array('user_tagging_submit')
    );

    // $form['submit_row']['exit_btn'] = array(
    //     '#type' => 'markup',
    //     '#markup' => l('Exit', 'user/logout', array('html'=>true, 'attributes'=>array('class'=>array('btn bg-info btn-sm px-5 rounded-pill text-white fw-bolder'))))
    // );
    return $form;
}

function user_tagging_submit($form, &$form_state) {
    global $user;
    $form_data = array();

    // echo '<pre>'; print_r($form_state);
    if($form_state['values']['physical_application_vs_cs'] == 'yes' && ($form_state['values']['physical_application_no'] == '' || $form_state['values']['application_type'] == '')){
        // echo "k";die;
        if($form_state['values']['physical_application_no'] == ''){
            drupal_set_message('Physical Application No. is required when "Yes" is selected. ', 'error');
        }
        if($form_state['values']['application_type'] == ''){
            drupal_set_message('Application Type is required when "Yes" is selected. ', 'error');
        }
    }else if($form_state['values']['physical_application_vs_cs'] == 'no' || ($form_state['values']['physical_application_vs_cs'] == 'yes' && $form_state['values']['physical_application_no'] != '' && $form_state['values']['application_type'] != '')){

        $form_data['applicant_name'] = trim($form_state['values']['applicant_name']);
        $form_data['mobile_no'] = trim($form_state['values']['mobile_no']);
        $form_data['email'] = trim($form_state['values']['email']);
        $form_data['physical_application_no'] = trim($form_state['values']['physical_application_no']);  //aded by dg 18-08-2025
        $form_data['application_type'] = trim($form_state['values']['application_type']);  //aded by dg 18-08-2025

        // $form_data['identification_no'] = trim($form_state['values']['identification_no']); // Turned off 24-07-2025 by Subham
        // $form_data['dob'] = date('Y-m-d',strtotime($form_state['values']['dob']));
        $form_data['license_no'] = trim($form_state['values']['license_no']);


        $form_data['license_issue_date'] = DateTime::createFromFormat('d/m/Y', $form_state['values']['license_issue_date'])->format('Y-m-d');
        $form_data['license_expiry_date'] = ($form_state['values']['license_expiry_date']!='') ? DateTime::createFromFormat('d/m/Y', $form_state['values']['license_expiry_date'])->format('Y-m-d') : NULL;
        
        // date('Y-m-d',strtotime($form_state['values']['license_expiry_date'])) 


        $form_data['flat_id'] = trim($form_state['values']['updated_flat_no']);
        $form_data['created_date'] = date('Y-m-d H:i:s');
        $form_data['status'] = '1';
        $form_data['flag'] = 'new';  // New or Tagged
        $form_data['hrms_id'] = $user->name;
        $form_data['uid'] = $user->uid;

        // echo '<pre>'; print_r($form_data);die;
        
        $chk_duplicate_data = db_select('housing_user_tagging', 'hut')
            ->fields('hut',array('flat_id'))
            ->condition('flat_id', $form_data['flat_id'])
            ->execute()
            ->fetchObject();
        
        if ($chk_duplicate_data) {
            drupal_set_message('This flat is already tagged. Duplicate entries are not allowed.','error');
        }else{
            db_insert('housing_user_tagging')
            ->fields($form_data)
            ->execute();
            drupal_set_message(t('Your data has been submitted for departmental approval. Please wait until the approval process is complete.'));
        }
    }
    
  
}

function _reload_rhewise_flat_info_callback($form, &$form_state) {
    $commands = array();
    
    $commands[] = ajax_command_replace("#flat_type_replace_updated", drupal_render($form['flat_info']['updated_flat_type']));
    $commands[] = ajax_command_replace("#block_name_replace_updated", drupal_render($form['flat_info']['updated_block_name']));
    $commands[] = ajax_command_replace("#floor_no_replace_updated", drupal_render($form['flat_info']['updated_floor_no']));
    $commands[] = ajax_command_replace("#flat_no_replace_updated", drupal_render($form['flat_info']['updated_flat_no']));

    return array('#type' => 'ajax', '#commands' => $commands);
}


