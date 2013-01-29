<?php
require_once('SELP_Config.inc');

#if((InSane()) != false)
#	die("InSanecheck Failed...");
InSane();

class CError {
	public static function LogStr($str){
		$Logfilename = $_SERVER['DOCUMENT_ROOT']."/logs/SGError-".$_SERVER['REMOTE_ADDR'].".log";
		//$Logfilename = "/dev/null";
		$f = fopen($Logfilename, "a");
		if($f){
			$LogStr = date("d.m.Y H:i:s")." - ".$_SERVER['SCRIPT_NAME']." -> ".$str."\r\n";
			fwrite($f,$LogStr);
			fclose($f);
		} else {
			print($LogStr . PHP_EOL);
		}
	}
} // class CError
function myErrorHandler($fehlercode, $fehlertext, $fehlerdatei, $fehlerzeile){
	switch ($fehlercode){
	case E_USER_ERROR:
		CError::LogStr("- Mein FEHLER Zeile $fehlerzeile in der Datei $fehlerdatei - [$fehlercode] $fehlertext.");
		exit(1);
		break;

	case E_USER_WARNING:
		CError::LogStr("- Meine WARNUNG - [$fehlercode] $fehlertext");
		break;

	case E_USER_NOTICE:
		CError::LogStr("- Mein HINWEIS - [$fehlercode] $fehlertext");
		break;

	default:
		//echo "Unbekannter Fehlertyp: [$fehlercode] $fehlertext<br />\n";
		CError::LogStr("- Unbekannter Fehlertyp - Zeile: $fehlerzeile in der Datei $fehlerdatei - [$fehlercode] $fehlertext.");
		break;
	}

	/* Damit die PHP-interne Fehlerbehandlung nicht ausgeführt wird */
	return true;
}

class mySXE extends SimpleXMLElement{
	function getName(){
		return dom_import_simplexml($this)->nodeName;
	}

