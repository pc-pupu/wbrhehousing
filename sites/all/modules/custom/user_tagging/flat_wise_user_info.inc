<?php

function flat_wise_user_info($status =''){
    // list all request for user tagging
	// echo $status;
    
	$query = db_select('housing_user_tagging', 'hut');
    $query->fields('hut',array());

	$query->innerJoin('housing_existing_occupant_draft','heod','heod.flat_id = hut.flat_id');
    $query->fields('heod',array());

    $query->innerJoin('housing_flat', 'hf', 'hf.flat_id = hut.flat_id');
    $query->fields('hf',array('floor','flat_no','flat_id'));

	$query->innerJoin('housing_estate', 'he', 'he.estate_id = hf.estate_id');
    $query->fields('he',array('estate_name'));

	$query->innerJoin('housing_block', 'hb', 'hb.block_id = hf.block_id');
    $query->fields('hb',array('block_name'));


    $query->innerJoin('housing_flat_type', 'hft', 'hft.flat_type_id = hf.flat_type_id');
    $query->fields('hft',array('flat_type'));
    
    $inarr = array('new','pending');
    
    // $query->condition('hut.flag',$inarr, 'in');
	// $query->condition('hf.flat_status_id',2); // off on 04-11-2025 by Moumita
	
    $result = $query->execute();


    // echo $query; die;

	$output = '';

    if(!empty($result)){

        $header = array(
			'slno' => array('data' => 'Sl. No.','width'=>'7%'),
			'estate_id' => array('data' => 'Estate Name'),
			'flat_id' => array('data' => 'Flat Info'),

			'hrms_id' => array('data' => 'HRMS ID'),
            'applicant_name' => array('data' => 'Occupant Name'),

			'status' => array('data'=>'Status'),
            'remarks' => array('data'=>'Remarks'),

            'details' => array('data' => 'Details'),
        );
        
		$rows =array();
		$output = '';
		$serialNumber = 1;

		
		while($data = $result->fetchObject()) {
            // print_r($data);die;
			if($data->flag == 'pending'){
				$flag = '<span style="color: #E97451;">'.ucwords($data->flag).'</span>';
			}else{
				$flag = '<span style="color: #228B22;">'.ucwords($data->flag).'</span>';
			}

			$fields = array();
      
			$fields[] = $serialNumber;
			$fields[] = $data->estate_name;
			$fields[] = 'Block - '.$data->block_name.', Flat Type - '.$data->flat_type.', Floor - '.$data->floor.', Flat No. - '.$data->flat_no;

			
			$fields[] = $data->hrms_id;
            $fields[] = $data->applicant_name;

			$fields[] = $flag;
			$fields[] = $data->remarks;
			$fields[] = l('Details','flat-wise-user-info-details/'.encrypt_url($data->flat_id),array('html'=>TRUE,'attributes'=> array('class'=>array('btn bg-success btn-sm px-4 rounded-pill text-white fw-bolder'))));

			$serialNumber++;

			$rows[] = $fields;
			
		}

		$variables = array(
			'attributes' => array('class'=>array('table table-list table-striped')),
			'header' => $header,
			'rows' => $rows,
			'sticky' => true,
			'empty' => t("No data found!")
		  );
  		//end

		if(count($rows) > 0) {
			// Render using Drupal's render API.
			$build['datatable'] = array(
			  '#theme' => 'datatable',
			  '#header' => $header,
			  '#rows' => $rows,
			  '#attributes' => array(),
			);
			
			// Or, render using a theme function.
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
    }
	
	return $output;

}

function flat_wise_user_details($form, $form_state, $en_id = 0){
    $flat_id = decrypt_url($en_id); 

	// get user info by flat_id from tagging table

	$query_tag = db_select('housing_user_tagging','hut')->fields('hut',array())->condition('hut.flat_id', $flat_id)->execute();
	$result_tag = $query_tag->fetchObject();
	// print_r($result_tag);

	$license_no_tag = ($result_tag->license_no !='') ? $result_tag->license_no : 'Not Available';


	// get user info by flat_id from draft table

	$query_draft = db_select('housing_existing_occupant_draft','heod')->fields('heod',array())->condition('heod.flat_id', $flat_id)->execute();
	$result_draft = $query_draft->fetchObject();
	// print_r($result_draft); die;

	$license_no_draft = ($result_draft->license_no !='') ? $result_draft->license_no : 'Not Available';

	$flat_info = get_flat_info_by_id(encrypt_url($flat_id));


	$tag_license_issue_date = ($result_tag->license_issue_date !='') ? date('d/m/Y',strtotime($result_tag->license_issue_date)) : 'Not Available';

	$draft_license_issue_date = ($result_draft->license_issue_date !='') ? date('d/m/Y',strtotime($result_draft->license_issue_date)) : 'Not Available';


	$tag_license_expiry_date = ($result_tag->license_expiry_date !='') ? date('d/m/Y',strtotime($result_tag->license_expiry_date)) : 'Not Available';

	$draft_license_expiry_date = ($result_draft->license_expiry_date !='') ? date('d/m/Y',strtotime($result_draft->license_expiry_date)) : 'Not Available';

	$draft_last_license_renewal_date = ($result_draft->last_license_renewal_date !='') ? date('d/m/Y',strtotime($result_draft->last_license_renewal_date)) : 'Not Available';

	$draft_hrms_id = !empty($result_draft->hrms_id) ? $result_draft->hrms_id : 'Not Available';  //added by dg 20-08-2025
	$authoried_or_not = !empty($result_draft->authorised_or_not) ? ucwords($result_draft->authorised_or_not) : 'Not Available'; //added by dg 20-08-2025
	$draft_ddo_id = (!empty($result_draft->ddo_id) && $result_draft->ddo_id != '') ? $result_draft->ddo_id : 1263;  //added by dg 29-08-2025
	$draft_pay_band_id = (!empty($result_draft->pay_band_id) && $result_draft->pay_band_id != '') ? $result_draft->pay_band_id : 10;  //added by dg 29-08-2025

	$display_output1 = '
		    <div class="table-bottom">
		        <table class="table table-list">
					<tr>
						<th style="border: 1px solid; border-color: white; background: none repeat scroll 0 0 #0a8bd6dc;color:white;text-align: center;font-size: 18px;line-height: 24px; font-weight: normal;font-family: "Dosis",Arial,Verdana,serif;" class="first">Parameters</th>
						<th style="border: 1px solid; border-color: white; background: none repeat scroll 0 0 #0a8bd6dc;color:white;text-align: center;font-size: 18px;line-height: 24px; font-weight: normal;font-family: "Dosis",Arial,Verdana,serif;" class="first">Applicant Information</th>
						<th style="border: 1px solid; border-color: white; background: none repeat scroll 0 0 #0a8bd6dc;color:white;text-align: center;font-size: 18px;line-height: 24px; font-weight: normal;font-family: "Dosis",Arial,Verdana,serif;" class="first">Departmental Information</th>
					</tr>
					<tr>
						<td>Flat Information</td>
						<td colspan="2" style="text-align: center;">'.$flat_info.'</td>
					</tr>
					<tr>
						<td>Applicant Name </td>
						<td align="center">'.$result_tag->applicant_name.'</td> 
						<td align="center">'.$result_draft->applicant_name.'</td> 
					</tr>
					<tr>
						<td>HRMS ID </td>
						<td align="center">'.$result_tag->hrms_id.'</td> 
						<td align="center">'.$result_draft->hrms_id.'</td> 
					</tr>
					<tr>
						<td>Contact Number </td>
						<td align="center">'.$result_tag->mobile_no.'</td> 
						<td align="center">'.$result_draft->mobile_no.'</td> 
					</tr>
					<tr>
						<td>License Information</td>
						<td align="center">'.$license_no_tag.'</td> 
						<td align="center">'.$license_no_draft.'</td> 
					</tr>
					<tr>
						<td>License Issue Date</td>
						<td align="center">'.$tag_license_issue_date.'</td> 
						<td align="center">'.$draft_license_issue_date.'</td> 
					</tr>
					<tr>
						<td>License Expiry Date</td>
						<td align="center">'.$tag_license_expiry_date.'</td> 
						<td align="center">'.$draft_license_expiry_date.'</td> 
					</tr>
					<tr>
						<td>Last License Renewal Date</td>
						<td align="center">---</td> 
						<td align="center">'.$draft_last_license_renewal_date.'</td> 
					</tr>
					<tr>
						<td>Authorised / Unauthorised </td>
						<td align="center">---</td> 
						<td align="center">'.ucwords($result_draft->authorised_or_not).'</td> 
					</tr>		          
		        </table>
		    </div>';

    $form['output1'] = array(
		'#type' => 'markup',
		'#markup' => $display_output1
	);

	$form['action_part_header'] = array(
		'#type' => 'markup',
		'#markup' => '<div class="card">
						<div class="card-header" style="background-color: #0a8bd6dc; color: white;">
							Action Taken
						</div>
						<div class="card-body">'
	);


	$info_array = array(
		'name' => $result_tag->applicant_name,
		'dob' => !empty($result_tag->dob) ? $result_tag->dob : '',  //added by dg 20-08-2025
		'mobile' => $result_tag->mobile_no,
		'hrmsid' => $result_tag->hrms_id,
		'user_id' => $result_tag->uid,
		'housing_existing_occupant_draft_id' => $result_draft->housing_existing_occupant_draft_id,
		'license_no' => ($result_draft->license_no !='') ? $result_draft->license_no : $result_tag->license_no,
		'license_issue_date' => ($result_draft->license_issue_date !='') ? $result_draft->license_issue_date : $result_tag->license_issue_date,
		'license_expiry_date' => ($result_draft->license_expiry_date !='') ? $result_draft->license_expiry_date : $result_tag->license_expiry_date,
		'authorised_or_not' => $result_draft->authorised_or_not,
		'draft_ddo_id' => $draft_ddo_id,
		'draft_pay_band_id' => $draft_pay_band_id
		
	);

	$form['form_info_array'] = array(
		'#type' => 'hidden',
		'#value' => serialize($info_array)
	);

	$form['action'] = array(
		'#prefix' => '<div class="col-md-6 mx-auto">',
		'#suffix' => '</div>',
		'#title' => t('Action'),
		'#type' => 'select',
		'#options' => array(''=>'-Select-','tagged'=>'Approved','reject'=>'Reject','pending'=>'Pending'),
		'#attributes' => array('class' => array('form-control')),
		'#required' => true
	);
	
	$form['remarks'] = array(
		'#prefix' => '<div class="col-md-6 mx-auto">',
		'#suffix' => '</div>',
		'#title' => t('Remarks'),
		'#type' => 'textarea',
		'#attributes' => array('class' => array('form-control')),
		'#required' => true
	);

	$form['flat_id_hidden'] = array(
		'#type' => 'hidden',
		'#value' => $flat_id
	);
	
	$form['housing_user_tagging_id_hidden'] = array(
		'#type' => 'hidden',
		'#value' => $result_tag->housing_user_tagging_id
	);

	$form['submitbtn'] = array(
		'#prefix' => '<div class="col-md-6 mx-auto">',
		'#suffix' => '</div>',
		'#type' => 'submit',
		'#value' => t('Save'),
		'#attributes' => array('class' => array('btn btn-secondary pull-right'))
	);

	$form['action_part_footer'] = array(
		'#type' => 'markup',
		'#markup' => '</div></div>'
	);
	
	return $form;

}

function flat_wise_user_details_submit($form, $form_state){
	global $user;
	$val = $form_state['values'];

	$action_type = $val['action'];
	$existing_flat_id_hidden = $val['flat_id_hidden'];

	$form_info_array = unserialize($val['form_info_array']);

	// print_r($form_info_array); die;

	if($action_type=='tagged'){

		//dg code//
		$query = db_select('housing_online_application', 'hoa')->fields('hoa', ['application_no']);
		$query->innerJoin('housing_applicant_official_detail', 'haod', 'haod.applicant_official_detail_id = hoa.applicant_official_detail_id');
		$query->leftJoin('housing_vs_application', 'hva', 'hva.online_application_id = hoa.online_application_id');
		$query->leftJoin('housing_cs_application', 'hca', 'hca.online_application_id = hoa.online_application_id');

		$query->condition(db_or()
				->condition('hva.occupation_flat', $existing_flat_id_hidden)
				->condition('hca.occupation_flat', $existing_flat_id_hidden)
		);
		$query->condition('hoa.status','housingapprover_approved_1', '=');
		$query->condition('haod.hrms_id', NULL, 'IS NOT NULL');
		$query->fields('haod',array('uid','hrms_id','applicant_official_detail_id'));
		$result = $query->execute()->fetchObject();

		$tagged_hrms_id = $form_info_array['hrmsid'];
		$tagged_uid = $form_info_array['user_id'];
		$vs_cs_uid = $result->uid;
		// echo "<pre>";print_r($result); die;
		// if(!empty($result) && $result->hrms_id == 0){
			
		// 	$application_no = $result->application_no;
		// 	list($application_type) = explode("-", $application_no, 2);
		// 	if($application_type =='VS' || $application_type == 'CS'){
		// 		db_update('users')->fields(array('status' => 0))
		// 				->condition('uid', $vs_cs_uid, '=')
		// 				->execute();

		// 		db_update('housing_applicant_official_detail')->fields(array('uid' => $tagged_uid, 'hrms_id' => $tagged_hrms_id,'is_active' => 1 ))
		// 				->condition('applicant_official_detail_id', $result->applicant_official_detail_id, '=')
		// 				->execute();

				

		// 	}

		// }
		// echo "<pre>";print_r($result); die;

		//end //
		//start moumita di //
		// set uid in housing_applicant, housing_applicant_official_details, update tagging table with remarks
		// housing_applicant
		$housingAppliArr = array(

			'uid' => $form_info_array['user_id'],
			'applicant_name' => $form_info_array['name'],
			'mobile_no' => $form_info_array['mobile'],

		);
		// check for duplicate entry
		$r1 = db_query("select 1 from housing_applicant where uid=:uid and mobile_no=:mobile_no",array(':uid'=>$form_info_array['user_id'],':mobile_no'=>$form_info_array['mobile']))->fetchObject();
		if(empty($r1)){
			$housing_applicant_id = db_insert('housing_applicant')->fields($housingAppliArr)->execute();
		}
				
		// housing_applicant_official_details
		$housingAppliOffic = array(
			'uid' => $form_info_array['user_id'],
			'housing_applicant_id' => $housing_applicant_id,
			'hrms_id' => $form_info_array['hrmsid'],
			'ddo_id' => $form_info_array['draft_ddo_id'],
			'pay_band_id' => $form_info_array['draft_pay_band_id']
		);
		// check for duplicate entry
		$r2 = db_query("select 1 from housing_applicant_official_detail where uid=:uid and hrms_id=:hrms_id",array(':uid'=>$form_info_array['user_id'],':hrms_id'=>$form_info_array['hrmsid']))->fetchObject();
		if(empty($r2)){
			$applicant_official_detail_id = db_insert('housing_applicant_official_detail')->fields($housingAppliOffic)->execute();
		}
		

		if(!empty($result) && $result->hrms_id == 0){
			
			$application_no = $result->application_no;
			list($application_type) = explode("-", $application_no, 2);
			if($application_type =='VS' || $application_type == 'CS'){
				db_update('users')->fields(array('status' => 0))
						->condition('uid', $vs_cs_uid, '=')
						->execute();

				db_update('housing_applicant_official_detail')->fields(array('uid' => $tagged_uid, 'hrms_id' => $tagged_hrms_id,'is_active' => 1 ))
						->condition('applicant_official_detail_id', $result->applicant_official_detail_id, '=')
						->execute();

				

			}

		}





		// housing_online_application
		$housing_online_applicationArr = array(
			'applicant_official_detail_id' => $applicant_official_detail_id,
			'status' => 'existing_occupant',
			'is_backlog_applicant' => 2
		);
		// check for duplicate entry
		$r3 = db_query("select 1 from housing_online_application where applicant_official_detail_id=:applicant_official_detail_id",array(':applicant_official_detail_id'=>$applicant_official_detail_id))->fetchObject();
		if(empty($r3)){
			$online_application_id = db_insert('housing_online_application')->fields($housing_online_applicationArr)->execute();

			// update for application_no generation
			db_update('housing_online_application')->fields(array('application_no' => 'EO-'.trim(date('dmY')).'-'.$online_application_id))->condition('online_application_id', $online_application_id)->execute();
			
			// housing_flat_occupant
			$housing_flat_occupantArr = array(
				'online_application_id' => $online_application_id,
				'flat_id' => $val['flat_id_hidden'],
				'allotment_date' => NULL, //added by dg 28-08-2025
			);
			// check for duplicate entry
			$r4 = db_query("select 1 from housing_flat_occupant where online_application_id=:online_application_id and flat_id=:flat_id",array(':online_application_id'=>$online_application_id,':flat_id'=>$val['flat_id_hidden']))->fetchObject();
			if(empty($r4)){
				$flat_occupant_id = db_insert('housing_flat_occupant')->fields($housing_flat_occupantArr)->execute();
			}

			// update housing_flat by flat_id 
			db_update('housing_flat')->fields(array('flat_status_id' => 2))->condition('flat_id', $val['flat_id_hidden'])->execute();
			
			// update housing_user_tagging by flat_id 
			db_update('housing_user_tagging')->fields(array('flag' => 'tagged','housing_existing_occupant_draft_id'=>$form_info_array['housing_existing_occupant_draft_id'],'remarks'=>$val['remarks']))->condition('housing_user_tagging_id', $val['housing_user_tagging_id_hidden'])->execute();

			// add housing_occupant_license
			$housing_occupant_license_arr = array(
				'flat_occupant_id' => $flat_occupant_id,
				'license_no' => $form_info_array['license_no'],
				'license_issue_date' => $form_info_array['license_issue_date'],
				'license_expiry_date' => $form_info_array['license_expiry_date'],
				'authorised_or_not' => $form_info_array['authorised_or_not']
			);
			db_insert('housing_occupant_license')->fields($housing_occupant_license_arr)->execute();

			// housing_new_allotment_application
			$housing_new_allotment_application_arr = array(
				'online_application_id' => $online_application_id
			);
			//db_insert('housing_new_allotment_application')->fields($housing_new_allotment_application_arr)->execute();


			if(empty($result)){ ///adde by dg 27-08-2025
			
				db_insert('housing_new_allotment_application')->fields($housing_new_allotment_application_arr)->execute();
			}	

			if(!empty($result)){  //adde by dg 27-08-2025
				db_update('housing_applicant_official_detail')->fields(array('is_active' => 0))->condition('applicant_official_detail_id', $applicant_official_detail_id)->execute();

			}
			
			drupal_set_message('User Successfully Tagged with Flat');
			drupal_goto('flat-wise-user-info');
		}else{
			drupal_set_message('User Already Tagged, Please Check Once Again','error');
		}		

	}elseif($action_type=='pending'){
		// update housing_user_tagging by flat_id 
		db_update('housing_user_tagging')->fields(array('flag' => 'pending','remarks'=>$val['remarks']))->condition('housing_user_tagging_id', $val['housing_user_tagging_id_hidden'])->execute();
		drupal_set_message('User Tagging is Awaiting Further Verification');
		drupal_goto('flat-wise-user-info');

	}elseif($action_type=='reject'){
		// update housing_user_tagging by flat_id 
		db_update('housing_user_tagging')->fields(array('flag' => 'reject','remarks'=>$val['remarks'], 'status' => 0))->condition('housing_user_tagging_id', $val['housing_user_tagging_id_hidden'])->execute();
		drupal_set_message('User Tagging is Rejected');
		drupal_goto('flat-wise-user-info');
	}
}

function tagged_user_list($status = ''){
	echo $status; die;
}