	/*jQuery(document).ready(function() {
		jQuery('.designation_selectize').select2();
	});*/

	/*jQuery(document).ready(function() {
		jQuery('.select2 .select2-container .select2-container--default').css('width', '317px');
	});*/
	
jQuery(document).ready(function() {
	
	jQuery("#mobile_no").keypress(function (e)  
	{ 
	  if( e.which!=8 && e.which!=0 && (e.which<48 || e.which>57))
	  {
	    return false;
      }	
	});
	
	
	jQuery(".numeric_positive").keypress(function (e)  
	{ 
	  if( e.which!=8 && e.which!=0 && (e.which<48 || e.which>57))
	  {
	    return false;
      }	
	});


	/*   added by debaleena 01-08-2024 */
	jQuery("#edit-confirm-computer-serial-no").keyup(function (e)  
	{ 
		//alert("confirm");
		let compSerialNo = jQuery("#edit-computer-serial-no").val();
		let conCompSerialNo = jQuery("#edit-confirm-computer-serial-no").val();
		var messageDiv = jQuery('#comp_ser_no_message');

		if(compSerialNo !== conCompSerialNo){
			messageDiv.text('**Computer Serial No. and Confirm Computer Serial No. do not Match.').css('color', 'red');
		}
		else{
			messageDiv.text('**Computer Serial No. and Confirm Computer Serial No. are Matched.').css('color', 'green');
		}
	});
	
});


/*  end  *//////

// jQuery(document).ready(function(){
// 		jQuery("#edit-dob").datepicker({
// 		dateFormat: "dd/mm/yy",
// 		//minDate: minDoj,
// 		changeMonth: true,
// 		changeYear: true,
// 		//yearRange: "-70:-18",
// 		yearRange: "-80:-18", //18 years or older up to 70year (oldest person ever, can be sensibly set to something much smaller in most cases)
// 	    maxDate: "-18Y", //Will only allow the selection of dates more than 18 years ago, useful if you need to restrict this
// 		autoSize: true
// 	});	
// });


jQuery(document).ready(function(){
    jQuery("#edit-dob").datepicker({
        dateFormat: "dd/mm/yy",
        changeMonth: true,
        changeYear: true,
        yearRange: "1947:" + (new Date().getFullYear() - 18),
        minDate: new Date(1947, 0, 1),
        maxDate: "-18Y",
        autoSize: true
    }); 
});

jQuery(document).ready(function(){
		jQuery("#edit-doj").datepicker({
		dateFormat: "dd/mm/yy",
		//minDate: minDoj,
		changeMonth: true,
		changeYear: true,
		yearRange: "-80:+0",
		maxDate: "0",
		autoSize: true
	});	
});
//////added by dg 16-12-2024////
jQuery(document).ready(function(){
		jQuery("#edit-dor").datepicker({
		dateFormat: "dd/mm/yy",
		//minDate: minDoj,
		changeMonth: true,
		changeYear: true,
		yearRange: "-80:+70",
		//minDate: "0",
		autoSize: true
	});	
});

// jQuery(document).ready(function(){
// 	jQuery("#edit-dol").datepicker({
// 	dateFormat: "dd/mm/yy",
// 	//minDate: minDoj,
// 	changeMonth: true,
// 	changeYear: true,
// 	//yearRange: "-0:+70",
// 	//minDate: "0",
// 	autoSize: true
// });	
// });

jQuery(document).ready(function(){
    jQuery("#edit-doa").datepicker({
		dateFormat: "dd/mm/yy",
        changeMonth: true,
        changeYear: true,
        yearRange: "1947:" + new Date().getFullYear(),
        minDate: new Date(1947, 0, 1),
        maxDate: new Date(),
        autoSize: true
    }); 
});

jQuery(document).ready(function(){
    jQuery("#edit-dol").datepicker({
        dateFormat: "dd/mm/yy",
        changeMonth: true,
        changeYear: true,
        yearRange: "1947:+25", // Allows selection from 1970 to 70 years ahead
        minDate: new Date(1947, 0, 1), // Optional: restricts selection before 1970
        autoSize: true
    }); 
});

jQuery(document).ready(function(){
	jQuery("#edit-possession").datepicker({
	dateFormat: "dd/mm/yy",
	//minDate: minDoj,
	changeMonth: true,
	changeYear: true,
	//yearRange: "-0:+70",
	//minDate: "0",
	autoSize: true
});	
});

jQuery(document).ready(function(){
	jQuery("#edit-release").datepicker({
	dateFormat: "dd/mm/yy",
	//minDate: minDoj,
	changeMonth: true,
	changeYear: true,
	//yearRange: "-0:+70",
	//minDate: "0",
	autoSize: true
});	
});

