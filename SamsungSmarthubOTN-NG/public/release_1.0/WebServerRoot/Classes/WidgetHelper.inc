<?php
# some helpers for widget data manipulation/transormation

function uniq_xml($xml, $tmplate = 'rsp_list_uniq.xsl') {
	$xslt = new XSLTProcessor();
	$xmlDoc = new DOMDocument('1.0', 'UTF-8');
	$xslDoc = new DOMDocument('1.0', 'UTF-8');
	$xslDoc->loadXML(file_get_contents($tmplate, TRUE));
	$xmlDoc->loadXML($xml->asXML());
	$xslt->importStylesheet($xslDoc);
	$domresu = $xslt->transformToDoc($xmlDoc);
	$xmlresu = new SimpleXMLElement($domresu->saveXML());
	return $xmlresu;
}

?>
