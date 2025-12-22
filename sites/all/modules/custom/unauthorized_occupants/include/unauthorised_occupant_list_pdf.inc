<?php
// Done by Subham dt.19-12-2025 for unauthorised occupant list (with HRMS ID) pdf generation //
function unauthorised_occupant_list_whrms_pdf_gen($rhe_id = 0, $flat_type_id = 0, $block_id = 0) {
    date_default_timezone_set("Asia/Kolkata");

    $rhe_id = decrypt_url($rhe_id);
    $flat_type_id = decrypt_url($flat_type_id);
    $block_id = decrypt_url($block_id);

    if((is_numeric($rhe_id) && $rhe_id != 0) && (is_numeric($flat_type_id) && $flat_type_id != 0) && (is_numeric($block_id) && $block_id != 0)) {
        $query = db_select('housing_online_application', 'hoa');
        $query->innerJoin('housing_applicant_official_detail', 'haod', 'hoa.applicant_official_detail_id = haod.applicant_official_detail_id');
        $query->innerJoin('housing_applicant', 'ha','ha.housing_applicant_id = haod.housing_applicant_id');
        $query->innerJoin('housing_flat_occupant', 'hfo','hoa.online_application_id = hfo.online_application_id');
        $query->leftJoin('housing_occupant_license', 'hol', 'hol.flat_occupant_id = hfo.flat_occupant_id');
        $query->innerJoin('housing_flat','hf','hf.flat_id = hfo.flat_id');
        $query->innerJoin('housing_flat_type','hft','hft.flat_type_id = hf.flat_type_id');  
        $query->innerJoin('housing_block','hb','hb.block_id = hf.block_id');
        $query->innerJoin('housing_estate','he','he.estate_id = hf.estate_id');
        $query->addField('ha', 'applicant_name');
        $query->addField('hoa', 'application_no');
        $query->fields('hf', array('floor', 'flat_no'));
        $query->fields('hol', array('license_no', 'possession_date', 'license_expiry_date', 'flat_occupant_id', 'authorised_or_not'));
        $query->fields('hft',array());
        $query->fields('hb',array());
        $query->fields('he',array('estate_id','estate_name'));
        $query->condition('hol.authorised_or_not','unauthorised');

        if($rhe_id != 0 && $flat_type_id != 0 && $block_id != 0){
            $db_and = db_and();
            $db_and->condition('hf.estate_id', $rhe_id, '=');
            $db_and->condition('hf.flat_type_id', $flat_type_id, '=');
            $db_and->condition('hf.block_id', $block_id, '=');
            $query->condition($db_and);
        }

        $result = $query->execute();

        $page_output = '';
        $page_output = '
        <div align="center">
            <font size="6"><b>Housing Department</b></font>
            <br>
            <font size="5"><b>Government of West Bengal</b></font>
            <br>
            1, K. S Roy Road, Kolkata - 700001
            <br>
            <font size="4"><b>e-Allotment of Rental Housing Estate</b></font>
            <br>
            <font size="4"><b>Unauthorised Occupant List (with HRMS ID)</b></font>
        </div>
        <br>
        <div align="right">
            <font size="4"><b>Date : </b>'.date('l d-m-Y h:i:s A').'</font>
        </div>
        <br>
        
        <div>
            <table style="font-family: Arial, sans-serif; border: 1px solid black; border-collapse: collapse;" align="center" width="100%">
            <thead>
                <tr bgcolor="#999999">
                    <th align="center" width="5%" style="border: 1px solid black; border-collapse: collapse;"><font size="5"><b>Sl. No.</b></font></th>
                    <th align="center" width="25%" style="border: 1px solid black; border-collapse: collapse;"><font size="5"><b>Applicant Name</b></font></th>
                    <th align="center" width="19%" style="border: 1px solid black; border-collapse: collapse;"><font size="5"><b>Application No.</b></font></th>
                    <th align="center" width="15%" style="border: 1px solid black; border-collapse: collapse;"><font size="5"><b>License No.</b></font></th>
                    <th align="center" width="13%" style="border: 1px solid black; border-collapse: collapse;"><font size="5"><b>Possession Date</b></font></th>
                    <th align="center" width="15%" style="border: 1px solid black; border-collapse: collapse;"><font size="5"><b>License Expiry Date</b></font></th>
                    <th align="center" width="19%" style="border: 1px solid black; border-collapse: collapse;"><font size="5"><b>Estate Name</b></font></th>
                    <th align="center" width="12%" style="border: 1px solid black; border-collapse: collapse;"><font size="5"><b>Flat Type</b></font></th>
                    <th align="center" width="15%" style="border: 1px solid black; border-collapse: collapse;"><font size="5"><b>Block Name</b></font></th>
                    <th align="center" width="9%" style="border: 1px solid black; border-collapse: collapse;"><font size="5"><b>Floor</b></font></th>
                    <th align="center" width="10%" style="border: 1px solid black; border-collapse: collapse;"><font size="5"><b>Flat No.</b></font></th>
                </tr>
            </thead>
        ';
            $i = 1;
            while($data = $result->fetchObject()) {
                // print_r($data);die;
                $page_output .='
            <tbody>
                <tr>
                    <td align="center" style="border: 0.5px solid black; border-collapse: collapse;"><font size="4">'.$i.'</font></td>
                    <td align="center" style="border: 0.5px solid black; border-collapse: collapse;"><font size="4">'.$data->applicant_name.'</font></td>
                    <td align="center" style="border: 0.5px solid black; border-collapse: collapse;"><font size="4">'.$data->application_no.'</font></td>
                    <td align="center" style="border: 0.5px solid black; border-collapse: collapse;"><font size="4">'.(!empty($data->license_no) ? $data->license_no : '--').'</font></td>
                    <td align="center" style="border: 0.5px solid black; border-collapse: collapse;"><font size="4">'.(!empty($data->possession_date) ? date('d/m/Y', strtotime($data->possession_date)) : '--').'</font></td>
                    <td align="center" style="border: 0.5px solid black; border-collapse: collapse;"><font size="4">'.(!empty($data->license_expiry_date) ? date('d/m/Y', strtotime($data->license_expiry_date)) : '--').'</font></td>
                    <td align="center" style="border: 0.5px solid black; border-collapse: collapse;"><font size="4">'.$data->estate_name.'</font></td>
                    <td align="center" style="border: 0.5px solid black; border-collapse: collapse;"><font size="4">'.$data->flat_type.'</font></td>
                    <td align="center" style="border: 0.5px solid black; border-collapse: collapse;"><font size="4">'.$data->block_name.'</font></td>
                    <td align="center" style="border: 0.5px solid black; border-collapse: collapse;"><font size="4">'.$data->floor.'</font></td>
                    <td align="center" style="border: 0.5px solid black; border-collapse: collapse;"><font size="4">'.$data->flat_no.'</font></td>
                </tr>';
                $i++;
            }
            $page_output .='
            </tbody></table>
        </div>';
    }
    else {
        $page_output = 'Invalid Access';	
    }
    $file_name = 'Unauthorised_Occupant_List_With_HRMS_ID_' . date('d_m_Y_His') . '.pdf';
    
    ob_end_clean();
    return pdf_using_mpdf_api($page_output, $file_name);
}


