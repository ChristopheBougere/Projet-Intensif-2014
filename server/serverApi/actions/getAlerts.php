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

$db = getDatabaseConnection();

$select = "select alert_type_id, alert_level, alert_date, alert_time "
        . "from alerts "
        . "where user_id = ? "
        . "order by alert_date desc, alert_time desc;";

$rs = $db->Execute($select,array($userId));
if (!$rs) {
    if ($DEBUG) {
        echo $db->ErrorMsg();
    } else {
        header('HTTP/1.1 400 Bad Request');
        return;
    }
}
$alerts = array();
while ($array = $rs->FetchRow()) {
    $alertType = intval($array["alert_type_id"]);
    $alertLevel = intval($array["alert_level"]);
    $date = $array["alert_date"];
    $hour = $array["alert_time"];
    $alerts[] = array("type"=>$alertType,"date"=>$date,"hour"=>$hour,"alert_level"=>$alertLevel);
}
$jsonString = json_encode($alerts);

header('Content-Type: application/json');
echo $jsonString;
