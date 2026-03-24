<?php


function allow_new_application($form, &$form_state, $online_application_id = 0) {
    
    $form = array();
    $form['remarks'] = array(
        '#type' => 'textarea',
        '#title' => t('Provide the reason for allowing new application'),
        '#required' => TRUE,
        '#attributes' => array(
            'placeholder' => t('Provide the reason for allowing new application'),
            'class' => array('form-control')
        ),
        '#prefix' => '<div class="col-md-6">',
        '#suffix' => '</div>',
    );

    $form['online_application_id'] = array(
        '#type' => 'hidden',
        '#value' => $online_application_id,
    );

    $form['actions']['#type'] = 'actions';
    $form['actions']['submit'] = array(
        '#type' => 'submit',
        '#value' => t('Submit'),
        '#attributes' => array('class' => array('btn btn-primary')),
    );
    return $form;
}

function allow_new_application_submit($form, &$form_state){
    global $user;
    $online_application_id = isset($form_state['values']['online_application_id']) ? decrypt_url($form_state['values']['online_application_id']) : 0;
    
    if($online_application_id != 0){

        // get infor from online application table and official detail table for log entry
        $query = db_select('housing_online_application', 'hoa');
        $query->innerJoin('housing_applicant_official_detail', 'haod', 'haod.applicant_official_detail_id = hoa.applicant_official_detail_id');
        $query->fields('hoa', ['online_application_id','status', 'application_no', 'computer_serial_no', 'physical_application_no']);
        $query->fields('haod', ['uid','hrms_id']);
        $query->condition('hoa.online_application_id', $online_application_id, '=');   
        $result = $query->execute();
        $application_data = $result->fetchObject();
        // echo '<pre>'; print_r($application_data); die;

        db_insert('housing_allow_new_application_log')
        ->fields(array(
            'hrms_id' => $application_data->hrms_id,
            'application_no' => $application_data->application_no,
            'computer_serial_no' => $application_data->computer_serial_no,
            'physical_application_no' => $application_data->physical_application_no,
            'status' => $application_data->status, // last status before allowing new application
            'online_application_id' => $online_application_id,
            'remarks' => $remarks,
            'done_by_uid' => $user->uid,
            'created_at' => date('Y-m-d H:i:s'),
        ))
        ->execute();

        // Update the status of the existing application to 'allowed_new_application'
        db_update('housing_online_application')
        ->fields(array(
            'status' => 'allow_new_application',
            'date_of_verified' => date('Y-m-d H:i:s'),
            'remarks' => $remarks,
        ))
        ->condition('online_application_id', $online_application_id, '=')
        ->execute();


        drupal_set_message(t('New application has been allowed successfully.'), 'status');
        drupal_goto('application_status_check');
    }
    else{
        drupal_set_message(t('Invalid application. Please try again.'), 'error');
    }
}