	function getType(){
		return dom_import_simplexml($this)->nodeType;
	}
	function keepNodesByAttrib($nodename = "widget", $attribute = "id", $values){
		if(!is_array($values))
			$values = explode(" ", $values);

		$tmp = $this->findm("//$nodename"."[@"."$attribute!='" .implode("' and @"."$attribute!='", $values) . "']");
		foreach ($tmp as $node) {
			$node->remove();
		}
		return $this;
	}
	function keepOneByAttrib($nodename = "widget", $attribute = "id", $value){
		$tmp = $this->findm("//$nodename"."[@".$attribute."!='$value']");
		foreach ($tmp as $node) {
			$node->remove();
		}
		return $this;
	}
	function removeNodesByAttrib($nodename = "widget", $attribute = "id", $values){
		if(!is_array($values))
			$values = explode(" ", $values);

		$tmp = $this->findm("//$nodename"."[@"."$attribute='" .implode("' or @"."$attribute='", $values) . "']");
		if(!is_array($tmp))
			return $this;
		foreach ($tmp as $node) {
			$node->remove();
		}
		return $this;
	}
	function removeDups($nodename = "widget", $attribute = "id"){
		$tmp = $this->findm("//$nodename"."[@".$attribute."=preceding-sibling::".$nodename."/@".$attribute."]");
		if(!is_array($tmp))
			return $this;
		foreach ($tmp as $node) {
			$node->remove();
		}
		return $this;
	}
	# nned both of find... ? 
	protected function findm($xpath){
		$tmp = $this->xpath($xpath);
		return ($tmp[0]) ? $tmp : null;
	}
	protected function findone($xpath){
		$tmp = $this->xpath($xpath);
		return isset($tmp[0]) ? $tmp[0] : null;
	}
	function ids($attribute = "id"){
		$tmp = $this->xpath("//@" . $attribute);
		foreach ($tmp as $Child) {
			$resid[] = $Child[$attribute];
		}
		$res ='<?xml version="1.0" encoding="utf-8"?><widgetList><widgetIDs>';
		$res .= implode(",", $resid);
		$res .= '</widgetIDs></widgetList>';
		$res = new mySXE($res);
		return $res;
	}
	function remove(){
		$dom = dom_import_simplexml($this);
		$Trashed = $dom->parentNode->removeChild($dom);
		return $this;
	}
	function Merge($Node, $onTop = false, $returntheoldnode = true){
		if(!$Node)
			return $this;
		$refNode = dom_import_simplexml($this);

		foreach ($Node->children() as $Child)
		{
			$Element = dom_import_simplexml($Child);

			if (!$refNode || !$Element)
				return $this;

			$merge = $refNode->ownerDocument->importNode($Element, true);
			if($onTop)
				$refNode->insertBefore($merge, $refNode->firstChild);
			else
				$refNode->appendChild($merge);
		}
		if($returntheoldnode)
			return $this;
		else
			return simplexml_import_dom($refNode, get_class($this));
	}
	function glueElement($Node){
		if(!$Node)
			return $this;
		$dom = dom_import_simplexml($this);
		$Element = dom_import_simplexml($Node);

		if (!$dom || !$Element)
			return $this;
		$merge = $dom->appendChild(
			$dom->ownerDocument->importNode($Element, true));
		return $this;
	}
	function replaceNode($Node){ /* http://php.net/manual/en/domnode.replacechild.php */
		# ähhhhhh, simply delete and inser the new one?????
	}
	function change($string, $value = NULL){
		foreach ($this as $child){
			$child->$string = null;
			unset($child->$string);
			$child->addChild($string, $value);
		}
		return $this;
	}
	function prependChild($name, $content = NULL){
		$dom = dom_import_simplexml($this);
		$leader = $dom->insertBefore(
			$dom->ownerDocument->createElement($name, $content),
			$dom->childNodes->item(2));
			# $dom->previousSibling);
			# $dom->nextSibling);
			# $dom->firstChild);
		return simplexml_import_dom($leader, get_class($this));
	}
	function appendChild($name, $content = NULL){
		$dom = dom_import_simplexml($this);
		$Node = $dom->appendChild(
			$dom->ownerDocument->createElement($name, $content));
		return simplexml_import_dom($Node, get_class($this));
	}
	function addCDChild($name, $string = NULL){
		$dom = dom_import_simplexml($this);
		$Node = $dom->appendChild(
			$dom->ownerDocument->createElement($name));
		$Node->appendChild($Node->ownerDocument->createCDATASection($string));
		return simplexml_import_dom($Node, get_class($this));
	}
	function addCData($string){
		$dom = dom_import_simplexml($this);
		$Node = $dom->appendChild(
			$dom->ownerDocument->createCDATASection($string));
		return $this;
	}	
	function changeChild($string, $value = NULL){
		$this->$string = NULL;
		unset($this->$string);
		$res = $this->addChild($string, $value);
		return $res;
	}
	function changeCData($string = NULL){
		$this[0] = NULL;
		$dom = dom_import_simplexml($this);
		$Node = $dom->appendChild(
			$dom->ownerDocument->createCDATASection($string));
		return $this;
	}	
	function insertCData($string = NULL){
		$dom = dom_import_simplexml($this);
		$Node = $dom->appendChild(
			$dom->ownerDocument->createCDATASection($string));
		return $this;
	}	
	function saveXML($string = NULL){
		$dom = new DOMDocument('1.0', 'UTF-8');
		$dom->preserveWhiteSpace = false;
		$dom->formatOutput = true;
		$dom->loadXML($this->asXML());

		return $dom->save($string);
	}
	function pPrint($string = NULL){
		$domnode = dom_import_simplexml($this);

		if (!$domnode) {
			echo 'Error while converting XML';
			exit;
		}

		$dom = new DOMDocument('1.0', 'UTF-8');
		$dom->preserveWhiteSpace = false;
		$dom->formatOutput = true;

		$domnode = $dom->importNode($domnode, true);
		$dom->appendChild($domnode);

		return $dom->saveXML($string);
	}
	function dumbPrint(){
		$unwanted = array("\t", "\n", "\r", '"/');
		$wanted = array('', '', '', '" /');
		$res = str_replace($unwanted, $wanted, $this->asXML());
		return $res;
	}
}

function nslookup($host){
	// execute nslookup command
			return gethostbyname($host);
	exec('nslookup '.$host.'. 8.8.8.8', $op);

	// php is running on windows machine
	if (substr(php_uname(), 0, 7) == "Windows") {
		# return str_replace(" ", "", substr($op[3], 6)); ip -> name
		return str_replace(" ", "", substr($op[4], 8)); # name -> ip
	}
	else {
		// on linux nslookup returns 2 diffrent line depending on
		// ip or hostname given for nslookup
		if (strpos($op[4], 'name = ') > 0){
			return gethostbyname($host);
			#return str_replace(" ", "", substr($op[4], strpos($op[4], 'name =') + 7, -1));
		} else {
			#return str_replace(" ", "", substr($op[4], strpos($op[4], 'Name:') + 6));
			return str_replace(" ", "", substr($op[5], strpos($op[5], 'Address:') + 8));
		}
	}
}

