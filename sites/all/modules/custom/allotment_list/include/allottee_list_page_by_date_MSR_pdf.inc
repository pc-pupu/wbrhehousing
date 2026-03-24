<?php

	function allottee_list_page_by_date_MSR_pdf($allotment_process_date = '', $allotment_process_type = '', $file_name = '') {

		$allotment_process_date = decrypt_url($allotment_process_date);
		$allotment_process_type = decrypt_url($allotment_process_type);
		
		date_default_timezone_set("Asia/Kolkata");
		
		$page_output = '';
		
		$page_output = '<!doctype html>
		<html>
		<head>
		<meta charset="utf-8">
		<link rel="shortcut icon" href="http://localhost/rhewbhousing/misc/favicon.ico" type="image/vnd.microsoft.icon" />
		<title>Allotees List</title>
		</head>	
		<body>	
		<div align="center">
			<img src="'.drupal_get_path('module', 'allotment_list').'/images/govt_logo1.png" alt="Government Logo">
			<br>
			Government of West Bengal
			<br>
			Housing Department : Allotment Branch
			<br>
			1, K. S Roy Road, N.S. Building, 1st Floor, Kolkata - 700001
			<br><br>
			<font style="font-size: 14px;"><b>e-Allotment of Rental Housing Estate</b></font>
			<br><br>
		</div>';
		if($allotment_process_type == 'MSR') {  

				$result = fetch_all_allottee_details_by_date_MSR($allotment_process_date, $allotment_process_type);
				
				$page_output .= '<div align="left"><font style="font-size: 16px;">Proposal for allotment of flats at Government Rental Housing Estate at Different R.H.E. of Flat  (Manual Allotment)</font></div>';
				
				$page_output .= '<br>
				<div align="center">
					<font style="font-size: 14px;"><b>Date of Allotment - '.implode('/', array_reverse(explode('-', $allotment_process_date))).'</b></font>
				</div>
				<br>
				<div align="right">
					<font size="3"><b>Date : </b>'.date('l d-m-Y h:i:s A').'</font>
				</div>
				<br>
				<div>
					<table style="font-family: Arial, sans-serif; border: 1px solid black; border-collapse: collapse;" align="center" width="100%">
						<thead>
							<tr bgcolor="#999999">
								<th align="left" width="4%" style="border: 1px solid black; border-collapse: collapse;"><font size="5"><b>Sl. No.</b></font></th>
								<th align="center" width="18%" style="border: 1px solid black; border-collapse: collapse;"><font size="5"><b>Name and designation</b></font></th>
								<th align="center" width="20%" style="border: 1px solid black; border-collapse: collapse;"><font size="5"><b>1. Application No.<br>2. Place of Posting<br>3. Date of Application</b></font></th>
								<th align="center" width="10%" style="border: 1px solid black; border-collapse: collapse;"><font size="5"><b>Flat Allotted</b></font></th>
								<th align="center" width="8%" style="border: 1px solid black; border-collapse: collapse;"><font size="5"><b>Floor</b></font></th>
								<th align="center" width="12%" style="border: 1px solid black; border-collapse: collapse;"><font size="5"><b>Name of R.H.E.</b></font></th>
								<th align="center" width="12%" style="border: 1px solid black; border-collapse: collapse;"><font size="5"><b>Name of Block</b></font></th>
								<th align="center" width="12%" style="border: 1px solid black; border-collapse: collapse;"><font size="5"><b>Date of Retirement</b></font></th>
								<th align="right" width="16%" style="border: 1px solid black; border-collapse: collapse;"><font size="5"><b>Allotment Approval Status</b></font></th>
								<th align="right" width="16%" style="border: 1px solid black; border-collapse: collapse;"><font size="5"><b>Application Status</b></font></th>
							</tr>
						</thead>
					';
					$i = 1;
					if($result->rowCount() > 0) {
						while($data = $result->fetchObject()) {
							if(trim($data->floor) == 'F') {
								$floor = '1st';	
							} else if(trim($data->floor) == 'S') {
								$floor = '2nd';	
							} else {
								$floor = $data->floor;	
							}
						$allotment_status=fetchAllotmentStatus($data->online_application_id);
						$page_output .='
						<tbody>
							<tr>
								<td align="left" style="border: 1px solid black; border-collapse: collapse;"><font size="4">'.$i.'</font></td>
								<td align="center" style="border: 1px solid black; border-collapse: collapse;"><font size="4">'.$data->applicant_name.'<br>'.$data->applicant_designation.'</font></td>
								<td align="center" style="border: 1px solid black; border-collapse: collapse;"><font size="4">'.'1. '.$data->application_no.'<br>'.'2. '.$data->applicant_posting_place.'<br>'.'3. '.implode('/', array_reverse(explode('-', $data->date_of_application))).'</font></td>
								<td align="center" style="border: 1px solid black; border-collapse: collapse;"><font size="4">'.$data->flat_no.'</font></td>
								<td align="center" style="border: 1px solid black; border-collapse: collapse;"><font size="4">'.$floor.'</font></td>
								<td align="center" style="border: 1px solid black; border-collapse: collapse;"><font size="4">'.$data->estate_name.'</font></td>
								<td align="right" style="border: 1px solid black; border-collapse: collapse;"><font size="4">'.$data->block_name.'</font></td>
								<td align="center" style="border: 1px solid black; border-collapse: collapse;"><font size="4">'.implode('/', array_reverse(explode('-', $data->date_of_retirement))).'</font></td>
								
								<td align="right" style="border: 1px solid black; border-collapse: collapse;"><font size="4">'.$allotment_status.'</font></td>
								<td align="right" style="border: 1px solid black; border-collapse: collapse;"><font size="4">'.$data->status_description.'</font></td>
							</tr>
						</tbody>';
						
						$i++;
						}
					}
					$page_output .='</table>
				</div>';
			
				
				$page_output .='<div>
				<br><br><br><br><br><br>
				<table width="100%" align="center">
					<tr>
						<td style="font-size:15px;" align="center" width="35%">
							<b>Competent Authority<br>
							Housing Department</b>
						</td>
						<td width="45%"></td>
						<td style="font-size:15px;" align="center" width="40%">
							<b>Principal Secretary<br>
							Housing Department</b>
						</td>
					</tr>
				</table>
				</div>';
				
		}
	$page_output .='</body>
</html>';		
			
		//echo $page_output;
		
		ob_end_clean();
		return pdf_using_mpdf_api($page_output, $file_name);
	}