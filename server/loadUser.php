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
    echo "<li>" . $array['alert_date'] . " ".$array['alert_time'] . " " . $array['label'] . "</li>\n";
}
?>
</ol></fieldset></div>

<!-- Traitement en cours -->
<div class="formulaire" id="traitement"><fieldset>
	<legend>Traitement en cours</legend>
</fieldset></div>
