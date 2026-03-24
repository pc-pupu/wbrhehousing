<?php

function download_and_upload($form, &$form_state, $online_application_id = ''){
    global $base_path, $user;
    

    $query_get_status = db_query('select status from housing_online_application where online_application_id = '.decrypt_url($online_application_id).'')->fetchObject();


//    if($query_get_status->status == 'applicant_acceptance_incomplete'){

         // Add a new item to the bottom of the form.
    

    $form['online_application_id'] = array(
        '#type' => 'hidden',
        '#value' => $online_application_id
    );


    $data = (object)getHRMSUserData($user->name);
<<<<<<< HEAD

=======
    // print_r($data);die;
>>>>>>> 2502ab393f49b61f25516e6bb0502acbb6d447a0
    $content = '
	<div>
		<h3 align="center" style="font-size:20px"><b>DECLARATION BEFORE COMPETENT AUTHORITY</b></h3>
		<p align="left">I,Sri/Smt. <b>'.$data->applicantName.'</b>   son/daughter/wife of Sri/Smt. <b>'.$data->guardianName.'</b>, resident at <b>'.$data->permanentStreet.', '.$data->permanentCityTownVillage.', '.$data->permanentPostOffice.' , '.$data->permanentDistrictCode.','.$data->permanentPincode.' </b>present working as <b>'.$data->applicantDesignation.'</b> in the office/ Department of <b>'.$data->applicantPostingPlace.'</b> hereby declare and affirm.</p>
		<ol>
			<li>That I have no house/flat in my name, in my spouse’s name or in name of and dependent member of my family within Kolkata Municipal Corporation area or Howrah Municipal Corporation area or within 20KM from Rental Housing Estate chosen by me.</li>
			<li>That I shall use the flat for the purpose for which it has been allotted to me and shall not use it for any other purpose.</li>
			<li>That I shall not let any other person to reside in flat issued in favour of me or keep it vacant for more than six months.</li>
			<li>I shall not assign or transfer the premises in any way to any person or put any person in procession of the premises. </li>
			<li>I shall not add to, or alter, any fixtures of the premises or make any structural alteration in the flat without the express permission in writing of the Competent Authority.</li>
			<li>I shall not cause, or suffer to be caused, any damage to the premises beyond the normal wear and tear through the proper use and occupation of the premises.</li>
			<li>I shall allow any offer duly authorized in this behalf by the Competent Authority to inspect the flat as when necessary.</li>
			<li>I shall pay such licence fee for the flat as may be determined from time to time by the Competent Authority.</li>
			<li> I shall inform the Competent Authority in Writing about every change in my post on place of posting and every change in my position by reason of going to leave or being placed under suspension or by any other reason within a week of the change taking place.</li>
			<li> That further I do hereby undertake that I shall not violate any of the terms and conditions as laid down in the license issued in my favour. If I violate any of the terms and conditions of license the Competent Authority shall be free to terminate my license.</li>
			<li> I further I do hereby undertake that upon the expiry of term license of upon termination or license. I shall delivered vacant possession of the flat to the Competent Authority or to any other person authorized by the Competent Authority in this behalf, in the same condition in which I took possession of the premises. </li>
			<li>	I further undertake that I will vacate the flat within 30 days from the date of my transfer (beyond 20 kms. From the concerned RHE)/ retirement/ resignation or dismissal/ removal from service. I shall be liable to pay whatever occupational charge will be fixed for the said period.</li>
		</ol> 
		<p align="left">Mobile No. <b>+91 '.$data->mobileNo.'</b> </p>
		
		<p align="left">Date: '.date('d/m/Y').'</p>		
		
	</div>';


    $form['declaration_content_show'] = array(
        '#type' => 'markup',
        '#markup' => $content,
    );

    $contentArr = array(
        'applicantName' => $data->applicantName,
        'guardianName' => $data->guardianName,
        'permanentStreet' => $data->permanentStreet,
        'permanentCityTownVillage' => $data->permanentCityTownVillage,
        'permanentPostOffice' => $data ->permanentPostOffice,
        'permanentDistrictCode' => $data->permanentDistrictCode,
        'permanentPincode' => $data->permanentPincode,
        'applicantDesignation' => $data->applicantDesignation,    
    );

    $form['declaration_content'] = array(
        '#type' => 'hidden',
        '#value' => serialize($contentArr),
    );

    $form['accept_btn'] = array(
        '#type' => 'checkbox',
        '#title' => t('I accept the terms and conditions stated in the Declaration before Competent Authority.'),
        '#required' => TRUE,
    );

    $form['submt_btn'] = array(
        '#type' => 'submit',
        '#value' => t('Submit'),
        '#attributes' => array('class' => array('btn', 'btn-primary')),
    );

    $form['exit_btn'] = array(
        '#markup' => l( t('Exit'), 'dashboard', array('attributes' => array('class' => array('btn', 'btn-danger', 'ps-3')))
        ),
    );


    // }
    return $form;
        
}

function download_and_upload_element_process($element, &$form_state, $form) {
    $element = file_managed_file_process($element, $form_state, $form);
    $element['upload_button']['#access'] = FALSE;
    return $element;
}


function download_and_upload_submit($form, &$form_state) {
    global $user;

    $val = $form_state['values'];

    $online_application_id = $val['online_application_id'];
        
    if($val['accept_btn'] == 1){
        // check for already exist the update else insert
        $check_declration = db_query('select 1 from housing_declration where online_application_id = '.decrypt_url($online_application_id).'')->fetchObject();
        $arrins = array(
            'uid' => $user->uid,
            'online_application_id' => decrypt_url($online_application_id),
            'declration_content' => $val['declaration_content'],
            'declration_date' => date('Y-m-d H:i:s'),            
        );
        if(!empty($check_declration)){
            db_update('housing_declration')->fields($arrins)->condition('online_application_id', decrypt_url($online_application_id), '=')->execute();
        }else{ 
            $arrins['created_at'] = date('Y-m-d H:i:s');
            db_insert('housing_declration')->fields($arrins)->execute();
        }

        /////code added by debaleena 25-11-2024////
        // db_update('housing_flat_occupant')
		// 	->fields(array('accept_reject_status' => 'Accept'))  
		// 	->condition('online_application_id', decrypt_url($online_application_id), '=')
		// 	->execute();

		// db_update('housing_online_application')
		// 	->fields(array('status' => 'applicant_acceptance'))  
		// 	->condition('online_application_id', decrypt_url($online_application_id), '=')
		// 	->execute();
        // $status_id = get_status_id('applicant_acceptance');
		// $ins_arr = array(
        //     'online_application_id'=> decrypt_url($online_application_id),
        //     'status_id' => $status_id,
        //     'created_at' => date('Y-m-d H:i:s'),
        //     'uid' => $user->uid,
        //     'short_code' => 'applicant_acceptance'   
		// );

<<<<<<< HEAD
		// db_insert('housing_process_flow')->fields($ins_arr)->execute();
=======
		//db_insert('housing_process_flow')->fields($ins_arr)->execute();
>>>>>>> 2502ab393f49b61f25516e6bb0502acbb6d447a0

        // drupal_set_message(t('You have accepted the terms and conditions stated in the Declaration before Competent Authority.'));
        drupal_goto('ddo-change/'.$online_application_id);

        // code revamped by Moumita 28-08-2025

    }else{
        drupal_set_message(t('Please accept the terms and conditions stated in the Declaration before Competent Authority.'), 'error');
        drupal_goto('download-and-upload/'.$online_application_id);
    }

    
}


