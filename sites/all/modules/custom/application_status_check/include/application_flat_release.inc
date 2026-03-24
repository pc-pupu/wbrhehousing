<?php 
function add_release_date_form($form,&$form_state, $app_id = '', $app_status = '') {

  $enc_app_id = decrypt_url($app_id);
  $enc_app_status = decrypt_url($app_status);
  // echo $enc_app_id;die;
  $form['add_release_date'] = array(
		'#title' => t('Enter Flat Release Date :'),
		'#type' => 'textfield',
		'#attributes' => array('id' => 'edit-release', 'class' => array('numeric_positive form-control')),
		'#default_value' => '',
		'#required' => TRUE,
		'#prefix' => '<div class="row"><div class="col-md-6"><div class="form-floating">',
		'#suffix' => '</div></div></div>',
	);

  $form['subdiv_remarks'] = array(
      '#title' => t('Remarks :'),
      '#type' => 'textarea',
      '#required' => TRUE,
      '#attributes' => array('class' => array('form-control')),
      '#prefix' => '<div class="row"><div class="col-md-6"><div class="form-floating">',
      '#suffix' => '</div></div></div>',
  );

  $form['app_id'] = array(
		'#type' => 'hidden',
		 '#default_value' => $enc_app_id,
	);

  $form['app_status'] = array(
		'#type' => 'hidden',
		'#default_value' => $enc_app_status,
	);

  $form['electric_bill_file_upload'] = array(
      '#title' => t('Upload Electric Bill'),
      '#type' => 'file',
      // '#required' => TRUE,
      '#description' => t('<b>Allowed File Type: .pdf<br>Maximum File Size: 1 MB</b>'),
      '#element_validate' => array('cms_content_validate')
  );

  $form['rent_deduction_file_upload'] = array(
      '#title' => t('Upload Rent Deduction File'),
      '#type' => 'file',
      // '#required' => TRUE,
      '#description' => t('<b>Allowed File Type: .pdf<br>Maximum File Size: 1 MB</b>'),
      '#element_validate' => array('cms_content_validate')
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
    '#prefix' => '<div class="col-md-4"><div class="form-floating">',
    '#suffix' => '</div></div>',
    '#attributes' => array('class' => array('text-centre btn bg-primary btn-sm px-5 mt-5 rounded-pill text-white fw-bolder')),
    '#weight' => 3
  );

  return $form;
}


