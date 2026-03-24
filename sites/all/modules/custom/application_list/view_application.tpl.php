
<?php

$isVal = $output[0];
if($isVal){
  $application_arr = $output[2];
  $entityList = $application_arr[0];
  $fields = $entityList['fields'];
  $expressions = $entityList['expressions'];
  $entityType = $entityList['type'];
  $application_data = $application_arr[1];
  $common_data = $output[1];
  $applicant_data = $output[3];
  $supporting_document = $output[10];
  
  //  echo "<pre>";print_r($applicant_data);exit;

  //debolina start
  $preference_data=$output[5];
<<<<<<< HEAD
  $status_description=$output[4]->status_description;   //by dg 2025
=======
  $status_description=$output[4]->applicant_show_status;   //by dg 26-02-2026
    
>>>>>>> 2502ab393f49b61f25516e6bb0502acbb6d447a0
  $status_short_code = $output[4]->short_code;  //by dg 28-08-2025
  $fetched_existing_occupant_data = $output[6];  //by dg 28-08-2025
   // echo "<pre>";print_r($output);die;
  //debolina end
?>
<div class="table-bottom row">
<div class = "col-md-2" >
  <?php
  $filename = str_replace( ' ', '', $entityType );
  
    //echo l('<img height="18" width="18" src="../'.drupal_get_path('module', 'application_list').'/images/pdf_icon.png" title="Download Application" alt="PDF Icon">Download Application', 'application-pdf-gen/'.encrypt_url($common_data->online_application_id).'/App_Details_'.$common_data->application_no, array('html'=>TRUE, 'attributes' => array('target' => '_blank', 'style' => 'color: #0090C7;font-weight: 400;text-decoration: none; font-size:14px')));
    echo l(' Download Details','application-pdf-gen/'.encrypt_url($common_data->online_application_id).'/App_Details_'.$common_data->application_no,array('html'=>TRUE,'attributes' => array('target' => '_blank','class' => 'btn bg-primary px-6 rounded-pill text-white fw-bolder mb-2 fa fa-download','role'=>'button')));//sd 21-06-2024  
///debaleena 27-08-2024

?>
</div>
<<<<<<< HEAD
<?php

//start added by dg 07-12-2025 //
=======
<?php 
 //start added by dg 07-12-2025 //
>>>>>>> 2502ab393f49b61f25516e6bb0502acbb6d447a0
 $result = db_query("SELECT status_id FROM {housing_allotment_status_master} WHERE short_code = :short_code", [':short_code' => $common_data->status,]);

 $status_id = $result->fetchField();
 $application_no_prifix = substr($common_data->application_no, 0, 2);
 // echo $status_id;die;
 // end and in below last allotment_category and status_id and vs cs checking also added//

if ($supporting_document != ''  && $application_data->allotment_category != 'General' && $status_id <= 8 && !in_array($application_no_prifix, array('VS', 'CS'))
) {
?>
<<<<<<< HEAD
<div class = "col-md-4"><?= l(' Download Supporting Document',$supporting_document,array('html'=>TRUE,'attributes' => array('target' => '_blank','class' => 'btn bg-primary px-6 rounded-pill text-white fw-bolder mb-2 fa fa-download','role'=>'button'))) ?></div>
<?php
=======
  <div class = "col-md-4"><?= l(' Download Supporting Document',$supporting_document,array('html'=>TRUE,'attributes' => array('target' => '_blank','class' => 'btn bg-primary px-6 rounded-pill text-white fw-bolder mb-2 fa fa-download','role'=>'button'))) ?></div>
  <?php
>>>>>>> 2502ab393f49b61f25516e6bb0502acbb6d447a0
}
?>

<div class = "col-md-2" >
   <?php
  if($common_data->status == 'license_generate' || $common_data->status == 'flat_possession_taken'){ //add flat_possession_taken by dg 22-07-2025
    echo l(' Download License','download_licence_pdf/'.encrypt_url($common_data->online_application_id).'/',array('html'=>TRUE,'attributes' => array('target' => '_blank','class' => 'btn bg-primary px-6 rounded-pill text-white fw-bolder mb-2 fa fa-download')));
  }

  ///end///
    
  ?>
  </div>
           
</div>


<!--Appication display theme new copy-->
<div class="table-bottom">
  <table class="table table-list"><!-- sd 21-06-2024-->
  <tr>
  <th colspan="2" style="background: none repeat scroll 0 0 #473a39;color:white;text-align: center;font-size: 18px;line-height: 24px;font-weight: normal;font-family: 'Dosis',Arial,Verdana,serif;" class="first">Application Status</th>
  </tr>
    <tr>
      <th style="background-color:#00000000" width="50%">Application Type</th>
      <?php if((trim($entityType) == '' || trim($entityType) != '') && isset($status_short_code) && $status_short_code == 'existing_occupant'){?>
      <td width="50%"><?php echo 'Application for Existing Occupant';?></td>
      <?php }else{ ?>
      <td width="50%"><?php echo 'Application for '.$entityType;?></td>
     <?php  } ?>
    </tr>
    <tr>
      <th style="background-color:#00000000" width="50%">Application No.</th>
      <td width="50%"><?php echo $common_data->application_no;?></td>
    </tr>
    <tr>
      <th style="background-color:#00000000">Date of Application</th>
      <td><?php echo implode('/', array_reverse(explode('-', $common_data->date_of_application)));?></td>
    </tr>
    <tr>
      <th style="background-color:#00000000">Application Status</th>
      <!-- <td><?php //echo ($common_data->status == 'allotted') ? 'Allotted Offer' : $common_data->status;?></td> -->
      <!-- debolina start-->
      <td>
        <?php if(isset($status_description)){
        echo $status_description;
        }
        else{
          echo 'NA'; 
        }?>
      </td>
      <!-- debolina end-->
    </tr>
    <!-- <tr>
      <th style="background-color:#00000000">Date of Action</th>
      <td><?php //echo implode('/', array_reverse(explode('-', $common_data->date_of_verified)));?></td>
    </tr> -->

<?php
   $headArr = array();
   $valArr = array();
   $l = 0;
   //echo "<pre>";print_r($application_data);exit;
  // print_r($fields);
    if(isset($expressions) && count($expressions) != 0){
    foreach($expressions as $entityAlias => $columnHeadFieldSet ){
      foreach($columnHeadFieldSet as $columnHead => $expression){
        $headArr[$l] = $columnHead;
        //$valArr[$l] = $application_data->$expression[1];
        $valArr[$l] = $application_data;
        $l++;
     }}} 
      if(isset($fields) &&count($fields) != 0){
    foreach($fields as $entityAlias => $columnHeadFieldSet ){
      foreach($columnHeadFieldSet as $columnHead => $field){

        $headArr[$l] = $columnHead;
        //$valArr[$l] = $application_data->$field[1];
        $valArr[$l] = $application_data;
        $l++;
   }}} 
   $l = 0;
  // echo "<pre>";print_r($entityType);die;
  if(trim($entityType) == 'New Allotment') {

    if(isset($status_short_code) &&  $status_short_code == 'existing_occupant' ){
      $heading = 'Possession Details';
    }else{
      $heading = 'Information for Allotment';
    }
    
  }
  else if(trim($entityType) == 'Vertical Shifting' || trim($entityType) == 'Category Shifting') { 
    $heading = 'Possession Details';
  }
  else if(trim($entityType) == 'New Licence' || trim($entityType) == 'VS Licence' || trim($entityType) == 'CS Licence' || trim($entityType) == 'Renew Licence') {
    $heading = 'Allotment Details'; 
  }else if(trim($entityType) == '' && isset($status_short_code) && $status_short_code == 'existing_occupant'){
    $heading = 'Possession Details';
  }
    
?>

<tr>
  <th colspan="2" style="background: none repeat scroll 0 0 #473a39;color:white;text-align: center;font-size: 18px;line-height: 24px;font-weight: normal;font-family: 'Dosis',Arial,Verdana,serif;" class="first">Applicant Personal Information(According to Service Book)</th>
  </tr>
    <tr>
      <th style="background-color:#00000000">Applicant's Name</th>
      <td ><?php echo !empty($applicant_data->applicant_name) ? $applicant_data->applicant_name : 'N/A'; ?></td>
    </tr>
    <tr>
      <th style="background-color:#00000000">Father's / Husband's Name</th>
      <td><?php echo !empty($applicant_data->guardian_name) ? $applicant_data->guardian_name : 'N/A'; ?></td>
    </tr>
    
    <?php
      // Fetch district names
      $result_permanent = particular_district_list($applicant_data->permanent_district);
      $data_permanent = $result_permanent->fetchObject();
      $permanent_district_name = $data_permanent->district_name ?? '';

      $result_present = particular_district_list($applicant_data->present_district);
      $data_present = $result_present->fetchObject();
      $present_district_name = $data_present->district_name ?? '';

      // Permanent address
      $permanent_address_parts = array_filter([
        $applicant_data->permanent_street ?? '',
        $applicant_data->permanent_city_town_village ?? '',
        !empty($applicant_data->permanent_post_office) ? 'P.O- ' . $applicant_data->permanent_post_office : '',
        $permanent_district_name,
        !empty($applicant_data->permanent_pincode) ? '-' . $applicant_data->permanent_pincode : '',
      ]);

      $permanent_address = !empty($permanent_address_parts) ? implode(', ', $permanent_address_parts) : 'Not Available';

      // Present address
      if ($applicant_data->permanent_present_same == 1) {
        $present_address = $permanent_address;
      } else {
        $present_address_parts = array_filter([
          $applicant_data->present_street ?? '',
          $applicant_data->present_city_town_village ?? '',
          !empty($applicant_data->present_post_office) ? 'P.O- ' . $applicant_data->present_post_office : '',
          $present_district_name,
          !empty($applicant_data->present_pincode) ? '-' . $applicant_data->present_pincode : '',
        ]);

        $present_address = !empty($present_address_parts) ? implode(', ', $present_address_parts) : 'Not Available';
      }
    ?>
    
    <tr>
      <th style="background-color:#00000000">Permanent Address</th>
      <td><?php echo $permanent_address;?></td>
    </tr>
    <tr>
      <th style="background-color:#00000000">Present Address</th>
      <td><?php echo $present_address;?></td>
    </tr>
    <tr>
      <th style="background-color:#00000000">Gender</th>
      <td><?php echo $applicant_data->gender == 'M'?'Male':'Female';?></td>
    </tr>
    <tr>
      <th style="background-color:#00000000">Date of Birth(According to Service Book)</th>
      <td><?php echo implode('/', array_reverse(explode('-', $applicant_data->date_of_birth)));?></td>
    </tr>
    <tr>
      <th style="background-color:#00000000">Mobile No</th>
      <td><?php echo !empty($applicant_data->mobile_no) ? $applicant_data->mobile_no : 'N/A'; ?></td>
    </tr>
    <tr>
      <th style="background-color:#00000000">Email ID</th>
      <td><?php echo $applicant_data->mail ;?></td>
    </tr>
<tr>
  <th colspan="2" style="background: none repeat scroll 0 0 #473a39;color:white;text-align: center;font-size: 18px;line-height: 24px;font-weight: normal;font-family: 'Dosis',Arial,Verdana,serif;" class="first">Applicant Official Information</th>
  </tr>
    <tr>
      <th style="background-color:#00000000">Employee HRMS ID</th>
      <td ><?php echo $common_data->hrms_id;?></td>
    </tr>
    <tr>
      <th style="background-color:#00000000">Designation</th>
      <td ><?php echo !empty($common_data->hrms_id) ? $common_data->hrms_id : 'N/A'; ?></td>
    </tr>
    <tr>
      <th style="background-color:#00000000">Office Headquarter</th>
      <td><?php echo !empty($applicant_data->applicant_headquarter) ? $applicant_data->applicant_headquarter : 'N/A'; ?></td>
    </tr>
    <tr>
      <th style="background-color:#00000000">Place of Posting</th>
      <td><?php echo !empty($common_data->applicant_posting_place) ? $common_data->applicant_posting_place : 'N/A'; ?></td>
    </tr>
    <tr>
      <th style="background-color:#00000000">Pay Band</th>
      <?php
             if($common_data->scale_from == 0 && $common_data->scale_to != 0){
                //$str =  $common_data->payband.' (Below Rs '.$common_data->scale_to.'/-)';
                $str =  $common_data->flat_type.' (Below Rs '.$common_data->scale_to.'/-)';
            }else if($common_data->scale_from != 0 && $common_data->scale_to == 0){
                //$str =  $common_data->payband.' (Rs '.$common_data->scale_from.'/- and above)';
                $str =  $common_data->flat_type.' (Rs '.$common_data->scale_from.'/- and above)';
            }else{
              //$str = $common_data->payband.' (Rs '.$common_data->scale_from.'/- to Rs '.$common_data->scale_to.'/-)';
              $str = $common_data->flat_type.' (Rs '.$common_data->scale_from.'/- to Rs '.$common_data->scale_to.'/-)';
            }?>
      <td><?php echo !empty($str) ? $str : 'N/A';?></td>
    </tr>
    <!-- <tr>
      <th style="background-color:#00000000">Pay in Pay Band</th>
      <td><?php //echo $common_data->pay_in_the_pay_band;?></td>
    </tr> -->
    <!-- <tr>
      <th style="background-color:#00000000">Grade Pay</th>
      <td><?php //echo $common_data->grade_pay;?></td>
    </tr> -->
    <tr>
      <th style="background-color:#00000000">Basic Pay</th>
      <td><?php echo !empty($common_data->pay_in_the_pay_band) ? $common_data->pay_in_the_pay_band : 'N/A';?></td>
    </tr>
    
   <?php /*?> <tr>
      <th style="background-color:#00000000">GPF No.</th>
      <td><?php echo $common_data->gpf_no;?></td>
    </tr><?php */?>
    
    <tr>
      <th style="background-color:#00000000">Date of Joining</th>
      <td><?php echo !empty($common_data->date_of_joining) ? implode('/', array_reverse(explode('-', $common_data->date_of_joining))) : 'N/A';
?></td>
    </tr>
    <tr>
      <th style="background-color:#00000000">Date of Retirement(According to Service Book)</th>
      <td><?php echo !empty($common_data->date_of_retirement) ? implode('/', array_reverse(explode('-', $common_data->date_of_retirement))) : 'N/A';?></td>
    </tr>
  <tr>
  <th colspan="2" style="background: none repeat scroll 0 0 #473a39;color:white;text-align: center;font-size: 18px;line-height: 24px;font-weight: normal;font-family: 'Dosis',Arial,Verdana,serif;" class="first">Applicant Office Name and Address</th>
  </tr>
    <tr>
      <th style="background-color:#00000000">Name of the Office</th>
      <td><?php echo $common_data->office_name;?></td>
    </tr>
    
    <?php
    $result_office = particular_district_list($common_data->office_district);
    $data_office = $result_office->fetchObject();
    
    $office_address = $common_data->office_street.', P.O - '.$common_data->office_post_office.', '.$common_data->office_city_town_village.', '.$data_office->district_name.' - '.$common_data->office_pin_code;
  ?>
    
    <tr>
      <th style="background-color:#00000000">Office Address</th>
      <td><?php echo $office_address;?></td>
    </tr>
    <tr>
      <th style="background-color:#00000000">Office Phone No.</th>
      <td><?php echo $common_data->office_phone_no;?></td>
    </tr>
    <tr>
      <th colspan="2" style="background: none repeat scroll 0 0 #473a39;color:white;text-align: center;font-size: 18px;line-height: 24px;font-weight: normal;font-family: 'Dosis',Arial,Verdana,serif;" class="first">Applicant DDO Information</th>
    </tr>
    <tr>
      <th style="background-color:#00000000">DDO District</th>
      <td><?php echo $common_data->district_name;?></td>
    </tr>
    <tr>
      <th style="background-color:#00000000">DDO Designation</th>
      <td><?php echo $common_data->ddo_designation;?></td>
    </tr>
    <tr>
      <th style="background-color:#00000000">DDO Address</th>
      <td><?php echo $common_data->ddo_address;?></td>
    </tr>


    <!----debolina start housing preferences are displayed---->
    <?php 
    if($preference_data != NULL){

     ?>
    <tr>
      <th colspan="2" style="background: none repeat scroll 0 0 #473a39;color:white;text-align: center;font-size: 18px;line-height: 24px;font-weight: normal;font-family: 'Dosis',Arial,Verdana,serif;" class="first">Applicant's Housing Estate Preference</th>
    </tr>
    <tr>
      <th style="background-color:#00000000">First Preference</th>
      <td><?php echo $preference_data[0]->estate_name;?></td>
    </tr>
    <tr>
      <th style="background-color:#00000000">Second Preference</th>
      <td><?php echo $preference_data[1]->estate_name;?></td>
    </tr>
    <tr>
      <th style="background-color:#00000000">Third Preference</th>
      <td><?php echo $preference_data[2]->estate_name;?></td>
    </tr>
    <tr>
      <th style="background-color:#00000000">Fourth Preference</th>   <?php //added by dg 28-08-2025 ?>
      <td><?php echo $preference_data[3]->estate_name;?></td>
    </tr>
    <tr>
      <th style="background-color:#00000000">Fifth Preference</th>  <?php  //added by dg 28-08-2025 ?>
      <td><?php echo $preference_data[4]->estate_name;?></td>
    </tr>
<?php } ?>
    <!---------end-------->
    
    <tr>
    <th colspan="2" style="background: none repeat scroll 0 0 #473a39;color:white;text-align: center;font-size: 18px;line-height: 24px;font-weight: normal;font-family: 'Dosis',Arial,Verdana,serif;" class="first"><?php echo $heading;?></th>
</tr>


    <?php 
//echo "<pre>";print_r($application_data);
    if(trim($entityType) == 'New Allotment') 
      {
        if(isset($status_short_code) &&  $status_short_code == 'existing_occupant' ){
          // echo "<pre>";print_r($headArr);die;
           ?>
        <tr>
            <th style="background-color:#00000000">Possession Date</th>
            <td ><?php
             echo !empty($fetched_existing_occupant_data->possession_date) ? $fetched_existing_occupant_data->possession_date : 'Not Available';
          ?></td>
        </tr>
        <tr>
          <th style="background-color:#00000000">Estate Name</th>
            <td ><?php 
                echo $fetched_existing_occupant_data->estate_name;
          ?></td>
        </tr>

        <tr>
          <th style="background-color:#00000000">Block Name</th>
            <td ><?php 
                echo $fetched_existing_occupant_data->block_name;
          ?></td>
        </tr>
        <tr>
          <th style="background-color:#00000000">Flat No.</th>
            <td ><?php 
                echo $fetched_existing_occupant_data->flat_no;
          ?></td>
        </tr>

      <?php 
        }else{
           ?>
        <tr>
          <th style="background-color:#00000000"><?php echo $headArr[0];?></th>
          <td ><?php 
              echo $application_data->allotment_category;
        ?></td>
      </tr>
      <tr>
        <th style="background-color:#00000000"><?php echo $headArr[1];?></th>
          <td ><?php 
              echo $application_data->flat_type;
        ?></td>
      </tr>

        <?php 
        }
     
      }else if(trim($entityType) == 'Vertical Shifting'|| trim($entityType) == 'Category Shifting')
      {
         ?>
        <tr>
            <th style="background-color:#00000000"><?php echo $headArr[0];?></th>
            <td ><?php 
                echo $application_data->possession_date;
          ?></td>
        </tr>
        <tr>
          <th style="background-color:#00000000"><?php echo $headArr[1];?></th>
            <td ><?php 
                echo $application_data->estate_name;
          ?></td>
        </tr>

        <tr>
          <th style="background-color:#00000000"><?php echo $headArr[2];?></th>
            <td ><?php 
                echo $application_data->block_name;
          ?></td>
        </tr>
        <tr>
          <th style="background-color:#00000000"><?php echo $headArr[3];?></th>
            <td ><?php 
                echo $application_data->flat_no;
          ?></td>
        </tr>

      <?php 

      }else if(trim($entityType) == '' && isset($status_short_code) && $status_short_code == 'existing_occupant'){
         ?>
        <tr>
            <th style="background-color:#00000000">Possession Date</th>
            <td ><?php
             echo !empty($fetched_existing_occupant_data->possession_date) ? $fetched_existing_occupant_data->possession_date : 'Not Available';
          ?></td>
        </tr>
        <tr>
          <th style="background-color:#00000000">Estate Name</th>
            <td ><?php 
                echo $fetched_existing_occupant_data->estate_name;
          ?></td>
        </tr>

        <tr>
          <th style="background-color:#00000000">Block Name</th>
            <td ><?php 
                echo $fetched_existing_occupant_data->block_name;
          ?></td>
        </tr>
        <tr>
          <th style="background-color:#00000000">Flat No.</th>
            <td ><?php 
                echo $fetched_existing_occupant_data->flat_no;
          ?></td>
        </tr>

      <?php 
      }else{

        while($l < count($headArr)){?>
        <tr>
          <th style="background-color:#00000000"><?php echo $headArr[$l];?></th>
          <td ><?php echo $valArr[$l];?></td>
        </tr>

        <?php $l++; }$l = 0;

      } ?>

  </table>
</div>
<!--End-->

<?php }else { ?>

  Invalid Access

<?php } ?>