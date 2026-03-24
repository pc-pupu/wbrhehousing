<?php // Done by Subham dt. 07-01-2025 & 08-01-2025
function view_unauthorised_occupants_list(){
    $current_date = date('Y-m-d');
    $query = db_select('housing_online_application', 'hoa');
    $query->innerJoin('housing_applicant_official_detail', 'haod', 'hoa.applicant_official_detail_id = haod.applicant_official_detail_id');
    $query->innerJoin('housing_applicant', 'ha','ha.housing_applicant_id = haod.housing_applicant_id');
    $query->innerJoin('housing_flat_occupant', 'hfo','hoa.online_application_id = hfo.online_application_id');
    $query->leftJoin('housing_occupant_license', 'hol', 'hol.flat_occupant_id = hfo.flat_occupant_id');
    $query->addField('ha', 'applicant_name');
    $query->addField('hoa', 'application_no');
    $query->fields('hol', array('license_no', 'possession_date', 'license_expiry_date', 'flat_occupant_id', 'authorised_or_not','existing_occupant_license_no'));
    //$query->condition('hol.license_expiry_date', $current_date, '<');
    $or = db_or()
          //->isNull('hol.authorised_or_not')  //off by dg 02-07-2025
          ->condition('hol.license_expiry_date', $current_date, '<') //added by dg 02-07-2025
          ->condition('hol.authorised_or_not', 'unauthorised');

    $query->condition($or);
    //$query->condition('hol.authorised_or_not', NULL, 'IS');
    $result = $query->execute();
    // echo $query;die;
    
    $header['Sl No.'] = array('data' => 'Sl No.');
    $header['Applicant Name'] = array('data' => 'Applicant Name');
    $header['Application No.'] = array('data' => 'Application No.');
    $header['License No.'] = array('data' => 'License No.');
    $header['Possession Date'] = array('data' => 'Possession Date');
    $header['License Expiry Date'] = array('data' => 'License Expiry Date');
    $header['Action'] = array('data' => 'Action');

    $rows = array();
    $output = '';
    $serialNumber = 1;
   
    while ($data = $result->fetchObject()) {
        $fields = array();

        $fields[] = $serialNumber;
        $fields[] = $data->applicant_name;
        $fields[] = $data->application_no;
        $fields[] = $data->license_no;
        $fields[] = $data->possession_date ? date('d-m-Y', strtotime($data->possession_date)) : '';
        $fields[] = $data->license_expiry_date ? date('d-m-Y', strtotime($data->license_expiry_date)) : '';
        $onclick = "return confirm('Are you sure you want to mark this applicant as Unauthorised?')";

        if($data->authorised_or_not == '' || ($data->authorised_or_not == 'authorised' && $data->license_expiry_date < $current_date)){
           $fields[] = l('<span class="btn bg-info btn-sm text-black rounded" style="font-size:12px"><b>Mark as Unauthorised</b></span>',
            'view-unauthorised-occupants-store/' . encrypt_url($data->flat_occupant_id), array('html' => TRUE,'attributes' => array('onclick' => $onclick))
            ); 
        }else if($data->authorised_or_not == 'unauthorised'){
            $fields[] = 'Marked as '.$data->authorised_or_not;
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

    if(count($rows) > 0) {
        $build['datatable'] = array(
            '#theme' => 'datatable',
            '#header' => $header,
            '#rows' => $rows,
            '#attributes' => array(),
        );

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

/* Done by Subham 08-01-2025 */
function unauthorised_occupants_list_store($flat_occupant_id) {
    $dc_flat_occupant_id = decrypt_url($flat_occupant_id);

    db_update('housing_occupant_license')
        ->fields(array(
            'authorised_or_not' => 'unauthorised',
        ))
        ->condition('flat_occupant_id', $dc_flat_occupant_id, '=')
        ->execute();

    drupal_set_message(t('Successfully marked as Unauthorised!'));
    drupal_goto('view-unauthorised-occupants');
}

function unauthorized_occupant_draft_list($form, $form_state){

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
            'callback' => '_reload_rhewise_list_view_draft',
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
                'callback' => '_reload_rhewise_list_view_draft',
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
                'callback' => '_reload_rhewise_list_view_draft',
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
            '#markup' => view_unauthorized_list($rhe_name, $flat_type, $block_name),
            '#weight' => 50,
            '#prefix' => '<div class="col-md-12" id="flat_list_replace">',
            '#suffix' => '</div>'
        );
    }

    return $form;    
}

function _reload_rhewise_list_view_draft($form, &$form_state) {
    $commands = array();
    
    $commands[] = ajax_command_replace("#flat_type_replace", drupal_render($form['list_info']['flat_type']));
    $commands[] = ajax_command_replace("#block_name_replace", drupal_render($form['list_info']['block_name']));
    $commands[] = ajax_command_replace("#flat_list_replace", drupal_render($form['fetch_list']));

    return array('#type' => 'ajax', '#commands' => $commands);
}

function view_unauthorized_list($rhe_id = 0, $flat_type_id = 0, $block_id = 0){

    // echo $rhe_id.'--'.$flat_type_id.'---'.$block_id; die;

    $query = db_select('housing_existing_occupant_draft', 'heod');
    $query->fields('heod',array());
    $query->innerJoin('housing_flat','hf','hf.flat_id = heod.flat_id');
    $query->fields('hf',array());
    $query->innerJoin('housing_flat_type','hft','hft.flat_type_id = hf.flat_type_id');
    $query->fields('hft',array());
    $query->innerJoin('housing_block','hb','hb.block_id = hf.block_id');
    $query->fields('hb',array());
    $query->innerJoin('housing_estate','he','he.estate_id = hf.estate_id');
    $query->fields('he',array('estate_id','district_code','estate_name'));
    $query->condition('authorised_or_not','unauthorised');
    
    if($rhe_id != 0 && $flat_type_id != 0 && $block_id != 0){
        $query->condition('he.estate_id',$rhe_id);
        $query->condition('hft.flat_type_id',$flat_type_id);
        $query->condition('hb.block_id',$block_id);

    }
    $result = $query->execute();
    // print_r($result);
    // die;
    // echo $result->rowCount(); die;

    if($result->rowCount() > 0){
        $header['Serial No.'] = array('data' => 'Serial No.');
        $header['Applicant Name'] = array('data' => 'Applicant Name');
        $header['Authorised or Not'] = array('data' => 'Authorised or Not');
        $header['Estate Name'] = array('data' => 'Estate Name');
        $header['Flat Type'] = array('data' => 'Flat Type');
        $header['Block Name'] = array('data' => 'Block Name'); // added by Moumita on 26-06-2025
        $header['Flat No.'] = array('data' => 'Flat No.');
        $header['Floor'] = array('data' => 'Floor');
        // $header['Details'] = array('data' => 'Details');
        // $header['Edit'] = array('data' => 'Edit');

        $c =1;
        while($data = $result->fetchObject()) {
            $fields = array();

            $fields[] = $c;
            $fields[] = $data->applicant_name;
            $fields[] = ucwords($data->authorised_or_not);
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
            // $fields[] = l(
            // 'View Details',
            // 'existing-occupant-view-det-draft/'. encrypt_url($data->housing_existing_occupant_draft_id),
            // array(
            //     'html'=>TRUE,
            //     'attributes'=> array('class'=>array('btn bg-success btn-sm px-4 rounded-pill text-white fw-bolder')),
            // ));	

            // $fields[] = l('<i class="fa fa-edit"></i> Edit Details', 'existing-occupant-draft-edit/'. encrypt_url($data->housing_existing_occupant_draft_id), array('html' => true, 'attributes' => array('class' => array('btn btn-sm bg-primary px-3 rounded-pill text-white fw-bolder'))));

            
            $c++;

            $rows[] = $fields;    

            
        }
        $variables = array(
            'attributes' => array('class'=>array('table table-list table-striped')),
            'header' => $header,
            'rows' => $rows,
            'sticky' => true,
            'empty' => t("No data found!")
        );
        
        $variables = array(
            'attributes' => array('class'=>array('table table-list table-striped')),
            'header' => $header,
            'rows' => $rows,
        );
        
        $output = theme('datatable', $variables);

    }else {
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