// Done by Subham dt.19-12-2025 for unauthorised occupant list (without HRMS ID) pdf generation //
function unauthorised_occupant_list_wohrms_pdf_gen($rhe_id = 0, $flat_type_id = 0, $block_id = 0) {
    date_default_timezone_set("Asia/Kolkata");

    $rhe_id = decrypt_url($rhe_id);
    $flat_type_id = decrypt_url($flat_type_id);
    $block_id = decrypt_url($block_id);

    if((is_numeric($rhe_id) && $rhe_id != 0) && (is_numeric($flat_type_id) && $flat_type_id != 0) && (is_numeric($block_id) && $block_id != 0)) {
        $query = db_select('housing_existing_occupant_draft', 'heod');
        $query->fields('heod',array());
        $query->innerJoin('housing_flat','hf','hf.flat_id = heod.flat_id');
        $query->fields('hf',array());
        $query->innerJoin('housing_flat_type','hft','hft.flat_type_id = hf.flat_type_id');
        $query->fields('hft',array());
        $query->innerJoin('housing_block','hb','hb.block_id = hf.block_id');
        $query->fields('hb',array());
        $query->innerJoin('housing_estate','he','he.estate_id = hf.estate_id');
        $query->fields('he',array('estate_id','district_code','estate_name'));
        $query->condition('authorised_or_not','unauthorised');
        
        if($rhe_id != 0 && $flat_type_id != 0 && $block_id != 0){
            $query->condition('he.estate_id',$rhe_id);
            $query->condition('hft.flat_type_id',$flat_type_id);
            $query->condition('hb.block_id',$block_id);

        }
        $result = $query->execute();
        
        $page_output = '';
        $page_output = '
        <div align="center">
            <font size="6"><b>Housing Department</b></font>
            <br>
            <font size="5"><b>Government of West Bengal</b></font>
            <br>
            1, K. S Roy Road, Kolkata - 700001
            <br>
            <font size="4"><b>e-Allotment of Rental Housing Estate</b></font>
            <br>
            <font size="4"><b>Unauthorised Occupant List (without HRMS ID)</b></font>
        </div>
        <br>
        <div align="right">
            <font size="4"><b>Date : </b>'.date('l d-m-Y h:i:s A').'</font>
        </div>
        <br>
        
        <div>
            <table style="font-family: Arial, sans-serif; border: 1px solid black; border-collapse: collapse;" align="center" width="100%">
            <thead>
                <tr bgcolor="#999999">
                    <th align="center" width="5%" style="border: 1px solid black; border-collapse: collapse;"><font size="5"><b>Sl. No.</b></font></th>
                    <th align="center" width="25%" style="border: 1px solid black; border-collapse: collapse;"><font size="5"><b>Applicant Name</b></font></th>
                    <th align="center" width="25%" style="border: 1px solid black; border-collapse: collapse;"><font size="5"><b>Authorised / Unauthorised</b></font></th>
                    <th align="center" width="25%" style="border: 1px solid black; border-collapse: collapse;"><font size="5"><b>Estate Name</b></font></th>
                    <th align="center" width="12%" style="border: 1px solid black; border-collapse: collapse;"><font size="5"><b>Flat Type</b></font></th>
                    <th align="center" width="15%" style="border: 1px solid black; border-collapse: collapse;"><font size="5"><b>Block Name</b></font></th>
                    <th align="center" width="15%" style="border: 1px solid black; border-collapse: collapse;"><font size="5"><b>Flat No.</b></font></th>
                    <th align="center" width="8%" style="border: 1px solid black; border-collapse: collapse;"><font size="5"><b>Floor</b></font></th>
                </tr>
            </thead>
        ';
            $i = 1;
            while($data = $result->fetchObject()) {
                $page_output .='
            <tbody>
                <tr>
                    <td align="center" style="border: 0.5px solid black; border-collapse: collapse;"><font size="4">'.$i.'</font></td>
                    <td align="center" style="border: 0.5px solid black; border-collapse: collapse;"><font size="4">'.$data->applicant_name.'</font></td>
                    <td align="center" style="border: 0.5px solid black; border-collapse: collapse;"><font size="4">'.$data->authorised_or_not.'</font></td>
                    <td align="center" style="border: 0.5px solid black; border-collapse: collapse;"><font size="4">'.$data->estate_name.'</font></td>
                    <td align="center" style="border: 0.5px solid black; border-collapse: collapse;"><font size="4">'.$data->flat_type.'</font></td>
                    <td align="center" style="border: 0.5px solid black; border-collapse: collapse;"><font size="4">'.$data->block_name.'</font></td>
                    <td align="center" style="border: 0.5px solid black; border-collapse: collapse;"><font size="4">'.$data->flat_no.'</font></td>
                    <td align="center" style="border: 0.5px solid black; border-collapse: collapse;"><font size="4">'.$data->floor.'</font></td>
                </tr>';
                $i++;
            }
            $page_output .='
            </tbody></table>
        </div>';
    }
    else {
        $page_output = 'Invalid Access';	
    }
    $file_name = 'Unauthorised_Occupant_List_Without_HRMS_ID_' . date('d_m_Y_His') . '.pdf';
    
    ob_end_clean();
    return pdf_using_mpdf_api($page_output, $file_name);
}
