<?php
/* 
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

function getDatabaseConnection() {
    require_once($GLOBALS['ROOT'] . 'serverApi/config/database_config.php');
    require_once($GLOBALS['ROOT'] . 'serverApi/lib/adodb5/adodb.inc.php');
    $DB = NewADOConnection('postgres');
    $DB->Connect($DATABASE_HOST, $DATABASE_USER, $DATABASE_PASSWORD, $DATABASE_NAME);
	//$DB->debug = 1;
    return $DB;
}
function sendPush($msg,$pushID){
    $pushStatus = "";	
    $gcmRegID  = $pushID;
    $pushMessage = $msg;		
    $gcmRegIds = array($gcmRegID);
    $message = array("m" => $pushMessage);	// la variable recue sur l'appli android s'apelle "m"
    $pushStatus = sendPushNotificationToGCM($gcmRegIds, $message);
    echo $pushStatus; 
}

    //generic php function to send GCM push notification
function sendPushNotificationToGCM($registration_ids, $message) {
    //Google cloud messaging GCM-API url
    $url = 'https://android.googleapis.com/gcm/send';
    $fields = array(
        'registration_ids' => $registration_ids,
        'data' => $message,
    );
            // Google Cloud Messaging GCM API Key
    define("GOOGLE_API_KEY", "AIzaSyBHUQ6I6V2AZOIFh0ORIAAjIA0lqIXLYPU"); 		
    $headers = array(
        'Authorization: key=' . GOOGLE_API_KEY,
        'Content-Type: application/json'
    );
    $ch = curl_init();
    curl_setopt($ch, CURLOPT_URL, $url);
    curl_setopt($ch, CURLOPT_POST, true);
    curl_setopt($ch, CURLOPT_HTTPHEADER, $headers);
    curl_setopt($ch, CURLOPT_RETURNTRANSFER, true);
            curl_setopt ($ch, CURLOPT_SSL_VERIFYHOST, 0);	
    curl_setopt($ch, CURLOPT_SSL_VERIFYPEER, false);
    curl_setopt($ch, CURLOPT_POSTFIELDS, json_encode($fields));
    $result = curl_exec($ch);				
    if ($result === FALSE) {
        die('Curl failed: ' . curl_error($ch));
    }
    curl_close($ch);
    return $result;
}

//	sendPush("kinzeur zerau uite","APA91bFkVQdnGkIgi2q0Uv6dWKGU6V_3jwoUKYkV2xv0RlYPuZlje3Rmdj0qvjVyH7wLnPqPmNQDZ8cJQ6oX2TPkTlXmIeqSNlW2l83khxbcILdECQP6gEv6K2Vfv5Te1VvYJpv91tHygQ2VBofsWX_70dNBfB2aig");

