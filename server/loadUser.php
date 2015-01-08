<?php
require_once("serverApi/config/global_config.php");
require_once($GLOBALS['ROOT'] . "serverApi/tools/utils.php");
require_once($GLOBALS['ROOT'] . 'serverApi/lib/adodb5/adodb.inc.php');
session_start();
if(!isset($_SESSION['id'])) {
	header('HTTP/1.1 400 Bad Request');
    return;
}
$db = getDatabaseConnection();
?>

<!-- Alertes -->
<div class="formulaire" id="alertes"><fieldset>
	<legend>Alertes</legend><ol>
		
	
<?php
$select = "select alert_date, alert_time, label from alerts join alert_types on alerts.alert_type_id=alert_types.id where user_id=?";
$rSelect = $db->Execute($select,array($_GET['id']));
if(!$rSelect) {
	if ($DEBUG) { echo $db->ErrorMsg(); return; }
	else { header('HTTP/1.1 400 Bad Request'); return; }
}

while($array = $rSelect->FetchRow()) {
    echo "<li>" . parseDate($array['alert_date']) . " à ".parseTime($array['alert_time']) . " " . $array['label'] . "</li>\n";
}
?>
</ol></fieldset></div>

<!-- Traitement en cours -->
<div class="formulaire" id="traitement"><fieldset>
	<legend>Traitement en cours</legend>
<?php

$select = "select d.name, h.posology_time, h.quantity, p.start_date, p.end_date "
			." from posologies as p, hours as h, drugs as d "
			." where p.id = h.posology_id "
			." and d.id = p.drug_id "
			." and p.user_id = ? "
			." and p.start_date <= current_date "
			." and p.end_date >= current_date "
			." order by p.end_date desc, p.start_date desc, d.name ;";
$rSelect = $db->Execute($select,array($_GET['id']));
if(!$rSelect) {
	if ($DEBUG) { echo $db->ErrorMsg(); return; }
	else { header('HTTP/1.1 400 Bad Request'); return; }
}

$oldDrug = "";
$oldSD = "";
$oldED = "";
while($array = $rSelect->FetchRow()) {

    $drug = $array["name"];
	if ( empty($drug)) {
		break;
	}
    $time = parseTime($array["posology_time"]);
    $quantity = $array["quantity"];
    $startDate = parseDate($array["start_date"]);
    $endDate = parseDate($array["end_date"]);
    if (strcmp($oldDrug,$drug) != 0 || strcmp($oldED,$endDate)  != 0 || strcmp($oldST,$startDate) != 0 ) {
		if (strcmp($oldDrug,"") != 0) {
			echo "</ul></li>";
		}
		$oldDrug = $drug;
		$oldED = $endDate;
		$oldST = $startDate;
		
		echo "<li> <p>$drug du $startDate au $endDate </p><ul>";
	}
	echo "<li>$quantity à $time</li>";
}


/*
foreach ($drugs as $drug=>$values) {
	
	echo "<li>".$drug." du ".;
}

echo "<li>" . $array['alert_date'] . " ".$array['alert_time'] . " " . $array['label'] . "</li>\n";
*/
?>
<p class="tp">
<input type="button" value="ajout traitement" onclick="addUser();">
</p>

</fieldset></div>
