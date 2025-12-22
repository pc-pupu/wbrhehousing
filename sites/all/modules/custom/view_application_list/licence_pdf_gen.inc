<?php 
// Done by Subham dt.07-08-2024
function licence_pdf_gen($app_id = '') {
    global $base_path, $base_root;
    $app_id= decrypt_url($app_id);
    $today_date = date('d-m-y');
    // $fetch_applicant_details = fetch_applicant_details($app_id);
    // print_r($fetch_applicant_details);die;
    
    // and haod.is_active = 1
    $sql = "select *
    from housing_online_application hoa
    inner join housing_license_application hla on hla.online_application_id = hoa.online_application_id
    inner join housing_occupant_license hol on hol.license_application_id = hla.license_application_id
    inner join housing_flat hf on hf.flat_id = hla.allotment_flat_id
    inner join housing_estate he on he.estate_id = hf.estate_id
    inner join housing_division hd on hd.division_id = he.division_id
    inner join housing_subdivision hs on hs.subdiv_id = he.subdiv_id
    inner join housing_applicant_official_detail haod on haod.applicant_official_detail_id = hoa.applicant_official_detail_id
    inner join housing_applicant ha on ha.housing_applicant_id = haod.housing_applicant_id
    inner join housing_ddo hdd on hdd.ddo_id = haod.ddo_id
    inner join housing_flat_type hft on hft.flat_type_id = hf.flat_type_id
    inner join housing_district hdis on hdis.district_code = haod.office_district
    inner join housing_treasury_estate_mapping htem on htem.estate_id = he.estate_id
    inner join housing_treasury ht on ht.treasury_id = htem.treasury_id
    inner join housing_block hb on hb.block_id = hf.block_id
    where hoa.online_application_id = :app_id";

    $fetch_applicant_details = db_query($sql, array(':app_id'=>$app_id))->fetchObject();
    //  print_r($fetch_applicant_details);die;

    if($fetch_applicant_details->type_of_application == 'New'){
       $allotment_type = 'Fresh'; 
    }else{
        $allotment_type = $fetch_applicant_details->type_of_application;
    }
    
    /*~~~~~~~~~~~~~~~~~~~~~ QR Code Generation Code Starts Here ~~~~~~~~~~~~~~~~~~~*/

    $PNG_TEMP_DIR = drupal_get_path('module','view_application_list').'/phpqrcode/upload/license_qr_code/';
    $PNG_WEB_DIR = drupal_get_path('module','view_application_list').'/phpqrcode/upload/license_qr_code/';

    include "./sites/all/modules/custom/view_application_list/phpqrcode/qrlib.php";

    if (!file_exists($PNG_TEMP_DIR))
    mkdir($PNG_TEMP_DIR);

    $filename = $PNG_TEMP_DIR.'test.png';
    $errorCorrectionLevel = 'H';
    $matrixPointSize = 10;

    $license_issue_date = date('d/m/Y', strtotime($fetch_applicant_details->license_issue_date));

    // Updated by Subham dt.19-05-2025 // modified by Moumita on 27-07-2025

    $flat_info = $fetch_applicant_details->estate_name.', '.$fetch_applicant_details->estate_address.', Flat Type:'.trim($fetch_applicant_details->flat_type).', Floor:'.$fetch_applicant_details->floor.', Flat No.:'.$fetch_applicant_details->flat_no; 

    $license_info = "License No.:".$fetch_applicant_details->license_no.', Issue Date: '.$license_issue_date;

    $applicant_info = "Name:".$fetch_applicant_details->applicant_name.", Designation:".$fetch_applicant_details->applicant_designation.", Office:".$fetch_applicant_details->office_name.', DDO Code:'.$fetch_applicant_details->ddo_code;
    

    $qr_content = $applicant_info.$license_info."Flat Info- ".$flat_info;

    $filename = $PNG_TEMP_DIR.time().'.png';

    QRcode::png($qr_content, $filename, $errorCorrectionLevel, $matrixPointSize, 10); 

    $generate_qr_image = $PNG_WEB_DIR.basename($filename);


    $img_data = base64_encode(file_get_contents(drupal_get_path('module','view_application_list') .'/phpqrcode/upload/license_qr_code/'. basename($filename)));
    $qr_image = '<img src="data:image/png;base64,' . $img_data . '" style="margin-top:20px width:150px; height:150px;" />';
// print $qr_image;die;
    /*~~~~~~~~~~~~~~~~~~~~~ QR Code Generation Code Ends Here ~~~~~~~~~~~~~~~~~~~*/

    $page_output ='';  // pdf Content changed by Subham dt.28-11-2025
    $page_output .= '
    <div style="border: 1px solid #000; padding: 20px;">
        <table width="100%" style="border-collapse:collapse; margin:0; padding:0;">
            <tr>
                <td align="center" style="padding:0;">
                    <div style="font-size:15px; font-weight:bold; line-height:1;">
                        GOVERNMENT OF WEST BENGAL
                    </div>
                    <div style="font-size:12px; line-height:1;">
                        HOUSING DEPARTMENT
                    </div>
                    <div style="font-size:12px; line-height:1;">
                        Licence for the Occupation of Government Premises
                    </div>
                </td>
            </tr>
        </table>
        <table>
            <tr>
                <td style="font-size:12px;">
                    <b>License No.: '.$fetch_applicant_details->license_no.'</b>
                    <td style="text-align:right; font-weight:bold;">
                        Dated: '.date('d/m/Y', strtotime($fetch_applicant_details->license_issue_date)).'
                    </td>
                    <br><br>
                </td>
            </tr>
            <tr>
                <td colspan="2" style="font-size:12px; line-height:1.5;">
                Licence under section 4 of the West Bengal Government Premises (Regulation of Occupancy) Act, 1984 is hereby granted to <b>'.$fetch_applicant_details->applicant_name.'</b> for the occupation of <b>Flat No:'.$fetch_applicant_details->block_name.'/'.$fetch_applicant_details->flat_no.', ('.$fetch_applicant_details->flat_type.'-Type) by way of '.$allotment_type.' Allotment at '.$fetch_applicant_details->estate_name.', '.$fetch_applicant_details->estate_address.'</b> subject to the provisions of the said Act and to the additional terms and conditions mentioned below.</td>
            </tr>
        </table>
        <table width="100%" style="border-collapse:collapse; margin-top:5px; margin-left:25px;">
            <tr>
                <td style="font-size:12px;">
                    <ol style="margin:0; padding-left:15px;">
                        <li>The validity of this licence will expire on <b> Date of retirement / Transfer from '.$fetch_applicant_details->district_name.' Zone / Death of Licensee during service / Any other reason as W. B. Government Premises Regulation of Occupancy Act,1984</b></li>
                        <li>Date Of Retirement: <b>'.date('d/m/Y',strtotime($fetch_applicant_details->date_of_retirement)).'</b></li>
                        <li>The physical possession of the flat must be taken within 15 days from the receipt of the license.</li>
                    </ol>
                </td>
            </tr>
        </table>
        <table width="100%" style="border-collapse:collapse; margin-top:10px;">
            <tr>
                <td align="right" style="font-size:12px;">
                    Yours faithfully,<br><br>
                    COMPETENT AUTHORITY UNDER<br>
                    THE WEST BENGAL GOVT. PREMISES<br>
                    (REGULATION OF OCCUPANCY) ACT, 1984
                </td>
            </tr>
        </table>
        <table width="100%" style="border-collapse:collapse; margin-top:10px;">
            <tr>
                <td align="center">
                    <h3 style="margin:0; font-size:14px;"><b>ADDITIONAL TERMS AND CONDITIONS</b></h3>
                </td>
            </tr>
        </table>
        <table width="100%" style="border-collapse:collapse; margin-left:25px;">
            <tr>
                <td style="font-size:12px;">
                    <ol style="margin:0; padding-left:15px;">
                        <li>He/She will not draw H.R.A.</li>
                        <li>He/She is requested to intimate this Department the date of possession of the flat.</li>
                    </ol>
                </td>
            </tr>
        </table>
        <table width="100%" style="border-collapse:collapse; margin-top:10px;">
            <tr>
                <td align="right" style="font-size:12px;">
                    Yours faithfully,<br><br>
                    COMPETENT AUTHORITY UNDER<br>
                    THE WEST BENGAL GOVT. PREMISES<br>
                    (REGULATION OF OCCUPANCY) ACT, 1984
                </td>
            </tr>
        </table>
        <table width="100%" style="border-collapse: collapse; font-size:12px;">
            <tr>
                <td style="text-align:left; font-weight:bold;">
                    License No.: '.$fetch_applicant_details->license_no.'
                    (Computerized Appliation No.: '.$fetch_applicant_details->application_no.')
                </td>
                <td style="text-align:right; font-weight:bold;">
                    Dated: '.$license_issue_date.'
                </td>
            </tr>
        </table>

        <table width="100%" style="margin-top:10px;">
            <tr>
                <td>
                    Copy forwarded, for information and necessary action to:
                </td>
            </tr>
            <tr>
                <td width="100%" style="font-size:12px;">
                    <ol style="margin:0; padding-left:15px;">
                        <li><b>'.$fetch_applicant_details->applicant_name.', '.$fetch_applicant_details->applicant_designation.'</b></li>
                        <li><b>'.$fetch_applicant_details->office_name.', '.$fetch_applicant_details->office_street.', '.$fetch_applicant_details->office_city_town_village.', '.$fetch_applicant_details->office_post_office.'-'.$fetch_applicant_details->office_pin_code.'</b></li>
                        <li><b>'.$fetch_applicant_details->ddo_designation.'</b>(Head of Office/D.D.O) He/She is requested to intimate this office regarding transfer / retirement of the license from the present posting.</li>
                        <li>The Executive Engineer, '.$fetch_applicant_details->division_name.'</li>
                        <li>The Executive Engineer, Electrical Division No.II,67.</li>
                        <li>The Pay and Accounts Officer, '.$fetch_applicant_details->treasury_name.'</li>
                        <li>The Work Assistant, '.$fetch_applicant_details->estate_name.', '.$fetch_applicant_details->estate_address.'</li>
                        <li>The Assistant Engineer, '.$fetch_applicant_details->subdiv_name.'</li>
                        <li>The Computer Cell.</li>
                    </ol>
                </td>
            </tr>
        </table>
        <table width="100%" style="border-collapse:collapse; margin-top:10px;">
            <tr>
                <td style="font-size:11px;">
                2. Special attention is invited to clauses 10 and 11 of the terms and conditions mentioned overleaf. Pending receipt of roll for licence fee from the Executive Engineer, licence fee at the rate indicated under additional terms and conditions should be realised from the salary of the licensee and credited to the head "0216-Housing-02-Urban Housing-105-Receipts from Rental Housing scheme-001 collection for RHS-05-Rent". Code No.[0216-02-105-001-05].
                </td>
            </tr>
        </table>
        <table width="100%" style="border-collapse:collapse; margin-top:10px;">
            <tr>
                <td width="50%" style="font-size:12px;">
                    '. $qr_image .'
                </td>
                <td width="50%" align="right" style="font-size:12px;">
                    Yours faithfully,<br><br>
                    COMPETENT AUTHORITY UNDER<br>
                    THE WEST BENGAL GOVT. PREMISES<br>
                    (REGULATION OF OCCUPANCY) ACT, 1984
                </td>
            </tr>
            <tr>
                <td align="center" colspan="2" style="padding-top: 5px; white-space: nowrap;">
                    Note: This is computer generated document and does not require signature. 
                </td>
            </tr>
        </table>
        <br>
        <table width="100%" style="border-collapse:collapse; margin-top:20px;">
            <tr>
                <td align="center">
                    <h4 style="margin:0;"><b>Terms and conditions applicable to all licences in respect of Government Premises</b></h4>
                </td>
            </tr>
            <tr>
                <td style="font-size:12px; margin-top:10px;" >
                    <ol style="font-size:1em"><br>
                        <li><font size="2">Subject to the provisions of item 2 and section 11, a license shall remain valid for specified period and such period may be renewed from time by the competent Authority. For this purpose specified period shall mean-</font></li>
                            <ol type="i">
                                <li><font size="2">the period specified in the liocense or, where nothing is specified in the license, a period of three years from the date of issue of license and such date, in the case of anything deemed to be a license under section 9, shall be the appointed day;or</font></li>
                                <li> <font size="2">any period for which the license is renewed by the Competent Authority.</font></li>
                            </ol>
                        <li><font size="2">An application for the renewal of a license shall be made to the Competent Authority in the prescribed manner. No such application shall be refused by the Competent Authority if the licensee continues to be eligible to hold the license in accordance with the provision of this Act. A license shall remain valid during the pendency of an application for its renewal.</font></li>
                        <li><font size="2">An application for the renewal of license shall state inter alia the licensee\'s post and place of posting.</font></li>
                        <li><font size="2">A license shall cease to be valid if the licensee fails to take possession of the premises covered by it (here in after referred to as the premises) within fifteen days of its issue or within such period as the competent Authority may grant, upon an application by the licensee.</font></li>
                        <li><font size="2">The licencee shall use the premises for the purpose for which they have been allotted to him and shall not use them for any other purpose.<br>Example- where a flat is allotted to any person of the purpose of his residence there will be a violation of this condition if he does not himself ordinarily reside in it and allows his dependents or any other person to reside in it or keeps it vacant.</font></li>
                        <li><font size="2">The licencee shall not assign or transfer the premises in any way to any person or put any person in possesission of the premises.</font></li>
                        <li><font size="2">The licencee shall not add to, or alter, any fixtures of the premises or make any structural alterations in the premises without the express permission in writing of the Competent Authoity.</font></li>
                        <li><font size="2">The licencee shall not cause, or suffer to be caused, any damage to the premises beyond the normal wear and tear through the proper use and occupation of the premises</font></li>
                        <li><font size="2">The licencee shall allow any officer duly authorised in this behalf by the Competent Authority to inspect the premises as and when necessary.</font></li>
                        <li><font size="2">The licencee shall pay such license fee for the premises as may be determined from time to time by the-Competent Authority.</font></li>
                        <li><font size="2">The licence fee in respect of each month shall be payable by the first week if the following month in such manner as may be stipulated by the Competent Authority.
                        <br>The licencee shall send to the Competent Authority as and when required by it a statement showing the details of the license fee paid by the license, and such statement shall be in such form, and shall be authenticated in such manner, as may be stipulated by the Competent Authority.</font></li>
                        <li><font size="2">The licencee, if he is in occupation of Government premises on the appointed day, shall inform the Competent Authority in Writing within one month from that day about the post held by him,the headquarters of his post and the date of his retirement on superannuation based upon entries contained in his Service Book.</li>
                        <li><font size="2">The licencee shall inform the Competent Authority in writing about every change in his post o place of posting and every change in his is by reason of his going on leave, or being oplaced under suspension or by any other reason within a week of the change taking place.</font></li>
                        <li><font size="2">A license may be terminated upon--
                            <ol type="a">
                            <li><font size="2">the acquisition by the licensee of any other premises by way of ownership,tenancy or otherwise either in his own name or in the name of any member of family dependent on him.</font></li>
                            <li><font size="2">the violation by the licensee of any of the terms and conditions of the license;
                            <li><font size="2">the licencee being placed under suspension, or upon proceeding on leave of any kind, for a period exceeding six months:<br>Provided that where, by reason of any declaration made under section 5, that post from which the license has proceeded on leave is a specified post in respect of the premises occupied by him and authority granting the leave has not certified that upon the expiry of the leave the licencee is likely to return to that post, the license may be terminated at any time after the commencement of the leave;</font></li>
                            <li><font size="2">the expiry of one calendar month\'s notice given by the competent Authority to the licensee.</font></li></ol>
                            <li><font size="2">Upon the expiry or termination of any license, the licencee shall deliver vacant possession of the premises to the competent Authority,or to any other person authorised by the Competent Authority in this behalf, in the same condition in which the licencee took possession of the premises.</font></li>
                        <li><font size="2">
                            The license shall automatically terminated upon:
                            <ol type="i">
                                <li><font size="2">The death of the licensee or </font></li>
                                <li><font size="2">the cessation of the licensee\'s employment under the state Government by reason of his retirement, resignation, discharge or dismissal or by any other reason,or</font></li>
                                <li><font size="2">the licensee ceasing to hold any specified post under the state Government by reason of his transfer from any such post on any other reason.</font></li>
                            </ol>
                        </li>
                    </ol>
                </td>
            </tr>
            <tr>
                <td align="center" colspan="2" style="padding-top: 5px; white-space: nowrap;">
                        Note: This is computer generated document and does not require signature. 
                </td>
            </tr>
        </table>
    </div>';
	ob_end_clean();
    $file_name = '';
    return pdf_using_mpdf_api($page_output, $file_name);
}


