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
# TODO: POST handling for - /tvGateway/service -> -  Like to POST - 
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
	$xmlstr = '<rsp><list/></rsp>';
	$res = new mySXE($xmlstr);

	foreach($ids as $id){
		$file = $_SERVER['DOCUMENT_ROOT'] . "/CustomWidgets/" . $id . ".xml";
		if(file_exists($file)){
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

	$xmlstr = '<rsp><list/></rsp>';
	$res = new mySXE($xmlstr);

	$widget = $res->list->addChild("widget");
	$widget->addAttribute("id", $from['id']);
	$widget->addChild("version", $from['version']);
	$widget->addChild("published", $from['published']);
	$widget->addChild("updated", $from['updated']);
	$size = $widget->addChild("size");
	$size->addChild("compressed", $from->ompression['size']);
	$size->addChild("uncompressed", $from['size']);
	$product = $widget->addChild("product");
	$product->addChild("productCategoryId", $QParts['categoryID']); # dunno from where to get now... maybe simply from the request? categoryID=
	$product->addCDChild("productCategoryName", "SamyGO");
	$product->addChild("productCode", "SamyGO"); # dunno from where to get now... normally numeric
	$product->addChild("productPrice", "0");
	$product->addCDChild("productTitle", $from->title);
	$product->addChild("productUpdatedDate", $from['updated']);
	$product->addCDChild("productVendor", $from->vendor);
	$product->addCDChild("productListInURI", "http://tv.samsungapps.com/tvstore/contents/20100224205034/picasa_85.png");
	$product->addCDChild("productListOutURI", "http://tv.samsungapps.com/tvstore/contents/20100224205034/picasa_95.png");
	$product->addCDChild("productListInURIHD", "http://tv.samsungapps.com/tvstore/contents/");
	$product->addCDChild("productListOutURIHD", "http://tv.samsungapps.com/tvstore/contents/");
	$product->addCDChild("screenShot1080Img", "http://tv.samsungapps.com/tvstore/contents/");
	$product->addCDChild("screenShot720LargeImg", "http://tv.samsungapps.com/tvstore/contents/");
	$product->addCDChild("screenShot720Img", "http://tv.samsungapps.com/tvstore/contents/");
	$product->addCDChild("screenShotImg", "http://tv.samsungapps.com/tvstore/contents/20100225005129/312x175album_list.jpg");
	$product->addCDChild("productThumbnailInURI", "http://tv.samsungapps.com/tvstore/contents/20100224205034/picasa_106.png");
	$product->addCDChild("productThumbnailOutURI", "http://tv.samsungapps.com/tvstore/contents/20100224205034/picasa_115.png");
	$product->addCDChild("productThumbnailInURIHD", "http://tv.samsungapps.com/tvstore/contents/");
	$product->addCDChild("productThumbnailOutURIHD", "http://tv.samsungapps.com/tvstore/contents/");

	return $res;
}
function WidgetToproductDetail($from, $extras = NULL){
	global $QParts;

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
	$res->addCDChild("productListInURI", "http://tv.samsungapps.com/tvstore/contents/111299002371/LIST_OFF_20121214065755748.png");
	$res->addCDChild("productListOutURI", "http://tv.samsungapps.com/tvstore/contents/111299002371/LIST_ON_20121214065755748.png");
	$res->addCDChild("productThumbnailInURI", "http://tv.samsungapps.com/tvstore/contents/111299002371/THUM_LIST_OFF_20121214065755748.png");
	$res->addCDChild("productThumbnailOutURI", "http://tv.samsungapps.com/tvstore/contents/111299002371/THUM_LIST_ON_20121214065755748.png");
	$res->addCDChild("productListInURIHD", "http://tv.samsungapps.com/tvstore/contents/111299002371/HD_LIST_OFF_20121214065755748.png");
	$res->addCDChild("productListOutURIHD", "http://tv.samsungapps.com/tvstore/contents/111299002371/HD_LIST_ON_20121214065755748.png");
	$res->addCDChild("productThumbnailInURIHD", "http://tv.samsungapps.com/tvstore/contents/111299002371/HD_THUM_LIST_OFF_20121214065755748.png");
	$res->addCDChild("productThumbnailOutURIHD", "http://tv.samsungapps.com/tvstore/contents/111299002371/HD_THUM_LIST_ON_20121214065755748.png");
	$res->addChild("productCode", $from['id']); # hm
	$res->addCDChild("productTitle", $from->title);
	$res->addChild("productPrice", "0.0");
	$res->addChild("productRated", "0");
	$res->addCDChild("productReatedURI", "");
	$res->addCDChild("productURI", "");
	$res->addChild("recommand");
	$res->addChild("productUpdatedDate", $from['updated']);
	$res->addChild("productVersion", $from['version']);
	$res->addChild("widgetID", $from['id']);
	$res->addCDChild("productVendor", $from['id']);
	$res->addChild("downloadcount", "0");
	$res->addChild("trialWidgetId", "0");
	$res->addCDChild("productCategoryId", "9999999999"); # hm
	$res->addCDChild("productCategory", "SamyGO"); # hm
	$res->addCDChild("productDescription", $from->description);
	$res->addCDChild("productLanguage", "en");
	$res->addChild("productRealSize", $from['size']);
	$res->addChild("productZipSize", $from->ompression['size']);
	$res->addChild("productRegisteredDate", $from->pubDate);
	$res->addChild("trialFlag", "N");
	$res->addChild("productDifficulty");
	$res->addChild("freeTrialCheck");
	$res->addCDChild("prevProductURI", "");
	$res->addCDChild("nextProductURI", "");
	$res->addCDChild("productDetailURI", "http://tv.samsungapps.com/tvstore/contents/");
	$res->addCDChild("screenShotImg1", "http://tv.samsungapps.com/tvstore/contents/111299002371/SCREENSHOT_20121214065824343.jpg");
	$res->addCDChild("screenShotImg2", "http://tv.samsungapps.com/tvstore/contents/111299002371/SCREENSHOT_20121214065824371.jpg");
	$res->addCDChild("screenShotImg3", "http://tv.samsungapps.com/tvstore/contents/111299002371/SCREENSHOT_20121214065824401.jpg");
	$res->addCDChild("screenShotImg4", "http://tv.samsungapps.com/tvstore/contents/111299002371/SCREENSHOT_20121214065824427.jpg");
	$res->addCDChild("screenShot720Img1", "http://tv.samsungapps.com/tvstore/contents/111299002371/HD_SCREENSHOT_20121214065824343.jpg");
	$res->addCDChild("screenShot720Img2", "http://tv.samsungapps.com/tvstore/contents/111299002371/HD_SCREENSHOT_20121214065824371.jpg");
	$res->addCDChild("screenShot720Img3", "http://tv.samsungapps.com/tvstore/contents/111299002371/HD_SCREENSHOT_20121214065824401.jpg");
	$res->addCDChild("screenShot720Img4", "http://tv.samsungapps.com/tvstore/contents/111299002371/HD_SCREENSHOT_20121214065824427.jpg");
	$res->addCDChild("screenShot720LargeImg1", "http://tv.samsungapps.com/tvstore/contents/111299002371/HDL_SCREENSHOT_20121214065824343.jpg");
	$res->addCDChild("screenShot720LargeImg2", "http://tv.samsungapps.com/tvstore/contents/111299002371/HDL_SCREENSHOT_20121214065824371.jpg");
	$res->addCDChild("screenShot720LargeImg3", "http://tv.samsungapps.com/tvstore/contents/111299002371/HDL_SCREENSHOT_20121214065824401.jpg");
	$res->addCDChild("screenShot720LargeImg4", "http://tv.samsungapps.com/tvstore/contents/111299002371/HDL_SCREENSHOT_20121214065824427.jpg");
	$res->addCDChild("screenShot1080Img1", "http://tv.samsungapps.com/tvstore/contents/111299002371/FULL_HD_SCREENSHOT_20121214065824343.jpg");
	$res->addCDChild("screenShot1080Img2", "http://tv.samsungapps.com/tvstore/contents/111299002371/FULL_HD_SCREENSHOT_20121214065824371.jpg");
	$res->addCDChild("screenShot1080Img3", "http://tv.samsungapps.com/tvstore/contents/111299002371/FULL_HD_SCREENSHOT_20121214065824401.jpg");
	$res->addCDChild("screenShot1080Img4", "http://tv.samsungapps.com/tvstore/contents/111299002371/FULL_HD_SCREENSHOT_20121214065824427.jpg");
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
		$res->list->Merge($tmp->list, true);
}
return $res;
}
?>
