<?php // Done by Subham 14-07-2025
function faq_view_page() {
    if (db_table_exists('housing_cms')) {
        $query = db_select('housing_cms', 'hc');
        $query->fields('hc', array('housing_cms_id','content_type', 'content_title', 'content_description'));
        $query->condition('hc.content_type', 'faq', '=');
        $query->orderBy('housing_cms_id', 'ASC');
        $result = $query->execute();

        $output = '<div class="accordion accordion-flush" id="accordionFlushExample">';

        $i = 0;
        while ($data = $result->fetchObject()) {
            $headingId = 'flush-heading' . $i;
            $collapseId = 'flush-collapse' . $i;

            $buttonClass = ($i === 0) ? 'accordion-button' : 'accordion-button collapsed';
            $ariaExpanded = ($i === 0) ? 'true' : 'false';
            $collapseClass = ($i === 0) ? 'accordion-collapse collapse show' : 'accordion-collapse collapse';

            $output .= '
                <div class="accordion-item">
                    <h2 class="accordion-header" id="' . $headingId . '">
                        <button class="' . $buttonClass . '" type="button" data-bs-toggle="collapse" data-bs-target="#' . $collapseId . '" aria-expanded="' . $ariaExpanded . '" aria-controls="' . $collapseId . '">
                            <b><i><svg class="w-6 h-6 text-gray-800 dark:text-white" aria-hidden="true" xmlns="http://www.w3.org/2000/svg" width="24" height="24" fill="none" viewBox="0 0 24 24">
                            <path stroke="currentColor" stroke-linecap="round" stroke-linejoin="round" stroke-width="2" d="m10 16 4-4-4-4"/>
                            </svg></i>' .  htmlspecialchars($data->content_title) . '</b>
                        </button>
                    </h2>
                    <div id="' . $collapseId . '" class="' . $collapseClass . '" aria-labelledby="' . $headingId . '" data-bs-parent="#accordionFlushExample">
                        <div class="accordion-body">' . $data->content_description . '</div>
                    </div>
                </div>
            ';
            $i++;
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

    $output .= '</div>';

    return $output;
}
