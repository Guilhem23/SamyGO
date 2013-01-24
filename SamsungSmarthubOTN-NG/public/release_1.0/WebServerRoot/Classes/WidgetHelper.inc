<?php
# some helpers for widget data manipulation/transormation used in different engines (Infolink/SmartHub/AppStore)

$KeepOriginalApps = array("400009002", "10120000000", "10110000001", "10120000099");  /* never remove the engines manager sh1 sh2 appstore */
$KeepOriginalApps[] = "11111000009"; /* Explore3D */
$KeepOriginalApps[] = "20121000003"; /* empWebBrowser */
$KeepOriginalApps[] = "10110001001"; /* empQs */
$KeepOriginalApps[] = "111199000764"; /* Camera */ 
$KeepOriginalApps[] = "10110001001"; /* MBRWidget */ 

$ReplaceOriginalApps = array("10120000000");  /* TODO: replace samsung app with other version/widget */

$CustomApps = array("SamyGO", "busybox", "ChannelInfo", "hack_widget", "B_hack_widget", "SamyGOemp");
$CustomApps = array("SamyGO", "SamyGOS");
$ForceSigned = array();		#TODO: required?
$ForceUnsigned = array();	#TODO: required?
$ForcePartners = array();	#TODO: required?
$ForceNoPartner = array();	#TODO: required?
$ForceInstall = array();	#TODO: required?

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
# TODO: POST handling for - /tvGateway/service -> -  Like to POST - 
if($_SERVER['REQUEST_METHOD'] != "GET"){ /* b-series submit full list, can't remember for c,d needs check... */
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
	if($WhoAmI != "downConfirm" && !is_empty($gUserName))
		$UserWList->saveXML($UserWidetList);
	header("HTTP/1.1 200 OK");
	SendPage($xml->asXML());
	#SendPage($respxml->asXML()); /* b-series response with error if samsungserver don't know the widget */
}

function ImportCustomWidgets($ids){
	$xmlstr = '<rsp><list/></rsp>';
	$res = new mySXE($xmlstr);

	foreach($ids as $id){
		$file = $_SERVER['DOCUMENT_ROOT'] . "/CustomWidgets/" . $id . ".xml";
		if(file_exists($file)){
			CError::LogStr("Found Widget description file $file");
			$widget = new mySXE(file_get_contents($file));
			$res->list->Merge($widget->list);
		} else {
			$widget = new mySXE($xmlstr);
			CError::LogStr("Missing Widget description file $file");
		}
	}
	return $res;
}

