<?php 
function application_status_check_form($form,&$form_state) {
<<<<<<< HEAD
  global $user_role;

  // echo $user_role; die;
=======
  global $user_role,$user;

  
>>>>>>> 2502ab393f49b61f25516e6bb0502acbb6d447a0

  
 // <div class="row">
  $optionList = array(
    1 => t('Application Number')
  );

  $title_txt = t('Enter Application Number: ');
  if($user_role == 6){
    $optionList[2] = t('HRMS Id');
    $title_txt = t('Enter Application Number or HRMS Id: ');
  }

<<<<<<< HEAD
=======
   $form['user_uid'] = array(
    '#type' => 'hidden',
    '#value' => $user->uid, 
    );

>>>>>>> 2502ab393f49b61f25516e6bb0502acbb6d447a0

  $form['select_button'] = array(
    '#type' => 'radios',
    '#title' => t('Select the option via you want to search application details :'),
    '#options' => $optionList,
    '#default_value' => 1
  );

  $form['application_or_hrms_no'] = array(
    '#type' => 'textfield',
    '#title' => $title_txt,
    '#default_value' => '',
    '#attributes' => array('placeholder' => 'Enter Details', 'class' => array('form-control form-control-sm')),
    '#required' => TRUE,
    '#prefix' => '<br><br><div class="col-md-8"><div class="form-floating">',
    '#suffix' => '</div></div>',
    '#weight' => 2
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Search'),
    '#prefix' => '<div class="col-md-4"><div class="form-floating">',
    '#suffix' => '</div></div>',
    '#attributes' => array('class' => array('text-centre btn bg-primary btn-sm px-5 mt-5 rounded-pill text-white fw-bolder')),
    '#weight' => 3
  );

  return $form;
}



function application_status_check_form_submit($form,&$form_state){

<<<<<<< HEAD
=======
  $user_uid = $form_state['values']['user_uid'];
  // echo $user_uid;die;
>>>>>>> 2502ab393f49b61f25516e6bb0502acbb6d447a0
  $select_button_status = $form_state['values']['select_button'];
 
  if($select_button_status == 1){
    $app_no = isset($form_state['values']['application_or_hrms_no']) && !empty($form_state['values']['application_or_hrms_no']) ? $form_state['values']['application_or_hrms_no'] : '';

    $query = db_select('housing_online_application', 'hoa');
<<<<<<< HEAD
    $query->fields('hoa', ['online_application_id', 'status']);
    $query->condition('hoa.application_no', $app_no);
    $result = $query->execute();
    $result = $result->fetchObject();
=======
    $query->innerJoin('housing_flat_occupant', 'hfo', 'hfo.online_application_id = hoa.online_application_id');
    $query->innerJoin('housing_flat', 'hf', 'hf.flat_id = hfo.flat_id');
    $query->innerJoin('housing_estate', 'he', 'he.estate_id = hf.estate_id');
    $query->innerJoin('housing_subdivision', 'hs', 'hs.subdiv_id = he.subdiv_id');
    $query->innerJoin('housing_division', 'hd', 'hd.division_id = hs.division_id');
    $query->innerJoin('users_details', 'ud', 'ud.subdiv_id = hs.subdiv_id');
    $query->fields('hoa', ['online_application_id', 'status']);
    $query->condition('hoa.application_no', $app_no);
    $query->condition('ud.uid', $user_uid);
    $result = $query->execute();
  // echo "<pre>"; print_r($result);die;
    $result = $result->fetchObject();
// echo "<pre>";print_r($result);die;
>>>>>>> 2502ab393f49b61f25516e6bb0502acbb6d447a0
    if ($result) {
      $app_id = $result->online_application_id;
      $app_status = $result->status;
      drupal_goto('common-application-view/'.encrypt_url($app_id).'/'.encrypt_url($app_status));
    } else {
      drupal_set_message(t('Invalid Application Number, Check Application Number once again.'), 'error');
    }
  }elseif($select_button_status == 2){
    $get_hrms_no = isset($form_state['values']['application_or_hrms_no']) && !empty($form_state['values']['application_or_hrms_no']) && is_numeric($form_state['values']['application_or_hrms_no']) ? $form_state['values']['application_or_hrms_no'] : '';
    if ($get_hrms_no) {
      $query = db_select('housing_applicant_official_detail', 'haod');
      $query->innerJoin('housing_online_application', 'hoa', 'hoa.applicant_official_detail_id = haod.applicant_official_detail_id');
      $query->fields('hoa', ['online_application_id', 'status']);
      $query->condition('haod.hrms_id', $get_hrms_no);
      $result = $query->execute();
      $result = $result->fetchObject();
      $app_id = $result->online_application_id;
      $app_status = $result->status;
      drupal_goto('common-application-view/'.encrypt_url($app_id).'/'.encrypt_url($app_status));
    } else {
      drupal_set_message(t('Invalid HRMS ID, Check HRMS ID once again.'), 'error');
    }
  }
}