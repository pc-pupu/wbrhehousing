<?php
function existing_applicant_vs_cs_form($form, &$form_state) {
    global $user;

    $rhe_name =  isset($form_state['values']['rhe_name']) && !empty($form_state['values']['rhe_name']) ? $form_state['values']['rhe_name'] : 0;
    $flat_type =  isset($form_state['values']['flat_type']) && !empty($form_state['values']['flat_type']) ? $form_state['values']['flat_type'] : 0;
    $block_name =  isset($form_state['values']['block_name']) && !empty($form_state['values']['block_name']) ? $form_state['values']['block_name'] : 0;
    $flat_id =  isset($form_state['values']['flat_no']) && !empty($form_state['values']['flat_no']) ? $form_state['values']['flat_no'] : 0;
    
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

    $form['list_info']['flat_no'] = array(
			'#prefix' => '<div class="three" id="flat_no_replace">',
			'#suffix' => '</div>',
		);
			
		if($rhe_name != 0 && $flat_type != 0 && $block_name != 0) {
			$form['list_info']['flat_no'] = array(
				'#title' => t('Flat No.'),
				'#type' => 'select',
				'#options' => flatno_under_rhe_updated($rhe_name, $flat_type, $block_name),
				'#required' => TRUE,
				//'#weight' => -10,
				'#prefix' => '<div class="col-md-4" id="flat_no_replace"><div class="form-floating">',
				'#suffix' => '</div>',
				//'#attributes' => array('class' => array('dropdown3')),
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
    
    if($rhe_name != 0 && $flat_type != 0 && $block_name != 0 && $flat_id != 0) {
        $form['fetch_list'] = array(
            '#type' => 'markup',
            '#markup' => view_legacy_applicant_details($rhe_name, $flat_type, $block_name, $flat_id),
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
    $commands[] = ajax_command_replace("#flat_no_replace", drupal_render($form['list_info']['flat_no']));
    $commands[] = ajax_command_replace("#flat_list_replace", drupal_render($form['fetch_list']));

    return array('#type' => 'ajax', '#commands' => $commands);
}

function view_legacy_applicant_details($rhe_name = 0, $flat_type = 0, $block_name = 0, $flat_id = 0)
{
     global $base_root, $base_path,$user,$user_role;
		$output = '';
        // echo "<pre>";print_r($flat_id);die;
       
        $query_dtls = db_select('users_details', 'ud'); 
        $query_dtls->fields('ud');
        $query_dtls->condition('uid',$user->uid,'=');
        $rsult = $query_dtls->execute();
        $data_fetch = $rsult->fetchObject();
       

        if($data_fetch->division_id != '' && $data_fetch->subdiv_id != '') { 
            if($data_fetch->subdiv_id != 0) {
                // echo "ppp";die;
                $query = db_select('housing_applicant', 'ha');
                $query->innerJoin('housing_applicant_official_detail', 'haod', 'ha.housing_applicant_id = haod.housing_applicant_id'); 
                $query->innerJoin('housing_online_application', 'hoa', 'hoa.applicant_official_detail_id = haod.applicant_official_detail_id');
                $query->innerJoin('housing_flat_occupant', 'hfo', 'hfo.online_application_id = hoa.online_application_id');
                $query->innerJoin('housing_flat', 'hf', 'hf.flat_id = hfo.flat_id');
                $query->innerJoin('housing_estate', 'he', 'he.estate_id = hf.estate_id');
                $query->innerJoin('housing_block', 'hb', 'hb.block_id = hf.block_id'); 
                $query->innerJoin('housing_flat_type', 'hft', 'hft.flat_type_id = hf.flat_type_id');
                $query->innerJoin('users', 'u', 'u.uid = haod.uid');

                $query->condition('hoa.status', 'existing_occupant', '=');
                // $query->condition('haod.is_active', 1, '=');


                $query->addField('hoa', 'online_application_id'); 
                $query->addField('ha', 'applicant_name');
                $query->addField('he', 'estate_name');
                $query->addField('hft', 'flat_type');
                $query->addField('haod', 'hrms_id');
                $query->addField('u', 'status');
                $query->addField('haod', 'uid');
                $query->addField('hf', 'flat_id');
                $query->addField('hb', 'block_name');
                $query->addField('hf', 'flat_no');

                $db_and = db_and();														
                $db_and->condition('he.division_id', $data_fetch->division_id, '=');	
                $db_and->condition('he.subdiv_id', $data_fetch->subdiv_id, '=');		
                $query->condition($db_and);												

                if($rhe_name != 0 && $flat_type != 0 && $block_name != 0 && $flat_id != 0){
                    $db_and = db_and();
                    $db_and->condition('hf.estate_id', $rhe_name, '=');
                    $db_and->condition('hf.flat_type_id', $flat_type, '=');
                    $db_and->condition('hf.block_id', $block_name, '=');
                    $db_and->condition('hf.flat_id', $flat_id, '=');
                    $query->condition($db_and);
                }

                $result =$query->execute();
                $data = $result->fetchObject();
                // echo $query;die;
                // echo "<pre>";print_r($data);

                if (empty($data)) {
                    // echo "kjh";die;
                    $query1 = db_select('housing_existing_occupant_draft', 'head'); 
                    $query1->innerJoin('housing_flat', 'hf', 'hf.flat_id = head.flat_id');
                    $query1->innerJoin('housing_estate', 'he', 'he.estate_id = hf.estate_id');
                    $query1->innerJoin('housing_block', 'hb', 'hb.block_id = hf.block_id'); 
                    $query1->innerJoin('housing_flat_type', 'hft', 'hft.flat_type_id = hf.flat_type_id');

                    $query1->addField('head', 'applicant_name');
                    $query1->addField('head', 'housing_existing_occupant_draft_id');
                    $query1->addField('he', 'estate_name');
                    $query1->addField('hft', 'flat_type');
                    $query1->addField('hf', 'flat_id');
                    $query1->addField('hb', 'block_name');
                    $query1->addField('hf', 'flat_no');

                    $db_and = db_and();														
                    $db_and->condition('he.division_id', $data_fetch->division_id, '=');	
                    $db_and->condition('he.subdiv_id', $data_fetch->subdiv_id, '=');		
                    $query1->condition($db_and);												

                    if($rhe_name != 0 && $flat_type != 0 && $block_name != 0 && $flat_id != 0){
                        $db_and = db_and();
                        $db_and->condition('hf.estate_id', $rhe_name, '=');
                        $db_and->condition('hf.flat_type_id', $flat_type, '=');
                        $db_and->condition('hf.block_id', $block_name, '=');
                        $db_and->condition('hf.flat_id', $flat_id, '=');
                        $query1->condition($db_and);
                    }

                    $result1 =$query1->execute();
                    $data = $result1->fetchObject();
                //    echo "<pre>";print_r($data);die;
                }
                
            } 
        }else{																		
                $query = db_select('housing_applicant', 'ha');
                $query->innerJoin('housing_applicant_official_detail', 'haod', 'ha.housing_applicant_id = haod.housing_applicant_id');
                $query->innerJoin('housing_online_application', 'hoa', 'hoa.applicant_official_detail_id = haod.applicant_official_detail_id');
                $query->innerJoin('housing_flat_occupant', 'hfo', 'hfo.online_application_id = hoa.online_application_id');
                $query->innerJoin('housing_flat', 'hf', 'hf.flat_id = hfo.flat_id');
                $query->innerJoin('housing_estate', 'he', 'he.estate_id = hf.estate_id');
                $query->innerJoin('housing_block', 'hb', 'hb.block_id = hf.block_id');
                $query->innerJoin('housing_flat_type', 'hft', 'hft.flat_type_id = hf.flat_type_id');
                $query->innerJoin('users', 'u', 'u.uid = haod.uid');
                
                $query->condition('hoa.status', 'existing_occupant', '=');
                // $query->condition('haod.is_active', 1, '=');

                $query->addField('hoa', 'online_application_id');

                $query->addField('ha', 'applicant_name');
                $query->addField('he', 'estate_name');
                $query->addField('hft', 'flat_type');
                $query->addField('haod', 'hrms_id');
                $query->addField('u', 'status');
                $query->addField('haod', 'uid');
                $query->addField('hf', 'flat_id');
                $query->addField('hb', 'block_name');
                $query->addField('hf', 'flat_no');

                $query->condition('he.division_id', $data_fetch->division_id, '=');		

                if($rhe_name != 0 && $flat_type != 0 && $block_name != 0 && $flat_id != 0){
                    $db_and = db_and();
                    $db_and->condition('hf.estate_id', $rhe_name, '=');
                    $db_and->condition('hf.flat_type_id', $flat_type, '=');
                    $db_and->condition('hf.block_id', $block_name, '=');
                    $db_and->condition('hf.flat_id', $flat_id, '=');
                    $query->condition($db_and);
                }

                $result =$query->execute();
                $data = $result->fetchObject();
                // echo "<pre>";print_r($data);die;

                if (empty($data)) {
                    
                    $query1 = db_select('housing_existing_occupant_draft', 'head'); 
                    $query1->innerJoin('housing_flat', 'hf', 'hf.flat_id = head.flat_id');
                    $query1->innerJoin('housing_estate', 'he', 'he.estate_id = hf.estate_id');
                    $query1->innerJoin('housing_block', 'hb', 'hb.block_id = hf.block_id'); 
                    $query1->innerJoin('housing_flat_type', 'hft', 'hft.flat_type_id = hf.flat_type_id');

                    $query1->addField('head', 'applicant_name');
                    $query1->addField('head', 'housing_existing_occupant_draft_id');
                    $query1->addField('he', 'estate_name');
                    $query1->addField('hft', 'flat_type');
                    $query1->addField('hf', 'flat_id');
                    $query1->addField('hb', 'block_name');
                    $query1->addField('hf', 'flat_no');

                    // $db_and = db_and();														
                    $db_and->condition('he.division_id', $data_fetch->division_id, '=');	
                    // $db_and->condition('he.subdiv_id', $data_fetch->subdiv_id, '=');		
                    // $query1->condition($db_and);												

                    if($rhe_name != 0 && $flat_type != 0 && $block_name != 0 && $flat_id != 0){
                        $db_and = db_and();
                        $db_and->condition('hf.estate_id', $rhe_name, '=');
                        $db_and->condition('hf.flat_type_id', $flat_type, '=');
                        $db_and->condition('hf.block_id', $block_name, '=');
                        $db_and->condition('hf.flat_id', $flat_id, '=');
                        $query1->condition($db_and);
                    }

                    $result1 =$query1->execute();
                    $data = $result1->fetchObject();
                }
        }
// echo "<pre>";print_r($data);die;
       $display_output1 = '
		    <div class="table-bottom">
		        <table class="table table-list">
		          <tr>
		            <th colspan="2" style="background: none repeat scroll 0 0 #0a8bd6dc;color:white;text-align: center;font-size: 18px;line-height: 24px; font-weight: normal;font-family: "Dosis",Arial,Verdana,serif;" class="first">Application Information</th>
		          </tr>
		           
		          <tr>
		            <th style="background-color:#00000000" width="50%">Applicant Name:</th>
		            <td width="50%">'.(isset($data->applicant_name) ? $data->applicant_name : 'Not Avalaible').'</td>
		          </tr>
                  <tr>
		            <th style="background-color:#00000000" width="50%">Estate Name:</th>
		            <td width="50%">'.(isset($data->estate_name) ? $data->estate_name : 'Not Avalaible').'</td>
		          </tr>
                  <tr>
		            <th style="background-color:#00000000" width="50%">Flat Type:</th>
		            <td width="50%">'.(isset($data->flat_type) ? $data->flat_type : 'Not Avalaible').'</td>
		          </tr>
                  <tr>
		            <th style="background-color:#00000000" width="50%">Block Name:</th>
		            <td width="50%">'.(isset($data->block_name) ? $data->block_name : 'Not Avalaible').'</td>
		          </tr>
                  <tr>
		            <th style="background-color:#00000000" width="50%">Flat Number:</th>
		            <td width="50%">'.(isset($data->flat_no) ? $data->flat_no : 'Not Avalaible').'</td>
		          </tr>
		         
		          
		        </table>
		    </div>';
            if (!empty($data)) {
                $id = isset($data->uid) ? encrypt_url($data->uid) : encrypt_url($data->housing_existing_occupant_draft_id);
                
                $query = db_select('housing_online_application', 'hoa')->fields('hoa', ['application_no']);
                $query->innerJoin('housing_applicant_official_detail', 'haod', 'haod.applicant_official_detail_id = hoa.applicant_official_detail_id');
                $query->leftJoin('housing_vs_application', 'hva', 'hva.online_application_id = hoa.online_application_id');
                $query->leftJoin('housing_cs_application', 'hca', 'hca.online_application_id = hoa.online_application_id');

                $query->condition(db_or()
                        ->condition('hva.occupation_flat', $data->flat_id)
                        ->condition('hca.occupation_flat', $data->flat_id)
                );
                $query->condition('hoa.status','housingapprover_approved_1', '=');
                $query->fields('haod',array('uid','hrms_id','applicant_official_detail_id'));
                $result = $query->execute()->fetchObject();
                if(empty($result)){
                    $next_link = l('Next', 'legay-vs-or-cs-form/' . $id, ['attributes' => ['class' => ['btn', 'btn-primary']]]);

                    $display_output1 .= '
                    <div class="text-right" style="margin-top: 15px;">' . $next_link . '</div>';
                }else{
                    $message = '<span style="color:red;">This Existing Applicant already applied for Vertical Shifting or Category Shifting</span>';
                     $display_output1 .= '
                    <div class="text-right" style="margin-top: 15px;">' . $message . '</div>';
                }
                
        }
    return $display_output1;
}

