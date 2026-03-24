<?php
function cms_content_edit_form($form, &$form_state, $housing_cms_id_enc = '') {
    $housing_cms_id = decrypt_url($housing_cms_id_enc);
    $query = db_select('housing_cms', 'hc')
        ->fields('hc', array())
        ->condition('housing_cms_id', $housing_cms_id)
        ->execute();
    $result = $query->fetchAssoc();

    if(!empty($result)){
        // print_r($result['date_of_notification']);die;
        $form['cms_info'] = array(
        '#prefix' => '<div class="row">',
        '#suffix' => '</div>',
        );

        $form['cms_info']['edited_content_type'] = array(
            '#title' => t('Content Type'),
            '#type' => 'select',
            '#options' => array(
                'faq' => t('FAQ'),
                'about_us' => t('About Us'),
                'contact_us' => t('Contact Us'),
                'what_is_new' => t('What\'s New'),
                'notice' => t('Notice'),
            ),
            '#default_value' => $result['content_type'],
            '#required' => TRUE,
            '#prefix' => '<div class="col-md-4"><div class="form-floating">',
            '#suffix' => '</div></div>'
        );

        $form['cms_info']['edited_link_title'] = array(
            '#title' => t('Link Title'),
            '#type' => 'textfield',
            '#prefix' => '<div class="col-md-4"><div class="form-floating">',
            '#suffix' => '</div></div>',
            '#default_value' => '',
            '#attributes' => array('class'=>array('form-control')),
            // '#required' => TRUE,
            '#default_value' => $result['link_title']
        );

        $form['cms_info']['edited_content_title'] = array(
            '#title' => t('Content Title'),
            '#type' => 'textfield',
            '#prefix' => '<div class="col-md-4"><div class="form-floating">',
            '#suffix' => '</div></div>',
            '#default_value' => '',
            '#attributes' => array('oninput'=>"this.value=this.value.toUpperCase()", 'class'=>array('form-control')),
            '#required' => TRUE,
            '#default_value' => $result['content_title']
        );

        $form['cms_info']['edited_content_description'] = array(
            '#title' => t('Content Description'),
            '#type' => 'textarea',
            '#prefix' => '<div class="col-md-12"><div class="form-floating">',
            '#suffix' => '</div></div>',
            '#required' => TRUE,
            '#default_value' => $result['content_description']
        );

        $form['cms_info']['edited_order_no'] = array(
            '#title' => t('Order No.'),
            '#type' => 'textfield',
            '#element_validate' => array('element_validate_numeric_positive'),
            '#prefix' => '<div class="col-md-4"><div class="form-floating">',
            '#suffix' => '</div></div>',
            '#attributes' => array('class' => array('numeric_positive','form-control')),
            '#disabled' => TRUE,
            '#required' => TRUE,
            '#default_value' => $result['order_no']
        );

        $form['cms_info']['edited_meta_keyword_text_box'] = array(
            '#title' => t('Meta Keyword'),
            '#type' => 'textfield',
            '#prefix' => '<div class="col-md-4"><div class="form-floating">',
            '#suffix' => '</div></div>',
            '#default_value' => '',
            '#attributes' => array('class'=>array('form-control')),
            '#default_value' => $result['meta_keyword']
        );

        $date = $result['date_of_notification'];
        $formatted_date = date('d/m/Y', strtotime($date));

        $form['cms_info']['edited_don'] = array(
            '#title' => t('Date of Notification'),
            '#type' => 'textfield',
            '#attributes' => array('readonly' => 'readonly', 'id' => 'edit-don','class'=>array('form-control')),
            '#required' => TRUE,
            '#validated' => TRUE,
            '#prefix' => '<div class="col-md-4"><div class="form-floating">',
            '#suffix' => '</div></div>',
            '#element_validate' => array('element_validate_date'),
            '#default_value' => $formatted_date
        );

        $form['edited_meta_desc_text'] = array(
            '#title' => t('Meta Description'),
            '#type' => 'textarea',
            '#prefix' => '<div class="col-md-12"><div class="form-floating">',
            '#suffix' => '</div></div>',
            '#default_value' => $result['meta_description']
        );

        $form['edited_is_active_radio'] = array(
            '#title' => t('Content Activation Status'),
            '#type' => 'radios',
            '#options' => array(
                1 => t('Active'),
                0 => t('Inactive')
            ),
            '#prefix' => '<div class="row"><div class="col-md-4"><div class="form-floating">',
            '#suffix' => '</div></div></div>',
            '#required' => TRUE,
            '#default_value' => $result['is_active']
        );

        $form['edited_is_new_chk_box'] = array(
            '#type' => 'checkbox',
            '#title' => t('<b>Is New</b>'),
            '#prefix' => '<div class="row"><div class="col-md-2"><div class="form-check">',
            '#suffix' => '</div></div></div>',
            '#attributes' => array('class' => array('form-check-input')),
            '#default_value' => $result['is_new']
        );

        $form['edited_content_file_upload'] = array(
            '#title' => t('Upload Content File'),
            '#type' => 'file',
            '#description' => t('<b>Allowed File Type: .pdf<br>Maximum File Size: 1 MB</b>'),
            '#element_validate' => array('cms_content_validate')
        );

        $form['hidden_textbx_cms_id'] = array(
            '#type' => 'hidden',
            '#default_value' => $housing_cms_id
        );

        $form['submit_cen'] = array(
            '#prefix' => '<div class="row"><div class="col-md-12 text-center">',
            '#suffix' => '</div></div>'
        );

        $form['submit_cen']['submit'] = array(
            '#type' => 'submit',
            '#value' => t('Save'),
            '#attributes' => array('class' => array('btn bg-primary btn-sm px-5 rounded-pill text-white fw-bolder')),
            '#submit' => array('cms_content_edit_submit')
        );

        return $form;
    }
}

