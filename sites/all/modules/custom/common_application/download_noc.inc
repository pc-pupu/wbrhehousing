<?php

function download_noc($online_application_id = '') {
    // $online_application_id = decrypt_url();
    
    date_default_timezone_set("Asia/Kolkata");
    
    $output = '';


    ob_end_clean();
	return pdf_using_mpdf_api($page_output, $file_name);



}