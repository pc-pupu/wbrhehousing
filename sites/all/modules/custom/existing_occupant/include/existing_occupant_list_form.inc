<?php
//~~ This function is for Division & Subdivision ~~\\
/*Done by Subham dt.24-06-2025*/
function existing_occupant_list_form($form, &$form_state) {
    $rhe_name =  isset($form_state['values']['rhe_name']) && !empty($form_state['values']['rhe_name']) ? $form_state['values']['rhe_name'] : 0;
    $flat_type =  isset($form_state['values']['flat_type']) && !empty($form_state['values']['flat_type']) ? $form_state['values']['flat_type'] : 0;
    $block_name =  isset($form_state['values']['block_name']) && !empty($form_state['values']['block_name']) ? $form_state['values']['block_name'] : 0;
    
    $form['list_info'] = array(
        '#prefix' => '<div class="row">',
        '#suffix' => '</div>'
    );
    
    $form['list_info']['rhe_name'] = array(
        '#title' => t('Name of the RHE'),
        '#type' => 'select',
        '#options' => rhe_list_specific(),
        '#prefix' => '<div class="row"><div class="col-md-4"><div class="form-floating">',
        '#suffix' => '</div></div>',
        '#attributes' => array('class'=>array('form-select')),
        '#ajax' => array(
            'event' => 'change',
            'callback' => '_reload_rhewise_list_view',
            'method' => 'replace',
        ),

    );
    
    $form['list_info']['flat_type'] = array(
        '#prefix' => '<div class="three" id="flat_type_replace">',
        '#suffix' => '</div>'
    );
        
    if($rhe_name != 0) {
        $form['list_info']['flat_type'] = array(
            '#title' => t('Flat Type'),
            '#type' => 'select',
            '#options' => flat_type_under_rhe($rhe_name),
            '#prefix' => '<div class="col-md-4" id="flat_type_replace"><div class="form-floating">',
            '#suffix' => '</div></div></div>',
            '#attributes' => array('class'=>array('form-select')),
            '#ajax' => array(
                'event' => 'change',
                'callback' => '_reload_rhewise_list_view',
                'method' => 'replace',
            ),
        );
    }

    $form['list_info']['block_name'] = array(
        '#prefix' => '<div class="three" id="block_name_replace">',
        '#suffix' => '</div>'
    );
        
    if($rhe_name != 0 && $flat_type != 0) {
        $form['list_info']['block_name'] = array(
            '#title' => t('Name of the Block'),
            '#type' => 'select',
            '#options' => block_name_under_rhe($rhe_name, $flat_type),
            '#prefix' => '<div class="col-md-4" id="block_name_replace"><div class="form-floating">',
            '#suffix' => '</div></div>',
            '#attributes' => array('class'=>array('form-select')),
            '#ajax' => array(
                'event' => 'change',
                'callback' => '_reload_rhewise_list_view',
                'method' => 'replace',
            ),
        );
    }

    $form['fetch_list'] = array(
        '#prefix' => '<div class="col-md-12" id="flat_list_replace">',
        '#suffix' => '</div>'
    );
    
    if($rhe_name != 0 && $flat_type != 0 && $block_name != 0) {
        $form['fetch_list'] = array(
            '#type' => 'markup',
            '#markup' => view_occupant_list($rhe_name, $flat_type, $block_name),
            '#weight' => 50,
            '#prefix' => '<div class="col-md-12" id="flat_list_replace">',
            '#suffix' => '</div>'
        );
    }

    return $form;
}

function _reload_rhewise_list_view($form, &$form_state) {
    $commands = array();
    
    $commands[] = ajax_command_replace("#flat_type_replace", drupal_render($form['list_info']['flat_type']));
    $commands[] = ajax_command_replace("#block_name_replace", drupal_render($form['list_info']['block_name']));
    $commands[] = ajax_command_replace("#flat_list_replace", drupal_render($form['fetch_list']));

    return array('#type' => 'ajax', '#commands' => $commands);
}

