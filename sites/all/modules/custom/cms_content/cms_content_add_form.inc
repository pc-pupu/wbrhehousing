<?php
// Done by Subham dt.05-06-2025
function cms_content_add_form($form, &$form_state) {
    $count = db_query("SELECT COUNT(*) FROM {housing_cms}")->fetchField();
    $order_no = $count + 1;

    $form['cms_info'] = array(
        '#prefix' => '<div class="row">',
        '#suffix' => '</div>',
    );

    $form['cms_info']['content_type'] = array(
        '#title' => t('Content Type'),
        '#type' => 'select',
        '#options' => array(
                'faq' => t('FAQ'),
                'about_us' => t('About Us'),
                'contact_us' => t('Contact Us'),
                'what_is_new' => t('What\'s New'),
                'notice' => t('Notice'),
        ),
        '#required' => TRUE,
        '#prefix' => '<div class="row"><div class="col-md-4"><div class="form-floating">',
        '#suffix' => '</div></div>'
    );

    $form['cms_info']['link_title'] = array(
        '#title' => t('Link Title'),
        '#type' => 'textfield',
        '#prefix' => '<div class="col-md-4"><div class="form-floating">',
        '#suffix' => '</div></div>',
        '#default_value' => '',
        '#attributes' => array('class'=>array('form-control')),
        // '#required' => TRUE
    );

    $form['cms_info']['content_title'] = array(
        '#title' => t('Content Title'),
        '#type' => 'textfield',
        '#prefix' => '<div class="col-md-4"><div class="form-floating">',
        '#suffix' => '</div></div>',
        '#default_value' => '',
        '#attributes' => array('oninput'=>"this.value=this.value.toUpperCase()", 'class'=>array('form-control')),
        '#required' => TRUE
    );

    $form['cms_info']['content_description'] = array(
        '#title' => t('Content Description'),
        '#type' => 'textarea',
        '#prefix' => '<div class="col-md-12"><div class="form-floating">',
        '#suffix' => '</div></div>',
        '#default_value' => '',
        '#required' => TRUE
    );

    $form['cms_info']['order_no'] = array(
        '#title' => t('Order No.'),
        '#type' => 'textfield',
        '#element_validate' => array('element_validate_numeric_positive'),
        '#prefix' => '<div class="col-md-4"><div class="form-floating">',
        '#suffix' => '</div></div>',
        '#default_value' => $order_no,
        '#attributes' => array('class' => array('numeric_positive','form-control')),
        '#disabled' => TRUE,
        '#required' => TRUE
    );

    $form['cms_info']['don'] = array(
        '#title' => t('Date of Notification'),
        '#type' => 'textfield',
        '#attributes' => array('readonly' => 'readonly', 'id' => 'edit-don','class'=>array('form-control')),
        '#required' => TRUE,
        '#validated' => TRUE,
        '#prefix' => '<div class="col-md-4"><div class="form-floating">',
        '#suffix' => '</div></div>',
        '#element_validate' => array('element_validate_date')
    );

    $form['cms_info']['meta_keyword_text_box'] = array(
        '#title' => t('Meta Keyword'),
        '#type' => 'textfield',
        '#prefix' => '<div class="col-md-4"><div class="form-floating">',
        '#suffix' => '</div></div>',
        '#default_value' => '',
        '#attributes' => array('class'=>array('form-control')),
    );

    $form['meta_desc_text'] = array(
        '#title' => t('Meta Description'),
        '#type' => 'textarea',
        '#prefix' => '<div class="col-md-12"><div class="form-floating">',
        '#suffix' => '</div></div>'
    );

    $form['is_active_radio'] = array(
        '#title' => t('Content Activation Status'),
        '#type' => 'radios',
        '#options' => array(
            1 => t('Active'),
            0 => t('Inactive')
        ),
        '#prefix' => '<div class="row"><div class="col-md-4"><div class="form-floating">',
        '#suffix' => '</div></div></div>',
        '#required' => TRUE,
        '#default_value' => '1'
    );

    $form['is_new_chk_box'] = array(
        '#type' => 'checkbox',
        '#title' => t('<b>Is New</b>'),
        '#prefix' => '<div class="row"><div class="col-md-2"><div class="form-check">',
        '#suffix' => '</div></div></div>',
        '#attributes' => array('class' => array('form-check-input'))
    );

    $form['content_file_upload'] = array(
        '#title' => t('Upload Content File'),
        '#type' => 'file',
        '#description' => t('<b>Allowed File Type: .pdf<br>Maximum File Size: 1 MB</b>'),
        '#element_validate' => array('cms_content_validate')
    );

    $form['submit_cen'] = array(
        '#prefix' => '<div class="row"><div class="col-md-12 text-center">',
        '#suffix' => '</div></div>'
    );

    $form['submit_cen']['submit'] = array(
        '#type' => 'submit',
        '#value' => t('Save'),
        '#attributes' => array('class' => array('btn bg-primary btn-sm px-5 rounded-pill text-white fw-bolder')),
        '#submit' => array('cms_content_add_submit')
    );

    return $form;
}

function cms_content_add_submit($form, &$form_state) {
    $values = $form_state['values'];
    $form_data = array();
    
    $content_title = trim($values['content_title']);
    $url = str_replace(' ', '_', strtolower($content_title));
    
    $content_type = trim($form_state['values']['content_type']);
    
    $form_data['content_type']              =  $content_type;
    $form_data['content_title']             =  $content_title;
    $form_data['link_title']                =  trim($values['link_title']);
    $form_data['order_no']                  =  trim($values['order_no']);
    $form_data['meta_keyword']              =  trim($values['meta_keyword_text_box']);
    $form_data['meta_description']          =  trim($values['meta_desc_text']);
    $form_data['date_of_notification']      =  trim(implode('-', array_reverse(explode('/', $values['don']))));
    $form_data['created_date']              =  date('Y-m-d H:i:s');
    $form_data['content_description']       =  trim($values['content_description']);
    $form_data['is_active']                 =  trim($values['is_active_radio']);
    $form_data['is_new']                    =  trim($values['is_new_chk_box']);
    $form_data['url']                       =  $url;
    
    /*File Upload Handling without Drupal 7's default file managed table with content type-based directories*/
    if (!empty($_FILES['files']['name']['content_file_upload'])) {
        $file = $_FILES['files'];
        $file_name = $file['name']['content_file_upload'];
        $file_tmp = $file['tmp_name']['content_file_upload'];
        
        $directory_name = str_replace(' ', '_', strtolower($content_type));
        $directory = 'public://doc/' . $directory_name . '/';

        file_prepare_directory($directory, FILE_CREATE_DIRECTORY | FILE_MODIFY_PERMISSIONS);
        
        $safe_name = preg_replace('/[^a-zA-Z0-9_\-\.]/', '_', $file_name);
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

    // print_r($form_data);die;
    $duplicate_content_exists = db_select('housing_cms', 'hc')
        ->fields('hc')
        ->condition('content_title', $form_data['content_title'])
        ->condition('link_title', $form_data['link_title'])
        ->execute()
        ->fetchObject();

    if ($duplicate_content_exists) {
        drupal_set_message(t('Duplicate content. Please change content title.'), 'error');
    } else {
        db_insert('housing_cms')
        ->fields($form_data)
        ->execute();
        drupal_set_message(t('Content added successfully.'));
    }
    drupal_goto('cms-content-list');
}