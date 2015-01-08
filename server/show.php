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

<!DOCTYPE html>
<html>
<head>
	<meta charset="utf-8" /> 
	<meta name="viewport" content="initial-scale=1.0, user-scalable=no" />
	<title>Care For You</title>
	<link rel="stylesheet/less" type="text/css" href="css/show.less" />
   <link rel="stylesheet/less" type="text/css" href="css/main.css" />
  	<script type="text/javascript" src="js/less.js"></script>
  	<script type="text/javascript" src="js/main.js"></script>
   <link rel="stylesheet" href="js/jquery/jquery-ui.min.css">
<script src="js/jquery/external/jquery/jquery.js"></script>
<script src="js/jquery/jquery-ui.min.js"></script>
   <script type="text/javascript"> 
   function addQuantity() {
	$("#quantities").append("<p class='quantity_element'><a>Quantité </a><select name=\"quantity\" class=\"quantity\" ><?php
			$q = 0;
			while ($q<11) {
				echo "<option>$q</option>";
				$q = $q+1;
			}
			?></select><br/><a>Heure</a><br/><select name=\"hour\" class=\"hour\"><?php
			$hour = 0;
			while ($hour<24) {
				echo "<option>$hour</option>";
				$hour = $hour+1;
			}
			?></select><select name=\"minute\" class=\"minute\"><?php
			$minute = 0;
			while ($minute<60) {
				echo "<option>$minute</option>";
				$minute = $minute+5;
			}
			?></select></p><hr/>");
			$( ".quantity" ).selectmenu() .selectmenu( "menuWidget" ).addClass( "overflow" );
			$( ".hour" ).selectmenu() .selectmenu( "menuWidget" ).addClass( "overflow" );
			$( ".minute" ).selectmenu() .selectmenu( "menuWidget" ).addClass( "overflow" );
}
   </script>
</head>
<body>
	<header class="w100">
		<div class="contenu">
   <img src="img/framtouch.png" height="60px" id="frmtch"/>
   <img src="img/cfy.png" height="60px" id="cfy"/>
			 <h1>Bonjour Dr. <?php echo $_SESSION['name']; ?> - <a href="#" onclick="disconnect();">Se déconnecter</a></h1>
		</div>
	</header>
	<div class="floating-menu" id="liste">
		<!-- Liste des patients -->
		<div class="formulaire" id="liste"><fieldset>
			<legend>Vos patients</legend>
			<select size="20" onchange="loadUser();" id="usersList">
<?php
$select = "select id, name, first_name from users where doctor_id=? order by name, first_name";
$rSelect = $db->Execute($select,array($_SESSION['id']));
if(!$rSelect) {
	if ($DEBUG) { echo $db->ErrorMsg(); return; }
	else { header('HTTP/1.1 400 Bad Request'); return; }
}

while($array = $rSelect->FetchRow()) {
    echo "<option value='" . $array['id'] . "'>".$array['name'] . " " . $array['first_name'] . "</option>\n";
}
?>
			</select>
			

			<input type="button" value="ajouter un patient"/>
</p>
		</fieldset></div>
	</div>
	<div id="details">
		
	</div>

<?php

?>

<div class="w100" id="dialog"></div>

</div>

	<footer class="w100">
		<div class="contenu"></div>
	</footer>
</body>
</html>
