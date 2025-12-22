<?php 

if ($_SERVER['REQUEST_METHOD'] == 'POST') {
    ddoSubmit();
}

 function ddoSubmit(){
        //dd('ss');

        // $hmac_secret="8392512033044595";
        $hmac_secret= "1Po/Rx7oUnNzy9QZ7NZJjA==";
        

        $req = array(
            'src'=>'NIC',
            'hrmsid'=>'1995002971', //'19921209876',
            'email'=>'test@example.com',
            'mobile'=>'9999999999',
            'name'=>'John Doe',
            'designation'=>'DDO',
            'ddo_code'=> 'COAHMA001',//'SPAHMA001', //'CABHOA001', //'6613',
            'status'=>'authenticated',
            'sysTimeStamp'=>date('d/m/Y H:i:s')
        );
        $jsonData =json_encode($req); //dd($jsonData);

        $encryptedData = encrypt($jsonData);

        $checksum=hash_hmac('sha256', mb_convert_encoding($jsonData, "UTF-8"),$hmac_secret);

        $payload = array(
            'encData' => $encryptedData,
            'cs' => $checksum, // Send IV so the recipient can decrypt
        );

        $payloadJson = json_encode($payload);   print($payloadJson); die;
        
        $curl = curl_init();

        curl_setopt_array($curl, array(
          //CURLOPT_URL => 'http://10.173.42.87:8080/rhewbhousing/rhe-wbhousing-v2/auth/login-ddo',
	    //   CURLOPT_URL => 'http://172.25.142.221/rhewbhousing/auth/login-ddo',
          CURLOPT_URL => 'https://rhe.wb.gov.in/auth/login-ddo',
        //   CURLOPT_URL => 'http://localhost/housing/auth/login-ddo',
          CURLOPT_RETURNTRANSFER => true,
          CURLOPT_ENCODING => '',
          CURLOPT_MAXREDIRS => 10,
          CURLOPT_TIMEOUT => 5,
          //CURLOPT_FOLLOWLOCATION => false,
          CURLOPT_HTTP_VERSION => CURL_HTTP_VERSION_1_1,
          CURLOPT_FOLLOWLOCATION=>0,
          CURLOPT_CUSTOMREQUEST => 'POST', 
          CURLOPT_POSTFIELDS => $payloadJson,
          CURLOPT_HTTPHEADER => array(
            'Content-Type: application/json',
            //'Content-Length: ' . strlen($payloadJson),
            // 'redirect_url: http://10.173.42.87:8080/rhewbhousing/rhe-wbhousing-v2/dashboard' 
          )
        ));

        $response = curl_exec($curl);  //dd($response);
        // Get the response headers
        $header = curl_getinfo($curl);
        // echo $header['redirect_url'];die;

        // Check if a redirect is being issued
        if (isset($header['redirect_url']) && $header['redirect_url']) {
            echo 'Redirecting to: ' . $header['redirect_url'];
        } else {
            echo 'No redirect, response: ' . $response;
        }
        //dd();

        curl_close($curl);
        if (isset($header['redirect_url']) && $header['redirect_url']) {
            header("Location: " . $header['redirect_url']);
            exit();
        }else{
            // header("Location: http://172.25.142.221/hrms-test-api/index.php");
            // header("Location: https://rhe.wb.gov.in/hrms-test-api/index.php");
        }

    }   


    function encrypt($data) {
        
        $information = $data;
        $cipher = "AES-256-CBC";
        // $cipher='aes-128-cbc';//to be shared with HRMS team
        // $secret = "3652874125963346";//uat////to be shared with HRMS team
        //$secret="8392512033044595";//prod
        //$option = "0" ;
        // $iv = str_repeat("0",openssl_cipher_iv_length($cipher));
        // $iv = "plkygtwsersuytvb";//uat//to be shared with HRMS team
        //$iv = "cbdhnwwwqkloieow";//prod

        //$encrptedString = openssl_encrypt($information, $cipher, $secret, $option , $iv);


        $secret= 'DDThkqkxlOYQzpZUbqnfEGir5mWHV5mY';
        // $iv = 'ykDWpfWyXXjTY0bguBzKmcEFZTINLPEe';
        $iv = 'ykDWpfWyXXjTY0bg';

        $encrptedString=openssl_encrypt($information,$cipher,$secret,OPENSSL_RAW_DATA,$iv);
        $encrptedString=base64_encode($encrptedString);

        return $encrptedString;
    } 


?>