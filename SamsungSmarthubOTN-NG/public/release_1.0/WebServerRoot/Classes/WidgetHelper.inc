<?php
# some helpers for widget data manipulation/transormation used in different engines (Infolink/SmartHub/AppStore)

$KeepOriginalApps = array("400009002", "10120000000", "10120000099");  /* never remove the engines manager sh2 appstore */
$KeepOriginalApps[] = "11111000009"; /* Explore3D */
$KeepOriginalApps[] = "20121000003"; /* empWebBrowser */
$KeepOriginalApps[] = "10110001001"; /* empQs */
$KeepOriginalApps[] = "111199000764"; /* Camera */ 
$KeepOriginalApps[] = "10110001001"; /* MBRWidget */ 

$ReplaceOriginalApps = array("10120000000");  /* never remove the engines manager sh2 appstore */

$CustomApps = array("SamyGO", "busybox", "ChannelInfo", "hack_widget", "B_hack_widget");
$ForceSigned = array();
$ForceUnsigned = array();
$ForcePartners = array();
$ForceNoPartner = array();
$ForceInstall = array();

# load users widget list
# first visit or user made reset.
if(!file_exists($UserWidetList)){
	$xmlstr = '<?xml version="1.0" encoding="UTF-8"?><list></list>';
	$UserWList = new mySXE($xmlstr);
} else {
	$UserWList = new mySXE(file_get_contents($UserWidetList));
}
CError::LogStr("REQUEST_METHOD: ".$_SERVER['REQUEST_METHOD']);
CError::LogStr("Servers User WidgetList: ".$UserWList->asXML());

# users installed/uninstalled/list handling
if($_SERVER['REQUEST_METHOD'] != "GET"){ /* TODO: b-series submit full list, can't remember for c,d needs check... */
	$NewWidget = new mySXE($gInData);
	$UserWList->Merge($NewWidget);

	if($WhoAmI == "mystorage"){ /* e-series part, one widget per request */
		if($_SERVER['REQUEST_METHOD'] == "DELETE"){ 
			$id = $NewWidget->widget['id'];
			CError::LogStr("widget Delete request widget $id" . $UserWList->asXML());
			$UserWList->removeNodesByAttrib("widget", "id", array($NewWidget->widget['id']));
			CError::LogStr("widget Delete widget $id" . $UserWList->asXML());
		} // user widget synq requests
	}

	$UserWList->removeDups();
	CError::LogStr("Merged Result: ".$UserWList->asXML());
	$UserWList->saveXML($UserWidetList);
	header("HTTP/1.1 200 OK");
	SendPage($xml->asXML());
	#SendPage($respxml->asXML()); /* b-series response with error if samsungserver don't know the widget */
}

function ImportCustomWidgets($ids){
	$xmlstr = '<?xml version="1.0" encoding="UTF-8"?><rsp><list xmlns:media="http://search.yahoo.com/mrss"></list></rsp>';
	$res = new mySXE($xmlstr);
	foreach($ids as $id){
		$file = $_SERVER['DOCUMENT_ROOT'] . "/CustomWidgets/" . $id . ".xml";
		CError::LogStr("Check for widget $id");
		if(file_exists($file)){
			$widget = new mySXE(file_get_contents($file));
		CError::LogStr("Fount for widget $id" . $widget->asXML());
			$res->list->Merge($widget->list);
			CError::LogStr("combined with $id" . $res->asXML());
		} else {
			$widget = new mySXE($xmlstr);
			CError::LogStr("Empty widget $id" . $widget->asXML());
		}
	}
	return $res;
}
?>
