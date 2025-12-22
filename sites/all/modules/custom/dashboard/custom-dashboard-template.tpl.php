<?php 
global $user, $user_role,$base_path,$base_root;

?>

<div class="row">
   <div class="col-md-12">
      <div class="counter-box p-3 rounded mb-3 position-relative shadow-sm row">
         <div class="col-md-9">
         <h4 class="title-lg">Welcome to e-Allotment of Rental Housing Estate</h4><br>
         <h6>Name:  <?= $output['user_info']['applicantName'] ?></h6> 
         <h6>Designation: <?= !empty($output['user_info']['applicantDesignation']) ? $output['user_info']['applicantDesignation'] : 'Data not found'; ?></h6>  
         <h6>Office: <?= !empty($output['user_info']['officeName']) ? $output['user_info']['officeName'] : 'Data not found'; ?></h6>
         <h6>Mobile Number: <?= !empty($output['user_info']['mobileNo']) ? $output['user_info']['mobileNo'] : 'Mobile No. not found'; ?></h6>
         <h6>Email: <?= $output['user_info']['email'] ?></h6>
         <?php if(isset($output['user_status']) && $output['user_status'] == 'offer_letter_cancel'){ ?>
                  <p style="color:red;">** Your offer letter has been marked as inactive by the system due to non-acceptance within the 15-day timeline. Please contact your Sub-Divisional Asst. Engineer within 5 days to request an offer letter extension. Otherwise, Your application will be cancelled 15 days after the offer letter becomes inactive. </p> <!-- Done by Subham 09-12-2024 -->
         <?php };
         //replace inactive instead of cancelled y dg 16-06-2025
            if(isset($output['user_status']) && $output['user_status'] == 'license_cancel'){ ?>
            <p style="color:red;">** The license has been Inactive due to the failure to receive the possession letter within 15 days. Please contact your sub-divisonal Exec. Engineer within 5 days to request for license extension otherwise, your application will be considered cancelled.</p>  
         <?php }; ?> <!-- Done by debaleena 27-11-2024 -->
         </div>
         <div class="col-md-3"><img src="<?php echo $base_root.$base_path?>sites/all/themes/housingtheme/images/dashboard-user.jpeg" style="border-radius: 50%;" /></div>
         <!-- <img src="<?php //echo $base_root.$base_path?>sites/all/themes/housingtheme/images/dashboard-user.jpeg" class="position-absolute# end-0# counter-box-icon top-0" /> -->
      </div>
   </div>
   <?php
         // print_r($output);die;
         $output['new-apply'] = isset($output['new-apply']) ? $output['new-apply'] : 0;
         $output['vs'] = isset($output['vs']) ? $output['vs'] : 0;
         $output['cs'] = isset($output['cs']) ? $output['cs'] : 0;

         $output['allotted-apply'] = isset($output['allotted-apply']) ? $output['allotted-apply'] : 0;
         $output['allotted-vs'] = isset($output['allotted-vs']) ? $output['allotted-vs'] : 0;
         $output['allotted-cs'] = isset($output['allotted-cs']) ? $output['allotted-cs'] : 0;
         $output['all-applications'] = isset($output['all-applications']) ? $output['all-applications'] : 0;
         $output['all-license'] = isset($output['all-license']) ? $output['all-license'] : 0;
         $output['all-exsting-occupant'] = isset($output['all-exsting-occupant']) ? $output['all-exsting-occupant'] : 0;
         $output['auto-cancellation'] = isset($output['auto-cancellation']) ? $output['auto-cancellation'] : 0;
         $output['special-recommendation-list-data'] = isset($output['special-recommendation-list-data']) ? $output['special-recommendation-list-data'] : 0;

   if($user_role == 11){ 
       // For DDO 
      $ddo_status = 'applied';
      $allotted_ddo_status = 'applicant_acceptance'; ?> 
   <div class="col-md-12">
      <div class="row">
         <div class="col-md-4">
            <div class="counter-box p-3 rounded mb-3 position-relative shadow-sm color-box1">
               <span class="counter"><?php echo $output['new-apply'];  ?></span>
               <p>New Application</p>
               <a href="<?php echo $base_root.$base_path.'view_application_list/'.encrypt_url($ddo_status).'/'.encrypt_url('new-apply').'/'.'action-list'?>" class="badge rounded-pill text-bg-success">View Details</a>
               <img src="<?php echo $base_root.$base_path?>sites/all/themes/housingtheme/images/icon-allotment.png"
                  class="position-absolute end-0 counter-box-icon top-0 mt-2 me-2" />
            </div>
         </div>
         <div class="col-md-4">
            <div class="counter-box p-3 rounded mb-3 position-relative shadow-sm color-box2">
               <!-- <i class="fa fa-group"></i> -->
               <span class="counter"><?php echo $output['vs'];  ?></span>
               <p>Vertical Shifting Application</p>
               <a href="<?php echo $base_root.$base_path.'view_application_list/'.encrypt_url($ddo_status).'/'.encrypt_url('vs').'/'.'action-list'?>" class="badge rounded-pill text-bg-success">View Details</a>
               <img src="<?php echo $base_root.$base_path?>sites/all/themes/housingtheme//images/icon-floor.png"
                  class="position-absolute end-0 counter-box-icon top-0 mt-2 me-2" />
            </div>
         </div>
         <div class="col-md-4">
            <div class="counter-box p-3 rounded mb-3 position-relative shadow-sm color-box3">
               <!-- <i class="fa fa-group"></i> -->
               <span class="counter"><?php echo $output['cs'];  ?></span>
               <p>Category Shifting Application</p>
               <a href="<?php echo $base_root.$base_path.'view_application_list/'.encrypt_url($ddo_status).'/'.encrypt_url('cs').'/'.'action-list'?>" class="badge rounded-pill text-bg-success">View Details</a>
               <img src="<?php echo $base_root.$base_path?>sites/all/themes/housingtheme//images/icon-category.png"
                  class="position-absolute end-0 counter-box-icon top-0 mt-2 me-2" />
            </div>
         </div>
         <div class="col-md-4">
            <div class="counter-box p-3 rounded mb-3 position-relative shadow-sm color-box1">
               <!-- <i class="fa fa-group"></i> -->
               <span class="counter"><?php echo $output['allotted-apply'] ?></span>
               <p>Allotted New Application</p>
               <a href="<?php echo $base_root.$base_path.'view_application_list/'.encrypt_url($allotted_ddo_status).'/'.encrypt_url('new-apply').'/'.'action-list'?>" class="badge rounded-pill text-bg-success">View Details</a>
               <img src="<?php echo $base_root.$base_path?>sites/all/themes/housingtheme//images/icon-allotment.png"
                  class="position-absolute end-0 counter-box-icon top-0 mt-2 me-2" />
            </div>
         </div>
         <div class="col-md-4">
            <div class="counter-box p-3 rounded mb-3 position-relative shadow-sm color-box2">
               <!-- <i class="fa fa-group"></i> -->
               <span class="counter"><?php echo $output['allotted-vs'] ?></span>
               <p>Allotted VS Application</p>
               <a href="<?php echo $base_root.$base_path.'view_application_list/'.encrypt_url($allotted_ddo_status).'/'.encrypt_url('vs').'/'.'action-list'?>" class="badge rounded-pill text-bg-success">View Details</a>
               <img src="<?php echo $base_root.$base_path?>sites/all/themes/housingtheme//images/icon-floor.png"
                  class="position-absolute end-0 counter-box-icon top-0 mt-2 me-2" />
            </div>
         </div>
         <div class="col-md-4">
            <div class="counter-box p-3 rounded mb-3 position-relative shadow-sm color-box3">
               <!-- <i class="fa fa-group"></i> -->
               <span class="counter"><?php echo $output['allotted-cs'] ?></span>
               <p>Allotted CS Application</p>
               <a href="<?php echo $base_root.$base_path.'view_application_list/'.encrypt_url($allotted_ddo_status).'/'.encrypt_url('cs').'/'.'action-list'?>" class="badge rounded-pill text-bg-success">View Details</a>
               <img src="<?php echo $base_root.$base_path?>sites/all/themes/housingtheme//images/icon-category.png"
                  class="position-absolute end-0 counter-box-icon top-0 mt-2 me-2" />
            </div>
         </div>
      </div>
   </div>
   <?php } ?>
   <?php if($user_role == 10){
      $osd_status = 'ddo_verified_1';
      $allotted_osd_status = 'ddo_verified_2'; // For Housing Supervisor ?> 
   <div class="col-md-12">
      <div class="row">
         <div class="col-md-4">
            <div class="counter-box p-3 rounded mb-3 position-relative shadow-sm color-box1">
               <span class="counter"><?php echo $output['new-apply'];?></span>
               <p>New Application</p>
               <a href="<?php echo $base_root.$base_path.'view_application_list/'.encrypt_url($osd_status).'/'.encrypt_url('new-apply').'/'.'action-list'?>" class="badge rounded-pill text-bg-success">View Details</a>
               <img src="<?php echo $base_root.$base_path?>sites/all/themes/housingtheme/images/icon-allotment.png"
                  class="position-absolute end-0 counter-box-icon top-0 mt-2 me-2" />
            </div>
         </div>
         <div class="col-md-4">
            <div class="counter-box p-3 rounded mb-3 position-relative shadow-sm color-box2">
               <!-- <i class="fa fa-group"></i> -->
               <span class="counter"><?php echo $output['vs'];?></span>
               <p>Vertical Shifting Application</p>
               <a href="<?php echo $base_root.$base_path.'view_application_list/'.encrypt_url($osd_status).'/'.encrypt_url('vs').'/'.'action-list'?>" class="badge rounded-pill text-bg-success">View Details</a>
               <img src="<?php echo $base_root.$base_path?>sites/all/themes/housingtheme//images/icon-floor.png"
                  class="position-absolute end-0 counter-box-icon top-0 mt-2 me-2" />
            </div>
         </div>
         <div class="col-md-4">
            <div class="counter-box p-3 rounded mb-3 position-relative shadow-sm color-box3">
               <!-- <i class="fa fa-group"></i> -->
               <span class="counter"><?php echo $output['cs'];?></span>
               <p>Category Shifting Application</p>
               <a href="<?php echo $base_root.$base_path.'view_application_list/'.encrypt_url($osd_status).'/'.encrypt_url('cs').'/'.'action-list'?>" class="badge rounded-pill text-bg-success">View Details</a>
               <img src="<?php echo $base_root.$base_path?>sites/all/themes/housingtheme//images/icon-category.png"
                  class="position-absolute end-0 counter-box-icon top-0 mt-2 me-2" />
            </div>
         </div>
         <div class="col-md-4">
            <div class="counter-box p-3 rounded mb-3 position-relative shadow-sm color-box1">
               <!-- <i class="fa fa-group"></i> -->
               <span class="counter"><?php echo $output['allotted-apply']; ?></span>
               <p>Allotted New Application</p>
               <a href="<?php echo $base_root.$base_path.'view_application_list/'.encrypt_url($allotted_osd_status).'/'.encrypt_url('new-apply').'/'.'action-list'?>" class="badge rounded-pill text-bg-success">View Details</a>
               <img src="<?php echo $base_root.$base_path?>sites/all/themes/housingtheme//images/icon-allotment.png"
                  class="position-absolute end-0 counter-box-icon top-0 mt-2 me-2" />
            </div>
         </div>
         <div class="col-md-4">
            <div class="counter-box p-3 rounded mb-3 position-relative shadow-sm color-box2">
               <!-- <i class="fa fa-group"></i> -->
               <span class="counter"><?php echo $output['allotted-vs']; ?></span>
               <p>Allotted VS Application</p>
               <a href="<?php echo $base_root.$base_path.'view_application_list/'.encrypt_url($allotted_osd_status).'/'.encrypt_url('vs').'/'.'action-list'?>" class="badge rounded-pill text-bg-success">View Details</a>
               <img src="<?php echo $base_root.$base_path?>sites/all/themes/housingtheme//images/icon-floor.png"
                  class="position-absolute end-0 counter-box-icon top-0 mt-2 me-2" />
            </div>
         </div>
         <div class="col-md-4">
            <div class="counter-box p-3 rounded mb-3 position-relative shadow-sm color-box3">
               <!-- <i class="fa fa-group"></i> -->
               <span class="counter"><?php echo $output['allotted-cs']; ?></span>
               <p>Allotted CS Application</p>
               <a href="<?php echo $base_root.$base_path.'view_application_list/'.encrypt_url($allotted_osd_status).'/'.encrypt_url('cs').'/'.'action-list'?>" class="badge rounded-pill text-bg-success">View Details</a>
               <img src="<?php echo $base_root.$base_path?>sites/all/themes/housingtheme//images/icon-category.png"
                  class="position-absolute end-0 counter-box-icon top-0 mt-2 me-2" />
            </div>
         </div>
      </div>
   </div>
   <?php } ?>
   <?php if($user_role == 7){ // For Sub-divison ?> 
   <!-- </Done by Subham dt. 30-12-2024> -->
   <div class="col-md-12">
      <div class="row">
         <div class="col-md-4 ">
            <div class="counter-box p-3 rounded mb-3 position-relative shadow-sm color-box1 w-100">
               <span class="counter"><?php echo $output['all-exsting-occupant']; ?></span>
               <p>Existing Occupant (with HRMS)</p>
               <a href="<?php echo $base_root.$base_path.'view-occupant-list/'?>" class="badge rounded-pill text-bg-success">View Details</a>
               <img src="<?php echo $base_root.$base_path?>sites/all/themes/housingtheme/images/icon-allotment.png"
                  class="position-absolute end-0 counter-box-icon top-0 mt-2 me-2" />
            </div>
         </div>
         <div class="col-md-4 ">
            <div class="counter-box p-3 rounded mb-3 position-relative shadow-sm color-box1 w-100">
               <span class="counter"><?php echo $output['existing_occupant_data']; ?></span>
               <p>Existing Occupant (without HRMS)</p>
               <a href="<?php echo $base_root.$base_path.'rhewise_occupant_draft_list/'?>" class="badge rounded-pill text-bg-success">View Details</a>
               <img src="<?php echo $base_root.$base_path?>sites/all/themes/housingtheme/images/icon-allotment.png"
                  class="position-absolute end-0 counter-box-icon top-0 mt-2 me-2" />
            </div>
         </div>
         <div class="col-md-4 ">
            <div class="counter-box p-3 rounded mb-3 position-relative shadow-sm color-box2 w-100">
               <span class="counter"><?php echo $output['auto-cancellation']; ?></span>
               <p>Auto Cancelled Offer Letters / Licenses</p>
               <a href="<?php echo $base_root.$base_path.'auto-cancellation-list/'?>" class="badge rounded-pill text-bg-success">View Details</a>
               <img src="<?php echo $base_root.$base_path?>sites/all/themes/housingtheme//images/icon-floor.png"
                  class="position-absolute end-0 counter-box-icon top-0 mt-2 me-2" />
            </div>
         </div>
      </div>
   </div>
   <?php } ?>
   <?php if($user_role == 8){ // For Divison ?> 
   <!-- </Done by Subham dt. 31-12-2024> -->
   <div class="col-md-12">
      <div class="row">
         <div class="col-md-6 ">
            <div class="counter-box p-3 rounded mb-3 position-relative shadow-sm color-box1 w-100">
               <span class="counter"><?php echo $output['all-exsting-occupant']; ?></span>
               <p>Existing Occupant</p>
               <a href="<?php echo $base_root.$base_path.'view-occupant-list/'?>" class="badge rounded-pill text-bg-success">View Details</a>
               <img src="<?php echo $base_root.$base_path?>sites/all/themes/housingtheme/images/icon-allotment.png"
                  class="position-absolute end-0 counter-box-icon top-0 mt-2 me-2" />
            </div>
         </div>
      </div>
   </div>
   <?php } ?>
   <?php if($user_role == 13){
      $approver_status = 'housing_sup_approved_1';
      $allotted_approver_status = 'housing_sup_approved_2';  // For Housing Approver ?> 
   <div class="col-md-12">
      <div class="row">
         <div class="col-md-4">
            <div class="counter-box p-3 rounded mb-3 position-relative shadow-sm color-box1">
               <span class="counter"><?php echo $output['new-apply'] ?></span>
               <p>New Application</p>
               <a href="<?php echo $base_root.$base_path.'view_application_list/'.encrypt_url($approver_status).'/'.encrypt_url('new-apply').'/'.'action-list'?>" class="badge rounded-pill text-bg-success">View Details</a>
               <img src="<?php echo $base_root.$base_path?>sites/all/themes/housingtheme/images/icon-allotment.png"
                  class="position-absolute end-0 counter-box-icon top-0 mt-2 me-2" />
            </div>
         </div>
         <div class="col-md-4">
            <div class="counter-box p-3 rounded mb-3 position-relative shadow-sm color-box2">
               <!-- <i class="fa fa-group"></i> -->
               <span class="counter"><?php echo $output['vs'] ?></span>
               <p>Vertical Shifting Application</p>
               <a href="<?php echo $base_root.$base_path.'view_application_list/'.encrypt_url($approver_status).'/'.encrypt_url('vs').'/'.'action-list'?>" class="badge rounded-pill text-bg-success">View Details</a>
               <img src="<?php echo $base_root.$base_path?>sites/all/themes/housingtheme//images/icon-floor.png"
                  class="position-absolute end-0 counter-box-icon top-0 mt-2 me-2" />
            </div>
         </div>
         <div class="col-md-4">
            <div class="counter-box p-3 rounded mb-3 position-relative shadow-sm color-box3">
               <!-- <i class="fa fa-group"></i> -->
               <span class="counter"><?php echo $output['cs'] ?></span>
               <p>Category Shifting Application</p>
               <a href="<?php echo $base_root.$base_path.'view_application_list/'.encrypt_url($approver_status).'/'.encrypt_url('cs').'/'.'action-list'?>" class="badge rounded-pill text-bg-success">View Details</a>
               <img src="<?php echo $base_root.$base_path?>sites/all/themes/housingtheme//images/icon-category.png"
                  class="position-absolute end-0 counter-box-icon top-0 mt-2 me-2" />
            </div>
         </div>
         <div class="col-md-4">
            <div class="counter-box p-3 rounded mb-3 position-relative shadow-sm color-box1">
               <!-- <i class="fa fa-group"></i> -->
               <span class="counter"><?php echo $output['allotted-apply'] ?></span>
               <p>Allotted New Application</p>
               <a href="<?php echo $base_root.$base_path.'view_application_list/'.encrypt_url($allotted_approver_status).'/'.encrypt_url('new-apply').'/'.'action-list'?>" class="badge rounded-pill text-bg-success">View Details</a>
               <img src="<?php echo $base_root.$base_path?>sites/all/themes/housingtheme//images/icon-allotment.png"
                  class="position-absolute end-0 counter-box-icon top-0 mt-2 me-2" />
            </div>
         </div>
         <div class="col-md-4">
            <div class="counter-box p-3 rounded mb-3 position-relative shadow-sm color-box2">
               <!-- <i class="fa fa-group"></i> -->
               <span class="counter"><?php echo $output['allotted-vs'] ?></span>
               <p>Allotted VS Application</p>
               <a href="<?php echo $base_root.$base_path.'view_application_list/'.encrypt_url($allotted_approver_status).'/'.encrypt_url('vs').'/'.'action-list'?>" class="badge rounded-pill text-bg-success">View Details</a>
               <img src="<?php echo $base_root.$base_path?>sites/all/themes/housingtheme//images/icon-floor.png"
                  class="position-absolute end-0 counter-box-icon top-0 mt-2 me-2" />
            </div>
         </div>
         <div class="col-md-4">
            <div class="counter-box p-3 rounded mb-3 position-relative shadow-sm color-box3">
               <!-- <i class="fa fa-group"></i> -->
               <span class="counter"><?php echo $output['allotted-cs'] ?></span>
               <p>Allotted CS Application</p>
               <a href="<?php echo $base_root.$base_path.'view_application_list/'.encrypt_url($allotted_approver_status).'/'.encrypt_url('cs').'/'.'action-list'?>" class="badge rounded-pill text-bg-success">View Details</a>
               <img src="<?php echo $base_root.$base_path?>sites/all/themes/housingtheme//images/icon-category.png"
                  class="position-absolute end-0 counter-box-icon top-0 mt-2 me-2" />
            </div>
         </div>
      </div>
   </div>
   <?php } ?>
   <?php if($user_role == 6 ){ //For Housing Official ?>
   <div class="col-md-12">
      <div class="row">
         <div class="col-md-4">

            <!-- <TURNED OFF BY Subham dt.19-05-2025 AS THIS SECTION IS FOR HoD LOGIN> -->
            <!-- <div class="row">
               <div class="col-md-12">
                  <div class="counter-box p-3 rounded mb-3 position-relative shadow-sm color-box1"> -->
                     <!-- <i class="fa fa-group"></i> -->
                     <!-- <span class="counter"><?php //echo $output['all-applications']?></span>
                     <p>All Pending Application</p>
                     <a href="<?php //echo $base_root.$base_path.'allotment_list_approve/'?>" class="badge rounded-pill text-bg-success">View Details</a>
                     <img src="<?php //echo $base_root.$base_path?>sites/all/themes/housingtheme//images/icon-allotment.png"
                     class="position-absolute end-0 counter-box-icon top-0 mt-2 me-2" />
                  </div>
               </div>
            </div> -->

            <div class="row">
               <div class="col-md-12">
                  <div class="counter-box p-3 rounded mb-3 position-relative shadow-sm color-box4">
                     <!-- <i class="fa  fa-shopping-cart"></i> -->
                     <span class="counter"><?php echo $output['all-license'] ?></span>
                     <p>Pending License Generation</p>
                     <a href="<?php echo $base_root.$base_path.'generate-license/'?>" class="badge rounded-pill text-bg-success">View Details</a>
                     <img src="<?php echo $base_root.$base_path?>sites/all/themes/housingtheme/images/icon-licence.png"
                     class="position-absolute end-0 counter-box-icon top-0 mt-2 me-2" />
                  </div>
               </div>
            </div>
         </div>
         <div class="col-md-8">
            <div class="row">
               <!-- <h4 class="mt-4">Flat Type Wise Waiting List</h4> -->
               <div class="table-responsive rounded counter-box shadow-sm p-3">
                  <table class="table table-list table-striped table-hover table-bordered">
                     <thead>
                        <tr class="table-primary">
                           <th>Flat Type</th>
                           <th>No. of Waitlisted Applications(For New Allotment)<!--19-09-2025-->
                           <a href="<?php echo $base_root.$base_path.'flat_type_waiting_list/'?>" class="badge rounded-pill text-bg-success">View Details</a>
                           </th>
                        </tr>
                     </thead>
                     <tbody>
                        <tr>
                           <td><b>A</b></td>
                           <?php $data=flat_type_wise_waiting_detail_for_competent_authority(1);?><!--19-09-2025-->
                           <td><?php echo count($data);?></td> 
                        </tr>
                        <tr>
                           <td><b>B</b></td>
                           <?php $data=flat_type_wise_waiting_detail_for_competent_authority(2);?><!--19-09-2025-->
                           <td><?php echo count($data);?></td>  
                        </tr>
                        <tr>
                           <td><b>C</b></td>
                           <?php $data=flat_type_wise_waiting_detail_for_competent_authority(3);?><!--19-09-2025-->
                           <td><?php echo count($data);?></td>   
                        </tr>
                        <tr>
                           <td><b>D</b></td>
                           <?php $data=flat_type_wise_waiting_detail_for_competent_authority(4);?><!--19-09-2025-->
                           <td><?php echo count($data);?></td>   
                        </tr>
                        <tr>
                           <td><b>A+</b></td>
                           <?php $data=flat_type_wise_waiting_detail_for_competent_authority(5);?><!--19-09-2025-->
                           <td><?php echo count($data);?></td>  
                        </tr>
                     </tbody>
               </table>
               </div>
            </div>
         </div>
      </div>
   </div>
   <?php } ?>
   <!-- </Done by Subham dt. 19-05-2025> -->
   <?php if($user_role == 17 ){ //For Head of the Department ?>
   <div class="col-md-12">
      <div class="row">
         <div class="col-md-6 ">
            <div class="counter-box p-3 rounded mb-3 position-relative shadow-sm color-box1 w-100">
               <span class="counter"><?php echo $output['all-applications']?></span>
               <p>All Pending Applications for Approval</p>
               <a href="<?php echo $base_root.$base_path.'allotment_list_approve/'?>" class="badge rounded-pill text-bg-success">View Details</a>
               <img src="<?php echo $base_root.$base_path?>sites/all/themes/housingtheme/images/icon-allotment.png"
                  class="position-absolute end-0 counter-box-icon top-0 mt-2 me-2" />
            </div>
         </div>
         <div class="col-md-6 ">
            <div class="counter-box p-3 rounded mb-3 position-relative shadow-sm color-box2 w-100">
               <span class="counter"><?php echo  $output['special-recommendation-list-data']; ?></span>
               <p>Special Recommendation</p>
               <a href="<?php echo $base_root.$base_path.'special-recommended-list/'?>" class="badge rounded-pill text-bg-success">View Details</a>
               <img src="<?php echo $base_root.$base_path?>sites/all/themes/housingtheme//images/icon-floor.png"
                  class="position-absolute end-0 counter-box-icon top-0 mt-2 me-2" />
            </div>
         </div>
      </div>
   </div>
   <?php } ?>
   <!-- <div class="col-md-3">
      <div class="counter-box p-3 rounded mb-3 position-relative shadow-sm">
         <i class="fa  fa-user"></i>
         <span class="counter">1563</span>
         <p> Floor Shifting Licence</p>
         <a href="#" class="badge rounded-pill text-bg-success">View Details</a>
         <img src="./images/allotment-icon.png" class="position-absolute end-0 counter-box-icon top-0" />
      </div>
   </div>
   <div class="col-md-3">
      <div class="counter-box p-3 rounded mb-3 position-relative shadow-sm">
         <i class="fa  fa-user"></i>
         <span class="counter">1563</span>
         <p>Category Shifting Licence</p>
         <a href="#" class="badge rounded-pill text-bg-success">View Details</a>
         <img src="./images/allotment-icon.png" class="position-absolute end-0 counter-box-icon top-0" />
      </div>
   </div> -->
