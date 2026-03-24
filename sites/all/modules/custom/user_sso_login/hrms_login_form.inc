<?php
function hrms_login_form($form, &$form_state) {
    if(!user_is_logged_in()) {

        $form['markup_header'] = array(
            '#type' => 'markup',
            '#markup' => '<div class="d-flex justify-content-center h-100 end-0"><div class="search">'
        );

        $form['name'] = array(
            '#type' => 'item',
            '#prefix' => '<div id="hrms" class="login-panel">',
            '#suffix' => '</div>',
        );
        $form['name']['hrms_id'] = array(
            '#type' => 'textfield',
            '#required' => TRUE,
            '#attributes' => array('class'=>array('search_input'), 'placeholder'=> 'Enter HRMS ID'),	
            // '#title' => t('Enter HRMS Id'),
        );
        $form['name']['submit'] = array(
            '#type' => 'submit',
            '#attributes' => array('class'=>array('search_icon')),
            '#value' => 'Go',
        );
        // $form['rhe_user_login'] = array(
        //     '#type'    => 'link',  
        //     '#prefix' => '<div class="m-3">',
        //     '#suffix' => '</div>',  
        //     '#title'   => t('Click here for Housing users login'),    
        //     '#href'  => '/user/login',
        //     '#attributes' => array('class' => array('btn btn-light btn-sm')),
        
        // );
        $form['markup_footer'] = array(
            '#type' => 'markup',
            '#markup' => '</div></div>'
        );
        return $form;
    } else { 
        // die('ekhanei dhukche');
        // drupal_goto('/user');
        drupal_goto('dashboard');
    }
    
}

function hrms_login_form_submit($form, &$form_state) {
    if(isset($form_state['values']['hrms_id']) && !empty($form_state['values']['hrms_id'])) {


        $account = user_load_by_name($form_state['values']['hrms_id']);
        if (empty($account)) {
            $mail = isset($form_state['values']['hrms_id']) ? trim($form_state['values']['hrms_id']) : $form_state['values']['hrms_id'].'@gmail.com';
            $appUserData = array(
                    'name' => trim($form_state['values']['hrms_id']) ,
                    'pass' => trim($form_state['values']['hrms_id']) , // note: do not md5 the password
                    'mail' => $mail,
                    'access' => REQUEST_TIME,
                    'login' => REQUEST_TIME,
                    'status' => 1,
                    //'init' => trim($userData['email']),
                    'roles' => array(
                        DRUPAL_AUTHENTICATED_RID => 'authenticated user',
                        4 => 'Applicant'
                    )
                );
            $userDetails = user_save(Null, $appUserData);
        }

        $hrmscode=encrypt_url($form_state['values']['hrms_id']); 
        $timestamp = time();
        $message = $hrmscode . "|" . $timestamp;
        $hmac = hash_hmac("sha256", $message, variable_get('hmac_secret_me',''));
        $token = base64_encode($message . "|" . $hmac);
        // $redirect_url=$base_root.$base_path.'user/sso/'.urlencode($token);
        
        drupal_goto('user/sso/'.urlencode($token));

    }
}
?>