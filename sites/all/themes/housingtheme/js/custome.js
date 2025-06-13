document.addEventListener("DOMContentLoaded", function(){
    document.querySelectorAll('.sidebar .nav-link').forEach(function(element){
      
      element.addEventListener('click', function (e) {
  
        let nextEl = element.nextElementSibling;
        let parentEl  = element.parentElement;	
  
          if(nextEl) {
              e.preventDefault();	
              let mycollapse = new bootstrap.Collapse(nextEl);
              
              if(nextEl.classList.contains('show')){
                mycollapse.hide();
              } else {
                  mycollapse.show();
                  // find other submenus with class=show
                  var opened_submenu = parentEl.parentElement.querySelector('.submenu.show');
                  // if it exists, then close all of them
                  if(opened_submenu){
                    new bootstrap.Collapse(opened_submenu);
                  }
              }
          }
      }); // addEventListener
    }) // forEach
  }); 

  (function ($) {
  Drupal.behaviors.setCookieAlert = {
    attach: function (context, settings) {
      $('#my-button', context).once('set-cookie-alert').click(function () {
        // Set cookie (simple way)
        document.cookie = "user_type=new; max-age=30;"; 
        // Show alert
        // alert('Cookie has been set!');
      });
    }
  };
})(jQuery);