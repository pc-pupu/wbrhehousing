<?php include('includes/header.php'); ?>
<section class="bg-banner"></section>
<section class="h-75 #mx-auto #p-3" style=" overflow: auto;">
    <div class="services small_pb">
        <div class="container">
            <h2 class="fw-bold text-body-emphasis abt-dept-heading2 poppins-extralight"><?php print $title ?></h2>
            <div class="row justify-content-center">
                <div class="col-xl-9 col-lg-9">
                    <?php print render($page['content']); ?>
                </div>
            </div>
        </div>
    </div>
</section>

<?php include('includes/footer.php'); ?>