# in openapi/samsungapps/product/list?categoryID=0000008929.......
# for now just quickndirty method...
function WidgetToSA($from, $extras = NULL){
	global $QParts;
	$ContentsRoot = "http://" . $_SERVER['SERVER_NAME'] . "/CustomWidgets/MediaFiles/" . $from['id'] . DIRECTORY_SEPARATOR . $from['version'];

	$xmlstr = '<rsp><list/></rsp>';
	$res = new mySXE($xmlstr);

	$widget = $res->list->addChild("widget");
	$widget->addAttribute("id", $from['id']);
	$widget->addChild("version", $from['version']);
	$widget->addChild("published", $from['published']);
	$widget->addChild("updated", $from['updated']);
	$size = $widget->addChild("size");
	$size->addChild("compressed", $from->compression['size']);
	$size->addChild("uncompressed", $from['size']);
	$product = $widget->addChild("product");
#	$product->addChild("productCategoryId", $QParts['categoryID']); # dunno from where to get now... maybe simply from the request? categoryID=
	$product->addCDChild("productCategoryName", "SamyGO"); # dunno from where to get now... but shown on widget information screen
	$product->addChild("productCategoryId", "0000008931"); # dunno from where to get now... maybe simply from the request? categoryID=
	$product->addChild("productCode", "SamyGO"); # dunno from where to get now... normally numeric
	$product->addChild("productPrice", "0");
	$product->addCDChild("productTitle", $from->title);
	$product->addChild("productUpdatedDate", $from['updated']);
	$product->addCDChild("productVendor", $from->vendor);
	$product->addCDChild("productListInURI", "$ContentsRoot/picasa_85.png");
	$product->addCDChild("productListOutURI", "$ContentsRoot/picasa_95.png");
	$product->addCDChild("productListInURIHD", "$ContentsRoot/");
	$product->addCDChild("productListOutURIHD", "$ContentsRoot/");
	$product->addCDChild("screenShot1080Img", "$ContentsRoot/");
	$product->addCDChild("screenShot720LargeImg", "$ContentsRoot/");
	$product->addCDChild("screenShot720Img", "$ContentsRoot/");
	$product->addCDChild("screenShotImg", "$ContentsRoot/312x175album_list.jpg");
	$product->addCDChild("productThumbnailInURI", "$ContentsRoot/picasa_106.png");
	$product->addCDChild("productThumbnailOutURI", "$ContentsRoot/picasa_115.png");
	$product->addCDChild("productThumbnailInURIHD", "$ContentsRoot/");
	$product->addCDChild("productThumbnailOutURIHD", "$ContentsRoot/");

	return $res;
}
function WidgetToproductDetail($from, $extras = NULL){
	global $QParts;
	$ContentsRoot = "http://" . $_SERVER['SERVER_NAME'] . "/CustomWidgets/MediaFiles/" . $from['id'] . DIRECTORY_SEPARATOR . $from['version'];

	$xmlstr = '<productDetail/>';
	$res = new mySXE($xmlstr);

	$res->addChild("resultCode", 200);
	$res->addCDChild("resultMessage", "OK");
	$res->addChild("GMT", "+00:00");
	$res->addChild("couponID");
	$res->addChild("couponUse");
	$res->addCDChild("productNew", "N");
	$res->addCDChild("productHot", "N");
	$res->addCDChild("productEvent", "");
	$res->addCDChild("productListInURI", "$ContentsRoot/LIST_OFF_20121214065755748.png");
	$res->addCDChild("productListOutURI", "$ContentsRoot/LIST_ON_20121214065755748.png");
	$res->addCDChild("productThumbnailInURI", "$ContentsRoot/THUM_LIST_OFF_20121214065755748.png");
	$res->addCDChild("productThumbnailOutURI", "$ContentsRoot/THUM_LIST_ON_20121214065755748.png");
	$res->addCDChild("productListInURIHD", "$ContentsRoot/HD_LIST_OFF_20121214065755748.png");
	$res->addCDChild("productListOutURIHD", "$ContentsRoot/HD_LIST_ON_20121214065755748.png");
	$res->addCDChild("productThumbnailInURIHD", "$ContentsRoot/HD_THUM_LIST_OFF_20121214065755748.png");
	$res->addCDChild("productThumbnailOutURIHD", "$ContentsRoot/HD_THUM_LIST_ON_20121214065755748.png");
	$res->addChild("productCode", $from['id']); # hm
	$res->addCDChild("productTitle", $from->title);
	#$res->addChild("productPrice", "10.0");
	$res->addChild("productPrice", "0.0");
	$res->addChild("productRated", "99");
	$res->addCDChild("productReatedURI", "");
	$res->addCDChild("productURI", "");
	$res->addChild("recommand");
	$res->addChild("productUpdatedDate", $from['updated']);
	$res->addChild("productVersion", $from['version']);
	$res->addChild("widgetID", $from['id']);
	$res->addCDChild("productVendor", $from['id']);
	$res->addChild("downloadcount", "0");
	$res->addChild("trialWidgetId", "0");
#	$res->addCDChild("productCategoryId", "9999999999"); # hm
#	$res->addCDChild("productCategory", "SamyGO"); # hm
	$res->addCDChild("productCategoryId", "0000008931"); # hm
	$res->addCDChild("productCategory", "0000008931"); # hm
	$res->addCDChild("productDescription", $from->description);
	$res->addCDChild("productLanguage", "en");
	$res->addChild("productRealSize", $from['size']);
	$res->addChild("productZipSize", $from->compression['size']);
	$res->addChild("productRegisteredDate", $from->pubDate);
	$res->addChild("trialFlag", "N");
	$res->addChild("productDifficulty");
	$res->addChild("freeTrialCheck");
	$res->addCDChild("prevProductURI", "");
	$res->addCDChild("nextProductURI", "");
	$res->addCDChild("productDetailURI", "$ContentsRoot/");
	$res->addCDChild("screenShotImg1", "$ContentsRoot/SCREENSHOT_20121214065824343.jpg");
	$res->addCDChild("screenShotImg2", "$ContentsRoot/SCREENSHOT_20121214065824371.jpg");
	$res->addCDChild("screenShotImg3", "$ContentsRoot/SCREENSHOT_20121214065824401.jpg");
	$res->addCDChild("screenShotImg4", "$ContentsRoot/SCREENSHOT_20121214065824427.jpg");
	$res->addCDChild("screenShot720Img1", "$ContentsRoot/HD_SCREENSHOT_20121214065824343.jpg");
	$res->addCDChild("screenShot720Img2", "$ContentsRoot/HD_SCREENSHOT_20121214065824371.jpg");
	$res->addCDChild("screenShot720Img3", "$ContentsRoot/HD_SCREENSHOT_20121214065824401.jpg");
	$res->addCDChild("screenShot720Img4", "$ContentsRoot/HD_SCREENSHOT_20121214065824427.jpg");
	$res->addCDChild("screenShot720LargeImg1", "$ContentsRoot/HDL_SCREENSHOT_20121214065824343.jpg");
	$res->addCDChild("screenShot720LargeImg2", "$ContentsRoot/HDL_SCREENSHOT_20121214065824371.jpg");
	$res->addCDChild("screenShot720LargeImg3", "$ContentsRoot/HDL_SCREENSHOT_20121214065824401.jpg");
	$res->addCDChild("screenShot720LargeImg4", "$ContentsRoot/HDL_SCREENSHOT_20121214065824427.jpg");
	$res->addCDChild("screenShot1080Img1", "$ContentsRoot/FULL_HD_SCREENSHOT_20121214065824343.jpg");
	$res->addCDChild("screenShot1080Img2", "$ContentsRoot/FULL_HD_SCREENSHOT_20121214065824371.jpg");
	$res->addCDChild("screenShot1080Img3", "$ContentsRoot/FULL_HD_SCREENSHOT_20121214065824401.jpg");
	$res->addCDChild("screenShot1080Img4", "$ContentsRoot/FULL_HD_SCREENSHOT_20121214065824427.jpg");
	$res->addChild("contentId", $from['id']); # hm
	$res->addChild("lang", "ko");
	$res->addChild("sptEmail", "");

	return $res;
}
function ListForSA($from, $extras = NULL){
	$xmlstr = "<rsp><list/></rsp>";
	$res = new mySXE($xmlstr);

	foreach($from->list->children() as $widget){
		$tmp = WidgetToSA($widget, $extras);
		$res->list->Merge($tmp->list);
}
return $res;
}
?>
