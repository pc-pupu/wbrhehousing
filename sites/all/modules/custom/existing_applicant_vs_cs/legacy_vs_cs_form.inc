<?php
function legacy_vs_cs_form($form, &$form_state, $uid = '') {
	//$form_state['rebuild'] = TRUE;
    global $user;

	module_load_include('inc', 'common_application', 'common_application_fetch_data.form');
	module_load_include('inc', 'common_application', 'common_application_validate.form');


	$uid = decrypt_url($uid);
<<<<<<< HEAD
    
=======
    // echo $uid;die;
>>>>>>> 2502ab393f49b61f25516e6bb0502acbb6d447a0
    $district = isset($form_state['values']['district']) && !empty($form_state['values']['district']) ? $form_state['values']['district'] : '';
    
    $designation = isset($form_state['values']['designation']) && !empty($form_state['values']['designation']) ? $form_state['values']['designation'] : '';
    //$dob = isset($form_state['values']['dob']) && !empty($form_state['values']['dob']) ? $form_state['values']['dob'] : '';

    
	$form['form_begin'] = array(
		'#type' => 'markup',
		'#markup' => '<div class="online_application">',
		'#weight' => -2
	);

	$form['application_info'] = array(
		'#type' => 'fieldset',
		'#title' => t('Application Type Information'),
		'#collapsible' => FALSE, // Added
		'#collapsed' => FALSE,  // Added
	);

	$form['application_type'] = array(
		'#title' => t('Application Type'),
		'#type' => 'select',
		'#options' => array(
			'VS' => t('Floor Shifting'),
			'CS' => t('Category Shifting'),
		),
		'#required' => TRUE,
		'#prefix' => '<div class = "row"><div class="col-md-6"><div class="form-floating">',
		'#suffix' => '</div></div>',
		'#validated' => TRUE,
		'#attributes' => array('class' => array('form-select')),
	);
	$form['application_date'] = array(
		'#title' => t('Date of Application'),
		'#type' => 'textfield',
		'#attributes' => array('id' => 'edit-doa', 'class'=>array('form-control')),
		'#required' => TRUE,
		//'#disabled'=>TRUE,
		//'#default_value' => '01/01/1900',
		'#prefix' => '<div class="col-md-6"><div class="form-floating">',
		'#suffix' => '</div></div></div>',
		'#element_validate' => array('element_validate_date'),
	);
		
	// personal information
	$form['prrsonal_info'] = array(
		'#type' => 'fieldset',
		'#title' => t('Personal Information(According to Service Book)'),
		'#collapsible' => FALSE, // Added
		'#collapsed' => FALSE,  // Added
	);

	$form['prrsonal_info']['applicant_name'] = array(
		'#title' => t('Applicant\'s Name'),
		'#type' => 'textfield',
		// '#prefix' => '<div class="two">',
		
		'#prefix' => '<div class="row"><div class="col-md-6"><div class="form-floating">',
		'#suffix' => '</div></div>',

		//'#element_validate' => array('element_validate_alphabatic_fullstop'), off by dg for demo
		'#required' => TRUE,
		'#attributes' => array('oninput'=>"this.value=this.value.toUpperCase()", 'class'=>array('form-control')), 
	);
		
	$form['prrsonal_info']['applicant_father_name'] = array(
		'#title' => t('Father\'s / Husband\'s Name'),
		'#type' => 'textfield',
		'#default_value' => 'NA',
		'#prefix' => '<div class="col-md-6"><div class="form-floating">',
		'#suffix' => '</div></div></div>',

		'#element_validate' => array('element_validate_alphabatic_fullstop'), 
		//'#required' => TRUE,
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
		'#default_value' => 'NA',
		'#prefix' => '<div class="row"><div class="col-md-6"><div class="form-floating">',
		'#suffix' => '</div></div>',
		'#element_validate' => array('element_validate_streetaddress'), 
		'#attributes' => array('oninput'=>"this.value=this.value.toUpperCase()", 'class'=>array('form-control')),
		
	);
	$form['prrsonal_info']['permanent_address']['permanent_city_town_village'] = array(
		'#title' => t('City / Town / Village'),
		'#type' => 'textfield',
		//'#required' => TRUE,
		'#default_value' => 'NA',
		'#prefix' => '<div class="col-md-6"><div class="form-floating">',
		'#suffix' => '</div></div></div>',
		'#element_validate' => array('element_validate_alphabatic_text'), 
		'#attributes' => array('oninput'=>"this.value=this.value.toUpperCase()", 'class'=>array('form-control')),
		
	);
	$form['prrsonal_info']['permanent_address']['permanent_post_office'] = array(
		'#title' => t('Post Office'),
		'#type' => 'textfield',
		//'#required' => TRUE,
		'#default_value' => 'NA',
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
		'#default_value' => '17',
		'#prefix' => '<div class="col-md-4"><div class="form-floating">',
		'#suffix' => '</div></div>',
		'#attributes' => array('class'=>array('form-select'))
	);
	$form['prrsonal_info']['permanent_address']['permanent_pincode'] = array(
		'#title' => t('Pincode'),
		'#type' => 'textfield',
		'#maxlength' => 6,
		//'#required' => TRUE,
		'#default_value' => '700001',
		'#element_validate' => array('element_validate_numeric_positive'), 
		'#prefix' => '<div class="col-md-4"><div class="form-floating">',
		'#suffix' => '</div></div></div>',
		'#attributes' => array('class' => array('numeric_positive'), 'class'=>array('form-control')),
	);

	$form['prrsonal_info']['present_address'] = array(
		'#type' => 'fieldset',
		'#title' => t('Present Address'),
		'#collapsible' => FALSE, // Added
		'#collapsed' => FALSE,  // Added
		'#prefix' => '<div id="reload_present_address">',
		'#suffix' => '</div>',
		'#default_value' => 'NA',
		
	);
	$form['prrsonal_info']['present_address']['present_street'] = array(
		'#title' => t('Address'),
		'#type' => 'textfield',
		//'#required' => TRUE,
		'#default_value' => 'NA',
		'#prefix' => '<div class="row"><div class="col-md-6" id="reload_present_street"><div class="form-floating">',
		'#suffix' => '</div></div>',
		'#element_validate' => array('element_validate_streetaddress'), 
		'#attributes' => array('oninput'=>"this.value=this.value.toUpperCase()", 'class'=>array('form-control')), //'id' => 'present_street',
		
	);
	$form['prrsonal_info']['present_address']['present_city_town_village'] = array(
		'#title' => t('City / Town / Village'),
		'#type' => 'textfield',
		//'#required' => TRUE,
		'#default_value' => 'NA',
		'#prefix' => '<div class="col-md-6" id="reload_present_city_town_village"><div class="form-floating">',
		'#suffix' => '</div></div></div>',
		'#element_validate' => array('element_validate_alphabatic_text'), 
		'#attributes' => array('oninput'=>"this.value=this.value.toUpperCase()", 'class'=>array('form-control')), //'id' => 'present_city_town_village', 
		
	);
	$form['prrsonal_info']['present_address']['present_post_office'] = array(
		'#title' => t('Post Office'),
		'#type' => 'textfield',
		//'#required' => TRUE,
		'#default_value' => 'NA',
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
		'#default_value' => '17',
		'#prefix' => '<div class="col-md-4" id="reload_present_district"><div class="form-floating">',
		'#suffix' => '</div></div>',
		//'#attributes' => array('id' => 'present_district'),
		// '#select2' => array(
		// 	'width' => '500',
		// 	//'placeholder' => '- Select -',
		// 	'allowClear' => TRUE
		// ),
		'#attributes' => array('class'=>array('form-select'))
	);
	$form['prrsonal_info']['present_address']['present_pincode'] = array(
		'#title' => t('Pincode'),
		'#type' => 'textfield',
		'#maxlength' => 6,
		//'#required' => TRUE,
		'#default_value' => '700001',
		'#element_validate' => array('element_validate_numeric_positive'), 
		'#prefix' => '<div class="col-md-4" id="reload_present_pincode"><div class="form-floating">',
		'#suffix' => '</div></div></div>',
		'#attributes' => array('class' => array('numeric_positive'), 'class'=>array('form-control')), //'id' => 'present_pincode', 
		
	);

    $form['prrsonal_info']['mobile'] = array(
		'#title' => t('Mobile no'),
		'#type' => 'textfield',
		'#default_value' => '9999999999',
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
		//'#element_validate' => array('element_validate_email', 'email_id_validate'),
		//'#default_value' => '',
		//'#required' => TRUE,
		'#attributes' => array('class'=>array('form-control')),
		
	);
	
	if (empty($user->mail)) {
		$form['prrsonal_info']['email']['#attributes'] = array('oninput'=>"this.value=this.value.toLowerCase()",'class'=>array('form-control'));
	}
	
	$form['prrsonal_info']['dob'] = array(
		'#title' => t('Date of Birth(According to Service Book)'),
		'#type' => 'textfield',
		'#attributes' => array('id' => 'edit-dob', 'class'=>array('form-control')),
		//'#required' => TRUE,
		//'#disabled'=>TRUE,
		'#default_value' => '01/01/1900',
		'#prefix' => '<div class="col-md-4"><div class="form-floating">',
		'#suffix' => '</div></div></div>',
		'#element_validate' => array('element_validate_date'),
	);
	
	$form['prrsonal_info']['gender'] = array(
		'#title' => t('Gender'),
		'#type' => 'radios',
		'#options' => array('M' => 'Male', 'F' => 'Female'),
		// '#attributes' => array('class' => array('')),
		// '#default_value' => '',
		'#default_value' => 'M',
		'#prefix' => '<div class="row"><div class="col-md-4"><div class="">',
		'#suffix' => '</div></div></div>',
		'#required' => TRUE,
		
	);


	$query = db_select('housing_applicant_official_detail', 'haod');
	$query->innerJoin('housing_online_application', 'hoa',
	'hoa.applicant_official_detail_id = haod.applicant_official_detail_id');
	$query->innerJoin('housing_flat_occupant', 'hfo', 
	'hfo.online_application_id = hoa.online_application_id');
	
	$query->innerJoin('housing_flat', 'hf', 'hf.flat_id = hfo.flat_id');
	$query->innerJoin('housing_estate', 'he', 'hf.estate_id = he.estate_id');
	$query->innerJoin('housing_flat_type', 'hft', 'hf.flat_type_id = hft.flat_type_id');
	$query->innerJoin('housing_block', 'hb', 'hf.block_id = hb.block_id');
	$query->addField('hf', 'flat_id');
	$query->addField('hft', 'flat_type_id');
	$query->addField('he', 'estate_id');
	$query->addField('hb', 'block_id');
	$query->addField('hb', 'block_name');
	$query->addField('he', 'estate_name');
	$query->addField('hf', 'flat_no');
	$query->addField('haod', 'hrms_id');
	$query->addField('haod', 'housing_applicant_id');
	$query->addField('haod', 'uid');
	
	$query->addField('hfo', 'flat_occupant_id');
	$query->condition('haod.uid', $uid, '=');
	$query->condition('haod.is_active', 1 , '=');   
	
	
	$fetch_data = $query->execute();
	$fetch_data = $fetch_data->fetchObject();

	if (empty($fetch_data)) {
		
		$query = db_select('housing_existing_occupant_draft', 'heod');
		$query->innerJoin('housing_flat', 'hf', 'hf.flat_id = heod.flat_id');
		$query->innerJoin('housing_estate', 'he', 'hf.estate_id = he.estate_id');
		$query->innerJoin('housing_flat_type', 'hft', 'hf.flat_type_id = hft.flat_type_id');
		$query->innerJoin('housing_block', 'hb', 'hf.block_id = hb.block_id');
		$query->Fields('heod',array('applicant_name', 'flat_id'));
		$query->Fields('he',array('estate_id', 'estate_name'));
		$query->Fields('hb',array('block_id', 'block_name'));
		$query->addField('hft', 'flat_type_id');
		$query->addField('hf', 'flat_no');
		$query->condition('heod.housing_existing_occupant_draft_id', $uid, '=');
		$fetch_data1 = $query->execute();
		$fetch_data = $fetch_data1->fetchObject();
	}

	if(isset($fetch_data->flat_no) && $fetch_data->flat_no != ''){  
		$housing_estate_arr_1[$fetch_data->estate_id]=$fetch_data->estate_name;
		$housing_block_arr_1[$fetch_data->block_id]=$fetch_data->block_name;
		$housing_flat_arr_1[$fetch_data->flat_id]=$fetch_data->flat_no;
	}
	
	
	$form['prrsonal_info']['housing_applicant_id'] = array(
			'#type' => 'hidden',
			'#default_value' => isset($fetch_data->housing_applicant_id) ? $fetch_data->housing_applicant_id : '',
	);
	$form['prrsonal_info']['housing_hrms_id'] = array(
			'#type' => 'hidden',
			'#default_value' => isset($fetch_data->hrms_id) ? $fetch_data->hrms_id : '',
	);
	$form['prrsonal_info']['housing_hidden_uid'] = array(
			'#type' => 'hidden',
			'#default_value' => isset($fetch_data->uid) ? $fetch_data->uid : '',
	);
	$form['prrsonal_info']['housing_hidden_uid_or_draft_id'] = array(
			'#type' => 'hidden',
<<<<<<< HEAD
			'#default_value' => isset($uid) ? $uid : '', //added by dg 28-08-2025
=======
			'#default_value' => isset($uid) ? $uid : '',
>>>>>>> 2502ab393f49b61f25516e6bb0502acbb6d447a0
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
		'#default_value' => isset($fetch_data->hrms_id) ? $fetch_data->hrms_id : '',
		'#disabled'=>isset($fetch_data->hrms_id) ? TRUE : FALSE,
		//'#required' => TRUE,
		//'#default_value' => NULL,
		'#maxlength' => 10,
		'#prefix' => '<div class="row"><div class="col-md-6"><div class="form-floating">',
		'#suffix' => '</div></div>',
		'#element_validate' => array('element_validate_hrms_id', 'hrms_id_validate'),
		'#attributes' => array('class' => array('numeric_positive form-control')),
	);
	$form['official_info']['app_designation'] = array(
		'#title' => t('Designation'),
		'#type' => 'textfield',
		//'#required' => TRUE,
		'#default_value' => 'NA',
		'#prefix' => '<div class="col-md-6"><div class="form-floating">',
		'#suffix' => '</div></div></div>',
		'#element_validate' => array('element_validate_textarea'),
		'#attributes' => array('oninput'=>"this.value=this.value.toUpperCase()",'class'=>array('form-control')),
		//'#attributes' => array('id' => 'edit-app-designation', 'oncopy' => "return false", 'onpaste' => "return false"),
		
	);

	// new code start

	$form['official_info']['pay_type_new'] = array(
		'#type' => 'radios',
  		'#title' => t('Select Pay Type'),
  		'#options' => array('new' => t('New'),'old' => t('Old')),
  		'#default_value' => isset($form_state['values']['pay_type_new']) ? $form_state['values']['pay_type_new'] : 'old',
		'#ajax'	=> array(
			'event' 		=> 'change',
			'effect' 		=> 'fade',			
			'callback'		=> 'get_info_form_ajax_callback',
			'progress'		=>	array('type' => 'throbber','message' => t('Loading Datas...')),
		),
		'#prefix' => '<div class="row"><div class="col-md-4" id="pay_type_new_replace_div"><div class="form-floating">',
		'#suffix' => '</div></div>',
		'#required' => TRUE,
	);

	$selected_type = isset($form_state['values']['pay_type_new']) && !empty($form_state['values']['pay_type_new']) ? $form_state['values']['pay_type_new'] : 'old';
<<<<<<< HEAD
	
=======
	// $selected_band = isset($form_state['values']['pay_type_new']) ? $form_state['values']['pay_type_new'] : '';

	// If Pay Type changed, reset Pay Band
	// if (isset($form_state['triggering_element']['#name']) && $form_state['triggering_element']['#name'] == 'pay_type_new') {
	//     $selected_band = ''; // clear selection when pay type changes
	// }
>>>>>>> 2502ab393f49b61f25516e6bb0502acbb6d447a0

	if(!empty($selected_type) && $selected_type!=''){

		// Get selected pay band ID
		$pay_band_id = isset($form_state['values']['pay_band']) ? $form_state['values']['pay_band'] : '';

		  // Determine flat type text
		if (!empty($pay_band_id) && $pay_band_id !='') {
		    $flat_type = flat_type_name_by_band($pay_band_id);
		    $flat_type_text = 'Your flat type is: ' . check_plain($flat_type);
		} else {
		    $flat_type_text = 'No pay band selected';
		}


		$form['official_info']['pay_band'] = array(
			'#type' => 'select',
			'#title' => t('Pay Band'),
			'#options' => array('' => t('- Select Pay Band First -')) + pay_band_list_eo_pa($selected_type),
			// '#default_value' => $selected_band,
			'#ajax'	=> array(
				'event'		=> 'change',
				'effect' 	=> 'fade',
				'callback'	=> 'get_info_form_ajax_callback',
			),
			'#prefix' => '<div class="col-md-4" id="pay_band_replace_div"><div class="form-floating">',
			'#suffix' => '</div></div>',
			'#required' => TRUE,
		);

		$flat_type_text = isset($form_state['values']['pay_band']) ? $flat_type_text : '';
<<<<<<< HEAD
		
=======
		// if($form_state['values']['pay_band'] ==''){
		// 	$flat_type_text = '';
		// }else{
		// 	$flat_type_text = $flat_type_text;
		// }
>>>>>>> 2502ab393f49b61f25516e6bb0502acbb6d447a0

		$form['official_info']['flat_type_display'] = array(
			'#type' => 'textfield',
			'#title' => t('Flat Type'),
			'#value' => $flat_type_text,
			'#ajax'	=> array(
					'event'		=> 'change',
					'effect' 	=> 'fade',
					'callback'	=> 'get_info_form_ajax_callback',
			),
			'#prefix' => '<div class="col-md-4" id="flat_type_display_div"><div class="form-floating">',
			'#suffix' => '</div></div>',
			'#attributes' => array('class'=>array('form-control')),
			'#required' => TRUE,
			'#disabled' => TRUE
		);
	}else{
		$form['official_info']['pay_band'] = array(
			'#type' => 'select',
			'#title' => t('Pay Band'),
			'#options' => array('' => t('- Select Pay Band First -')) + pay_band_list_eo_pa('old'),
			// '#default_value' => $selected_band,
			'#ajax'	=> array(
				'event'		=> 'change',
				'effect' 	=> 'fade',
				'callback'	=> 'get_info_form_ajax_callback',
			),
			'#prefix' => '<div class="col-md-4" id="pay_band_replace_div"><div class="form-floating">',
			'#suffix' => '</div></div>',
			'#required' => TRUE,
		);

		$flat_type_text = isset($form_state['values']['pay_band']) ? $flat_type_text : '';

		$form['official_info']['flat_type_display'] = array(
			'#type' => 'textfield',
			'#title' => t('Flat Type'),
			'#value' => '',
			'#ajax'	=> array(
					'event'		=> 'change',
					'effect' 	=> 'fade',
					'callback'	=> 'get_info_form_ajax_callback',
			),
			'#prefix' => '<div class="col-md-4" id="flat_type_display_div"><div class="form-floating">',
			'#suffix' => '</div></div>',
			'#attributes' => array('class'=>array('form-control')),
			'#required' => TRUE,
			'#disabled' => TRUE
		);

	}

	// $form['official_info']['flat_type_display'] = array(
	//   '#type' => 'markup',
	//   '#markup' => '<div id="flat_type_display_div">Your flat type is: </div>',
	//   '#prefix' => '<div class="col-md-4" id="flat_type_display_wrapper"><div class="form-floating">',
	//   '#suffix' => '</div></div>',
	//   '#ajax'	=> array(
	// 			'event'		=> 'change',
	// 			'effect' 	=> 'fade',
	// 			'callback'	=> 'get_info_form_ajax_callback',
	// 		),
	// );

	



	

	$form['official_info']['pay_in'] = array(
		 '#title' => t('Basic Pay'),
		 '#type' => 'textfield',
		 //'#required' => TRUE,
		 '#default_value' => '1',
		 '#element_validate' => array('element_validate_numeric_positive'), 
		 '#prefix' => '<div class="col-md-4"><div class="form-floating">',
		 '#suffix' => '</div></div></div>',
		 '#attributes' => array('class' => array('numeric_positive form-control')),
	);
	
	// new code end


    $form['official_info']['app_posting_place'] = array(
		'#title' => t('Place of Posting'),
		'#type' => 'textfield',
		//'#required' => TRUE,
		'#default_value' => 'NA',
		'#prefix' => '<div class="row"><div class="col-md-6"><div class="form-floating">',
		'#suffix' => '</div></div>',
		//'#element_validate' => array('element_validate_alphabatic_text'),
		'#attributes' => array('oninput'=>"this.value=this.value.toUpperCase()", 'class' => array('form-control')), 
	); 

	$form['official_info']['app_headquarter'] = array(
		'#title' => t('Headquarter'),
		'#type' => 'textfield',
		//'#required' => TRUE,
		'#default_value' => 'NA',
		'#prefix' => '<div class="col-md-6"><div class="form-floating">',
		'#suffix' => '</div></div></div>',
		'#element_validate' => array('element_validate_alphabatic_text'),
		'#attributes' => array('oninput'=>"this.value=this.value.toUpperCase()", 'class' => array('form-control')), 	
	);

	$form['official_info']['doj'] = array(
		'#title' => t('Date of Joining'),
		'#type' => 'textfield',
		'#default_value' => '01/01/1900',
		//'#attributes' => array('readonly' => 'readonly', 'id' => 'edit-doj'),
		'#attributes' => array('id' => 'edit-doj', 'class' => array('numeric_positive form-control')),
		//'#required' => TRUE,
		'#prefix' => '<div class="row"><div class="col-md-6"><div class="form-floating">',
		'#suffix' => '</div></div>',
		'#element_validate' => array('element_validate_date'),
	);
	
	$form['official_info']['dor'] = array(
		'#title' => t('Date of Retirement(According to Service Book)'),
		'#type' => 'textfield',
		//'#value' => $date2,
		//'#attributes' => array('readonly' => 'readonly', 'id' => 'edit-dor'),
		'#attributes' => array('id' => 'edit-dor', 'class' => array('numeric_positive form-control')),
		//'#default_value' => '',
		'#required' => TRUE,
		//#prefix' => '<div class="four" id="replace_dor">',
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
		//'#required' => TRUE,
		'#default_value' => 'NA',
		'#prefix' => '<div class="row"><div class="col-md-4"><div class="form-floating">',
		'#suffix' => '</div></div>',
		'#element_validate' => array('element_validate_textarea'), 
		// '#attributes' => array('oninput'=>"this.value=this.value.toUpperCase()", 'class' => array('numeric_positive form-control')), // off on 22-07-2025 By Moumita
		'#attributes' => array('oninput'=>"this.value=this.value.toUpperCase()", 'class' => array('form-control')),
	);
	
	$form['office_add']['office_street'] = array(
		'#title' => t('Address'),
		'#type' => 'textfield',
		//'#required' => TRUE,
		'#default_value' => 'NA',
		'#prefix' => '<div class="col-md-4"><div class="form-floating">',
		'#suffix' => '</div></div>',
		//'#element_validate' => array('element_validate_streetaddress'), 
		'#attributes' => array('oninput'=>"this.value=this.value.toUpperCase()",'class' => array('form-control') ),	
	);

	$form['office_add']['office_city'] = array(
		'#title' => t('City / Town / Village'),
		'#type' => 'textfield',
		//'#required' => TRUE,
		'#default_value' => 'NA',
		'#prefix' => '<div class="col-md-4"><div class="form-floating">',
		'#suffix' => '</div></div></div>',
		'#element_validate' => array('element_validate_alphabatic_text'), 
		'#attributes' => array('oninput'=>"this.value=this.value.toUpperCase()", 'class' => array('form-control')),
	);

	$form['office_add']['office_post_office'] = array(
		'#title' => t('Post Office'),
		'#type' => 'textfield',
		//'#required' => TRUE,
		'#default_value' => 'NA',
		'#prefix' => '<div class="row"><div class="col-md-3"><div class="form-floating">',
		'#suffix' => '</div></div>',
		'#element_validate' => array('element_validate_alphabatic_text'), 
		'#attributes' => array('oninput'=>"this.value=this.value.toUpperCase()", 'class' => array('form-control')),	
	);
	
	$form['office_add']['office_district'] = array(
		'#title' => t('District'),
		'#type' => 'select',
		'#options' => district_list(),
		//'#required' => TRUE,
		'#default_value' => '17',
		'#prefix' => '<div class="col-md-3"><div class="form-floating">',
		'#suffix' => '</div></div>',
		'#attributes' => array('class' => array('form-select'))
	);

	$form['office_add']['office_pincode'] = array(
		'#title' => t('Pincode'),
		'#type' => 'textfield',
		'#maxlength' => 6,
		//'#required' => TRUE,
		'#default_value' => '700001',
		'#element_validate' => array('element_validate_numeric_positive'), 
		'#prefix' => '<div class="col-md-3"><div class="form-floating">',
		'#suffix' => '</div></div>',
		'#attributes' => array('class' => array('numeric_positive form-control')),
	);

	$form['office_add']['office_phone_no'] = array(
		'#title' => t('Phone No.(With STD Code)'),
		'#type' => 'textfield',
		'#maxlength' => 15,
		//'#required' => TRUE,
		'#default_value' => '033-22222222',
		'#element_validate' => array('element_validate_telephoneno'), 
		'#prefix' => '<div class="col-md-3"><div class="form-floating">',
		'#suffix' => '</div></div></div>',
		'#attributes' => array('class' => array('numeric_positive form-select')),
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
		//'#options' => array('- Select -'),
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
		'#default_value' => '17', //$designation,
		//'#required' => TRUE,
		//'#validated' => TRUE,
		'#prefix' => '<div id="replace_designation" class="col-md-6"><div class="form-floating">',
		'#suffix' => '</div></div>',
	);
	 
    
    $form['prrsonal_info']['online_vs_id'] = array(
			'#type' => 'hidden',
			'#default_value' => '0',
			//'#validated' => TRUE,
	);
			
	  
    $form['current_occupation'] = array(
        '#type' => 'fieldset',
        '#title' => t('Current Occupency Details'),
        '#collapsible' => FALSE, // Added
        '#collapsed' => FALSE,  // Added
    );
    
    $form['current_occupation']['occupation_estate_start'] = array(
        '#type' => 'markup',
        '#markup' => '<div class="row">'
    );

    $form['current_occupation']['occupation_estate'] = array(
        '#title' => t('Select Housing'),
        '#type' => 'select',
        //'#options' => get_housing_estate(),
        '#options' => ($fetch_data->estate_id !='') ? $housing_estate_arr_1 : get_housing_estate(),
        //'#options' => $housing_estate_arr_1,
        '#default_value' => ($fetch_data->estate_id != '') ? $fetch_data->estate_id : $occupation_estate,
        '#required' => TRUE,
        '#ajax' => array(
                        'event' => 'change',
                        'callback' => '_reload_block_no_vs',
                        'effect' => 'fade',
                        'progress' => array(
                            'type' => 'throbber',
                            'message' => '',
                        ),
            ),
        '#prefix' => '<div class="col-md-6"><div class="form-floating">',
        '#suffix' => '</div></div>',
        '#validated' => TRUE,
        '#attributes' => array('class' => array('form-select')),
        // '#select2' => array(
        // 	'width' => '500',
        // 	//'placeholder' => '- Select -',
        // 	'allowClear' => TRUE
        // )
    );
    
    
    if($fetch_data->estate_id != '') {
        $form['current_occupation']['occupation_block'] = array(
            '#title' => t('Select Block'),
            '#type' => 'select',
            //'#options' => get_housing_block($occupation_estate),
            '#default_value' =>$fetch_data->block_id,
            '#options' => $housing_block_arr_1,
            '#required' => TRUE,
            '#ajax' => array(
                            'event' => 'change',
                            'callback' => '_reload_flat_no',
                            'effect' => 'fade',
                            'progress' => array(
                                'type' => 'throbber',
                                'message' => '',
                            ),
                ),
            '#prefix' => '<div class="col-md-6" id="block_replace"><div class="form-floating">',
            '#suffix' => '</div></div>',
            '#attributes' => array('class' => array('form-select')),
            '#validated' => TRUE,
            // '#select2' => array(
            // 	'width' => '500',
            // 	//'placeholder' => '- Select -',
            // 	'allowClear' => TRUE
            // )
        );	
    }else{
        $form['current_occupation']['occupation_block'] = array(
            '#title' => t('Select Block'),
            '#type' => 'select',
            '#options' => get_housing_block($form_state['values']['occupation_estate']),
            '#default_value' => $occupation_block,
            '#required' => TRUE,
            '#ajax' => array(
                            'event' => 'change',
                            'callback' => '_reload_flat_no',
                            'effect' => 'fade',
                            'progress' => array(
                                'type' => 'throbber',
                                'message' => '',
                                ),
                    ),
            '#prefix' => '<div class="col-md-6" id="block_replace"><div class="form-floating">',
            '#suffix' => '</div></div>',
            '#attributes' => array('class' => array('form-select')),
            '#validated' => TRUE,
            // '#select2' => array(
            // 	'width' => '500',
            // 	//'placeholder' => '- Select -',
            // 	'allowClear' => TRUE
            // )
        );

    }

    $form['current_occupation']['occupation_block_end'] = array(
        '#type' => 'markup',
        '#markup' => '</div>'
    );

    $form['current_occupation']['occupation_flat_start'] = array(
        '#type' => 'markup',
        '#markup' => '<div class="row">'
    );
    
    
    $form['current_occupation']['occupation_flat'] = array(
        '#title' => t('Flat No'),
        '#type' => 'select',
        '#options' => array('' => '-- Select --'),
        '#default_value' => isset($form_state['values']['occupation_flat']) ? $form_state['values']['occupation_flat'] : '',
        '#required' => TRUE,
        '#prefix' => '<div class="col-md-6" id="flat_no_replace"><div class="form-floating">',
        '#suffix' => '</div></div>',
        '#attributes' => array('class' => array('form-select')),
        '#validated' => TRUE,
        
    );
        
    if($fetch_data->estate_id != '' && $fetch_data->block_id != '') {
        $form['current_occupation']['occupation_flat'] = array(
            '#title' => t('Flat No'),
            '#type' => 'select',
            //'#options' => get_housing_flat($occupation_estate, $occupation_block),
            '#default_value' => isset($form_state['values']['occupation_flat']) ? $form_state['values']['occupation_flat'] : '',
            '#options' => $housing_flat_arr_1,
            '#required' => TRUE,
            '#prefix' => '<div class="col-md-6" id="flat_no_replace"><div class="form-floating">',
            '#suffix' => '</div></div>',
            '#validated' => TRUE,
            '#attributes' => array('class' => array('form-select')),
           
        );
    }else{
            
            $form['current_occupation']['occupation_flat'] = array(
            '#title' => t('Flat No'),
            '#type' => 'select',
            '#options' => get_housing_flat($occupation_estate, $occupation_block),
            '#default_value' => isset($form_state['values']['occupation_flat']) ? $form_state['values']['occupation_flat'] : '',
            '#required' => TRUE,
            '#prefix' => '<div class="col-md-6" id="flat_no_replace"><div class="form-floating">',
            '#suffix' => '</div></div>',
            '#attributes' => array('class' => array('form-select')),
            '#validated' => TRUE,
            
        );

    }
    
    $form['current_occupation']['possession_date'] = array(
        '#title' => t('Date of Possession'),
        '#type' => 'textfield',
        '#required' => TRUE,
        '#attributes' => array('readonly' => 'readonly', 'id' => 'dop_vs', 'class' => array('form-control')),
        //'#default_value' => '',
        //'#validated' => TRUE,
        '#prefix' => '<div class="col-md-6"><div class="form-floating">',
        '#suffix' => '</div></div>',
        '#element_validate' => array('element_validate_date'),
    );

    $form['current_occupation']['possession_date_end'] = array(
        '#type' => 'markup',
        '#markup' => '</div>'
    );

	$form['current_occupation']['license_no'] = array(
			'#title' => t('License Number'),
			'#type' => 'textfield',
			'#default_value' => 'NA',
			'#prefix' => '<div class="row"><div class="col-md-6"><div class="form-floating">',
			'#suffix' => '</div></div>',

			//'#element_validate' => array(''), 
			//'#required' => TRUE,
			'#attributes' => array('oninput'=>"this.value=this.value.toUpperCase()", 'class'=>array('form-control')),
	);

	$form['current_occupation']['physical_application_no'] = array(
			'#title' => t('Physical Application Number'),
			'#type' => 'textfield',
			'#default_value' => 'NA',
			//'#required' => TRUE,
			'#attributes' => array('class' => array('form-control ')),
			// '#element_validate' => array('element_validate_numeric_positive'),
			'#prefix' => '<div class="col-md-6"><div class="form-floating">',
			'#suffix' => '</div></div></div>',
			//'#attributes' => array('class' => array('numeric_positive form-control')),
	);
    
        
    $form['submit'] = array(
		'#prefix' =>'<div class="col-12 border-top pt-3">',
		'#suffix' => '</div><div class="mb-5 pb-3"></div>',
		'#value' => t('Apply'),
		'#type' => 'submit',
		'#weight' => 51,
		'#attributes' => array('class'=>array('btn bg-primary btn-sm px-5 rounded-pill text-white fw-bolder'),'id' => array('savebtn')),
	); 
    
    

	$form['form_end'] = array(
		'#type' => 'markup',
		'#markup' => '</div>',
	);

	return $form;
}



