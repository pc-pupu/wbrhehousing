<?php
// Done by Moumita on 28-08-2025
function ddo_change($form, &$form_state, $online_application_id = '') {
    global $user;
    $online_application_id = decrypt_url($online_application_id);


    $query = db_select('housing_ddo', 'hd');
    $query->innerJoin('housing_applicant_official_detail', 'haod', 'haod.ddo_id = hd.ddo_id');
    $query->innerJoin('housing_online_application', 'hoa', 'hoa.applicant_official_detail_id = haod.applicant_official_detail_id');
    $query->condition('hoa.online_application_id', $online_application_id, '=');
    $query->fields('hd', array('ddo_id', 'ddo_designation', 'ddo_address', 'ddo_code', 'is_active'));
    $query->fields('haod', array('applicant_official_detail_id'));

    //echo $query;die;

    $deptresult = $query->execute()->fetchObject();
    echo '<pre>';print_r($deptresult);die;

    if(!empty($deptresult)){
        $ddo_designation =  $deptresult->ddo_designation;
        $ddo_address =  $deptresult->ddo_address;
        $ddo_code =  $deptresult->ddo_code;
        $applicant_official_detail_id = $deptresult->applicant_official_detail_id;
        $ddo_id = $deptresult->ddo_id;
    }else{
        $ddo_designation =  '';
        $ddo_address =  '';
        $ddo_code =  '';
        $applicant_official_detail_id = '';
        $ddo_id = 0;
    }

    // current ddo data from HRMS
  $hrmsdata = (object)getHRMSUserData($user->name); 
    if(!empty($hrmsdata)){
        $current_ddo_code = $hrmsdata->ddoId;
        $current_ddo_designation_arr = get_ddo_designation_by_ddoid($current_ddo_code); // calling from manage_ddo.module

        if(!empty($current_ddo_designation_arr)){
            $current_ddo_code = $current_ddo_designation_arr['ddo_code'];
            $current_ddo_id = $current_ddo_designation_arr['ddo_id'];   //added by dg 26-11-2025
            $current_ddo_designation = $current_ddo_designation_arr['ddo_designation'];
        }else{
            $current_ddo_code = '<span style="color:red;">DDO code not found in the Housing Department records. Please contact the department for updation.</span>';
            $current_ddo_designation = '';
        }

    }else{
        $current_ddo_code = '';
        $current_ddo_designation = '';
    }

    // echo '<pre>';print_r($hrmsdata);die;
    
    $output = '<div class="col-md-12 mt-4">
                    <h4 style="font-weight: 600;line-height: 30px;font-size: 17px;">Your DDO details as recorded with the Housing Department:<br/><span style="font-size: 17px;font-weight: 300;margin-top: 11px;">'.$ddo_designation.'('.$ddo_code.')'.'</span></h4>
                    
                </div>

                <div class="col-md-12">
                    <h4 style="font-weight: 600;line-height: 30px;font-size: 17px;">Your current DDO information, as per HRMS data:<br/><span style="font-size: 17px;font-weight: 300;margin-top: 11px;">'.$current_ddo_designation.'('.$current_ddo_code.')</span></h4>
                    
                    <div>All further proceedings related to this application will be routed through this DDO</div>
                </div>
                        
                ';
    $form['content_markup'] = array(
        '#type' => 'markup',
        '#markup' => $output,
        '#prefix' => '<div class="row"><div class="col-md-12">',
        '#suffix' => '</div></div><br>',
    );

    $info_array = array(
        'old_ddo_code' => $ddo_code,
        'current_ddo_code' => $current_ddo_code,
        'applicant_official_detail_id' => $applicant_official_detail_id,
        'online_application_id' => $online_application_id,
        'old_ddo_id' => $ddo_id,
        'current_ddo_id' => $current_ddo_id,
    );

    $form['hidden_data'] = array(
        '#type' => 'hidden',
        '#value' => serialize($info_array),
    );

    $form['agree_chk'] = array(
        '#type' => 'checkbox',
        '#title' => t('I hereby declare that the DDO information furnished above is true and correct to the best of my knowledge.
'),
    );

    $form['submit'] = array(
        '#type' => 'submit',
        '#value' => t('Save'),
        '#attributes' => array('class' => array('btn bg-primary btn-sm px-5 rounded-pill text-white fw-bolder')),
        '#submit' => array('ddo_change_submit')
    );

    return $form;
    
}

function ddo_change_submit($from, &$form_state) {
    global $user;
    $values = $form_state['values'];

    $hiddenArr = unserialize($values['hidden_data']); echo '<pre>';print_r($hiddenArr);die;
    $online_application_id = $hiddenArr['online_application_id'];

    if($values['agree_chk']!=1){
        drupal_set_message(t('Please agree to the declaration to proceed.'),'error');
        drupal_goto('ddo-change/'.$hiddenArr['online_application_id']);
        return;

    }else{
        if(empty($values['hidden_data'])){
            drupal_set_message(t('Some problem occurred, please try again.'),'error');
            drupal_goto('ddo-change/'.$hiddenArr['online_application_id']);
            return;
        }else{

            $current_ddo_designation_arr = get_ddo_designation_by_ddoid($hiddenArr['current_ddo_code']); // calling from manage_ddo.module   
            if(empty($current_ddo_designation_arr)){
                drupal_set_message(t('Current DDO code not found in the Housing Department records. Please contact the department for updation.'),'error');
                drupal_goto('dashboard');
                return;
            }            
            

            if($hiddenArr['current_ddo_code'] == $hiddenArr['old_ddo_code']){
                
                echo $hiddenArr['current_ddo_code'].'----'.$hiddenArr['old_ddo_code']; die;
                // added on 17-01-2026 by Moumita //
                // update housing_declration table

                $ddodeclration_data = array(
                    'ddo_change_date' => date('Y-m-d H:i:s'),
                    'ddo_id_from' => $hiddenArr['old_ddo_id'],
                    'ddo_id_to' => $hiddenArr['current_ddo_id'],
                );

                echo '<pre>';print_r($ddodeclration_data);die;

                db_update('housing_declration')
                ->fields($ddodeclration_data)
                ->condition('online_application_id', $online_application_id)
                ->condition('uid', $user->uid)
                ->execute();

                // added on 17-10-2025 by Moumita //
                db_update('housing_flat_occupant')
                 ->fields(array('accept_reject_status' => 'Accept'))  
                 ->condition('online_application_id', $online_application_id, '=')
                 ->execute();

                db_update('housing_online_application')
                 ->fields(array('status' => 'applicant_acceptance'))  
                 ->condition('online_application_id', $online_application_id, '=')
                 ->execute();
                $status_id = get_status_id('applicant_acceptance');
                $ins_arr = array(
                    'online_application_id'=> $online_application_id,
                    'status_id' => $status_id,
                    'created_at' => date('Y-m-d H:i:s'),
                    'uid' => $user->uid,
                    'short_code' => 'applicant_acceptance'   
                );

                db_insert('housing_process_flow')->fields($ins_arr)->execute();

                drupal_set_message(t('DDO details updated successfully and You have accepted the terms and conditions stated in the Declaration before Competent Authority'));
                drupal_goto('dashboard');
                return;

            }else{
                $form_data = array(
                    'ddo_id' => $hiddenArr['current_ddo_id'],
                );
                // update ddo_id in housing_applicant_official_detail table
                db_update('housing_applicant_official_detail')
                ->fields($form_data)
                ->condition('applicant_official_detail_id', $hiddenArr['applicant_official_detail_id'])
                ->condition('uid', $user->uid)
                ->condition('ddo_id', $hiddenArr['old_ddo_id'])
                ->execute();
                
                // update housing_declration table

                $ddodeclration_data = array(
                    'ddo_change_date' => date('Y-m-d H:i:s'),
                    'ddo_id_from' => $hiddenArr['old_ddo_id'],
                    'ddo_id_to' => $hiddenArr['current_ddo_id'],
                );

                db_update('housing_declration')
                ->fields($ddodeclration_data)
                ->condition('online_application_id', $online_application_id)
                ->condition('uid', $user->uid)
                ->execute();

                // added on 17-10-2025 by Moumita //
                db_update('housing_flat_occupant')
                 ->fields(array('accept_reject_status' => 'Accept'))  
                 ->condition('online_application_id', $online_application_id, '=')
                 ->execute();

                db_update('housing_online_application')
                 ->fields(array('status' => 'applicant_acceptance'))  
                 ->condition('online_application_id', $online_application_id, '=')
                 ->execute();
                $status_id = get_status_id('applicant_acceptance');
                $ins_arr = array(
                    'online_application_id'=> $online_application_id,
                    'status_id' => $status_id,
                    'created_at' => date('Y-m-d H:i:s'),
                    'uid' => $user->uid,
                    'short_code' => 'applicant_acceptance'   
                );

                db_insert('housing_process_flow')->fields($ins_arr)->execute();

                drupal_set_message(t('DDO details updated successfully and You have accepted the terms and conditions stated in the Declaration before Competent Authority.'));
                drupal_goto('dashboard');
            }


            
        }
    }

    
    
    
}
