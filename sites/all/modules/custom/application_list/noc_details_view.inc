<?php

function noc_details_view($form, &$form_state, $online_application_id = '',$allotment_flat_id = ''){
    global $user, $user_role; 
    $form = array();
    $flat_id = decrypt_url($allotment_flat_id);
    $online_application_id = decrypt_url($online_application_id);
    // echo $flat_id;die;
    $query = db_select('housing_applicant', 'ha');
    $query->innerJoin('housing_applicant_official_detail', 'haod','haod.housing_applicant_id = ha.housing_applicant_id');
    $query->innerJoin('housing_online_application', 'hoa','hoa.applicant_official_detail_id = haod.applicant_official_detail_id');
    $query->innerJoin('housing_flat_occupant', 'hfo','hfo.online_application_id = hoa.online_application_id');
    $query->innerJoin('housing_flat', 'hf','hf.flat_id = hfo.flat_id');
	$query->innerJoin('housing_block', 'hb','hb.block_id = hf.block_id');
    $query->innerJoin('housing_estate', 'he','he.estate_id = hf.estate_id');
    $query->innerJoin('housing_flat_type', 'hft','hft.flat_type_id = hf.flat_type_id');
    $query->innerJoin('housing_flat_occupant', 'hfo','hfo.online_application_id = hoa.online_application_id');

    $query->fields('hf', array('flat_no','floor'));
    $query->fields('hb', array('block_name'));
    $query->fields('he', array('estate_name'));
    $query->fields('hft', array('flat_type'));
    $query->fields('ha', array('applicant_name'));
    $query->fields('hoa', array('application_no'));
    $query->condition('hf.flat_id',$flat_id);

    $result = $query->execute();
    $data = $result->fetchObject();
    // echo "<pre>";print_r($data);die;

    $query1 = db_select('housing_process_flow', 'hpf');
    $query1->fields('hpf', array('remarks'));
    $query1->condition('hpf.online_application_id', $online_application_id);
    $query1->condition('hpf.short_code', 'flat_released');
    $query1->orderBy('hpf.id', 'DESC');
    $query1->range(0, 1);

    $latest_remark = $query1->execute()->fetchField();
    // echo $latest_remark;die;

    $output =   '<div class="table-bottom">
                    <table class="table table-list">
                        <tr>
                            <th colspan="100%" style="border: 1px solid; border-color: white; background: none repeat scroll 0 0 #0a8bd6dc;color:white;text-align: center;font-size: 18px;line-height: 24px; font-weight: normal;font-family: "Dosis",Arial,Verdana,serif;" class="first">Applicant & Flat Information</th>
                            
                        </tr>
                        <tr>
                            <td>Applicant Name :</td>
                            <td colspan="2" style="text-align: center;">'.$data->applicant_name.'</td>
                        </tr>
                        <tr>
                            <td>Application No :</td>
                            <td colspan="2" style="text-align: center;">'.$data->application_no.'</td>
                        </tr>
                        <tr>
                            <td>Estate Name :</td>
                            <td colspan="2" style="text-align: center;">'.$data->estate_name.'</td>
                        </tr>
                        <tr>
                            <td>Block :</td>
                            <td colspan="2" style="text-align: center;">'.$data->block_name.'</td>
                        </tr>
                        <tr>
                            <td>Flat Type :</td>
                            <td colspan="2" style="text-align: center;">'.$data->flat_type.'</td>
                        </tr>
                         <tr>
                            <td>Flat No.:</td>
                            <td colspan="2" style="text-align: center;">'.$data->flat_no.'</td>
                        </tr>
                    </table>
                </div>';

    $form['flat_info'] = array(
        '#type' => 'markup',
        '#markup' => $output
    );
    $form['online_application_id'] = array(
    '#type' => 'hidden',
    '#value' => $online_application_id, 
    );

    if($user_role == 7){

        $query2 = db_select('housing_process_flow', 'hpf');
        $query2->fields('hpf', array('remarks'));
        $query2->condition('hpf.online_application_id', $online_application_id);
        $or = db_or()
        ->condition('hpf.short_code', 'flat_released')
        ->condition('hpf.short_code', 'flat_released_div')
        ->condition('hpf.short_code', 'flat_released_comp_auth');
        $query2->condition($or);
        
        $remark_arr = $query2->execute()->fetchAll();
        $latest_subdiv_remark = !empty($remark_arr[0]->remarks) ? check_plain($remark_arr[0]->remarks)  : '';
        $latest_div_remark = !empty($remark_arr[1]->remarks) ? check_plain($remark_arr[1]->remarks)  : '';
        $latest_ca_remark = !empty($remark_arr[2]->remarks) ? check_plain($remark_arr[2]->remarks)  : '';

        // $query2 = db_select('housing_process_flow', 'hpf');
        // $query2->fields('hpf', array('remarks'));
        // $query2->condition('hpf.online_application_id', $online_application_id);
        // $query2->condition('hpf.short_code', 'flat_released');
        // $query2->orderBy('hpf.id', 'DESC');
        // $query2->range(0, 1);

        // $latest_remark_subdiv = $query2->execute()->fetchField();

        // $subdiv_remark = !empty($latest_remark_subdiv) ? check_plain($latest_remark_subdiv) : 'NA';

        if (!empty($latest_subdiv_remark) && empty($latest_ca_remark) && empty($latest_div_remark)) {

            $output2 =   '<div class="table-bottom">
                    <table class="table table-list">
                        <tr>
                            <th colspan="100%" style="border: 1px solid; border-color: white; background: none repeat scroll 0 0 #0a8bd6dc;color:white;text-align: center;font-size: 18px;line-height: 24px; font-weight: normal;font-family: "Dosis",Arial,Verdana,serif;" class="first">Current Status</th>
                            
                        </tr>
                         <tr>
                            <td>Sub Division Remarks :</td>
                            <td colspan="2" style="text-align: center;">'.$latest_subdiv_remark.'</td>
                        </tr>
                        <tr>
                            <td>Current Status :</td>
                            <td colspan="2" style="text-align: center;">Forwarded to Division</td>
                        </tr>
                        
                    </table>
                </div>';
            $form['subdiv_remarks'] = array(
                '#type' => 'markup',
                '#markup' => $output2,
            
            );

        }else if(!empty($latest_div_remark) && !empty($latest_ca_remark)){
            
            $output2 =   '<div class="table-bottom">
                    <table class="table table-list">
                        <tr>
                            <th colspan="100%" style="border: 1px solid; border-color: white; background: none repeat scroll 0 0 #0a8bd6dc;color:white;text-align: center;font-size: 18px;line-height: 24px; font-weight: normal;font-family: "Dosis",Arial,Verdana,serif;" class="first">Current Status</th>
                            
                        </tr>
                        <tr>
                            <td>Sub Division Remarks :</td>
                            <td colspan="2" style="text-align: center;">'.$latest_subdiv_remark.'</td>
                        </tr>
                        <tr>
                            <td>Division Remarks :</td>
                            <td colspan="2" style="text-align: center;">'.$latest_div_remark.'</td>
                        </tr>
                        <tr>
                            <td>Competent Authority Remarks :</td>
                            <td colspan="2" style="text-align: center;">'.$latest_ca_remark.'</td>
                        </tr>
                        <tr>
                            <td>Current Status :</td>
                            <td colspan="2" style="text-align: center;">NOC Issuance Completed</td>
                        </tr>
                        
                    </table>
                </div>';
            $form['div_remarks'] = array(
                '#type' => 'markup',
                '#markup' => $output2,
            
            );
        } 




        // $output2 =   '<div class="table-bottom">
        //             <table class="table table-list">
        //                 <tr>
        //                     <th colspan="100%" style="border: 1px solid; border-color: white; background: none repeat scroll 0 0 #0a8bd6dc;color:white;text-align: center;font-size: 18px;line-height: 24px; font-weight: normal;font-family: "Dosis",Arial,Verdana,serif;" class="first">Current Status</th>
                            
        //                 </tr>
        //                 <tr>
        //                     <td>Sub Division Remarks :</td>
        //                     <td colspan="2" style="text-align: center;">'.$subdiv_remark.'</td>
        //                 </tr>
        //                 <tr>
        //                     <td>Current Status :</td>
        //                     <td colspan="2" style="text-align: center;">Forwarded to Division</td>
        //                 </tr>
                        
        //             </table>
        //         </div>';
       
        $form['subdiv_remarks'] = array(
            '#type' => 'markup',
            '#markup' => $output2,
            
        );
        $form['div_remarks'] = array(
            '#type' => 'hidden',
            '#value' => ''
        );

        $form['ca_remarks'] = array(
            '#type' => 'hidden',
            '#value' => ''
        );

    }else if($user_role == 8){
        $subdiv_remark = !empty($latest_remark) ? check_plain($latest_remark)  : 'NA';
        
        $form['subdiv_remarks'] = array(
        '#type' => 'markup',
        '#markup' => '<span style="color: #0a5bdc; font-weight: bold;">Sub Division Remarks: </span>' .
                         '<span style="color: #d9534f; font-weight: bold;">' . $subdiv_remark . '</span>',
            //'#prefix' => '<div style="color: #0a5bdc; font-weight: bold;">',
        '#prefix' => '<div style="color: #0a5bdc; font-weight: bold;">',
        '#suffix' => '</div>',
        );

        $query2 = db_select('housing_process_flow', 'hpf');
        $query2->fields('hpf', array('remarks'));
        $query2->condition('hpf.online_application_id', $online_application_id);
        $or = db_or()
        ->condition('hpf.short_code', 'flat_released')
        ->condition('hpf.short_code', 'flat_released_div')
        ->condition('hpf.short_code', 'flat_released_comp_auth');
        $query2->condition($or);
        
        $remark_arr = $query2->execute()->fetchAll();
        $latest_subdiv_remark = !empty($remark_arr[0]->remarks) ? check_plain($remark_arr[0]->remarks)  : '';
        $latest_div_remark = !empty($remark_arr[1]->remarks) ? check_plain($remark_arr[1]->remarks)  : '';
        $latest_ca_remark = !empty($remark_arr[2]->remarks) ? check_plain($remark_arr[2]->remarks)  : '';

        
       if (!empty($latest_div_remark) && empty($latest_ca_remark)) {

            $output2 =   '<div class="table-bottom">
                    <table class="table table-list">
                        <tr>
                            <th colspan="100%" style="border: 1px solid; border-color: white; background: none repeat scroll 0 0 #0a8bd6dc;color:white;text-align: center;font-size: 18px;line-height: 24px; font-weight: normal;font-family: "Dosis",Arial,Verdana,serif;" class="first">Current Status</th>
                            
                        </tr>
                         <tr>
                            <td>Sub Division Remarks :</td>
                            <td colspan="2" style="text-align: center;">'.$latest_subdiv_remark.'</td>
                        </tr>
                        <tr>
                            <td>Division Remarks :</td>
                            <td colspan="2" style="text-align: center;">'.$latest_div_remark.'</td>
                        </tr>
                        <tr>
                            <td>Current Status :</td>
                            <td colspan="2" style="text-align: center;">Forwarded to Competent Authority</td>
                        </tr>
                        
                    </table>
                </div>';
            $form['div_remarks'] = array(
                '#type' => 'markup',
                '#markup' => $output2,
            
            );

        }else if(!empty($latest_div_remark) && !empty($latest_ca_remark)){
            
            $output2 =   '<div class="table-bottom">
                    <table class="table table-list">
                        <tr>
                            <th colspan="100%" style="border: 1px solid; border-color: white; background: none repeat scroll 0 0 #0a8bd6dc;color:white;text-align: center;font-size: 18px;line-height: 24px; font-weight: normal;font-family: "Dosis",Arial,Verdana,serif;" class="first">Current Status</th>
                            
                        </tr>
                        <tr>
                            <td>Sub Division Remarks :</td>
                            <td colspan="2" style="text-align: center;">'.$latest_subdiv_remark.'</td>
                        </tr>
                        <tr>
                            <td>Division Remarks :</td>
                            <td colspan="2" style="text-align: center;">'.$latest_div_remark.'</td>
                        </tr>
                        <tr>
                            <td>Competent Authority Remarks :</td>
                            <td colspan="2" style="text-align: center;">'.$latest_ca_remark.'</td>
                        </tr>
                        <tr>
                            <td>Current Status :</td>
                            <td colspan="2" style="text-align: center;">NOC Issuance Completed</td>
                        </tr>
                        
                    </table>
                </div>';
            $form['div_remarks'] = array(
                '#type' => 'markup',
                '#markup' => $output2,
            
            );
        } else {
            $form['div_remarks'] = array(
                '#title' => t('Division Remarks'),
                '#type' => 'textarea',
                '#required' => TRUE,
                '#attributes' => array(
                'class' => array('form-control'),
                ),
            );
        }

        $form['ca_remarks'] = array(
            '#type' => 'hidden',
            '#value' => ''
        );
        
    }else if($user_role == 6){
        $query1 = db_select('housing_process_flow', 'hpf');
        $query1->fields('hpf', array('remarks'));
        $query1->condition('hpf.online_application_id', $online_application_id);
        $or = db_or()
        ->condition('hpf.short_code', 'flat_released')
        ->condition('hpf.short_code', 'flat_released_div')
        ->condition('hpf.short_code', 'flat_released_comp_auth');
        

        $query1->condition($or);
        
        $remark_arr = $query1->execute()->fetchAll();
        $latest_subdiv_remark = !empty($remark_arr[0]->remarks) ? check_plain($remark_arr[0]->remarks)  : 'NA';
        $latest_div_remark = !empty($remark_arr[1]->remarks) ? check_plain($remark_arr[1]->remarks)  : 'NA';
        $latest_ca_remark = !empty($remark_arr[1]->remarks) ? check_plain($remark_arr[2]->remarks)  : '';

        // echo "<pre>";print_r($latest_ca_remark);die;

        
        if (!empty($latest_ca_remark)) {
           $output2 =   '<div class="table-bottom">
                    <table class="table table-list">
                        <tr>
                            <th colspan="100%" style="border: 1px solid; border-color: white; background: none repeat scroll 0 0 #0a8bd6dc;color:white;text-align: center;font-size: 18px;line-height: 24px; font-weight: normal;font-family: "Dosis",Arial,Verdana,serif;" class="first">Current Status</th>
                            
                        </tr>
                        <tr>
                            <td>Sub Division Remarks :</td>
                            <td colspan="2" style="text-align: center;">'.$latest_subdiv_remark.'</td>
                        </tr>
                        <tr>
                            <td>Division Remarks :</td>
                            <td colspan="2" style="text-align: center;">'.$latest_div_remark.'</td>
                        </tr>
                        <tr>
                            <td>Competent Authority Remarks :</td>
                            <td colspan="2" style="text-align: center;">'.$latest_ca_remark.'</td>
                        </tr>
                        <tr>
                            <td>Current Status :</td>
                            <td colspan="2" style="text-align: center;">NOC Issuance Completed</td>
                        </tr>
                        
                    </table>
                </div>';
            $form['div_remarks'] = array(
                '#type' => 'markup',
                '#markup' => $output2,
            
            );

        }else{

            $form['subdiv_remarks'] = array(
            '#type' => 'markup',
            '#markup' => '<span style="color: #0a5bdc; font-weight: bold;">Sub Division Remarks: </span>' .
                         '<span style="color: #d9534f; font-weight: bold;">' . $latest_subdiv_remark . '</span>',
            '#prefix' => '<div style="color: #0a5bdc; font-weight: bold;">',
            '#suffix' => '</div>',
            );
            $form['div_remarks'] = array(
            '#type' => 'markup',
            '#markup' => '<span style="color: #0a5bdc; font-weight: bold;">Division Remarks: </span>' .
                         '<span style="color: #d9534f; font-weight: bold;">' . $latest_div_remark . '</span>',
            '#prefix' => '<div style="color: #0a5bdc; font-weight: bold;">',
            '#suffix' => '</div>',
            );
            $form['ca_remarks'] = array(
            '#title' => t('Competent Authority Remarks:'),
            '#type' => 'textarea',
            '#required' => TRUE,
            '#attributes' => array('class' => array('form-control'))
            );
        }

       
        
    }else{
        $form['subdiv_remarks'] = array(
            '#type' => 'markup',
            '#value' => t('Remarks of the Sub division end')
        );

        $form['div_remarks'] = array(
            '#type' => 'markup',
            '#value' => t('Remarks of the Division end')
        );
        $form['ca_remarks'] = array(
            '#type' => 'markup',
            '#value' => t('Remarks of the CA end')
        );
    }
    
    // // echo $latest_remark_div;die;
    // if (empty($latest_div_remark) && empty($latest_ca_remark) && empty($latest_subdiv_remark)) {
    //     if($user_role == 6){
    //         $form['save_btn'] = array(
    //             '#type' => 'submit',
    //             '#value' => 'Submit',
    //         );
    //     }else if($user_role == 8){
    //         $form['save_btn'] = array(
    //             '#type' => 'submit',
    //             '#value' => 'Forward to Competent Authority',
    //         );
    //     }
            
       
        
    // }
    // echo $latest_div_remark;die;

    if($user_role == 6 && empty($latest_ca_remark)){
        $form['save_btn'] = array(
                '#type' => 'submit',
                '#value' => 'Submit', 
            );
    }else if($user_role == 8 && empty($latest_div_remark)){
         $form['save_btn'] = array(
                '#type' => 'submit',
                '#value' => 'Forward to Competent Authority',
            );
    }

    return $form;
        
} 

