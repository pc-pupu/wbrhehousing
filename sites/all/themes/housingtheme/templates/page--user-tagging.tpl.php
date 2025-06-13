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
            <h3 class = "" style="padding-top:10%;">
                If you are a new applicant please click to continue button. 
                <?php echo l('Click to Continue','dashboard', array('html'=>true,'attributes'=>array('class'=>array('btn btn-success'), 'id' => 'my-button' ))); ?>
            </h3>
            <div class="" style="padding-top:5%;">
                <h3>If you are an existing occupant, please fill the form for verification.</h3>
                <?php print $messages; ?>
                <?php print render($page['content']); ?>
            </div>
        </div>
    </div>
</div>