/*Done by Subham 05-06-2025*/
jQuery(document).ready(function(){
    jQuery("#edit-don").datepicker({
        dateFormat: "dd/mm/yy",
        changeMonth: true,
        changeYear: true,
        yearRange: "2000:+15", // Allows selection starting from 2000 to current year +15 years ahead
        minDate: new Date(2000, 0, 1), // Optional: restricts selection before 2000
        autoSize: true
    }); 
});
/*End*/

/*Done by Subham 13-06-2025*/
jQuery(document).ready(function(){
    jQuery("#edit-date-of-birth").datepicker({
        dateFormat: "dd/mm/yy",
        changeMonth: true,
        changeYear: true,
        yearRange: "1947:" + new Date().getFullYear(),
        minDate: new Date(1947, 0, 1),
        maxDate: new Date(),
        autoSize: true
    }); 
});
/*End*/

/*Done by Subham 24-07-2025*/
jQuery(document).ready(function(){
    jQuery("#edit-issue-date").datepicker({
        dateFormat: "dd/mm/yy",
        changeMonth: true,
        changeYear: true,
        yearRange: "1947:" + new Date().getFullYear(),
        minDate: new Date(1947, 0, 1),
        maxDate: new Date(),
        autoSize: true,
        onSelect: function(selectedDate) {
            var parts = selectedDate.split('/');
            var day = parseInt(parts[0], 10);
            var month = parseInt(parts[1], 10) - 1; // zero-based month
            var year = parseInt(parts[2], 10);

            var issueDate = new Date(year, month, day);
            // Set minDate for expiry datepicker = issueDate (same day)
            // Set maxDate = exactly 3 years after issueDate
            var maxExpiryDate = new Date(issueDate);
            maxExpiryDate.setFullYear(maxExpiryDate.getFullYear() + 3);

            // Update expiry datepicker options dynamically
            jQuery("#edit-expiry-date").datepicker("option", "minDate", issueDate);
            jQuery("#edit-expiry-date").datepicker("option", "maxDate", maxExpiryDate);
        }
    });
	/*End*/

	/*Done by Subham 24-07-2025*/
    jQuery("#edit-expiry-date").datepicker({
        dateFormat: "dd/mm/yy",
        changeMonth: true,
        changeYear: true,
        yearRange: "1947:+15",
        minDate: new Date(1947, 0, 1), 
        autoSize: true
    });
});
/*End*/


// /*Done by Subham 13-06-2025*/								// Turned off 24-07-2025 by Subham
// jQuery(document).ready(function(){
//     jQuery("#edit-issue-date").datepicker({
//         dateFormat: "dd/mm/yy",
//         changeMonth: true,
//         changeYear: true,
//         yearRange: "1947:" + new Date().getFullYear(),
//         minDate: new Date(1947, 0, 1),
//         maxDate: new Date(),
//         autoSize: true
//     }); 
// });
// /*End*/

// /*Done by Subham 13-06-2025*/
// jQuery(document).ready(function(){
//     jQuery("#edit-expiry-date").datepicker({
//         dateFormat: "dd/mm/yy",
//         changeMonth: true,
//         changeYear: true,
//         yearRange: "1947:+15", // Allows selection starting from 2000 to current year +15 years ahead
//         minDate: new Date(1947, 0, 1), // Optional: restricts selection before 2000
//         autoSize: true
//     }); 
// });
// /*End*/

/*Done by Subham 30-06-2025*/
jQuery(document).ready(function(){
    jQuery("#edit-license-renewal-date").datepicker({
        dateFormat: "dd/mm/yy",
        changeMonth: true,
        changeYear: true,
        yearRange: "1947:" + new Date().getFullYear(),
        minDate: new Date(1947, 0, 1),
        maxDate: new Date(),
        autoSize: true
    }); 
});
/*End*/

// jQuery(document).ready(function () {   //previous code
	
// 	var minDateStr = Drupal.settings.application_status_check.date_only; // e.g., '2025-05-06'
// 	var minDateObj = new Date(minDateStr); // Converts string to Date object
// 	var maxDateObj = new Date(minDateObj); 
// 	maxDateObj.setDate(minDateObj.getDate() + 14); // Add 14 days
  
// 	jQuery("#edit-extension").datepicker({
// 	  dateFormat: "dd/mm/yy",
// 	  minDate: minDateObj,
// 	  maxDate: maxDateObj,
// 	  changeMonth: true,
// 	  changeYear: true,
// 	  yearRange: "-0:+70",
// 	  autoSize: true
// 	});
// });


