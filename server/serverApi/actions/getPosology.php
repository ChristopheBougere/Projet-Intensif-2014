<?php

require_once("../config/global_config.php");
require_once($GLOBALS['ROOT']."serverApi/tools/utils.php");

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

$request = "select d.name, h.quantity, h.posology_time "
        . "from posologies as p, hours as h, drugs as d "
        . "where user_id = ? "
        . "and p.drug_id = d.id "
        . "and h.posology_id = p.id "
        . "and start_date <= current_date "
        . "and end_date >= current_date "
        . "order by h.posology_time";

$rs = $db->Execute($request,array($userId));
if (!$rs) {
    if ($DEBUG) {
        echo $db->ErrorMsg();
    } else {
        header('HTTP/1.1 400 Bad Request');
        return;
    }
}


if (array_key_exists("pi", $tab) && !empty($tab["pi"]) && strcasecmp($tab["pi"], "true")==0) {
    $times = array();
    while ($array = $rs->FetchRow()) {
        $drug = $array["name"];
        $quantity = intval($array["quantity"]);
        $time = $array["posology_time"];
        $timeTab = split(":",$time);
        $hour = $timeTab[0];
        $minute = $timeTab[1];
        if (!array_key_exists($time, $times)) {
            $times[$time]=array();
            $times[$time]["hour"] = $hour;
            $times[$time]["minute"] = $minute;
            $times[$time]["drugs"] = array();
        }
        $times[$time]["drugs"][]=array("name"=>$drug,"quantity"=>$quantity);
    }
    $json=array();
    foreach($times as $time) {
        $json[]=$time;
    }
    $jsonString = json_encode($json);

    header('Content-Type: application/json');
    echo $jsonString; 
} else {
    $drugs = array();
    while ($array = $rs->FetchRow()) {
        $drug = $array["name"];
        $quantity = intval($array["quantity"]);
        $hour = $array["posology_time"];
        if (!array_key_exists($drug,$drugs)) {
            $drugs[$drug] = array();
        }
        $drugs[$drug][] = array("quantity"=>$quantity,"hour"=>$hour);
    }
    $json=array();
    foreach ($drugs as $drug=>$posology) {
        $json[] = array("name"=>$drug,"posology"=>$posology);
    }
    $jsonString = json_encode($json);

    header('Content-Type: application/json');
    echo $jsonString; 
}



?>