function cms_content_edit_submit($from, &$form_state) {
    $values = $form_state['values'];
    $form_data = array();

    $content_title = trim($values['edited_content_title']);
    $url = str_replace(' ', '_', strtolower($content_title));

    $content_type = trim($values['edited_content_type']);

    $form_data['content_type']              =  $content_type;
    $form_data['content_title']             =  $content_title;
    $form_data['link_title']                =  trim($values['edited_link_title']);
    $form_data['order_no']                  =  trim($values['edited_order_no']);
    $form_data['meta_keyword']              =  trim($values['edited_meta_keyword_text_box']);
    $form_data['meta_description']          =  trim($values['edited_meta_desc_text']);
    $form_data['date_of_notification']      =  trim(implode('-', array_reverse(explode('/', $values['edited_don']))));
    $form_data['created_date']              =  date('Y-m-d H:i:s');
    $form_data['content_description']       =  trim($values['edited_content_description']);
    $form_data['is_active']                 =  trim($values['edited_is_active_radio']);
    $form_data['is_new']                    =  trim($values['edited_is_new_chk_box']);
    $form_data['url']                       =  $url;

    /*File Upload Handling without Drupal 7's default file managed table with content type-based directories*/
    if (!empty($_FILES['files']['name']['edited_content_file_upload'])) {
        $file = $_FILES['files'];
        $file_name = $file['name']['edited_content_file_upload'];
        $file_tmp = $file['tmp_name']['edited_content_file_upload'];

        $directory_name = str_replace(' ', '_', strtolower($content_type));
        $directory = 'public://doc/' . $directory_name . '/';

        file_prepare_directory($directory, FILE_CREATE_DIRECTORY | FILE_MODIFY_PERMISSIONS);

        $ext = pathinfo($file_name, PATHINFO_EXTENSION);
        $base_name = pathinfo($file_name, PATHINFO_FILENAME);
        $timestamp = date('dmY_His'); // Create timestamp string
        $new_file_name = $base_name . '_' . $timestamp . '.' . $ext; // Concatenate base name, timestamp and file extension

        $safe_name = preg_replace('/[^a-zA-Z0-9_\-\.]/', '_', $new_file_name);
        $destination = $directory . $safe_name;
        $real_path = drupal_realpath($destination);

        if (move_uploaded_file($file_tmp, $real_path)) {
            $form_data['file_name'] = $file_name;
            $form_data['file_path'] = $destination;
        } else {
            drupal_set_message(t('File upload failed.'), 'error');
            return;
        }
    }

    db_update('housing_cms')
        ->fields($form_data)
        ->condition('housing_cms_id', $values['hidden_textbx_cms_id'])
        ->execute();
    drupal_set_message(t('Content edited successfully.'));
    drupal_goto('cms-content-list');
}