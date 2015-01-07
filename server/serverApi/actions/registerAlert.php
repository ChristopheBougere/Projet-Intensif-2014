<?php

require_once '../config/global_config.php';
require_once '../tools/utils.php';
/* 
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
$user_ids = array(1, 2, 3, 4, 5, 6, 7);
$alert_type_ids = array(1, 2);

$user_id = $user_id[rand(0,count($user_id))];
$alert_type_id = $alert_type_ids[rand(0,count($alert_type_ids))];

$db = getDatabaseConnection();
$select = "select max(id) as new_id from users";
$rSelect = $db->Execute($select,array());

if (!$rSelect) {
    if ($DEBUG) {
        echo $db->ErrorMsg();
    } else {
        header('HTTP/1.1 400 Bad Request');
        return;
    }
}

if ($array = $rSelect->FetchRow()) {
    $newId = intval($array["new_id"])+1;
} else {
    header('HTTP/1.1 400 Bad Request');
    return; 
}

$insert = "insert into users(id,name,first_name,doctor_id) values (?,?,?,?)";
$rs = $db->Execute($insert,array($newId,$name,$firstName,$doctorId));
if (!$rs) {
    if ($DEBUG) {
        echo $db->ErrorMsg();
    } else {
        header('HTTP/1.1 400 Bad Request');
        return;
    }
}

$json = array("user_id" => $newId);

$jsonString = json_encode($json);

header('Content-Type: application/json');
echo $jsonString;
