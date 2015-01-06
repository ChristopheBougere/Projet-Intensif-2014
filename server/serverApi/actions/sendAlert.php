<?php

/* 
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
require_once("../tools/utils.php");
require_once("../config/global_config.php");

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

$db = getDatabaseConnection();

$insert = "insert into alerts (user_id, alert_type_id, alert_date, alert_time) "
        . "values (?, ?, current_date, current_time);";

$rs = $db->Execute($insert,array($userId,$alertType));
if (!$rs) {
    if ($DEBUG) {
        echo $db->ErrorMsg();
    } else {
        header('HTTP/1.1 400 Bad Request');
        return;
    }
}

// TODO gérer le push au superviseur si besoin ?