function view_occupant_list($rhe_name = 0, $flat_type = 0, $block_name = 0) {  
    // global $base_root, $base_path,$user;
    // $output = '';

    //     /* Done by Subham 30-12-2024*/
    //     $query_dtls = db_select('users_details', 'ud'); 
    //     $query_dtls->fields('ud');
    //     $query_dtls->condition('uid',$user->uid,'=');
    //     $rsult = $query_dtls->execute();
    //     $data_fetch = $rsult->fetchObject();
    //     /* End */

    //     if($data_fetch->division_id != '' && $data_fetch->subdiv_id != '') {   // Added by Subham 30-12-2024
    //         if($data_fetch->subdiv_id != 0) {								   // Added by Subham 30-12-2024
    //             $query = db_select('housing_applicant', 'ha');

    //             // $query->innerJoin('housing_applicant_official_detail', 'haod', 'ha.uid = haod.uid');  //off by dg 30-12-2024
    //             $query->innerJoin('housing_applicant_official_detail', 'haod', 'ha.housing_applicant_id = haod.housing_applicant_id');  //added by dg 30-12-2024 
    //             $query->innerJoin('housing_online_application', 'hoa', 'hoa.applicant_official_detail_id = haod.applicant_official_detail_id');
    //             $query->innerJoin('housing_flat_occupant', 'hfo', 'hfo.online_application_id = hoa.online_application_id');
    //             $query->innerJoin('housing_flat', 'hf', 'hf.flat_id = hfo.flat_id');
    //             $query->innerJoin('housing_estate', 'he', 'he.estate_id = hf.estate_id');
    //             $query->innerJoin('housing_flat_type', 'hft', 'hft.flat_type_id = hf.flat_type_id');
    //             $query->innerJoin('users', 'u', 'u.uid = haod.uid');
    //             // $query->leftJoin('users', 'u', 'u.uid = haod.uid');

    //             $query->condition('hoa.status', 'existing_occupant', '=');
    //             // $query->condition('haod.is_active', 1, '=');

    //             $query->addField('hoa', 'online_application_id'); // added by Moumita on 24-06-2025

    //             $query->addField('ha', 'applicant_name');
    //             $query->addField('he', 'estate_name');
    //             $query->addField('hft', 'flat_type');
    //             $query->addField('haod', 'hrms_id');
    //             $query->addField('u', 'status');
    //             $query->addField('haod', 'uid');

    //             $db_and = db_and();														// Added by Subham 30-12-2024
    //             $db_and->condition('he.division_id', $data_fetch->division_id, '=');	// Added by Subham 30-12-2024
    //             $db_and->condition('he.subdiv_id', $data_fetch->subdiv_id, '=');		// Added by Subham 30-12-2024
    //             $query->condition($db_and);												// Added by Subham 30-12-2024

    //             if($rhe_name != 0 && $flat_type != 0 && $block_name != 0){
    //                 $db_and = db_and();
    //                 $db_and->condition('hf.estate_id', $rhe_name, '=');
    //                 $db_and->condition('hf.flat_type_id', $flat_type, '=');
    //                 $db_and->condition('hf.block_id', $block_name, '=');
    //                 $query->condition($db_and);
    //             }

    //             $result =$query->execute();

    //         } else{																		// Added by Subham 30-12-2024
    //             $query = db_select('housing_applicant', 'ha');

    //             // $query->innerJoin('housing_applicant_official_detail', 'haod', 'ha.uid = haod.uid');  //off by dg 30-12-2024
    //             $query->innerJoin('housing_applicant_official_detail', 'haod', 'ha.housing_applicant_id = haod.housing_applicant_id');  //added by dg 30-12-2024 
    //             $query->innerJoin('housing_online_application', 'hoa', 'hoa.applicant_official_detail_id = haod.applicant_official_detail_id');
    //             $query->innerJoin('housing_flat_occupant', 'hfo', 'hfo.online_application_id = hoa.online_application_id');
    //             $query->innerJoin('housing_flat', 'hf', 'hf.flat_id = hfo.flat_id');
    //             $query->innerJoin('housing_estate', 'he', 'he.estate_id = hf.estate_id');
    //             $query->innerJoin('housing_flat_type', 'hft', 'hft.flat_type_id = hf.flat_type_id');
    //             $query->innerJoin('users', 'u', 'u.uid = haod.uid');

    //             $query->condition('hoa.status', 'existing_occupant', '=');
    //             // $query->condition('haod.is_active', 1, '=');

    //             $query->addField('hoa', 'online_application_id'); // added by Moumita on 24-06-2025

    //             $query->addField('ha', 'applicant_name');
    //             $query->addField('he', 'estate_name');
    //             $query->addField('hft', 'flat_type');
    //             $query->addField('haod', 'hrms_id');
    //             $query->addField('u', 'status');
    //             $query->addField('haod', 'uid');

    //             $query->condition('he.division_id', $data_fetch->division_id, '=');		// Added by Subham 30-12-2024

    //             if($rhe_name != 0 && $flat_type != 0 && $block_name != 0){
    //                 $db_and = db_and();
    //                 $db_and->condition('hf.estate_id', $rhe_name, '=');
    //                 $db_and->condition('hf.flat_type_id', $flat_type, '=');
    //                 $db_and->condition('hf.block_id', $block_name, '=');
    //                 $query->condition($db_and);
    //             }

    //             $result =$query->execute();
    //         }
    //     }
        
    //     $header['Serial No.'] = array('data' => 'Serial No.');
    //     $header['Applicant Name'] = array('data' => 'Applicant Name');
    //     $header['HRMS Code'] = array('data' => 'HRMS Code');
    //     $header['Estate Name'] = array('data' => 'Estate Name');
    //     $header['Flat Type'] = array('data' => 'Flat Type');
    //     $header['Approval Status'] = array('data' => 'Approval Status');
    //     $header['Details'] = array('data' => 'Details');
    //     $header['Edit'] = array('data' => 'Edit');
    //     // Added 22-05-2025 by Subham
    //     if($user_role == 8){
    //         $header['Action'] = array('data' => 'Action');
    //     }

    //     $rows =array();
    //     $output = '';
    //     $serialNumber = 1;
    
    // while($data = $result->fetchObject()) {
    //     $fields = array();
    
    //     $fields[] = $serialNumber;
    //     $fields[] = $data->applicant_name;
    //     $fields[] = $data->hrms_id;
    //     $fields[] = $data->estate_name;
    //     $fields[] = $data->flat_type;
    //     if($data->status == 0){
    //         $fields[] = 'Pending Approval at Division Level';
    //     }else{
    //         $fields[] = 'Approved by Division';
    //     }
    //     $fields[] = l(
    //     'View Details',
    //     'existing-occupant-view-det/'. encrypt_url($data->uid),
    //     array(
    //         'html'=>TRUE,
    //         'attributes'=> array('class'=>array('btn bg-success btn-sm px-4 rounded-pill text-white fw-bolder')),
    //     ));	
    //     $serialNumber++;

    //     $rows[] = $fields;
        
    // }

    // $variables = array(
    //     'attributes' => array('class'=>array('table table-list table-striped')),
    //     'header' => $header,
    //     'rows' => $rows,
    //     'sticky' => true,
    //     'empty' => t("No data found!") // The message to be displayed if table is empty
    //     );
    // //end

    // if(count($rows) > 0) {
    //     // Render using Drupal's render API.
    //     $build['datatable'] = array(
    //         '#theme' => 'datatable',
    //         '#header' => $header,
    //         '#rows' => $rows,
    //         '#attributes' => array(),
    //     );
        
    //     // Or, render using a theme function.
    //     $variables = array(
    //         'attributes' => array('class'=>array('table table-list table-striped')),
    //         'header' => $header,
    //         'rows' => $rows,
    //     );
        
    //     $output = theme('datatable', $variables);
    // }
    // else {
    //     $output = '<div>
    //                     <table class="datatable_no_data_found table table-list">
    //                         <tr class="tr_no_data_found">
    //                             <th class="th_no_data_found"></th>
    //                         </tr>
    //                         <tr class="tr_no_data_found">
    //                             <td class="td_no_data_found">No data found!</td>
    //                         </tr>
    //                     </table>
    //                 </div>';
    // }

    // return $output;

    global $base_root, $base_path,$user,$user_role;
		$output = '';

        /* Done by Subham 30-12-2024*/
        $query_dtls = db_select('users_details', 'ud'); 
        $query_dtls->fields('ud');
        $query_dtls->condition('uid',$user->uid,'=');
        $rsult = $query_dtls->execute();
        $data_fetch = $rsult->fetchObject();
        /* End */

        if($data_fetch->division_id != '' && $data_fetch->subdiv_id != '') { // Added by Subham 30-12-2024
            if($data_fetch->subdiv_id != 0) {								// Added by Subham 30-12-2024
                $query = db_select('housing_applicant', 'ha');

                // $query->innerJoin('housing_applicant_official_detail', 'haod', 'ha.uid = haod.uid');  //off by dg 30-12-2024
                $query->innerJoin('housing_applicant_official_detail', 'haod', 'ha.housing_applicant_id = haod.housing_applicant_id');  //added by dg 30-12-2024 
                $query->innerJoin('housing_online_application', 'hoa', 'hoa.applicant_official_detail_id = haod.applicant_official_detail_id');
                $query->innerJoin('housing_flat_occupant', 'hfo', 'hfo.online_application_id = hoa.online_application_id');
                $query->innerJoin('housing_flat', 'hf', 'hf.flat_id = hfo.flat_id');
                $query->innerJoin('housing_estate', 'he', 'he.estate_id = hf.estate_id');
                $query->innerJoin('housing_block', 'hb', 'hb.block_id = hf.block_id'); //added by dg 28-07-2025
                $query->innerJoin('housing_flat_type', 'hft', 'hft.flat_type_id = hf.flat_type_id');
                $query->innerJoin('users', 'u', 'u.uid = haod.uid');
                // $query->leftJoin('users', 'u', 'u.uid = haod.uid');

                $query->condition('hoa.status', 'existing_occupant', '=');
                // $query->condition('haod.is_active', 1, '=');


                $query->addField('hoa', 'online_application_id'); // added by Moumita on 24-06-2025
                

                $query->addField('ha', 'applicant_name');
                $query->addField('he', 'estate_name');
                $query->addField('hft', 'flat_type');
                $query->addField('haod', 'hrms_id');
                $query->addField('u', 'status');
                $query->addField('haod', 'uid');
                $query->addField('hf', 'flat_id');
                $query->addField('hb', 'block_name');

                $db_and = db_and();														// Added by Subham 30-12-2024
                $db_and->condition('he.division_id', $data_fetch->division_id, '=');	// Added by Subham 30-12-2024
                $db_and->condition('he.subdiv_id', $data_fetch->subdiv_id, '=');		// Added by Subham 30-12-2024
                $query->condition($db_and);												// Added by Subham 30-12-2024

                if($rhe_name != 0 && $flat_type != 0 && $block_name != 0){
                    $db_and = db_and();
                    $db_and->condition('hf.estate_id', $rhe_name, '=');
                    $db_and->condition('hf.flat_type_id', $flat_type, '=');
                    $db_and->condition('hf.block_id', $block_name, '=');
                    $query->condition($db_and);
                }

                $result =$query->execute();

            } else{																		// Added by Subham 30-12-2024
                $query = db_select('housing_applicant', 'ha');

                // $query->innerJoin('housing_applicant_official_detail', 'haod', 'ha.uid = haod.uid');  //off by dg 30-12-2024
                $query->innerJoin('housing_applicant_official_detail', 'haod', 'ha.housing_applicant_id = haod.housing_applicant_id');  //added by dg 30-12-2024 
                $query->innerJoin('housing_online_application', 'hoa', 'hoa.applicant_official_detail_id = haod.applicant_official_detail_id');
                $query->innerJoin('housing_flat_occupant', 'hfo', 'hfo.online_application_id = hoa.online_application_id');
                $query->innerJoin('housing_flat', 'hf', 'hf.flat_id = hfo.flat_id');
                $query->innerJoin('housing_estate', 'he', 'he.estate_id = hf.estate_id');
                $query->innerJoin('housing_block', 'hb', 'hb.block_id = hf.block_id');
                $query->innerJoin('housing_flat_type', 'hft', 'hft.flat_type_id = hf.flat_type_id');
                $query->innerJoin('users', 'u', 'u.uid = haod.uid');
                
                $query->condition('hoa.status', 'existing_occupant', '=');
                // $query->condition('haod.is_active', 1, '=');

                $query->addField('hoa', 'online_application_id'); // added by Moumita on 24-06-2025

                $query->addField('ha', 'applicant_name');
                $query->addField('he', 'estate_name');
                $query->addField('hft', 'flat_type');
                $query->addField('haod', 'hrms_id');
                $query->addField('u', 'status');
                $query->addField('haod', 'uid');
                $query->addField('hf', 'flat_id');
                $query->addField('hb', 'block_name');

                $query->condition('he.division_id', $data_fetch->division_id, '=');		// Added by Subham 30-12-2024

                if($rhe_name != 0 && $flat_type != 0 && $block_name != 0){
                    $db_and = db_and();
                    $db_and->condition('hf.estate_id', $rhe_name, '=');
                    $db_and->condition('hf.flat_type_id', $flat_type, '=');
                    $db_and->condition('hf.block_id', $block_name, '=');
                    $query->condition($db_and);
                }

                $result =$query->execute();
            }
        }
        

        $header['Serial No.'] = array('data' => 'Serial No.');
        $header['Applicant Name'] = array('data' => 'Applicant Name');
        $header['HRMS Code'] = array('data' => 'HRMS Code');
        $header['Estate Name'] = array('data' => 'Estate Name');
        $header['Flat Type'] = array('data' => 'Flat Type');
        $header['Block Name'] = array('data' => 'Block Name'); // added by Moumita on 26-06-2025
        $header['Approval Status'] = array('data' => 'Approval Status');
        $header['Details'] = array('data' => 'Details');
        $header['edit'] = array('data' => 'Edit');
        // Added 22-05-2025 by Subham
        if($user_role == 8){
            $header['Action'] = array('data' => 'Action');
        }

        $rows =array();
        $output = '';
        $serialNumber = 1;
    
    while($data = $result->fetchObject()) {
        $fields = array();
    
        $fields[] = $serialNumber;
        $fields[] = $data->applicant_name;
        $fields[] = $data->hrms_id;
        $fields[] = $data->estate_name;
        $fields[] = $data->flat_type;
        $fields[] = $data->block_name;
        if($data->status == 0){
            $fields[] = 'Pending Approval at Division Level';
        }else{
            $fields[] = 'Approved by Division';
        }
        $fields[] = l(
        'View Details',
        'existing-occupant-view-det/'. encrypt_url($data->uid),
        array(
            'html'=>TRUE,
            'attributes'=> array('class'=>array('btn bg-success btn-sm px-4 rounded-pill text-white fw-bolder')),
        ));
        // Added 22-05-2025
        if($user_role == 8){
            /*Added by Moumita 21-05-2025  ~~~~~~ Flat id added in link by Subham dt.22-05-2025*/
            $fields[] = l('<i class="fa fa-trash"></i> Delete', 'rhe-wise-flat-occupant-delete/legacy/'.encrypt_url($data->housing_existing_occupant_draft_id).'/'.encrypt_url($data->flat_id), array('html' => true, 'attributes' => array('class' => array('btn btn-sm bg-primary px-3 rounded-pill text-white fw-bolder'))));	
            /*end*/	
        }

        $fields[] = l('<i class="fa fa-edit"></i> Edit Details', 'existing-occupant-edit/'. encrypt_url($data->online_application_id), array('html' => true, 'attributes' => array('class' => array('btn btn-sm bg-primary px-3 rounded-pill text-white fw-bolder'))));
        
        $serialNumber++;

        $rows[] = $fields;
        
    }

    $variables = array(
        'attributes' => array('class'=>array('table table-list table-striped')),
        'header' => $header,
        'rows' => $rows,
        'sticky' => true,
        'empty' => t("No data found!") // The message to be displayed if table is empty
        );
    //end

    if(count($rows) > 0) {
        // Render using Drupal's render API.
        $build['datatable'] = array(
            '#theme' => 'datatable',
            '#header' => $header,
            '#rows' => $rows,
            '#attributes' => array(),
        );
        
        // Or, render using a theme function.
        $variables = array(
            'attributes' => array('class'=>array('table table-list table-striped')),
            'header' => $header,
            'rows' => $rows,
        );
        
        $output = theme('datatable', $variables);
    }
    else {
        $output = '<div>
                        <table class="datatable_no_data_found table table-list">
                            <tr class="tr_no_data_found">
                                <th class="th_no_data_found"></th>
                            </tr>
                            <tr class="tr_no_data_found">
                                <td class="td_no_data_found">No data found!</td>
                            </tr>
                        </table>
                    </div>';
    }

    return $output;
}

