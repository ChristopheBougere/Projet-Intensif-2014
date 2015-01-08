<?php
require_once("serverApi/config/global_config.php");
require_once($GLOBALS['ROOT'] . "serverApi/tools/utils.php");
require_once($GLOBALS['ROOT'] . 'serverApi/lib/adodb5/adodb.inc.php');

//si les champs ont été remplis
if (array_key_exists("pseudo", $_POST) && !empty($_POST["pseudo"])) {
    $name = $_POST['pseudo'];
} else {
    header('HTTP/1.1 400 Bad Request');
    return;
}

if (array_key_exists("mot_de_passe", $_POST) && !empty($_POST["mot_de_passe"])) {
    $pass = md5($_POST['mot_de_passe']);
} else {
    header('HTTP/1.1 400 Bad Request');
    return;
}

$db = getDatabaseConnection();

$select = "select name, password from doctors";
$rSelect = $db->Execute($select,array());

if (!$rSelect) {
    if ($DEBUG) {
        echo $db->ErrorMsg();
        return;
    } else {
        header('HTTP/1.1 400 Bad Request');
        return;
    }
}

$allowed = false;
while($array = $rSelect->FetchRow()) {
    if( $name == $array["name"] and $pass == $array["password"]) {
		$allowed = true;
		break;
	}
}
if($allowed) {
	//initialisation de la session
	session_start();
	$_SESSION['name'] = $name;
	//On récupère l'id du docteur
	$select = "select id from doctors where name=?";
	$rSelect = $db->Execute($select,array($name));
	if (!$rSelect) {
		if ($DEBUG) {
			echo $db->ErrorMsg();
		} else {
			header('HTTP/1.1 400 Bad Request');
			return;
		}
	}
	if ($array = $rSelect->FetchRow()) {
		$_SESSION['id'] = $array['id'];
	} else {
		header('HTTP/1.1 400 Bad Request');
		return; 
	}
	
	header('Location: show.php');
} else {
	header('Location: index.html');
}
?>
