<?php
function physical_applicant_detailed_view_form($app_no = 0){
    $dec_app_no = decrypt_url($app_no);
    $query1 = "SELECT hoa.application_no, ha.applicant_name, haod.applicant_designation, hasm.status_description,hasm.short_code, ha.guardian_name, ha.date_of_birth,ha.mobile_no,ha.gender,ha.permanent_street,ha.permanent_city_town_village,ha.permanent_post_office,ha.permanent_pincode,ha.permanent_district,ha.present_street,ha.present_city_town_village,ha.present_post_office,ha.present_pincode,ha.present_district, haod.applicant_headquarter, haod.applicant_posting_place, haod.date_of_joining, haod.date_of_retirement, haod.office_name, haod.office_street, haod.office_city_town_village, haod.office_post_office, haod.office_pin_code, haod.hrms_id, haod.office_phone_no, haod.pay_in_the_pay_band ,hdis_present.district_name AS present_district_name, hdis_permanent.district_name AS permanent_district_name, hdis_office.district_name AS office_district_name
              FROM housing_online_application hoa
              INNER JOIN housing_applicant_official_detail haod ON hoa.applicant_official_detail_id = haod.applicant_official_detail_id
              INNER JOIN housing_applicant ha ON ha.uid = haod.uid
              INNER JOIN housing_allotment_status_master hasm ON hasm.short_code = hoa.status
              LEFT JOIN housing_district AS hdis_present ON hdis_present.district_code = ha.present_district
              LEFT JOIN housing_district AS hdis_permanent ON hdis_permanent.district_code = ha.permanent_district
              LEFT JOIN housing_district AS hdis_office ON hdis_office.district_code = haod.office_district
              WHERE hoa.application_no like :dec_app_no";
    $result1 = db_query($query1, array(':dec_app_no' => '%' . db_like($dec_app_no) . '%',));
    $output1 = $result1->fetchAssoc();
    // print_r($output1);die;
    $present_address_parts = array_filter([
      $output1['present_street'] ?? '',
      $output1['present_city_town_village'] ?? '',
      $output1['present_post_office'] ? 'P.O- ' . $output1['present_post_office'] : '',
      $output1['present_district_name'] ?? '',
      $output1['present_pincode'] ? 'Pin-' . $output1['present_pincode'] : '',
    ]);

    $present_address = !empty($present_address_parts) ? implode(', ', $present_address_parts) : 'Not Available';

    $permanent_address_parts = array_filter([
      $output1['permanent_street'] ?? '',
      $output1['permanent_city_town_village'] ?? '',
      $output1['permanent_post_office'] ? 'P.O- ' . $output1['permanent_post_office'] : '',
      $output1['permanent_district_name'] ?? '',
      $output1['permanent_pincode'] ? 'Pin-' . $output1['permanent_pincode'] : '',
    ]);
    
    $permanent_address = !empty($permanent_address_parts) ? implode(', ', $permanent_address_parts) : 'Not Available';
    
    $office_address_parts = array_filter([
      $output1['office_street'] ?? '',
      $output1['office_city_town_village'] ?? '',
      $output1['office_post_office'] ? 'P.O- ' . $output1['office_post_office'] : '',
      $output1['office_district_name'] ?? '',
      $output1['office_pin_code'] ? 'Pin-' . $output1['office_pin_code'] : '',
    ]);
    
    $office_address = !empty($office_address_parts) ? implode(', ', $office_address_parts) : 'Not Available';

    $hrms_id = $output1['hrms_id'] ? $output1['hrms_id'] : 'Not Available';

    $dateOfBirth = $output1['date_of_birth'];
    $formatted_dob = date('d/m/Y', strtotime($dateOfBirth));
    $dob = (isset($formatted_dob) && strtotime($formatted_dob)) ? date('d/m/Y', strtotime($formatted_dob)) : 'Not Available';

    $dateOfBirth = $output1['date_of_birth'];
    $dob = !empty($dateOfBirth) ? date('d/m/Y', strtotime($dateOfBirth)) : 'Not Available'; 

    $dateOfJoin = $output1['date_of_joining'];
    $doj = !empty($dateOfJoin) ? date('d/m/Y', strtotime($dateOfJoin)) : 'Not Available';

    $dateOfRetirement = $output1['date_of_retirement'];
    $dor = !empty($dateOfRetirement) ? date('d/m/Y', strtotime($dateOfRetirement)) : 'Not Available';

    if($output1['short_code'] == 'allotted' || $output1['short_code'] == 'housing_official_approved' || $output1['short_code'] == 'housing_official_reject' || $output1['short_code'] == 'offer_letter_generate' || $output1['short_code'] == 'applicant_acceptance' || $output1['short_code'] == 'applicant_reject' || $output1['short_code'] == 'ddo_verified_2' || $output1['short_code'] == 'housing_sup_approved_2' || $output1['short_code'] == 'license_generate' || $output1['short_code'] == 'existing_occupant'){
        $query2 = "SELECT he.estate_name, he.estate_address, hft.flat_type, hf.floor, hf.flat_no, hf.block_id, hb.block_name
                   FROM housing_online_application hoa
                   INNER JOIN housing_applicant_official_detail haod ON haod.applicant_official_detail_id = hoa.applicant_official_detail_id
                   INNER JOIN housing_flat_occupant hfo ON hfo.online_application_id = hoa.online_application_id
                   INNER JOIN housing_flat hf ON hf.flat_id = hfo.flat_id
                   INNER JOIN housing_estate he ON he.estate_id = hf.estate_id
                   INNER JOIN housing_flat_type hft ON hft.flat_type_id = hf.flat_type_id
                   INNER JOIN housing_block hb ON hf.block_id = hb.block_id
                   WHERE hoa.application_no = '$dec_app_no'";
        $result2 = db_query($query2);
        $output2 = $result2->fetchAssoc();
    } else {
        $extra_query = [];
        if (isset($query2)) {
          $result2 = db_query($query2);
          $extra_query = $result2->fetchAssoc();
        }
    }

    if (!isset($extra_query) || !is_array($extra_query)) {
      $extra_query = [];
    }

    if (!empty($output1) || !empty($extra_query)) {
      $output = array_merge($output1 ?: [], $extra_query);
    }


    $display_output1 = '
    <div class="table-bottom">
        <table class="table table-list">
          <tr>
            <th colspan="2" style="background: none repeat scroll 0 0 #0a8bd6dc;color:white;text-align: center;font-size: 18px;line-height: 24px; font-weight: normal;font-family: "Dosis",Arial,Verdana,serif;" class="first">Application Information</th>
          </tr>
          <tr>
            <th style="background-color:#00000000" width="50%">Application Number:</th>
            <td width="50%">'.$output['application_no'].'</td>
          </tr>
          <tr>
            <th style="background-color:#00000000">Application Status:</th>
            <td>'.$output['status_description'].'</td>
          </tr>
          <tr>
            <th colspan="2" style="background: none repeat scroll 0 0 #0a8bd6dc;color:white;text-align: center;font-size: 18px;line-height: 24px; font-weight: normal;font-family: "Dosis",Arial,Verdana,serif;" class="first">Applicant Personal Information (According to Service Book)</th>
          </tr>
          <tr>
            <th style="background-color:#00000000">Applicant\'s Name:</th>
            <td>'.$output['applicant_name'].'</td>
          </tr>
          <tr>
            <th style="background-color:#00000000">Applicant\'s Guardian Name:</th>
            <td>'.$output['guardian_name'].'</td>
          </tr>
          <tr>
            <th style="background-color:#00000000">Date of Birth (According to Service Book):</th>
            <td>'.$dob.'</td>
          </tr>
          <tr>
            <th style="background-color:#00000000">Present Address:</th>
            <td>'.$present_address.'</td>
          </tr>
          <tr>
            <th style="background-color:#00000000">Parmanent Address:</th>
            <td>'.$permanent_address.'</td>
          </tr>
          <tr>
            <th style="background-color:#00000000">Mobile No.:</th>
            <td>'.(isset($output['mobile_no']) ? $output['mobile_no'] : 'Not Avalaible') .'</td>
          </tr>
          <tr>
            <th colspan="2" style="background: none repeat scroll 0 0 #0a8bd6dc;color:white;text-align: center;font-size: 18px;line-height: 24px; font-weight: normal;font-family: "Dosis",Arial,Verdana,serif;" class="first">Applicant Official Information</th>
          </tr>
          <tr>
            <th style="background-color:#00000000">HRMS ID:</th>
            <td>'.$hrms_id.'</td>
          </tr>
          <tr>
            <th style="background-color:#00000000">Designation:</th>
            <td>'.$output['applicant_designation'].'</td>
          </tr>
          <tr>
            <th style="background-color:#00000000">Office Name:</th>
            <td>'.$output['office_name'].'</td>
          </tr>
          <tr>
            <th style="background-color:#00000000">Office Address:</th>
            <td>'.$office_address.'</td>
          </tr>
          <tr>
            <th style="background-color:#00000000">Date of Joining (According to Service Book):</th>
            <td>'.$doj.'</td>
          </tr>
          <tr>
            <th style="background-color:#00000000">Date of Retirement (According to Service Book):</th>
            <td>'.$dor.'</td>
          </tr>
          <tr>
            <th style="background-color:#00000000">Office Phone Number:</th>
            <td>'.(isset($output['office_phone_no']) ? $output['office_phone_no'] : 'Not Avalaible').'</td>
          </tr>
          <tr>
            <th style="background-color:#00000000">Applicant Headquarter:</th>
            <td>'.(isset($output['applicant_headquarter']) ? $output['applicant_headquarter'] : 'Not Avalaible').'</td> 
          </tr>
          <tr>
            <th style="background-color:#00000000">Applicant Posting Place:</th>
            <td>'.(isset($output['applicant_posting_place']) ? $output['applicant_posting_place'] : 'Not Avalaible').'</td>
          </tr>
          <tr>
            <th style="background-color:#00000000">Basic Pay:</th>
            <td>'.(isset($output['pay_in_the_pay_band']) ? $output['pay_in_the_pay_band'] : 'Not Avalaible').'</td>
          </tr>
          <tr>
            <th colspan="2" style="background: none repeat scroll 0 0 #0a8bd6dc;color:white;text-align: center;font-size: 18px;line-height: 24px; font-weight: normal;font-family: "Dosis",Arial,Verdana,serif;" class="first">Information for Allotment</th>
          </tr>
          <tr>
            <th style="background-color:#00000000">Estate Name:</th>
            <td>'.(isset($output['estate_name']) ? $output['estate_name'] : 'Not alloted').'</td>
          </tr>
          <tr>
            <th style="background-color:#00000000">Estate Address:</th>
            <td>'.(isset($output['estate_address']) ? $output['estate_address'] : 'Not alloted').'</td>
          </tr>
          <tr>
            <th style="background-color:#00000000">Flat Type:</th>
            <td>'.(isset($output['flat_type']) ? $output['flat_type'] : 'Not alloted').'</td>
          </tr>
          <tr>
            <th style="background-color:#00000000">Floor:</th>
            <td>'.(isset($output['floor']) ? $output['floor'] : 'Not alloted').'</td>
          </tr>
          <tr>
            <th style="background-color:#00000000">Flat Number:</th>
            <td>'.(isset($output['flat_no']) ? $output['flat_no'] : 'Not alloted').'</td>
          </tr>
          <tr>
            <th style="background-color:#00000000">Block Name:</th>
            <td>'.(isset($output['block_name']) ? $output['block_name'] : 'Not alloted').'</td>
          </tr>
        </table>
    </div>';

    return $display_output1;
}