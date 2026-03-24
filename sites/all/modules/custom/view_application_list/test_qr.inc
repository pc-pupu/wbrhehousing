<?php 
// Done by Subham dt.07-08-2024
function test_qr($app_id = '') {
    global $base_path, $base_root;

    
    

    /*~~~~~~~~~~~~~~~~~~~~~ QR Code Generation Code Starts Here ~~~~~~~~~~~~~~~~~~~*/

    $PNG_TEMP_DIR = drupal_get_path('module','view_application_list').'/phpqrcode/upload/test/';
    $PNG_WEB_DIR = drupal_get_path('module','view_application_list').'/phpqrcode/upload/test/';

    include "./sites/all/modules/custom/view_application_list/phpqrcode/qrlib.php";

    if (!file_exists($PNG_TEMP_DIR))
    mkdir($PNG_TEMP_DIR);

    $filename = $PNG_TEMP_DIR.'test.png';
    $errorCorrectionLevel = 'H';
    $matrixPointSize = 10;

    
    $qr_content = 'test';

    $filename = $PNG_TEMP_DIR.time().'.png';

    QRcode::png($qr_content, $filename, $errorCorrectionLevel, $matrixPointSize, 10); 

    $generate_qr_image = $PNG_WEB_DIR.basename($filename);


    $img_data = base64_encode(file_get_contents(drupal_get_path('module','view_application_list') .'/phpqrcode/upload/test/'. basename($filename)));
    $qr_image = '<img src="data:image/png;base64,' . $img_data . '" style="margin-top:20px width:150px; height:150px;" />';
    
    print $qr_image;die;
    
    /*~~~~~~~~~~~~~~~~~~~~~ QR Code Generation Code Ends Here ~~~~~~~~~~~~~~~~~~~*/

    return;
}

