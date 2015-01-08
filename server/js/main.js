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
	var xmlhttp = getXmlHttp();
    var xmlhttp = new XMLHttpRequest();
	xmlhttp.open('GET','./add.php?id=' + id, true);
    xmlhttp.onreadystatechange=function(){
	if (xmlhttp.readyState == 4){
	    if(xmlhttp.status == 200){
		document.getElementById("dialog").innerHTML = xmlhttp.responseText;
		showDialog();
	    }
	}
    };
    xmlhttp.send(null);
}

function showDialog() {
	

	$( "#dialog" ).dialog({
	      modal: true,
	      width: 450,
	      height: 800,
		title: "Ajouter un traitement",
	      buttons: {
		Annuler: function() {
		  $( this ).dialog( "close" );
		},
		Sauvegarder: function() {
		  send();
		  $( this ).dialog( "close" );
		  loadUser();
		}	
	      }
    	});
	$( "#start_date" ).datepicker({ dateFormat: 'yy-mm-dd' });
	$( "#end_date" ).datepicker({ dateFormat: 'yy-mm-dd' });
	$( "#drug" ).selectmenu();
	addQuantity();
}


function send() {
    var med = document.getElementById("drug");
    var med_id = med.options[med.selectedIndex].value;
    var date_debut = $("#start_date").val();
    var date_fin = $("#end_date").val();
var myselect = document.getElementById("usersList");
    var id = myselect.options[myselect.selectedIndex].value;
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
