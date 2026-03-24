<?php // Done by Subham dt.18-07-2025
function user_manual() {
    global $base_root, $base_path;
    $output = '';

    if (db_table_exists('housing_cms')) { 
        
        $result = array();
        $result[] = array(
            'description' => 'User Manual for New Application',
            'file_path' => $base_root.$base_path.'sites/default/files/user_manual/New_Application-User_Manual.pdf',
        ); 
        $result[] = array(
            'description' => 'User Manual for Vertical(Floor) Shifting Application',
            'file_path' => $base_root.$base_path.'sites/default/files/user_manual/Vertical(Floor)_Shifting_Application-User_Manual.pdf',
        ); 
        $result[] = array(
            'description' => 'User Manual for Category Shifting Application',
            'file_path' => $base_root.$base_path.'sites/default/files/user_manual/Category_Shifting_Application-User_Manual.pdf',
        ); 
<<<<<<< HEAD
        $result[] = array(
            'description' => 'User Manual for DDOs',
            'file_path' => $base_root.$base_path.'sites/default/files/user_manual/DDO-User Manual.pdf',
        ); 
=======
>>>>>>> 2502ab393f49b61f25516e6bb0502acbb6d447a0

        
        $header['serial_no'] = array('data' => 'Serial No.', 'width' => '2%');
        $header['description'] = array('data' => 'Description', 'width' => '10%');
        $header['file_path'] = array('data' => 'Download File', 'width' => '3%');
        
        $rows = array();
        $output = '';
        $serialNumber = 1;
        
        foreach($result as $data) {
            $fields = array();

            $fields[] = $serialNumber;
            $fields[] = $data['description'];

            if (!empty($data['file_path'])) {
                $file_url = $data['file_path'];
                $fields[] = l('View', 
                                $file_url,
                                array(
                                    'html' => TRUE, 
                                    'attributes' => array(
                                        'target' => '_blank', 
                                        'class' => array('btn', 'btn-info', 'btn-sm'),
                                        'style' => 'background: linear-gradient(45deg, #007BFF, #28A745); color: white; border: none;'
                                    )
                                )
                            );
            } else {
                $fields[] = '<div style="text-align: center;"><span style="font-size: 0.8em; color: #666;">File not available to download</span></div>';
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

            // $output = '<h2 class="fw-bold text-body-emphasis">User Manual</h2>';
            $output .= theme('datatable', $variables);
        } 
    } else {
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
                
        return '<div class="col-md-12">'.$output.'</div>';
    
}