function legacy_vs_cs_form_submit($form, &$form_state){
<<<<<<< HEAD
	// echo "<pre>";print_r($form_state['values']);
	// echo 'Under Maintenance';die;


// 	$bigint_fields = ['ddo_id', 'pay_band_id', 'office_phone_no'];

// foreach ($bigint_fields as $field) {
//   if (isset($form_state['values'][$field]) && $form_state['values'][$field] === '') {
//     $form_state['values'][$field] = NULL;
//   }
// }
=======
	// echo "<pre>";print_r($form_state['values']);die;

>>>>>>> 2502ab393f49b61f25516e6bb0502acbb6d447a0
	$housing_applicant_id = $form_state['values']['housing_applicant_id'];
	$housing_hidden_hrms_id = $form_state['values']['housing_hrms_id'];
	$housing_hidden_uid = $form_state['values']['housing_hidden_uid'];
	$housing_hidden_uid_or_draft_id = $form_state['values']['housing_hidden_uid_or_draft_id'];
	// echo $housing_hidden_uid;die;
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
			
	$username = str_replace(".","",$name).rand(1, 100000);

	if($housing_hidden_hrms_id == ''){
		if($form_state['values']['hrms_id'] != '')
		{
			$newUser	   	   = array();
			$newUser['name']   = trim($form_state['values']['hrms_id']);
			$newUser['pass']   = trim($form_state['values']['hrms_id']);

			if($form_state['values']['email'] != ''){
				$newUser['mail']   = trim($form_state['values']['email']);
			}else{
				$newUser['mail']   = trim($form_state['values']['hrms_id']).'@gmail.com';
			}	
			$newUser['status'] = 1;
		}else{
			/*insert into user table*/
			$newUser	   	   = array();
			$newUser['name']   = $username;
			$newUser['pass']   = $username;
			
			if($form_state['values']['email'] != ''){
				$newUser['mail']   = trim($form_state['values']['email']);
			}else{
				$newUser['mail']   = $username.'@gmail.com';
			}																																				$newUser['status'] = 1;
			
		}	
		// echo "<pre>";print_r($newUser);die;
		user_save(null, $newUser);	
		/*get last inserted uid*/
		
		$query = db_select('users','u');
		$query->addExpression('MAX(uid)','max_uid');
		$result = $query->execute();
		$data = $result->fetchObject();
	}else{
		$email = trim($form_state['values']['email']);

		db_update('users')  
		->fields(array('mail' => $email))  
		->condition('name', $housing_hidden_hrms_id)  
		->execute();


	}



	if(empty($housing_hidden_hrms_id) && !empty($form_state['values']['hrms_id'])){
		$query = db_select('housing_existing_occupant_draft', 'heod');
		$query->innerJoin('housing_flat', 'hf', 'hf.flat_id = heod.flat_id');
		$query->innerJoin('housing_estate', 'he', 'hf.estate_id = he.estate_id');
		$query->innerJoin('housing_flat_type', 'hft', 'hf.flat_type_id = hft.flat_type_id');
		$query->innerJoin('housing_block', 'hb', 'hf.block_id = hb.block_id');
		$query->Fields('heod',array());
		$query->Fields('he',array('estate_id', 'estate_name'));
		$query->Fields('hb',array('block_id', 'block_name'));
		$query->addField('hft', 'flat_type_id');
		$query->addField('hf', 'flat_no');
		$query->condition('heod.housing_existing_occupant_draft_id', $housing_hidden_uid_or_draft_id, '=');
		$fetch_draft_data = $query->execute();
		$fetch_draft_data = $fetch_draft_data->fetchObject();
		// echo "<pre>";print_r($fetch_draft_data);die;

		$query = db_query("SELECT MAX(uid) FROM {users} WHERE name = :name", array(':name' => $form_state['values']['hrms_id']));
		$max_hrms_uid = $query->fetchField();
		//echo $max_hrms_id;die;
		$housingAppliArr = array(
			'uid' => $max_hrms_uid,
			'applicant_name' => $fetch_draft_data->applicant_name,
			'guardian_name' => $fetch_draft_data->guardian_name,
			'gender' => $fetch_draft_data->gender,
			'date_of_birth' => $fetch_draft_data->date_of_birth,
			'mobile_no' => $fetch_draft_data->mobile_no,
			'permanent_street' => $fetch_draft_data->permanent_street,
			'permanent_city_town_village' => $fetch_draft_data->permanent_city_town_village,
			'permanent_post_office' => $fetch_draft_data->permanent_post_office,
			'permanent_district' => $fetch_draft_data->permanent_district,
			'permanent_pincode' => $fetch_draft_data->permanent_pincode,
			'present_street' => $fetch_draft_data->present_street,
			'present_city_town_village' => $fetch_draft_data->present_city_town_village,
			'present_post_office' => $fetch_draft_data->present_post_office,
			'present_district' => $fetch_draft_data->present_district,
			'present_pincode' => $fetch_draft_data->present_pincode,

		);

		$max_housing_applicant_id = db_insert('housing_applicant')        
		  ->fields($housingAppliArr)
		  ->execute();
		  //echo $max_housing_applicant_id;die;
		if(!empty($max_housing_applicant_id)){
			$checked_ddo = (!empty($fetch_draft_data->ddo_id) || ($fetch_draft_data->ddo_id  != ''))  ? $fetch_draft_data->ddo_id :  1263;
			$checked_pay_band_id = (!empty($fetch_draft_data->pay_band_id) || ($fetch_draft_data->pay_band_id  != ''))  ? $fetch_draft_data->pay_band_id :  10;
			$housingAppliOffic = array(
			'uid' => $max_hrms_uid,
			'housing_applicant_id' => $max_housing_applicant_id,
			'hrms_id' => $form_state['values']['hrms_id'],
			'ddo_id' => $checked_ddo,
			'applicant_designation' => $fetch_draft_data->applicant_designation,
			'applicant_headquarter' => $fetch_draft_data->applicant_headquarter,
			'applicant_posting_place' => $fetch_draft_data->applicant_posting_place,
			'pay_band_id' => $checked_pay_band_id,
			'pay_in_the_pay_band' => $fetch_draft_data->pay_in_the_pay_band,
			'date_of_joining' => $fetch_draft_data->date_of_joining,
			'date_of_retirement' => $fetch_draft_data->date_of_retirement,
			'office_name' => $fetch_draft_data->office_name,
			'office_street' => $fetch_draft_data->office_street,
			'office_city_town_village' => $fetch_draft_data->office_city_town_village,
			'office_post_office' => $fetch_draft_data->office_post_office,
			'office_pin_code' => $fetch_draft_data->office_pin_code,
			'office_district' => $fetch_draft_data->office_district,
			'office_phone_no' => $fetch_draft_data->office_phone_no,
			'is_active' => 0,

		);

		$max_housing_applicant_official_id = db_insert('housing_applicant_official_detail')        
														  ->fields($housingAppliOffic)
														  ->execute();
		}

		if(!empty($max_housing_applicant_official_id)){
			$housing_online_applicationArr = array(
			'applicant_official_detail_id' => $max_housing_applicant_official_id,
			'status' => 'existing_occupant',
			'is_backlog_applicant' => 2
		);

		$max_housing_online_application_id = db_insert('housing_online_application')        
											  ->fields($housing_online_applicationArr)
											  ->execute();
		db_update('housing_online_application')->fields(array('application_no' => 'EO-'.trim(date('dmY')).'-'.$max_housing_online_application_id))->condition('online_application_id', $max_housing_online_application_id)->execute();

		}

		if(!empty($max_housing_online_application_id)){
			$housing_flat_occupantArr = array(
				'online_application_id' => $max_housing_online_application_id,
				'flat_id' => $fetch_draft_data->flat_id,
				'allotment_date' => NULL, //added by dg 28-08-2025
			);

			$max_flat_occupant_id = db_insert('housing_flat_occupant')        
											  ->fields($housing_flat_occupantArr)
											  ->execute();

			db_update('housing_flat')->fields(array('flat_status_id' => 2))->condition('flat_id', $fetch_draft_data->flat_id)->execute();
			
			$housing_occupant_license_arr = array(
				'flat_occupant_id' => $max_flat_occupant_id,
				'license_no' => $fetch_draft_data->license_no,
				'license_issue_date' => $fetch_draft_data->license_issue_date,
				'license_expiry_date' => $fetch_draft_data->license_expiry_date,
				'authorised_or_not' => $fetch_draft_data->authorised_or_not
			);
			db_insert('housing_occupant_license')->fields($housing_occupant_license_arr)->execute();

			$housing_new_allotment_application_arr = array(
				'online_application_id' => $max_housing_online_application_id
			);
			db_insert('housing_new_allotment_application')->fields($housing_new_allotment_application_arr)->execute();								  

		}
	}


		//echo (trim($data->max_uid));
		//.'<br>AFASfa'.print_r($data);
		//die;
		$applicant_personal_detail_arr = array();
		$applicant_personal_detail_arr['uid'] 				=  !empty($housing_hidden_hrms_id) ? $housing_hidden_uid : trim($data->max_uid);
		$applicant_personal_detail_arr['applicant_name'] 	=  trim($form_state['values']['applicant_name']);
		$applicant_personal_detail_arr['guardian_name'] 		=  trim($form_state['values']['applicant_father_name']);
		
		$applicant_personal_detail_arr['permanent_street']  =  strtoupper(trim($form_state['values']['permanent_street']));
		$applicant_personal_detail_arr['permanent_city_town_village']  =  strtoupper(trim($form_state['values']['permanent_city_town_village']));
		$applicant_personal_detail_arr['permanent_post_office']  =  strtoupper(trim($form_state['values']['permanent_post_office']));
		$applicant_personal_detail_arr['permanent_district']  =  trim($form_state['values']['permanent_district']);
		$applicant_personal_detail_arr['permanent_pincode']  =  !empty($form_state['values']['permanent_pincode']) ? trim($form_state['values']['permanent_pincode']) : 0;

		$applicant_personal_detail_arr['present_street']  =  strtoupper(trim($form_state['values']['present_street']));
		$applicant_personal_detail_arr['present_city_town_village']  =  strtoupper(trim($form_state['values']['present_city_town_village']));
		$applicant_personal_detail_arr['present_post_office']  =  strtoupper(trim($form_state['values']['present_post_office']));
		$applicant_personal_detail_arr['present_district']  =  trim($form_state['values']['present_district']);
		$applicant_personal_detail_arr['present_pincode']  =  !empty($form_state['values']['present_pincode']) ? trim($form_state['values']['present_pincode']) : 0;
		$applicant_personal_detail_arr['mobile_no'] 			= !empty($form_state['values']['mobile']) ? trim($form_state['values']['mobile']) : 0; 
		$applicant_personal_detail_arr['date_of_birth'] 		=  trim(implode('-', array_reverse(explode('/', $form_state['values']['dob']))));
		$applicant_personal_detail_arr['gender'] 			=  trim($form_state['values']['gender']);
		// echo "<pre>";print_r($applicant_personal_detail_arr);die;
		$last_housing_applicant_id = db_insert('housing_applicant')
		->fields($applicant_personal_detail_arr)
		->execute();

		$arr_user_roles = array();
		$arr_user_roles['uid'] = !empty($housing_hidden_hrms_id) ? $housing_hidden_uid : trim($data->max_uid);
		$arr_user_roles['rid'] = 4;
		
		db_insert('users_roles')
		->fields($arr_user_roles)
		->execute();

		/*add applicant_official_detail  data*/
		$app_off_detail_arr = array();
		$app_off_detail_arr['uid']  =  !empty($housing_hidden_hrms_id) ? $housing_hidden_uid : trim($data->max_uid);
		$app_off_detail_arr['hrms_id'] = !empty($form_state['values']['hrms_id']) ? trim($form_state['values']['hrms_id']) : 0;
		$app_off_detail_arr['applicant_designation']  =  strtoupper(trim($form_state['values']['app_designation']));
<<<<<<< HEAD
		$app_off_detail_arr['pay_band_id']  = $form_state['values']['pay_band']; 
=======
		$app_off_detail_arr['pay_band_id']  = !empty($form_state['values']['pay_band']) ? trim($form_state['values']['pay_band']) : 0; 
>>>>>>> 2502ab393f49b61f25516e6bb0502acbb6d447a0
		$app_off_detail_arr['pay_in_the_pay_band']  =  trim($form_state['values']['pay_in']);
		$app_off_detail_arr['applicant_posting_place']  =  strtoupper(trim($form_state['values']['app_posting_place']));
		$app_off_detail_arr['applicant_headquarter']  =  strtoupper(trim($form_state['values']['app_headquarter']));
		$app_off_detail_arr['date_of_joining']  =  trim( implode('-', array_reverse(explode('/', $form_state['values']['doj']))));
		$app_off_detail_arr['date_of_retirement']  =  trim( implode('-', array_reverse(explode('/', $form_state['values']['dor']))));
		$app_off_detail_arr['office_name']  =  strtoupper(trim($form_state['values']['office_name']));
		$app_off_detail_arr['office_street']  =  strtoupper(trim($form_state['values']['office_street']));
		$app_off_detail_arr['office_city_town_village']  = strtoupper(trim($form_state['values']['office_city']));
		$app_off_detail_arr['office_post_office']  =  strtoupper(trim($form_state['values']['office_post_office']));
		$app_off_detail_arr['office_district']  =  trim($form_state['values']['office_district']);
		$app_off_detail_arr['office_pin_code']  = !empty($form_state['values']['office_pincode']) ? trim($form_state['values']['office_pincode']) : 0;  
		$app_off_detail_arr['office_phone_no']  = !empty($form_state['values']['office_phone_no']) ? trim($form_state['values']['office_phone_no']) : 0;   
<<<<<<< HEAD
		//$app_off_detail_arr['ddo_id']  = !empty($form_state['values']['designation']) ? trim($form_state['values']['designation']) : NULL; 
		$designation = trim($form_state['values']['designation']);
		if ($designation !== '') {
		    $app_off_detail_arr['ddo_id'] = (int) $designation;
		}  
=======
		//$app_off_detail_arr['ddo_id']  = trim($form_state['values']['designation']);
		$designation = trim($form_state['values']['designation']);
		if ($designation !== '') {
		    $app_off_detail_arr['ddo_id'] = (int) $designation;
		}
		//$app_off_detail_arr['ddo_id'] = (int) trim($form_state['values']['designation']);   
>>>>>>> 2502ab393f49b61f25516e6bb0502acbb6d447a0
		$app_off_detail_arr['housing_applicant_id']  =   $last_housing_applicant_id;
		// echo "<pre>";print_r($app_off_detail_arr);die;
		db_insert('housing_applicant_official_detail')
  	  	->fields($app_off_detail_arr)
  	  	->execute();

		/*fetching applicant_official_detail_id*/
		$query1 = db_select('housing_applicant_official_detail', 'haod');
		$query1->addExpression('MAX(applicant_official_detail_id)','applicant_official_detail_id');
		if(!empty($housing_hidden_hrms_id)){
			$query1->condition('haod.uid', $housing_hidden_uid, '=');
		}else{
			$query1->condition('haod.uid', $data->max_uid, '=');
		}
			
		$result1 = $query1->execute();
		$data1 = $result1->fetchObject();
		//  echo "<pre>";print_r($form_state['values']['application_type']);die;
		/*add online application*/
		
		$online_app_arr = array();
		$online_app_arr['applicant_official_detail_id']  =  trim($data1->applicant_official_detail_id);
		$online_app_arr['status']  =  'housingapprover_approved_1'; 
		$online_app_arr['is_backlog_applicant']  =  1;	
		$online_app_arr['physical_application_no']  =  trim($form_state['values']['physical_application_no']);
<<<<<<< HEAD
		$online_app_arr['application_no'] = trim($form_state['values']['application_type']);	
		$online_app_arr['date_of_application'] = trim( implode('-', array_reverse(explode('/', $form_state['values']['application_date']))));
=======
		$online_app_arr['application_no'] = trim($form_state['values']['application_type']);
		$online_app_arr['date_of_application'] = trim( implode('-', array_reverse(explode('/', $form_state['values']['application_date']))));	
>>>>>>> 2502ab393f49b61f25516e6bb0502acbb6d447a0
				
		$last_online_application_id = db_insert('housing_online_application')
		->fields($online_app_arr)
		->execute();

		//$last_online_application_id = db_last_insert_id('housing_online_application', 'online_application_id');

		$query = db_select('housing_pay_band_categories', 'hpbc');
		$query->fields('hpbc', array('flat_type_id'));
		$query->condition('hpbc.pay_band_id', $form_state['values']['pay_band'], '=');
		$result = $query->execute();
		$data = $result->fetchObject();

		/*add data for vs or cs*/
		$shifting_app_arr = array();
		$shifting_app_arr['online_application_id']  =  $last_online_application_id;
		$shifting_app_arr['flat_type_id']  =  $data->flat_type_id;
		$shifting_app_arr['occupation_estate']  =   trim($form_state['values']['occupation_estate']);
		$shifting_app_arr['occupation_block']  =   trim($form_state['values']['occupation_block']);
		$shifting_app_arr['occupation_flat']  =   trim($form_state['values']['occupation_flat']);
		$shifting_app_arr['possession_date']  =   trim( implode('-', array_reverse(explode('/', $form_state['values']['possession_date']))));
		$shifting_app_arr['license_no']  =   trim($form_state['values']['license_no']);

		if(trim($form_state['values']['application_type']) == 'VS'){
			db_insert('housing_vs_application')
			->fields($shifting_app_arr)
			->execute();
		}else if(trim($form_state['values']['application_type']) == 'CS'){
			db_insert('housing_cs_application')
			->fields($shifting_app_arr)
			->execute();
		}

		$is_active_update_arr = array();
		if(!empty($form_state['values']['hrms_id']) || !empty($housing_hidden_hrms_id)){
			
			$is_active_update_arr['is_active'] = 1;
			if(!empty($housing_hidden_hrms_id)){
				// echo $housing_applicant_id;die;
				$eo_status_updated = array();
				$eo_status_updated['is_active'] = 0;
				db_update('housing_applicant_official_detail')  
				->fields($eo_status_updated)  
				->condition('housing_applicant_id', $housing_applicant_id)  
				->execute();
			}
		}else{
			$is_active_update_arr['is_active'] = 0;
		}
		

		//$checked_housing_applicant_id = !empty($housing_applicant_id) ? $housing_applicant_id : $last_housing_applicant_id;

		db_update('housing_applicant_official_detail')  
		->fields($is_active_update_arr)  
		->condition('housing_applicant_id', $last_housing_applicant_id)  
		->execute();

		
		drupal_set_message(t('Your application has been submitted successfully.'));
		drupal_goto('legacy-vs-cs');
	
}


