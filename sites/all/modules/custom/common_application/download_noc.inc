<?php
<<<<<<< HEAD

function download_noc($online_application_id = '') {
    // $online_application_id = decrypt_url();
    
    date_default_timezone_set("Asia/Kolkata");
    
    $output = '';


    ob_end_clean();
	return pdf_using_mpdf_api($page_output, $file_name);
=======
date_default_timezone_set("Asia/Kolkata");

function download_noc($application_id = '') {
     
    // echo $application_id = decrypt_url($application_id); die;
    
    // get flat information from application id
    $query = db_select('housing_online_application', 'hoa');
    $query->fields('hoa', array('online_application_id','applicant_official_detail_id'));
    $query->innerJoin('housing_applicant_official_detail','haod', 'haod.applicant_official_detail_id = hoa.applicant_official_detail_id');
    $query->fields('haod', array('uid','ddo_id','hrms_id'));
    $query->innerJoin('housing_applicant','ha','ha.housing_applicant_id = haod.housing_applicant_id');
    $query->fields('ha', array('applicant_name'));
    $query->innerJoin('housing_flat_occupant','hfo','hfo.online_application_id = hoa.online_application_id');
    $query->fields('hfo', array('flat_occupant_id','flat_id','release_date'));
    $query->innerJoin('housing_flat','hf','hf.flat_id = hfo.flat_id');
    $query->fields('hf', array('estate_id','block_id','floor','flat_type_id','flat_no','flat_status_id'));

    $query->innerJoin('housing_estate','he','he.estate_id = hf.estate_id');
    $query->fields('he', array('estate_id','estate_name','estate_address'));

    $query->innerJoin('housing_block','hb','hb.block_id = hf.block_id');
    $query->fields('hb', array('block_name'));

    $query->condition('hoa.online_application_id', $application_id, '=');

    $result = $query->execute()->fetchAssoc();
    // echo '<pre>';print_r($query); print_r($result);    die;
    
    
    $page_output = '';
		
		$page_output = '
		<div align="center">
			<img src="'.drupal_get_path('module', 'waiting_list').'/images/govt_logo1.png" alt="Government Logo">
			<br>Government of West Bengal<br>Housing Department: Allotment Branch<br>
			N. S. Building, 1, K. S Roy Road <br><u>Kolkata - 700001</u><br>
		</div>
		<br>
        <div align="left">
            <font size="4"><b>No: '.date('Ymd').'</b></font>
		<div align="right">
			<font size="3"><b>Date : </b>'.date('l d-m-Y h:i:s A').'</font>
		</div>
		<div>
			<p>From: <b> The Competent Authority under the W. B. Govt. Premises (Regulation of Occupancy Act, 1984)</b></p>
		</div>
        <div>
            <p>To, <b> '.$result['applicant_name'].'</b></p>
        </div>
        <div>
            <p>Subject: Surrender Certificate of Flat No. <b><u>'.$result['block_name'].'/'.$result['flat_no'].'</u></b> <b><u>Floor '.$resul['floor'].' at '.$result['estate_name'].'</u></b> RHE.</p>
        </div>
        <div>
            <p>The undersigned is directed to inform him/her that Flat No. <b><u>'.$result['block_name'].'/'.$result['flat_no'].'</u></b> <b><u>Floor '.$resul['floor'].'</u></b> at <b><u>'.$result['estate_name'].'</u></b> RHE, which was allotted to <b><u>'.$result['applicant_name'].'</u></b>, vide this Department\'s Memo No. ________________, dated <b><u>'.$result['release_date'].'</u></b>, has been vacated and surrendered by him/her on ____________. As per Clause 15 of the Terms & Conditions under the West Bengal Government Premises (Regulation of Occupancy) Act, 1984, the surrender of the said flat has duly been certified by the Work Assistant of the concerned RHE.</p>
        </div>
        ';
		
		ob_end_clean();
		return pdf_using_mpdf_api($page_output, 'noc_'.$online_application_id.'.pdf');
>>>>>>> 2502ab393f49b61f25516e6bb0502acbb6d447a0



}