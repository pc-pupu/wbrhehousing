<?php
function legacy_vs_list_view_wohrms_form($form, &$form_state) {
    global $user, $user_role;

    $rhe_name =  isset($form_state['values']['rhe_name']) && !empty($form_state['values']['rhe_name']) ? $form_state['values']['rhe_name'] : 0;
    $flat_type =  isset($form_state['values']['flat_type']) && !empty($form_state['values']['flat_type']) ? $form_state['values']['flat_type'] : 0;

    $form['list_info'] = array(
        '#prefix' => '<div class="row">',
        '#suffix' => '</div>'
    );

	if($user_role == 13){
		$options = rhe_list_fetch();
	}else{
		$options = rhe_list_specific();
	}
    
    $form['list_info']['rhe_name'] = array(
        '#title' => t('Name of the RHE'),
        '#type' => 'select',
        '#options' => $options,
        '#prefix' => '<div class="row"><div class="col-md-4"><div class="form-floating">',
        '#suffix' => '</div></div>',
        '#attributes' => array('class'=>array('form-select')),
        '#ajax' => array(
            'event' => 'change',
            'callback' => '_reload_vs_wohrms_list_view',
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
                'callback' => '_reload_vs_wohrms_list_view',
                'method' => 'replace',
            ),
        );
    }

    $form['fetch_list'] = array(
        '#prefix' => '<div class="col-md-12" id="flat_list_replace">',
        '#suffix' => '</div>'
    );
    
    if($rhe_name != 0 && $flat_type != 0) {
        $form['fetch_list'] = array(
            '#type' => 'markup',
            '#markup' => legacy_vs_list_fetch_without_hrms($rhe_name, $flat_type),
            '#weight' => 50,
            '#prefix' => '<div class="col-md-12" id="flat_list_replace">',
            '#suffix' => '</div>'
        );
    }
    return $form;
}

function _reload_vs_wohrms_list_view($form, &$form_state) {
    $commands = array();
    
    $commands[] = ajax_command_replace("#flat_type_replace", drupal_render($form['list_info']['flat_type']));
    $commands[] = ajax_command_replace("#flat_list_replace", drupal_render($form['fetch_list']));

    return array('#type' => 'ajax', '#commands' => $commands);
}

