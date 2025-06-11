<?php

function flat_wise_user_info(){
    // list all request for user tagging

    $query = db_select('housing_user_tagging', 'hut');
    $query->fields('hut',array());
    
    $query->innerJoin('housing_estate', 'he', 'he.estate_id = hut.estate_id');
    $query->fields('he',array());
    $query->innerJoin('housing_flat_type', 'hft', 'hft.flat_type_id = hut.flat_type_id');
    $query->fields('hft',array());
    $query->innerJoin('housing_block', 'hb', 'hb.block_id = hut.block_id');
    $query->fields('hb',array());
    $query->innerJoin('housing_flat', 'hf', 'hf.flat_id = hut.flat_id');
    $query->fields('hf',array());
    
    $query->condition('flag','new');
	
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
			
			
			$fields[] = l('Details','',array('html'=>TRUE,'attributes'=> array('class'=>array('btn bg-success btn-sm px-4 rounded-pill text-white fw-bolder'))));

			


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