function noc_details_view_submit($form, &$form_state) {
    
    global $user;
    $online_application_id = $form_state['values']['online_application_id'];
    $status_id = get_status_id('flat_released_div');
    $div_remarks = $form_state['values']['div_remarks'];

    $ca_remarks = $form_state['values']['ca_remarks'];
    $ca_status_id = get_status_id('flat_released_comp_auth');
    // echo "kk".$ca_remarks;die;
    $process_flow_arr = array(
					'online_application_id' => $online_application_id,
					'status_id' => !empty($form_state['values']['div_remarks']) ? $status_id : $ca_status_id,
					'created_at' => date('Y-m-d H:i:s'),
					'uid' => $user->uid,
					'short_code' => !empty($form_state['values']['div_remarks']) ? 'flat_released_div' : 'flat_released_comp_auth', 
                    'remarks' => !empty($form_state['values']['div_remarks']) ? $div_remarks : $ca_remarks,
				);
                // echo "<pre>";print_r($process_flow_arr);die;
	db_insert('housing_process_flow')->fields($process_flow_arr)->execute();

    db_update('housing_online_application')->fields(array(
					'status' => !empty($form_state['values']['div_remarks']) ? 'flat_released_div' : 'flat_released_comp_auth', 
					'date_of_verified' => date('Y-m-d H:i:s')  
					)
				)
				->condition('online_application_id', $online_application_id, '=')
				->execute();
    if(!empty($form_state['values']['div_remarks'])) {
         drupal_set_message(t('Division Remarks Added Successfully.'));
    }else if(!empty($form_state['values']['ca_remarks'])){
         drupal_set_message(t('Competent Authority Remarks Added Successfully.'));
    }          
   
	drupal_goto('view-flat-released-ddo'.'/'.encrypt_url($online_application_id));
}