function legacy_vs_list_fetch_without_hrms($rhe_name = 0, $flat_type = 0) {  //updated by dg 07-11-2025,in demo previous function is turned off
    global $base_root, $base_path, $user, $user_role;
    $output = '';
    // echo "<pre>";print_r($flat_type);die;                

    $query_dtls = db_select('users_details', 'ud'); 
    $query_dtls->fields('ud');
    $query_dtls->condition('uid',$user->uid,'=');
    $rsult = $query_dtls->execute();
    $data_fetch = $rsult->fetchObject();

	$query = db_select('housing_existing_occupant_draft', 'heod');
	$query->innerJoin('housing_vs_application', 'hva', 'hva.occupation_flat = heod.flat_id');
	$query->innerJoin('housing_flat', 'hf', 'hf.flat_id = heod.flat_id');
	$query->innerJoin('housing_estate', 'he', 'he.estate_id = hva.occupation_estate');
	$query->innerJoin('housing_block', 'hb', 'hb.block_id = hva.occupation_block');
	$query->innerJoin('housing_flat_type', 'hft', 'hft.flat_type_id = hva.flat_type_id');
	$query->innerJoin('housing_online_application', 'hoa', 'hoa.online_application_id = hva.online_application_id');
	$query->innerJoin('housing_applicant_official_detail', 'haod', 'haod.applicant_official_detail_id = hoa.applicant_official_detail_id');
	$query->innerJoin('users', 'u', 'u.uid = haod.uid');
	$query->innerJoin('housing_applicant', 'ha', 'ha.uid = haod.uid');

	$query->condition('hoa.application_no', 'VS%', 'LIKE');
	$query->condition('hoa.is_backlog_applicant', '1', '=');
	//$query->isNull('heod.hrms_id');
	$query->condition('haod.hrms_id', '0', '=');

	$query->fields('ha', array('applicant_name', 'gender', 'mobile_no'));
	$query->fields('hoa', array('application_no', 'date_of_application', 'online_application_id'));
	
	$query->addField('he', 'estate_name');
	$query->addField('hb', 'block_name');
	$query->addField('hft', 'flat_type');
	$query->fields('hf', array('flat_no', 'flat_id'));

    if($data_fetch->division_id != '' && $data_fetch->subdiv_id != '') { 
        if($data_fetch->subdiv_id != 0) {
			$db_and = db_and();														
			$db_and->condition('he.division_id', $data_fetch->division_id, '=');	
			$db_and->condition('he.subdiv_id', $data_fetch->subdiv_id, '=');		
			$query->condition($db_and);	
		}else{
			$db_and = db_and();														
            $db_and->condition('he.division_id', $data_fetch->division_id, '=');			
            $query->condition($db_and);	
		}
	}											

	if($rhe_name != 0 && $flat_type != 0){
		$db_and = db_and();
		$db_and->condition('hf.estate_id', $rhe_name, '=');
		$db_and->condition('hf.flat_type_id', $flat_type, '=');

		$query->condition($db_and);
	}
	$query->orderBy('hoa.date_of_application', 'ASC');
	$result =$query->execute();

    $flat_type_name = '';
    if ($flat_type != 0) {
        $query_flat_type = db_select('housing_flat_type', 'hft')
            ->fields('hft', array('flat_type'))
            ->condition('flat_type_id', $flat_type, '=')
            ->execute();
        
        if ($result_ft = $query_flat_type->fetchObject()) {
            $flat_type_name = $result_ft->flat_type;
        }
    }
    
    $header['sl_no'] = array('data' => 'Sl. No.');
    $header['application_no'] = array('data' => 'Application No');
    $header['applicant_name'] = array('data' => 'Applicant Name');
    $header['date_of_application'] = array('data' => 'Date of Application');
    $header['mobile_no'] = array('data' => 'Mobile No.');
    $header['gender'] = array('data' => 'Gender');
    $header['current_occupancy_details'] = array('data' => 'Current Occupancy Details');
    $header['flat_type_applied_for'] = array('data' => 'Flat Type Applied For');
    $header['action'] = array('data' => 'Action');
    
    $rows =array();
    $output = '';
    $c= 1;

    while($data = $result->fetchObject()) {
      $fields = array();

      $fields[] = $c;		
      $fields[] = $data->application_no;
      $fields[] = $data->applicant_name;
      $fields[] = date('d/m/Y', strtotime($data->date_of_application));
      $fields[] = isset($data->mobile_no) ? $data->mobile_no : 'Data Not Found';
      $fields[] = isset($data->gender) ? $data->gender : 'Data Not Found';
      $fields[] = '<b>RHE Name:</b> ' . $data->estate_name . '<br> <b>Block Name:</b> ' . $data->block_name . '<br> <b>Flat No.:</b> ' . $data->flat_no . '<br> <b>Flat Type:</b> ' . $flat_type_name;
      $fields[] = $data->flat_type;
			
      $fields[] = l('<i class="fa fa-edit"></i> Edit', 'legacy-vs-wohrms-edit/' . encrypt_url($data->online_application_id), array('html' => true, 'attributes' => array('class' => array('btn btn-sm bg-primary px-3 rounded-pill text-white fw-bolder'))));
    
      $rows[] = $fields;
      $c++;
    }

    $variables = array(
      'attributes' => array('class'=>array('table table-list table-striped')),
      'header' => $header,
      'rows' => $rows,
      'sticky' => true,
      'empty' => t("No data found!")
      );
    if(count($rows) > 0) {
      $build['datatable'] = array(
        '#theme' => 'datatable',
        '#header' => $header,
        '#rows' => $rows,
        '#attributes' => array(),
      );
      
      $variables = array(
        'attributes' => array('class'=>array('table table-list table-striped')),
        'header' => $header,
        'rows' => $rows,
      );
      
      $output = theme('datatable', $variables);
    }
    else {
      $output = '<div>
                    <table class="datatable_no_data_found table table-list">
                      <tr class="tr_no_data_found">
                        <th class="th_no_data_found"></th>
                      </tr>
                      <tr class="tr_no_data_found">
                        <td class="td_no_data_found">No data found!</td>
                      </tr>
                    </table>
                </div>';
    }
    return $output;
}

