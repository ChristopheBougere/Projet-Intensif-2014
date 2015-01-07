<?php

/* 
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

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

if (array_key_exists("push_id", $tab) && !empty($tab["push_id"])) {
    $pushId = $tab["push_id"];
} else {
    header('HTTP/1.1 400 Bad Request');
    return;
}

$db = getDatabaseConnection();

$insert = "insert into supervisors(push_id) values (?)";

$rs = $db->Execute($insert,array($pushId));
if (!$rs) {
    if ($DEBUG) {
        echo $db->ErrorMsg();
    } else {
        header('HTTP/1.1 400 Bad Request');
        return;
    }
}
$db->Close();

$update = "update users "
        . "set supervisor_id = ("
            . "select id from supervisors where push_id = ? limit 1) "
        . "where id = ?";
$db2 = getDatabaseConnection();

$rs2 = $db2->Execute($update,array($pushId,$userId));
if (!$rs2) {
    if ($DEBUG) {
        echo $db->ErrorMsg();
        return;
    } else {
        header('HTTP/1.1 400 Bad Request');
        return;
    }
}

