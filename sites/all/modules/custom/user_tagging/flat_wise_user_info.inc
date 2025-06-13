<?php

function flat_wise_user_info(){
    // list all request for user tagging

    $query = db_select('housing_user_tagging', 'hut');
    $query->fields('hut',array());
    
    $query->innerJoin('housing_flat', 'hf', 'hf.flat_id = hut.flat_id');
    $query->fields('hf',array('floor','flat_no','flat_id'));

	$query->innerJoin('housing_estate', 'he', 'he.estate_id = hf.estate_id');
    $query->fields('he',array('estate_name'));

	$query->innerJoin('housing_block', 'hb', 'hb.block_id = hf.block_id');
    $query->fields('hb',array('block_name'));


    $query->innerJoin('housing_flat_type', 'hft', 'hft.flat_type_id = hf.flat_type_id');
    $query->fields('hft',array('flat_type'));
    
    $inarr = array('new','pending');
    
    $query->condition('hut.flag',$inarr, 'in');
	
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
            'email' => array('data' => 'Email'),
            'mobile_no' => array('data' => 'Mobile No'),
			'identification_no' => array('data' => 'identification_no'),

            'details' => array('data' => 'Details'),
        );
        
		$rows =array();
		$output = '';
		$serialNumber = 1;
		
		while($data = $result->fetchObject()) {
            // print_r($data);die;
			$fields = array();
      
			$fields[] = $serialNumber;
			$fields[] = $data->estate_name;
			$fields[] = 'Block - '.$data->block_name.', Flat Type - '.$data->flat_type.', Floor - '.$data->floor.', Flat No. - '.$data->flat_no;

			
			$fields[] = $data->hrms_id;
            $fields[] = $data->applicant_name;
			$fields[] = $data->email;
			$fields[] = $data->mobile_no;
			$fields[] = $data->identification_no;
			
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

	// get user info by flat_id from draft table

	$query_draft = db_select('housing_existing_occupant_draft','heod')->fields('heod',array())->condition('heod.flat_id', $flat_id)->execute();
	$result_draft = $query_draft->fetchObject();
	// print_r($result_draft); die;

	$flat_info = get_flat_info_by_id(encrypt_url($flat_id));

	$display_output1 = '
		    <div class="table-bottom">
		        <table class="table table-list">
					<tr>
						<th style="border: 1px solid; border-color: white; background: none repeat scroll 0 0 #473a39;color:white;text-align: center;font-size: 18px;line-height: 24px; font-weight: normal;font-family: "Dosis",Arial,Verdana,serif;" class="first">Parameters</th>
						<th style="border: 1px solid; border-color: white; background: none repeat scroll 0 0 #473a39;color:white;text-align: center;font-size: 18px;line-height: 24px; font-weight: normal;font-family: "Dosis",Arial,Verdana,serif;" class="first">Applicant Information</th>
						<th style="border: 1px solid; border-color: white; background: none repeat scroll 0 0 #473a39;color:white;text-align: center;font-size: 18px;line-height: 24px; font-weight: normal;font-family: "Dosis",Arial,Verdana,serif;" class="first">Departmental Information</th>
					</tr>
					<tr>
						<td>Applicant Name </td>
						<td>'.$result_tag->applicant_name.'</td> 
						<td>'.$result_draft->applicant_name.'</td> 
					</tr>
					<tr>
						<td>HRMS </td>
						<td>'.$result_tag->hrms_id.'</td> 
						<td>'.$result_draft->hrms_id.'</td> 
					</tr>
					<tr>
						<td>Contact Number </td>
						<td>'.$result_tag->mobile_no.'</td> 
						<td>'.$result_draft->mobile_no.'</td> 
					</tr>
					<tr>
						<td>Date of Birth </td>
						<td>Nite hobe</td> 
						<td>'.$result_draft->date_of_birth.'</td> 
					</tr>
		           <tr>
						<td>Flat Information</td>
						<td>'.$flat_info.'</td> 
						<td>'.$flat_info.'</td> 
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
						<div class="card-header bg-class">
							Action Taken
						</div>
						<div class="card-body">'
	);

	$form['action'] = array(
		'#prefix' => '<div class="col-md-6 mx-auto">',
		'#suffix' => '</div>',
		'#title' => t('Action'),
		'#type' => 'select',
		'#options' => array(''=>'-Select-','tagged'=>'Approved','reject'=>'Reject','pending'=>'Pending'),
		'#attributes' => array('class' => array('form-control'))
	);
	
	$form['remarks'] = array(
		'#prefix' => '<div class="col-md-6 mx-auto">',
		'#suffix' => '</div>',
		'#title' => t('Remarks'),
		'#type' => 'textarea',
		'#attributes' => array('class' => array('form-control'))
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
	if($action_type=='tagged'){
		// set uid in housing_applicant, housing_applicant_official_details, update tagging table with remarks
		// housing_applicant
		$housingAppliArr = array(
			'uid' => $user->uid,
			'applicant_name' => '',
			'date_of_birth' => '',
			'mobile_no' => '',
		);
		$housing_applicant_id = db_insert('housing_applicant')->fields($housingAppliArr)->execute();
		// housing_applicant_official_details
		$housingAppliOffic = array(
			'uid' => $user->uid,
			'housing_applicant_id' => $housing_applicant_id
		);
		$applicant_official_detail_id = db_insert('housing_applicant_official_detail')->fields($housingAppliArr)->execute();

		// housing_online_application
		$housing_online_applicationArr = array(
			'applicant_official_detail_id' => $applicant_official_detail_id,
			'status' => '',
			'application_no' => 'EO'
		);
		$online_application_id = db_insert('housing_online_application')->fields($housing_online_applicationArr)->execute();

		// housing_flat_occupant
		$housing_flat_occupantArr = array(
			'online_application_id' => $online_application_id,
			'flat_id' => $val['flat_id_hidden'],
			'housing_existing_flat_occupant_id' => $user->uid
		);
		db_insert('housing_flat_occupant')->fields($housing_flat_occupantArr)->execute();

		// update housing_flat by flat_id 
		db_update('housing_flat')->fields(array('flat_status_id' => 2))->condition('flat_id', $val['flat_id_hidden'])->execute();
		
		// update housing_user_tagging by flat_id 
		db_update('housing_flat')->fields(array('flag' => 'tagged'))->condition('housing_user_tagging_id', $val['housing_user_tagging_id_hidden'])->execute();

	}elseif($action_type=='pending'){
		// update housing_user_tagging by flat_id 
		db_update('housing_flat')->fields(array('flag' => 'pending'))->condition('housing_user_tagging_id', $val['housing_user_tagging_id_hidden'])->execute();

	}elseif($action_type=='reject'){
		// update housing_user_tagging by flat_id 
		db_update('housing_flat')->fields(array('flag' => 'reject', 'status' => 0))->condition('housing_user_tagging_id', $val['housing_user_tagging_id_hidden'])->execute();
	}
}