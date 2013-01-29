<?php
# handles openapi/service/widget/list

# c-series:
#

# d-series:
#

# e-series:
#

CError::LogStr("In $WhoAmI, need manipulation/merge?");
#$respxml->list->removeNodesByAttrib("featured", "system",  array("preinstall", "forced", "partners"));
# DONE: merge in users allready installed widgets...
# hm, whats if we not remove anything, but just change forced...
#	$respxml->list->Merge($UserWList)->removeDups();
$respxml->keepNodesByAttrib("widget", "id", $KeepOriginalApps)->list->Merge($CustList->list)->list->Merge($UserWList);
#$respxml->keepNodesByAttrib("widget", "id", $KeepOriginalApps);
#$respxml->list->Merge($CustList->list)->removeDups();
#$respxml->list->Merge($CustList->list);

# f-series:
# ModelID 13_FOXP ?
#echo $doc->savexml();

?>
