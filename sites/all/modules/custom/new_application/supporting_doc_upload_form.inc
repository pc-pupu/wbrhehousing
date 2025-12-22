<?php // Done by Subham dt.24-09-2025
function supporting_doc_upload_form($form, &$form_state, $online_application_id = ''){
    $form = array();

    $form['text-markup'] = array(
        '#type' => 'markup',
        '#markup' => '<div class="alert alert-info" role="alert">
                        <h4 class="alert-heading">Instructions to Upload Supporting Document</h4>
                        <p>You must upload the supporting document because your allotment category is within "Recommended or Single Earning Lady or Legal Heir or Physically Handicapped or Serious Illness or Transfer". If you do not upload it, your allotment may be rejected and you will not be able to access your dashboard.</p> 
                     </div>',
    );

    // $form[''] = array(
    //     '#title' => t('Upload Content File'),
    //     '#type' => 'file',
    //     '#description' => t('<b>Allowed File Type: .pdf<br>Maximum File Size: 1 MB</b>'),
    //     '#element_validate' => array('supporting_doc_validate')
    // );

    $form['supporting_doc_file'] = array(
        '#title' => t('Upload Declaration Signed Form'),
        '#type' => 'managed_file',
        '#description' => t('<b>Allowed Extension: pdf<br>Maximum File Size: 1 MB</b>') ,
        '#progress_message' => t('Please wait...'),
        '#progress_indicator' => 'bar',
        '#default_value' => variable_get('Upload File', ''),
        '#upload_validators' => array(
                                        'file_validate_extensions' => array("pdf"),
                                        // 'file_validated_size' => array(MAX_FILE_SIZE*1024*1024),
                                        'file_validated_size' => array(1*1024*1024),
                                     ),
        '#upload_location' => 'public://doc/extra_doc/',
        '#required' => TRUE,
        '#process' => array('download_and_upload_element_process_new'),
    );

    $form['hidden_online_application_id'] = array(
        '#type' =>'hidden',
        '#value' => $online_application_id,
        '#attributes' => array('readonly' => 'readonly'),
    );

    $form['submit'] = array(
        '#type' => 'submit',
        '#value' => t('Upload'),
        '#attributes' => array('class' => array('btn bg-primary btn-sm px-5 rounded-pill text-white fw-bolder')),
        '#submit' => array('supporting_doc_upload_submit')
    );

    return $form;
}

function download_and_upload_element_process_new($element, &$form_state, $form) {
    $element = file_managed_file_process($element, $form_state, $form);
    $element['upload_button']['#access'] = FALSE;
    return $element;
}

function supporting_doc_upload_submit($form, &$form_state) {
  global $user;

  $online_application_id = decrypt_url($form_state['values']['hidden_online_application_id']);


  if (!empty($form_state['values']['supporting_doc_file'])) {
        // Load the file object.
        $file = file_load($form_state['values']['supporting_doc_file']);
        $dynamic_folder = 'public://doc/extra_doc';
        file_prepare_directory($dynamic_folder, FILE_CREATE_DIRECTORY | FILE_MODIFY_PERMISSIONS);
        $file_name = time().$file->filename;
        $destination = $dynamic_folder . '/' .$file_name ;
        $file = file_copy($file, $destination, FILE_EXISTS_REPLACE);
        $file->status = FILE_STATUS_PERMANENT;
        file_save($file);

        db_update('housing_new_allotment_application')
        ->fields(array(
          'extra_doc' => $file->fid,
          'extra_doc_path' => $file->uri,
        ))
        ->condition('online_application_id', $online_application_id, '=')
        ->execute();

        drupal_set_message(t('Supporting Document uploaded successfully.'));
        drupal_goto('dashboard');
        
    }else{
        drupal_set_message(t('No file was uploaded.'), 'error');
    }
}
