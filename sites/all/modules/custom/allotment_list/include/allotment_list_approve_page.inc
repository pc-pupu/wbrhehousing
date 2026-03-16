<?php

	function allotment_list_approve_form($form, &$form_state) {
	
		$allotment_process_date =  isset($form_state['values']['allotment_process_date']) && !empty($form_state['values']['allotment_process_date']) ? $form_state['values']['allotment_process_date'] : '';
		
		$allotment_process_no =  isset($form_state['values']['allotment_process_no']) && !empty($form_state['values']['allotment_process_no']) ? $form_state['values']['allotment_process_no'] : '';
		
		$allotment_process_type =  isset($form_state['values']['allotment_process_type']) && !empty($form_state['values']['allotment_process_type']) ? $form_state['values']['allotment_process_type'] : '';
		

		
		$form['allotment_process_date'] = array(
			'#title' => t('Allotment Process Date'),
			'#type' => 'select',
			'#options' => allotment_process_date(),
			'#required' => TRUE,
			//'#attributes' => array('class' => array('dropdown3')),//sd 24-06-2024 turned off
			'#ajax' => array(
					   'event' => 'change',
					   'callback' => '_reload_allotment_process_no',
					   'method' => 'replace',
			),
			//'#prefix' => '<div class="three">',//sd 24-06-2024 turned off
			//'#suffix' => '</div>',//sd 24-06-2024 turned off
			'#attributes' => array('class' => array('form-select')),//sd 24-06-2024
			'#prefix' => '<div class="row"><div class="col-md-4"><div class="form-floating">',//sd 24-06-2024
			'#suffix' => '</div></div>',//sd 24-06-2024
			'#default_value' => $allotment_process_date,
	  	);
		
		
		$form['allotment_process_no'] = array(
			/*'#title' => t('Allotment Process No.'),
			'#type' => 'select',
			'#options' => array('' => '-- Select --'),
			'#required' => TRUE,
			'#attributes' => array('class' => array('dropdown3')),
			'#ajax' => array(
					   'event' => 'change',
					   'callback' => '_reload_allotment_process_type',
					   'method' => 'replace',
			),*/
			//'#prefix' => '<div class="three" id="allotment_process_no_replace">', //sd 24-06-2024 turned off
			//'#suffix' => '</div>', //sd 24-06-2024 turned off
			'#attributes' => array('class' => array('form-select')),//sd 24-06-2024 turned off
			'#prefix' => '<div class="col-md-4" id="allotment_process_no_replace"><div class="form-floating">',//sd 24-06-2024
			'#suffix' => '</div></div>',//sd 24-06-2024

			//'#default_value' => $allotment_process_no,
		);		

		
		if($allotment_process_date != '') {
			$form['allotment_process_no'] = array(
				'#title' => t('Allotment Process No.'),
				'#type' => 'select',
				'#options' => allotment_process_no($allotment_process_date),
				'#required' => TRUE,
				'#attributes' => array('class' => array('dropdown3')),
				'#ajax' => array(
						   'event' => 'change',
						   'callback' => '_reload_allotment_process_type',
						   'method' => 'replace',
				),
				//'#prefix' => '<div class="three" id="allotment_process_no_replace">', //sd 24-06-2024 turned off
				//'#suffix' => '</div>', //sd 24-06-2024 turned off
				'#attributes' => array('class' => array('form-select')),//sd 24-06-2024
				'#prefix' => '<div class="col-md-4" id="allotment_process_no_replace"><div class="form-floating">',//sd 24-06-2024
				'#suffix' => '</div></div>',//sd 24-06-2024

				'#default_value' => $allotment_process_no,
			);
		}
		
		$form['allotment_process_type'] = array(
			/*'#title' => t('Allotment Type'),
			'#type' => 'select',
			'#options' => array('' => '-- Select --'),
			'#required' => TRUE,
			'#attributes' => array('class' => array('dropdown3')),
			'#ajax' => array(
					   'event' => 'change',
					   'callback' => '_reload_allotment_list',
					   'method' => 'replace',
			),*/
			//'#prefix' => '<div class="three" id="allotment_process_type_replace">', //sd 24-06-2024  turned off
			//'#suffix' => '</div>', //sd 24-06-2024  turned off
			'#attributes' => array('class' => array('form-select')),//sd 24-06-2024
			'#prefix' => '<div class="col-md-4" id="allotment_process_type_replace"><div class="form-floating">',//sd 24-06-2024
			'#suffix' => '</div></div></div>',//sd 24-06-2024

			//'#default_value' => $allotment_process_type,
		);		

		
		if($allotment_process_date != '' && $allotment_process_no != '') {
			$form['allotment_process_type'] = array(
				'#title' => t('Allotment Process Type'),
				'#type' => 'select',
				'#options' => array('' => 'Select Allotment Process Type', 'NAL' => 'New Allotment', 'VSAL' => 'Floor Shifting', 'CSAL' => 'Category Shifting','MSR' => 'Manual Special Recomandation'),// added by Moumita on 06-08-2025
				'#required' => TRUE,
				//'#attributes' => array('class' => array('dropdown3')),//sd 24-06-2024 turned off
				'#ajax' => array(
						   'event' => 'change',
						   'callback' => '_reload_allottee_list',
						   'method' => 'replace',
				),
				'#attributes' => array('class' => array('form-select')),//sd 24-06-2024
				'#prefix' => '<div class="col-md-4" id="allotment_process_type_replace"><div class="form-floating">',//sd 24-06-2024
				'#suffix' => '</div></div></div>',//sd 24-06-2024
				'#default_value' => $allotment_process_type,
			);
		}
		
		$form['submit_approve'] = array(
			'#prefix' => '<div id="submit_approve_allotteelist" class="col-md-4 text-start" >',
			'#suffix' => '</div>',
  		);
		$form['submit_reject'] = array(
			'#prefix' => '<div id="submit_reject_allotteelist"  class="col-md-4 text-center">',
			'#suffix' => '</div>',
  		);
		$form['submit_hold'] = array(
			'#prefix' => '<div id="submit_hold_allotteelist"  class="col-md-4 text-end">',
			'#suffix' => '</div></div>',
  		);

		$form['allottee_deleted_info'] = array(
			'#type' => 'markup',
			'#markup' => '<div id="allottee_deleted_info"  class="col-md-4 text-end"></div>',				
		);

		$form['allottee_info'] = array(
			'#prefix' => '<div id="allottee_info">',
			'#suffix' => '</div>',
		);
		
		
		if($allotment_process_date != '' && $allotment_process_no != '' && $allotment_process_type != '') {			
			$result = fetch_all_allottee_details_for_approve($allotment_process_date, $allotment_process_no, $allotment_process_type);
			// echo $allotment_process_no.$allotment_process_type;die;
			if($allotment_process_type == 'MSR'){
				$allotment_process = 'MANUAL';
			}else{
				$allotment_process = 'ALOT';
			}
			// get allotment process status

			$query = db_select('housing_allotment_process','hap');
			$query->addField('hap', 'allotment_status');
			$query->condition('hap.allotment_process_no', $allotment_process_no, '=');
			$query->condition('hap.allotment_process_type', $allotment_process, '=');
			$result_1 = $query->execute();
			$process_data = $result_1->fetchObject();
			
			if($allotment_process_type == 'NAL') {
				$header = array(
					'sl_no' => array('data' => 'Sl. No.'),
					'name_desig' => array('data' => 'Name and Designation'),
					'app_no_posting_doa' => array('data' => '1. Application No.<br>2. Place of Posting<br>3. Date of Application'),
					'flat_allotted' => array('data' => 'Flat Allotted'),
					'floor' => array('data' => 'Floor'),
					'rhe_name' => array('data' => 'Name of R.H.E.'),
					'block_name' => array('data' => 'Name of Block'),  // added by dg on 14-11-2025
					'dor' => array('data' => 'Date of Retirement'),
					'remarks' => array('data' => 'Remarks'),
					'status' => array('data' => 'Status'),
				);
			} else if($allotment_process_type == 'VSAL' || $allotment_process_type == 'CSAL') {
				$header = array(
					'sl_no' => array('data' => 'Sl. No.'),
					'applicant_name' => array('data' => 'Name'),
					'dop_doa_dor' => array('data' => '1. Date of Possession<br>2. Date of Application<br>3. Date of Retirement'),
					'flat_allotted' => array('data' => 'Allotted Flat No.'),
					'flat_type' => array('data' => 'Type of Flat'),
					'floor' => array('data' => 'Floor'),
					'block_name' => array('data' => 'Name of Block'),  // added by dg on 14-11-2025
					'from_flat_no' => array('data' => 'From Flat No.'),
					'to_flat_no' => array('data' => 'To Flat No.'),
					'remarks' => array('data' => 'Remarks'),
					'status' => array('data' => 'Status'),
				);
			}else if($allotment_process_type == 'MSR') { // added by dg on 14-11-2025
				// For Manual Allotment
				$header = array(
					'sl_no' => array('data' => 'Sl. No.'),
					'name_desig' => array('data' => 'Name and Designation'),
					'app_no_posting_doa' => array('data' => '1. Application No.<br>2. Place of Posting<br>3. Date of Application'),
					'flat_allotted' => array('data' => 'Flat Allotted'),
					'floor' => array('data' => 'Floor'),
					'rhe_name' => array('data' => 'Name of R.H.E.'),
					'block_name' => array('data' => 'Name of Block'),
					'dor' => array('data' => 'Date of Retirement'),
					'remarks' => array('data' => 'Remarks'),
					'status' => array('data' => 'Status'),
				);
			}
			
			$i = 1;
			$onclick="return confirm('Are you sure you want to Approve?')";
			$onclick1="return confirm('Are you sure you want to Reject?')";
			$onclick2="return confirm('Are you sure you want to Hold?')";
			
			if($result->rowCount() > 0 ) {
				// Approve Button - Left
				$form['submit_approve'] = array(
				'#type' => 'submit',
				'#value' => t('Click to Approve Allotment'),
				'#attributes' => array(
					'onclick' => $onclick,
					'class' => array('btn', 'bg-success', 'btn-sm', 'px-5', 'rounded-pill', 'text-white', 'mb-4', 'fw-bolder', 'w-100'),
				),
				// '#prefix' => '<div class="col-md-4"><div id="submit_approve_allotteelist">',
				// '#suffix' => '</div></div>',
				'#prefix' => '<div id="submit_approve_allotteelist" class="col-md-4 text-start" >',
				'#suffix' => '</div>',
				);

				// Reject Button - Center
				$form['submit_reject'] = array(
				'#type' => 'submit',
				'#value' => t('Click to Reject Allotment'),
				'#attributes' => array(
					'onclick' => $onclick1,
					'class' => array('btn', 'bg-danger', 'btn-sm', 'px-5', 'rounded-pill', 'text-white', 'mb-4', 'fw-bolder', 'w-100'),
				),
				'#submit' => array('allottee_list_reject'),
				// '#prefix' => '<div class="col-md-4"><div id="submit_reject_allotteelist">',
				// '#suffix' => '</div></div>',
				'#prefix' => '<div id="submit_reject_allotteelist"  class="col-md-4 text-center">',
				'#suffix' => '</div>',
				);

				// Hold Button - Right
				$form['submit_hold'] = array(
				'#type' => 'submit',
				'#value' => t('Click to Hold Allotment'),
				'#attributes' => array(
					'onclick' => $onclick2,
					'class' => array('btn', 'bg-warning', 'btn-sm', 'px-5', 'rounded-pill', 'text-white', 'mb-4', 'fw-bolder', 'w-100'),
				),
				'#submit' => array('allottee_list_hold'),
				// '#prefix' => '<div class="col-md-4"><div id="submit_hold_allotteelist">',
				// '#suffix' => '</div></div>', // Closes the .row div
				'#prefix' => '<div id="submit_hold_allotteelist"  class="col-md-4 text-end">',
				'#suffix' => '</div></div>',
				);

				if(!empty($process_data)){
					
					if($process_data->allotment_status =='partial_deleted'){
						$delete_link = l(
							t('Click to View Partially Deleted Allottee Information'), 
							'allotment-deleted-info', 
							array('attributes' => array('class' => array('btn', 'bg-info', 'btn-sm', 'px-5', 'rounded-pill', 'text-white', 'mb-4', 'fw-bolder', 'w-100')))
						);

						$form['allottee_deleted_info'] = array(
							'#type' => 'markup',
							'#markup' => '<div id="allottee_deleted_info"  class="col-md-4 text-end">'.$delete_link.'</div>',				
						);
					}
					// else if($process_data->allotment_status =='full_deleted'){
					// 	$delete_link = l(
					// 		t('Click to View Fully Deleted Allottee Information'), 
					// 		'allotment-deleted-info', 
					// 		array('attributes' => array('class' => array('btn', 'bg-info', 'btn-sm', 'px-5', 'rounded-pill', 'text-white', 'mb-4', 'fw-bolder', 'w-100')))
					// 	);

					// 	$form['allottee_deleted_info'] = array(
					// 		'#type' => 'markup',
					// 		'#markup' => '<div id="allottee_deleted_info"  class="col-md-4 text-end">'.$delete_link.'</div>',				
					// 	);
					// }
					else{
						$form['allottee_deleted_info'] = array(
							'#type' => 'markup',
							'#markup' => '<div id="allottee_deleted_info" class="col-md-4 text-end"></div>',				
						);
					}
				}
				// added on 25-02-2026 by Moumita

			}else{
				// added on 25-02-2026 by Moumita
				if($allotment_process_date != '' && $allotment_process_no != '' && $allotment_process_type != '' && $process_data->allotment_status =='full_deleted'){
					$delete_link = l(
						t('Click to View Fully Deleted Allottee Information'), 
						'allotment-deleted-info', 
						array('attributes' => array('class' => array('btn', 'bg-info', 'btn-sm', 'px-5', 'rounded-pill', 'text-white', 'mb-4', 'fw-bolder', 'w-100')))
					);

					$form['allottee_deleted_info'] = array(
						'#type' => 'markup',
						'#markup' => '<div id="allottee_deleted_info"  class="col-md-4 text-end">'.$delete_link.'</div>',				
					);
				}
			}
			
			$options = array();
			
			foreach($result->fetchAll() as $record) {
				
				
				/*if(trim($record->floor) == 'F') {
					$floor = '1st';	
				} else if(trim($record->floor) == 'S') {
					$floor = '2nd';	
				} else {
					$floor = $record->floor;	
				}*/

				$floor = $record->floor; // sd 24-06-2024 
				
				if($record->allowed_for_floor_shifting != '' && $record->allowed_for_floor_shifting == 'N') {
					$allowed_for_fs = 'No Floor Shifting Henceforth';		
				} else {
					$allowed_for_fs = '';	
				}
				
				if($allotment_process_type == 'NAL') {
					if($record->roaster_vacancy_position != '') {
						if($record->roaster_vacancy_position == 1 || $record->roaster_vacancy_position == 21) {
							$remarks = $record->roaster_vacancy_position.'st Vacancy<br>'.$record->allotment_reason.'<br>'.$allowed_for_fs;	
						} else if($record->roaster_vacancy_position == 2 || $record->roaster_vacancy_position == 22) {
							$remarks = $record->roaster_vacancy_position.'nd Vacancy<br>'.$record->allotment_reason.'<br>'.$allowed_for_fs;	
						} else if($record->roaster_vacancy_position == 3 || $record->roaster_vacancy_position == 23) {
							$remarks = $record->roaster_vacancy_position.'rd Vacancy<br>'.$record->allotment_reason.'<br>'.$allowed_for_fs;	
						} else {
							$remarks = $record->roaster_vacancy_position.'th Vacancy<br>'.$record->allotment_reason.'<br>'.$allowed_for_fs;
						}
					} else {
						$remarks = $allowed_for_fs;		
					}
					
					$options[$record->online_application_id] = array(
						'sl_no' => $i,
						'name_desig' => $record->applicant_name.'<br>'.$record->applicant_designation,
						'app_no_posting_doa' => '1. '.$record->application_no.'<br>'.'2. '.$record->applicant_posting_place.'<br>'.'3. '.implode('/', array_reverse(explode('-', $record->date_of_application))),
						'flat_allotted' => $record->flat_no,
						'floor' => $floor,
						'rhe_name' => $record->estate_name,
						'block_name' => $record->block_name,  //added by dg 14-11-2025
						'dor' => implode('/', array_reverse(explode('-', $record->date_of_retirement))),
						'remarks' => $remarks,
						'status' => $record->status_description,  //added dg 23-05-2025
					);
					
				} else if($allotment_process_type == 'VSAL' || $allotment_process_type == 'CSAL') {
					if($allotment_process_type == 'VSAL') {
						$occupied_flat = $record->occupied_flat_vs;
						$remarks = 'Floor Shifting';	
					} else if($allotment_process_type == 'VSAL' || $allotment_process_type == 'CSAL') {
						$occupied_flat = $record->occupied_flat_cs;
						$remarks = 'Category Shifting';	
					}
					
					$options[$record->online_application_id] = array(
						'sl_no' => $i,
						'applicant_name' => $record->applicant_name,
						'dop_doa_dor' => '1. '.implode('/', array_reverse(explode('-', $record->possession_date))).'<br>'.'2. '.implode('/', array_reverse(explode('-', $record->date_of_application))).'<br>'.'3. '.implode('/', array_reverse(explode('-', $record->date_of_retirement))),
						'flat_allotted' => $record->flat_no,
						'flat_type' => $record->flat_type,
						'floor' => $floor,
						'block_name' => $record->block_name,  //added by dg 14-11-2025
						'from_flat_no' => $occupied_flat,
						'to_flat_no' => $record->flat_no,
						'remarks' => $remarks,
						'status' => $record->status_description,  //added dg 23-05-2025
					);	
					
				} else if($allotment_process_type == 'MSR'){ // added by dg on 14-11-2025
					$remarks = $allowed_for_fs;
					
					$options[$record->online_application_id] = array(
						'sl_no' => $i,
						'name_desig' => $record->applicant_name.'<br>'.$record->applicant_designation,
						'app_no_posting_doa' => '1. '.$record->application_no.'<br>'.'2. '.$record->applicant_posting_place.'<br>'.'3. '.implode('/', array_reverse(explode('-', $record->date_of_application))),
						'flat_allotted' => $record->flat_no,
						'floor' => $floor,
						'rhe_name' => $record->estate_name,
						'block_name' => $record->block_name,  //added by dg 14-11-2025
						'dor' => implode('/', array_reverse(explode('-', $record->date_of_retirement))),
						'remarks' => $remarks,
						'status' => $record->status_description,  //added dg 23-05-2025
					);
				}
				
				$i++;
			}
		
			$form['allottee_info'] = array(
				'#type' => 'tableselect',
				'#header' => $header,
				'#options' => $options,
				'#empty' => t('No allottee available.'),
				'#prefix' => '<div id="allottee_info">',
				'#suffix' => '</div>',
				'#attributes'=>array('class'=>array('table table-list')),//sd 24-06-2024
			); 	
			
	  	}
		
		return $form;
	}
	
	function allotment_list_approve_form_validate($form, &$form_state) {
		#krumo($form_state['values']['table']);
		
		$i = 0;
		$all_allottee = $form_state['values']['allottee_info'];
		foreach($all_allottee as $allottee_app_id => $selected_allottee) {
			if($selected_allottee != 0) {
				$i = 1;	
			}
		}
		if($i == 0) {
			form_set_error($selected_allottee, t('No allottees are selected for allotment approve or reject!!!'));	
		}
	}
	
	function allotment_list_approve_form_submit($form, &$form_state) {
		//$form_state['rebuild'] = TRUE;
		
		$all_allottee = $form_state['values']['allottee_info'];
		
	
		foreach($all_allottee as $allottee_app_id => $selected_allottee) {
			if($selected_allottee != 0) {
				db_update('housing_online_application')
				->fields(array(
					//'status' => 'allotted_approved',// turned off sd 10-09-2024
					'status' => 'housing_official_approved',// added sd 10-09-2024
					'date_of_verified' => date('Y-m-d H:i:s')  // added by debaleena 25-11-2024
					)
				)
				->condition('online_application_id', $allottee_app_id, '=')
				->execute();

				//added sd 10-09-2024
				global $user;
				$process_flow_arr = array(
					'online_application_id' => $allottee_app_id,
					'status_id' => 9, // status_id of housing_official_approved // Updated by Subham 22/11/2024
					'created_at' => date('Y-m-d H:i:s'),
					'uid' => $user->uid,
					'short_code' =>'housing_official_approved'
				);
				db_insert('housing_process_flow')->fields($process_flow_arr)->execute();

				//end
				
				db_update('housing_flat_occupant')
				->fields(array(
					'allotment_approve_or_reject_date' => date('Y-m-d'),
					)
				)
				->condition('online_application_id', $allottee_app_id, '=')
				->execute();
				
			//code for allottee email list array for email and sms send
				$query_sendmail = db_select('users', 'u');
				$query_sendmail->innerJoin('housing_applicant_official_detail', 'haod', 'u.uid = haod.uid');
				$query_sendmail->innerJoin('housing_online_application', 'hoa', 'haod.applicant_official_detail_id = hoa.applicant_official_detail_id');
				$query_sendmail->innerJoin('housing_applicant', 'ha', 'ha.housing_applicant_id = haod.housing_applicant_id');
				$query_sendmail->fields('u', array('mail'));
				$query_sendmail->addField('ha', 'applicant_name');
				$query_sendmail->addField('ha', 'mobile_no');
				$query_sendmail->condition('hoa.online_application_id', $allottee_app_id, '=');
				$result_sendmail = $query_sendmail->execute();
				$data_sendmail = $result_sendmail->fetchObject();
				
				$receiver_email_arr[] = $data_sendmail->mail;

				$to = $data_sendmail->mail;
				$name = 'RHE';

				//code for email send to approved allottee
				$receiver_email_list[] = $receiver_email_arr;
				$subject = 'Offer of Allotment';
				$message = '<html><body>Dear Applicant '.$data_sendmail->applicant_name.',<br><br>		
				Dear Applicant, your application for RHE has been approved and a flat has been allotted to you. Kindly visit the portal to view allotment details and complete further necessary formalities. -Dept. of Housing, GoWB
				<br><br>			
				Regards,<br>
				Housing Department<br>
				Government of West Bengal
				</html></body>';
				
				//custom_mail_function($to, $subject, $message, $name);

				$dest = $data_sendmail->mobile_no;
				$sms_message = 'Dear '.$data_sendmail->applicant_name.', your application for RHE has been approved and a flat has been allotted to you. Kindly visit the portal to view allotment details and complete further necessary formalities. -Dept. of Housing, GoWB';
				$template_id = '1107175508647688715';

				//custom_sms_function($dest, $sms_message, $template_id);
			}
		}
		
		//code for email send to approved allottee
		$receiver_email_list[] = $receiver_email_arr;
		$email_subject = 'Offer of Allotment';
		$message = '<html><body>Dear Applicant,<br><br>		
		A flat has been allotted to you. Please login to your account and accept the Offer of Allotment within 30 days from the date you receive this mail to avoid cancellation of the flat allotment.
		<br><br>			
		Regards,<br>
		Housing Department<br>
		Government of West Bengal
		</html></body>';
				
		//sendmail_allotment($receiver_email_list, $email_subject, $message); // turned of sd 10-09-2024
	
		drupal_set_message(t('Allotment of Allottees are Approved.'));
		
	}
	//updated allottee_list_reject by dg 24-02-2026
	function allottee_list_reject($form, &$form_state) {
		global $user;
		
		$all_allottee = $form_state['values']['allottee_info'];

		$total_allottee_count = count($all_allottee);
		$non_zero = array_filter($all_allottee);
		$deleted_id_count = count($non_zero);

		foreach($all_allottee as $allottee_app_id => $selected_allottee) {
			if($selected_allottee != 0) {
								

				$query = db_select('housing_applicant_official_detail','haod');
				$query->innerjoin('housing_online_application ', 'hoa', 'hoa.applicant_official_detail_id = haod.applicant_official_detail_id');
				$query->leftjoin('housing_flat_occupant ', 'hfo', 'hfo.online_application_id = hoa.online_application_id');
				$query->innerjoin('housing_allotment_process ', 'hop', 'hop.allotment_process_no = hfo.allotment_process_no');
				$query->Fields('haod', array('applicant_official_detail_id','hrms_id'));
				$query->Fields('hfo', array('flat_id','allotment_no'));
				$query->Fields('hoa', array('application_no','is_backlog_applicant','computer_serial_no','physical_application_no','online_application_id','created_datetime','date_of_verified'));
				$query->Fields('hop', array('allotment_process_no','allotment_date','allotment_process_type','allotment_flat_type','allotment_process_id'));
				$query->condition('hoa.online_application_id', $allottee_app_id, '=');
				$result = $query->execute();
				$data = $result->fetchObject();
				// echo "<pre>";print_r($data);die;
				$allotment_process_id = $data->allotment_process_id;
				
				$parts = explode('-', $data->application_no);
				$application_no_type = $parts[0];
				if($application_no_type == 'PA' || $data->is_backlog_applicant == 1){
					$date_of_approver_approved = $data->created_datetime;
					$fields = array(
					    'status' => 'housingapprover_approved_1',
						'date_of_verified' => $date_of_approver_approved,
						'created_datetime' => $date_of_approver_approved
					);
				}else if($application_no_type == 'NA' || $application_no_type == 'VS' || $application_no_type == 'CS'){
					$date_of_approver_approved = $data->date_of_verified;
					// get approver_approve date
					
					$query1 = db_select('housing_process_flow','hpf')
						->Fields('hpf',array('created_at','status_id','short_code'))
						->condition('hpf.online_application_id',$allottee_app_id)
						->condition('hpf.short_code','housingapprover_approved_1','=')
						->execute();

					$result1 = $query1->fetchObject();

					$fields = array(
					    'status' => 'housingapprover_approved_1',
						'date_of_verified' => $result1->created_at,
					);
				 }
				// echo '<pre>'; print_r($fields); die;

				db_update('housing_online_application')
				    ->fields($fields)
				    ->condition('online_application_id', $allottee_app_id)
				    ->execute();

				db_delete('housing_process_flow')
				  ->condition('online_application_id', $allottee_app_id, '=')
				  ->condition('short_code', 'allotted', '=')
				  ->execute();
				
				db_update('housing_flat')
					->fields(array('flat_status_id' => 1,))
					->condition('flat_id', $data->flat_id, '=')
					->execute();

				db_delete('housing_flat_occupant')
				  ->condition('online_application_id', $allottee_app_id, '=')
				  ->execute();

				$data_for_log = array(
					'online_application_id' => $allottee_app_id,
					'status' => 'allotted',
					'application_no' => $data->application_no,
					'is_backlog_applicant' => $data->is_backlog_applicant,
					'computer_serial_no' => $data->computer_serial_no,
					'physical_application_no' => $data->physical_application_no,
					'hrms_id' => $data->hrms_id,
					'done_by_uid' => $user->uid,
					'allotment_process_no' => $data->allotment_process_no,
					'allotment_process_type' => $data->allotment_process_type,
					'allotment_date' => $data->allotment_date,
					'allotment_flat_type' => $data->allotment_flat_type,
					'allotted_flat_id' => $data->flat_id,
					'allotment_no' => $data->allotment_no,
					'created_at' => date('Y-m-d H:i:s'),
					'updated_at' => date('Y-m-d H:i:s')
				);

				db_insert('housing_allotment_rejection_log')->fields($data_for_log)->execute();
				
			}
		}
		// update housing_allotment_process table

		if($total_allottee_count == $deleted_id_count){
			$allotment_status = 'full_deleted';
		}else if($total_allottee_count > $deleted_id_count){
			$allotment_status = 'partial_deleted';
		}else{
			$allotment_status = '';
		}

		db_update('housing_allotment_process')
		->fields(array('allottment_status' => $allotment_status))
		->condition('allotment_process_id', $allotment_process_id)
		->execute();

		drupal_set_message(t('Allotments are Cancelled.'));
	}


	/* dg 23-05-2025 */

	function allottee_list_hold($form, &$form_state){
		global $user;
		$all_allottee = $form_state['values']['allottee_info'];

		foreach($all_allottee as $allottee_app_id => $selected_allottee) {
			if($selected_allottee != 0) {
				db_update('housing_online_application')
				->fields(array(
					'status' => 'allotment_on_hold',
					'date_of_verified' => date('Y-m-d H:i:s')  
					)
				)
				->condition('online_application_id', $allottee_app_id, '=')
				->execute();

				$status_id = get_status_id('allotment_on_hold');  

				$process_flow_arr = array(
					'online_application_id' => $allottee_app_id,
					'status_id' => $status_id,
					'created_at' => date('Y-m-d H:i:s'),
					'uid' => $user->uid,
					'short_code' =>'allotment_on_hold'
				);
				
				db_insert('housing_process_flow')->fields($process_flow_arr)->execute();
				drupal_set_message('Aolltment Kept on Hold Successfully.');
				
			}
		}


	}


	/* end */
	
	function _reload_allotment_process_no($form, &$form_state) {
		$commands[] = ajax_command_replace('#allotment_process_no_replace', drupal_render($form['allotment_process_no']));
		$commands[] = ajax_command_replace('#allotment_process_type_replace', drupal_render($form['allotment_process_type']));
		$commands[] = ajax_command_replace("#submit_approve_allotteelist", drupal_render($form['submit_approve']));
		$commands[] = ajax_command_replace("#submit_reject_allotteelist", drupal_render($form['submit_reject']));
		$commands[] = ajax_command_replace("#submit_hold_allotteelist", drupal_render($form['submit_hold']));  //added by dg 12-06-2025
		$commands[] = ajax_command_replace("#allottee_info", drupal_render($form['allottee_info']));

		$commands[] = ajax_command_replace("#allottee_deleted_info", drupal_render($form['allottee_deleted_info'])); // added on 25-02-2026 by Moumita

		return array('#type' => 'ajax', '#commands' => $commands);
 
	}
	
	function _reload_allotment_process_type($form, &$form_state) {
		$commands[] = ajax_command_replace('#allotment_process_type_replace', drupal_render($form['allotment_process_type']));
		$commands[] = ajax_command_replace("#submit_approve_allotteelist", drupal_render($form['submit_approve']));
		$commands[] = ajax_command_replace("#submit_reject_allotteelist", drupal_render($form['submit_reject']));
		$commands[] = ajax_command_replace("#submit_hold_allotteelist", drupal_render($form['submit_hold']));  //added by dg 12-06-2025
		$commands[] = ajax_command_replace("#allottee_info", drupal_render($form['allottee_info']));

		$commands[] = ajax_command_replace("#allottee_deleted_info", drupal_render($form['allottee_deleted_info'])); // added on 25-02-2026 by Moumita

		return array('#type' => 'ajax', '#commands' => $commands);
 
	}
	
	function _reload_allottee_list($form, &$form_state) {
		$commands = array();
		$commands[] = ajax_command_replace("#submit_approve_allotteelist", drupal_render($form['submit_approve']));
		$commands[] = ajax_command_replace("#submit_reject_allotteelist", drupal_render($form['submit_reject']));
		$commands[] = ajax_command_replace("#submit_hold_allotteelist", drupal_render($form['submit_hold']));  //added by dg 12-06-2025
		$commands[] = ajax_command_replace("#allottee_info", drupal_render($form['allottee_info']));

		$commands[] = ajax_command_replace("#allottee_deleted_info", drupal_render($form['allottee_deleted_info'])); // added on 25-02-2026 by Moumita

		return array('#type' => 'ajax', '#commands' => $commands);
  
	}