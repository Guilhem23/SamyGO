<?php
# handles openapi/service/widget/gallery 

$CustList = ImportCustomWidgets($CustomApps);
if(isset($QParts['category']) === FALSE){
	#        $xml = uniq__xml($xml);
	#        // $xml = transform_to_xml($xml);
	CError::LogStr("- TODO - xml make uniq?");
}

# b-series:
#  - START -/openapi/service/widget/gallery?category=Manager
# sample how to change version to 1.067, but need check if device is b-series
if(strstr($gModelId, "XX")){
	CError::LogStr("b-series detected $WhoAmI");
	if($QParts['category'] == "Manager" || $WhoAmI == "400009002"){
		$respxml->list->widget['version'] = '1.067'; /* ignore date and size, don't care */
		SendPage($respxml->asXML());
	}
	# and on dl-location request: /openapi/service/widget/400009002 
	# on dec. 2012 some new attributes added by samsung, don't care
	$CustList = ImportCustomWidgets(array("B_hack_widget"));
	$respxml->list->Merge($CustList->list);
	SendPage($respxml->asXML());
}

# c-series:
#

# d-series:
#

# e-series:
#

if($QParts['featured'] == "partners"){
	CError::LogStr("e-series gallery on top in $WhoAmI");
	$respxml->keepNodesByAttrib("widget", "id", $KeepOriginalApps);
} elseif($QParts['featured'] == "signature"){
	CError::LogStr("e-series gallery in middle bar in $WhoAmI");
	$respxml->keepNodesByAttrib("widget", "id", $KeepOriginalApps);
	$respxml->list->removeNodesByAttrib("featured", "system",  array("preinstall", "forced"));
} else {
	CError::LogStr("e-series gallery on bottom $WhoAmI");
	# TODO: post widgetlist to tvgateway saveList
	#$tmpd = $respxml;
	#$tmph = $gInHeaders;
	#$gInHeaders = array();
	#$gInHeaders['Host'] = "tvstoregw.pavv.co.kr";
	#$gInHeaders['Content-Type'] = "text/xml";
	#$_SERVER['REQUEST_METHOD'] = "POST";
	#$_SERVER['REQUEST_URI'] = "/tvGateway/service/AT/de/CPCGYQLKIJBFU/12_ECHOP/saveList";
	#$_SERVER['HTTPS'] = "on";
	#$gInData = $tmpd->list->ids()->asXML();
	#CError::LogStr("ID list from widgetlist " . $gInData);
	#DoProxy(true);
	#CError::LogStr("UPLOAD Response " . $respxml->asXML());
	#$respxml = $tmpd;
	#	$respxml->list->keepNodesByAttrib("widget", "id", $KeepOriginalApps);
	$respxml->list->removeNodesByAttrib("featured", "system",  array("preinstall", "forced", "partners"));
}

# f-series:
# ModelID 13_FOXP ?
#echo $doc->savexml();
$respxml->list->Merge($CustList->list);
$respxml['SamyGOCounter'] = count($respxml->list->children());
SendPage($respxml->asXML());
?>