</div>
<?php
   //foreach ($output['fetch_cuurent_status'] as $data) {
   // print_r($output['fetch_current_status']->short_code);die;
   // $redirect_link = '<a href="'.$base_root.$base_path.'allotment_detail_pdf_gen/'.encrypt_url($output['fetch_current_status']->online_application_id).'/'.$output['fetch_current_status']->allotment_no.'">Click Here</a>';
   // echo $redirect_link;die;

    
      // Done by dg 26-12-2024
      // if(strpos($output['fetch_current_status']->allotment_no, 'NAL') !== false)
      // {
      //    $redirect_link = '<a href="'.$base_root.$base_path.'allotment_detail_pdf_gen/'.encrypt_url($output['fetch_current_status']->online_application_id).'/'.encrypt_url($output['fetch_current_status']->allotment_no).'"> Download Now</a>';

      // }else if(strpos($output['fetch_current_status']->allotment_no, 'VSAL') !== false)
      // {
      //    $redirect_link = '<a href="'.$base_root.$base_path.'vs_allotment_detail_pdf_gen/'.encrypt_url($output['fetch_current_status']->online_application_id).'/'.encrypt_url($output['fetch_current_status']->allotment_no).'"> Download Now</a>';

         
      // }else if(strpos($output['fetch_current_status']->allotment_no, 'CSAL') !== false)
      // {
      //    $redirect_link = '<a href="'.$base_root.$base_path.'cs_allotment_detail_pdf_gen/'.encrypt_url($output['fetch_current_status']->online_application_id).'/'.encrypt_url($output['fetch_current_status']->allotment_no).'"> Download Now</a>';

      // }
      
     
   
