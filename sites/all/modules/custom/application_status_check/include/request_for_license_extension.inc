<?php // Done by Subham 06-12-2024
function request_for_license_extension_form($form,&$form_state, $app_id = '', $app_status = '', $uid = '', $off_detail_id = '') {
  global $user;

  $enc_app_id = decrypt_url($app_id);
  $enc_app_status = decrypt_url($app_status);
  $enc_uid = decrypt_url($uid);
  $enc_off_detail_id = decrypt_url($off_detail_id);

  $resonArr = array(
      '' => t('- Select -'),
      'medical_purpose' => t('Medical Purpose'),
      'headquarter_leave' => t('Headquarter Leave'),
      'renovation_work' => t('Renovation Work'),
      'pending_electrical_work' => t('Pending Electrical Work'), // Added by Subham dt.23-03-2026
      'others' => t('Others'),
  );

  // get data from housing_license_offer_letter_extension
  $getQuery = db_select('housing_license_offer_letter_extension', 'hlole')
                ->fields('hlole', array('reason_for_extension', 'reason_for_extension_other','doc_fid','extended_upto','status'))
                ->condition('online_application_id', $enc_app_id, '=');
  $excute = $getQuery->execute();
  // $result = $excute->fetchAll();
  // echo '<pre>'; print_r($result); echo '</pre>'; // For debugging purpose only, should be removed later

  if($excute->rowCount() > 0){
    $output = '<table class="table table-bordered">
                <thead>
                  <tr>
                    <th>Reason for Extension</th>
                    <th>Supporting Document</th>
                    <th>Extended Upto</th>
                    <th>Status</th>
                  </tr>
                </thead>
                <tbody>';
    foreach($excute->fetchAll() as $record){
      $existing_reason = ucwords(str_replace('_',' ',$record->reason_for_extension));
      $existing_other_reason = $record->reason_for_extension_other;
      $existing_doc_fid = $record->doc_fid;
      $existing_extended_upto = date('d/m/Y',strtotime($record->extended_upto));
      $status = ucwords(str_replace('_',' ',$record->status));

      // Load file details
      $file = file_load($existing_doc_fid);
      // Condition added by Subham dt.23-03-2026
      if (!empty($existing_doc_fid) && ($file = file_load($existing_doc_fid))) {
        $file_url = file_create_url($file->uri);
        $file_link = l('<i class="fa fa-file-pdf-o"></i> View', $file_url, array('attributes' => array('target' => '_blank', 'class' => 'btn btn-sm btn-info'), 'html' => TRUE));
      } else {
        $file_link = '<span style="font-size: 0.8em; color: #666;">File not available to download</span>';
      }
      $output .= '<tr>
                    <td>' . check_plain($existing_reason) .'<br>'. check_plain($existing_other_reason) . ' </td>
                    <td>' . $file_link . '</td>
                    <td>' . check_plain($existing_extended_upto) . '</td>
                    <td>' . $status . ' </td>
                  </tr>';
    }
    $output .= '</tbody></table>';
  }

  if(isset($output)){
    $form['existing_extension_details'] = array(
      '#type' => 'markup',
      '#markup' => '<div class="mb-4"><h4>Existing License Extension Details:</h4>' . $output . '</div>',
    );
  }



  // Edited by Subham 08-01-2026
  $reason = isset($form_state['values']['license_extension_reason_dropdown']) ? $form_state['values']['license_extension_reason_dropdown'] : '';

  $form['license_extension_reason_dropdown'] = array(
    '#title' => t('Reason for License Extension'),
    '#type' => 'select',
    '#options' => $resonArr,
    '#required' => TRUE,
    '#default_value' => $reason,
    '#attributes' => array('class' => array('form-select')),
    '#ajax' => array(
      'callback' => '_reload_reason_view',
      'wrapper' => 'other-reason-wrapper',
      'event' => 'change',
    ),
    '#prefix' => '<div class="row"><div class="col-md-4"><div class="form-floating">',
		'#suffix' => '</div></div>'
  );

  $form['other_reason'] = array(
    '#type' => 'container',
    '#attributes' => array('id' => 'other-reason-wrapper','class' => array('col-md-4')),
  );
      
  if ($reason === 'others') {
    $form['other_reason']['other_reason_text'] = array(
      '#title' => t('Give Other Reason'),
      '#type' => 'textarea',
      '#required' => TRUE,
      '#attributes' => array('class' => array('form-control')),
    );
  }
  // End

  $form['add_license_extension_date'] = array(
		'#title' => t('Enter Extension Date: '),
		'#type' => 'textfield',
		'#attributes' => array('id' => 'edit-extension-license', 'class' => array('numeric_positive form-control')),  //id change by dg 07-12-2025
		'#default_value' => '',
		'#required' => TRUE,
		'#prefix' => '<div class="row"><div class="col-md-4"><div class="form-floating">',
		'#suffix' => '</div></div>'
	);

  $form['license_extension_reason_file'] = array(
    '#title' => t('Upload valid reason for license extension: '),
    '#type' => 'managed_file',
    '#description' => t('<b>Allowed Extension: pdf<br>Maximum File Size: 2 MB</b>'),
    '#progress_message' => t('Please wait...'),
    '#progress_indicator' => 'bar',
    '#default_value' => variable_get('Upload File', ''),
    '#upload_validators' => array(
                   'file_validate_extensions' => array("pdf"),
                   'file_validated_size' => array(2*1024*1024),
                ),
    '#upload_location' => 'public://doc/'.$user->uid,
    // '#required' => TRUE,   // turned off required validation for file upload as per discussion with housing dept. on 19-03-2026
    '#process' => array('license_upload_element_process')
  );


    $form['app_id'] = array(
        '#title' => t('App_id: '),
        '#type' => 'hidden',
        '#default_value' => $enc_app_id,
    );

    $form['official_detail_id'] = array(
        '#title' => t('Off_detail_id: '),
        '#type' => 'hidden',
        '#default_value' => $enc_off_detail_id,
    );

    $form['uid'] = array(
        '#title' => t('Uid: '),
        '#type' => 'hidden',
        '#default_value' => $enc_uid,
    );

    $form['app_status'] = array(
		'#title' => t('App_sts: '),
		'#type' => 'hidden',
		'#default_value' => $enc_app_status
	);

    if($record->status != 'license_extention_request'){
        $form['submit'] = array(
            '#type' => 'submit',
            '#value' => t('Forward to Division for Approval'),
            '#prefix' => '<div class="col-md-4"><div class="form-floating">',
            '#suffix' => '</div></div></div>',
            '#attributes' => array('class' => array('text-centre btn bg-primary btn-sm px-5 mt-5 rounded-pill text-white fw-bolder')),
        );
    }

    
  
  return $form;
}

