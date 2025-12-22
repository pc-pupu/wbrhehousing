<?php // Done by Subham dt.18-07-2025
function notice_view_page() {
    global $base_root, $base_path;
    $output = '';

    if (db_table_exists('housing_cms')) { 
        $query = db_select('housing_cms', 'hc');
        $query->fields('hc', array('housing_cms_id','content_type', 'content_title','file_path','date_of_notification'));
        $query->condition('hc.content_type', 'notice', '=');
        $query->orderBy('housing_cms_id', 'DESC');
    
        $result = $query->execute();

        
        $header['serial_no'] = array('data' => 'Serial No.', 'width' => '2%');
        $header['description'] = array('data' => 'Description', 'width' => '10%');
        $header['download_content'] = array('data' => 'Download File', 'width' => '3%');
        $header['don'] = array('data' => 'Date of Notification', 'width' => '2%');

        $rows = array();
        $output = '';
        $serialNumber = 1;
            
        while($data = $result->fetchObject()) {
            $fields = array();

            $fields[] = $serialNumber;
            $fields[] = $data->content_title;

            if (!empty($data->file_path)) {
                $file_url = file_create_url($data->file_path);
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
            
                $fields[] = date('d/m/Y', strtotime($data->date_of_notification));

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

            $output = '<h2 class="fw-bold text-body-emphasis">Notice</h2>';
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