function SendPage($string, $bytes = 512){
	print($string);
	session_commit();
	if(function_exists("headers_list"))
		CError::LogStr("- FINAL HEADERS - " . __FUNCTION__ . " - " . print_r(headers_list(), 1));
	CError::LogStr("- FINAL $bytes of OUTGOING DATA - " . __FUNCTION__ . " - " . substr($string, 0, $bytes));	
	exit();
}

function parseQueryString($str) {
	$op = array();
	$pairs = explode("&", $str);
	foreach ($pairs as $pair) {
		list($k, $v) = array_map("urldecode", explode("=", $pair));
		$op[$k] = $v;
	}
	return $op;
} 

function GeoInfoOTN()
{
	global $g_RegionNames;

	$RegionName = strtolower($g_RegionNames[$_SERVER['GEOIP_CONTINENT_CODE']]);
	$FormatedRegion = $RegionName.':'.strtolower($_SERVER['GEOIP_COUNTRY_NAME']).':'.strtolower($_SERVER['GEOIP_REGION_NAME']);

	return $FormatedRegion;
}
function GeoInfoOTNSG()
{
	global $g_RegionNames;

	$RegionName = strtolower($g_RegionNames[$_SERVER['GEOIP_CONTINENT_CODE']]);
	$FormatedRegion = $RegionName.':'.strtolower($_SERVER['GEOIP_COUNTRY_NAME']).':'.strtolower($_SERVER['GEOIP_REGION_NAME']).':'.strtolower($_SERVER['GEOIP_CITY']);

	return $FormatedRegion;
}

function millsec()
{
	list($usec, $sec) = explode(" ", microtime());
	return ceil((float)$usec * 1000 + (float)$sec * 1000);
}

function StrToHex($string)
{
	$hex='';
	for ($i=0; $i < strlen($string); $i++)
	{
		$hex .= sprintf("%02x", ord($string[$i]));
	}
	return $hex;
}
function HexToStr($hex)
{
	$string='';
	for ($i=0; $i < strlen($hex)-1; $i+=2)
	{
		$string .= chr(hexdec($hex[$i].$hex[$i+1]));
	}
	return $string;
}

function toWurstl($str)
{
	$unwanted = array("\t", "\n", "\r", '"/');
	$wanted = array('', '', '', '" /');
	$string = str_replace($unwanted, $wanted, $str);
	return $string;
}

function get_GMT()
{
	$GreenTime = new DateTime;
	return strftime("%Y-%m-%dT%H:%M:%S+00:00", $_SERVER['REQUEST_TIME'] - $GreenTime->getOffset());
}

function get_GMT_Short($plus = 30)
{
	/* 2011-11-19 23:07:38 */
	$GreenTime = new DateTime;
	return strftime("%Y-%m-%d %H:%M:%S", $_SERVER['REQUEST_TIME'] - $GreenTime->getOffset() + $plus * 3600);
}

function get_KoreanTimeforSeedKeyGMT()
{
	$GreenTime = new DateTime;
	return strftime("%Y-%m-%dT%H:%M:%SZ", $_SERVER['REQUEST_TIME'] - $GreenTime->getOffset() + 24 * 3600);
}

function get_KoreanTimeforSeedKey($plus = 22)
{
	return strftime("%Y-%m-%dT%H:%M:%S+0900", $_SERVER['REQUEST_TIME'] + (9 + $plus) * 3600 - 180);
}

function get_KoreanHour($plus = 22)
{
	return strftime("%Y-%m-%dT%H:00:00+0900", $_SERVER['REQUEST_TIME'] + (9 + $plus) * 3600 - 180);
}

function get_KoreanTime($plus = 9)
{
	return strftime("%a, %d  %b %Y %H:%M:%S +0900", $_SERVER['REQUEST_TIME'] + $plus * 3600);
}