if ($user_role == 4 || $user_role == 5){ // For Applicant/Occupant 

     // Done by Subham 26-12-2024
     $allotment_no = isset($output['fetch_current_status']->allotment_no) ? $output['fetch_current_status']->allotment_no : '';

     // Done by Subham 27-12-2024
     $redirect_link = strpos($allotment_no, 'NAL') !== false ? '<a href="'.$base_root.$base_path.'allotment_detail_pdf_gen/'.encrypt_url($output['fetch_current_status']->online_application_id).'/'.encrypt_url($allotment_no).'"> Download Now</a>': (strpos($allotment_no, 'VSAL') !== false ? '<a href="'.$base_root.$base_path.'vs_allotment_detail_pdf_gen/'.encrypt_url($output['fetch_current_status']->online_application_id).'/'.encrypt_url($allotment_no).'"> Download Now</a>': (strpos($allotment_no, 'CSAL') !== false ? '<a href="'.$base_root.$base_path.'cs_allotment_detail_pdf_gen/'.encrypt_url($output['fetch_current_status']->online_application_id).'/'.encrypt_url($allotment_no).'"> Download Now</a>': null));
     //echo $allotment_no;die;


    // Done by Subham 26-12-2024
    $license_no = isset($output['fetch_license_status']->online_application_id) ? $output['fetch_license_status']->online_application_id : '';
   
    // Done by dg 26-12-2024
    $redirect_link_license = '<a href="'.$base_root.$base_path.'download_licence_pdf/'.encrypt_url($license_no).'/"> Download Now</a>';
?>


<!-- <div class="col-md-12"> -->
   <div class="row">
      <h4 class="mt-4">Application List</h4>
      <div class="col-md-9">
         <div class="table-responsive rounded counter-box shadow-sm p-3">
            <?php if (!empty($output['all-application-data'])){  ?>
               <table class="table table-list table-striped table-hover table-bordered">
                  <thead>
                     <tr class="table-primary">
                        <th>Name</th>
                        <th>Application Number</th>
                        <th>Date of Application</th>
                        <th>Status of Application</th>
                        <th>Action</th>
                     </tr>
                  </thead>
                  <tbody>
                     <tr> 
                        <?php foreach ($output['all-application-data'] as $application): // foreach start ?>
                         <tr>
                             <td><b><?= $application->applicant_name ?></b></td>
                             <td><?= $application->application_no ?></td>
                             <td><?= !empty($application->date_of_application) ? date('d-m-Y', strtotime($application->date_of_application)) : 'N/A' ?></td>
                             <td><?= $application->status_description ?></td>

                             <td>
                                 <a href="<?php echo $base_root . $base_path . 'view-application/' . encrypt_url($application->online_application_id); ?>" 
                                    class="btn btn-outline-primary btn-sm px-5 rounded-pill fw-bolder">
                                     View
                                 </a>
                                 <?php if (strpos($application->application_no, 'VS') !== false || strpos($application->application_no, 'CS') !== false):  // condition added by subham dt.03-12-2025?>
                                 <?php else: ?>
                                  <?php   //start added by dg 07-12-2025 //
                                          $result = db_query("SELECT status_id FROM {housing_allotment_status_master} WHERE short_code = :short_code", [':short_code' => $application->hoastatus,]);

                                          $status_id = $result->fetchField();
                                          // end and in below last status_id checking also added//
                                          if ($application->allotment_category != 'General' && $application->extra_doc == ''  && $status_id <= 8): ?>
                                             <br>
                                               <a href="<?php echo $base_root . $base_path . 'supporting-doc-upload/' . encrypt_url($application->online_application_id); ?>" 
                                                  class="btn btn-outline-primary btn-sm px-5 rounded-pill fw-bolder">
                                                   Upload Supporting Documents
                                               </a>
                                     <?php endif; ?>
                                 <?php endif; ?>
                             </td>
                         </tr>
                         <?php endforeach; ?>
                     
                     <?php }else{  // Done by Subham 03-01-2025?>
                        <tr>No Application Data Found</tr> 
                     <?php } ?>
                  </tbody>
               </table>
         </div>
      </div>
      <div class="col-md-3">
         <div class="card h-100 notification-box">
            <div class="card-body">
               <div id="carouselExampleCaptions" class="carousel slide h-100" data-bs-ride="carousel">
                  <div class="carousel-indicators">
                     <button type="button" data-bs-target="#carouselExampleCaptions" data-bs-slide-to="0"
                        class="active" aria-current="true" aria-label="Slide 1"></button>
                     <button type="button" data-bs-target="#carouselExampleCaptions" data-bs-slide-to="1"
                        aria-label="Slide 2"></button>
                  </div>
                  <div class="carousel-inner text-center">
                 
                     <div class="carousel-item active p-3">
                        <img src="<?php echo $base_root.$base_path?>sites/all/themes/housingtheme/images/notification.png" class="w-75" />
                        <!-- <h5>Latest Notification</h5> -->
                        <?php if($allotment_no != ''){?>
                           <p style="color:red; font-family: Arial, sans-serif; font-size: 16px;">1. Your Offer Letter is generated, download from here!</p>
                           <i class="fa fa-download" style="margin-right: 4px; color:blue;"><?php echo $redirect_link; // Done by dg 26-12-2024 ?></i>
                        <?php }else{  ?>
                           <p style="color:red; font-family: Arial, sans-serif; font-size: 16px;">No Latest Notifications</p>
                        <?php } ?>   
                       
                     </div>
                  

                    
                     <div class="carousel-item p-3">
                        <img src="<?php echo $base_root.$base_path?>sites/all/themes/housingtheme/images/notification.png" class="w-75" />
                        <!-- <h5>Latest Notification</h5> -->
                        <?php if($license_no != ''){?> 
                           <p style="color:red; font-family: Arial, sans-serif; font-size: 16px;">2. Your License is generated, download from here!</p><i class="fa fa-download" style="margin-right: 4px; color:blue;"><?php echo $redirect_link_license;  // Done by dg 26-12-2024?></i>
                        <?php }else{ ?>
                           <p style="color:red; font-family: Arial, sans-serif; font-size: 16px;">No Latest Notifications</p>
                        <?php } ?>
                        <!-- <button type="button" class="btn btn-secondary btn-sm rounded-pill">Download
                           List</button> -->
                     </div>
                  
                  </div>
               </div>
            </div>
         </div>
      </div>
   </div>
<!-- </div> -->
<?php } ?>
<div class="row mt-5">
   <!-- <div class="col-md-3">
      <div class="card">
         <img src="<?php// echo $base_root.$base_path?>sites/all/themes/housingtheme/images/bela_housing_3.jpeg" class="card-img-top w-100" alt="Bela Housing Complex">
         <div class="card-body">
            <h5 class="card-title">Card title</h5>
            <p class="card-text text-justify">All the Lorem Ipsum generators on the Internet tend to repeat
               predefined
               chunks as necessary, making this the first true generator on the Internet.</p>
            <a href="#" class="btn btn-primary rounded-pill">Go somewhere</a>
         </div>
      </div>
   </div> -->
   <!-- <div class="col-md-3">
      <div class="card">
         <img src="<?php// echo $base_root.$base_path?>sites/all/themes/housingtheme/images/bela_housing_2.jpeg" class="card-img-top w-100" alt="Bela Housing Complex">
         <div class="card-body">
            <h5 class="card-title">Card title</h5>
            <p class="card-text">All the Lorem Ipsum generators on the Internet tend to repeat predefined
               chunks as necessary, making this the first true generator on the Internet.</p>
            <a href="#" class="btn btn-primary rounded-pill">Go somewhere</a>
         </div>
      </div>
   </div> -->
   <!-- <div class="col-md-3">
      <div class="card">
         <img src="<?php //echo $base_root.$base_path?>sites/all/themes/housingtheme/images/bela_housing_3.jpeg" class="card-img-top w-100" alt="Bela Housing Complex">
         <div class="card-body">
            <h5 class="card-title">Card title</h5>
            <p class="card-text">All the Lorem Ipsum generators on the Internet tend to repeat predefined
               chunks as necessary, making this the first true generator on the Internet.</p>
            <a href="#" class="btn btn-primary rounded-pill">Go somewhere</a>
         </div>
      </div>
   </div> -->
   <!-- <div class="col-md-3">
      <div class="card">
         <img src="<?php //echo $base_root.$base_path?>sites/all/themes/housingtheme/images/bela_housing_2.jpeg" class="card-img-top w-100" alt="Bela Housing Complex">
         <div class="card-body">
            <h5 class="card-title">Card title</h5>
            <p class="card-text">All the Lorem Ipsum generators on the Internet tend to repeat predefined
               chunks as necessary, making this the first true generator on the Internet.</p>
            <a href="#" class="btn btn-primary rounded-pill">Go somewhere</a>
         </div>
      </div>
   </div> -->
</div>
<!-- <hr/> -->
            <!-- <div class="row ftr-content">
               <div class="col-md-7">
                  <p class="copyright">Copyright © All Rights Reserved</p>
               </div>
               <div class="col-md-5 text-end">Designed & Developed by <a href="https://www.nic.in/" target="_blank"><img
                        src="images/nic-logo2.jpg" alt="NIC - National Informatics Centre"
                        title="NIC - National Informatics Centre" width="80"></a></div>
            </div> -->