<?php
global $Engine, $QParts, $QURLParts, $xml, $respxml, $WhoAmI, $gInHeaders, $gInData, $gOutHeaders, $gOutData, $gUserName;
$Engine = "SELP";

if((!stristr($_SERVER['HTTP_HOST'], "pavv.co.kr") && !stristr($_SERVER['HTTP_HOST'], "samsung") && !stristr($_SERVER['HTTP_HOST'], "internetat.tv") && !stristr($_SERVER['HTTP_HOST'], "cloud")) && strstr($_SERVER["REQUEST_URI"], "openapi"))
	die("made a bobo...");

# TODO: drowsing mode on uplink, check user agent (curl/wget) and use stored authinfo or reauth...

require_once('SimpleMidleManHelpers.inc');

function StringToHeader(){} # mabe...

function DoProxy()
{
	global $xml, $respxml, $WhoAmI, $gInHeaders, $gInData, $gOutHeaders, $gOutData, $gUserName;

	CError::LogStr("- START for ". GeoInfoOTN() ." - $WhoAmI -" . $_SERVER["REQUEST_URI"]);

	$gInHeaders = getallheaders();
	$Uplink = nslookup($gInHeaders['Host']); # add check here?
	$UplinkURI = ((isset($_SERVER['HTTPS'])) ? "https://" : "http://") . $Uplink .$_SERVER["REQUEST_URI"];
	$ReqMethod = $_SERVER["REQUEST_METHOD"];
	$CurlOutHeaders = array();
	foreach ($gInHeaders as $header => $value)
	{
		if ($header != "Authorization") # b-series fix for requests (drop Authorization)
			array_push($CurlOutHeaders, $header . ": " . $value);
	}
	CError::LogStr("- Clients header -" . print_r($gInHeaders, 1));
	CError::LogStr("- Routed headers -" . print_r($CurlOutHeaders, 1));
	$gInData = file_get_contents("php://input"); 

	$tuCurl = curl_init();
	curl_setopt($tuCurl, CURLOPT_URL, $UplinkURI);
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
		$xml['hubsite'] = $Uplink;
		$xml->addChild("list");
		$gOutData = $xml->asXML();
		flush();
		header("HTTP/1.0 200 Ok");
		$gOutHeaders = array();
		$gOutHeaders['Content-Type'] = "text/xml";
		$gOutHeaders['cache-control'] = "no-cache";
		}
		CError::LogStr("Code " . print_r($respinfo, 1));
		CError::LogStr("HTTP ($ReqMethod) $RespCode " . $respinfo['http_code'] . " Es wurden " . $respinfo['total_time'] . " Sekunden benötigt für einen Request an " . $respinfo['url']);
	} else {
		CError::LogStr("Request failed. HTTP: (" . $respinfo['http_code'] . ")");
#		CError::LogStr("Dump Vars: for uplink $Uplink " . print_r($UplinkURI, 1) . print_r($GLOBALS, 1));
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
		$xml['hubsite'] = $Uplink;
		$xml->addChild("list");
		$gOutData = $xml->asXML();
		header("HTTP/1.0 200 Ok");
		$gOutHeaders = array();
		$gOutHeaders['Content-Type'] = "text/xml";
		$gOutHeaders['cache-control'] = "no-cache";
	}

	CError::LogStr("- Response Header (maybe selfmage!!!!!!!!!) - " . print_r($gOutHeaders, 1));

	if(strstr($gOutData, "xml")){
		$respxml = new mySXE($gOutData);
	} elseif(strstr($gOutData, "Salted_")){
		CError::LogStr("- OTN crypted file: $UplinkURI -");
#		SendPage($gOutData);
	} else {
		CError::LogStr("- Response error: not a xml !!! gzip or html content? -" . bin2hex($gOutData)); # gzinflate and co fails...
#		SendPage($gOutData);
	}
}

DoProxy();

header("Content-Type: " . $gOutHeaders['Content-Type']);
header("cache-control: " . $gOutHeaders['cache-control']);
#header("Transfer-Encoding: chunked", true);
#if(isset($gOutHeaders['Set-Cookie'])){
#		header("Set-Cookie: ". $gOutHeaders['Set-Cookie'], true);
#}

$oDir = dirname($_SERVER['DOCUMENT_ROOT'] . "/OfflinePages/" . $Engine . $_SERVER['REQUEST_URI']);
$oFile = $_SERVER['DOCUMENT_ROOT'] . "/OfflinePages/" . $Engine . $_SERVER['REQUEST_URI'];
$oFile = $oDir . DIRECTORY_SEPARATOR . basename($oFile);
$saveHeader = $oFile . "_LastHeader.h";
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
if($WhoAmI == "list")
	file_put_contents($saveHeader, serialize($gInHeaders));
?>
