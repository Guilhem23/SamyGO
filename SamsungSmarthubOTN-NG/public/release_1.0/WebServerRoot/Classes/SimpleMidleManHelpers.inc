<?php
require_once('SELP_Config.inc');

class CError {
	public static function LogStr($str)
	{
		$Logfilename = $_SERVER['DOCUMENT_ROOT']."/logs/SGError-".$_SERVER['REMOTE_ADDR'].".log";
		//$Logfilename = "/dev/null";
		$f = fopen($Logfilename,"a");
		if ($f)
		{
			$LogStr = date("d.m.Y H:i:s")." - ".$_SERVER['SCRIPT_NAME']." -> ".$str."\r\n";
			fwrite($f,$LogStr);
			fclose($f);
		}
	}
} // class CError

class mySXE extends SimpleXMLElement{
	function getName(){
		return dom_import_simplexml($this)->nodeName;
	}

	function getType(){
		return dom_import_simplexml($this)->nodeType;
	}
	function keepNodesByAttrib($nodename = "widget", $attribute = "id", $values){
		$arg = '@' . $attribute . '!=';
		foreach($values as $val){
			$arg .= $val . ' and @' . $attribute . '!=' ;
		}
		$arg = substr($arg, 0, -(strlen($attribute) + 8));
		$tmp = $this->findm("//$nodename"."[".$arg."]");
		foreach ($tmp as $node) {
			$node->remove();
		}
		return $this;
	}
	function keepOneByAttrib($nodename = "widget", $attribute = "id", $value){
		$tmp = $this->findm("//$nodename"."[@".$attribute."!=$value]");
		foreach ($tmp as $node) {
			$node->remove();
		}
		return $this;
	}
	function removeNodesByAttrib($nodename = "widget", $attribute = "id", $values){
		foreach($values as $val){
			$this->findone("//$nodename"."[@".$attribute."=$val]")->remove();	
		}
		return $this;
	}
	function removeDups($nodename = "widget", $attribute = "id"){
		$tmp = $this->findm("//$nodename"."[@".$attribute."=preceding-sibling::".$nodename."/@".$attribute."]");
		foreach ($tmp as $node) {
			$node->remove();
		}
		return $this;
	}
	protected function findm($xpath) {
		$tmp = $this->xpath($xpath);
		return ($tmp[0]) ? $tmp : null;
	}
	protected function findone($xpath) {
		$tmp = $this->xpath($xpath);
		return isset($tmp[0]) ? $tmp[0] : null;
	}
	function remove() {
		$dom = dom_import_simplexml($this);
		return $dom->parentNode->removeChild($dom);
	}
	function Merge($Node, $onTop = false){
		if(!$Node)
			return;

		$refNode = dom_import_simplexml($this);

		foreach ($Node->children() as $Child)
		{
			$Element = dom_import_simplexml($Child);

			if (!$refNode || !$Element)
				return;

			$merge = $refNode->ownerDocument->importNode($Element, true);
			if($onTop)
				$refNode->insertBefore($merge, $refNode->firstChild);
			else
				$refNode->appendChild($merge);
		}
		return $this;
	}
	function appendChild($name, $content) {
		$dom = dom_import_simplexml($this);
		$dom->appendChild($dom->ownerDocument->createElement($name, $content));
	}
	function addCDChild($name, $string) {
		$dom = dom_import_simplexml($this);
		$dom->appendChild($dom->ownerDocument->createElement($name));
		$cdata = $dom->ownerDocument->createCDATASection($string);
		return $dom->appendChild($cdata);
	}
	function addCData($path, $string){
		$dom = dom_import_simplexml($path);
		$cdata = $dom->ownerDocument->createCDATASection($string);
		return $dom->appendChild($cdata);
	}	
	function insertCData($string){
		$dom = dom_import_simplexml($this);
		$cdata = $dom->ownerDocument->createCDATASection($string);
		return $dom->appendChild($cdata);
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
}

function nslookup($host){
	// execute nslookup command
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
			return str_replace(" ", "", substr($op[4], strpos($op[4], 'name =') + 7, -1));
		} else {
			#return str_replace(" ", "", substr($op[4], strpos($op[4], 'Name:') + 6));
			return str_replace(" ", "", substr($op[5], strpos($op[5], 'Address:') + 8));
		}
	}
}

function SendPage($string){
	print($string);
	session_commit();
	CError::LogStr("- ". __FUNCTION__ ." - " . substr($string, 0, 200));	
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

function sxml_cdata($path, $string){
	$dom = dom_import_simplexml($path);
	$cdata = $dom->ownerDocument->createCDATASection($string);
	$dom->appendChild($cdata);
}
function sxml_change($path, $string, $value = NULL){
	foreach ($path as $child){
		$child->$string = null;
		unset($child->$string);
		$child->addChild($string, $value);
	}
}
function sxml_change_attr($path, $string, $value = NULL){
	$path[$string] = null;
	unset($path[$string]);
	$path->addAttribute($string, $value);
}
function sxml_append(SimpleXMLElement $to, SimpleXMLElement $from) {     
	$toDom = dom_import_simplexml($to);     
	$fromDom = dom_import_simplexml($from);     
	$toDom->appendChild($toDom->ownerDocument->importNode($fromDom, true)); 
}

$gUserName = preg_replace('/.*\+_(.*)\+.*/', '$1', $_SERVER['HTTP_TOKEN']);
$QURLParts = preg_split('/\//', $_SERVER['REQUEST_URI'], -1, PREG_SPLIT_NO_EMPTY);
$WhoAmI = basename($_SERVER['PHP_SELF'], ".php");
$UserWidetList = $_SERVER['DOCUMENT_ROOT'] . "/UserWidgetInstall/" . $gUserName . ".xml";
parse_str(@$_SERVER['QUERY_STRING'], $QParts);
$xmlstr = '<?xml version="1.0" encoding="UTF-8"?><rsp></rsp>';
$xml = new mySXE($xmlstr);
$xml->addAttribute("stat", "ok");
$xml->addAttribute("cachedate", get_KoreanTimeforSeedKey(0));
?>
