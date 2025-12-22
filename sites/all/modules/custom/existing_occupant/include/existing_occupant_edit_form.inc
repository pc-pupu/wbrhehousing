<?php
function existing_occupant_edit_form($form, &$form_state, $en_id = 0) {
    // $ocuupant_id = decrypt_url($en_id);

    $online_application_id = decrypt_url($en_id);

    $query = db_query("   
        SELECT * FROM housing_online_application hoa     
        inner join housing_applicant_official_detail haod on haod.applicant_official_detail_id = hoa.applicant_official_detail_id
        inner join housing_applicant ha on ha.housing_applicant_id = haod.housing_applicant_id
        inner join housing_ddo hd on hd.ddo_id = haod.ddo_id
        inner join users u on u.uid = haod.uid 
        left join housing_flat_occupant hfo on hfo.online_application_id = hoa.online_application_id
        left join housing_occupant_license hol on hol.flat_occupant_id = hfo.flat_occupant_id

        where hoa.online_application_id = ".$online_application_id.""
    );

    $data = $query->fetchObject();
    //echo "<pre>";print_r($data);die;

    $chk_permanent_address = 0;

    $form['form_begin'] = array(
        '#type' => 'markup',
        '#markup' => '<div class="online_application">',
        '#weight' => -2
    );

    $form['hidden_online_application_id'] = array(
        '#type' =>'hidden',
        '#value' =>$data->online_application_id,
        '#attributes' => array('readonly' => 'readonly'),
    );

    $form['hidden_uid'] = array(
        '#type' =>'hidden',
        '#value' =>$data->uid,
        '#attributes' => array('readonly' => 'readonly'),
    );
    $form['hidden_applicant_id'] = array(
        '#type' =>'hidden',
        '#value' =>$data->housing_applicant_id,
        '#attributes' => array('readonly' => 'readonly'),
    );

    $form['hidden_applicant_official_detail_id'] = array(
        '#type' =>'hidden',
        '#value' =>$data->applicant_official_detail_id,
        '#attributes' => array('readonly' => 'readonly'),
    );

    $form['hidden_flat_occupant_id'] = array(
        '#type' =>'hidden',
        '#value' =>$data->occupant_license_id,
        '#attributes' => array('readonly' => 'readonly'),
    );

    //personal information
    $form['prrsonal_info'] = array(
        '#type' => 'fieldset',
        '#title' => t('Occupant\'s Personal Information(According to Service Book)'),
        '#collapsible' => FALSE, // Added
        '#collapsed' => FALSE,  // Added
    );
                    
    $form['prrsonal_info']['occupant_name'] = array(
        '#title' => t('Occupant\'s Name'),
        '#type' => 'textfield',
        '#prefix' => '<div class="row"><div class="col-md-6"><div class="form-floating">',
        '#suffix' => '</div></div>',
        '#default_value' => strtoupper($data->applicant_name),
        '#attributes' => array('oninput'=>"this.value=this.value.toUpperCase()",'class'=>array('form-control')), 
        '#required' => TRUE,
    );
    
    $form['prrsonal_info']['occupant_father_name'] = array(
        '#title' => t('Father\'s / Husband\'s Name'),
        '#type' => 'textfield',
        '#default_value' => 'NA',
        '#prefix' => '<div class="col-md-6"><div class="form-floating">',
        '#suffix' => '</div></div></div>',
        '#default_value' => strtoupper($data->guardian_name),
        '#attributes' => array('oninput'=>"this.value=this.value.toUpperCase()",'class'=>array('form-control')),
        '#required' => TRUE,
    );
		
	//Permanent Address
    $form['prrsonal_info']['permanent_address'] = array(
        '#type' => 'fieldset',
        '#title' => t('Permanent Address'),
        '#collapsible' => FALSE, // Added
        '#collapsed' => FALSE,  // Added
    );
    $form['prrsonal_info']['permanent_address']['permanent_street'] = array(
        '#title' => t('Address'),
        '#type' => 'textfield',
        '#required' => TRUE,
        '#prefix' => '<div class="row"><div class="col-md-6"><div class="form-floating">',
        '#suffix' => '</div></div>',
        '#element_validate' => array('element_validate_streetaddress'), 
        '#default_value' => $data->permanent_street,
        '#attributes' => array('oninput'=>"this.value=this.value.toUpperCase()",'class'=>array('form-control')),
    );
    $form['prrsonal_info']['permanent_address']['permanent_city_town_village'] = array(
        '#title' => t('City / Town / Village'),
        '#type' => 'textfield',
        '#required' => TRUE,
        '#prefix' => '<div class="col-md-6"><div class="form-floating">',
        '#suffix' => '</div></div></div>',
        '#element_validate' => array('element_validate_alphabatic_text'), 
        '#default_value' => $data->permanent_city_town_village,
        '#attributes' => array('oninput'=>"this.value=this.value.toUpperCase()",'class'=>array('form-control')),
    );
    $form['prrsonal_info']['permanent_address']['permanent_post_office'] = array(
        '#title' => t('Post Office'),
        '#type' => 'textfield',
        '#required' => TRUE,
        '#prefix' => '<div class="row"><div class="col-md-6"><div class="form-floating">',
        '#suffix' => '</div></div>',
        '#element_validate' => array('element_validate_alphabatic_text'), 
        '#default_value' => $data->permanent_post_office,
        '#attributes' => array('oninput'=>"this.value=this.value.toUpperCase()",'class'=>array('form-control')),
    );
    $form['prrsonal_info']['permanent_address']['permanent_district'] = array(
        '#title' => t('District'),
        '#type' => 'select',
        '#options' => district_list(),
        '#required' => TRUE,
        '#prefix' => '<div class="col-md-6"><div class="form-floating">',
        '#suffix' => '</div></div></div>',
        '#attributes' => array('class'=>array('form-select')),
        '#default_value' => $data->permanent_district,
        // '#select2' => array(
        // 	'width' => '500',
        // 	//'placeholder' => '- Select -',
        // 	'allowClear' => TRUE
//      	)
    );
    $form['prrsonal_info']['permanent_address']['permanent_pincode'] = array(
        '#title' => t('Pincode'),
        '#type' => 'textfield',
        '#maxlength' => 6,
        '#required' => TRUE,
        '#element_validate' => array('element_validate_numeric_positive'), 
        '#prefix' => '<div class="row"><div class="col-md-6"><div class="form-floating">',
        '#suffix' => '</div></div></div>',
        '#attributes' => array('class' => array('numeric_positive'),'class'=>array('form-control')),
        '#default_value' => $data->permanent_pincode
    );
	//end
	
    if($chk_permanent_address != 1) {
    //Present Address
        $form['prrsonal_info']['present_address'] = array(
            '#type' => 'fieldset',
            '#title' => t('Present Address'),
            '#collapsible' => FALSE, // Added
            '#collapsed' => FALSE,  // Added
            '#prefix' => '<div id="reload_present_address">',
            '#suffix' => '</div>',
        );
        
        $form['prrsonal_info']['present_address']['present_street'] = array(
            '#title' => t('Address'),
            '#type' => 'textfield',
            '#required' => TRUE,
            '#prefix' => '<div class="row"><div class="col-md-6" id="reload_present_street"><div class="form-floating">',
            '#suffix' => '</div></div>',
            '#element_validate' => array('element_validate_streetaddress'), 
            '#attributes' => array('oninput'=>"this.value=this.value.toUpperCase()",'class'=>array('form-control')), //'id' => 'present_street', 
            '#default_value' => $data->present_street,
        );
        $form['prrsonal_info']['present_address']['present_city_town_village'] = array(
            '#title' => t('City / Town / Village'),
            '#type' => 'textfield',
            '#required' => TRUE,
            '#prefix' => '<div class="col-md-6" id="reload_present_city_town_village"><div class="form-floating">',
            '#suffix' => '</div></div></div>',
            '#element_validate' => array('element_validate_alphabatic_text'), 
            '#default_value' => $data->present_city_town_village,
            '#attributes' => array('oninput'=>"this.value=this.value.toUpperCase()",'class'=>array('form-control')), //'id' => 'present_city_town_village', 
        );
        $form['prrsonal_info']['present_address']['present_post_office'] = array(
            '#title' => t('Post Office'),
            '#type' => 'textfield',
            '#required' => TRUE,
            '#prefix' => '<div class="row"><div class="col-md-6" id="reload_present_post_office"><div class="form-floating">',
            '#suffix' => '</div></div>',
            '#element_validate' => array('element_validate_alphabatic_text'), 
            '#default_value' => $data->present_post_office,
            '#attributes' => array('oninput'=>"this.value=this.value.toUpperCase()",'class'=>array('form-control')), //'id' => 'present_post_office', 
        );
        $form['prrsonal_info']['present_address']['present_district'] = array(
            '#title' => t('District'),
            '#type' => 'select',
            '#options' => district_list(),
            '#required' => TRUE,
            '#prefix' => '<div class="col-md-6" id="reload_present_district"><div class="form-floating">',
            '#suffix' => '</div></div></div>',
            '#attributes' => array('class'=>array('form-select')),
            '#default_value' => $data->present_district,
            //'#attributes' => array('id' => 'present_district'),
            // '#select2' => array(
            // 	'width' => '500',
            // 	//'placeholder' => '- Select -',
            // 	'allowClear' => TRUE
            // ),
        );
        $form['prrsonal_info']['present_address']['present_pincode'] = array(
            '#title' => t('Pincode'),
            '#type' => 'textfield',
            '#maxlength' => 6,
            '#required' => TRUE,
            '#element_validate' => array('element_validate_numeric_positive'), 
            '#prefix' => '<div class="row"><div class="col-md-6" id="reload_present_pincode"><div class="form-floating">',
            '#suffix' => '</div></div></div>',
            '#attributes' => array('class' => array('numeric_positive'),'class'=>array('form-control')), //'id' => 'present_pincode', 
            '#default_value' => $data->present_pincode
        );
	} else {
			
        $form['prrsonal_info']['present_address'] = array(
            '#prefix' => '<div id="reload_present_address">',
            '#suffix' => '</div>',
        );
	}
	//end
	
    $form['prrsonal_info']['mobile'] = array(
        '#title' => t('Mobile no'),
        '#type' => 'textfield',
        '#default_value' => $data->mobile_no,
        '#maxlength' => 10,
        '#required' => TRUE,
        '#prefix' => '<div class="row"><div class="col-md-6"><div class="form-floating">',
        '#suffix' => '</div></div>',
        '#element_validate' => array('element_validate_mobile'),
        '#attributes' => array('id' => 'mobile_no','class'=>array('form-control')),
    );
    
    $form['prrsonal_info']['email'] = array(
        '#title' => t('Email ID'),
        '#type' => 'textfield',
        '#prefix' => '<div class="col-md-6"><div class="form-floating">',
        '#suffix' => '</div></div></div>',
        '#required' => TRUE,
        '#element_validate' => array('element_validate_email'),
        '#attributes' => array('oninput'=>"this.value=this.value.toLowerCase()",'class'=>array('form-control')),
        '#default_value' => $data->mail,
    );
    
    $form['prrsonal_info']['dob'] = array(
        '#title' => t('Date of Birth(According to Service Book)'),
        '#type' => 'textfield',
        '#attributes' => array('readonly' => 'readonly', 'id' => 'edit-dob','class'=>array('form-control')),
        '#required' => TRUE,
        '#validated' => TRUE,
        '#prefix' => '<div class="row"><div class="col-md-6"><div class="form-floating">',
        '#suffix' => '</div></div>',
        '#element_validate' => array('element_validate_date', 'element_validate_age'),
        '#default_value' => date('d/m/Y',strtotime($data->date_of_birth)),
    );
    
    $form['prrsonal_info']['gender'] = array(
        '#title' => t('Gender'),
        '#type' => 'radios',
        '#options' => array('M' => 'Male', 'F' => 'Female'),
        '#default_value' => $data->gender,
        '#prefix' => '<div class="col-md-6"><div class="form-floating">',
        '#suffix' => '</div></div></div>',
        '#required' => TRUE,
    );
    
    // occupant's official information
    $form['official_info'] = array(
        '#type' => 'fieldset',
        '#title' => t('Occupant\'s Official Information'),
        '#collapsible' => FALSE, // Added
        '#collapsed' => FALSE,  // Added
    );
    
    $form['official_info']['hrms_id'] = array(
        '#title' => t('Employee HRMS ID'),
        '#type' => 'textfield',
        '#default_value' => $data->hrms_id,
        '#attributes' => array('class' => array('numeric_positive'),'class'=>array('form-control')),
        '#prefix' => '<div class="row"><div class="col-md-6"><div class="form-floating">',
        '#suffix' => '</div></div>',
        '#element_validate' => array('element_validate_hrms_id'),
        '#required' => TRUE,
    );
    
    $form['official_info']['occupant_designation'] = array(
        '#title' => t('Designation'),
        '#type' => 'textfield',
        '#required' => TRUE,
        '#prefix' => '<div class="col-md-6"><div class="form-floating">',
        '#suffix' => '</div></div></div>',
        '#element_validate' => array('element_validate_textarea'),
        '#attributes' => array('oninput'=>"this.value=this.value.toUpperCase()",'class'=>array('form-control')),
        //'#attributes' => array('id' => 'edit-app-designation', 'oncopy' => "return false", 'onpaste' => "return false"),
        '#default_value' => $data->applicant_designation,
    );

    $form['official_info']['pay_band'] = array(
        '#title' => t('Basic Pay Range'),
        '#type' => 'select',
        '#options' => pay_band_list(),
        '#required' => TRUE,
        '#prefix' => '<div class="row"><div class="col-md-6"><div class="form-floating">',
        '#suffix' => '</div></div>',
        '#validated' => TRUE,
        '#attributes' => array('class'=>array('form-control')),
        '#default_value' => $data->pay_band_id,
        
    );
    $form['official_info']['pay_in'] = array(
        '#title' => t('Basic Pay'),
        '#type' => 'textfield',
        '#required' => TRUE,
        '#element_validate' => array('element_validate_numeric_positive'), 
        '#prefix' => '<div class="col-md-6"><div class="form-floating">',
        '#suffix' => '</div></div></div>',
        '#attributes' => array('class' => array('numeric_positive'),'class'=>array('form-control')),
        '#default_value' => $data->pay_in_the_pay_band,
    );
	
		
    $form['official_info']['occupant_posting_place'] = array(
        '#title' => t('Place of Posting'),
        '#type' => 'textfield',
        '#required' => TRUE,
        '#prefix' => '<div class="row"><div class="col-md-6"><div class="form-floating">',
        '#suffix' => '</div></div>',
        '#element_validate' => array('element_validate_alphabatic_text'),
        '#default_value' => $data->applicant_posting_place,
        '#attributes' => array('oninput'=>"this.value=this.value.toUpperCase()",'class'=>array('form-control')),  
    ); 
    $form['official_info']['occupant_headquarter'] = array(
        '#title' => t('Headquarter'),
        '#type' => 'textfield',
        '#required' => TRUE,
        '#prefix' => '<div class="col-md-6"><div class="form-floating">',
        '#suffix' => '</div></div></div>',
        '#element_validate' => array('element_validate_alphabatic_text'), 
        '#default_value' => $data->applicant_headquarter,
        '#attributes' => array('oninput'=>"this.value=this.value.toUpperCase()",'class'=>array('form-control')), 
    );
    $form['official_info']['doj'] = array(
        '#title' => t('Date of Joining'),
        '#type' => 'textfield',
        '#attributes' => array('readonly' => 'readonly', 'id' => 'edit-doj','class'=>array('form-control')),
        '#default_value' => date('d/m/Y',strtotime($data->date_of_joining)),
        '#required' => TRUE,
        '#prefix' => '<div class="row"><div class="col-md-6"><div class="form-floating">',
        '#suffix' => '</div></div>',
        // '#element_validate' => array('element_validate_date', 'element_validate_joining_age'),
    );
    
    $form['official_info']['dor'] = array(
        '#title' => t('Date of Retirement(According to Service Book)'),
        '#type' => 'textfield',
        //'#attributes' => array('readonly' => 'readonly'),
        '#attributes' => array('readonly' => 'readonly', 'id' => 'edit-dor','class'=>array('form-control')),
        '#default_value' => date('d/m/Y',strtotime($data->date_of_retirement)),
        '#required' => TRUE,
        '#prefix' => '<div class="col-md-6"><div class="form-floating">',
        '#suffix' => '</div></div></div>',
        // '#element_validate' => array('element_validate_date', 'element_validate_joining_age'),
    );
    
    
    // occupant's office address
    $form['office_add'] = array(
        '#type' => 'fieldset',
        '#title' => t('Name and Address of the Office'),
        '#collapsible' => FALSE, // Added
        '#collapsed' => FALSE,  // Added
    );
    $form['office_add']['office_name'] = array(
        '#title' => t('Name of the Office'),
        '#type' => 'textfield',
        '#required' => TRUE,
        '#prefix' => '<div class="row"><div class="col-md-6"><div class="form-floating">',
        '#suffix' => '</div></div>',
        '#element_validate' => array('element_validate_textarea'),
        '#default_value' => $data->office_name,
        '#attributes' => array('oninput'=>"this.value=this.value.toUpperCase()",'class'=>array('form-control')),  
    );
    
    $form['office_add']['office_street'] = array(
        '#title' => t('Address of the Office'),
        '#type' => 'textfield',
        '#required' => TRUE,
        '#prefix' => '<div class="col-md-6"><div class="form-floating">',
        '#suffix' => '</div></div></div>',
        '#element_validate' => array('element_validate_streetaddress'), 
        '#default_value' => $data->office_street,
        '#attributes' => array('oninput'=>"this.value=this.value.toUpperCase()",'class'=>array('form-control')), 
    );
    $form['office_add']['office_city'] = array(
        '#title' => t('City / Town / Village'),
        '#type' => 'textfield',
        '#required' => TRUE,
        '#prefix' => '<div class="row"><div class="col-md-6"><div class="form-floating">',
        '#suffix' => '</div></div>',
        '#element_validate' => array('element_validate_alphabatic_text'), 
        '#default_value' => $data->office_city_town_village,
        '#attributes' => array('oninput'=>"this.value=this.value.toUpperCase()",'class'=>array('form-control')), 
    );
    $form['office_add']['office_post_office'] = array(
        '#title' => t('Post Office'),
        '#type' => 'textfield',
        '#required' => TRUE,
        '#prefix' => '<div class="col-md-6"><div class="form-floating">',
        '#suffix' => '</div></div></div>',
        '#element_validate' => array('element_validate_alphabatic_text'), 
        '#default_value' => $data->office_post_office,
        '#attributes' => array('oninput'=>"this.value=this.value.toUpperCase()",'class'=>array('form-control')), 
    );
    $form['office_add']['office_district'] = array(
        '#title' => t('District'),
        '#type' => 'select',
        '#options' => district_list(),
        '#required' => TRUE,
        '#prefix' => '<div class="row"><div class="col-md-6"><div class="form-floating">',
        '#suffix' => '</div></div>',
        '#attributes' => array('class'=>array('form-select')),
        '#default_value' => $data->office_district,
        // '#select2' => array(
        // 	'width' => '500',
        // 	//'placeholder' => '- Select -',
        // 	'allowClear' => TRUE
//      	)
    );
    $form['office_add']['office_pincode'] = array(
        '#title' => t('Pincode'),
        '#type' => 'textfield',
        '#maxlength' => 6,
        '#required' => TRUE,
        '#element_validate' => array('element_validate_numeric_positive'), 
        '#prefix' => '<div class="col-md-6"><div class="form-floating">',
        '#suffix' => '</div></div></div>',
        '#attributes' => array('class' => array('numeric_positive'),'class'=>array('form-control')),
        '#default_value' => $data->office_pin_code
    );
    $form['office_add']['office_phone_no'] = array(
        '#title' => t('Phone No.(With STD Code)'),
        '#type' => 'textfield',
        '#maxlength' => 15,
        '#required' => TRUE,
        '#element_validate' => array('element_validate_telephoneno'), 
        '#prefix' => '<div class="row"><div class="col-md-6"><div class="form-floating">',
        '#suffix' => '</div></div></div>',
        '#attributes' => array('class' => array('numeric_positive'),'class'=>array('form-control')),
        '#default_value' => $data->office_phone_no
    );
    
    // ddo details
    $form['ddo'] = array(
        '#type' => 'fieldset',
        '#title' => t('DDO with Full Address'),
        '#collapsible' => FALSE, // Added
        '#collapsed' => FALSE,  // Added
    );
    $form['ddo']['district'] = array(
        '#title' => t('DDO District'),
        '#type' => 'select',
        '#options' => district_list(),
        '#ajax' => array(
                    'event' => 'change',
                    'callback' => '_reload_ddo_designation',
                    // 'wrapper' => 'replace_designation',
                    'effect' => 'fade',
                    'progress' => array(
                        'type' => '',
                        'message' => '',
                    ), 
            ),
        '#required' => TRUE,
        '#validated' => TRUE,
        '#prefix' => '<div class="row"><div class="col-md-6"><div class="form-floating">',
        '#suffix' => '</div></div>',
        '#attributes' => array('class'=>array('form-select')),
        '#default_value' => $data->district_code
        // '#select2' => array(
        // 	'width' => '500',
        // 	//'placeholder' => '- Select -',
        // 	'allowClear' => TRUE
        // )
    );
    

    $form['ddo']['designation'] = array(
        '#title' => t('DDO Designation'),
        '#type' => 'select',
        '#options' => ($data->district_code !='') ? ddo_desig_list($data->district_code) : '17',
        '#ajax' => array(
            'event' => 'change',
            'callback' => '_reload_ddo_address',
            'wrapper' => 'replace_ddo_address',
            'effect' => 'fade',
            'progress' => array(
                'type' => '',
                'message' => '',
            ),
        ),
        '#default_value' => $data->ddo_id,
        '#required' => TRUE,
        '#validated' => TRUE,
        '#prefix' => '<div id="replace_designation" class="col-md-6"><div class="form-floating">',
        '#suffix' => '</div></div></div>',
        '#attributes' => array('class'=>array('form-select')),
        // '#select2' => array(
        //  'width' => '500',
        //  //'placeholder' => '- Select -',
        //  'allowClear' => TRUE
        // )
        );

    
    $form['ddo']['address'] = array(
        '#title' => t('DDO Address'),
        '#type' => 'textarea',
        '#attributes' => array('readonly' => 'readonly'),
        '#prefix' => '<div id="replace_ddo_address" class="col-md-12"><div class="form-floating">',
        '#suffix' => '</div></div>',
        // '#required' => TRUE,
        '#validated' => TRUE,
        '#default_value' => $data->ddo_address,
        '#attributes' => array('class'=>array('form-control')),
    );
    //license part//
    $form['license'] = array(
        '#type' => 'fieldset',
        '#title' => t('License Details'),
        '#collapsible' => FALSE, // Added
        '#collapsed' => FALSE,  // Added
    );

    $form['license']['license_no'] = array(
        '#title' => t('License No.'),
        '#type' => 'textfield',
        '#required' => TRUE,
        '#prefix' => '<div class="row"><div class="col-md-6"><div class="form-floating">',
        '#suffix' => '</div></div>',
        //'#element_validate' => array('element_validate_alphabatic_text'), 
        '#attributes' => array('oninput'=>"this.value=this.value.toUpperCase()",'class'=>array('form-control')), 
        '#default_value' => $data->existing_occupant_license_no
    );

    $form['license']['dol'] = array(
        '#title' => t('License Issue Date'),
        '#type' => 'textfield',
        '#attributes' => array('readonly' => 'readonly', 'id' => 'edit-dol','class'=>array('form-control')),
        '#required' => TRUE,
        '#validated' => TRUE,
        '#prefix' => '<div class="col-md-6"><div class="form-floating">',
        '#suffix' => '</div></div></div>',
        '#element_validate' => array('element_validate_date', 'element_validate_age'),
        '#default_value' => !empty($data->license_issue_date) ? date('d/m/Y', strtotime($data->license_issue_date)) : '',
    );

    $form['license']['authorised_or_not'] = array(
        '#title' => t('Occupant Status'),
        '#type' => 'select',
        //'#options' => pay_band_list(),
        '#options' => ['authorised' => 'Authorised Occupant', 'unauthorised' => 'Unauthorised Occupant'],
        '#required' => TRUE,
        '#prefix' => '<div class="row"><div class="col-md-6"><div class="form-floating">',
        '#suffix' => '</div></div>',
        '#validated' => TRUE,
        '#attributes' => array('class'=>array('form-control')),
        '#default_value' => $data->authorised_or_not
    );

    /*$form['license']['uploaded_licence'] = array(
        '#title' => t('Upload Current Licence'),
        '#type' => 'managed_file',
        '#description' => t('<b>Allowed Extension: pdf<br>Maximum File Size: 1 MB</b>'),
        '#progress_message' => t('Please wait...'),
        '#progress_indicator' => 'bar',
        '#default_value' => variable_get('Upload File', ''),
        // '#prefix' => '<div class="col-md-6"><div class="form-floating">',
        // '#suffix' => '</div></div></div>',
        //'#theme' => 'common_application_thumb_upload',
        '#upload_validators' => array(
                                        'file_validate_extensions' => array("pdf"),
                                    //'file_validated_size' => array(MAX_FILE_SIZE*1024*1024),
        
                                ),
        '#upload_location' => 'public://doc/',
        // '#required' => TRUE,
        '#process' => array('vertical_shifting_element_process'),
    );*/

			
    $form['submit_button']=array(
        '#type'=>'submit',
        '#value'=>t('Submit'),
        '#attributes' => array('class'=>array('btn bg-primary btn-sm px-5 rounded-pill text-white fw-bolder')),
    );
								
    $form['cancel_button']=array(
        '#type'=>'button',
        '#value'=>t('Cancel'),
        '#attributes' => array('onClick' => 'history.go(-1); event.preventDefault();','class'=>array('btn bg-danger btn-sm px-5 rounded-pill text-white fw-bolder')),
    );

    $form['form_end'] = array(
        '#type' => 'markup',
        '#markup' => '</div><div class="mb-5 pb-3"></div>',
    );
    return $form;
}

