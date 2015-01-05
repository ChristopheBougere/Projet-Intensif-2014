<?php

/* 
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

function getDatabaseConnection() {
    require_once('../config/database_config.php');
    require_once('../lib/adodb5/adodb.inc.php');
    $DB = NewADOConnection('postgres');
    $DB->Connect($DATABASE_HOST, $DATABASE_USER, $DATABASE_PASSWORD, $DATABASE_NAME);
    return $DB;
}

?>