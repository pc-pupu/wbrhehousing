<?php // Done by Subham 15-07-2025
function about_us_view_page() {
    $query = db_select('housing_cms', 'hc')
        ->fields('hc', array('housing_cms_id','content_type', 'content_description'))
        ->condition('hc.content_type', 'about_us', '=')
        ->orderBy('housing_cms_id', 'DESC')
        ->execute();
    $result = $query->fetchObject();
    // print_r($result);die;
    $content_body = '';
    if(!empty($result)){
        $content_body = '<div class="row">
                            <p class="text-body-secondary"><b>' . $result->content_description . '</b></p>
                        </div>';
    }
    $output = '<div class="" id="aboutUs">' . $content_body . '</div>';
    return $output;
}