function legacy_vs_wohrms_edit_form($form, &$form_state, $online_application_id = '') {
    $online_application_id = decrypt_url($online_application_id);
	
    $query = db_select('housing_existing_occupant_draft', 'heod');
    $query->innerJoin('housing_vs_application', 'hva', 'hva.occupation_flat = heod.flat_id');
	$query->innerJoin('housing_flat', 'hf', 'hf.flat_id = heod.flat_id');
    $query->innerJoin('housing_estate', 'he', 'he.estate_id = hva.occupation_estate');
    $query->innerJoin('housing_block', 'hb', 'hb.block_id = hva.occupation_block');
    $query->innerJoin('housing_flat_type', 'hft', 'hft.flat_type_id = hva.flat_type_id');
    $query->innerJoin('housing_online_application', 'hoa', 'hoa.online_application_id = hva.online_application_id');
    $query->innerJoin('housing_applicant_official_detail', 'haod', 'haod.applicant_official_detail_id = hoa.applicant_official_detail_id');
    $query->innerJoin('users', 'u', 'u.uid = haod.uid');
    $query->innerJoin('housing_applicant', 'ha', 'ha.uid = haod.uid');
    $query->innerJoin('housing_district', 'hds', 'hds.district_code = haod.office_district');
    $query->leftJoin('housing_ddo', 'hd', 'hd.ddo_id = haod.ddo_id');
    $query->innerJoin('housing_pay_band_categories', 'hpbc', 'hpbc.pay_band_id = haod.pay_band_id');
    $query->condition('hoa.online_application_id', $online_application_id);
    $query->fields('haod', array());
    $query->fields('ha', array());
    $query->fields('hva', array());
    $query->fields('hoa', array('date_of_application','physical_application_no'));
    $query->addField('u', 'mail');
    $query->fields('he', array('estate_id','estate_name'));
    $query->fields('hb', array('block_id','block_name'));
	$query->fields('hf', array('flat_id', 'flat_no'));
    $query->addField('hft', 'flat_type');
    $query->addField('hpbc', 'flag');
    $query->addField('hds','district_name');
    $query->fields('hd',array('ddo_code','district_code', 'ddo_id'));


    $result = $query->execute();
    $data = $result->fetchObject();
    // print_r($data);die;

	if(isset($data->flat_no) && $data->flat_no != ''){  
		$housing_estate_arr_1[$data->estate_id]=$data->estate_name;
		$housing_block_arr_1[$data->block_id]=$data->block_name;
		$housing_flat_arr_1[$data->flat_id]=$data->flat_no;
	}

    $form['form_begin'] = array(
		'#type' => 'markup',
		'#markup' => '<div class="online_application">',
		'#weight' => -2
	);

	$form['hidden_applicant_id'] = array(
        '#type' =>'hidden',
        '#value' =>$data->housing_applicant_id,
        '#attributes' => array('readonly' => 'readonly'),
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

	$form['hidden_applicant_official_detail_id'] = array(
        '#type' =>'hidden',
        '#value' =>$data->applicant_official_detail_id,
        '#attributes' => array('readonly' => 'readonly'),
    );

	$form['application_info'] = array(
		'#type' => 'fieldset',
		'#title' => t('Application Type Information'),
		'#collapsible' => FALSE, // Added
		'#collapsed' => FALSE,  // Added
	);

	$form['application_date'] = array(
		'#title' => t('Date of Application'),
		'#type' => 'textfield',
		'#attributes' => array('id' => 'edit-doa', 'class'=>array('form-control')),
		'#required' => TRUE,
		'#default_value' => date('d/m/Y', strtotime($data->date_of_application)),
		'#prefix' => '<div class="col-md-6"><div class="form-floating">',
		'#suffix' => '</div></div></div>',
		'#element_validate' => array('element_validate_date')
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
		'#prefix' => '<div class="row"><div class="col-md-6"><div class="form-floating">',
		'#suffix' => '</div></div>',
        '#default_value' => $data->applicant_name,
		'#required' => TRUE,
		'#attributes' => array('oninput'=>"this.value=this.value.toUpperCase()", 'class'=>array('form-control'))
	);
		
	$form['prrsonal_info']['applicant_father_name'] = array(
		'#title' => t('Father\'s / Husband\'s Name'),
		'#type' => 'textfield',
		'#default_value' => $data->guardian_name,
		'#prefix' => '<div class="col-md-6"><div class="form-floating">',
		'#suffix' => '</div></div></div>',
		'#element_validate' => array('element_validate_alphabatic_fullstop'), 
		//'#required' => TRUE,
		'#attributes' => array('oninput'=>"this.value=this.value.toUpperCase()", 'class'=>array('form-control'))
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
		'#default_value' => $data->permanent_street,
		'#prefix' => '<div class="row"><div class="col-md-6"><div class="form-floating">',
		'#suffix' => '</div></div>',
		'#element_validate' => array('element_validate_streetaddress'), 
		'#attributes' => array('oninput'=>"this.value=this.value.toUpperCase()", 'class'=>array('form-control'))
	);

	$form['prrsonal_info']['permanent_address']['permanent_city_town_village'] = array(
		'#title' => t('City / Town / Village'),
		'#type' => 'textfield',
		//'#required' => TRUE,
		'#default_value' => $data->permanent_city_town_village,
		'#prefix' => '<div class="col-md-6"><div class="form-floating">',
		'#suffix' => '</div></div></div>',
		'#element_validate' => array('element_validate_alphabatic_text'), 
		'#attributes' => array('oninput'=>"this.value=this.value.toUpperCase()", 'class'=>array('form-control'))
	);

	$form['prrsonal_info']['permanent_address']['permanent_post_office'] = array(
		'#title' => t('Post Office'),
		'#type' => 'textfield',
		//'#required' => TRUE,
		'#default_value' => $data->permanent_post_office,
		'#prefix' => '<div class="row"><div class="col-md-4"><div class="form-floating">',
		'#suffix' => '</div></div>',
		'#element_validate' => array('element_validate_alphabatic_text'), 
		'#attributes' => array('oninput'=>"this.value=this.value.toUpperCase()", 'class'=>array('form-control'))
	);

	$form['prrsonal_info']['permanent_address']['permanent_district'] = array(
		'#title' => t('District'),
		'#type' => 'select',
		'#options' => district_list(),
		'#default_value' => $data->permanent_district,
		'#prefix' => '<div class="col-md-4"><div class="form-floating">',
		'#suffix' => '</div></div>',
		'#attributes' => array('class'=>array('form-select'))
	);

	$form['prrsonal_info']['permanent_address']['permanent_pincode'] = array(
		'#title' => t('Pincode'),
		'#type' => 'textfield',
		'#maxlength' => 6,
		//'#required' => TRUE,
		'#default_value' => $data->permanent_pincode,
		'#element_validate' => array('element_validate_numeric_positive'), 
		'#prefix' => '<div class="col-md-4"><div class="form-floating">',
		'#suffix' => '</div></div></div>',
		'#attributes' => array('class' => array('numeric_positive'), 'class'=>array('form-control'))
	);

	$form['prrsonal_info']['present_address'] = array(
		'#type' => 'fieldset',
		'#title' => t('Present Address'),
		'#collapsible' => FALSE, // Added
		'#collapsed' => FALSE,  // Added
		'#prefix' => '<div id="reload_present_address">',
		'#suffix' => '</div>'
	);

	$form['prrsonal_info']['present_address']['present_street'] = array(
		'#title' => t('Address'),
		'#type' => 'textfield',
		//'#required' => TRUE,
		'#default_value' => $data->present_street,
		'#prefix' => '<div class="row"><div class="col-md-6" id="reload_present_street"><div class="form-floating">',
		'#suffix' => '</div></div>',
		'#element_validate' => array('element_validate_streetaddress'), 
		'#attributes' => array('oninput'=>"this.value=this.value.toUpperCase()", 'class'=>array('form-control')), //'id' => 'present_street',
		
	);

	$form['prrsonal_info']['present_address']['present_city_town_village'] = array(
		'#title' => t('City / Town / Village'),
		'#type' => 'textfield',
		//'#required' => TRUE,
		'#default_value' => $data->present_city_town_village,
		'#prefix' => '<div class="col-md-6" id="reload_present_city_town_village"><div class="form-floating">',
		'#suffix' => '</div></div></div>',
		'#element_validate' => array('element_validate_alphabatic_text'), 
		'#attributes' => array('oninput'=>"this.value=this.value.toUpperCase()", 'class'=>array('form-control')), //'id' => 'present_city_town_village', 
		
	);

	$form['prrsonal_info']['present_address']['present_post_office'] = array(
		'#title' => t('Post Office'),
		'#type' => 'textfield',
		//'#required' => TRUE,
		'#default_value' => $data->present_post_office,
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
		'#default_value' => $data->present_district,
		'#prefix' => '<div class="col-md-4" id="reload_present_district"><div class="form-floating">',
		'#suffix' => '</div></div>',
		'#attributes' => array('class'=>array('form-select'))
	);

	$form['prrsonal_info']['present_address']['present_pincode'] = array(
		'#title' => t('Pincode'),
		'#type' => 'textfield',
		'#maxlength' => 6,
		//'#required' => TRUE,
		'#default_value' => $data->present_pincode,
		'#element_validate' => array('element_validate_numeric_positive'), 
		'#prefix' => '<div class="col-md-4" id="reload_present_pincode"><div class="form-floating">',
		'#suffix' => '</div></div></div>',
		'#attributes' => array('class' => array('numeric_positive'), 'class'=>array('form-control')), //'id' => 'present_pincode', 
	);

    $form['prrsonal_info']['mobile'] = array(
		'#title' => t('Mobile no'),
		'#type' => 'textfield',
		'#default_value' => $data->mobile_no,
		'#maxlength' => 10,
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
		'#attributes' => array('oninput'=>"this.value=this.value.toLowerCase()",'class'=>array('form-control')),
	);
	
	$form['prrsonal_info']['dob'] = array(
		'#title' => t('Date of Birth(According to Service Book)'),
		'#type' => 'textfield',
		'#attributes' => array('id' => 'edit-dob', 'class'=>array('form-control')),
		'#default_value' => date('d/m/Y', strtotime($data->date_of_birth)),
		'#prefix' => '<div class="col-md-4"><div class="form-floating">',
		'#suffix' => '</div></div></div>',
		'#element_validate' => array('element_validate_date'),
	);
	
	$form['prrsonal_info']['gender'] = array(
		'#title' => t('Gender'),
		'#type' => 'radios',
		'#options' => array('M' => 'Male', 'F' => 'Female'),
		'#default_value' => $data->gender,
		'#prefix' => '<div class="row"><div class="col-md-4"><div class="">',
		'#suffix' => '</div></div></div>',
		'#required' => TRUE,
		
	);
		
	// applicant's official information
	$form['official_info'] = array(
		'#type' => 'fieldset',
		'#title' => t('Applicant\'s Official Information'),
		'#collapsible' => FALSE, // Added
		'#collapsed' => FALSE,  // Added
	);

	$form['official_info']['app_designation'] = array(
		'#title' => t('Designation'),
		'#type' => 'textfield',
		//'#required' => TRUE,
		'#default_value' => $data->applicant_designation,
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
  		'#default_value' => isset($form_state['values']['pay_type_new']) ? $form_state['values']['pay_type_new'] : $data->flag,
		'#ajax'	=> array(
			'event' 		=> 'change',
			'effect' 		=> 'fade',			
			'callback'		=> '_reload_get_paydetails_info',
			'progress'		=>	array('type' => 'throbber','message' => t('Loading Datas...')),
		),
		'#prefix' => '<div class="row"><div class="col-md-4" id="pay_type_new_replace_div"><div class="form-floating">',
		'#suffix' => '</div></div>',
		'#required' => TRUE,
	);

	$selected_type = isset($form_state['values']['pay_type_new']) && !empty($form_state['values']['pay_type_new']) ? $form_state['values']['pay_type_new'] : $data->flag;

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
			'#default_value' => $data->pay_band_id,
			'#ajax'	=> array(
				'event'		=> 'change',
				'effect' 	=> 'fade',
				'callback'	=> '_reload_get_paydetails_info',
			),
			'#prefix' => '<div class="col-md-4" id="pay_band_replace_div"><div class="form-floating">',
			'#suffix' => '</div></div>',
			'#required' => TRUE,
		);

		$flat_type_text = isset($form_state['values']['pay_band']) ? $flat_type_text : $data->flat_type;

		$form['official_info']['flat_type_display'] = array(
			'#type' => 'textfield',
			'#title' => t('Flat Type'),
			'#value' => $flat_type_text,
			// '#default_value' => $data->flat_type,
			'#ajax'	=> array(
					'event'		=> 'change',
					'effect' 	=> 'fade',
					'callback'	=> '_reload_get_paydetails_info',
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
				'callback'	=> '_reload_get_paydetails_info',
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
					'callback'	=> '_reload_get_paydetails_info',
			),
			'#prefix' => '<div class="col-md-4" id="flat_type_display_div"><div class="form-floating">',
			'#suffix' => '</div></div>',
			'#attributes' => array('class'=>array('form-control')),
			'#required' => TRUE,
			'#disabled' => TRUE
		);
	}

	$form['official_info']['pay_in'] = array(
		 '#title' => t('Basic Pay'),
		 '#type' => 'textfield',
		 //'#required' => TRUE,
		 '#default_value' => $data->pay_in_the_pay_band,
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
		'#default_value' => $data->applicant_posting_place,
		'#prefix' => '<div class="row"><div class="col-md-6"><div class="form-floating">',
		'#suffix' => '</div></div>',
		//'#element_validate' => array('element_validate_alphabatic_text'),
		'#attributes' => array('oninput'=>"this.value=this.value.toUpperCase()", 'class' => array('form-control')), 
	); 

	$form['official_info']['app_headquarter'] = array(
		'#title' => t('Headquarter'),
		'#type' => 'textfield',
		//'#required' => TRUE,
		'#default_value' => $data->applicant_headquarter,
		'#prefix' => '<div class="col-md-6"><div class="form-floating">',
		'#suffix' => '</div></div></div>',
		'#element_validate' => array('element_validate_alphabatic_text'),
		'#attributes' => array('oninput'=>"this.value=this.value.toUpperCase()", 'class' => array('form-control')), 	
	);

	$form['official_info']['doj'] = array(
		'#title' => t('Date of Joining'),
		'#type' => 'textfield',
		'#default_value' => date('d/m/Y', strtotime($data->date_of_joining)),
		'#attributes' => array('id' => 'edit-doj', 'class' => array('numeric_positive form-control')),
		'#prefix' => '<div class="row"><div class="col-md-6"><div class="form-floating">',
		'#suffix' => '</div></div>',
		'#element_validate' => array('element_validate_date'),
	);
	
	$form['official_info']['dor'] = array(
		'#title' => t('Date of Retirement(According to Service Book)'),
		'#type' => 'textfield',
		'#attributes' => array('id' => 'edit-dor', 'class' => array('numeric_positive form-control')),
		'#default_value' => date('d/m/Y', strtotime($data->date_of_retirement)),
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
        '#default_value' => $data->office_name,
		'#default_value' => 'NA',
		'#prefix' => '<div class="row"><div class="col-md-4"><div class="form-floating">',
		'#suffix' => '</div></div>',
		'#element_validate' => array('element_validate_textarea'), 
		'#attributes' => array('oninput'=>"this.value=this.value.toUpperCase()", 'class' => array('form-control')),
	);
	
	$form['office_add']['office_street'] = array(
		'#title' => t('Address'),
		'#type' => 'textfield',
		//'#required' => TRUE,
		'#default_value' => $data->office_street,
		'#prefix' => '<div class="col-md-4"><div class="form-floating">',
		'#suffix' => '</div></div>',
		'#attributes' => array('oninput'=>"this.value=this.value.toUpperCase()",'class' => array('form-control') ),	
	);

	$form['office_add']['office_city'] = array(
		'#title' => t('City / Town / Village'),
		'#type' => 'textfield',
		'#default_value' => $data->office_city_town_village,
		'#prefix' => '<div class="col-md-4"><div class="form-floating">',
		'#suffix' => '</div></div></div>',
		'#element_validate' => array('element_validate_alphabatic_text'), 
		'#attributes' => array('oninput'=>"this.value=this.value.toUpperCase()", 'class' => array('form-control')),
	);

	$form['office_add']['office_post_office'] = array(
		'#title' => t('Post Office'),
		'#type' => 'textfield',
		'#default_value' => $data->office_post_office,
		'#prefix' => '<div class="row"><div class="col-md-3"><div class="form-floating">',
		'#suffix' => '</div></div>',
		'#element_validate' => array('element_validate_alphabatic_text'), 
		'#attributes' => array('oninput'=>"this.value=this.value.toUpperCase()", 'class' => array('form-control')),	
	);
	
	$form['office_add']['office_district'] = array(
		'#title' => t('District'),
		'#type' => 'select',
		'#options' => district_list(),
		'#default_value' => $data->office_district,
		'#prefix' => '<div class="col-md-3"><div class="form-floating">',
		'#suffix' => '</div></div>',
		'#attributes' => array('class' => array('form-select'))
	);

	$form['office_add']['office_pincode'] = array(
		'#title' => t('Pincode'),
		'#type' => 'textfield',
		'#maxlength' => 6,
		'#default_value' => $data->office_pin_code,
		'#element_validate' => array('element_validate_numeric_positive'), 
		'#prefix' => '<div class="col-md-3"><div class="form-floating">',
		'#suffix' => '</div></div>',
		'#attributes' => array('class' => array('numeric_positive form-control')),
	);

	$form['office_add']['office_phone_no'] = array(
		'#title' => t('Phone No.(With STD Code)'),
		'#type' => 'textfield',
		'#maxlength' => 15,
		'#default_value' => $data->office_phone_no,
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
		//'#required' => TRUE,
		//'#validated' => TRUE,
		'#prefix' => '<div class="row"><div class="col-md-6"><div class="form-floating">',
		'#suffix' => '</div></div>',
	);
	
	$district = isset($form_state['values']['district']) && !empty($form_state['values']['district']) ? $form_state['values']['district'] : $data->district_code;
	
	$form['ddo']['designation'] = array(
		'#title' => t('DDO Designation'),
		'#type' => 'select',
		//'#options' => array('- Select -'),
		'#default_value' => $data->ddo_id,
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
		//'#required' => TRUE,
		//'#validated' => TRUE,
		'#prefix' => '<div id="replace_designation" class="col-md-6"><div class="form-floating">',
		'#suffix' => '</div></div>',
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
        '#options' =>get_housing_estate(),
        //'#options' => $housing_estate_arr_1,
        '#default_value' => ($data->estate_id != '') ? $data->estate_id : $occupation_estate,
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
    );
    
    
    if($data->estate_id != '') {
        $form['current_occupation']['occupation_block'] = array(
            '#title' => t('Select Block'),
            '#type' => 'select',
            //'#options' => get_housing_block($occupation_estate),
            '#default_value' =>$data->block_id,
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
        
    if($data->estate_id != '' && $data->block_id != '') {
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
        '#default_value' => date('d/m/Y', strtotime($data->possession_date)),
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
        '#default_value' => $data->license_no,
        '#prefix' => '<div class="row"><div class="col-md-6"><div class="form-floating">',
        '#suffix' => '</div></div>',

        '#attributes' => array('oninput'=>"this.value=this.value.toUpperCase()", 'class'=>array('form-control')),
    );

	$form['current_occupation']['physical_application_no'] = array(
        '#title' => t('Physical Application Number'),
        '#type' => 'textfield',
        '#default_value' => $data->physical_application_no,
        //'#required' => TRUE,
        '#attributes' => array('class' => array('form-control ')),
        // '#element_validate' => array('element_validate_numeric_positive'),
        '#prefix' => '<div class="col-md-6"><div class="form-floating">',
        '#suffix' => '</div></div></div>',
    );
    
        
    $form['submit'] = array(
		'#prefix' =>'<div class="col-12 border-top pt-3">',
		'#suffix' => '</div><div class="mb-5 pb-3"></div>',
		'#value' => t('Update'),
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

function _reload_get_paydetails_info($form, &$form_state) {
  $commands = array();

  $commands[] = ajax_command_replace('#pay_type_new_replace_div', drupal_render($form['official_info']['pay_type_new']));
  $commands[] = ajax_command_replace('#pay_band_replace_div', drupal_render($form['official_info']['pay_band']));
  $commands[] = ajax_command_replace('#flat_type_display_div', drupal_render($form['official_info']['flat_type_display']));

  return array('#type' => 'ajax', '#commands' => $commands);
}

function legacy_vs_wohrms_edit_form_submit($form, &$form_state) {
	$val = $form_state['values'];

	$user_id = $val['hidden_uid'];
    $applicant_official_detail_id = $val['hidden_applicant_official_detail_id'];
    $online_application_id = $val['hidden_online_application_id'];
	$housing_applicant_id = $val['hidden_applicant_id'];

    // print_r($val);die;

    $personal_info_array = array(
        'applicant_name' => $val['applicant_name'],
        'guardian_name' => $val['applicant_father_name'],
        'date_of_birth' => trim(implode('-', array_reverse(explode('/', $val['dob'])))),
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
        'ddo_id' => empty($val['designation']) ? null : $val['designation'],
        'applicant_designation'=> $val['app_designation'],
        'applicant_headquarter' => $val['app_headquarter'],
        'applicant_posting_place' => $val['app_posting_place'],

        'pay_band_id' => $val['pay_band'],
        'pay_in_the_pay_band' => $val['pay_in'],
        'date_of_joining' => trim(implode('-', array_reverse(explode('/', $val['doj'])))),
        'date_of_retirement' => trim(implode('-', array_reverse(explode('/', $val['dor'])))),
        'office_name'=> $val['office_name'],
        'office_street'=> $val['office_street'],
        'office_city_town_village'=> $val['office_city'],
        'office_post_office'=> $val['office_post_office'],
        'office_pin_code'=> $val['office_pincode'],
        // 'office_district'=> $val['office_district'],
        'office_phone_no'=> $val['office_phone_no'],
    );
    // echo "<pre>";print_r($official_info_array);
    db_update('housing_applicant_official_detail')->fields($official_info_array)->condition('uid', $user_id)->condition('applicant_official_detail_id',$applicant_official_detail_id )->execute();

    $user_array = array(
        'mail' => $val['email'],
    );
    db_update('users')->fields($user_array)->condition('uid',$user_id)->execute();

	$online_application_array = array(
        'date_of_application' => trim(implode('-', array_reverse(explode('/', $val['application_date'])))),
		'physical_application_no' => $val['physical_application_no'],
    );
    db_update('housing_online_application')->fields($online_application_array)->condition('online_application_id',$online_application_id)->execute();

	$query = db_select('housing_pay_band_categories', 'hpbc');
	$query->fields('hpbc', array('flat_type_id'));
	$query->condition('hpbc.pay_band_id', $val['pay_band'], '=');
	$result = $query->execute();
	$data = $result->fetchObject();

	$vs_app_array = array(
        'flat_type_id'  =>  $data->flat_type_id,
		'license_no' => $val['license_no'],
		'occupation_estate'  =>   trim($val['occupation_estate']),
		'occupation_block'  =>   trim($val['occupation_block']),
		'occupation_flat'  =>   trim($val['occupation_flat']),
		'possession_date' => trim(implode('-', array_reverse(explode('/', $val['possession_date'])))),
    );
    db_update('housing_vs_application')->fields($vs_app_array)->condition('online_application_id',$online_application_id)->execute();
    drupal_set_message('Data has been updated successfully');
	drupal_goto('legacy-vs-list-wohrms');
}