function _reload_reason_view($form, &$form_state) { //added by Subham 08-01-2026
  return $form['other_reason'];
}

function license_upload_element_process($element, &$form_state, $form) {
  $element = file_managed_file_process($element, $form_state, $form);
  $element['upload_button']['#access'] = FALSE;
  return $element;
}

function request_for_license_extension_form_submit($form, &$form_state){
  global $user, $user_role;


  $official_detail_id = $form_state['values']['official_detail_id'];
  $license_extension_reason = $form_state['values']['license_extension_reason_dropdown'];
  $other_reason_text = $form_state['values']['other_reason_text'];  // Added by Subham 08-01-2026
  $extension_date = $form_state['values']['add_license_extension_date'];
  $app_id = $form_state['values']['app_id'];
  $uid = $form_state['values']['uid'];
  $app_status = $form_state['values']['app_status'];

  if($user_role == 7){
    // sub division workflow
    $fid = NULL;  // Initialize $fid to NULL to handle cases where no file is uploaded, added by Subham dt.23-03-2026

    if (!empty($form_state['values']['license_extension_reason_file'])) {
      $file = file_load($form_state['values']['license_extension_reason_file']);
      $dynamic_folder = 'public://doc/'. $user->uid;
      file_prepare_directory($dynamic_folder, FILE_CREATE_DIRECTORY | FILE_MODIFY_PERMISSIONS);
      $file_name = time().$file->filename;
      $destination = $dynamic_folder . '/' .$file_name ;
      $file = file_copy($file, $destination, FILE_EXISTS_REPLACE);
      $file->status = FILE_STATUS_PERMANENT;
      file_save($file);
      $fid = $file->fid;
    }

    $status_id = get_status_id('license_extended');

    if ($app_status == 'license_extended' || $app_status == 'license_generate') {  //added license_generate by dg 07-12-2025

        $display_type_of_extension = 'License';
    }
    $insert_housing_license_offer_letter_extension = array(
                                  'uid' => $uid,
                                  'online_application_id' => $app_id,
                                  'status' => 'license_extention_request',
                                  'created_date' => date('Y-m-d H:i:s'),
                                  'extended_upto' => trim(implode('-', array_reverse(explode('/', $extension_date)))),
                                  'type_of_extension' => $display_type_of_extension,
                                  'reason_for_extension' => $license_extension_reason,
                                  'doc_fid' => $fid,
                                  'departmental_uid' => $user->uid
    );

    if ($license_extension_reason == 'others' && $other_reason_text !== '') { // Added by Subham 08-01-2026
      $insert_housing_license_offer_letter_extension['reason_for_extension_other'] = $other_reason_text;
    }

    db_insert('housing_license_offer_letter_extension')
    ->fields($insert_housing_license_offer_letter_extension)
    ->execute();

    $update_housing_flat_occupant = array(
                    'cancellation_extension_status' => 'license_extended',
                    'cancellation_extension_date' => date('Y-m-d H:i:s')
    );
    db_update('housing_flat_occupant')
    ->fields($update_housing_flat_occupant)
    ->condition('online_application_id', $app_id, '=')
    ->execute();
    
    drupal_set_message(t('The request is pending division approval.'));
    drupal_goto('common-application-view/'.encrypt_url($app_id).'/'.encrypt_url($app_status));

  }else if($user_role == 8){ /** code is modified due to Division approval, modified by Moumita on 20-01-2026 */
    // division workflow 

    db_update('housing_license_offer_letter_extension')->fields(array('status' => 'license_extended'))->condition('online_application_id', $app_id, '=')->execute();

    $status_updation_arr  = array();
    $status_updation_arr['status']  =  'license_extended';
    $status_updation_arr['date_of_verified']  =  date('Y-m-d H:i:s');

    db_update('housing_online_application')
    ->fields($status_updation_arr)
    ->condition('online_application_id', $app_id, '=')
    ->execute();
    
    $app_off_detail_arr_status_update['is_active']  =  '1';
    // echo $official_detail_id;die;
    db_update('housing_applicant_official_detail')
    ->fields($app_off_detail_arr_status_update)
    ->condition('applicant_official_detail_id', $official_detail_id, '=')
    ->execute();

    $insert_process_flow = array(
                                  'online_application_id' => $app_id,
                                  'status_id' => $status_id,
                                  'created_at' => date("Y-m-d H:i:s"),
                                  'uid' => $user->uid,
                                  'short_code' => 'license_extended'
    );

    db_insert('housing_process_flow')
    ->fields($insert_process_flow)
    ->execute();
    drupal_set_message(t('License has been Extended!'));
    drupal_goto('common-application-view/'.encrypt_url($app_id).'/'.encrypt_url($app_status));

  }else{
    // other user
    drupal_set_message(t('You are not authorized to perform this action.'), 'error');
    drupal_goto('dashboard');
  }
  
  
}