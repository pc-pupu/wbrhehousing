<?php
    global $user, $base_root, $base_path;
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
        <a href="<?= $base_root.$base_path ?>" class="d-flex flex-column align-items-center mb-5 text-center">
            <img src="<?= $base_root.$base_path?>sites/all/themes/housingtheme/images/wb-logo.png" class="img-fluid" alt="e-Allotment of Rental Housing Estate">
            <div class="dashboard-logo">
                <div class="fs-5 fw-semibold lh-1">e-Allotment of Rental Housing Estate</div>
                <small>Housing Department <br> Government of West Bengal</small>
            </div>
        </a>
    </div>
    <div id="content-wrapper" class="content-wrapper">
        <div class="main-content p-5 min-vh-100">
            <?php 
            if($checkExist == 0){
            ?>
            <div class="counter-box p-3 rounded mb-3 position-relative shadow-sm row">
                <h6 class = "">
                    <b class="text-danger">If you have already submitted your flat allotment application in physical form to the Housing Department, Government of West Bengal, click here to link it with your HRMS code
                    <?php echo l('Click Here','search-with-physical-application-no', array('html'=>true,'attributes'=>array('class'=>array('btn btn-success btn-sm')))); ?>
                    </b>
                </h6>
                <h6 class = "">
                    <b class="text-danger">Continue to Dashboard for Fresh Online Application & Related Activities
                    <?php echo l('Click to Continue','dashboard', array('html'=>true,'attributes'=>array('class'=>array('btn btn-success btn-sm'), 'id' => 'my-button' ))); ?></b>
                </h6>
            </div>
            <div class="pt-5">
                <h5><b>If you are currently occupying a rental housing flat alloted by the Housing Department Govt. of WB, please fill the form for verification and click Submit.</b></h5>
                <?php print $messages; ?>
                <?php print render($page['content']); ?>
            </div>
            <?php
            }else{
                print $messages;
            }
            ?> 
            <?= l('Exit', 'user/logout', array('html'=>true, 'attributes'=>array('class'=>array('btn bg-info btn-sm px-5 rounded-pill text-white fw-bolder')))) ?>
        </div>
    </div>
</div>