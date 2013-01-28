<?php
# handles openapi/service/widget/gallery

	$CustList = ImportCustomWidgets($CustomApps);
# b-series:
if(strstr($gModelId, "XX")){ /* or if no timestamp is in the query*/ 
	CError::LogStr("b-series detected $WhoAmI");
	$CustList = ImportCustomWidgets(array("B_hack_widget"));
}

# c-series:
#

# d-series:
#

# e-series:
#

CError::LogStr("e-series in $WhoAmI, need manipulation/merge?");
# DONE: merge in users allready installed widgets...
# hm, whats if we not remove anything, but just change forced...
#	$respxml->list->Merge($UserWList)->removeDups();
$respxml->keepNodesByAttrib("widget", "id", $KeepOriginalApps)->list->Merge($UserWList);
	$respxml->list->Merge($CustList->list)->removeDups();

# f-series:
# ModelID 13_FOXP ?
#echo $doc->savexml();

SendPage($respxml->asXML());
?>