function BrowserLang(){
	if(isset($_SERVER['HTTP_ACCEPT_LANGUAGE']))
		$slang = substr($_SERVER['HTTP_ACCEPT_LANGUAGE'], 0, 2);
	else
		$slang = "de-DE";
	return $slang;
}
function InSane(){
	global $IgnoreProxy, $WhoAmI, $gUserName, $gModelId, $gModelName, $gFirmcode, $gDUID, $gToken;

	$WhoAmI = basename($_SERVER['PHP_SELF'], ".php");
	$gUserName = (isset($_SERVER['HTTP_TOKEN'])) ? preg_replace('/.*\+_(.*)\+.*/', '$1', $_SERVER['HTTP_TOKEN']) : $_SERVER['HTTP_MODELID'] . "_" . $_SERVER['HTTP_MACADDR'];
	$gModelId = (isset($_SERVER['HTTP_MODELID'])) ? $_SERVER['HTTP_MODELID'] :  ((isset($_SERVER['HTTP_CHIPSET'])) ? "12_" . $_SERVER['HTTP_CHIPSET'] . "_" . $_SERVER['HTTP_MACADDR'] : substr($gUserName, 0, -13));
	
	if(isset($_SERVER['HTTP_TOKEN']))
		$gToken = $_SERVER['HTTP_TOKEN'];

	if($_SERVER['HTTP_HOST'] == "store"){
		# /tvGateway/service/AT/de/CPCGYQLKIJBFU/12_ECHOP/downConfirm
	}
	return false;
	session_start();
	CError::LogStr("START Sanity COOKIE vars: " . print_r($_COOKIE, 1));
	CError::LogStr("START Sanity SESSION vars: " . print_r($_SESSION, 1));
	CError::LogStr("START Sanity check ($WhoAmI): " . print_r($_SERVER, 1));

	#	if($IgnoreProxy)
	#		return FALSE;

	if($WhoAmI == "request"){
	# request?id=2100000650_001&s=330x186&dt=03&did=CPCGYQLKIJBFU&pt=04&pv=T-INFOLINK2012-1013&nt=10&coc=at&lnc=deu&ts=1359380413300&tz=60&scr=1920x1080&dy=2012&md=12_ECHOP&mf=Samsung
	return false;
	}

	if(isset($_SERVER['HTTP_MODELID']) && (empty($_SERVER['HTTP_MODELID']) || strstr($_SERVER['HTTP_MODELID'], " ") || strlen($_SERVER['HTTP_MODELID'] < 5)))
		return true;

	#/openapi/conf/application
	#[DUID] => CPCA3M6BJJVEO
	#[ModelID] => LEXXB650_EU
	#[CountryCode] => AT
	if($WhoAmI == "application"){
		$gDUID = $_SERVER['HTTP_DUID'];
		$gModelId = $_SERVER['HTTP_MODELID'];
		$gUserName = $gModelId . "_112233445566";
		return false;
	}

	if($WhoAmI == "countries"){
		$gDUID = "AABBCCDDEE";
		$gModelId = "12_ECHOP";
		$gUserName = $gModelId . "_112233445566";
		return false;
	}

	if($WhoAmI == ("date" || "version")){
		$gUserName = preg_replace('/.*\+_(.*)\+.*/', '$1', $_SERVER['HTTP_TOKEN']);
		return false;
	}

	if($WhoAmI == "query"){
		$gDUID = $_SERVER['HTTP_DUID'];
		$gModelId = $_SERVER['HTTP_MODELID'];
		$gUserName = $gModelId . "_" . $_SERVER['HTTP_MACADDR'];
		return false;
	}

	if(strstr($WhoAmI, "notice")){
		$gDUID = $_SERVER['HTTP_DUID'];
		# ModelId>/<ClientFWVersion
		$tmp = preg_split('/\//', $_SERVER['REQUEST_URI'], -1, PREG_SPLIT_NO_EMPTY);
		$ClientFWVer = $tmp[3];
		#$ClientSecKey = $tmp[4];
		$gModelId =  $tmp[2];
		$gFirmcode = $tmp[3];
		return false;
	}

	if($WhoAmI == "seedkey"){
		$gDUID = $_SERVER['HTTP_DUID'];
		$gFirmcode = $_SERVER['HTTP_FIRMCODE'];
		$gModelId = $_SERVER['HTTP_MODELID'];
		$gUserName = $gModelId . "_" . $_SERVER['HTTP_MACADDR'];
		return false;
	}


	foreach ($_SERVER as $k => $v)
	{
		if (substr($k, 0, 5) == "HTTP_")
		{
			if(preg_match('/(VIA|FORWARD|PROXY|CLIENT|CUDA|COMING|FROM)/', $k)){
				return true;
			}
		}
	}
	return false;
}

