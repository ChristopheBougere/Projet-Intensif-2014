<?php

require_once '../config/global_config.php';
require_once $GLOBALS['ROOT'].'serverApi/tools/utils.php';

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

$request = "select count(*) as c "
        . "from alerts "
        . "where user_id = ? "
        . "and alert_type_id = 2 "
        . "and alert_date + alert_time+ interval '15 second' > current_date + current_time ";

$rs = $db->Execute($request,array($userId));
if (!$rs) {
    if ($DEBUG) {
        header('HTTP/1.1 400 Bad Request');
        echo $db->ErrorMsg();
        return;
    } else {
        header('HTTP/1.1 400 Bad Request');
        return;
    }
}
if ($array = $rs->FetchRow()) {
    if ($array["c"] > 0) {
        $taken = 1;
    } else {
        $taken = 0;
    }
    $json = array("recentFalldown" => $taken);
    $jsonString = json_encode($json);

    header('Content-Type: application/json');
    echo $jsonString;
} else {
   header('HTTP/1.1 400 Bad Request'); 
}


