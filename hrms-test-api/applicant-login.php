<?php 

if ($_SERVER['REQUEST_METHOD'] == 'POST') {
    employeeSubmit();
}

function employeeSubmit(){
    
        // $hmac_secret="8392512033044595";
        $hmac_secret= "1Po/Rx7oUnNzy9QZ7NZJjA==";

        $req = array(
            'src'=>'HRMS',
            'hrmsid'=>'1995002970', //'19921209876',
            'email'=>'test@example.com',
            'mobile'=>'9999999999',
            'name'=>'John Doe',
            'designation'=>'Scientific Officer/Engineer-SB',
            'status'=>'authenticated',
            'sysTimeStamp'=>date('d/m/Y H:i:s')
        );
        $jsonData =json_encode($req); //dd($jsonData);

        $encryptedData = encrypt($jsonData);

        $checksum=hash_hmac('sha256', mb_convert_encoding($jsonData, "UTF-8"),$hmac_secret);

        $payload = array(
            'encdata' => $encryptedData,
            'cs' => $checksum, // Send IV so the recipient can decrypt
        );

        $payloadJson = json_encode($payload); // print($payloadJson); die;
        
        $curl = curl_init();

        curl_setopt_array($curl, array(
          //CURLOPT_URL => 'http://10.173.42.87:8080/rhewbhousing/rhe-wbhousing-v2/auth/login-hrms',
	      //CURLOPT_URL => 'http://172.25.142.221/rhewbhousing/auth/login-hrms',
        //   CURLOPT_URL => 'http://172.25.142.221/auth/login-hrms',
          CURLOPT_URL => 'http://localhost/housing/auth/login-hrms',
          CURLOPT_RETURNTRANSFER => true,
          CURLOPT_ENCODING => '',
          CURLOPT_MAXREDIRS => 10,
          CURLOPT_TIMEOUT => 5,
          //CURLOPT_FOLLOWLOCATION => true,
          CURLOPT_HTTP_VERSION => CURL_HTTP_VERSION_1_1,
          CURLOPT_FOLLOWLOCATION=>0,
          CURLOPT_CUSTOMREQUEST => 'POST',
          CURLOPT_POSTFIELDS => $payloadJson,
          CURLOPT_TIMEOUT => 100,
          CURLOPT_HTTPHEADER => array(
            'Content-Type: application/json',
            //'Content-Length: ' . strlen($payloadJson),
            // 'redirect_url: http://10.173.42.87:8080/rhewbhousing/rhe-wbhousing-v2/dashboard'
          )
        ));

        $response = curl_exec($curl);  //dd($response);
        // Get the response headers
        $header = curl_getinfo($curl);
        //print_r($header);
        //echo $header['redirect_url'];die;

        // Check if a redirect is being issued
        // if (isset($header['redirect_url']) && $header['redirect_url']) {
        //     echo 'Redirecting to: ' . $header['redirect_url'];
        // } else {
        //     echo 'No redirect, response: ' . $response;
        // }


        curl_close($curl);
        //echo $response;
        //dd();
        if (isset($header['redirect_url']) && $header['redirect_url']) {
            //return redirect($header['redirect_url']);
             header("Location: " . $header['redirect_url']);
             exit();
        }else{
            header("Location: http://localhost/housing/hrms-test-api/index.php");
        }


        //new code start to solve redirect_url missing issue-- turned off cz it didnt work--might be solved by whitelisting
        // If you want to check for a manual redirect, parse the response headers
        /*$http_code = curl_getinfo($curl, CURLINFO_HTTP_CODE);


        if(curl_errno($curl)) {
            echo "cURL error: " . curl_error($curl);
            exit();
        }

        // Close the cURL session
        curl_close($curl);
        echo 'httpcode'.$http_code;

        if ($http_code == 302 || $http_code == 301) {
          
            // Extract the redirect URL from the headers
            preg_match('/Location: (.*?)\n/', $response, $matches);
            $redirect_url = $matches[1];
            
            // Now you can redirect manually
            header("Location: " . $redirect_url);
            exit();
        } else {
            // No redirect, proceed as normal
            header("Location: http://172.25.142.221/hrms-test-api/index.php");
            exit();
        }*/
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
        $iv = 'ykDWpfWyXXjTY0bguBzKmcEFZTINLPEe';

        $encrptedString=openssl_encrypt($information,$cipher,$secret,OPENSSL_RAW_DATA,$iv);
        $encrptedString=base64_encode($encrptedString);

        return $encrptedString;
    }  


    ?>

