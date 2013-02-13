<?php
# handles openapi/service/widget/list

# c-series:
#

# d-series:
#

# e-series:
#

CError::LogStr("In $WhoAmI, need manipulation/merge?");
if(strstr($gModelId, "XX")){ # b-series
	#$respxml->keepNodesByAttrib("widget", "id", $KeepOriginalApps);
	$respxml->list->removeNodesByAttrib("featured", "system",  array("preinstall", "forced", "default"));
	$respxml->list->Merge($CustList->list);
} else {
	$respxml->list->removeNodesByAttrib("featured", "system",  array("preinstall", "forced", "partners", "default", "signature"));
	#$respxml->keepNodesByAttrib("widget", "id", array_merge($KeepOriginalApps, $UserWidgetListAsArray))->list->Merge($CustList->list)->Merge($UserWList)->removeDups();
	$respxml->keepNodesByAttrib("widget", "id", array_merge($KeepOriginalApps, $UserWidgetListAsArray))->list->Merge($CustList->list);
#	$respxml->list->removeNodesByAttrib("featured", "system",  array("preinstall", "forced", "default"));
#	$respxml->list->Merge($CustList->list)->Merge($UserWList)->removeDups();
}

# f-series:
# ModelID 13_FOXP ?
#echo $doc->savexml();
$respxml['SamyGOCounter'] = count($respxml->list->children());
?>
