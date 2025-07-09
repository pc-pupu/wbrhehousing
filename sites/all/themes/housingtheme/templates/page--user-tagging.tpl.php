<?php
    global $user;
    // check hrms exist in user-tagging table
    $checkExist = 0;
    $query = db_query("select housing_user_tagging_id from housing_user_tagging where hrms_id= '".$user->name."'");
    $result = $query->fetchObject(); 
    if(!empty($result)){
        $checkExist = 1;
    }else{
        $checkExist = 0;
    }

?>

<div class="dashboard">
    <div class="sidebar d-flex flex-column p-3">
        <a href="http://localhost/housing/" class="d-flex flex-column align-items-center mb-5 text-center">
            <img src="http://localhost/housing/sites/all/themes/housingtheme/images/wb-logo.png" class="img-fluid" alt="e-Allotment of Rental Housing Estate">
            <div class="dashboard-logo">
                <div class="fs-5 fw-semibold lh-1">e-Allotment of Rental Housing Estate</div>
                <small>Housing Department <br> Government of West Bengal</small>
            </div>
        </a>
    </div>
    <div id="content-wrapper" class="content-wrapper">
        <div class="main-content p-5 min-vh-100">
<<<<<<< HEAD
            <?php 
            if($checkExist == 0){
            ?>
            <div class="counter-box p-3 rounded mb-3 position-relative shadow-sm row">
                <h6 class = "">
                    <b class="text-danger">If you want to apply for new allotment of a rental housing flat please click here 
                    <?php echo l('Click to Continue','dashboard', array('html'=>true,'attributes'=>array('class'=>array('btn btn-success btn-sm'), 'id' => 'my-button' ))); ?></b>
                </h6>

                <h6 class = "">
                    <b class="text-danger">If you have already submitted the physical hard copy application to the Housing Department Govt. of WB, please click here 
                    <?php echo l('Click to Continue','#', array('html'=>true,'attributes'=>array('class'=>array('btn btn-success btn-sm'), 'id' => 'my-button' ))); ?>
                    to link it with your HRMS code</b>
                </h6>
            </div>
            <?php
            }
            ?> 
            <div class="pt-5">
                <h5><b>If you are currently occupying in a rental housing flat alloted by the Housing Department Govt. of WB, please fill the form for verification.</b></h5>
=======
        <?php 
        if($checkExist == 0){
            ?>
            <h3 class = "" style="padding-top:10%;">
                If you are a new applicant please click to continue button. 
                <?php echo l('Click to Continue','dashboard', array('html'=>true,'attributes'=>array('class'=>array('btn btn-success'), 'id' => 'my-button' ))); ?>
            </h3>
            <?php
        }
        ?>    
            <div class="" style="padding-top:5%;">
                <h3>If you are an existing occupant, please fill the form for verification.</h3>
>>>>>>> 0cb717b8a54072318052f16c41c9f9bccf5211e2
                <?php print $messages; ?>
                <?php print render($page['content']); ?>
            </div>
        </div>
    </div>
</div>