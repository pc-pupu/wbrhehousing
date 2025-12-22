<?php

function vacancy_list_page($district_id = 0, $flat_type_id = 0) {
	global $base_path;
	
	$flat_status_id = fetch_flat_status_id('vacant');
	$result1 = rhe_flat_status($flat_status_id);
	$data1 = $result1->fetchObject();
	 
	$output = array();
	$result = rhe_flat_allotment_list($district_id, $flat_type_id, $flat_status_id);
	
	$result2 = rhe_flat_allotment_list($district_id, $flat_type_id, $flat_status_id);
	$data2 = $result2->fetchObject();
	
	//$rows = array();//sd turned off 21-06-2024
	//$header = array();//sd turned off 21-06-2024
	$header['slno'] = array('data' => 'Sl. No.', 'width' => '5%');
	$header['district_name'] = array('data' => 'District', 'width' => '14%');
	$header['estate_name'] = array('data' => 'Name of the RHE', 'width' => '23%');
	$header['estate_address'] = array('data' => 'Estate Address', 'width' => '23%');
	$header['flat_list'] = array('data' => 'Flat No.', 'width' => '20%');
	$header['no_of_vacant_flat'] = array('data' => 'No. of vacant Flat(s)', 'width' => '15%');
	//$header['flat_status'] = array('data' => 'Flat Status');
	
	$i = 1;
	 while($data = $result->fetchObject()) {
		 $fields = array();
		 $fields['slno'] = $i;
		 $fields['district_name'] = $data->district_name;
		 $fields['estate_name'] = $data->estate_name;
		 $fields['estate_address'] = $data->estate_address;
		 $out = rhe_flat_allotment_list_rst_estate($flat_type_id, $flat_status_id, $data->estate_id);
		 $fields['flat_list'] = $out[1];
		 $fields['no_of_vacant_flat'] = $out[0];
		 //$fields['flat_status'] = $data1->flat_status;
		 
		 $rows[] = $fields;
		 $i++;
		 $total_no_flat = $total_no_flat + $out[0];
	 }
	 
	 if ($result->rowCount() == 0 || empty($rows)) {  //changed if empty showing no data found by dg 15-10-2025
		  
	  return '
	   <div class="text-center my-5">
	    <div class="alert alert-danger d-inline-block px-4 py-3 shadow-sm" 
	         style="font-size:16px; border-radius:30px; background-color:#ffe6e6; color:#cc0000; font-weight:500;">
	      ⚠️ There are currently no vacant flats available for the selected criteria.
	    </div>
		  </div>';
	}

	
	$variables = array(
	  'attributes' => array('class' => array('table table-list table-striped')),
	  'header' => $header,
	  'rows' => $rows,
	  'sticky' => true,
	);

	$output = theme('datatable', $variables);

	// Download link + table
	return l(
	  'Download Flat Vacancy List',
	  'flat_vacancy_list_pdf/' . encrypt_url($district_id) . '/' . encrypt_url($flat_type_id) . '/' . encrypt_url($flat_status_id) . '/' . $data2->flat_type . '_type_flat_vacancy_list_' . time(),
	  array(
	    'html' => TRUE,
	    'attributes' => array(
	      'target' => '_blank',
	      'class' => 'btn bg-primary px-5 rounded-pill text-white fw-bolder mb-2'
	    )
	  )
	) . '<div class="mb-5 pb-3">' . $output . '</div>';
}

/* Done by Subham dt.10-11-2025 ~~ This function used for vacancy list fetch (for Sub-Division user) */
function show_vacancy_list($rhe_name = '', $flat_type_id = '') {
	global $base_path;
	
	$flat_status_id = fetch_flat_status_id('vacant');
	$result1 = rhe_flat_status($flat_status_id);
	$data1 = $result1->fetchObject();
	 
	$output = array();
	$result = rhe_wise_vacancy_list_fetch($rhe_name, $flat_type_id, $flat_status_id);
	
	$result2 = rhe_wise_vacancy_list_fetch($rhe_name, $flat_type_id, $flat_status_id);
	$data2 = $result2->fetchObject();

	$header['slno'] = array('data' => 'Sl. No.', 'width' => '5%');
	$header['estate_name'] = array('data' => 'Name of the RHE', 'width' => '23%');
	$header['estate_address'] = array('data' => 'Estate Address', 'width' => '23%');
	$header['flat_type'] = array('data' => 'Flat Type', 'width' => '10%');
	$header['flat_no'] = array('data' => 'Flat No.', 'width' => '20%');
	$header['no_of_vacant_flat'] = array('data' => 'No. of vacant Flat(s)', 'width' => '15%');
	
	$i = 1;
	 while($data = $result->fetchObject()) {
		$fields = array();
		$fields['slno'] = $i;
		$fields['estate_name'] = $data->estate_name;
		$fields['estate_address'] = $data->estate_address;
		$fields['flat_type'] = $data->flat_type;
		$out = rhe_flat_allotment_list_rst_estate($flat_type_id, $flat_status_id, $data->estate_id);
		$fields['flat_list'] = $out[1];
		$fields['no_of_vacant_flat'] = $out[0];		 
		
		$rows[] = $fields;
		$i++;
		$total_no_flat = $total_no_flat + $out[0];
	 }
	 
	 if ($result->rowCount() == 0 || empty($rows)) {  //changed if empty showing no data found by dg 15-10-2025
		  
	  return '
	   <div class="text-center my-5">
	    <div class="alert alert-danger d-inline-block px-4 py-3 shadow-sm" 
	         style="font-size:16px; border-radius:30px; background-color:#ffe6e6; color:#cc0000; font-weight:500;">
	      ⚠️ There are currently no vacant flats available for the selected criteria.
	    </div>
		  </div>';
	}

	
	$variables = array(
	  'attributes' => array('class' => array('table table-list table-striped')),
	  'header' => $header,
	  'rows' => $rows,
	  'sticky' => true,
	);

	$output = theme('datatable', $variables);

	// Download link + table
	return l(
	  '<i class="fa fa-download"></i> Download Flat Vacancy List',
	  'vacancy-list-pdf/' . encrypt_url($rhe_name) . '/' . encrypt_url($flat_type_id) . '/' . encrypt_url($flat_status_id) . '/' . $data2->flat_type . '_type_flat_vacancy_list_' . time(),
	  array(
	      'html' => TRUE,
		  'attributes' => array(
	      'target' => '_blank',
	      'class' => 'btn bg-primary px-5 rounded-pill text-white fw-bolder mb-2'
	    )
	  )
	) . '<div class="mb-5 pb-3">' . $output . '</div>';
}
/* End */

