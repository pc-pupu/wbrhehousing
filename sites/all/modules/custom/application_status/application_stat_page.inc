<?php

function application_status_form($form,&$form_state) {
		
		
		$application_no = isset($form_state['values']['application_no']) && !empty($form_state['values']['application_no']) ? $form_state['values']['application_no'] : '';
		
		
		$form['application_no']=array(
			'#title' => t('Enter Application No.'),
			'#type' => 'textfield',
			'#default_value' => '',
			/*'#attributes' => array('placeholder' => 'Enter application no.'),
			'#required' => TRUE,
			'#prefix' => '<div class="two">',
			'#suffix' => '</div>',*/
			'#attributes' => array('placeholder' => 'Enter application no.','class'=>array('form-control form-control-sm')),//sd 20-06-2024
			'#required' => TRUE,
			'#prefix' => '<div class="row"><div class="col-md-8"><div class="form-floating">',//sd 20-06-2024
			'#suffix' => '</div></div>',//sd 20-06-2024
			'#weight' =>2,
			//'#element_validate' => array('validate_application_number') // Done by Subham 31-01-2025
		);
		$form['search'] = array(
			'#value' => t('Search'),
			'#type' => 'submit',
			/*'#prefix' => '<div class="two" style="margin-top:43px;height: 30px;">',
			'#suffix' => '</div>',*/
			'#prefix' => '<div class="col-md-4"><div class="form-floating">',//sd 20-06-2024
			'#suffix' => '</div></div></div>',//sd-20-06-2024
			'#attributes'=>array('class'=>array('text-centre btn bg-primary btn-sm px-5 mt-5 rounded-pill text-white fw-bolder')),//sd-20-06-2024
			'#weight' =>3
	  	);
		
		$form['#attributes'] = array('onsubmit' => 'return validate_application_status_form()');
	  
	    if(!empty($application_no)) {
	  		$output = application_stat_page($application_no);
			$form['breadcrumb'] = array(
				'#type' => 'markup',
				'#markup' => $output,
				'#weight' => 5
			);
		}
	  return $form;	
	}
	
	function application_status_form_submit($form, &$form_state) {
		
		$form_state['rebuild'] = TRUE;
		
	}

	function application_stat_page($application_no = '') {
		global $base_path;
		//drupal_add_css('.text-align-r {text-align:right}', 'inline');
		
		$output = '';
		$online_application_id = 0;
		// $result = application_detail_fetch($online_application_id, trim($application_no)); // turned off by debaleena

		//debaleena start 11-06-2024
		// $result = fetch_full_application_status(trim($application_no));
		$result = fetch_full_application_status_updated(trim($application_no));   //added by dg 26-02-2026


		//end
		$rows =array();
		$header = array();
		$fields = array();

		$header[] = array('data' => 'Status', 'class' => array('text-align-c'));
		$header[] = array('data' => 'Verification Date', 'class' => array('text-align-c'), 'width'=>'40%');
	
		//$flat_type_id = 0;
		
		 while($data = $result->fetchObject()) {
			// print_r($data);die;
			$fields[] = array('data' => $data->applicant_show_status, 'class' => array('text-align-c'));
			$fields[] = array('data' => date('d/m/Y',strtotime($data->created_at)), 'class' => array('text-align-c'));
			$rows[] = $fields;

			// Remove data from array 
			unset($fields); 
		}
		 
		
		$output = theme_table(
		array(
		  'header' => $header,
		  'rows' => $rows,
		  'attributes' => array('class'=>array('table table-list table-striped')),
		  'sticky' => true,
		  'caption' => '',
		  'colgroups' => array(),
		  'empty' => t("No data found!") // The message to be displayed if table is empty
		)
	  ).theme('pager');
	  
	  return $output;
	}
	
	/* Done by Subham 31-01-2025 */
	function validate_application_number($element, &$form_state) {
		$input = $form_state['values'][$element['#name']];
	
		if (!preg_match('/^[A-Z]{2}-[A-Z]{3}-\d{8}-\d+$/', $input)) {
			form_set_error($element['#name'], t('Invalid Application Number. Enter Proper Application Number'));
		}
	}