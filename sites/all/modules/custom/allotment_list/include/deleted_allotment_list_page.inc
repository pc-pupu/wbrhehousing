<?php

function fetch_deleted_allotment_list($allotment_process_no = '', $allotment_process_type = ''){
	global $base_root, $base_path;
<<<<<<< HEAD
=======
	$allotment_process_no = decrypt_url($allotment_process_no);
	$allotment_process_type = decrypt_url($allotment_process_type);
>>>>>>> 2502ab393f49b61f25516e6bb0502acbb6d447a0
	$output = '';
		$query = db_select('housing_allotment_rejection_log', 'harl');
		$query->innerJoin('housing_online_application', 'hoa', 'hoa.online_application_id = harl.online_application_id');
		$query->innerJoin('housing_applicant_official_detail', 'haod', 'hoa.applicant_official_detail_id = haod.applicant_official_detail_id');
		$query->innerJoin('housing_applicant', 'ha', 'ha.housing_applicant_id = haod.housing_applicant_id');
		$query->innerJoin('housing_flat', 'hf', 'hf.flat_id = harl.allotted_flat_id');
		$query->innerJoin('housing_block', 'hb', 'hb.block_id = hf.block_id');
		$query->innerJoin('housing_estate', 'he', 'he.estate_id = hf.estate_id');
		$query->innerJoin('housing_flat_type', 'hft', 'hft.flat_type_id = hf.flat_type_id');
		
		$query->fields('ha',array('applicant_name','mobile_no'));
		$query->fields('harl',array());
		$query->fields('hf',array('flat_no','floor'));
		$query->fields('hb',array('block_name'));
		$query->fields('he',array('estate_name'));
		$query->fields('hft',array('flat_type'));
		
<<<<<<< HEAD
		//$query->condition('harl.allotment_process_no',$allotment_process_no);
		//$query->condition('harl.allotment_process_type',$allotment_process_type);
=======
		$query->condition('harl.allotment_process_no',$allotment_process_no);
		$query->condition('harl.allotment_process_type',$allotment_process_type);
>>>>>>> 2502ab393f49b61f25516e6bb0502acbb6d447a0

	


		$result = $query->execute();
		// echo $query;die;
		//$data = $result->fetchAll();
		// echo "<pre>";print_r($data);die;

		///////////////////debaleena 23-09-2024////////////////////////
<<<<<<< HEAD
		$header['sl_no'] = array('data' => 'Name');
=======
		// $header['sl_no'] = array('data' => 'Name');
>>>>>>> 2502ab393f49b61f25516e6bb0502acbb6d447a0
		$header['applicant_name'] = array('data' => 'Applicant Name');
		$header['application_no'] = array('data' => 'Application No.');
		$header['computer_serial_no'] = array('data' => 'Computer Serial No');
		$header['physical_application_no'] = array('data' => 'Physical Application No.');
		$header['hrms_id'] = array('data' => 'HRMS Id');
		$header['is_backlog_applicant'] = array('data' => 'Backlog Applicant');
		$header['allotment_process_no'] = array('data' => 'Allotment Process No.');
		$header['created_at'] = array('data' => 'Date of Deletion');
		$header['estate_name'] = array('data' => 'Alloted R.H.E');
		$header['block_name'] = array('data' => 'Alloted Block');
		$header['flat_no'] = array('data' => 'Alloted flat No.');
		$header['floor'] = array('data' => 'Alloted Floor');
		$header['flat_type'] = array('data' => 'Alloted Flat Type');
		
	
		$rows =array();
		$output = '';
		
		while($data = $result->fetchObject()) {
			$fields = array();
		
			$fields[] = $data->applicant_name;
			$fields[] = $data->application_no;
			$fields[] = $data->computer_serial_no;
			$fields[] = $data->physical_application_no;
			$fields[] = $data->hrms_id;
			$fields[] = $data->is_backlog_applicant;
			$fields[] = $data->allotment_process_no;
			$fields[] = $data->created_at;
			$fields[] = $data->estate_name;
			$fields[] = $data->block_name;
			$fields[] = $data->flat_no;
			$fields[] = $data->floor;
			$fields[] = $data->flat_type;

			
			
			$rows[] = $fields;
		}

			

			$variables = array(
<<<<<<< HEAD
				'attributes' => array('class'=>array('table table-list table-striped')),
				'header' => $header,
				'rows' => $rows,
				'sticky' => true,
				'empty' => t("No data found!") // The message to be displayed if table is empty
			);
			
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
=======
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
>>>>>>> 2502ab393f49b61f25516e6bb0502acbb6d447a0
			
	
}
	
	