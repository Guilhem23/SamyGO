<?php
global $Engine, $QParts, $QURLParts, $xml, $respxml, $WhoAmI, $gInHeaders, $gInData, $gOutHeaders, $gOutData, $gUserName;
$Engine = "SELP";

if(!stristr($_SERVER['HTTP_HOST'], "pavv.co.kr") && !stristr($_SERVER['HTTP_HOST'], "samsung") && !stristr($_SERVER['HTTP_HOST'], "internetat.tv") && !stristr($_SERVER['HTTP_HOST'], "cloud"))
	die("made a bobo...");

# TODO: browsing mode on uplink, check user agent (curl/wget) and use stored authinfo or reauth...

require_once('SimpleMidleManHelpers.inc');

function StringToHeader(){} # mabe...

function DoProxy($internalcall = false)
{
	global $Engine, $gModelId, $xml, $respxml, $WhoAmI, $gInHeaders, $gInData, $gOutHeaders, $gOutData, $gUserName, $gUpSiteHUB;

	CError::LogStr("- START for ". GeoInfoOTN() ." - $WhoAmI -" . $_SERVER["REQUEST_URI"]);

	if(!$internalcall)
		$gInHeaders = getallheaders();
	CError::LogStr("- Clients header -" . print_r($gInHeaders, 1));
	$gUpSiteHUB = nslookup($gInHeaders['Host']); # add check here?
	$gUpSiteHUBURI = ((isset($_SERVER['HTTPS'])) ? "https://" : "http://") . $gUpSiteHUB .$_SERVER["REQUEST_URI"];
	$ReqMethod = $_SERVER["REQUEST_METHOD"];
	$CurlOutHeaders = array();
	# TODO: for browsermaode, check user agent and load saved authinfo + add it to headers
	# User-Agent... load from $saveHeader = $oDir . DIRECTORY_SEPARATOR . $gModelId . "_LastHeader.h";
#	if(((empty($_SERVER['HTTP_DUID']) && empty($_SERVER['HTTP_TOKEN'])) && $WhoAmI != ("gallery" || "list")) || $internalcall){
#			$gModelId = "12_X10PLUS_2D";
#			$gModelId = "12_ECHOP";
#	#		$gModelId = "LEXXB650_EU";
#				CError::LogStr("- ######## ASUME BROWSERMODE (Browse as $gModelId) ######## -");
#			$gInHeaders = unserialize(file_get_contents($_SERVER['DOCUMENT_ROOT'] . "/OfflinePages/$Engine/$gUpSiteHUB-$gModelId-Auth.h"));
#			if(!is_array($gInHeaders)){
#				CError::LogStr("- ERROR: smthg. went wrong in SELP-land...");
#			}
#			$gUserName = $gInHeaders['ModelId'] . "_" . $gInHeaders['MACAddr'];
#			unset($gInHeaders['MACAddr']);
#			unset($gInHeaders['Firmcode']);
#			unset($gInHeaders['ModelId']);
#	#		unset($gInHeaders['Cookie']);
#	}
	foreach ($gInHeaders as $header => $value)
	{
#		if (!strstr($header, "-") || $header != "Authorization" || stristr($header, "duid")) # b-series fix for requests (drop Authorization)
		if (!strstr($header, "-") || $header != "Authorization") # b-series fix for requests (drop Authorization)
			array_push($CurlOutHeaders, $header . ": " . $value);
	}
	CError::LogStr("- Routed headers -" . print_r($CurlOutHeaders, 1));
	if(!$internalcall)
		$gInData = file_get_contents("php://input"); 

	$tuCurl = curl_init();
	curl_setopt($tuCurl, CURLOPT_URL, $gUpSiteHUBURI);
	curl_setopt($tuCurl, CURLOPT_HEADER, true);
	curl_setopt($tuCurl, CURLOPT_POST, false);
	curl_setopt($tuCurl, CURLOPT_NOBODY, false);
	if ($ReqMethod != "POST" && $ReqMethod != "GET"){
		curl_setopt($tuCurl, CURLOPT_CUSTOMREQUEST, $ReqMethod);
	}
	if ($ReqMethod != "GET" && $ReqMethod != "HEAD"){
		CError::LogStr("- $gUserName Like to $ReqMethod -\n$gInData\n");
		curl_setopt($tuCurl, CURLOPT_POSTFIELDS, $gInData);
	}
	curl_setopt($tuCurl, CURLOPT_RETURNTRANSFER, true);
	curl_setopt($tuCurl, CURLOPT_HTTPHEADER, $CurlOutHeaders);
	curl_setopt($tuCurl, CURLOPT_COOKIEJAR, $_SERVER['DOCUMENT_ROOT']."/logs/".$_SERVER['REMOTE_ADDR'].".jar");
	curl_setopt($tuCurl, CURLOPT_VERBOSE, 	     true);
#		if(isset($ClientSend['Cookie']))
#		        curl_setopt($tuCurl, CURLOPT_COOKIE, $ClientSend['Cookie']);
	#	curl_setopt($tuCurl, CURLOPT_BINARYTRANSFER, true); 
	curl_setopt($tuCurl, CURLOPT_ENCODING, "" );
	curl_setopt($tuCurl, CURLOPT_CONNECTTIMEOUT, 5);
	curl_setopt($tuCurl, CURLOPT_TIMEOUT,        5);
#	curl_setopt($tuCurl, CURLOPT_SSLVERSION, 3);
	curl_setopt($tuCurl, CURLOPT_SSL_VERIFYPEER, false);
	curl_setopt($tuCurl, CURLOPT_SSL_VERIFYHOST, false);
	curl_setopt($tuCurl, CURLOPT_CAINFO, $_SERVER['DOCUMENT_ROOT']."/CA/ca.pem");

	$response = curl_exec($tuCurl);
	$RespCode = curl_errno($tuCurl);
	$respinfo = curl_getinfo($tuCurl);
	curl_close($tuCurl); 
	$CurlInHeaders = substr($response, 0, $respinfo['header_size']);
	$gOutData = substr($response, $respinfo['header_size']); 
	$tmp = explode("\r\n", trim($CurlInHeaders));
	foreach($tmp as $val){
		$tmp = preg_split('/:/', trim($val), 2);
		$gOutHeaders[$tmp[0]] = $tmp[1];
	}
	CError::LogStr("- Response Header - " . print_r($gOutHeaders, 1));

	if(!$RespCode)
	{
		switch($respinfo['http_code']){
		case 200:
			break;
		case 302:
			break;
		default:
			CError::LogStr("Notice: Uplink reported an error (" . $respinfo['http_code'] . ")");
		$xml->addAttribute("offline", true);
		$xml['hubsite'] = $gUpSiteHUB;
		$xml->addChild("list");
		$gOutData = $xml->asXML();
		break;
		}
		CError::LogStr("Code " . print_r($respinfo, 1));
		CError::LogStr("HTTP ($ReqMethod) $RespCode " . $respinfo['http_code'] . " Es wurden " . $respinfo['total_time'] . " Sekunden benötigt für einen Request an " . $respinfo['url']);
	} else {
		CError::LogStr("Request failed. HTTP: (" . $respinfo['http_code'] . ")");
#		CError::LogStr("Dump Vars: for uplink $gUpSiteHUB " . print_r($gUpSiteHUBURI, 1) . print_r($GLOBALS, 1));
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
		$xml['hubsite'] = $gUpSiteHUB;
		$xml->addChild("list");
		$gOutData = $xml->asXML();
		header("HTTP/1.0 200 Ok");
		$gOutHeaders = array();
		$gOutHeaders['Content-Type'] = "text/xml";
		$gOutHeaders['cache-control'] = "no-cache";
	}


	if(strstr($gOutData, "xml")){
		$respxml = new mySXE($gOutData);
	} elseif(strstr($gOutData, "Salted_")){
		CError::LogStr("- OTN crypted file: $gUpSiteHUBURI -");
		$respxml = $xml;
		$respxml->addCDChild("OtnData", $gOutData);
#		SendPage($gOutData);
	} else {
		CError::LogStr("- Response error: not a xml !!! gzip or html content? -" . bin2hex($gOutData)); # gzinflate and co fails...
		$respxml = $xml;
		$respxml->addCDChild("OtnData", $gOutData);
#		SendPage($gOutData);
	}
}

