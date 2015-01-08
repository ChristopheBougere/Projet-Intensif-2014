<?php

require_once("serverApi/config/global_config.php");
require_once($GLOBALS['ROOT'] . "serverApi/tools/utils.php");
require_once($GLOBALS['ROOT'] . 'serverApi/lib/adodb5/adodb.inc.php');
session_start();
if(!isset($_SESSION['id'])) {
	header('HTTP/1.1 400 Bad Request');
    return;
}
if(!isset($_GET['id'])) {
   header('HTTP/1.1 400 Bad Request');
   return;
 }
$db = getDatabaseConnection();
?>
<div>
<div class="formulaire" id="addPopUp"><fieldset>		
   <label for="drug"><a>Traitement</a></label>
   <select name="drug" id="drug">
   <?php
   $select = "select id, name from drugs order by name";
$rSelect = $db->Execute($select,array());
if(!$rSelect) {
   if ($DEBUG) { echo $db->ErrorMsg(); return; }
   else { header('HTTP/1.1 400 Bad Request'); return; }
 }

while($array = $rSelect->FetchRow()) {
   echo "<option value='" . $array['id'] . "'>".$array['name'] ."</option>\n";
 }
?>
</select>
		
<p><a>Date de début</a> <input type="text" id="start_date"></p>
   <p><a>Date de fin</a> <input type="text" id="end_date"></p>
   <hr/>
   <div id="quantities">
			
   </div>
   <button onclick="addQuantity()" id="plus">+</button>
   </fieldset>
   </div>
   </div>

