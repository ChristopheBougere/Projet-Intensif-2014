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
if (array_key_exists("drug_id", $tab) && !empty($tab["drug_id"])) {
    $drugId = $tab["drug_id"];
} else {
    header('HTTP/1.1 400 Bad Request');
    return;
}
if (array_key_exists("start_date", $tab) && !empty($tab["start_date"])) {
    $startDate = $tab["start_date"];
} else {
    header('HTTP/1.1 400 Bad Request');
    return;
}
if (array_key_exists("end_date", $tab) && !empty($tab["end_date"])) {
    $endDate = $tab["end_date"];
} else {
    header('HTTP/1.1 400 Bad Request');
    return;
}
if (array_key_exists("hour", $tab) && !empty($tab["hour"])) {
    $hour = $tab["hour"].":00";
} else {
    header('HTTP/1.1 400 Bad Request');
    return;
}
if (array_key_exists("quantity", $tab) && !empty($tab["quantity"])) {
    $quantity = $tab["quantity"];
} else {
    header('HTTP/1.1 400 Bad Request');
    return;
}
insertPosology($userId,$drugId,$startDate,$endDate,$hour, $quantity,true);

function insertPosology($userId,$drugId,$startDate,$endDate,$hour, $quantity,$recursive) {
    /* On check si l'entrée existe dans posology */
    $db = getDatabaseConnection();

    $select = "select id "
            . "from posologies "
            . "where user_id = ? "
            . "and drug_id = ? "
            . "and start_date = ? "
            . "and end_date = ? ";

    $rs = $db->Execute($select,array($userId,$drugId,$startDate,$endDate));
    if (!$rs) {
        if (true) {
            echo $db->ErrorMsg();
        } else {
            header('HTTP/1.1 400 Bad Request');
            return;
        }
    }

    if ($array = $rs->FetchRow()) { // déjà en base de donnés
        
        $posologyId = $array["id"];
        addPosologyHour($posologyId, $hour, $quantity);
        
    } else { // à ajouter
        if ($recursive) {
            // insert
            $insert = "insert into posologies(user_id,drug_id,start_date,end_date)"
                    . "values(?,?,?,?);";
            $ri = $db->Execute($insert,array($userId,$drugId,$startDate,$endDate));
            if (!$ri) {
                if (true) {
                    echo $db->ErrorMsg();
                } else {
                    header('HTTP/1.1 400 Bad Request');
                    return;
                }
            }
            insertPosology($userId,$drugId,$startDate,$endDate,$hour, $quantity,false);
        }
    }
}

function addPosologyHour($posologyId, $posologyTime, $quantity) {
    $insert = "insert into hours(posology_id,posology_time,quantity) "
            . "values (?,?,?);";
    $db = getDatabaseConnection();
    $rs = $db->Execute($insert,array($posologyId, $posologyTime, $quantity));
    if (!$rs) {
        if (true) {
            echo $db->ErrorMsg();
        } else {
            header('HTTP/1.1 400 Bad Request');
            return;
        }
    }   
}

?>