/* debaleena start 08-08-2024 */

function fetch_applicant_details($app_id = '') {
    $query = db_select('housing_online_application', 'hoa');
	$query->innerJoin('housing_flat_occupant', 'hfo','hfo.online_application_id = hoa.online_application_id');
	$query->innerJoin('housing_applicant_official_detail', 'haod','haod.applicant_official_detail_id = hoa.applicant_official_detail_id');
    $query->innerJoin('housing_ddo','hdo','hdo.ddo_id = haod.ddo_id'); // Added by Subham dt.19-05-2025
	$query->innerJoin('housing_flat', 'hf','hf.flat_id = hfo.flat_id');
	$query->innerJoin('housing_estate', 'he','he.estate_id = hf.estate_id');
	$query->innerJoin('housing_district', 'hd','hd.district_code = he.district_code');
    $query->innerJoin('housing_flat_type', 'hft','hft.flat_type_id = hf.flat_type_id');
    // $query->innerJoin('housing_applicant', 'ha','ha.uid = haod.uid');   // Turned off by Subham Dt.22-04-2025
    $query->innerJoin('housing_applicant', 'ha','ha.housing_applicant_id = haod.housing_applicant_id'); // Done by Subham Dt.22-04-2025
    $query->innerJoin('housing_license_application', 'hla','hla.online_application_id = hoa.online_application_id');
    $query->innerJoin('housing_occupant_license', 'hol','hol.license_application_id = hla.license_application_id');
	$query->condition('hoa.online_application_id', $app_id, '=');
	//$query->condition('haod.is_active', '1', '=');   off by debaleena for license generated list 05/05/2025
	$query->addField('hoa', 'application_no');
	$query->addField('hfo', 'allotment_no');
	$query->addField('hfo', 'allotment_date');
	$query->addField('hfo', 'flat_id');
	$query->addField('he', 'estate_name');
	$query->addField('he', 'estate_address');
	$query->addField('hd', 'district_name');
	$query->addField('hfo', 'flat_occupant_id');
    $query->addField('hf', 'flat_no');
    $query->addField('hft', 'flat_type');
    $query->addField('ha', 'applicant_name');
    $query->addField('hdo', 'ddo_code');  // Added by Subham dt.19-05-2025
    $query->fields('haod', array('applicant_designation','office_name','office_street','office_post_office','office_pin_code'));  // Added by Subham dt.19-05-2025
    $query->addField('hol', 'license_no');

    $query->addField('hol', 'license_expiry_date');
	//$query->addField('haod', 'uid');

	$result = $query->execute(); 
	$result = $result->fetchObject();
    return $result;
}

/* end */