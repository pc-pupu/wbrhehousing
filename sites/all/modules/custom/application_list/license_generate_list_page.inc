<?php

function license_generation_list() {
	global $base_path,$user,$user_role;
	// echo "<pre>";print_r($user);die;
	
	$output = '';
	$result = fetch_application_license_generation_list($user_role,$user->uid);
	// echo "<pre>";print_r($result);die;
	$rows =array();
	$header = array();
	$header[] = array('data' => 'Applicant Name');
	$header[] = array('data' => 'Application No.');
	$header[] = array('data' => 'Date of Application');
	$header[] = array('data' => 'License No.');
	$header[] = array('data' => 'Computer Serial No.');
	$header[] = array('data' => 'Flat Type');
	$header[] = array('data' => 'Estate Name');
	$header[] = array('data' => 'Block Name');
	$header[] = array('data' => 'Flat No.');
	

			
	 while($data = $result->fetchObject()) {
	 
			$fields = array();
			$fields[] = $data->applicant_name;
			$fields[] = $data->application_no;
			$fields[] = $data->date_of_application;
			$fields[] = $data->license_no;	
			$fields[] = $data->computer_serial_no;
			$fields[] = $data->flat_type;	
			$fields[] = $data->estate_name;
			$fields[] = $data->block_name;
			$fields[] = $data->flat_no;


			
			
			
			// $fields[] = l(
			// 		'<span class="btn bg-dark btn-sm fa fa-download text-white rounded" style="font-size:12px" ></span>' ,
			// 		'download_licence_pdf/'.encrypt_url($data->online_application_id).'/', //
			// 		array('html'=>TRUE,'attributes'=> array('target' => '_blank'))
			// 	);

		 $rows[] = $fields;
		  
	 }
	
	$variables = array(
			'attributes' => array('class'=>array('table table-list table-striped')),
			'header' => $header,
			'rows' => $rows,
			'sticky' => true,
			'empty' => t("No data found!") // The message to be displayed if table is empty
		  );
  		//end
		//sd turned off 21-06-2024
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
	
	return $output;
		
}
