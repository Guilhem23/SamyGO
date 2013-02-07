<?php
if($_SERVER['REQUEST_METHOD'] == "DELETE"){ 
	$id = $NewWidget->widget['id'];
	CError::LogStr("widget Delete request widget $id" . $UserWList->asXML());
	$UserWList->removeNodesByAttrib("widget", "id", array($NewWidget->widget['id']));
	CError::LogStr("widget Delete widget $id" . $UserWList->asXML());
	if($WhoAmI != "downConfirm" && !empty($gUserName))
		                $UserWList->saveXML($UserWidetList);
header("HTTP/1.1 200 OK");
SendPage($xml->asXML());
} // user widget synq requests

?>