# setup GLOBALS:
# EMP: rm
#/openapi/device/auth/query -> - SERVER VARS on PRE Enter - Array
#	[HTTP_DUID] => ZPCB3EXS33NPU
#	[HTTP_MACADDR] => 1c5a3e29af93
#	[HTTP_MODELID] => sometimes empty -> reject request!!!
#	[HTTP_CATEGORY] => EMP
#	[HTTP_PARAM] => X10P,0.970
#	[HTTP_COUNTRYCODE] => US
#	[HTTP_HOST] => www.samsungrm.net
#	[HTTP_CONNECTION] => close
# HUB:
#/openapi/device/auth/seedkey -> - SERVER VARS on PRE Enter - Array
#	[HTTPS] => on
#	[HTTP_ACCEPT] => text/html, image/png, image/*, */*
#	[HTTP_ACCEPT_LANGUAGE] => ko
#	[HTTP_HOST] => infolink.pavv.co.kr
#	[HTTP_USER_AGENT] => Mozilla/5.0 (Windows; U; en-US; rv:1.8.1.11; Gecko/20071129; Firefox/2.5.0) Maple 6.0.00067 Navi
#	[HTTP_CONNECTION] => close
#	[CONTENT_TYPE] => text/xml
#	[HTTP_CACHE_CONTROL] => no-cache
#	[HTTP_APPKEY] => 5cdfb2dc-8598-17af-b9a3-100000230005-3c290b4b-9c98-4974-890a-3b465274d549
#	[HTTP_COUNTRYCODE] => CA
#	[HTTP_MODELID] => X6_PDP
#	[HTTP_MACADDR] => 606bbd91bd47
#	[HTTP_FIRMCODE] => T-INFOLINK2011-1005
#	[HTTP_DUID] => P7CFP7UY6ZBDG
#/openapi/conf/date -> - SERVER VARS on PRE Enter - Array
#	[HTTP_ACCEPT] => text/html, image/png, image/*, */*
#	[HTTP_ACCEPT_LANGUAGE] => ko
#	[HTTP_HOST] => infolink.pavv.co.kr
#	[HTTP_TOKEN] => c46fcf10+_X6_PDP_606bbd91bd47+92da379848417e6c32e2b5c68815f67b356fbb68
#	[HTTP_USER_AGENT] => Mozilla/5.0 (Windows; U; en-US; rv:1.8.1.11; Gecko/20071129; Firefox/2.5.0) Maple 6.0.00067 Navi
#	[HTTP_COOKIE] => WMONID=4df3ef0a
#	[HTTP_CONNECTION] => close
#	[CONTENT_TYPE] => text/xml
#	[HTTP_CACHE_CONTROL] => no-cache, no-cache
#	[HTTP_APPKEY] => 5cdfb2dc-8598-17af-b9a3-100000230005-3c290b4b-9c98-4974-890a-3b465274d549
#/openapi/service/channel/gallery
#	[HTTP_ACCEPT] => text/html, image/png, image/*, */*
#	[HTTP_ACCEPT_LANGUAGE] => ko
#	[HTTP_HOST] => infolink.pavv.co.kr
#	[HTTP_USER_AGENT] => Mozilla/5.0 (Windows; U; en-US; rv:1.8.1.11; Gecko/20071129; Firefox/2.5.0) Maple 6.0.00067 Navi
#	[HTTP_CONNECTION] => close
#	[CONTENT_TYPE] => text/xml
#	[HTTP_CACHE_CONTROL] => no-cache, no-cache
#	[HTTP_APPKEY] => e921ab54-3744-109f-bb40-100000230005-1989ecc0-ae89-445a-bbdb-6a6a647efec6
#/openapi/conf/log -> /openapi/conf/log?service=widget&timestamp=1359096485874
#	[HTTP_ACCEPT] => text/html, image/png, image/*, */*
#	[HTTP_ACCEPT_LANGUAGE] => ko
#	[HTTP_HOST] => infolink.pavv.co.kr
#	[HTTP_TOKEN] => c46fcf10+_X6_PDP_606bbd91bd47+92da379848417e6c32e2b5c68815f67b356fbb68
#	[HTTP_USER_AGENT] => Mozilla/5.0 (Windows; U; en-US; rv:1.8.1.11; Gecko/20071129; Firefox/2.5.0) Maple 6.0.00067 Navi
#	[HTTP_COOKIE] => WMONID=c46fcf10
#	[HTTP_CONNECTION] => close
#	[CONTENT_TYPE] => text/xml
#	[HTTP_CACHE_CONTROL] => no-cache, no-cache
#	[HTTP_APPKEY] => 5cdfb2dc-8598-17af-b9a3-100000230005-3c290b4b-9c98-4974-890a-3b465274d549
#/openapi/service/widget -> - SERVER VARS on PRE Enter - Array
#	[HTTP_ACCEPT] => text/html, image/png, image/*, */*
#	[HTTP_ACCEPT_LANGUAGE] => ko
#	[HTTP_HOST] => infolink.pavv.co.kr
#	[HTTP_TOKEN] => c46fcf10+_X6_PDP_606bbd91bd47+92da379848417e6c32e2b5c68815f67b356fbb68
#	[HTTP_USER_AGENT] => Mozilla/5.0 (Windows; U; en-US; rv:1.8.1.11; Gecko/20071129; Firefox/2.5.0) Maple 6.0.00067 Navi
#	[HTTP_COOKIE] => WMONID=c46fcf10
#	[HTTP_CONNECTION] => close
#	[CONTENT_TYPE] => text/xml
#	[HTTP_CACHE_CONTROL] => no-cache, no-cache
#	[HTTP_APPKEY] => 5cdfb2dc-8598-17af-b9a3-100000230005-3c290b4b-9c98-4974-890a-3b465274d549


