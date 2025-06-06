<?php
		
	function rhewise_flatlist_draft_form($form, &$form_state) {
		
		$rhe_name =  isset($form_state['values']['rhe_name']) && !empty($form_state['values']['rhe_name']) ? $form_state['values']['rhe_name'] : 0;
		$flat_type =  isset($form_state['values']['flat_type']) &&!empty($form_state['values']['flat_type']) ? $form_state['values']['flat_type'] : 0;
		$block_name =  isset($form_state['values']['block_name']) &&!empty($form_state['values']['block_name']) ? $form_state['values']['block_name'] : 0;
		$flat_no =  isset($form_state['values']['flat_no']) &&!empty($form_state['values']['flat_no']) ? $form_state['values']['flat_no'] : 0;
		
		
		$form['rhe_name'] = array(
			'#title' => t('Name of the RHE'),
			'#type' => 'select',
			'#options' => rhe_list_specific(),
			'#required' => TRUE,
			//'#weight' => -10,
			//'#prefix' => '<div>',
			//'#suffix' => '</div>',
			'#prefix' => '<div class="row"><div class="col-md-6"><div class="form-floating">',
			'#suffix' => '</div></div>',
			'#attributes' => array('class'=>array('form-select')),
			'#ajax' => array(
			'event' => 'change',
			'callback' => '_reload_rhewise_flat_type',
			'method' => 'replace',
			),
			// '#select2' => array(
			//  'width' => '500',
			//  //'placeholder' => '- Select -',
			//  'allowClear' => TRUE
			   //       )
			);
		
		$form['flat_type'] = array(
			'#prefix' => '<div class="three" id="flat_type_replace">',
			'#suffix' => '</div>',
		);
			
		if($rhe_name != 0) {
			$form['flat_type'] = array(
				'#title' => t('Flat Type'),
				'#type' => 'select',
				'#options' => flat_type_under_rhe($rhe_name),
				'#required' => TRUE,
				//'#weight' => -10,
				'#prefix' => '<div class="col-md-6" id="flat_type_replace"><div class="form-floating">',
				'#suffix' => '</div></div></div>',
				//'#attributes' => array('class' => array('dropdown3')),
				'#attributes' => array('class'=>array('form-select')),
				'#ajax' => array(
					'event' => 'change',
					'callback' => '_reload_rhewise_blocklist',
					'method' => 'replace',
				),
			);
		}
		
		$form['block_name'] = array(
			'#prefix' => '<div class="three" id="block_name_replace">',
			'#suffix' => '</div>',
		);
			
		if($rhe_name != 0 && $flat_type != 0) {
			$form['block_name'] = array(
				'#title' => t('Name of the Block'),
				'#type' => 'select',
				'#options' => block_name_under_rhe($rhe_name, $flat_type),
				'#required' => TRUE,
				//'#weight' => -10,
				'#prefix' => '<div class="col-md-6" id="block_name_replace"><div class="form-floating">',
				'#suffix' => '</div></div>',
				//'#attributes' => array('class' => array('dropdown3')),
				'#attributes' => array('class'=>array('form-select')),
				'#ajax' => array(
					'event' => 'change',
					'callback' => '_reload_rhewise_flatlist',
					'method' => 'replace',
				),
			);
		}
		
		$form['flat_no'] = array(
			'#prefix' => '<div class="three" id="flat_no_replace">',
			'#suffix' => '</div>',
		);
			
		if($rhe_name != 0 && $flat_type != 0 && $block_name != 0) {
			$form['flat_no'] = array(
				'#title' => t('Flat No.'),
				'#type' => 'select',
				'#options' => flatno_under_rhe($rhe_name, $flat_type, $block_name),
				'#required' => TRUE,
				//'#weight' => -10,
				'#prefix' => '<div class="col-md-6" id="flat_no_replace"><div class="form-floating">',
				'#suffix' => '</div>',
				//'#attributes' => array('class' => array('dropdown3')),
				'#attributes' => array('class'=>array('form-select')),
				'#ajax' => array(
					'event' => 'change',
					'callback' => '_reload_rhewise_occupant_entry',
					'method' => 'replace',
				),
			);
		}
		
		
		/*$form['hrms_id'] = array(
			'#prefix' => '<div class="two" id="hrms_id_replace">',
			'#suffix' => '</div>',
		);*/
		
		$form['submit_button'] = array(
			'#prefix' => '<div class="two-one" id="submit_button_replace">',
			'#suffix' => '</div>',
		);
		
		if($rhe_name != 0 && $flat_type != 0 && $block_name != 0 && $flat_no != 0) {
			/*$form['hrms_id'] = array(
				'#title' => t('Employee HRMS ID'),
				'#type' => 'textfield',
				'#prefix' => '<div class="two" id="hrms_id_replace">',
				'#suffix' => '</div>',
				'#element_validate' => array('element_validate_hrms_id'),
				'#attributes' => array('class' => array('numeric_positive')), 
				'#required' => TRUE,
			);*/
			
			$form['submit_button']=array(
				'#type' => 'submit',
				'#prefix' => '<div id="submit_button_replace">',
				'#suffix' => '</div>',
				'#attributes' => array('class'=>array('btn bg-primary btn-sm px-5 rounded-pill text-white fw-bolder')),
				//'#value' => t('Search Details'),
				'#value' => t('Insert Occupant Details'),
				);
		}
		$form['#attributes'] = array('onsubmit' => 'return validate_rhewise_occupant_entry_form()');
		
		return $form;
	}
	
	function rhewise_flatlist_form_validate($form, &$form_state) {
		#krumo($form_state['values']['table']);
		
	}
	
	function rhewise_flatlist_draft_form_submit($form, &$form_state) {
		
		 $flat_id = $form_state['values']['flat_no'];
		 $estate_id = $form_state['values']['rhe_name'];// 10-06-2024
		
		
		
		drupal_goto('rhewise_occupant_draft_data_entry/'.encrypt_url($flat_id).'/'.encrypt_url($estate_id));// 10-06-2024
	}
	
	function _reload_rhewise_flat_type($form, &$form_state) {
		$commands = array();
		$commands[] = ajax_command_replace("#flat_type_replace", drupal_render($form['flat_type']));
		$commands[] = ajax_command_replace("#block_name_replace", drupal_render($form['block_name']));
		$commands[] = ajax_command_replace("#flat_no_replace", drupal_render($form['flat_no']));
		
		//$commands[] = ajax_command_replace("#hrms_id_replace", drupal_render($form['hrms_id']));
		$commands[] = ajax_command_replace("#submit_button_replace", drupal_render($form['submit_button']));
		return array('#type' => 'ajax', '#commands' => $commands);
	}
	
	function _reload_rhewise_blocklist($form, &$form_state) {
		$commands = array();
		$commands[] = ajax_command_replace("#block_name_replace", drupal_render($form['block_name']));
		$commands[] = ajax_command_replace("#flat_no_replace", drupal_render($form['flat_no']));
		
		//$commands[] = ajax_command_replace("#hrms_id_replace", drupal_render($form['hrms_id']));
		$commands[] = ajax_command_replace("#submit_button_replace", drupal_render($form['submit_button']));
		return array('#type' => 'ajax', '#commands' => $commands);
	}
	
	function _reload_rhewise_flatlist($form, &$form_state) {
		$commands = array();
		$commands[] = ajax_command_replace("#flat_no_replace", drupal_render($form['flat_no']));
		
		//$commands[] = ajax_command_replace("#hrms_id_replace", drupal_render($form['hrms_id']));
		$commands[] = ajax_command_replace("#submit_button_replace", drupal_render($form['submit_button']));
		return array('#type' => 'ajax', '#commands' => $commands);
	}
	
	function _reload_rhewise_occupant_entry($form, &$form_state) {
		
		$commands = array();
		//$commands[] = ajax_command_replace("#hrms_id_replace", drupal_render($form['hrms_id']));
		$commands[] = ajax_command_replace("#submit_button_replace", drupal_render($form['submit_button']));
		return array('#type' => 'ajax', '#commands' => $commands);
	}