/* added by dg because of getting error for previous code */ 
<<<<<<< HEAD
jQuery(document).ready(function () {
  // Ensure the Drupal settings object exists and contains the expected date
  if (Drupal.settings && Drupal.settings.application_status_check && Drupal.settings.application_status_check.date_only) {

    var minDateStr = Drupal.settings.application_status_check.date_only; // e.g., '2025-05-06'
    
    // Convert 'yyyy-mm-dd' to Date object
    var parts = minDateStr.split('-');
    var minDateObj = new Date(parts[0], parts[1] - 1, parts[2]); // Month is 0-based

    var maxDateObj = new Date(minDateObj);
    maxDateObj.setDate(minDateObj.getDate() + 14); // Add 14 days to minDate

    // Initialize the datepicker
    jQuery("#edit-extension").datepicker({
      dateFormat: "dd/mm/yy",
      minDate: minDateObj,
      maxDate: maxDateObj,
      changeMonth: true,
      changeYear: true,
	//   yearRange: `${minDateObj.getFullYear()}:${maxDateObj.getFullYear()}`,
      yearRange: "-0:+70",
      autoSize: true
    });
    
  } 
//   else {
//     console.error("Required Drupal setting 'application_status_check.date_only' is missing.");
//   }
=======
jQuery(document).ready(function ($) {
  if (
    typeof Drupal !== "undefined" &&
    Drupal.settings.application_status_check &&
    Drupal.settings.application_status_check.current_date &&
    Drupal.settings.application_status_check.extension_upto_date
  ) {

    // Min date (date_only)
    var minDateStr = Drupal.settings.application_status_check.current_date;
    var minParts = minDateStr.split('-');
    var minDateObj = new Date(minParts[0], minParts[1] - 1, minParts[2]);

    // Max date (extension_upto_date)
    var maxDateStr = Drupal.settings.application_status_check.extension_upto_date;
    var maxParts = maxDateStr.split('-');
    var maxDateObj = new Date(maxParts[0], maxParts[1] - 1, maxParts[2]);

    $("#edit-extension").datepicker({
      dateFormat: "dd/mm/yy",
      changeMonth: true,
      changeYear: true,
      minDate: minDateObj,
      maxDate: maxDateObj,
      autoSize: true
    });
  }
>>>>>>> 2502ab393f49b61f25516e6bb0502acbb6d447a0
});
/* end */

// jQuery(document).ready(function () {
	
// 	var minDateStr = Drupal.settings.application_status_check.date_only; // e.g., '2025-05-06'
// 	var minDateObj = new Date(minDateStr); // Converts string to Date object
// 	var maxDateObj = new Date(minDateObj); 
// 	maxDateObj.setDate(minDateObj.getDate() + 14); // Add 14 days
  
// 	jQuery("#edit-extension").datepicker({
// 	  dateFormat: "dd/mm/yy",
// 	  minDate: minDateObj,
// 	  maxDate: maxDateObj,
// 	  changeMonth: true,
// 	  changeYear: true,
// 	  yearRange: "-0:+70",
// 	  autoSize: true
// 	});
// });

jQuery(document).ready(function(){  //by dg 07-12-2025
	jQuery("#edit-extension-license").datepicker({
	dateFormat: "dd/mm/yy",
	changeMonth: true,
	changeYear: true,
	yearRange: "-0:+70",
<<<<<<< HEAD
	autoSize: true
=======
	autoSize: true,
	minDate: +1, // Set minDate to tomorrow
>>>>>>> 2502ab393f49b61f25516e6bb0502acbb6d447a0
});	
});

//end///

/*jQuery(document).ready(function(){
	
	   var dob = jQuery('#edit-dob').val();
       var dob_arr =  dob.split("/");
	   var day = parseInt(dob_arr[0]) ;
	   var month = parseInt(dob_arr[1]);
	   var year = parseInt(dob_arr[2]) + 18 ;
	   var minDoj = day +'/'+ month +'/'+ year; 
	  
	  
		jQuery("#edit-doj").datepicker({
		dateFormat: "dd/mm/yy",
		minDate: minDoj,
		changeMonth: true,
		changeYear: true,
		yearRange: "-70:+0",
		autoSize: true
	});
	
});*/

/*jQuery(document).ready(function(){
    jQuery('.cutcopypasteoff').bind("cut copy paste",function(e) {
        e.preventDefault();
    });
});*/


/*function isNumberKey(evt) {
    var charCode = (evt.which) ? evt.which : event.keyCode;
    if (charCode != 46 && charCode > 31
    && (charCode < 48 || charCode > 57))
        return false;

    return true;
}*/

	function validate_common_application_form(){
		//alert('OK'); 
		//return false;
		var formElements =
		{
			"applicant_name":["Applicant's Name","alphabetFullstop"],
			"applicant_father_name":["Father's / Husband's Name","alphabetFullstop"],
			
			"permanent_street":["Permanent Address", "streetAddress"],
			//"permanent_city_town_village":["City / Town / Village", "onlyAlphabet"],
			"permanent_post_office":["Post Office", "hyphenFullStop"],
			"permanent_district":["District", "selectBox"],
			"permanent_pincode":["Pincode", "onlyNo"],
		}
		
		//if(jQuery("#chk_present_address").not(':checked')) {
		if(jQuery("#edit-chk-permanent-address-1").not(':checked')) {
			var formElements1 = 
			{
				"present_street":["Present Address", "streetAddress"],
				//"present_city_town_village":["City / Town / Village", "onlyAlphabet"],
				"present_post_office":["Post Office", "hyphenFullStop"],
				"present_district":["District", "selectBox"],
				"present_pincode":["Pincode", "onlyNo"],
			}
			formElements = jQuery.extend(formElements, formElements1);
		}
		
		var formElements2 =
		{
			"dob":["Date of Birth","date"],
			"gender":["Gender", "radioBox"],
			"mobile":["Mobile No", "mobileNo"],
			"email":["Email Id", "emailAddress"],
			
			"hrms_id":["Employee HRMS ID", "hrmsId"],
			"app_designation":["Designation", "textArea"],
			"pay_band":["Pay Band", "selectBox"],
			"pay_in":["Pay in the Pay Band", "onlyNo"],
			"grade_pay":["Grade Pay", "onlyNo"],
			//"gpf_no":["GPF No", "gpfNo"],
			//"app_posting_place":["Place of Posting", "onlyAlphabet"],
			//"app_headquarter":["Headquarter", "onlyAlphabet"],
			"doj":["Date of Joining", "date"],
			"dor":["Date of Retirement", "date"],
			
			"office_name":["Name of the Office", "textArea"],
			//"office_street":["Office Address", "streetAddress"],
			"office_city":["City / Town / Village", "onlyAlphabet"],
			"office_post_office":["Post Office", "hyphenFullStop"],
			"office_district":["District", "selectBox"],
			"office_pincode":["Pincode", "onlyNo"],
			//"office_phone_no":["Telephone No.", "telephoneNo"],
			
			"district":["DDO District", "selectBox"],
			"designation":["DDO Designation", "selectBox"],
			"dol":["Date of License","date"],
			
		};
		
		formElements = jQuery.extend(formElements, formElements2);
		
		return formElements;	
	
		/*if(!validateForm(formElements)) {
			return false;
		} else {
			return true;
		}*/
	}
	
	
	
	/*jQuery(document).ready(function(){
	
		jQuery('#chk_present_address').click(function () {
			if(jQuery(this).is(':checked')) {
				jQuery('#present_street').hide();
				jQuery('#present_city_town_village').hide();
				jQuery('#present_post_office').hide();
				jQuery('#present_district').hide();
				jQuery('#present_pincode').hide();
				
				jQuery('#present_street').val('');
				jQuery('#present_city_town_village').val('');
				jQuery('#present_post_office').val('');
				jQuery('#present_district').val('');
				jQuery('#present_pincode').val('');
				
				jQuery('#edit-present-address').hide();
			
			} else if(jQuery(this).not(':checked')) {
				jQuery('#present_street').show();
				jQuery('#present_city_town_village').show();
				jQuery('#present_post_office').show();
				jQuery('#present_district').show();
				jQuery('#present_pincode').show();
				
				jQuery('#edit-present-address').show();
			
			}
		});
	
	});
	
	jQuery(document).ready(function(){
		if(jQuery('#chk_extra_doc').is(':checked')) {
			jQuery('#present_street').hide();
			jQuery('#present_city_town_village').hide();
			jQuery('#present_post_office').hide();
			jQuery('#present_district').hide();
			jQuery('#present_pincode').hide();
			
			jQuery('#present_street').val('');
			jQuery('#present_city_town_village').val('');
			jQuery('#present_post_office').val('');
			jQuery('#present_district').val('');
			jQuery('#present_pincode').val('');
			
			jQuery('#edit-present-address').hide();
		} else {
			jQuery('#present_street').show();
			jQuery('#present_city_town_village').show();
			jQuery('#present_post_office').show();
			jQuery('#present_district').show();
			jQuery('#present_pincode').show();
			
			jQuery('#edit-present-address').show();
		}	
	});*/

