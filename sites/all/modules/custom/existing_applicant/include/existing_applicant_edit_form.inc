<?php

	function existing_applicant_edit_form($form, &$form_state, $online_application_id = '') {

		$online_application_id = decrypt_url($online_application_id);
		
		if (!empty($online_application_id) && is_numeric($online_application_id)) {
			
			$query = db_select('housing_applicant', 'ha');
			$query->innerJoin('housing_applicant_official_detail', 'haod', 'ha.housing_applicant_id = haod.housing_applicant_id');
			$query->innerJoin('housing_online_application', 'hoa', 'hoa.applicant_official_detail_id = haod.applicant_official_detail_id');
			$query->innerJoin('users', 'u', 'u.uid = haod.uid');
			$query->innerJoin('housing_ddo', 'hd', 'hd.ddo_id = haod.ddo_id');
			$query->innerjoin('housing_new_allotment_application','hnaa','hnaa.online_application_id = hoa.online_application_id');
			$query->condition('hoa.online_application_id', $online_application_id);
			$query->fields('ha', array());
			$query->fields('haod', array());
			$query->fields('hnaa', array('allotment_category'));
			$query->fields('hoa', array());
			$query->fields('u', array('mail'));
			$query->fields('hd', array('ddo_designation','district_code'));


			$result = $query->execute();
			$data = $result->fetchObject();
			// echo "<pre>";print_r($data);die;
		} else {
			// If the ID is not valid, you may handle this accordingly
			drupal_set_message('Invalid or missing application ID.', 'error');
			$data = null;
		}
		if(!empty($data->date_of_birth))
			$dob = date_format_change($data->date_of_birth);

		if(!empty($data->date_of_joining))
			$doj = date_format_change($data->date_of_joining);

		if(!empty($data->date_of_retirement))
			$dor  = date_format_change($data->date_of_retirement);

		if(!empty($data->date_of_application))
			$doa  = date_format_change($data->date_of_application);
		
			
		// echo $dob;die;
		// $district = isset($form_state['values']['district']) && !empty($form_state['values']['district']) ? $form_state['values']['district'] : '';
		
		// $designation = isset($form_state['values']['designation']) && !empty($form_state['values']['designation']) ? $form_state['values']['designation'] : '';
		// $dob = isset($form_state['values']['dob']) && !empty($form_state['values']['dob']) ? $form_state['values']['dob'] : '';
		
		// $pay_band_id = isset($form_state['values']['pay_band']) && !empty($form_state['values']['pay_band']) ? $form_state['values']['pay_band'] : 0;
	    $rhe_flat_type = rhe_flat_type_rst_pay_band_id($data->pay_band_id);
		// echo $rhe_flat_type;die;
	    // $reason = isset($form_state['values']['reason']) && !empty($form_state['values']['reason']) ? $form_state['values']['reason'] : '';
		
		// $chk_permanent_address = isset($form_state['values']['chk_permanent_address']) ? $form_state['values']['chk_permanent_address'] : '';
		
		$form['form_begin'] = array(
			'#type' => 'markup',
			'#markup' => '<div class="online_application">',
			'#weight' => -2
		);

		$form['app_uid'] = array(
				'#type' => 'hidden',
				'#value' => $data->uid,
				'#attributes' => array('readonly' => 'readonly'),
		);

		$form['housing_applicant_id'] = array(
				'#type' => 'hidden',
				'#value' => $data->housing_applicant_id,
				'#attributes' => array('readonly' => 'readonly'),
		);
		$form['housing_official_detail_id'] = array(
				'#type' => 'hidden',
				'#value' => $data->applicant_official_detail_id,
				'#attributes' => array('readonly' => 'readonly'),
		);

		$form['housing_online_application_id'] = array(
				'#type' => 'hidden',
				'#value' => $data->online_application_id,
				'#attributes' => array('readonly' => 'readonly'),
		);
		// echo "<pre>";print_r($form);die;
		//personal information
		$form['prrsonal_info'] = array(
			'#type' => 'fieldset',
			'#title' => t('Personal Information(According to Service Book)'),
			'#collapsible' => FALSE, // Added
			'#collapsed' => FALSE,  // Added
		);
						
		$form['prrsonal_info']['applicant_name'] = array(
			'#title' => t('Applicant\'s Name'),
			'#type' => 'textfield',
			'#default_value' => $data->applicant_name,
			'#prefix' => '<div class="row"><div class="col-md-6"><div class="form-floating">',
			'#suffix' => '</div></div>',
			'#element_validate' => array('element_validate_alphabatic_fullstop'), 
			'#required' => TRUE,
			'#attributes' => array('oninput'=>"this.value=this.value.toUpperCase()", 'class'=>array('form-control')),
		);
		
		$form['prrsonal_info']['applicant_father_name'] = array(
			'#title' => t('Father / Husband Name'),
			'#type' => 'textfield',
			'#default_value' => $data->guardian_name,
			'#prefix' => '<div class="col-md-6"><div class="form-floating">',
			'#suffix' => '</div></div></div>',
			'#element_validate' => array('element_validate_alphabatic_fullstop'), 
			'#required' => TRUE,
			'#attributes' => array('oninput'=>"this.value=this.value.toUpperCase()", 'class'=>array('form-control')),
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
			'#default_value' => $data->permanent_street,
			//'#required' => TRUE,
			'#prefix' => '<div class="row"><div class="col-md-6"><div class="form-floating">',
			'#suffix' => '</div></div>',
			'#element_validate' => array('element_validate_streetaddress'), 
			'#attributes' => array('oninput'=>"this.value=this.value.toUpperCase()", 'class'=>array('form-control')),
		);
		$form['prrsonal_info']['permanent_address']['permanent_city_town_village'] = array(
			'#title' => t('City / Town / Village'),
			'#type' => 'textfield',
			'#default_value' => $data->permanent_city_town_village,
			//'#required' => TRUE,
			'#prefix' => '<div class="col-md-6"><div class="form-floating">',
			'#suffix' => '</div></div></div>',
			'#element_validate' => array('element_validate_alphabatic_text'), 
			'#attributes' => array('oninput'=>"this.value=this.value.toUpperCase()", 'class'=>array('form-control')),
		);
		$form['prrsonal_info']['permanent_address']['permanent_post_office'] = array(
			'#title' => t('Post Office'),
			'#type' => 'textfield',
			'#default_value' => $data->permanent_post_office,
			//'#required' => TRUE,
			'#prefix' => '<div class="row"><div class="col-md-4"><div class="form-floating">',
			'#suffix' => '</div></div>',
			'#element_validate' => array('element_validate_alphabatic_text'), 
			'#attributes' => array('oninput'=>"this.value=this.value.toUpperCase()", 'class'=>array('form-control')),
		);
		$form['prrsonal_info']['permanent_address']['permanent_district'] = array(
			'#title' => t('District'),
			'#type' => 'select',
			'#default_value' => $data->permanent_district,
			'#options' => district_list(),
			//'#required' => TRUE,
			'#prefix' => '<div class="col-md-4"><div class="form-floating">',
			'#suffix' => '</div></div>',
			// '#select2' => array(
			// 	'width' => '500',
			// 	//'placeholder' => '- Select -',
			// 	'allowClear' => TRUE
   //      	)
		);
		$form['prrsonal_info']['permanent_address']['permanent_pincode'] = array(
			'#title' => t('Pincode'),
			'#type' => 'textfield',
			'#default_value' => $data->permanent_pincode,
			'#maxlength' => 6,
			//'#required' => TRUE,
			'#element_validate' => array('element_validate_numeric_positive'), 
			'#prefix' => '<div class="col-md-4"><div class="form-floating">',
			'#suffix' => '</div></div></div>',
			'#attributes' => array('class' => array('numeric_positive'), 'class'=>array('form-control')),
		);
	//end
	
		// Turned off checkbox 23-04-2025	

		// $form['prrsonal_info']['chk_permanent_address'] = array(
		// 	/*'#type' => 'checkboxes',
		// 	'#title' => '',
		// 	'#options' => array(1 => t('Check if Your Present Address is same as Your Permanent Address.')),*/
			
		// 	'#type' => 'checkbox',
		// 	'#title' => 'Check if Your Present Address is same as Your Permanent Address.',
		// 	'#default_value' => '',
		// 	//'#attributes' => array('id' => 'chk_present_address'),     //for this ajax callback not working.
		// 	'#ajax' => array(
		// 			   'event' => 'change',
		// 			   'callback' => '_reload_present_address',
		// 			   'wrapper' => 'replace_present_address',
		// 			   'effect' => 'fade',
		// 			   'progress' => array(
		// 				  'type' => '',
		// 				  'message' => '',
		// 				), 
		// 	 ),
	    // );
		
		// if($chk_permanent_address != 1) {
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
				'#default_value' => $data->present_street,
				//'#required' => TRUE,
				'#prefix' => '<div class="row"><div class="col-md-6" id="reload_present_street"><div class="form-floating">',
				'#suffix' => '</div></div>',
				'#element_validate' => array('element_validate_streetaddress'), 
				'#attributes' => array('oninput'=>"this.value=this.value.toUpperCase()", 'class'=>array('form-control')), //'id' => 'present_street', 
			);
			$form['prrsonal_info']['present_address']['present_city_town_village'] = array(
				'#title' => t('City / Town / Village'),
				'#type' => 'textfield',
				'#default_value' => $data->present_city_town_village,
				//'#required' => TRUE,
				'#prefix' => '<div class="col-md-6" id="reload_present_city_town_village"><div class="form-floating">',
				'#suffix' => '</div></div></div>',
				'#element_validate' => array('element_validate_alphabatic_text'), 
				'#attributes' => array('oninput'=>"this.value=this.value.toUpperCase()", 'class'=>array('form-control')), //'id' => 'present_city_town_village', 
			);
			$form['prrsonal_info']['present_address']['present_post_office'] = array(
				'#title' => t('Post Office'),
				'#type' => 'textfield',
				'#default_value' => $data->present_post_office,
				//'#required' => TRUE,
				'#prefix' => '<div class="row"><div class="col-md-4" id="reload_present_post_office"><div class="form-floating">',
				'#suffix' => '</div></div>',
				'#element_validate' => array('element_validate_alphabatic_text'), 
				'#attributes' => array('oninput'=>"this.value=this.value.toUpperCase()", 'class'=>array('form-control')), //'id' => 'present_post_office', 
			);
			$form['prrsonal_info']['present_address']['present_district'] = array(
				'#title' => t('District'),
				'#type' => 'select',
				'#default_value' => $data->present_district,
				'#options' => district_list(),
				//'#required' => TRUE,
				'#prefix' => '<div class="col-md-4" id="reload_present_district"><div class="form-floating">',
				'#suffix' => '</div></div>',
				'#attributes' => array('class'=>array('form-control')),
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
				'#default_value' => $data->present_pincode,
				'#maxlength' => 6,
				//'#required' => TRUE,
				'#element_validate' => array('element_validate_numeric_positive'), 
				'#prefix' => '<div class="col-md-4" id="reload_present_pincode"><div class="form-floating">',
				'#suffix' => '</div></div></div>',
				'#attributes' => array('class' => array('numeric_positive'),'class'=>array('form-control')), //'id' => 'present_pincode', 
			);

			// Turned off 23-04-2025	

		// } else {
			// $form['prrsonal_info']['present_address'] = array(
			// 	'#prefix' => '<div id="reload_present_address">',
			// 	'#suffix' => '</div>',
			// );
			/*$form['prrsonal_info']['present_address']['present_street'] = array(
				'#prefix' => '<div id="reload_present_street">',
				'#suffix' => '</div>',
			);
			$form['prrsonal_info']['present_address']['present_city_town_village'] = array(
				'#prefix' => '<div id="reload_present_city_town_village">',
				'#suffix' => '</div>',
			);
			$form['prrsonal_info']['present_address']['present_post_office'] = array(
				'#prefix' => '<div id="reload_present_post_office">',
				'#suffix' => '</div>',
			);
			$form['prrsonal_info']['present_address']['present_district'] = array(
				'#prefix' => '<div id="reload_present_district">',
				'#suffix' => '</div>',
			);
			$form['prrsonal_info']['present_address']['present_pincode'] = array( 
				'#prefix' => '<div id="reload_present_pincode">',
				'#suffix' => '</div>',
			);*/
		// }
	//end
	
		$form['prrsonal_info']['mobile'] = array(
			'#title' => t('Mobile no'),
			'#type' => 'textfield',
			'#default_value' => $data->mobile_no,
			'#maxlength' => 10,
			//'#required' => TRUE,
			'#prefix' => '<div class="row"><div class="col-md-4"><div class="form-floating">',
			'#suffix' => '</div></div>',
			'#element_validate' => array('element_validate_mobile'),
			'#attributes' => array('id' => 'mobile_no', 'class'=>array('form-control')),
		);
		$form['prrsonal_info']['email'] = array(
			'#title' => t('Email ID'),
			'#type' => 'textfield',
			'#default_value' => $data->mail,
			'#prefix' => '<div class="col-md-4"><div class="form-floating">',
			'#suffix' => '</div></div>',
			'#element_validate' => array('element_validate_email'),
			'#attributes' => array('oninput'=>"this.value=this.value.toLowerCase()", 'class'=>array('form-control')),

		);
		
		$form['prrsonal_info']['dob'] = array(
			'#title' => t('Date of Birth(According to Service Book)'),
			'#type' => 'textfield',
			'#default_value' => $dob,
			'#attributes' => array('readonly' => 'readonly', 'id' => 'edit-dob', 'class'=>array('form-control')),
			'#required' => TRUE,
			//'#validated' => TRUE,
			'#prefix' => '<div class="col-md-4"><div class="form-floating">',
			'#suffix' => '</div></div></div>',
			'#element_validate' => array('element_validate_date', 'element_validate_age'),
		);
		
		$form['prrsonal_info']['gender'] = array(
			'#title' => t('Gender'),
			'#type' => 'radios',
			'#default_value' => $data->gender,
			'#options' => array('M' => 'Male', 'F' => 'Female'),
			'#attributes' => array('class' => array('gender')),
			'#default_value' => 'M',
			'#prefix' => '<div class="col-md-4"><div class="">',
			'#suffix' => '</div></div>',
			'#required' => TRUE,
		);
		
		// applicant's official information
		$form['official_info'] = array(
			'#type' => 'fieldset',
			'#title' => t('Applicant\'s Official Information'),
			'#collapsible' => FALSE, // Added
			'#collapsed' => FALSE,  // Added
		);
		$form['official_info']['hrms_id'] = array(
			'#title' => t('Employee HRMS ID'),
			'#type' => 'textfield',
			'#default_value' => $data->hrms_id,
			'#maxlength' => 10,
			'#attributes' => array('class' => array('numeric_positive'), 'class'=>array('form-control')),
			'#prefix' => '<div class="row"><div class="col-md-4"><div class="form-floating">',
			'#suffix' => '</div></div>',
			'#element_validate' => array('element_validate_hrms_id'),
			//'#required' => TRUE,
		);
		$form['official_info']['app_designation'] = array(
			'#title' => t('Designation'),
			'#type' => 'textfield',
			'#default_value' => $data->applicant_designation,
			'#required' => TRUE,
			'#prefix' => '<div class="col-md-4"><div class="form-floating">',
			'#suffix' => '</div></div>',
			'#element_validate' => array('element_validate_textarea'), 
			'#attributes' => array('oninput'=>"this.value=this.value.toUpperCase()", 'class'=>array('form-control')),
			//'#attributes' => array('id' => 'edit-app-designation', 'oncopy' => "return false", 'onpaste' => "return false"),
		);
		
		$form['official_info']['pay_band'] = array(
			'#title' => t('Basic Pay Range'),
			'#type' => 'select',
			'#default_value' => $data->pay_band_id,
			'#options' => pay_band_list(),
			'#required' => TRUE,
			'#validated' => TRUE,
			'#prefix' => '<div class="col-md-4"><div class="form-floating">',
			'#suffix' => '</div></div></div>',
		);
		$form['official_info']['pay_in'] = array(
			'#title' => t('Basic Pay'),
			'#type' => 'textfield',
			'#default_value' => $data->pay_in_the_pay_band,
			'#required' => TRUE,
			'#element_validate' => array('element_validate_numeric_positive'), 
			'#prefix' => '<div class="row"><div class="col-md-4"><div class="form-floating">',
		 	'#suffix' => '</div></div>',
			'#attributes' => array('class' => array('numeric_positive form-control')),
		);
		
		/*$form['official_info']['grade_pay'] = array(
			'#title' => t('Grade Pay'),
			'#type' => 'textfield',
			//'#required' => TRUE,
			'#element_validate' => array('element_validate_numeric_positive'), 
			'#prefix' => '<div class="four">',
			'#suffix' => '</div>',
			'#attributes' => array('class' => array('numeric_positive')),
		);
		
		/*$form['official_info']['gpf_no'] = array(
			'#title' => t('GPF No.'),
			'#type' => 'textfield',
			//'#required' => TRUE,
			'#element_validate' => array('gpf_no_validate'), 
			'#prefix' => '<div class="three">',
			'#suffix' => '</div>',
			'#attributes' => array('oninput'=>"this.value=this.value.toUpperCase()"),
		);*/
		
		$form['official_info']['app_posting_place'] = array(
			'#title' => t('Place of Posting'),
			'#type' => 'textfield',
			'#default_value' => $data->applicant_posting_place,
			'#required' => TRUE,
			'#prefix' => '<div class="col-md-4"><div class="form-floating">',
			'#suffix' => '</div></div>',
			'#element_validate' => array('element_validate_alphabatic_text'), 
			'#attributes' => array('oninput'=>"this.value=this.value.toUpperCase()", 'class' => array(' form-control')),
		); 
		$form['official_info']['app_headquarter'] = array(
			'#title' => t('Headquarter'),
			'#type' => 'textfield',
			'#default_value' => $data->applicant_headquarter,
			//'#required' => TRUE,
			'#prefix' => '<div class="col-md-4"><div class="form-floating">',
			'#suffix' => '</div></div></div>',
			'#element_validate' => array('element_validate_alphabatic_text'), 
			'#attributes' => array('oninput'=>"this.value=this.value.toUpperCase()", 'class' => array('form-control')),
		);
		$form['official_info']['doj'] = array(
			'#title' => t('Date of Joining'),
			'#type' => 'textfield',
			'#default_value' => $doj,
			'#attributes' => array('readonly' => 'readonly', 'id' => 'edit-doj', 'class' => array('numeric_positive form-control')),
			//'#required' => TRUE,
			'#prefix' => '<div class="row"><div class="col-md-6"><div class="form-floating">',
			'#suffix' => '</div></div>',
			'#element_validate' => array('element_validate_date', 'element_validate_joining_age'),
		);
		
		$form['official_info']['dor'] = array(
			'#title' => t('Date of Retirement(According to Service Book)'),
			'#type' => 'textfield',
			'#default_value' => $dor,
			//'#attributes' => array('readonly' => 'readonly'),
			'#attributes' => array('readonly' => 'readonly', 'id' => 'edit-dor', 'class' => array('numeric_positive form-control')),
			'#required' => TRUE,
			'#prefix' => '<div class="col-md-6"><div class="form-floating">',
			'#suffix' => '</div></div></div>',
			'#element_validate' => array('element_validate_date'),
		);
		
		
		// applicant's office address
		$form['office_add'] = array(
			'#type' => 'fieldset',
			'#title' => t('Name and Address of the Office'),
			'#collapsible' => FALSE, // Added
			'#collapsed' => FALSE,  // Added
		);
		$form['office_add']['office_name'] = array(
			'#title' => t('Name of the Office'),
			'#type' => 'textfield',
			'#default_value' => $data->office_name,
			'#required' => TRUE,
			'#prefix' => '<div class="row"><div class="col-md-4"><div class="form-floating">',
			'#suffix' => '</div></div>',
			'#element_validate' => array('element_validate_textarea'), 
			'#attributes' => array('oninput'=>"this.value=this.value.toUpperCase()", 'class' => array('form-control')),
		);
		
		$form['office_add']['office_street'] = array(
			'#title' => t('Address'),
			'#type' => 'textfield',
			'#default_value' => $data->office_street,
			'#required' => TRUE,
			'#prefix' => '<div class="col-md-4"><div class="form-floating">',
			'#suffix' => '</div></div>',
			'#element_validate' => array('element_validate_streetaddress'),
			'#attributes' => array('oninput'=>"this.value=this.value.toUpperCase()", 'class' => array('form-control')), 
		);
		$form['office_add']['office_city'] = array(
			'#title' => t('City / Town / Village'),
			'#type' => 'textfield',
			'#default_value' => $data->office_city_town_village,
			'#required' => TRUE,
			'#prefix' => '<div class="col-md-4"><div class="form-floating">',
			'#suffix' => '</div></div></div>',
			'#element_validate' => array('element_validate_alphabatic_text'), 
			'#attributes' => array('oninput'=>"this.value=this.value.toUpperCase()", 'class' => array('form-control')),
		);
		$form['office_add']['office_post_office'] = array(
			'#title' => t('Post Office'),
			'#type' => 'textfield',
			'#default_value' => $data->office_post_office,
			//'#required' => TRUE,
			'#prefix' => '<div class="row"><div class="col-md-3"><div class="form-floating">',
			'#suffix' => '</div></div>',
			'#element_validate' => array('element_validate_alphabatic_text'), 
			'#attributes' => array('oninput'=>"this.value=this.value.toUpperCase()", 'class' => array('form-control')),
		);
		$form['office_add']['office_district'] = array(
			'#title' => t('District'),
			'#type' => 'select',
			'#default_value' => $data->office_district,
			'#options' => district_list(),
			'#required' => TRUE,
			'#prefix' => '<div class="col-md-3"><div class="form-floating">',
			'#suffix' => '</div></div>',
			'#attributes' => array('class' => array('numeric_positive form-control')),
			// '#select2' => array(
			// 	'width' => '500',
			// 	//'placeholder' => '- Select -',
			// 	'allowClear' => TRUE
        	//)
		);
		$form['office_add']['office_pincode'] = array(
			'#title' => t('Pincode'),
			'#type' => 'textfield',
			'#default_value' => $data->office_pin_code,
			'#maxlength' => 6,
			'#required' => TRUE,
			'#element_validate' => array('element_validate_numeric_positive'), 
			'#prefix' => '<div class="col-md-3"><div class="form-floating">',
			'#suffix' => '</div></div>',
			'#attributes' => array('class' => array('numeric_positive'), 'class' => array('form-control')),
		);
		$form['office_add']['office_phone_no'] = array(
			'#title' => t('Phone No.(With STD Code)'),
			'#type' => 'textfield',
			'#default_value' => $data->office_phone_no,
			'#maxlength' => 15,
			//'#required' => TRUE,
			'#element_validate' => array('element_validate_telephoneno'), 
			'#prefix' => '<div class="col-md-3"><div class="form-floating">',
			'#suffix' => '</div></div></div>',
			'#attributes' => array('class' => array('numeric_positive'), 'class' => array('form-control')),
		);
		
		// ddo details
		$form['ddo'] = array(
			'#type' => 'fieldset',
			'#title' => t('DDO with full address'),
			'#collapsible' => FALSE, // Added
			'#collapsed' => FALSE,  // Added
		);
		$form['ddo']['district'] = array(
			'#title' => t('DDO District'),
			'#type' => 'select',
			'#default_value' => $data->district_code,
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
		);
		
		
		$form['ddo']['designation'] = array(
			'#title' => t('DDO Designation'),
			'#type' => 'select',
			'#default_value' => $data->ddo_id,
			//'#options' => array('- Select -'),
			//'#options' => ddo_desig_list($district),
			'#options' => ddo_desig_list($data->district_code),
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
			//'#default_value' => $designation,
			'#required' => TRUE,
			'#validated' => TRUE,
			'#prefix' => '<div id="replace_designation" class="col-md-6"><div class="form-floating">',
			'#suffix' => '</div></div>',
		);
		
		$form['ddo']['address'] = array(
			'#title' => t('DDO Address'),
			//'#default_value' => $data->address,
			'#type' => 'textarea',
			'#attributes' => array('readonly' => 'readonly', 'class' => array('form-control')),
			'#prefix' => '<div id="replace_ddo_address" class="col-md-12"><div class="form-floating">',
			'#suffix' => '</div></div>',
			
			//'#required' => TRUE,
			//'#validated' => TRUE,
		);
		
		//allotment type and category
		$form['allotment_reason'] = array(
			'#type' => 'fieldset',
			//'#default_value' => $data->allotment_reason,
			'#title' => t('Allotment Category'),
			'#collapsible' => FALSE, // Added
			'#collapsed' => FALSE,  // Added
		);
		
		$form['allotment_reason']['rhe_flat_type'] = array(
			'#title' => t('Flat TYPE'),
			'#type' => 'textfield',
			'#default_value' => $rhe_flat_type,
			'#attributes' => array('readonly' => 'readonly', 'class' => array('form-control')),
			'#required' => TRUE,
			'#prefix' => '<div class="row"><div class="col-md-6" id="replace_rhe_flat_type" ><div class="form-floating">',
			'#suffix' => '</div></div>',
			'#element_validate' => array('element_validate_flat_type'),
		 );
	     
		 
		 $form['allotment_reason']['reason'] = array(
			'#title' => t('Allotment Category'),
			'#type' => 'select',
			//'#options' => array(''=>'- Select -'),
			'#options' => allotment_category_list(trim($rhe_flat_type)),
			'#default_value' => $data->allotment_category,
			'#required' => TRUE,
			'#validated' => TRUE,
			'#prefix' => '<div id="replace_allotment_category" class="col-md-6"><div class="form-floating">',
			'#suffix' => '</div></div></div>',
			'#attributes' => array('class' => array('form-control')),
		 );
		 
		 $form['allotment_reason']['doa'] = array(
			'#title' => t('Date of Application'),
			'#type' => 'textfield',
			'#default_value' => $doa,
			'#attributes' => array('readonly' => 'readonly', 'id' => 'edit-doa','class' => array('form-control')),
			'#required' => TRUE,
			//'#validated' => TRUE,
			'#prefix' => '<div class="row"><div class="col-md-4"><div class="form-floating">',
			'#suffix' => '</div></div>',
			'#element_validate' => array('element_validate_date'),
		);
		
		$form['allotment_reason']['computer_serial_no'] = array(
			'#title' => t('Computer Serial No.'),
			'#type' => 'textfield',
			'#default_value' => $data->computer_serial_no,
			'#required' => TRUE,
			'#element_validate' => array('element_validate_numeric_positive'),
			'#prefix' => '<div id="computr_serial_no" class="col-md-4"><div class="form-floating">',
			'#suffix' => '</div></div>',
			'#attributes' => array('class' => array('numeric_positive form-control'), 'readonly' => 'readonly'),
		);
		/* added by debaleena 01-08-2024 */   
		$form['allotment_reason']['confirm_computer_serial_no'] = array(
			// '#title' => t('Confirm Computer Serial No.'),
			// '#type' => 'password',
			// '#default_value' => $data->computer_serial_no,
			// '#required' => TRUE,
			// '#element_validate' => array('element_validate_numeric_positive'),
			// '#prefix' => '<div id="confirm_computr_serial_no" class="col-md-4"><div class="form-floating">',
			// '#suffix' => '</div></div></div>',
			// '#attributes' => array('class' => array('numeric_positive form-control')),
			'#type' => 'hidden',
			'#value' => $data->computer_serial_no,
			'#attributes' => array('readonly' => 'readonly'),
		);
		// added on 19-06-2025 by moumita
		$form['allotment_reason']['physical_application_no'] = array(
			'#title' => t('Physical Application Number'),
			'#type' => 'textfield',
			'#default_value' => $data->physical_application_no,
			'#required' => TRUE,
			// '#element_validate' => array('element_validate_numeric_positive'),
			'#prefix' => '<div class="row"><div id="" class="col-md-4"><div class="form-floating">',
			'#suffix' => '</div></div></div>',
			'#attributes' => array('class' => array('numeric_positive form-control')),
		);


		 // Message area for validation feedback
		 $form['allotment_reason']['comp_ser_no_message'] = array(
			'#type' => 'markup',
			//'#default_value' => $data->comp_ser_no_message,
			'#markup' => '<div id="comp_ser_no_message" class="form-floating"></div>', // Use form-text for styling
			'#prefix' => '<div class="row"><div class="col-md-12">', // Ensure it aligns with the rest of the form
			'#suffix' => '</div></div>', // Close the div properly
		  );
		
		/* end */

		
		$form['allotment_reason']['remarks'] = array(
			'#title' => t('Remarks'),
			'#type' => 'textarea',
			'#default_value' => $data->remarks,
			'#prefix' => '<div class="col-md-12"><div class="form-floating">',
			'#suffix' => '</div></div>',
			'#element_validate' => array('element_validate_textarea'), 
			'#attributes' => array('class' => array('form-control')),
		);
		 
		 $form['official_info']['pay_band']['#ajax'] = array(
					   'event' => 'change',
					   'callback' => '_reload_allotment_category',
					   'effect' => 'fade',
					   'progress' => array(
						  'type' => '',
						  'message' => '',
						 )
		);
		
		$form['#attributes'] = array('onsubmit' => 'return validate_existing_applicant_form()');
		
		$form['form_end'] = array(
			'#type' => 'markup',
			'#markup' => '</div>',
		);
		
		$form['submit_button']=array(
				'#type'=>'submit',
				'#value'=>t('Submit'),
				'#attributes' => array('class'=>array('btn bg-primary btn-sm px-5 rounded-pill text-white fw-bolder')),
				'#suffix' => '<div class="mb-5 pb-3"></div>',
			);
								
		
		return $form; 
	}
	
	
	function existing_applicant_form_validate($form, &$form_state) {
		
		if($form_state['values']['pay_band'] == '') {
  			form_set_error('district', t('Please select payband.'));
		}
		if($form_state['values']['district'] == '') {
  			form_set_error('district', t('Please select district.'));
		}
		if($form_state['values']['designation'] == '') {
  			form_set_error('designation', t('Please select ddo designation.'));
		}
		if($form_state['values']['reason'] == '') {
  			form_set_error('reason', t('Please select allotment reason.'));
		}
		
		if(!empty($form_state['values']['mobile']) && is_numeric($form_state['values']['mobile'])) {
		   
			 $mobile = trim($form_state['values']['mobile']);
			 
			 $query = db_select('housing_applicant','applicant');
			 $query->addExpression('mobile_no','mobile');
			 $query->condition('mobile_no', $mobile);
			 $result =$query->execute();
			 
			 
			 if($result->rowCount() > 0)
			 	form_set_error('mobile', 'This mobile no. already exist.');
		
		}
		
		if(!empty($form_state['values']['email'])) {
		   
			$email = trim($form_state['values']['email']);
			
			$query = db_select('users','u');
			$query->addExpression('mail','email');
			$query->condition('mail', $email);
			$result =$query->execute();
			
			/*if (!valid_email_address($email)) {
				form_set_error('[email]', t('The email address appears to be invalid.'));
			}*/
			
			if($result->rowCount() > 0)
				form_set_error('email', 'This email address already exist.');
		
		}
		
		if(!empty($form_state['values']['hrms_id']) && is_numeric($form_state['values']['hrms_id'])) {
		   
			 $hrms_id = trim($form_state['values']['hrms_id']);
			 
			 $query = db_select('housing_applicant_official_detail','haod');
			 $query->fields('haod');
			 $query->condition('hrms_id', $hrms_id);
			 $result =$query->execute();
			 
			 
			 if($result->rowCount() > 0)
			 	form_set_error('hrms_id', 'This Employee HRMS ID already exist.');
		
		}
		
		
		/*if($form_state['values']['pay_band'] == 1) {
			if($form_state['values']['pay_in'] < 0 || $form_state['values']['pay_in'] > 7999) {
				form_set_error('pay_in', t('Please enter proper Pay in the Pay Band.'));	
			}
		}
		else if($form_state['values']['pay_band'] == 2) {
			if($form_state['values']['pay_in'] < 8000 || $form_state['values']['pay_in'] > 12499) {
				form_set_error('pay_in', t('Please enter proper Pay in the Pay Band.'));	
			}
		}
		else if($form_state['values']['pay_band'] == 3) {
			if($form_state['values']['pay_in'] < 12500 || $form_state['values']['pay_in'] > 15999) {
				form_set_error('pay_in', t('Please enter proper Pay in the Pay Band.'));	
			}
		}
		else if($form_state['values']['pay_band'] == 4) {
			if($form_state['values']['pay_in'] < 16000) {
				form_set_error('pay_in', t('Please enter proper Pay in the Pay Band.'));	
			}
			else {
				if($form_state['values']['grade_pay'] != '') {
					if($form_state['values']['grade_pay'] < 0 || $form_state['values']['grade_pay'] > 7599) {
						form_set_error('grade_pay', t('Please enter proper Grade Pay.'));	
					}
				}		
			}
		}
		else if($form_state['values']['pay_band'] == 5) {
			if($form_state['values']['pay_in'] < 16000) {
				form_set_error('pay_in', t('Please enter proper Pay in the Pay Band.'));	
			}
			else {
				if($form_state['values']['grade_pay'] != '') {
					if($form_state['values']['grade_pay'] < 7600) {
						form_set_error('grade_pay', t('Please enter proper Grade Pay.'));	
					}
				}		
			}
		}*/
		
	}
	
	
	function existing_applicant_edit_form_submit($form, &$form_state) {
		
		
		$computer_serial_no = trim($form_state['values']['computer_serial_no']);
		$confirm_computer_serial_no = trim($form_state['values']['confirm_computer_serial_no']);

		if($computer_serial_no == $confirm_computer_serial_no)
		{
			$word_c = str_word_count(trim($form_state['values']['applicant_name']));
			$splitter = " ";
			$pieces = explode($splitter, trim($form_state['values']['applicant_name']));
			
			if($word_c < 2) {
				$name = strtolower(substr($pieces[0], 0, 3));
			}
			else if($word_c == 2) {
				if($pieces[0] == 'Dr.' || $pieces[0] == 'Dr' || $pieces[0] == 'dr.' || $pieces[0] == 'dr' || $pieces[0] == 'DR.' || $pieces[0] == 'DR') {
					$name = strtolower(substr($pieces[1], 0, 3));
				}
				else {
					$name = strtolower(substr($pieces[0], 0, 3)).strtolower(substr($pieces[1], 0, 3));
				}
			}
			else {
				if($pieces[0] == 'Dr.' || $pieces[0] == 'Dr' || $pieces[0] == 'dr.' || $pieces[0] == 'dr' || $pieces[0] == 'DR.' || $pieces[0] == 'DR') {
					$name = strtolower(substr($pieces[1], 0, 3)).strtolower(substr($pieces[2], 0, 3));
				}
				else {
					$name = strtolower(substr($pieces[0], 0, 3)).strtolower(substr($pieces[1], 0, 3));
				}
			}
			
			// $username = str_replace(".","",$name).rand(1, 100000); // off on 19-06-2025 by moumita
			// added on 19-06-2025 by moumita
			$username = str_replace(".","",$name).'_'.$form_state['values']['physical_application_no'].'_'.$form_state['values']['computer_serial_no'];


			if($form_state['values']['hrms_id'] != '')
			{
				$newUser	   	   = array();
				$newUser['name']   = trim($form_state['values']['hrms_id']);
				$newUser['pass']   = trim($form_state['values']['hrms_id']);
				/*$str[0].$str[1].trim($form_state['values']['username']).'@'.$dob_t.strrev($str1[0].$str1[1]);*/
				if($form_state['values']['email'] !=''){										
					$newUser['mail']   = trim($form_state['values']['email']);
				}	
				$newUser['status'] = 1;
			}else{
				/*insert into user table*/
				$newUser	   	   = array();
				$newUser['name']   = $username;
				$newUser['pass']   = $username;
				/*$str[0].$str[1].trim($form_state['values']['username']).'@'.$dob_t.strrev($str1[0].$str1[1]);*/
				if($form_state['values']['email'] != ''){
					$newUser['mail']   = trim($form_state['values']['email']);
				}
				$newUser['status'] = 1;
				
			}

			$hidden_users_uid = $form_state['values']['app_uid'];
			$hidden_housing_applicant_id = $form_state['values']['housing_applicant_id'];
			$hidden_online_application_id = $form_state['values']['housing_online_application_id'];
			$hidden_official_detail_id = $form_state['values']['housing_official_detail_id'];
			// echo $hidden_official_detail_id;die;
			db_update('users')
			->fields($newUser)
			->condition('uid', $hidden_users_uid)
			->execute();

			/*update into housing_applicant table*/
			
			$applicant_personal_detail_arr = array();
			$applicant_personal_detail_arr['uid'] 				=  $hidden_users_uid;
			$applicant_personal_detail_arr['applicant_name'] 	=  trim($form_state['values']['applicant_name']);
			$applicant_personal_detail_arr['guardian_name'] 		=  trim($form_state['values']['applicant_father_name']);
			$applicant_personal_detail_arr['date_of_birth'] 		=  trim(implode('-', array_reverse(explode('/', $form_state['values']['dob']))));
			$applicant_personal_detail_arr['gender'] 			=  trim($form_state['values']['gender']);
			
			if($form_state['values']['mobile'] != '')
				$applicant_personal_detail_arr['mobile_no'] 			=  trim($form_state['values']['mobile']);
			
			if($form_state['values']['permanent_street'] != '')	
				$applicant_personal_detail_arr['permanent_street']  =  strtoupper(trim($form_state['values']['permanent_street']));
			if($form_state['values']['permanent_city_town_village'] != '')
				$applicant_personal_detail_arr['permanent_city_town_village']  =  strtoupper(trim($form_state['values']['permanent_city_town_village']));
			if($form_state['values']['permanent_post_office'] != '')
				$applicant_personal_detail_arr['permanent_post_office']  =  strtoupper(trim($form_state['values']['permanent_post_office']));
			if($form_state['values']['permanent_district'] != '')
				$applicant_personal_detail_arr['permanent_district']  =  trim($form_state['values']['permanent_district']);
			if($form_state['values']['permanent_pincode'] != '')
				$applicant_personal_detail_arr['permanent_pincode']  =  trim($form_state['values']['permanent_pincode']);
			
			if(($form_state['values']['permanent_street'] != '') && ($form_state['values']['permanent_city_town_village'] != '') && ($form_state['values']['permanent_post_office'] != '') && ($form_state['values']['permanent_district'] != '') && ($form_state['values']['permanent_pincode'] != ''))
				// $applicant_personal_detail_arr['permanent_present_same']  =  trim($form_state['values']['chk_permanent_address']);
			
			// if(trim($form_state['values']['chk_permanent_address']) != 1) {   // // Turned off 23-04-2025	
				//$applicant_personal_detail_arr['permanent_present_same'] = 0;
			if($form_state['values']['present_street'] != '')
				$applicant_personal_detail_arr['present_street']  =  strtoupper(trim($form_state['values']['present_street']));
			if($form_state['values']['present_city_town_village'] != '')
				$applicant_personal_detail_arr['present_city_town_village']  =  strtoupper(trim($form_state['values']['present_city_town_village']));
			if($form_state['values']['present_post_office'] != '')
				$applicant_personal_detail_arr['present_post_office']  =  strtoupper(trim($form_state['values']['present_post_office']));
			if($form_state['values']['present_district'] != '')
				$applicant_personal_detail_arr['present_district']  =  trim($form_state['values']['present_district']);
			if($form_state['values']['present_pincode'] != '')
				$applicant_personal_detail_arr['present_pincode']  =  trim($form_state['values']['present_pincode']);
			
			
			db_update('housing_applicant')
			->fields($applicant_personal_detail_arr)
			->condition('housing_applicant_id', $hidden_housing_applicant_id)
			->execute();

			
			/*update applicant_official_detail  data*/
			$app_off_detail_arr = array();
			$app_off_detail_arr['uid']  =  $hidden_users_uid;
			if($form_state['values']['hrms_id'] != '')
				$app_off_detail_arr['hrms_id']  =  trim($form_state['values']['hrms_id']);
			
			$app_off_detail_arr['applicant_designation']  =  strtoupper(trim($form_state['values']['app_designation']));
			$app_off_detail_arr['pay_band_id']  =  trim($form_state['values']['pay_band']);
			$app_off_detail_arr['pay_in_the_pay_band']  =  trim($form_state['values']['pay_in']);
			$app_off_detail_arr['applicant_posting_place']  =  strtoupper(trim($form_state['values']['app_posting_place']));
			if($form_state['values']['app_headquarter'] != '')
				$app_off_detail_arr['applicant_headquarter']  =  strtoupper(trim($form_state['values']['app_headquarter']));
			
			if($form_state['values']['doj'] != '')
				$app_off_detail_arr['date_of_joining']  =  trim( implode('-', array_reverse(explode('/', $form_state['values']['doj']))));
			
			$app_off_detail_arr['date_of_retirement']  =  trim( implode('-', array_reverse(explode('/', $form_state['values']['dor']))));
			$app_off_detail_arr['office_name']  =  strtoupper(trim($form_state['values']['office_name']));
			$app_off_detail_arr['office_street']  =  strtoupper(trim($form_state['values']['office_street']));
			$app_off_detail_arr['office_city_town_village']  =  strtoupper(trim($form_state['values']['office_city']));
			if($form_state['values']['office_post_office'] != '')
				$app_off_detail_arr['office_post_office']  =  strtoupper(trim($form_state['values']['office_post_office']));
			
			$app_off_detail_arr['office_district']  =  trim($form_state['values']['office_district']);
			$app_off_detail_arr['office_pin_code']  =  trim($form_state['values']['office_pincode']);
			if($form_state['values']['office_phone_no'] != '')
				$app_off_detail_arr['office_phone_no']  =  trim($form_state['values']['office_phone_no']);
			
			$app_off_detail_arr['ddo_id']  =   trim($form_state['values']['designation']);
			//$app_off_detail_arr['housing_applicant_id'] = $housing_applicant_id;
			
			// echo "<pre>";print_r($app_off_detail_arr);
			db_update('housing_applicant_official_detail')
			->fields($app_off_detail_arr)
			->condition('applicant_official_detail_id', $hidden_official_detail_id)
			->execute();

			
			/*update online application*/
			
			$online_app_arr = array();
			$online_app_arr['applicant_official_detail_id']  =  $hidden_official_detail_id;
			$online_app_arr['status']  =  'housingapprover_approved_1';  // housing_sup_1 to change this by debaleena 04-09-2024
			$online_app_arr['date_of_verified']  =  date('Y-m-d');
			$online_app_arr['date_of_application']  =  trim( implode('-', array_reverse(explode('/', $form_state['values']['doa']))));
			$online_app_arr['is_backlog_applicant']  =  1;
			$online_app_arr['computer_serial_no']  =  $form_state['values']['computer_serial_no'];
			$online_app_arr['physical_application_no']  =  $form_state['values']['physical_application_no'];

			if($form_state['values']['remarks'] != '')
				$online_app_arr['remarks'] = $form_state['values']['remarks'];
			// echo "<pre>";print_r($online_app_arr);		
					
			db_update('housing_online_application')
			->fields($online_app_arr)
			->condition('online_application_id', $hidden_online_application_id)
			->execute();
			
			
			$new_allotment_app_arr['flat_type_id'] = fetch_rhe_flat_id(trim($form_state['values']['rhe_flat_type']));
			$new_allotment_app_arr['allotment_category']  =  trim($form_state['values']['reason']);
			// echo "<pre>";print_r($new_allotment_app_arr);die;

			db_update('housing_new_allotment_application')
			->fields($new_allotment_app_arr)
			->condition('online_application_id', $hidden_online_application_id)
			->execute();
			
			drupal_set_message('Data Updated successfully.');
		}else{
			drupal_set_message('The Computer Serial No. and Confirm Computer Serial No. do not match.Please enter same.','error');
		}
			
	}

	function date_format_change($input_date = ''){
		$input_date = $input_date;
		$date = DateTime::createFromFormat('Y-m-d', $input_date);
		$formatted_date = $date->format('d/m/Y');
		return $formatted_date;
	}