DoProxy();

header("Content-Type: " . $gOutHeaders['Content-Type']);
#header("Transfer-Encoding: chunked", true);
#if(isset($gOutHeaders['Set-Cookie'])){
#		header("Set-Cookie: ". $gOutHeaders['Set-Cookie'], true);
#}

$oDir = dirname($_SERVER['DOCUMENT_ROOT'] . "/OfflinePages/" . $Engine . $_SERVER['REQUEST_URI']);
$oFile = $_SERVER['DOCUMENT_ROOT'] . "/OfflinePages/" . $Engine . $_SERVER['REQUEST_URI'];
$oFile = $oDir . DIRECTORY_SEPARATOR . basename($oFile);
$saveHeader = $_SERVER['DOCUMENT_ROOT'] . "/OfflinePages/$Engine/$gUpSiteHUB-$gModelId-Auth.h";
if($_SERVER['REQUEST_METHOD'] == 'GET'){
	# TODO: filename translation
	if(!file_exists($oFile)){
		# write file
		if(!file_exists($oDir . DIRECTORY_SEPARATOR ."."))
			mkdir($oDir, 0775, true);
		CError::LogStr("- Save File: -  $oFile ");
			$respxml->saveXML($oFile);
	} else {
		# make backup if changed and save new.
	}
}
if($WhoAmI == "list" || $WhoAmI == "seedkey" || $WhoAmI == "bind" ){
	# store auth informations and token for offlin browsing.
	$tmp = unserialize(file_get_contents($saveHeader));
	if(is_array($tmp))
		$data = array_merge($tmp, $gInHeaders);
	else
		$data = $gInHeaders;
	file_put_contents($saveHeader, serialize($data));
}
?>