// function get_info_form_ajax_callback($form, &$form_state){
// 	$data 	= array();
// 	$data[] = ajax_command_replace('#pay_type_new_replace_div', drupal_render($form['official_info']['pay_type_new']));
// 	$data[] = ajax_command_replace('#pay_band_replace_div', drupal_render($form['official_info']['pay_band']));
// 	return array('#type' => 'ajax', '#commands' => $data);

// }

function get_info_form_ajax_callback($form, &$form_state) {
  $commands = array();
<<<<<<< HEAD

=======
  // print_r($form_state['values']['pay_band']);
  // unset($form_state['values']['pay_band']);
  // Replace radios and select dropdown
>>>>>>> 2502ab393f49b61f25516e6bb0502acbb6d447a0
  $commands[] = ajax_command_replace('#pay_type_new_replace_div', drupal_render($form['official_info']['pay_type_new']));
  $commands[] = ajax_command_replace('#pay_band_replace_div', drupal_render($form['official_info']['pay_band']));

  $commands[] = ajax_command_replace('#flat_type_display_div', drupal_render($form['official_info']['flat_type_display']));

<<<<<<< HEAD
 
=======
  // // Get selected pay band ID
  // $pay_band_id = isset($form_state['values']['pay_band']) ? $form_state['values']['pay_band'] : '';

  // // Determine flat type text
  // if (!empty($pay_band_id)) {
  //   $flat_type = flat_type_name_by_band($pay_band_id);
  //   $flat_type_text = 'Your flat type is: ' . check_plain($flat_type);
  // } else {
  //   $flat_type_text = 'No pay band selected';
  // }

  // // Replace the flat type display
  // $commands[] = ajax_command_replace('#flat_type_display_div', '<div id="flat_type_display_div">' . $flat_type_text . '</div>');

>>>>>>> 2502ab393f49b61f25516e6bb0502acbb6d447a0
  return array('#type' => 'ajax', '#commands' => $commands);
}





<<<<<<< HEAD




=======
>>>>>>> 2502ab393f49b61f25516e6bb0502acbb6d447a0
