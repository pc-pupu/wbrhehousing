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
                <?php print render($page['content']); ?>
            </div>
    </div>
</div>