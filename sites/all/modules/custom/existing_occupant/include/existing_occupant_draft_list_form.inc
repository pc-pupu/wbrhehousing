<?php
/*Done by Subham dt.24-06-2025*/
function existing_occupant_draft_list_form($form, &$form_state) {
    $rhe_name =  isset($form_state['values']['rhe_name']) && !empty($form_state['values']['rhe_name']) ? $form_state['values']['rhe_name'] : 0;
    $flat_type =  isset($form_state['values']['flat_type']) &&!empty($form_state['values']['flat_type']) ? $form_state['values']['flat_type'] : 0;
    $block_name =  isset($form_state['values']['block_name']) &&!empty($form_state['values']['block_name']) ? $form_state['values']['block_name'] : 0;
    
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
            '#markup' => display_specific_flat_list($rhe_name, $flat_type, $block_name),
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

function display_specific_flat_list($rhe_name = 0, $flat_type = 0, $block_name = 0) {
    global $base_root, $base_path,$user, $user_role;
    $output = '';
    $query_dtls = db_select('users_details', 'ud'); 
    $query_dtls->fields('ud');
    $query_dtls->condition('uid',$user->uid,'=');
    $rsult = $query_dtls->execute();
    $data_fetch = $rsult->fetchObject();

    if($data_fetch->division_id != '' && $data_fetch->subdiv_id != '') { 
        if($data_fetch->subdiv_id != 0) {								
            $query = db_select('housing_existing_occupant_draft', 'heod');
            
            $query->innerJoin('housing_flat', 'hf', 'hf.flat_id = heod.flat_id');
            $query->innerJoin('housing_block', 'hb', 'hb.block_id = hf.block_id');
            $query->innerJoin('housing_estate', 'he', 'he.estate_id = hf.estate_id');
            $query->innerJoin('housing_flat_type', 'hft', 'hft.flat_type_id = hf.flat_type_id');
            $query->innerJoin('housing_district', 'hd', 'he.district_code = hd.district_code');

            $query->addField('heod', 'applicant_name');
            $query->fields('he', array('estate_name', 'estate_address'));
            $query->addField('hft', 'flat_type');
            $query->fields('hf', array('floor', 'flat_no'));
            $query->addField('hd', 'district_name');
            $query->addField('hb', 'block_name');
            $query->addField('heod', 'housing_existing_occupant_draft_id');

            $db_and = db_and();							
            $db_and->condition('he.division_id', $data_fetch->division_id, '=');	
            $db_and->condition('he.subdiv_id', $data_fetch->subdiv_id, '=');		
            $query->condition($db_and);

            /* added by dg 11-06-2025 */
            $query->addExpression(
                "CASE
                    WHEN hf.flat_no ~ '^[0-9]+$' THEN CAST(hf.flat_no AS INTEGER) 
                    WHEN hf.flat_no ~ '^([0-9]+)/([0-9]+)$' THEN CAST(SPLIT_PART(hf.flat_no, '/', 1) AS INTEGER) 
                    WHEN hf.flat_no ~ '^[0-9]+' THEN CAST(SUBSTRING(hf.flat_no FROM '^[0-9]+') AS INTEGER) 
                    ELSE NULL 
                END",
                'flat_no_sort_key'
            );

            $query->orderBy('hb.block_name', 'ASC');
            $query->orderBy('flat_no_sort_key', 'ASC', 'NULLS LAST');
            $query->orderBy('hf.flat_no', 'ASC');
            /* end */

            if($rhe_name != 0 && $flat_type != 0 && $block_name != 0){
                $db_and = db_and();
                $db_and->condition('hf.estate_id', $rhe_name, '=');
                $db_and->condition('hf.flat_type_id', $flat_type, '=');
                $db_and->condition('hf.block_id', $block_name, '=');
                $query->condition($db_and);
            }
            $result =$query->execute();

        } else{																		
            $query = db_select('housing_existing_occupant_draft', 'heod');
            
            $query->innerJoin('housing_flat', 'hf', 'hf.flat_id = heod.flat_id');
            $query->innerJoin('housing_block', 'hb', 'hb.block_id = hf.block_id');
            $query->innerJoin('housing_estate', 'he', 'he.estate_id = hf.estate_id');
            $query->innerJoin('housing_flat_type', 'hft', 'hft.flat_type_id = hf.flat_type_id');
            $query->innerJoin('housing_district', 'hd', 'he.district_code = hd.district_code');

            $query->addField('heod', 'applicant_name');
            $query->fields('he', array('estate_name', 'estate_address'));
            $query->addField('hft', 'flat_type');
            $query->fields('hf', array('floor', 'flat_no'));
            $query->addField('hd', 'district_name');
            $query->addField('hb', 'block_name');
            $query->addField('heod', 'housing_existing_occupant_draft_id');

            $query->condition('he.division_id', $data_fetch->division_id, '=');

            /* added by dg 11-06-2025 */
            $query->addExpression(
                "CASE
                    WHEN hf.flat_no ~ '^[0-9]+$' THEN CAST(hf.flat_no AS INTEGER) 
                    WHEN hf.flat_no ~ '^([0-9]+)/([0-9]+)$' THEN CAST(SPLIT_PART(hf.flat_no, '/', 1) AS INTEGER) 
                    WHEN hf.flat_no ~ '^[0-9]+' THEN CAST(SUBSTRING(hf.flat_no FROM '^[0-9]+') AS INTEGER) 
                    ELSE NULL 
                END",
                'flat_no_sort_key'
            );

            $query->orderBy('hb.block_name', 'ASC');
            $query->orderBy('flat_no_sort_key', 'ASC', 'NULLS LAST');
            $query->orderBy('hf.flat_no', 'ASC');

            /* end */

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
    $header['District'] = array('data' => 'District');
    $header['Estate Name'] = array('data' => 'Estate Name');
    $header['Flat Type'] = array('data' => 'Flat Type');
    $header['Block Name'] = array('data' => 'Block Name'); // added by Moumita on 26-06-2025
    $header['Flat No.'] = array('data' => 'Flat No.');
    $header['Floor'] = array('data' => 'Floor');
    $header['Details'] = array('data' => 'Details');
    $header['Edit'] = array('data' => 'Edit');
    if($user_role == 8) {
        $header['Delete'] = array('data' => 'Delete');
    }
    $rows =array();
    $output = '';
    $serialNumber = 1;

    while($data = $result->fetchObject()) {
        // print_r($data);die;
        $fields = array();

        $fields[] = $serialNumber;
        $fields[] = $data->applicant_name;
        $fields[] = $data->district_name;
        $fields[] = $data->estate_name;
        $fields[] = $data->flat_type;
        $fields[] = $data->block_name;
        $fields[] = $data->flat_no;
        $fields[] = $data->floor;
        // if($data->status == 0){
        // 	$fields[] = 'Pending Approval at Division Level';
        // }else{
        // 	$fields[] = 'Approved by Division';
        // }
        $fields[] = l(
        'View Details',
        'existing-occupant-view-det-draft/'. encrypt_url($data->housing_existing_occupant_draft_id),
        array(
            'html'=>TRUE,
            'attributes'=> array('class'=>array('btn bg-success btn-sm px-4 rounded-pill text-white fw-bolder')),
        ));	

        $fields[] = l('<i class="fa fa-edit"></i> Edit Details', 'existing-occupant-draft-edit/'. encrypt_url($data->housing_existing_occupant_draft_id), array('html' => true, 'attributes' => array('class' => array('btn btn-sm bg-primary px-3 rounded-pill text-white fw-bolder'))));

        if($user_role == 8) {
            $onclick = 'Are you want to delete?';
            /*Added by Moumita 21-05-2025  ~~~~~~ Flat id added in link by Subham dt.22-05-2025*/
            $fields[] = l('<i class="fa fa-trash"></i> Delete', 'rhe-wise-flat-occupant-delete/draft/'.encrypt_url($data->housing_existing_occupant_draft_id).'/'.encrypt_url($data->flat_id), array('html' => true, 'attributes' => array('onclick' => 'return confirm("Are you sure you want to Delete?")', 'class' => array('btn btn-sm bg-primary px-3 rounded-pill text-white fw-bolder'))));	
            /*end*/
        }
        
        $serialNumber++;

        $rows[] = $fields;
        
    }

    $variables = array(
        'attributes' => array('class'=>array('table table-list table-striped')),
        'header' => $header,
        'rows' => $rows,
        'sticky' => true,
        'empty' => t("No data found!")
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

function existing_occupant_draft_edit_form($form, &$form_state, $en_row = 0){
    $row_id = decrypt_url($en_row); 
    if($row_id != 0){

        $query = db_select('housing_existing_occupant_draft','heod');
        $query->leftJoin('housing_ddo', 'hdd', 'heod.ddo_id = hdd.ddo_id');
        $query->fields('heod',array());
        $query->addField('hdd', 'district_code');
        $query->condition('heod.housing_existing_occupant_draft_id', $row_id);
        $result = $query->execute();
        
        $data = $result->fetchObject();

        // echo '<pre>'; print_r($data); die;
        // ddo info
    
        $form['hidden_row_id'] = array(
            '#type' => 'hidden',
            '#value' => $row_id,
            '#attributes' => array('readonly' => 'readonly')
        );

    //personal information
    $form['prrsonal_info'] = array(
        '#type' => 'fieldset',
        '#title' => t('Occupant\'s Personal Information(According to Data Entry)'),
        //'#collapsible' => TRUE, // Added
        //'#collapsed' => TRUE,  // Added
        '#collapsible' => FALSE, // Added
        '#collapsed' => FALSE,  // Added
    );
                    
    $form['prrsonal_info']['occupant_name'] = array(
        '#title' => t('Occupant\'s Name'),
        '#type' => 'textfield',
        '#prefix' => '<div class="row"><div class="col-md-6"><div class="form-floating">',
        '#suffix' => '</div></div>',
        '#default_value' => $data->applicant_name,
        '#element_validate' => array('element_validate_alphabatic_fullstop'),
        '#attributes' => array('oninput'=>"this.value=this.value.toUpperCase()",'class'=>array('form-control')), 
        '#required' => TRUE,
    );
			
    $form['prrsonal_info']['occupant_father_name'] = array(
        '#title' => t('Father\'s / Husband\'s Name'),
        '#type' => 'textfield',
        '#default_value' => $data->guardian_name,
        '#prefix' => '<div class="col-md-6"><div class="form-floating">',
        '#suffix' => '</div></div></div>',
        '#attributes' => array('oninput'=>"this.value=this.value.toUpperCase()",'class'=>array('form-control')),
        // '#required' => TRUE,
    );
			
    //Permanent Address
    $form['prrsonal_info']['permanent_address'] = array(
        '#type' => 'fieldset',
        '#title' => t('Permanent Address'),
        '#collapsible' => FALSE, // Added
        '#collapsed' => FALSE,  // Added
    );
    $form['prrsonal_info']['permanent_address']['permanent_street'] = array(
        '#title' => t('Address'),
        '#type' => 'textfield',
        // '#required' => TRUE,
        '#default_value' => $data->permanent_street,
        '#prefix' => '<div class="row"><div class="col-md-6"><div class="form-floating">',
        '#suffix' => '</div></div>',
        //'#element_validate' => array('element_validate_streetaddress'), 
        '#attributes' => array('oninput'=>"this.value=this.value.toUpperCase()",'class'=>array('form-control')),
    );

    $form['prrsonal_info']['permanent_address']['permanent_city_town_village'] = array(
        '#title' => t('City / Town / Village'),
        '#type' => 'textfield',
        // '#required' => TRUE,
        '#default_value' => $data->permanent_city_town_village,
        '#prefix' => '<div class="col-md-6"><div class="form-floating">',
        '#suffix' => '</div></div></div>',
        //'#element_validate' => array('element_validate_alphabatic_text'), 
        '#attributes' => array('oninput'=>"this.value=this.value.toUpperCase()",'class'=>array('form-control')),
    );

    $form['prrsonal_info']['permanent_address']['permanent_post_office'] = array(
        '#title' => t('Post Office'),
        '#type' => 'textfield',
        // '#required' => TRUE,
        '#default_value' => $data->permanent_post_office,
        '#prefix' => '<div class="row"><div class="col-md-6"><div class="form-floating">',
        '#suffix' => '</div></div>',
        //'#element_validate' => array('element_validate_alphabatic_text'), 
        '#attributes' => array('oninput'=>"this.value=this.value.toUpperCase()",'class'=>array('form-control')),
    );
    $form['prrsonal_info']['permanent_address']['permanent_district'] = array(
        '#title' => t('District'),
        '#type' => 'select',
        '#options' => district_list(),
        // '#required' => TRUE,
        '#default_value' => $data->permanent_district,
        '#prefix' => '<div class="col-md-6"><div class="form-floating">',
        '#suffix' => '</div></div></div>',
        '#attributes' => array('class'=>array('form-select')),
        // '#select2' => array(
        // 	'width' => '500',
        // 	//'placeholder' => '- Select -',
        // 	'allowClear' => TRUE
    //      	)
    );

    $form['prrsonal_info']['permanent_address']['permanent_pincode'] = array(
        '#title' => t('Pincode'),
        '#type' => 'textfield',
        '#maxlength' => 6,
        // '#required' => TRUE,
        '#default_value' => $data->permanent_pincode,
        //'#element_validate' => array('element_validate_numeric_positive'), 
        '#prefix' => '<div class="row"><div class="col-md-6"><div class="form-floating">',
        '#suffix' => '</div></div></div>',
        '#attributes' => array('class' => array('numeric_positive'),'class'=>array('form-control')),
    );
    //end	
			
    //Present Address
    $form['prrsonal_info']['present_address'] = array(
        '#type' => 'fieldset',
        '#title' => t('Present Address'),
        '#collapsible' => FALSE, // Added
        '#collapsed' => FALSE,  // Added
        '#prefix' => '<div id="reload_present_address">',
        '#suffix' => '</div>',
    );
    
    $form['prrsonal_info']['present_address']['present_street'] = array(
        '#title' => t('Address'),
        '#type' => 'textfield',
        // '#required' => TRUE,
        '#default_value' => $data->present_street,
        '#prefix' => '<div class="row"><div class="col-md-6" id="reload_present_street"><div class="form-floating">',
        '#suffix' => '</div></div>',
        '#element_validate' => array('element_validate_streetaddress'), 
        '#attributes' => array('oninput'=>"this.value=this.value.toUpperCase()",'class'=>array('form-control')), //'id' => 'present_street', 
    );
    $form['prrsonal_info']['present_address']['present_city_town_village'] = array(
        '#title' => t('City / Town / Village'),
        '#type' => 'textfield',
        // '#required' => TRUE,
        '#default_value' => $data->present_city_town_village,
        '#prefix' => '<div class="col-md-6" id="reload_present_city_town_village"><div class="form-floating">',
        '#suffix' => '</div></div></div>',
        '#element_validate' => array('element_validate_alphabatic_text'), 
        '#attributes' => array('oninput'=>"this.value=this.value.toUpperCase()",'class'=>array('form-control')), //'id' => 'present_city_town_village', 
    );
    $form['prrsonal_info']['present_address']['present_post_office'] = array(
        '#title' => t('Post Office'),
        '#type' => 'textfield',
        // '#required' => TRUE,
        '#default_value' => $data->present_post_office,
        '#prefix' => '<div class="row"><div class="col-md-6" id="reload_present_post_office"><div class="form-floating">',
        '#suffix' => '</div></div>',
        '#element_validate' => array('element_validate_alphabatic_text'), 
        '#attributes' => array('oninput'=>"this.value=this.value.toUpperCase()",'class'=>array('form-control')), //'id' => 'present_post_office', 
    );
    $form['prrsonal_info']['present_address']['present_district'] = array(
        '#title' => t('District'),
        '#type' => 'select',
        '#options' => district_list(),
        // '#required' => TRUE,
        '#default_value' => $data->present_post_office,
        '#prefix' => '<div class="col-md-6" id="reload_present_district"><div class="form-floating">',
        '#suffix' => '</div></div></div>',
        '#attributes' => array('class'=>array('form-select')),
    );

    $form['prrsonal_info']['present_address']['present_pincode'] = array(
        '#title' => t('Pincode'),
        '#type' => 'textfield',
        '#maxlength' => 6,
        // '#required' => TRUE,
        '#default_value' => $data->present_pincode,
        // '#element_validate' => array('element_validate_numeric_positive'), 
        '#prefix' => '<div class="row"><div class="col-md-6" id="reload_present_pincode"><div class="form-floating">',
        '#suffix' => '</div></div></div>',
        '#attributes' => array('class' => array('numeric_positive'),'class'=>array('form-control')), //'id' => 'present_pincode', 
    );

    //end
		
    $form['prrsonal_info']['mobile'] = array(
        '#title' => t('Mobile no'),
        '#type' => 'textfield',
        '#default_value' => '',
        '#maxlength' => 10,
        // '#required' => TRUE,
        '#default_value' => $data->mobile_no,
        '#prefix' => '<div class="row"><div class="col-md-6"><div class="form-floating">',
        '#suffix' => '</div></div>',
        //'#element_validate' => array('element_validate_mobile'),
        '#attributes' => array('id' => 'mobile_no','class'=>array('form-control')),
    );
    
    $form['prrsonal_info']['email'] = array(
        '#title' => t('Email ID'),
        '#type' => 'textfield',
        '#prefix' => '<div class="col-md-6"><div class="form-floating">',
        '#suffix' => '</div></div></div>',
        // '#required' => TRUE,
        '#default_value' => $data->email,
        //'#element_validate' => array('element_validate_email'),
        '#attributes' => array('oninput'=>"this.value=this.value.toLowerCase()",'class'=>array('form-control')),
    );
    
    $form['prrsonal_info']['dob'] = array(
        '#title' => t('Date of Birth(According to Service Book)'),
        '#type' => 'textfield',
        '#attributes' => array('readonly' => 'readonly', 'id' => 'edit-dob','class'=>array('form-control')),
        // '#required' => TRUE,
        '#default_value' => date('d/m/Y',strtotime($data->date_of_birth)),
        //'#validated' => TRUE,
        '#prefix' => '<div class="row"><div class="col-md-6"><div class="form-floating">',
        '#suffix' => '</div></div>',
        //'#element_validate' => array('element_validate_date', 'element_validate_age'),
    );
			
    $form['prrsonal_info']['gender'] = array(
        '#title' => t('Gender'),
        '#type' => 'radios',
        '#options' => array('M' => 'Male', 'F' => 'Female'),
        '#default_value' => $data->gender,
        '#prefix' => '<div class="col-md-6"><div class="form-floating">',
        '#suffix' => '</div></div></div>',
        '#required' => TRUE,
    );
    
    // occupant's official information
    $form['official_info'] = array(
        '#type' => 'fieldset',
        '#title' => t('Occupant\'s Official Information'),
        '#collapsible' => FALSE, // Added
        '#collapsed' => FALSE,  // Added
    );
    
    $form['official_info']['hrms_id'] = array(
        '#title' => t('Employee HRMS ID'),
        '#type' => 'textfield',
        '#default_value' => '',
        //'#default_value' => $hrms_id,
        //for api or hrms custom table
        //'#attributes' => array('readonly' => 'readonly', 'class' => array('numeric_positive')),
        '#maxlength' => 10,
        '#attributes' => array('class' => array('numeric_positive'),'class'=>array('form-control')),
        '#prefix' => '<div class="row"><div class="col-md-6"><div class="form-floating">',
        '#suffix' => '</div></div>',
        //'#element_validate' => array('element_validate_hrms_id'),
        // '#required' => TRUE,
    );
    
    $form['official_info']['occupant_designation'] = array(
        '#title' => t('Designation'),
        '#type' => 'textfield',
        // '#required' => TRUE,
        '#default_value' => $data->applicant_designation,
        '#prefix' => '<div class="col-md-6"><div class="form-floating">',
        '#suffix' => '</div></div></div>',
        //'#element_validate' => array('element_validate_textarea'),
        '#attributes' => array('oninput'=>"this.value=this.value.toUpperCase()",'class'=>array('form-control')),
        //'#attributes' => array('id' => 'edit-app-designation', 'oncopy' => "return false", 'onpaste' => "return false"),
    );

    $form['official_info']['pay_band'] = array(
        '#title' => t('Basic Pay Range'),
        '#type' => 'select',
        '#options' => pay_band_list(),
        // '#required' => TRUE,
        '#default_value' => $data->pay_band_id,
        '#prefix' => '<div class="row"><div class="col-md-6"><div class="form-floating">',
        '#suffix' => '</div></div>',
        '#validated' => TRUE,
        '#attributes' => array('class'=>array('form-control')),
        
    );
    $form['official_info']['pay_in'] = array(
        '#title' => t('Basic Pay'),
        '#type' => 'textfield',
        // '#required' => TRUE,
        '#default_value' => $data->pay_in_the_pay_band,
        //'#element_validate' => array('element_validate_numeric_positive'), 
        '#prefix' => '<div class="col-md-6"><div class="form-floating">',
        '#suffix' => '</div></div></div>',
        '#attributes' => array('class' => array('numeric_positive'),'class'=>array('form-control')),
    );

    
    $form['official_info']['occupant_posting_place'] = array(
        '#title' => t('Place of Posting'),
        '#type' => 'textfield',
        // '#required' => TRUE,
        '#default_value' => $data->applicant_posting_place,
        '#prefix' => '<div class="row"><div class="col-md-6"><div class="form-floating">',
        '#suffix' => '</div></div>',
        //'#element_validate' => array('element_validate_alphabatic_text'),
        '#attributes' => array('oninput'=>"this.value=this.value.toUpperCase()",'class'=>array('form-control')),  
    ); 
    $form['official_info']['occupant_headquarter'] = array(
        '#title' => t('Headquarter'),
        '#type' => 'textfield',
        // '#required' => TRUE,
        '#default_value' => $data->applicant_headquarter,
        '#prefix' => '<div class="col-md-6"><div class="form-floating">',
        '#suffix' => '</div></div></div>',
        //'#element_validate' => array('element_validate_alphabatic_text'), 
        '#attributes' => array('oninput'=>"this.value=this.value.toUpperCase()",'class'=>array('form-control')), 
    );
    $form['official_info']['doj'] = array(
        '#title' => t('Date of Joining'),
        '#type' => 'textfield',
        '#attributes' => array('readonly' => 'readonly', 'id' => 'edit-doj','class'=>array('form-control')),
        '#default_value' => '',
        // '#required' => TRUE,
        '#default_value' => date('d/m/Y',strtotime($data->date_of_joining)),
        '#prefix' => '<div class="row"><div class="col-md-6"><div class="form-floating">',
        '#suffix' => '</div></div>',
        //'#element_validate' => array('element_validate_date', 'element_validate_joining_age'),
    );
    
    $form['official_info']['dor'] = array(
        '#title' => t('Date of Retirement(According to Service Book)'),
        '#type' => 'textfield',
        '#attributes' => array('readonly' => 'readonly', 'id' => 'edit-dor','class'=>array('form-control')),
        '#default_value' => '',
        // '#required' => TRUE,
        '#default_value' => date('d/m/Y',strtotime($data->date_of_retirement)),
        '#prefix' => '<div class="col-md-6"><div class="form-floating">',
        '#suffix' => '</div></div></div>',
        //'#element_validate' => array('element_validate_date', 'element_validate_joining_age'),
    );
    
    
    // occupant's office address
    $form['office_add'] = array(
        '#type' => 'fieldset',
        '#title' => t('Name and Address of the Office'),
        '#collapsible' => FALSE, // Added
        '#collapsed' => FALSE,  // Added
    );
    $form['office_add']['office_name'] = array(
        '#title' => t('Name of the Office'),
        '#type' => 'textfield',
        // '#required' => TRUE,
        '#default_value' => $data->office_name,
        '#prefix' => '<div class="row"><div class="col-md-6"><div class="form-floating">',
        '#suffix' => '</div></div>',
        //'#element_validate' => array('element_validate_textarea'),
        '#attributes' => array('oninput'=>"this.value=this.value.toUpperCase()",'class'=>array('form-control')),  
    );
    
    $form['office_add']['office_street'] = array(
        '#title' => t('Address of the Office'),
        '#type' => 'textfield',
        // '#required' => TRUE,
        '#default_value' => $data->office_street,
        '#prefix' => '<div class="col-md-6"><div class="form-floating">',
        '#suffix' => '</div></div></div>',
        //'#element_validate' => array('element_validate_streetaddress'), 
        '#attributes' => array('oninput'=>"this.value=this.value.toUpperCase()",'class'=>array('form-control')), 
    );
    $form['office_add']['office_city'] = array(
        '#title' => t('City / Town / Village'),
        '#type' => 'textfield',
        // '#required' => TRUE,
        '#default_value' => $data->office_city_town_village,
        '#prefix' => '<div class="row"><div class="col-md-6"><div class="form-floating">',
        '#suffix' => '</div></div>',
        //'#element_validate' => array('element_validate_alphabatic_text'), 
        '#attributes' => array('oninput'=>"this.value=this.value.toUpperCase()",'class'=>array('form-control')), 
    );
    $form['office_add']['office_post_office'] = array(
        '#title' => t('Post Office'),
        '#type' => 'textfield',
        // '#required' => TRUE,
        '#default_value' => $data->office_post_office,
        '#prefix' => '<div class="col-md-6"><div class="form-floating">',
        '#suffix' => '</div></div></div>',
        //'#element_validate' => array('element_validate_alphabatic_text'), 
        '#attributes' => array('oninput'=>"this.value=this.value.toUpperCase()",'class'=>array('form-control')), 
    );
    $form['office_add']['office_district'] = array(
        '#title' => t('District'),
        '#type' => 'select',
        '#options' => district_list(),
        // '#required' => TRUE,
        '#default_value' => $data->office_district,
        '#prefix' => '<div class="row"><div class="col-md-6"><div class="form-floating">',
        '#suffix' => '</div></div>',
        '#attributes' => array('class'=>array('form-select')),
        // '#select2' => array(
        // 	'width' => '500',
        // 	//'placeholder' => '- Select -',
        // 	'allowClear' => TRUE
    //      	)
    );
    $form['office_add']['office_pin_code'] = array(
        '#title' => t('Pincode'),
        '#type' => 'textfield',
        '#maxlength' => 6,
        // '#required' => TRUE,
        '#default_value' => $data->office_pin_code,
        // '#element_validate' => array('element_validate_numeric_positive'), 
        '#prefix' => '<div class="col-md-6"><div class="form-floating">',
        '#suffix' => '</div></div></div>',
        '#attributes' => array('class' => array('numeric_positive'),'class'=>array('form-control')),
    );
    $form['office_add']['office_phone_no'] = array(
        '#title' => t('Phone No.(With STD Code)'),
        '#type' => 'textfield',
        '#maxlength' => 15,
        // '#required' => TRUE,
        '#default_value' => $data->office_phone_no,
        //'#element_validate' => array('element_validate_telephoneno'), 
        '#prefix' => '<div class="row"><div class="col-md-6"><div class="form-floating">',
        '#suffix' => '</div></div></div>',
        '#attributes' => array('class' => array('numeric_positive'),'class'=>array('form-control')),
    );
    
    // ddo details
    $form['ddo'] = array(
        '#type' => 'fieldset',
        '#title' => t('DDO with Full Address'),
        '#collapsible' => FALSE, // Added
        '#collapsed' => FALSE,  // Added
    );
    $form['ddo']['district'] = array(
        '#title' => t('DDO District'),
        '#type' => 'select',
        '#options' => district_list(),
        '#ajax' => array(
                    'event' => 'change',
                    'callback' => '_reload_ddo_designation',
                    // 'wrapper' => 'replace_designation',
                    'effect' => 'fade',
                    'progress' => array(
                        'type' => '',
                        'message' => '',
                    ), 
            ),
        // '#required' => TRUE,
        // '#default_value' => $data->ddo_id,
        '#validated' => TRUE,
        '#prefix' => '<div class="row"><div class="col-md-6"><div class="form-floating">',
        '#suffix' => '</div></div>',
        '#attributes' => array('class'=>array('form-select')),
        // '#select2' => array(
        // 	'width' => '500',
        // 	//'placeholder' => '- Select -',
        // 	'allowClear' => TRUE
        // )
    );
    

    $form['ddo']['designation'] = array(
        '#title' => t('DDO Designation'),
        '#type' => 'select',
        '#options' => ddo_desig_list($data->district_code),
        '#ajax' => array(
            'event' => 'change',
            'callback' => '_reload_ddo_address',
            'wrapper' => 'replace_ddo_address',
            'effect' => 'fade',
            'progress' => array(
                'type' => '',
                'message' => '',
            ),
        ),
        '#default_value' => $data->ddo_id, // $designation
        // '#required' => TRUE,
        '#validated' => TRUE,
        '#prefix' => '<div id="replace_designation" class="col-md-6"><div class="form-floating">',
        '#suffix' => '</div></div></div>',
        '#attributes' => array('class'=>array('form-select')),
        // '#select2' => array(
        //  'width' => '500',
        //  //'placeholder' => '- Select -',
        //  'allowClear' => TRUE
        // )
        );

    
    $form['ddo']['address'] = array(
        '#title' => t('DDO Address'),
        '#type' => 'textarea',
        '#attributes' => array('readonly' => 'readonly'),
        '#prefix' => '<div id="replace_ddo_address" class="col-md-6"><div class="form-floating">',
        '#suffix' => '</div></div>',
        //'#// '#required' => TRUE,
        //'#validated' => TRUE,
    );
    //license part//
    $form['license'] = array(
        '#type' => 'fieldset',
        '#title' => t('License Details'),
        '#collapsible' => FALSE, // Added
        '#collapsed' => FALSE,  // Added
    );

    $form['license']['license_no'] = array(
        '#title' => t('License No.'),
        '#type' => 'textfield',
        // '#required' => TRUE,
        '#default_value' => $data->license_no,
        '#prefix' => '<div class="row"><div class="col-md-4"><div class="form-floating">',
        '#suffix' => '</div></div>',
        //'#element_validate' => array('element_validate_alphabatic_text'), 
        '#attributes' => array('oninput'=>"this.value=this.value.toUpperCase()",'class'=>array('form-control')), 
    );

    $form['license']['dol'] = array(
        '#title' => t('License Issue Date'),
        '#type' => 'textfield',
        '#attributes' => array('readonly' => 'readonly', 'id' => 'edit-dol','class'=>array('form-control')),
        // '#required' => TRUE,
        '#default_value' => date('d/m/Y',strtotime($data->license_issue_date)),
        //'#validated' => TRUE,
        '#prefix' => '<div class="col-md-4"><div class="form-floating">',
        '#suffix' => '</div></div>',
        //'#element_validate' => array('element_validate_date', 'element_validate_age'),
    );
    /*Added by Subham 30-06-2025*/
    $form['license']['license_last_renewal'] = array(
        '#title' => t('Last License Renewal Date'),
        '#type' => 'textfield',
        '#attributes' => array('readonly' => 'readonly', 'id' => 'edit-license-renewal-date','class'=>array('form-control')),
        // '#required' => TRUE,
        '#default_value' => date('d/m/Y',strtotime($data->last_license_renewal_date)),
        '#prefix' => '<div class="col-md-4"><div class="form-floating">',
        '#suffix' => '</div></div></div>',
        //'#element_validate' => array('element_validate_date', 'element_validate_age'),
    );
    /*End*/

    $form['license']['authorised_or_not'] = array(
        '#title' => t('Occupant Status'),
        '#type' => 'select',
        //'#options' => pay_band_list(),
        '#options' => ['0' => 'Select', 'authorised' => 'Authorised Occupant', 'unauthorised' => 'Unauthorised Occupant'],
        // '#required' => TRUE,
        '#default_value' => $data->authorised_or_not,
        '#prefix' => '<div class="row"><div class="col-md-6"><div class="form-floating">',
        '#suffix' => '</div></div>',
        '#validated' => TRUE,
        '#attributes' => array('class'=>array('form-control')),
        
    );

    $form['license']['uploaded_licence'] = array(
        '#title' => t('Upload Current Licence'),
        '#type' => 'managed_file',
        '#description' => t('<b>Allowed Extension: pdf<br>Maximum File Size: 1 MB</b>'),
        '#progress_message' => t('Please wait...'),
        '#progress_indicator' => 'bar',
        '#default_value' => variable_get('Upload File', ''),
        '#upload_validators' => array(
            'file_validate_extensions' => array("pdf"),
            //'file_validated_size' => array(MAX_FILE_SIZE*1024*1024),
        ),
        '#upload_location' => 'public://doc/',
        // '#required' => TRUE,
        '#process' => array('vertical_shifting_element_process'),
    );
    
    $form['submit_button']=array(
        '#type'=>'submit',
        '#value'=>t('Submit'),
        '#attributes' => array('class'=>array('btn bg-primary btn-sm px-5 rounded-pill text-white fw-bolder')),
    );
                            
    $form['cancel_button'] = array(
        '#type'=>'button',
        '#value'=>t('Cancel'),
        '#attributes' => array('onClick' => 'history.go(-1); event.preventDefault();','class'=>array('btn bg-danger btn-sm px-5 rounded-pill text-white fw-bolder')),
    );
                            
    $form['form_end'] = array(
        '#type' => 'markup',
        '#markup' => '</div><div class="mb-5 pb-3"></div>',
    );
    
    return $form;

    }
}

function existing_occupant_draft_edit_form_submit($form, &$form_state){
    $val = $form_state['values'];

    $updated_array = array(
        'applicant_name' => !empty($val['occupant_name']) ? $val['occupant_name'] : NULL,
    'guardian_name' => !empty($val['occupant_father_name']) ? $val['occupant_father_name'] : NULL,

    'permanent_street' => !empty($val['permanent_street']) ? $val['permanent_street'] : NULL,
    'permanent_city_town_village' => !empty($val['permanent_city_town_village']) ? $val['permanent_city_town_village'] : NULL,
    'permanent_post_office' => !empty($val['permanent_post_office']) ? $val['permanent_post_office'] : NULL,
    'permanent_district' => !empty($val['permanent_district']) ? $val['permanent_district'] : NULL,
    'permanent_pincode' => !empty($val['permanent_pincode']) ? $val['permanent_pincode'] : 0,

    'present_street' => !empty($val['present_street']) ? $val['present_street'] : NULL,
    'present_city_town_village' => !empty($val['present_city_town_village']) ? $val['present_city_town_village'] : NULL,
    'present_post_office' => !empty($val['present_post_office']) ? $val['present_post_office'] : NULL,
    'present_pincode' => !empty($val['present_pincode']) ? $val['present_pincode'] : 0,

    'mobile_no' => !empty($val['mobile']) ? $val['mobile'] : 0,
    'email' => !empty($val['email']) ? $val['email'] : NULL,
    'date_of_birth' => !empty($val['dob']) ? date('Y-m-d', strtotime($val['dob'])) : NULL,
    'gender' => !empty($val['gender']) ? $val['gender'] : 'M',

    'applicant_designation' => !empty($val['occupant_designation']) ? $val['occupant_designation'] : NULL,
    
    'pay_band_id' => $val['pay_band'] != '' ? $val['pay_band'] : 0,

    'pay_in_the_pay_band' => $val['pay_in'] != '' ? $val['pay_in'] : 0,


    'applicant_posting_place' => !empty($val['occupant_posting_place']) ? $val['occupant_posting_place'] : NULL,
    'applicant_headquarter' => !empty($val['occupant_headquarter']) ? $val['occupant_headquarter'] : NULL,
    'date_of_joining' => !empty($val['doj']) ? date('Y-m-d', strtotime($val['doj'])) : NULL,
    'date_of_retirement' => !empty($val['dor']) ? date('Y-m-d', strtotime($val['dor'])) : NULL,

    'office_name' => !empty($val['office_name']) ? $val['office_name'] : NULL,
    'office_street' => !empty($val['office_street']) ? $val['office_street'] : NULL,
    'office_city_town_village' => !empty($val['office_city']) ? $val['office_city'] : NULL,
    'office_post_office' => !empty($val['office_post_office']) ? $val['office_post_office'] : NULL,
    'office_district' => !empty($val['office_district']) ? $val['office_district'] : NULL,
    'office_pin_code' => !empty($val['office_pin_code']) ? $val['office_pin_code'] : 0,
    'office_phone_no' => !empty($val['office_phone_no']) ? $val['office_phone_no'] : NULL,

    'ddo_id' => $val['designation'] != '' ? $val['designation'] : 0,

    'license_no' => !empty($val['license_no']) ? $val['license_no'] : NULL,
    'license_issue_date' => !empty($val['dol']) ? date('Y-m-d', strtotime($val['dol'])) : NULL,

    'last_license_renewal_date' => !empty($val['license_last_renewal']) ? date('Y-m-d', strtotime($val['license_last_renewal'])) : NULL, // added on 01-07-2025 by Moumita

    'authorised_or_not' => isset($val['authorised_or_not']) ? $val['authorised_or_not'] : NULL,

    );

    db_update('housing_existing_occupant_draft')->fields($updated_array)->condition('housing_existing_occupant_draft_id',$val['hidden_row_id'])->execute();

    if($val['uploaded_licence'] != 0) {
        $file_licence = file_load($form_state['values']['uploaded_licence']);
        if($file_licence->status==0) {
            $file_licence->status = FILE_STATUS_PERMANENT;
            file_save($file_licence);
            file_usage_add($file_licence, 'existing_occupant', 'Current Licence', $user->uid);
            $applicant_personal_detail_arr['uploaded_licence']  =  $file_licence->fid;	
        }
    }

    drupal_set_message('Data has been updated successfully');

}