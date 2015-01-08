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
<html><head>
<link rel="stylesheet" href="js/jquery/jquery-ui.min.css">
<script src="js/jquery/external/jquery/jquery.js"></script>
<script src="js/jquery/jquery-ui.min.js"></script>
<script type="text/javascript">
<?php echo "var id = " . $_SESSION['id'] . ";\n"; ?>
$(function() {
	$( "#start_date" ).datepicker({ dateFormat: 'yy-mm-dd' });
	$( "#end_date" ).datepicker({ dateFormat: 'yy-mm-dd' });
	$( "#drug" ).selectmenu();
	addQuantity();
});

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

function addQuantity() {
	$("#quantities").append("<p class='quantity_element'>Quantité <select name=\"quantity\" class=\"quantity\"><?php
			$q = 0;
			while ($q<11) {
				echo "<option>$q</option>";
				$q = $q+1;
			}
			?></select> à <select name=\"hour\" class=\"hour\"><?php
			$hour = 0;
			while ($hour<24) {
				echo "<option>$hour</option>";
				$hour = $hour+1;
			}
			?></select>:<select name=\"minute\" class=\"minute\"><?php
			$minute = 0;
			while ($minute<60) {
				echo "<option>$minute</option>";
				$minute = $minute+5;
			}
			?></select></p>");
			$( ".quantity" ).selectmenu() .selectmenu( "menuWidget" ).addClass( "overflow" );
			$( ".hour" ).selectmenu() .selectmenu( "menuWidget" ).addClass( "overflow" );
			$( ".minute" ).selectmenu() .selectmenu( "menuWidget" ).addClass( "overflow" );
}

function send() {
	var med = document.getElementById("drug");
	var med_id = med.options[med.selectedIndex].value;
	var date_debut = $("#start_date").val();
	var date_fin = $("#end_date").val();
	
	$(".quantity_element").each(function() {
		
		var quantity = $(this).find(".quantity").first().val();
		var hour = "" + $(this).find(".hour").first().val() + ":" + $(this).find(".minute").first().val();
		
		var xmlhttp = getXmlHttp();
		var xmlhttp = new XMLHttpRequest();
		var url = 'serverApi/actions/insertPosology.php?';
		url += 'user_id=' + id;
		url += '&drug_id=' + med_id;
		url += '&start_date=' + date_debut;
		url += '&end_date=' + date_fin;
		url += '&hour=' + hour;
		url += '&quantity=' + quantity;
		xmlhttp.open('GET', url, true);
		xmlhttp.onreadystatechange=function(){
		   if (xmlhttp.readyState == 4){
			  if(xmlhttp.status == 200){
				 location.href = "show.php";
			 }
		   }
		};
		xmlhttp.send(null);
	});
}
</script>
 <style>
label {
display: block;
margin: 30px 0 0 0;
}
select {
width: 300px;
height: 10px;
}
.overflow {
height: 100px;
}
</style>
</head><body>
<div class="floating-menu">
	<div class="formulaire"><fieldset>
		<legend>Ajouter un traitement</legend>
		
		<label for="drug">Traitement</label>
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
		
		<p>Date de début <input type="text" id="start_date"></p>
		<p>Date de fin <input type="text" id="end_date"></p>
		<div id="quantities">
			
		</div>
		<button onclick="addQuantity()">+</button>
		<button onclick="send()">Sauvegarder le traitement</button>
		</fieldset>
	</div>
</div>
</body>
</html>

