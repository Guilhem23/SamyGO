<?php
# handles openapi/service/widget/gallery 

# c-series:
#

# d-series:
#

# e-series:
#

#	$respxml->list->removeNodesByAttrib("featured", "system",  array("preinstall", "forced"));
if($QParts['featured'] == "partners"){
	CError::LogStr("gallery on top in $WhoAmI");
	$respxml->keepNodesByAttrib("widget", "id", $KeepOriginalApps)->list->Merge($CustList->list->removeNodesByAttrib("featured", "system",  array("signature"), true));
} elseif($QParts['featured'] == "signature"){
	CError::LogStr("gallery in middle bar in $WhoAmI");
	$respxml->keepNodesByAttrib("widget", "id", $KeepOriginalApps)->list->Merge($CustList->list->removeNodesByAttrib("featured", "system",  array("partners"), true));
} elseif($QParts['category'] == "Manager"){
	CError::LogStr("b-series Manager request $WhoAmI");
	# maybe handle $ReplaceOriginalApps ? or later ....
	# ?category=CP_YourVideo
	# ?category=CP_Fitness 
	# ?category=CP_KidsStory
} elseif(strstr($gModelId, "XX")){ # b-series
	#$respxml->keepNodesByAttrib("widget", "id", $KeepOriginalApps);
	$respxml->list->removeNodesByAttrib("featured", "system",  array("preinstall", "forced", "default"));
	$respxml->list->Merge($CustList->list);
} else {
	CError::LogStr("gallery bottom $WhoAmI");
	$respxml->list->removeNodesByAttrib("featured", "system",  array("preinstall", "forced", "partners"));
	$respxml->list->Merge($CustList->list);
	#	$respxml->list->removeNodesByAttrib("featured", "system",  array("preinstall", "forced", "default"));
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
	#	$respxml->list->removeNodesByAttrib("featured", "system",  array("signature", "partners"));
	#respxml->list->Merge($CustList->list);
}

# f-series:
# ModelID 13_FOXP ?
#echo $doc->savexml();
#$respxml->list->Merge($CustList->list);
#$respxml->list->Merge($CustList->list)->list->Merge($UserWList)->removeDups();
#$respxml->list->Merge($UserWList)->removeDups();
$respxml['SamyGOCounter'] = count($respxml->list->children());
?>