# OTN:
#/openapi/device/auth/seedkey
#	[HTTPS] => on
#	[SSL_TLS_SNI] => www.samsungotn.net
#	[HTTP_HOST] => www.samsungotn.net
#	[HTTP_ACCEPT] => */*
#	[HTTP_APPKEY] => ed29e9fe-4225-418c-88ee-2f1bc01e6a8e-2ad7f3ce-23c0-4cdb-afff-d1481531f1bb
#	[HTTP_DUID] => H3CISIPCYNNGE
#	[HTTP_MACADDR] => e47cf921af62
#	[HTTP_FIRMCODE] => SWU_T-VAL6DEUC_001006_I04_ES000DS000KS000RS000_100422
#	[HTTP_MODELID] => T-VAL6DEUC
#	[HTTP_VERSION] => 1006.100422
# d and e series +
#		[HTTP_MODELNAME] => UE40D6500
#/openapi/conf/version
#	[HTTPS] => on
#	[SSL_TLS_SNI] => www.samsungotn.net
#	[HTTP_HOST] => www.samsungotn.net
#	[HTTP_ACCEPT] => */*
#	[HTTP_TOKEN] => 5041a148+_T-VAL6DEUC_5437490089cd+395e1c631a07c70ee717fd4b22d2cb1ffa456555
#	[HTTP_APPKEY] => ed29e9fe-4225-418c-88ee-2f1bc01e6a8e-2ad7f3ce-23c0-4cdb-afff-d1481531f1bb
#/openapi/tv -> /openapi/tv/<ModelId>/<ClientFWVersion>/m_notice
#	[HTTP_HOST] => www.samsungotn.net
#	[HTTP_ACCEPT] => */*
#	[HTTP_DUID] => H3CISIPCYNNGE
#/openapi/log/otn -> /openapi/log/otn/<DUID> 
#	[HTTP_HOST] => www.samsungotn.net
#	[HTTP_ACCEPT] => */*
#	[CONTENT_TYPE] => application/x-www-form-urlencoded
#	[CONTENT_LENGTH] => 158

$UserWidetList = $_SERVER['DOCUMENT_ROOT'] . "/UserWidgetInstall/" . $gUserName . ".xml";
$QURLParts = preg_split('/\//', $_SERVER['REQUEST_URI'], -1, PREG_SPLIT_NO_EMPTY);
parse_str(@$_SERVER['QUERY_STRING'], $QParts);
# $QURLParts[6]
#  https://tvstoregw.pavv.co.kr/tvGateway/service/AT/ko/CPCGYQLKIJBFU/12_ECHOP/detail
#  https://tvstoregw.pavv.co.kr/tvGateway/service/ 2/ 3/     4       /   5    /   6  
# for tvgateway:
/* xml version="1.0" encoding="UTF-8"<result><resultCode>200</resultCode><resultMessage><![CDATA[OK]]></resultMessage></result> */
# elsewhere: 
/* xml version="1.0" encoding="UTF-8" <rsp></rsp> */
$xmlstr = '<?xml version="1.0" encoding="UTF-8"?><rsp></rsp>';
$xml = new mySXE($xmlstr);
$xml->addAttribute("stat", "ok");
$xml->addAttribute("cachedate", get_KoreanTimeforSeedKey(0));
?>