function existing_occupant_edit_form_submit($form, &$form_state) {
    $val = $form_state['values'];
    
    $user_id = $val['hidden_uid'];
    $applicant_official_detail_id = $val['hidden_applicant_official_detail_id'];
    $online_application_id = $val['hidden_online_application_id'];
    $occupant_license_id = $val['hidden_flat_occupant_id'];
    $housing_applicant_id = $val['hidden_applicant_id'];

    // personal info 

    $personal_info_array = array(
        'applicant_name' => $val['occupant_name'],
        'guardian_name' => $val['occupant_father_name'],
        //'date_of_birth' => $val['dob'],
        'date_of_birth' => trim(implode('-', array_reverse(explode('/', $val['dob'])))), //added by dg 22-07-2025
        'mobile_no' => $val['mobile'],
        'gender'=> $val ['gender'],

        
        'permanent_street' => $val['permanent_street'],
        'permanent_city_town_village' => $val['permanent_city_town_village'],
        'permanent_post_office' => $val['permanent_post_office'],
        'permanent_district' => $val['permanent_district'],
        'permanent_pincode' => $val['permanent_pincode'],
        
        
        'present_street' => $val['present_street'],
        'present_city_town_village' => $val['present_city_town_village'],
        'present_post_office' => $val['present_post_office'],
        'present_district' => $val['present_district'],
        'present_pincode' => $val['present_pincode'],
    );

    // echo "<pre>";print_r($personal_info_array);

    db_update('housing_applicant')->fields($personal_info_array)->condition('housing_applicant_id',$housing_applicant_id)->execute();


    $official_info_array = array(
        'ddo_id' => $val['designation'],
        'applicant_designation'=> $val['occupant_designation'],
        'applicant_headquarter' => $val['occupant_headquarter'],
        'applicant_posting_place' => $val['occupant_posting_place'],

        'pay_band_id' => $val['pay_band'],
        'pay_in_the_pay_band' => $val['pay_in'],
        
        //'date_of_joining' => $val['doj'],
        'date_of_joining' => trim(implode('-', array_reverse(explode('/', $val['doj'])))), //added by dg 22-07-2025
        //'date_of_retirement' => $val['dor'],
        'date_of_retirement' => trim(implode('-', array_reverse(explode('/', $val['dor'])))), //added by dg 22-07-2025
        
        'office_name'=> $val['office_name'],
        'office_street'=> $val['office_street'],
        'office_city_town_village'=> $val['office_city'],
        'office_post_office'=> $val['office_post_office'],
        'office_pin_code'=> $val['office_pincode'],
        // 'office_district'=> $val['office_district'],
        'office_phone_no'=> $val['office_phone_no'],
    );
    // echo "<pre>";print_r($official_info_array);
    db_update('housing_applicant_official_detail')->fields($official_info_array)->condition('uid',$user_id)->condition('applicant_official_detail_id',$applicant_official_detail_id )->execute();

    $user_array = array(
        'mail' => $val['email'],
    );
    //  echo "<pre>";print_r($user_array);
    db_update('users')->fields($user_array)->condition('uid',$user_id)->execute();

    // license table update
    $license_array = array(
        'existing_occupant_license_no' => $val['license_no'],
        //'license_issue_date' => $val['dol'],
        'license_issue_date' => trim(implode('-', array_reverse(explode('/', $val['dol'])))), //added by dg 22-07-2025
        'authorised_or_not' =>$val['authorised_or_not']
    );
    // echo "<pre>";print_r($license_array);die;
    db_update('housing_occupant_license')->fields($license_array)->condition('occupant_license_id',$occupant_license_id)->execute();
    
    drupal_set_message('Data has been updated successfully');
}