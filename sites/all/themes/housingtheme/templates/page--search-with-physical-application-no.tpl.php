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
            <div class="pt-5">
                <?php print $messages; ?>
                <?php print render($page['content']); ?>
            </div>
        </div>
    </div>
</div>