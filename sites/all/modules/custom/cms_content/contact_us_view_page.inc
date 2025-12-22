<?php // Done by Subham 15-07-2025
function contact_us_view_page() { 
    $query = db_select('housing_cms', 'hc')
        ->fields('hc', array('housing_cms_id','content_type', 'content_description'))
        ->condition('hc.content_type', 'contact_us', '=')
        ->orderBy('housing_cms_id', 'DESC')
        ->execute();
    $result = $query->fetchObject();
    $content_body = '';
    $output = '';
    if(!empty($result)){
        $output = '<div class="row">
                        <p class="text-body-secondary"><b>' . $result->content_description . '</b></p>
                        <div class="col border">
                            <iframe src="https://www.google.com/maps/embed?pb=!1m18!1m12!1m3!1d3684.252607583039!2d88.34062037599892!3d22.569653633076953!2m3!1f0!2f0!3f0!3m2!1i1024!2i768!4f13.1!3m3!1m2!1s0x3a0277a200f6df61%3A0xca19a1b2688346e!2sNew%20Secretariat%20Building!5e0!3m2!1sen!2sin!4v1717760081773!5m2!1sen!2sin" width="100%" height="400" style="border:0;" allowfullscreen="" loading="lazy" referrerpolicy="no-referrer-when-downgrade"></iframe>
                        </div>
                    </div>';
    }else{
        $output = '<div class="row">
                        <p class="text-body-secondary"></p>
                        <div class="col border">
                            <iframe src="https://www.google.com/maps/embed?pb=!1m18!1m12!1m3!1d3684.252607583039!2d88.34062037599892!3d22.569653633076953!2m3!1f0!2f0!3f0!3m2!1i1024!2i768!4f13.1!3m3!1m2!1s0x3a0277a200f6df61%3A0xca19a1b2688346e!2sNew%20Secretariat%20Building!5e0!3m2!1sen!2sin!4v1717760081773!5m2!1sen!2sin" width="100%" height="400" style="border:0;" allowfullscreen="" loading="lazy" referrerpolicy="no-referrer-when-downgrade"></iframe>
                        </div>
                    </div>';
    }
    
    return $output;
}