<?php global $base_root, $base_path; 
$actual_path = drupal_get_path_alias($_GET['q']);?>

    <!-- Top header section start -->
    <section class="top-header">
        <div class="container">
            <div class="row">
                <div class="col-md-10">
                    <div class="contact-info"> <!-- Added by Subham 08-12-2025 -->
                        <span class="contact-note">
                            If you need additional assistance after reviewing the user manual, you can contact us using the details.
                        </span>

                        <!-- Email -->
                        <a href="mailto:rhehoushing@gmail.com" class="contact-item">
                            <svg xmlns="http://www.w3.org/2000/svg" width="18" height="18" fill="currentColor" viewBox="0 0 24 24">
                                <path d="M20 4H4a2 2 0 0 0-2 2v12a2 2 0 0 0 2 2h16a2 2 0 0 0 2-2V6a2 2 0 0 0-2-2zm0 4-8 5-8-5V6l8 5 8-5v2z"/>
                            </svg>
                            rhehoushing@gmail.com
                        </a>

                        <!-- Phone -->
                        <a href="tel:033322486943" class="contact-item">
                            <svg xmlns="http://www.w3.org/2000/svg" width="18" height="18" fill="currentColor" viewBox="0 0 24 24">
                                <path d="M6.62 10.79a15.09 15.09 0 0 0 6.59 6.59l2.2-2.2a1 1 0 0 1 1-.24 11.36 11.36 0 0 0 3.58.57 1 1 0 0 1 1 1V20a1 1 0 0 1-1 1A17 17 0 0 1 3 4a1 1 0 0 1 1-1h3.5a1 1 0 0 1 1 1 11.36 11.36 0 0 0 .57 3.58 1 1 0 0 1-.24 1z"/>
                            </svg>
                            03322486943
                        </a>
                    </div> <!-- End -->
                </div>
            </div>
        </div>
    </section>
    <!-- Top header section end -->
    <!-- Top header logo section start -->
    <section class="top-header-logo-bg">
        <div class="container">
            <div class="row">
                <div class="col-md-6"><a href="<?= $base_root.$base_path ?>"><img src="<?= $base_root.$base_path ?>sites/all/themes/outertheme/images/e-allotment-rhe-logo2.jpg" alt="Housing Department"
                        title="Housing Department"></a></div>
                <div class="col-md-6 text-end mt-5">
                    <a href="<?= $base_root.$base_path.'user/login'?>" target="_self" class="btn-admin-login mx-2">Official Login</a>
                    <!-- <a href="<?= $base_root.$base_path.'hrms-login'?>" target="_self" class="btn-admin-login">Applicant Login</a> -->
                </div>
            </div>
        </div>
    </section>
    <!-- Top header logo section end -->
    <!-- Top header section start -->
    <section class="top-header-menu-bg">
        <div class="container">
            <div class="row">
                <div class="col-md-12">
                    <nav class="navbar navbar-expand-sm bg-dark# navbar-dark#">
                        <button class="navbar-toggler" type="button" data-bs-toggle="collapse"
                            data-bs-target="#collapsibleNavbar">
                            <span class="navbar-toggler-icon"></span>
                        </button>
                        <div class="collapse navbar-collapse" id="collapsibleNavbar">
                            <ul class="navbar-nav ">
                                <li class="nav-item">
                                    <a class="nav-link" href="<?= $base_root.$base_path ?>">Home</a>
                                </li>
                                <li class="nav-item">
                                    <a class="nav-link <?= ($actual_path == 'about-us') ? 'active' : '' ?>" href="<?= $base_root . $base_path . 'about-us' ?>">About Us</a>
                                </li>
                                <li class="nav-item">
                                    <a class="nav-link <?= ($actual_path == 'faq') ? 'active' : '' ?>" href="<?= $base_root . $base_path . 'faq' ?>">FAQ</a>
                                </li> 
                                <li class="nav-item">
                                    <a class="nav-link <?= ($actual_path == 'contact-us') ? 'active' : '' ?>" href="<?= $base_root . $base_path . 'contact-us' ?>">Contact Us</a>
                                </li>
                                <li class="nav-item">
                                    <a class="nav-link <?= ($actual_path == 'notice') ? 'active' : '' ?>" href="<?= $base_root . $base_path . 'notice' ?>">Notice</a>
                                </li>
                                <li class="nav-item">
                                    <a class="nav-link <?= ($actual_path == 'user-manual') ? 'active' : '' ?>" href="<?= $base_root . $base_path . 'user-manual' ?>">User Manual / SOP</a>
                                </li>
                            </ul>
                        </div>
                    </nav>
                </div>
            </div>
        </div>
    </section>
    <!-- Top header section end -->