function add_release_date_form_submit($form, &$form_state){
  global $user;
 
  $release_date = $form_state['values']['add_release_date'];
  $app_id = $form_state['values']['app_id'];
  $app_status = $form_state['values']['app_status'];
  $remarks = $form_state['values']['subdiv_remarks'];  //added by dg 21-01-2026
  $electric_bill = $form_state['values']['electric_bill_file_upload'];  // added by Subham dt.23-03-2026
  $rent_deduction = $form_state['values']['rent_deduction_file_upload'];  // added by Subham dt.23-03-2026

  $date_object = DateTime::createFromFormat('d/m/Y', $release_date);
  $release_date = $date_object->format('Y-m-d');

  $enc_app_id = encrypt_url($app_id);
  $enc_app_status = encrypt_url($app_status);
  echo $electric_bill;die;                        // work half done by subham dt.23-03-2026 for file upload in release date form

  // Electric Bill Upload done by Subham dt.23-03-2026
  if (!empty($_FILES['files']['name']['electric_bill_file_upload'])) {
      $file = $_FILES['files'];
      $file_name = $file['name']['electric_bill_file_upload'];
      $file_tmp = $file['tmp_name']['electric_bill_file_upload'];

      $directory = 'public://doc/electric_bill/';

      file_prepare_directory($directory, FILE_CREATE_DIRECTORY | FILE_MODIFY_PERMISSIONS);

      $safe_name = preg_replace('/[^a-zA-Z0-9_\-\.]/', '_', $file_name);
      $destination = $directory . $safe_name;
      $real_path = drupal_realpath($destination);

      if (move_uploaded_file($file_tmp, $real_path)) {
          $form_data['electric_bill_file_name'] = $file_name;
          $form_data['electric_bill_file_path'] = $destination;
      } else {
          drupal_set_message(t('Electric bill upload failed.'), 'error');
          return;
      }
  }

  // Rent Deduction Upload done by Subham dt.23-03-2026
  if (!empty($_FILES['files']['name']['rent_deduction_file_upload'])) {
      $file = $_FILES['files'];
      $file_name = $file['name']['rent_deduction_file_upload'];
      $file_tmp = $file['tmp_name']['rent_deduction_file_upload'];

      $directory = 'public://doc/rent_deduction/';

      file_prepare_directory($directory, FILE_CREATE_DIRECTORY | FILE_MODIFY_PERMISSIONS);

      $safe_name = preg_replace('/[^a-zA-Z0-9_\-\.]/', '_', $file_name);
      $destination = $directory . $safe_name;
      $real_path = drupal_realpath($destination);

      if (move_uploaded_file($file_tmp, $real_path)) {
          $form_data['rent_file_name'] = $file_name;
          $form_data['rent_file_path'] = $destination;
      } else {
          drupal_set_message(t('Rent deduction file upload failed.'), 'error');
          return;
      }
  }

  $query = db_select('housing_occupant_license', 'hol');
  $query->innerJoin('housing_license_application', 'hla', 'hla.license_application_id = hol.license_application_id');
  $query->innerJoin('housing_flat_occupant', 'hfo', 'hfo.flat_occupant_id = hol.flat_occupant_id');
  $query->addField('hol', 'occupant_license_id');

  
  $query->condition('hla.online_application_id',$app_id,'=');

  $result = $query->execute();
  $data = $result->fetchObject();
  
  $occupant_license_id = $data->occupant_license_id;
  

  $release_date_arr['release_date']  = $release_date;

  db_update('housing_occupant_license')
  ->fields($release_date_arr)
  ->condition('occupant_license_id', $occupant_license_id)
  ->execute();

  db_update('housing_online_application')
    ->fields(array(
      'status' => 'flat_released',
      'date_of_verified' => date('Y-m-d H:i:s'),
      )
    )
  ->condition('online_application_id',$app_id, '=')
  ->execute();

  $query1 = db_select('housing_allotment_status_master', 'hasm');
  $query1->addField('hasm', 'status_id');
  
  $query1->condition('hasm.short_code','flat_released','=');

  $result1 = $query1->execute();
  $data1 = $result1->fetchObject();

  $possession_date_process_flow_arr['online_application_id']  = $app_id;
  $possession_date_process_flow_arr['status_id']              = $data1->status_id;
  $possession_date_process_flow_arr['created_at']             = date("Y-m-d H:i:s");
  $possession_date_process_flow_arr['uid']                    = $user->uid;
  $possession_date_process_flow_arr['short_code']             = 'flat_released';
  $possession_date_process_flow_arr['remarks']                = $remarks;  //added by dg 21-01-2026

  db_insert('housing_process_flow')
  ->fields($possession_date_process_flow_arr)
  ->execute();

  $query2 = db_select('housing_applicant_official_detail','haod');
  $query2->innerjoin('housing_online_application ', 'hoa', 'hoa.applicant_official_detail_id = haod.applicant_official_detail_id');
  $query2->leftjoin('housing_flat_occupant ', 'hfo', 'hfo.online_application_id = hoa.online_application_id');
  $query2->addField('haod', 'applicant_official_detail_id');
  $query2->addField('hfo', 'flat_id');
  $query2->condition('hoa.online_application_id', $app_id, '=');
  $result2 = $query2->execute();
  $data2 = $result2->fetchObject();
  db_update('housing_applicant_official_detail')
    ->fields(array(
      'is_active' => 0,
      )
    )
    ->condition('applicant_official_detail_id',$data2->applicant_official_detail_id, '=')
    ->execute();
  
  db_update('housing_flat')         ///vacant due to release
    ->fields(array(
      'flat_status_id' => 9,
      )
    )
  ->condition('flat_id', $data2->flat_id, '=')
  ->execute();


  
  drupal_set_message(t('Release Date Added Successfully from Subdivision.'));
  drupal_goto('add-release-date'.'/'.$enc_app_id.'/'.$enc_app_status);
}