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
	<title>Maintien à domicile - Connexion</title>
	<link rel="stylesheet/less" type="text/css" href="css/show.less" />
  	<script type="text/javascript" src="js/less.js"></script>
  	<script type="text/javascript" src="js/jquery-1.11.2.min.js"></script>
  	<script type="text/javascript">
	
	function getXmlHttp() {
		var x = false;
		try {
			x = new XMLHttpRequest();
		} catch(e) {
			try {
				x = new ActiveXObject("Microsoft.XMLHTTP");
			} catch(ex) {
				try {
					req = new ActiveXObject("Msxml2.XMLHTTP");
				}
				catch(e1) {
					x = false;
				}
			}
		}
		return x;
	}
	
  	function disconnect() {
		var xmlhttp = getXmlHttp();
		var xmlhttp = new XMLHttpRequest();
		xmlhttp.open('GET','./disconnect.php', true);
		xmlhttp.onreadystatechange=function(){
		   if (xmlhttp.readyState == 4){
			  if(xmlhttp.status == 200){
				 alert("Vous êtes maintenant déconnecté: " + xmlhttp.responseText);
				 location.href = "index.html";
			 }
		   }
		};
		xmlhttp.send(null);
	}
	
	function loadUser() {
		var myselect = document.getElementById("usersList");
		var id = myselect.options[myselect.selectedIndex].value;
		var xmlhttp = getXmlHttp();
		var xmlhttp = new XMLHttpRequest();
		xmlhttp.open('GET','./loadUser.php?id=' + id, true);
		xmlhttp.onreadystatechange=function(){
		   if (xmlhttp.readyState == 4){
			  if(xmlhttp.status == 200){
				 document.getElementById("details").innerHTML = xmlhttp.responseText;
			 }
		   }
		};
		xmlhttp.send(null);
	}
	
	function addUser() {
		var myselect = document.getElementById("usersList");
		var id = myselect.options[myselect.selectedIndex].value;
		location.href = 'add.php?id=' + id;
	}
  	</script>
</head>
<body>
	<header class="w100">
		<div class="contenu">
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
			
			<button type="type"button" id='search' class="btn btn-primary btn-lg btn-block">

<input type="button" value="ajouter un patient" onclick="ajoutebtn()">
</p>
		</fieldset></div>
	</div>
	<div class="w100" id="details">
		
	</div>

<?php

?>
	<footer class="w100">
		<div class="contenu"></div>
	</footer>
</body>
</html>
