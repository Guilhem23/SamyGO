<?php
global $Engine, $respbody, $respxml, $respcode, $WhoAmI, $QParts, $QURLParts, $gUserName, $gUserData;
$Engine = "SELP";

if(!stristr($_SERVER['HTTP_HOST'], "pavv.co.kr") && !stristr($_SERVER['HTTP_HOST'], "samsung") && !stristr($_SERVER['HTTP_HOST'], "internetat.tv") && !stristr($_SERVER['HTTP_HOST'], "cloud"))
	die("made a bobo...");

require_once('SimpleMidleManHelpers.inc');

function StringToHeader(){} # mabe...

function DoProxy()
{
	global $xml, $respbody, $respxml, $WhoAmI, $gUserName, $gUserData;

	CError::LogStr("- START for ". GeoInfoOTN() ." - $WhoAmI -" . $_SERVER["REQUEST_URI"]);

	$allheaders = getallheaders();
	$Uplink = nslookup($allheaders['Host']); # add check here?
	$UplinkURI = ((isset($_SERVER['HTTPS'])) ? "https://" : "http://") . $Uplink .$_SERVER["REQUEST_URI"];
	$target_method = $_SERVER["REQUEST_METHOD"];
	$target_headers = array();
	foreach ($allheaders as $header => $value)
	{
		if ($header!="Connection" && $header != "Authorization") # b-series fix for requests (drop Authorization)
			array_push($target_headers,$header.": ".$value);
	}
	CError::LogStr("- Clients header -" . print_r($allheaders, 1));
	CError::LogStr("- Routed headers -" . print_r($target_headers, 1));
	$gUserData = file_get_contents("php://input"); 

	$tuCurl = curl_init();
	curl_setopt($tuCurl, CURLOPT_URL, $UplinkURI);
	curl_setopt($tuCurl, CURLOPT_HEADER, TRUE);
	curl_setopt($tuCurl, CURLOPT_POST, FALSE);
	curl_setopt($tuCurl, CURLOPT_NOBODY, FALSE);
	if ($target_method != "POST" && $target_method != "GET"){
		curl_setopt($tuCurl, CURLOPT_CUSTOMREQUEST, $target_method);
	}
	if ($target_method != "GET"){
		CError::LogStr("- $gUserName Like to $target_method -\n$gUserData\n");
		curl_setopt($tuCurl, CURLOPT_POSTFIELDS, $gUserData);
	}
	curl_setopt($tuCurl, CURLOPT_RETURNTRANSFER, TRUE);
	curl_setopt($tuCurl, CURLOPT_HTTPHEADER, $target_headers);
	curl_setopt($tuCurl, CURLOPT_COOKIEJAR, $_SERVER['DOCUMENT_ROOT']."/logs/".$_SERVER['REMOTE_ADDR'].".jar");
	curl_setopt($tuCurl, CURLOPT_VERBOSE, 	     TRUE);
	#	if(isset($ClientSend['Cookie']))
	#	        curl_setopt($tuCurl, CURLOPT_COOKIE, $ClientSend['Cookie']);
	#	curl_setopt($tuCurl, CURLOPT_BINARYTRANSFER, TRUE); 
	curl_setopt($tuCurl, CURLOPT_ENCODING, "" );
	curl_setopt($tuCurl, CURLOPT_CONNECTTIMEOUT, 4);
	curl_setopt($tuCurl, CURLOPT_TIMEOUT,        9);
	curl_setopt($tuCurl, CURLOPT_SSLVERSION, 3);
	curl_setopt($tuCurl, CURLOPT_SSL_VERIFYPEER, FALSE);
	curl_setopt($tuCurl, CURLOPT_SSL_VERIFYHOST, FALSE);
	curl_setopt($tuCurl, CURLOPT_CAINFO, $_SERVER['DOCUMENT_ROOT']."/CA/ca.pem");
	$response = curl_exec($tuCurl);
	$RespCode = curl_errno($tuCurl);

	if(!$RespCode)
	{
		$respinfo = curl_getinfo($tuCurl);
		curl_close($tuCurl); 

		$allheaders = substr($response, 0, $respinfo['header_size']);
		$respbody = substr($response, $respinfo['header_size']); 

		switch($respinfo['http_code']){
		case 200:
			break;
		case 302:
			break;
		default:
			CError::LogStr("Notice: Uplink reported an error (" . $respinfo['http_code'] . ")");
		}
		CError::LogStr("Es wurden " . $respinfo['total_time'] . " Sekunden benötigt für einen Request an " . $respinfo['url']);
	} else {

		CError::LogStr("Request failed.");
		CError::LogStr("Dump Vars: for uplink $Uplink " . print_r($UplinkURI, 1) . print_r($GLOBALS, 1));
		#return;
		#	if(function_exists('http_response_code'))
		#		http_response_code(200);
		#	else
		#		header("HTTP/1.0 200 Ok");
		#	$respxml = $xml;
		#        header("Status: 504 Gateway timeout", "true", 504);
		#header("HTTP/1.1 504 Gateway timeout", "true", 504);
		#header("Location: /errors/errorpage.php?force=504");
		#exit();
		$xml->addAttribute("offline", true);
		$respxml = $xml;
		header("Content-Type: text/xml");
		return curl_close($tuCurl);
	}

	$target_headers = explode("\r\n", trim($allheaders));
	foreach($target_headers as $val){
		$tmp = preg_split('/:/', trim($val), 2);
		$Headers[$tmp[0]] = $tmp[1];
	}
	CError::LogStr("- Response Header - " . print_r($Headers, 1));

	if(stristr($Headers['Content-Type'], '/xml')){
		$respxml = new mySXE($respbody);
	} elseif(strstr($respbody, "Salted_")){
		CError::LogStr("- OTN crypted file: $UplinkURI -");
		SendPage($respbody);
	} else {
		CError::LogStr("- Response error: not a xml !!! gzip or html content? -" . bin2hex($respbody)); # gzinflate and co fails...
		SendPage($respbody);
	}
}
DoProxy();
header("Content-Type: " . $Headers['Content-Type']);
#header("Transfer-Encoding: chunked", true);
#if(isset($Headers['Set-Cookie'])){
#		header("Set-Cookie: ". $Headers['Set-Cookie'], true);
#}
CError::LogStr("- response code: $respcode - " . substr($respxml->pPrint(), 0, 512));
if($_SERVER['REQUEST_METHOD'] == 'GET'){
	if(!file_exists($_SERVER['DOCUMENT_ROOT'] . "/OfflinePages/" . $Engine . $_SERVER['REQUEST_URI'])){
		# write file
		$LocalDir = dirname($_SERVER['DOCUMENT_ROOT'] . "/OfflinePages/" . $Engine . $_SERVER['REQUEST_URI']);
		exec("mkdir -p ".$LocalDir);
		CError::LogStr("- Save File: - " . $_SERVER['DOCUMENT_ROOT'] . "/OfflinePages/" . $Engine . $_SERVER['REQUEST_URI']);
		$respxml->saveXML($_SERVER['DOCUMENT_ROOT'] . "/OfflinePages/" . $Engine . $_SERVER['REQUEST_URI']);
	} else {
		# make backup if changed and save new.
	}
}
?>