//~~ This function is for other official users except Division & Subdivision ~~\\
// Done by Subham dt.22-08-2025
function existing_occupant_list_whrms_form($form, &$form_state) {

    $form['flat_info'] = array(
        '#prefix' => '<div class="row">',
        '#suffix' => '</div>'
    );

    $form['flat_info']['updated_rhe_name'] = array(
        '#title' => t('Name of the RHE'),
        '#type' => 'select',
        '#options' => rhe_list_fetch(),
        '#attributes' => array('class' => array('form-select')),
        '#required' => TRUE,
        '#prefix' => '<div class="col-md-4"><div class="form-floating">',
        '#suffix' => '</div></div>',
        '#ajax' => array(
            'event' => 'change',
            'callback' => '_reload_flatinfo_callback',
            'method' => 'replace',
        ),
    );

    $rhe_name =  isset($form_state['values']['updated_rhe_name']) && !empty($form_state['values']['updated_rhe_name']) ? $form_state['values']['updated_rhe_name'] : 0;
     
    $form['flat_info']['updated_flat_type'] = array(
        '#prefix' => '<div class="col-md-4" id="flat_type_replace_updated">',
        '#suffix' => '</div>',
    );

    $form['flat_info']['updated_block_name'] = array(
        '#prefix' => '<div class="col-md-6" id="block_name_replace_updated"><div class="form-floating">',
        '#suffix' => '</div></div>',
    );

    if($rhe_name != 0) {
        $form['flat_info']['updated_flat_type'] = array(
            '#title' => t('Flat Type'),
            '#type' => 'select',
            '#options' => flat_type_under_rhe($rhe_name),
            '#required' => TRUE,
            '#prefix' => '<div class="col-md-4" id="flat_type_replace_updated"><div class="form-floating">',
            '#suffix' => '</div></div>',
            '#attributes' => array('class'	=> array('form-select')), 
            '#ajax' => array(
                'event' => 'change',
                'callback' => '_reload_flatinfo_callback',
                'method' => 'replace',
            ),
        );
    }
    
    $flat_type =  isset($form_state['values']['updated_flat_type']) &&!empty($form_state['values']['updated_flat_type']) ? $form_state['values']['updated_flat_type'] : 0;
     
    if($rhe_name != 0 && $flat_type != 0) {
        $form['flat_info']['updated_block_name'] = array(
            '#title' => t('Name of the Block'),
            '#type' => 'select',
            '#options' => block_name_under_rhe($rhe_name, $flat_type),
            '#required' => TRUE,
            '#prefix' => '<div class="col-md-4" id="block_name_replace_updated"><div class="form-floating">',
            '#suffix' => '</div></div>',
            '#attributes' => array('class'	=> array('form-select')), 
            '#ajax' => array(
                'event' => 'change',
                'callback' => '_reload_flatinfo_callback',
                'method' => 'replace',
            ),
        );
    }

    $block_name =  isset($form_state['values']['updated_block_name']) && !empty($form_state['values']['updated_block_name']) ? $form_state['values']['updated_block_name'] : 0;

    $form['fetch_list'] = array(
        '#prefix' => '<div class="col-md-12" id="flat_list_replace">',
        '#suffix' => '</div>'
    );
    
    if($rhe_name != 0 && $flat_type != 0 && $block_name != 0) {
        $form['fetch_list'] = array(
            '#type' => 'markup',
            '#markup' => view_occupant_data($rhe_name, $flat_type, $block_name),
            '#weight' => 50,
            '#prefix' => '<div class="col-md-12" id="flat_list_replace">',
            '#suffix' => '</div>'
        );
    }
    return $form;
}

