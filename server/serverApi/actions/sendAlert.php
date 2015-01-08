<?php

/* 
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
require_once("../config/global_config.php");
require_once($GLOBALS['ROOT'].'serverApi/tools/utils.php');

if (empty($_GET)) {
    $tab = $_POST;
} else {
    $tab = $_GET;
}

if (array_key_exists("user_id", $tab) && !empty($tab["user_id"])) {
    $userId = $tab["user_id"];
} else {
    header('HTTP/1.1 400 Bad Request');
    return;
}

if (array_key_exists("alert_type", $tab) && !empty($tab["alert_type"])) {
    $alertType = $tab["alert_type"];
} else {
    header('HTTP/1.1 400 Bad Request');
    return;
}

if (array_key_exists("alert_level", $tab) && !empty($tab["alert_level"])) {
    $alertLevel = $tab["alert_level"];
} else {
    header('HTTP/1.1 400 Bad Request');
    return;
}

if (array_key_exists("stream_url", $tab) && !empty($tab["stream_url"])) {
    $streamUrl = $tab["stream_url"];
} else {
    $streamUrl = "";
}

$db = getDatabaseConnection();

$insert = "insert into alerts (user_id, alert_type_id, alert_date, alert_time, alert_level) "
        . "values (?, ?, current_date, current_time,?);";

$rs = $db->Execute($insert,array($userId,$alertType,$alertLevel));
if (!$rs) {
    if ($DEBUG) {
        echo $db->ErrorMsg();
    } else {
        header('HTTP/1.1 400 Bad Request');
        return;
    }
}

if ($alertLevel >= 2) {
    $messages = array (
    1=>"La personne que vous suivez n'a pas pris son taitement",
    2=>"La personne que vous suivez a chuté et n'a pas donné d'informations",
    3=>"La personne que vous suivez demande de l'aide"
    );
    
    $select = "Select push_id "
            . "from supervisors as s, users as u "
            . "where u.id = ? "
            . "and u.supervisor_id = s.id ;";
    
    $rs = $db->Execute($select,array($userId));
    if (!$rs) {
        if ($DEBUG) {
            echo $db->ErrorMsg();
        } else {
            header('HTTP/1.1 400 Bad Request');
            return;
        }
    }
    
    $time = date('Y-m-d');
    $timeArray = getdate();
    $hour = $timeArray["hours"].":".$timeArray["minutes"].":".$timeArray["seconds"];
    if ($array = $rs->FetchRow()) {
        $pushId = $array["push_id"];
        
        $json = array("message" => $messages[$alertType],
            "date"=>$time,
            "hour"=>$hour);
        
        if (empty($streamUrl)) {
            $json["is_stream_available"] = false;
        } else {
            $json["is_stream_available"] = true;
            $json["stream_addr"] = $streamUrl;
        }
        $jsonString = json_encode($json);
        
        sendPush($jsonString,$pushId);
    } 
}