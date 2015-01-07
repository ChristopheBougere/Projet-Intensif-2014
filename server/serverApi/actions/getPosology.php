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

$request = "select d.name, p.quantity, h.posology_time "
        . "from posologies as p, hours as h, drugs as d "
        . "where user_id = ? "
        . "and p.drug_id = d.id "
        . "and h.posology_id = p.id "
        . "and start_date <= current_date "
        . "and end_date >= current_date";

$rs = $db->Execute($request,array($userId));
if (!$rs) {
    if ($DEBUG) {
        echo $db->ErrorMsg();
    } else {
        header('HTTP/1.1 400 Bad Request');
        return;
    }
}

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
?>