function view_occupant_data($rhe_name = 0, $flat_type = 0, $block_name = 0) {
    global $base_root, $base_path,$user,$user_role;
		$output = '';

        $query = db_select('housing_applicant', 'ha');

        // $query->innerJoin('housing_applicant_official_detail', 'haod', 'ha.uid = haod.uid');  //off by dg 30-12-2024
        $query->innerJoin('housing_applicant_official_detail', 'haod', 'ha.housing_applicant_id = haod.housing_applicant_id');  //added by dg 30-12-2024 
        $query->innerJoin('housing_online_application', 'hoa', 'hoa.applicant_official_detail_id = haod.applicant_official_detail_id');
        $query->innerJoin('housing_flat_occupant', 'hfo', 'hfo.online_application_id = hoa.online_application_id');
        $query->innerJoin('housing_flat', 'hf', 'hf.flat_id = hfo.flat_id');
        $query->innerJoin('housing_estate', 'he', 'he.estate_id = hf.estate_id');
        $query->innerJoin('housing_block', 'hb', 'hb.block_id = hf.block_id'); //added by dg 28-07-2025
        $query->innerJoin('housing_flat_type', 'hft', 'hft.flat_type_id = hf.flat_type_id');
        $query->innerJoin('users', 'u', 'u.uid = haod.uid');
        // $query->leftJoin('users', 'u', 'u.uid = haod.uid');

        $query->condition('hoa.status', 'existing_occupant', '=');
        // $query->condition('haod.is_active', 1, '=');


        $query->addField('hoa', 'online_application_id'); // added by Moumita on 24-06-2025
        

        $query->addField('ha', 'applicant_name');
        $query->addField('he', 'estate_name');
        $query->addField('hft', 'flat_type');
        $query->addField('haod', 'hrms_id');
        $query->addField('u', 'status');
        $query->addField('haod', 'uid');
        $query->addField('hf', 'flat_id');
        $query->addField('hb', 'block_name');

        if($rhe_name != 0 && $flat_type != 0 && $block_name != 0){
            $db_and = db_and();
            $db_and->condition('hf.estate_id', $rhe_name, '=');
            $db_and->condition('hf.flat_type_id', $flat_type, '=');
            $db_and->condition('hf.block_id', $block_name, '=');
            $query->condition($db_and);
        }

        $result =$query->execute();
        

        $header['Serial No.'] = array('data' => 'Serial No.');
        $header['Applicant Name'] = array('data' => 'Applicant Name');
        $header['HRMS Code'] = array('data' => 'HRMS Code');
        $header['Estate Name'] = array('data' => 'Estate Name');
        $header['Flat Type'] = array('data' => 'Flat Type');
        $header['Block Name'] = array('data' => 'Block Name'); // added by Moumita on 26-06-2025
        $header['Approval Status'] = array('data' => 'Approval Status');
        $header['Details'] = array('data' => 'Details');
        $header['edit'] = array('data' => 'Edit');
        // Added 22-05-2025 by Subham
        if($user_role == 8){
            $header['Action'] = array('data' => 'Action');
        }

        $rows =array();
        $output = '';
        $serialNumber = 1;
    
    while($data = $result->fetchObject()) {
        $fields = array();
    
        $fields[] = $serialNumber;
        $fields[] = $data->applicant_name;
        $fields[] = $data->hrms_id;
        $fields[] = $data->estate_name;
        $fields[] = $data->flat_type;
        $fields[] = $data->block_name;
        if($data->status == 0){
            $fields[] = 'Pending Approval at Division Level';
        }else{
            $fields[] = 'Approved by Division';
        }
        $fields[] = l(
        'View Details',
        'existing-occupant-view-det/'. encrypt_url($data->uid),
        array(
            'html'=>TRUE,
            'attributes'=> array('class'=>array('btn bg-success btn-sm px-4 rounded-pill text-white fw-bolder')),
        ));
        // Added 22-05-2025
        if($user_role == 8){
            /*Added by Moumita 21-05-2025  ~~~~~~ Flat id added in link by Subham dt.22-05-2025*/
            $fields[] = l('<i class="fa fa-trash"></i> Delete', 'rhe-wise-flat-occupant-delete/legacy/'.encrypt_url($data->housing_existing_occupant_draft_id).'/'.encrypt_url($data->flat_id), array('html' => true, 'attributes' => array('class' => array('btn btn-sm bg-primary px-3 rounded-pill text-white fw-bolder'))));	
            /*end*/	
        }

        $fields[] = l('<i class="fa fa-edit"></i> Edit Details', 'existing-occupant-edit/'. encrypt_url($data->online_application_id), array('html' => true, 'attributes' => array('class' => array('btn btn-sm bg-primary px-3 rounded-pill text-white fw-bolder'))));
        
        $serialNumber++;

        $rows[] = $fields;
        
    }

    $variables = array(
        'attributes' => array('class'=>array('table table-list table-striped')),
        'header' => $header,
        'rows' => $rows,
        'sticky' => true,
        'empty' => t("No data found!") // The message to be displayed if table is empty
        );
    //end

    if(count($rows) > 0) {
        // Render using Drupal's render API.
        $build['datatable'] = array(
            '#theme' => 'datatable',
            '#header' => $header,
            '#rows' => $rows,
            '#attributes' => array(),
        );
        
        // Or, render using a theme function.
        $variables = array(
            'attributes' => array('class'=>array('table table-list table-striped')),
            'header' => $header,
            'rows' => $rows,
        );
        
        $output = theme('datatable', $variables);
    }
    else {
        $output = '<div>
                        <table class="datatable_no_data_found table table-list">
                            <tr class="tr_no_data_found">
                                <th class="th_no_data_found"></th>
                            </tr>
                            <tr class="tr_no_data_found">
                                <td class="td_no_data_found">No data found!</td>
                            </tr>
                        </table>
                    </div>';
    }

    return $output;
}

function _reload_flatinfo_callback($form, &$form_state) {
    $commands = array();
    
    $commands[] = ajax_command_replace("#flat_type_replace_updated", drupal_render($form['flat_info']['updated_flat_type']));
    $commands[] = ajax_command_replace("#block_name_replace_updated", drupal_render($form['flat_info']['updated_block_name']));
    $commands[] = ajax_command_replace("#flat_list_replace", drupal_render($form['fetch_list']));

    return array('#type' => 'ajax', '#commands' => $commands);
}
//~~ End ~~\\