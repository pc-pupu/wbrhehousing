<?php
		
	function rhe_wise_flat_list_form($form, &$form_state) {
		global $user_role;
		
		$rhe_name =  !empty($form_state['values']['rhe_name']) ? $form_state['values']['rhe_name'] : 0;
		
		if($user_role == 7 || $user_role == 8) {
			$form['rhe_name'] = array(
				'#title' => t('Name of the RHE'),
				'#type' => 'select',
				'#options' => rhe_list_specific(),
				'#required' => TRUE,
				'#attributes' => array('class' => array('form-select')),//sd 20-06-2024
				'#prefix' => '<div class="col-md-6"><div class="form-floating">',//sd 20-06-2024
				'#suffix' => '</div></div>',//sd 21-06-2024
				'#ajax' => array(
					'event' => 'change',
					//'wrapper' => 'no_of_vacancy',
					'callback' => '_reload_rhe_wise_flat_list',
					'method' => 'replace',
				),
				//'#attributes' => array('class' => array('update_rhe_selectize')),
			);
		} else {
			$form['rhe_name'] = array(
				'#title' => t('Name of the RHE'),
				'#type' => 'select',
				'#options' => rhe_list(),
				'#required' => TRUE,
				//'#weight' => -10,
				//'#prefix' => '<div>',// sd 24-06-2024
				//'#suffix' => '</div>', //sd 24-06-2024
				'#attributes' => array('class' => array('form-select')),//sd 20-06-2024
				'#prefix' => '<div class="col-md-6"><div class="form-floating">',//sd 20-06-2024
				'#suffix' => '</div></div>',//sd 21-06-2024
				'#ajax' => array(
					'event' => 'change',
					//'wrapper' => 'no_of_vacancy',
					'callback' => '_reload_rhe_wise_flat_list',
					'method' => 'replace',
				),
			);
		}
		
		$form['rhe_wise_flat_list'] = array(
			'#type' => 'markup',
			'#markup' => '<div  id="rhe_wise_flat_list"></div>'
		);
		
		if($rhe_name != 0) {
			
			$form['rhe_wise_flat_list'] = array(
				'#type' => 'markup',
				'#markup' => rhe_wise_flat_list_page($rhe_name),
				'#prefix' => '<div id="rhe_wise_flat_list">',
				'#suffix' => '</div>',
			);
		 
	  	}
		
		return $form;
	}
	
	function rhe_wise_flat_list_form_validate($form, &$form_state) {
		#krumo($form_state['values']['table']);
	}
	
	function rhe_wise_flat_list_form_submit($form, &$form_state) {
		$form_state['rebuild'] = TRUE;
	}
	
	function _reload_rhe_wise_flat_list($form, &$form_state) {
		$commands = array();
		$commands[] = ajax_command_replace("#rhe_wise_flat_list", drupal_render($form['rhe_wise_flat_list']));
		return array('#type' => 'ajax', '#commands' => $commands);
  
	}
	
	function rhe_wise_flat_list_page($rhe_name = '') {
		
		global $base_path, $user_role, $user;
		
		$output = '';

		$result = rhe_wise_flat_list_detail($rhe_name);
		
		$result1 = rhe_wise_flat_list_detail($rhe_name);
		$data1 = $result1->fetchObject();

		// echo $rhe_name; die;
		// print_r($result);die;
		
		$header = array(); // sd 24-06-2024 turned off
		$rows =array(); // sd 24-06-2024 turned off
		
		$header[] = array('data' => 'Sl. No.');
		$header[] = array('data' => 'District');
		$header[] = array('data' => 'Flat Type');
		$header[] = array('data' => 'Name of the RHE');
		$header[] = array('data' => 'Estate Address');
		$header[] = array('data' => 'Block');
		$header[] = array('data' => 'Floor');
		$header[] = array('data' => 'Flat No');
		$header[] = array('data' => 'Flat Status');

		if($user_role == 8){
			$header[] = array('data' => 'Action');
		}
		
		$i = 1;
		// if($result->rowCount() > 0){
		while($data = $result->fetchObject()) {
			
			 $fields = array();
			 $fields[] = $i;
			 $fields[] = $data->district_name;
			 $fields[] = $data->flat_type;
			 $fields[] = $data->estate_name;
			 $fields[] = $data->estate_address;
			 $fields[] = $data->block_name.' Block';
			//  if($data->floor == 'G')
			//  	$fields[] = 'Ground';
			//  else if($data->floor == 'F')
			//  	$fields[] = 'First';
			//  else if($data->floor == 'S')
			//  	$fields[] = 'Second';
			//  else if($data->floor == 'T')
			//  	$fields[] = 'Third';
			//  else if($data->floor == '')
			// 		$fields[] = '';
			$fields[] = $data->floor;
			$fields[] = $data->flat_no;
			// $fields[] = $data->flat_status;   //turned off by debaleena 26-07-2024
			if(!empty($data->remarks)){
				
				$fields[] = $data->flat_status_code."<br>"."(".$data->remarks.")"; //added by debaleena 26-07-2024
			}else{
				
			 	$fields[] = $data->flat_status_code;
			}

			/*Added by Moumita 21-05-2025*/
			if($user_role == 8){
			$fields[] = l('<i class="fa fa-trash"></i> Delete', 'rhe-wise-flat-list-delete/'.encrypt_url($data->flat_id), array('html' => true, 'attributes' => array('class' => array('btn btn-sm bg-primary px-3 rounded-pill text-white fw-bolder mb-4'))));
			}
			 	
			 
			 $rows[]  = $fields;
			 
			 $i++;
		 }
		// }
		
		/*$output = theme_table(
			array(
			  'header' => $header,
			  'rows' => $rows,
			  'attributes' => array(),
			  'sticky' => true,
			  'caption' => '',
			  'colgroups' => array(),
			  'empty' => t("No data found!") // The message to be displayed if table is empty
			)
		).theme('pager');*/
		
		if($result->rowCount() > 0) { 
			// Render using Drupal's render API.
			$build['datatable'] = array(
			  '#theme' => 'datatable',
			  '#header' => $header,
			  '#rows' => $rows,
			  '#attributes' => array(),
			);
		
			// Or, render using a theme function.
			$variables = array(
			  //'attributes' => array(),// sd 24-06-2024
			  'header' => $header,
			  'rows' => $rows,
			  'attributes'=>array('class'=>array('table table-list table-striped')),// sd 24-06-2024
			);
			
			$output = theme('datatable', $variables);
		}else { 
			$output = '<div>
							<table class="datatable_no_data_found table table-list table-striped">
								<tr class="tr_no_data_found">
									<th class="th_no_data_found"></th>
								</tr>
								<tr class="tr_no_data_found">
									<td class="td_no_data_found">No data found!</td>
								</tr>
							</table>
						</div>';
		}
		
		
		if(count($rows) > 0) {
			$replace_item = array(',', '/', ' ', '.', '(', ')');
			$file_name = str_replace($replace_item, '_', trim($data1->estate_name));
			
			//sd 24-06-2024 turned off
			//return l('<img height="18" width="18" src="'.$base_path.drupal_get_path('module', 'update_flat').'/images/pdf_icon.png" title="Download Flat List" alt="PDF Icon">Download Flat List Of '.$data1->estate_name,'rhe_wise_flat_list_pdf/'.encrypt_url($rhe_name).'/'.$file_name, array('html'=>TRUE, 'attributes' => array('target' => '_blank', 'style' => 'color: #0090C7;font-weight: 400;text-decoration: none; font-size:14px'))).$output;
			
			$url = l('Download Flat List of '.$data1->estate_name,'rhe_wise_flat_list_pdf/'.encrypt_url($rhe_name).'/'.$file_name,array('html' => true, 'attributes' => array('target' => '_blank','class' => array('btn bg-primary  px-5 rounded-pill text-white fw-bolder mb-4'))));

			return $url.'<div class="mb-5 pb-3">'.$output.'</div>';//sd 24-06-2024
		}
		else {
			return $output;
		}
	}

	// Done by Moumita 21-05-2025
	function rhe_wise_flat_list_delete($flat_id_enc = ''){
	$flat_id = decrypt_url($flat_id_enc);

	// checking if existing occupant is present or not
	/* if yes the only message show and redirect to existing occupant data view-occupant-list/  or rhewise_occupant_draft_list 
	else row delete */

	$sql = "select
	CAST(exists (select 1 from housing_flat_occupant where flat_id = ".$flat_id." ) AS INT) as withhrms,
	CAST(exists (select 1 from housing_existing_occupant_draft where flat_id = ".$flat_id." ) AS INT) as withouthrms";

	$query = db_query($sql);
	$result = $query->fetchObject();
	if(!empty($result)){
		$withhrms = $result->withhrms;
		$withouthrms = $result->withouthrms;

		if($withhrms ==1 || $withouthrms ==1){
			drupal_set_message('Existing Occupant present, Please remove that first','error');
			if($withhrms == 1){
				$redirect_url = 'view-occupant-list';
			}else if($withouthrms == 1){
				$redirect_url = 'rhewise_occupant_draft_list';
			}
			drupal_goto($redirect_url);
		}else{
				// call delete function
				delete_flat_info($flat_id_enc);
				drupal_goto('rhe_wise_flat_list');
			}
	}
}
