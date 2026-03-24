<?php

	function existing_applicant_form($form, &$form_state) {
		
		$district = isset($form_state['values']['district']) && !empty($form_state['values']['district']) ? $form_state['values']['district'] : '';
		
		$designation = isset($form_state['values']['designation']) && !empty($form_state['values']['designation']) ? $form_state['values']['designation'] : '';
		$dob = isset($form_state['values']['dob']) && !empty($form_state['values']['dob']) ? $form_state['values']['dob'] : '';
		
		$pay_band_id = isset($form_state['values']['pay_band']) && !empty($form_state['values']['pay_band']) ? $form_state['values']['pay_band'] : 0;
	    $rhe_flat_type = rhe_flat_type_rst_pay_band_id($pay_band_id);
	    $reason = isset($form_state['values']['reason']) && !empty($form_state['values']['reason']) ? $form_state['values']['reason'] : '';
		
		$chk_permanent_address = isset($form_state['values']['chk_permanent_address']) ? $form_state['values']['chk_permanent_address'] : '';
		
		$form['form_begin'] = array(
			'#type' => 'markup',
			'#markup' => '<div class="online_application">',
			'#weight' => -2
		);
		
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
			'#prefix' => '<div class="row"><div class="col-md-6"><div class="form-floating">',
			'#suffix' => '</div></div>',
			'#element_validate' => array('element_validate_alphabatic_fullstop'), 
			'#required' => TRUE,
			'#attributes' => array('oninput'=>"this.value=this.value.toUpperCase()", 'class'=>array('form-control')),
		);
		
		$form['prrsonal_info']['applicant_father_name'] = array(
			'#title' => t('Father / Husband Name'),
			'#type' => 'textfield',
			'#default_value' => '',
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
			//'#required' => TRUE,
			'#prefix' => '<div class="row"><div class="col-md-6"><div class="form-floating">',
			'#suffix' => '</div></div>',
			'#element_validate' => array('element_validate_streetaddress'), 
			'#attributes' => array('oninput'=>"this.value=this.value.toUpperCase()", 'class'=>array('form-control')),
		);
		$form['prrsonal_info']['permanent_address']['permanent_city_town_village'] = array(
			'#title' => t('City / Town / Village'),
			'#type' => 'textfield',
			//'#required' => TRUE,
			'#prefix' => '<div class="col-md-6"><div class="form-floating">',
			'#suffix' => '</div></div></div>',
			'#element_validate' => array('element_validate_alphabatic_text'), 
			'#attributes' => array('oninput'=>"this.value=this.value.toUpperCase()", 'class'=>array('form-control')),
		);
		$form['prrsonal_info']['permanent_address']['permanent_post_office'] = array(
			'#title' => t('Post Office'),
			'#type' => 'textfield',
			//'#required' => TRUE,
			'#prefix' => '<div class="row"><div class="col-md-4"><div class="form-floating">',
			'#suffix' => '</div></div>',
			'#element_validate' => array('element_validate_alphabatic_text'), 
			'#attributes' => array('oninput'=>"this.value=this.value.toUpperCase()", 'class'=>array('form-control')),
		);
		$form['prrsonal_info']['permanent_address']['permanent_district'] = array(
			'#title' => t('District'),
			'#type' => 'select',
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
				//'#required' => TRUE,
				'#prefix' => '<div class="row"><div class="col-md-6" id="reload_present_street"><div class="form-floating">',
				'#suffix' => '</div></div>',
				'#element_validate' => array('element_validate_streetaddress'), 
				'#attributes' => array('oninput'=>"this.value=this.value.toUpperCase()", 'class'=>array('form-control')), //'id' => 'present_street', 
			);
			$form['prrsonal_info']['present_address']['present_city_town_village'] = array(
				'#title' => t('City / Town / Village'),
				'#type' => 'textfield',
				//'#required' => TRUE,
				'#prefix' => '<div class="col-md-6" id="reload_present_city_town_village"><div class="form-floating">',
				'#suffix' => '</div></div></div>',
				'#element_validate' => array('element_validate_alphabatic_text'), 
				'#attributes' => array('oninput'=>"this.value=this.value.toUpperCase()", 'class'=>array('form-control')), //'id' => 'present_city_town_village', 
			);
			$form['prrsonal_info']['present_address']['present_post_office'] = array(
				'#title' => t('Post Office'),
				'#type' => 'textfield',
				//'#required' => TRUE,
				'#prefix' => '<div class="row"><div class="col-md-4" id="reload_present_post_office"><div class="form-floating">',
				'#suffix' => '</div></div>',
				'#element_validate' => array('element_validate_alphabatic_text'), 
				'#attributes' => array('oninput'=>"this.value=this.value.toUpperCase()", 'class'=>array('form-control')), //'id' => 'present_post_office', 
			);
			$form['prrsonal_info']['present_address']['present_district'] = array(
				'#title' => t('District'),
				'#type' => 'select',
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
			'#default_value' => '',
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
			'#prefix' => '<div class="col-md-4"><div class="form-floating">',
			'#suffix' => '</div></div>',
			'#element_validate' => array('element_validate_email'),
			'#attributes' => array('oninput'=>"this.value=this.value.toLowerCase()", 'class'=>array('form-control')),

		);
		
		$form['prrsonal_info']['dob'] = array(
			'#title' => t('Date of Birth(According to Service Book)'),
			'#type' => 'textfield',
			'#attributes' => array('autocomplete' => 'off', 'id' => 'edit-dob', 'class'=>array('form-control'), 'placeholder' => 'DD/MM/YYYY'),
			'#required' => TRUE,
			//'#validated' => TRUE,
			'#maxlength' => 10,
			'#prefix' => '<div class="col-md-4"><div class="form-floating">',
			'#suffix' => '</div></div></div>',
			'#description' => '<span style="color:red;">Please input in DD/MM/YYYY format. Do not use any other format.</span>',
			'#element_validate' => array('element_validate_date', 'element_validate_age'),
		);
		
		$form['prrsonal_info']['gender'] = array(
			'#title' => t('Gender'),
			'#type' => 'radios',
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
			'#default_value' => '',
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
			'#required' => TRUE,
			'#prefix' => '<div class="col-md-4"><div class="form-floating">',
			'#suffix' => '</div></div>',
			// '#element_validate' => array('element_validate_textarea'), 
			'#attributes' => array('oninput'=>"this.value=this.value.toUpperCase()", 'class'=>array('form-control')),
			//'#attributes' => array('id' => 'edit-app-designation', 'oncopy' => "return false", 'onpaste' => "return false"),
		);

		/* by dg 28-07-2025 */
		$form['official_info']['app_posting_place'] = array(
			'#title' => t('Place of Posting'),
			'#type' => 'textfield',
			'#required' => TRUE,
			'#prefix' => '<div class="col-md-4"><div class="form-floating">',
			'#suffix' => '</div></div></div>',
			'#element_validate' => array('element_validate_alphabatic_text'), 
			'#attributes' => array('oninput'=>"this.value=this.value.toUpperCase()", 'class' => array(' form-control')),
		); 
			
		$form['official_info']['row_open'] = array(
		'#type' => 'markup',
		'#markup' => '<div class="row">',
		);

		$form['official_info']['pay_band_type'] = array(
		'#title' => t('Pay Band Type'),
		'#type' => 'radios',
		'#required' => TRUE,
		'#options' => array('old' => 'Old', 'new' => 'New'),
		'#default_value' => isset($form_state['values']['pay_band_type']) ? $form_state['values']['pay_band_type'] : 'new',
		'#prefix' => '<div class="col-md-4"><div class="form-floating">',  
		'#suffix' => '</div></div>', 
		'#ajax' => array(
			'callback' => 'update_pay_band_callback_pa',
			'wrapper' => 'pay-band-wrapper',
			'event' => 'change',
		),
		);

		$form['official_info']['pay_band_wrapper'] = array(
		'#type' => 'markup',
		'#markup' => '<div id="pay-band-wrapper" class="col-md-4"><div class="form-floating">', 
		);

		
		$selected_type = isset($form_state['values']['pay_band_type']) ? $form_state['values']['pay_band_type'] : 'new';

		
		$form['official_info']['pay_band'] = array(
		'#title' => t('Basic Pay Range'),
		'#type' => 'select',
		'#required' => TRUE,
		'#options' => pay_band_list_eo_pa($selected_type),
		'#attributes' => array('class' => array('form-control', 'form-select')),
		'#validated' => TRUE,
		);

		
		$form['official_info']['pay_band_wrapper_close'] = array(
		'#type' => 'markup',
		'#markup' => '</div></div>', 
		);

		
		$form['official_info']['pay_in'] = array(
		'#title' => t('Basic Pay'),
		'#type' => 'textfield',
		'#required' => TRUE,
		'#prefix' => '<div class="col-md-4"><div class="form-floating">',
		'#suffix' => '</div></div>',
		'#attributes' => array(
			'class' => array('form-control', 'form-text', 'required', 'numeric_positive'),
		),
		);

		
		$form['official_info']['row_close'] = array(
		'#type' => 'markup',
		'#markup' => '</div>', 
		);
			/* by dg end 28-07-2025 */

		// $form['official_info']['pay_band_type'] = array(
		// 	'#title' => t('Pay Band Type'),
		// 	'#type' => 'radios',
		// 	'#options' => array('M' => 'Old', 'F' => 'New'),
		// 	'#attributes' => array('class' => array('gender')),
		// 	'#default_value' => 'M',
		// 	'#prefix' => '<div class="col-md-4"><div class="">',
		// 	'#suffix' => '</div></div>',
		// 	'#required' => TRUE,
		// );
		/* turned off by dg 28-07-2025 
		$form['official_info']['pay_band'] = array(
			'#title' => t('Basic Pay Range'),
			'#type' => 'select',
			'#options' => pay_band_list(),
			'#required' => TRUE,
			'#validated' => TRUE,
			'#prefix' => '<div class="col-md-4"><div class="form-floating">',
			'#suffix' => '</div></div></div>',
		);
		$form['official_info']['pay_in'] = array(
			'#title' => t('Basic Pay'),
			'#type' => 'textfield',
			'#required' => TRUE,
			'#element_validate' => array('element_validate_numeric_positive'), 
			'#prefix' => '<div class="row"><div class="col-md-4"><div class="form-floating">',
		 	'#suffix' => '</div></div>',
			'#attributes' => array('class' => array('numeric_positive form-control')),
		);
		 end  */
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
		);
		
		$form['official_info']['app_posting_place'] = array(
			'#title' => t('Place of Posting'),
			'#type' => 'textfield',
			'#required' => TRUE,
			'#prefix' => '<div class="col-md-4"><div class="form-floating">',
			'#suffix' => '</div></div>',
			'#element_validate' => array('element_validate_alphabatic_text'), 
			'#attributes' => array('oninput'=>"this.value=this.value.toUpperCase()", 'class' => array(' form-control')),
		);  */
		$form['official_info']['app_headquarter'] = array(
			'#title' => t('Headquarter'),
			'#type' => 'textfield',
			//'#required' => TRUE,
			'#prefix' => '<div class="col-md-4"><div class="form-floating">',
			'#suffix' => '</div></div></div>',
			'#element_validate' => array('element_validate_alphabatic_text'), 
			'#attributes' => array('oninput'=>"this.value=this.value.toUpperCase()", 'class' => array('form-control')),
		);
		$form['official_info']['doj'] = array(
			'#title' => t('Date of Joining'),
			'#type' => 'textfield',
			'#attributes' => array('autocomplete' => 'off', 'id' => 'edit-doj', 'class' => array('form-control'), 'placeholder' => 'DD/MM/YYYY'),
			'#default_value' => '',
			//'#required' => TRUE,
			'#maxlength' => 10,
			'#prefix' => '<div class="row"><div class="col-md-6"><div class="form-floating">',
			'#suffix' => '</div></div>',
			'#description' => '<span style="color:red;">Please input in DD/MM/YYYY format. Do not use any other format.</span>',
			'#element_validate' => array('element_validate_date', 'element_validate_joining_age'),
		);
		
		$form['official_info']['dor'] = array(
			'#title' => t('Date of Retirement(According to Service Book)'),
			'#type' => 'textfield',
			//'#attributes' => array('readonly' => 'readonly'),
			'#attributes' => array('autocomplete' => 'off', 'id' => 'edit-dor', 'class' => array('form-control'), 'placeholder' => 'DD/MM/YYYY'),
			'#default_value' => '',
			'#required' => TRUE,
			'#maxlength' => 10,
			'#prefix' => '<div class="col-md-6"><div class="form-floating">',
			'#suffix' => '</div></div></div>',
			'#description' => '<span style="color:red;">Please input in DD/MM/YYYY format. Do not use any other format.</span>',
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
			'#required' => TRUE,
			'#prefix' => '<div class="row"><div class="col-md-4"><div class="form-floating">',
			'#suffix' => '</div></div>',
			// '#element_validate' => array('element_validate_textarea'), 
			'#attributes' => array('oninput'=>"this.value=this.value.toUpperCase()", 'class' => array('form-control')),
		);
		
		$form['office_add']['office_street'] = array(
			'#title' => t('Address'),
			'#type' => 'textfield',
			'#required' => TRUE,
			'#prefix' => '<div class="col-md-4"><div class="form-floating">',
			'#suffix' => '</div></div>',
			// '#element_validate' => array('element_validate_streetaddress'),
			'#attributes' => array('oninput'=>"this.value=this.value.toUpperCase()", 'class' => array('form-control')), 
		);
		$form['office_add']['office_city'] = array(
			'#title' => t('City / Town / Village'),
			'#type' => 'textfield',
			'#required' => TRUE,
			'#prefix' => '<div class="col-md-4"><div class="form-floating">',
			'#suffix' => '</div></div></div>',
			'#element_validate' => array('element_validate_alphabatic_text'), 
			'#attributes' => array('oninput'=>"this.value=this.value.toUpperCase()", 'class' => array('form-control')),
		);
		$form['office_add']['office_post_office'] = array(
			'#title' => t('Post Office'),
			'#type' => 'textfield',
			//'#required' => TRUE,
			'#prefix' => '<div class="row"><div class="col-md-3"><div class="form-floating">',
			'#suffix' => '</div></div>',
			'#element_validate' => array('element_validate_alphabatic_text'), 
			'#attributes' => array('oninput'=>"this.value=this.value.toUpperCase()", 'class' => array('form-control')),
		);
		$form['office_add']['office_district'] = array(
			'#title' => t('District'),
			'#type' => 'select',
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
			'#default_value' => $district,
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
			//'#required' => TRUE,
			//'#validated' => TRUE,
			'#prefix' => '<div class="row"><div class="col-md-6"><div class="form-floating">',
			'#suffix' => '</div></div>',
		);
		
		
		$form['ddo']['designation'] = array(
			'#title' => t('DDO Designation'),
			'#type' => 'select',
			'#default_value' => $designation,
			//'#options' => array('- Select -'),
			//'#options' => ddo_desig_list($district),
			'#options' => ddo_desig_list($district),
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
			//'#required' => TRUE,
			//'#validated' => TRUE,
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
			'#title' => t('Allotment Category'),
			'#collapsible' => FALSE, // Added
			'#collapsed' => FALSE,  // Added
		);
		
		$form['allotment_reason']['rhe_flat_type'] = array(
			'#title' => t('Flat TYPE'),
			'#type' => 'textfield',
			'#attributes' => array('readonly' => 'readonly', 'class' => array('form-control')),
			'#default_value' => '',
			'#required' => TRUE,
			'#prefix' => '<div class="row"><div class="col-md-6" id="replace_rhe_flat_type" ><div class="form-floating">',
			'#suffix' => '</div></div>',
			'#element_validate' => array('element_validate_flat_type'),
		 );
	     // echo $rhe_flat_type;
		 
		 $form['allotment_reason']['reason'] = array(
			'#title' => t('Allotment Category'),
			'#type' => 'select',
			//'#options' => array(''=>'- Select -'),
			'#options' => allotment_category_list(trim($rhe_flat_type)),
			'#default_value' => $reason,
			'#required' => TRUE,
			'#validated' => TRUE,
			'#prefix' => '<div id="replace_allotment_category" class="col-md-6"><div class="form-floating">',
			'#suffix' => '</div></div></div>',
			'#attributes' => array('class' => array('form-control check_allotment_reason_cls'),'id' => 'check_allotment_reason_add'),
		 );
		 
		 $form['allotment_reason']['doa'] = array(
			'#title' => t('Date of Application'),
			'#type' => 'textfield',
			'#attributes' => array('autocomplete' => 'off', 'id' => 'edit-doa','class' => array('form-control')),
			'#required' => TRUE,
			//'#validated' => TRUE,
			'#prefix' => '<div class="row"><div class="col-md-4"><div class="form-floating">',
			'#suffix' => '</div></div>',
			'#element_validate' => array('element_validate_date'),
		);
		
		$form['allotment_reason']['computer_serial_no'] = array(
			'#title' => t('Computer Serial No.'),
			'#type' => 'textfield',
			'#required' => TRUE,
			//'#element_validate' => array('element_validate_numeric_positive'),
			'#prefix' => '<div id="computr_serial_no" class="col-md-4"><div class="form-floating">',
			'#suffix' => '</div></div>',
			'#attributes' => array('class' => array('form-control')),
		);
		/* added by debaleena 01-08-2024 */   
		$form['allotment_reason']['confirm_computer_serial_no'] = array(
			'#title' => t('Confirm Computer Serial No.'),
			'#type' => 'password',
			'#required' => TRUE,
			//'#element_validate' => array('element_validate_numeric_positive'),
			'#prefix' => '<div id="confirm_computr_serial_no" class="col-md-4"><div class="form-floating">',
			'#suffix' => '</div></div></div>',
			'#attributes' => array('class' => array('form-control')),
		);
		// added on 19-06-2025 by moumita
		$form['allotment_reason']['physical_application_no'] = array(
			'#title' => t('Physical Application Number'),
			'#type' => 'textfield',
			'#required' => TRUE,
			// '#element_validate' => array('element_validate_numeric_positive'),
			'#prefix' => '<div class="row"><div id="" class="col-md-4"><div class="form-floating">',
			'#suffix' => '</div></div></div>',
			'#attributes' => array('class' => array('form-control')),
		);


		 // Message area for validation feedback
		 $form['allotment_reason']['comp_ser_no_message'] = array(
			'#type' => 'markup',
			'#markup' => '<div id="comp_ser_no_message" class="form-floating"></div>', // Use form-text for styling
			'#prefix' => '<div class="row"><div class="col-md-12">', // Ensure it aligns with the rest of the form
			'#suffix' => '</div></div>', // Close the div properly
		  );
		
		/* end */

		$form['document_upload']['file_markup'] = array(
            '#type' => 'markup',
            '#markup' => '<div class="row">'

        );
		
        
        $form['document_upload']['extra_doc_new'] = array(
            '#title' => t('Upload Allotment Reason Supporting Document') ,
            '#type' => 'managed_file',
            '#description' => t('<b>Allowed Extension: pdf<br>Maximum File Size: 1 MB</b>') ,
            '#progress_message' => t('Please wait...') ,
            //'#progress_indicator' => 'bar',
            //'#default_value' => variable_get('Upload File', '') ,
            //'#theme' => 'common_application_thumb_upload',
            '#upload_validators' => array(
                'file_validate_extensions' => array(
                    "pdf"
                ) ,
                //'file_validated_size' => array(MAX_FILE_SIZE*1024*1024),
            ) ,
            '#upload_location' => 'public://doc/extra_doc/',
            //'#required' => TRUE,
            //'#validated' => TRUE,
            '#prefix' => '<div id="extra_doc">',
            '#suffix' => '</div>',
            '#process' => array(
                'new_application_element_process'
            ) ,

        );

        $form['document_upload']['file_markup_end'] = array(
            '#type' => 'markup',
            '#markup' => '</div>'

        );

		
		$form['allotment_remarks']['remarks'] = array(
			'#title' => t('Remarks'),
			'#type' => 'textarea',
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
		// if($form_state['values']['district'] == '') {   turned off by dg 22-08-2025
  		// 	form_set_error('district', t('Please select district.'));  
		// }
		// if($form_state['values']['designation'] == '') {           turned off by dg 22-08-2025
  		// 	form_set_error('designation', t('Please select ddo designation.'));
		// }
		if($form_state['values']['reason'] == '') {
  			form_set_error('reason', t('Please select allotment reason.'));
		}
		
		// if(!empty($form_state['values']['mobile']) && is_numeric($form_state['values']['mobile'])) {
		   
		// 	 $mobile = trim($form_state['values']['mobile']);
			 
		// 	 $query = db_select('housing_applicant','applicant');
		// 	 $query->addExpression('mobile_no','mobile');
		// 	 $query->condition('mobile_no', $mobile);
		// 	 $result =$query->execute();
			 
			 
		// 	 if($result->rowCount() > 0)
		// 	 	form_set_error('mobile', 'This mobile no. already exist.');
		
		// }
		
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
	
	
	function existing_applicant_form_submit($form, &$form_state) {
		global $user;
		//$name = strtolower(trim($form_state['values']['applicant_name']));
		//$name1 = str_replace(" ","",$name);
		//$username = str_replace(".","",$name1).rand();
		// echo "<pre>";print_r($form_state);die;

		$physical_application_no = $form_state['values']['physical_application_no']; // by dg 01-07-2025

		$computer_serial_no = trim($form_state['values']['computer_serial_no']);
		$confirm_computer_serial_no = trim($form_state['values']['confirm_computer_serial_no']);

		// Sanitize integer-like input fields by removing any non-digit characters.
		//added by jyoti on 11-12-2025 start
		$raw = isset($form_state['values']) ? $form_state['values'] : array();
		$mobile_clean = isset($raw['mobile']) ? preg_replace('/\D+/', '', $raw['mobile']) : '';
		$hrms_id_clean = isset($raw['hrms_id']) ? preg_replace('/\D+/', '', $raw['hrms_id']) : '';
		$pay_band_clean = isset($raw['pay_band']) ? preg_replace('/\D+/', '', $raw['pay_band']) : '';
		$pay_in_clean = isset($raw['pay_in']) ? preg_replace('/[^0-9\-]/', '', $raw['pay_in']) : ''; // allow minus only if needed
		$present_pincode_clean = isset($raw['present_pincode']) ? preg_replace('/\D+/', '', $raw['present_pincode']) : '';
		$permanent_pincode_clean = isset($raw['permanent_pincode']) ? preg_replace('/\D+/', '', $raw['permanent_pincode']) : '';
		$office_pincode_clean = isset($raw['office_pincode']) ? preg_replace('/\D+/', '', $raw['office_pincode']) : '';
		$office_phone_no_clean = isset($raw['office_phone_no']) ? trim($raw['office_phone_no']) : '';
			//added by jyoti on 11-12-2025 end


		/* by dg 01-07-2025 */ 
		$query_4 = db_select('housing_online_application', 'hoa');
		$query_4->fields('hoa',array('computer_serial_no'));
		$query_4->condition('hoa.computer_serial_no', $computer_serial_no, '=');   
			
		$result_4 = $query_4->execute();
		$data_4 = $result_4->fetchObject();
		// echo $physical_application_no;die;
		/* end */
		
		if(empty($data_4->computer_serial_no) || $data_4->computer_serial_no =='')  // by dg 01-07-2025 modified on 02-02-2026
		{    

			if($computer_serial_no == $confirm_computer_serial_no)
			{

				/* by dg 01-07-2025 */ 
				$query_5 = db_select('housing_online_application', 'hoa');
				$query_5->fields('hoa',array('physical_application_no'));
				$query_5->condition('hoa.physical_application_no', $physical_application_no, '=');   
					
				$result_5 = $query_5->execute();
				$data_5 = $result_5->fetchObject();
				/* end */
				if(empty($data_5->physical_application_no) || $data_5->physical_application_no ==''){  // by dg 01-07-2025 and 15-10-2025 by Moumita
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
					$physical_application_no = preg_replace('/[^a-zA-Z0-9_]/', '_', $physical_application_no);  // by dg 01-07-2025
					$username = str_replace(".","",$name).'_'.$physical_application_no.'_'.$form_state['values']['computer_serial_no'];

					//debaleena start 11.06.24///

					if($form_state['values']['hrms_id'] != '')
					{
						// echo "hrms id";die;
						$newUser	   	   = array();
						$newUser['name']   = trim($form_state['values']['hrms_id']);
						$newUser['pass']   = trim($form_state['values']['hrms_id']);
						/*$str[0].$str[1].trim($form_state['values']['username']).'@'.$dob_t.strrev($str1[0].$str1[1]);*/
						if($form_state['values']['email'] !=''){										
							$newUser['mail']   = trim($form_state['values']['email']);
						}	
						$newUser['status'] = 1;
						$username = trim($form_state['values']['hrms_id']); // added by Moumita on 10-12-2025
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
						// echo "<pre>";print_r($newUser);die;
						
					}

					//end//
					$querycheck = db_query("select uid from users where name ilike '%".$username."%' "); // changed by Moumita on 09-12-2025
					$resultchk = $querycheck->fetchObject();
					if(empty($resultchk)){
						// echo "ll";die;
						user_save(null, $newUser);
						/*get last inserted uid by dg 12-12-2025 start*/
						$query = db_select('users','u');
						$query->addExpression('MAX(uid)','max_uid');
						$result = $query->execute();
						$data = $result->fetchObject();
						$user_uid = $data->max_uid;
					}else{
						// echo "pp";die;
						$query = db_select('users', 'u');
						$query->innerJoin('housing_applicant', 'ha', 'u.uid = ha.uid');
						$query->innerJoin('users_roles', 'ur', 'ur.uid = u.uid');

						// LEFT JOIN to find missing official detail
						$query->leftJoin('housing_applicant_official_detail', 'haod', 'haod.housing_applicant_id = ha.housing_applicant_id');
						$query->fields('u', array('uid', 'name'));
						$query->fields('ha', array());
						$query->fields('ur', array());

						// Only fetch where official detail does NOT exist
						$query->isNull('haod.housing_applicant_id');
						$query->condition('u.uid', $resultchk->uid, '=');
						$result = $query->execute()->fetchObject();

						if (!empty($result)) {
							// echo "<pre>";print_r($result);die;
							$user_applicant_det = array();
							$user_applicant_det['uid'] = !empty($result->uid) ? $result->uid : 0;
							$user_applicant_det['applicant_name'] = $result->applicant_name;
							$user_applicant_det['guardian_name'] = $result->guardian_name;
							$user_applicant_det['date_of_birth'] = $result->date_of_birth;
							$user_applicant_det['user_name'] = $result->name;
							$user_applicant_det['housing_applicant_id'] = $result->housing_applicant_id;
							$user_applicant_det['mobile_no'] = $result->mobile_no;
							$user_applicant_det['created_at'] = date('Y-m-d H:i:s');
							$user_applicant_det['updated_at'] = date('Y-m-d H:i:s');
							
							$checked_insert = db_insert('housing_users_applicant_role_deletion_log')
												->fields($user_applicant_det)
												->execute();
							
							if (!empty($checked_insert)) {
								$housing_appliant_deletion = db_delete('housing_Applicant')
																->condition('uid', $result->uid)
																->execute();
								if($housing_appliant_deletion > 0){
									
									$users_deletion = db_delete('users')
														->condition('uid', $result->uid)
														->execute();
									if($users_deletion > 0){
										$user_roles_deletion = db_delete('users_roles')
																->condition('uid', $result->uid)
																->execute();

										user_save(null, $newUser);
										
									    /*get last inserted uid*/
										$query = db_select('users','u');
										$query->addExpression('MAX(uid)','max_uid');
										$result = $query->execute();
										$data = $result->fetchObject();
										$user_uid = $data->max_uid;
									}else{
										drupal_set_message('Users deletion not Completed.');
									}
								}else{
									drupal_set_message('housing Applicant deletion not Completed.');
								}
							}else{
								drupal_set_message('Log insertion not Completed.');
							}

								
							
						} else {
							// echo "poipoipo";die;
							$query = db_select('users', 'u');
							$query->innerJoin('users_roles', 'ur', 'ur.uid = u.uid');

							// LEFT JOIN housing_applicant
							$query->leftJoin('housing_applicant', 'ha', 'ha.uid = u.uid');
							$query->fields('u', array('uid', 'name'));
							$query->fields('ur', array('rid'));

							// Check: user NOT present in housing_applicant
							$query->isNull('ha.housing_applicant_id');
							$query->condition('u.uid', $resultchk->uid, '=');
							$result = $query->execute()->fetchObject();
							$user_uid = $result->uid;
							
							// echo "<pre>";print_r($user_uid);die;
						}
						// drupal_set_message('This user is already exist','error');
						// drupal_goto('existing_applicant_entry');
					}
					/*by dg 12-12-2025 end*/
					/*insert into housing_applicant table*/
					
					$applicant_personal_detail_arr = array();
					$applicant_personal_detail_arr['uid'] 				=  $user_uid; //$data->max_uid replaced with $user_uid in all place by dg 12-12-2025
					$applicant_personal_detail_arr['applicant_name'] 	=  trim($form_state['values']['applicant_name']);
					$applicant_personal_detail_arr['guardian_name'] 		=  trim($form_state['values']['applicant_father_name']);
					$applicant_personal_detail_arr['date_of_birth'] 		=  trim(implode('-', array_reverse(explode('/', $form_state['values']['dob']))));
					$applicant_personal_detail_arr['gender'] 			=  trim($form_state['values']['gender']);
					
					// if($form_state['values']['mobile'] != '')
					// 	$applicant_personal_detail_arr['mobile_no'] 			=  trim($form_state['values']['mobile']);
					
					if($form_state['values']['permanent_street'] != '')	
						$applicant_personal_detail_arr['permanent_street']  =  strtoupper(trim($form_state['values']['permanent_street']));
					if($form_state['values']['permanent_city_town_village'] != '')
						$applicant_personal_detail_arr['permanent_city_town_village']  =  strtoupper(trim($form_state['values']['permanent_city_town_village']));
					if($form_state['values']['permanent_post_office'] != '')
						$applicant_personal_detail_arr['permanent_post_office']  =  strtoupper(trim($form_state['values']['permanent_post_office']));
					if($form_state['values']['permanent_district'] != '')
						$applicant_personal_detail_arr['permanent_district']  =  trim($form_state['values']['permanent_district']);

					// if($form_state['values']['permanent_pincode'] != '')
					// 	$applicant_personal_detail_arr['permanent_pincode']  =  trim($form_state['values']['permanent_pincode']);
					// added by jyoti on 11-12-2025
					if($mobile_clean != '')
						$applicant_personal_detail_arr['mobile_no'] 			=  $mobile_clean;
					if($permanent_pincode_clean != '')
						$applicant_personal_detail_arr['permanent_pincode']  =  $permanent_pincode_clean;
					
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
					// if($form_state['values']['present_pincode'] != '')
					// 	$applicant_personal_detail_arr['present_pincode']  =  trim($form_state['values']['present_pincode']);

					// added by jyoti on 11-12-2025
					if($present_pincode_clean != '')
						$applicant_personal_detail_arr['present_pincode']  =  $present_pincode_clean;
					// } else {
					// 	$applicant_personal_detail_arr['present_street']  =  NULL;
					// 	$applicant_personal_detail_arr['present_city_town_village']  =  NULL;
					// 	$applicant_personal_detail_arr['present_post_office']  =  NULL;
					// 	$applicant_personal_detail_arr['present_district']  =  NULL;
					// 	$applicant_personal_detail_arr['present_pincode']  =  NULL;	
					// }
					
					// echo "<pre>";print_r($applicant_personal_detail_arr);die;
					$last_id = db_insert('housing_applicant')
					->fields($applicant_personal_detail_arr)
					->execute();

					// get housing_applicant_id by uid
					
					$haid_query = db_select('housing_applicant','ha')->fields('ha',array('housing_applicant_id'))->condition('uid',$data->max_uid)->execute();
					$haid_result = $haid_query->fetchObject();
					// print_r($haid_result);
					$housing_applicant_id = $haid_result->housing_applicant_id; 
					/*assign user roles*/
					
					if(!empty($user_uid)){  //condition added by dg 12-12-2025
						$arr_user_roles = array();
						$arr_user_roles['uid'] = $applicant_personal_detail_arr['uid'];
						$arr_user_roles['rid'] = 4;
						
						db_insert('users_roles')
						->fields($arr_user_roles)
						->execute();
					}
					
					/*add applicant_official_detail  data*/
					$app_off_detail_arr = array();
					$app_off_detail_arr['uid']  =  $user_uid;

					// if($form_state['values']['hrms_id'] != '')
					// 	$app_off_detail_arr['hrms_id']  =  trim($form_state['values']['hrms_id']);

					// added by jyoti on 11-12-2025
					if($hrms_id_clean != '')
						$app_off_detail_arr['hrms_id']  =  $hrms_id_clean;
					
					$app_off_detail_arr['applicant_designation']  =  strtoupper(trim($form_state['values']['app_designation']));

					// $app_off_detail_arr['pay_band_id']  =  trim($form_state['values']['pay_band']);
					// $app_off_detail_arr['pay_in_the_pay_band']  =  trim($form_state['values']['pay_in']);

					// Ensure pay band and pay_in are integers (or numeric strings) before storing.
					$app_off_detail_arr['pay_band_id']  =  ($pay_band_clean !== '') ? (int) $pay_band_clean : NULL; // added by jyoti on 11-12-2025
					$app_off_detail_arr['pay_in_the_pay_band']  =  ($pay_in_clean !== '') ? $pay_in_clean : NULL; // added by jyoti on 11-12-2025

					// if($form_state['values']['grade_pay'] != '')
					// 	$app_off_detail_arr['grade_pay']  =  trim($form_state['values']['grade_pay']);
					
					/*if($form_state['values']['gpf_no'] != '')
						$app_off_detail_arr['gpf_no']  =  trim($form_state['values']['gpf_no']);*/
					
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
					// $app_off_detail_arr['office_pin_code']  =  trim($form_state['values']['office_pincode']);
					$app_off_detail_arr['office_pin_code']  =  ($office_pincode_clean !== '') ? $office_pincode_clean : NULL; // added by jyoti on 11-12-2025
					
					if($form_state['values']['office_phone_no'] != '')
						$app_off_detail_arr['office_phone_no']  =  trim($form_state['values']['office_phone_no']);
					
					//$app_off_detail_arr['ddo_id']  =   !empty($form_state['values']['designation']) ? trim($form_state['values']['designation']) : NULL;    //by dg 22-08-2025
					$designation = trim($form_state['values']['designation']); //by dg 27-08-2025
					if ($designation !== '') {
					    $app_off_detail_arr['ddo_id'] = (int) $designation;
					}
					$app_off_detail_arr['housing_applicant_id'] = $housing_applicant_id;
					
					
					db_insert('housing_applicant_official_detail')
					->fields($app_off_detail_arr)
					->execute();
					
					/*fetching applicant_official_detail_id*/
					$query1 = db_select('housing_applicant_official_detail', 'haod');
					$query1->addExpression('MAX(applicant_official_detail_id)','id');
					$query1->condition('haod.uid', $user_uid, '=');
					$result1 =$query1->execute();
					$data1 = $result1->fetchObject();
					
					/*add online application*/
					
					$online_app_arr = array();
					$online_app_arr['applicant_official_detail_id']  =  $data1->id;
					$online_app_arr['status']  =  'housingapprover_approved_1';  // housing_sup_1 to change this by debaleena 04-09-2024
					$online_app_arr['date_of_verified']  =  date('Y-m-d');
					$online_app_arr['date_of_application']  =  trim( implode('-', array_reverse(explode('/', $form_state['values']['doa']))));
					$online_app_arr['is_backlog_applicant']  =  1;
					$online_app_arr['computer_serial_no']  =  $form_state['values']['computer_serial_no'];
					$online_app_arr['physical_application_no']  =  $form_state['values']['physical_application_no'];

					if($form_state['values']['remarks'] != '')
						$online_app_arr['remarks'] = $form_state['values']['remarks'];
							
							
					db_insert('housing_online_application')
					->fields($online_app_arr)
					->execute();
					
					/*fetching online_application_id*/
					$query2 = db_select('housing_online_application', 'hoa');
					$query2->addExpression('MAX(online_application_id)','oid');
					$query2->condition('hoa.applicant_official_detail_id', $data1->id, '=');
					$result2 =$query2->execute();
					$data2 = $result2->fetchObject();
					
					$online_app_arr_upd = array();
					//$online_app_arr_upd['application_no']  = 'NA-'.trim( implode(explode('/', $form_state['values']['doa']))).'-'.$data2->oid;
					$online_app_arr_upd['application_no']  = 'PA-'.trim( implode(explode('/', $form_state['values']['doa']))).'-'.$data2->oid; // by debaleena 11.06.24
						
					db_update('housing_online_application')
					->fields($online_app_arr_upd)
					->condition('online_application_id', $data2->oid)
					->execute();
					
					/* add new alltment application*/  
					$new_allotment_app_arr = array();
					$new_allotment_app_arr['online_application_id']  =  $data2->oid;
					
					$new_allotment_app_arr['flat_type_id'] = fetch_rhe_flat_id(trim($form_state['values']['rhe_flat_type']));
					$new_allotment_app_arr['allotment_category']  =  trim($form_state['values']['reason']);


					/*  added on 14-10-2025 by dg & moumita*/

					if(trim($form['allotment_reason']['reason']['#value']) == 'Transfer' || trim($form['allotment_reason']['reason']['#value']) == 'Physically Handicaped Or Serious Illness' || trim($form['allotment_reason']['reason']['#value']) == 'Recommended' || trim($form['allotment_reason']['reason']['#value']) == 'Single Earning Lady' || trim($form['allotment_reason']['reason']['#value']) == 'Legal Heir') {
					
						if($form_state['values']['extra_doc_new'] != 0){
							// file exists
							$file_extra_doc = file_load($form_state['values']['extra_doc_new']);
							
							if($file_extra_doc->status == 0)
							{ 
								$file_extra_doc->status = FILE_STATUS_PERMANENT;
								file_save($file_extra_doc);
								file_usage_add($file_extra_doc, 'new_application', 'Allotment Reason Supporting Document', $user->uid);
								
								$new_allotment_app_arr['extra_doc']  =  $file_extra_doc->fid;
								$new_allotment_app_arr['extra_doc_path']  =  $file_extra_doc->path;
							}
						}
					} else {
						if($form_state['values']['extra_doc_new'] == 0) {
							$new_allotment_app_arr['extra_doc'] = NULL;
						}
					}
					/*END*/

					
					db_insert('housing_new_allotment_application')
					->fields($new_allotment_app_arr)
					->execute();
					
					drupal_set_message('Data inserted successfully.');
				}else{
					drupal_set_message('This Physical Application No. is already Exist.','error');    //added by dg 01-07-2025
				}	
				
			}else{
				drupal_set_message('The Computer Serial No. and Confirm Computer Serial No. do not match.Please enter same.','error');
			}
		}else{
			drupal_set_message('This Computer Serial No. is already Exist.','error');   //added by dg 01-07-2025
			
		}
	}

	function _reload_ddo_designation_new($form, &$form_state) {
		
		$district = isset($form_state['values']['ddo_district']) && !empty($form_state['values']['ddo_district']) ? $form_state['values']['ddo_district'] : '';
		
		$form['ddo']['address']['#value'] = '';
		if($district == '') {
			$form['ddo']['designation']['#options'] = array(''=>'- Select -');
		}
		else {
			$form['ddo']['designation']['#options'] = ddo_desig_list($district);
		}
		$commands = array();
		$commands[] = ajax_command_replace('#replace_ddo_address', drupal_render($form['ddo']['address']));
		$commands[] = ajax_command_replace('#replace_designation', drupal_render($form['ddo']['designation']));
		$form_state['rebuild'] = TRUE;
		return array('#type' => 'ajax', '#commands' => $commands);
	}


