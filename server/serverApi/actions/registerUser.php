<?php

require_once '../config/global_config.php';
require_once $GLOBALS['ROOT'].'serverApi/tools/utils.php';
/* 
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

$firstNames = array("Jean","Alfred", "Benoit", "Conan", "Gustave", "Yvette", "Germaine", "Rosemary", "Bernadette", "Aurélie", "Marguerite", "Juliette");
$names = array("Revault","Rivereau","Tyson","Tataume","Neymar","Potter","Sackey","Martin","Dupont","Marie","Asselins");

$firstName = $firstNames[rand(0,count($firstNames))];
$name = $names[rand(0,count($names))];
$doctorId = 1;

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