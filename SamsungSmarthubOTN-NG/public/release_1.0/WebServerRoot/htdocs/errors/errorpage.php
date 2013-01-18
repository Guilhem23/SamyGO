<?php
require_once('SimpleMidleManHelpers.inc');

$Mtime = time() - 3600 * 4 * 24;
CError::LogStr("##################### ".print_r($_SERVER, 1)." #################: " . gmdate("D, d M Y H:i:s",  $Mtime ) . " GMT");

// Get Variables
$error_code = $_SERVER['REDIRECT_STATUS'];

if (isset($QParts['force'])) {
	$error_code = $QParts['force'];
}

$error_num = str_replace('_', '.', $error_code);
$error_doc_name = str_replace('_', '-', $error_code);

$error_msg = array(
	"100" => "Invalid Parameter",
	"101" => "Invalid AppKey", # UserID Conflict/Unique UserID Required
	"102" => "Invalid Firmware Code",
	"103" => "Invalid DUID",
	"104" => "Invalid Session Token",
	"105" => "Permission denied", /* err code="105" msg="Wrong User Password */
"106" => "Not implemented",
"107" => "Invalid Service Request",
"108" => "Unknown IP Location",
"109" => "Invalid Model ID",
"110" => "Unknown Country Code",
"111" => "Database Access Error",
"116" => "Invalid Service Request", /* <err code="116" msg="Invalid Service Request"/> from sso.samsung server */
"200" => "Invalid User ID",
"300" => "Invalid Widget ID",
"301" => "Invalid Channel ID",
"309" => "Not exist Product Id", /* tvstoregw >309</resultCode><resultMessage><![CDATA[Not exist Product Id]] */
"313" => "Not Exist Model",
"400" => "Server Under Construction",
"404" => "Page not found!",
/* original HTTP codes */
"401" => "Unauthorized",
"402" => "Payment Required",
"403" => "Forbidden",
"500" => "Internal Server Error",
"501" => "Not Implemented", /* http://www.w3.org/Protocols/rfc2616/rfc2616-sec10.html */
/* IIS HTTP http://support.microsoft.com/kb/318380/ */
"100" => "Continue", /* (Fortfahren) */
"101" => "Switching protocols", /* (Protokolle werden gewechselt) */

"200" => "OK. The client request has succeeded", /* (OK. Die Clientanforderung war erfolgreich) */
"201" => "Created", /* (Erstellt) */
"202" => "Accepted", /* (Akzeptiert) */
"203" => "Non-authoritative information", /* (Nicht autorisierte Information) */
"204" => "No content", /* (Kein Inhalt) */
"205" => "Reset content", /* (Inhalt zurücksetzen) */
"206" => "Partial content", /* (Teilinhalt) */
"207" => "Multi-Status, WebDAV", /* (Multistatus, WebDAV) */

"301" => "Moved Permanently", /* (Dauerhaft verschoben) */
"302" => "Object moved", /* (Objekt verschoben) */
"304" => "Not modified", /* (Nicht geändert) */
"307" => "Temporary redirect", /* (Temporäre Umleitung) */

"400" => "Bad request", /* (Ungültige Anforderung) */
"401" => "Access denied", /* (Zugriff verweigert) */ 
"401_1" => "Logon failed", /* (Anmeldung fehlgeschlagen) */
"401_2" => "Logon failed due to server configuration", /* (Anmeldung aufgrund der Serverkonfiguration fehlgeschlagen) */
"401_3" => "Unauthorized due to ACL on resource", /* (Zugriff auf die Ressource wurde durch eine ACL verweigert) */
"401_4" => "Authorization failed by filter", /* (Autorisierung aufgrund von Filter fehlgeschlagen) */
"401_5" => "Authorization failed by ISAPI/CGI application", /* (Autorisierung aufgrund von ISAPI/CGI-Anwendung fehlgeschlagen) */
"401_7" => "Access denied by URL authorization policy on the Web server", /* (Zugriff aufgrund von URL-Autorisierungsrichtlinie auf dem Webserver verweigert) */ 
"403" => "Forbidden", /* (Verboten) */ 
"403_1" => "Execute access forbidden", /* (Ausführungszugriff verboten) */
"403_2" => "Read access forbidden", /* (Lesezugriff verboten) */
"403_3" => "Write access forbidden", /* (Schreibzugriff verboten) */
"403_4" => "SSL required", /* (SSL erforderlich) */
"403_5" => "SSL 128 required", /* (SSL 128 erforderlich) */
"403_6" => "IP address rejected", /* (IP-Adresse zurückgewiesen) */
"403_7" => "Client certificate required", /* (Clientzertifikat erforderlich) */
"403_8" => "Site access denied", /* (Sitezugriff verweigert) */
"403_9" => "Too many users", /* (zu viele Benutzer) */
"403_10" => "Invalid configuration", /* (Ungültige Konfiguration) */
"403_11" => "Password change", /* (Kennwortänderung) */
"403_12" => "Mapper denied access", /* (Zugriff durch Mapper verweigert) */
"403_13" => "Client certificate revoked", /* (Clientzertifikat widerrufen) */
"403_14" => "Directory listing denied", /* (Auflistung in Verzeichnis verweigert) */
"403_15" => "Client Access Licenses exceeded", /* (Clientzugriffslizenzen überschritten) */
"403_16" => "Client certificate is untrusted or invalid", /* (Clientzertifikat nicht vertrauenswürdig oder ungültig) */
"403_17" => "Client certificate has expired or is not yet valid", /* (Clientzertifikat abgelaufen oder noch nicht gültig) */
"403_18" => "Cannot execute requested URL in the current application pool", /* (Angeforderte URL kann im aktuellen Anwendungspool nicht ausgeführt werden) */ 
"403_19" => "Cannot execute CGIs for the client in this application pool", /* (CGIs für den Client können in diesem Anwendungspool nicht ausgeführt werden) */ 
"403_20" => "Passport logon failed", /* (Passport-Anmeldung fehlgeschlagen) */ 
"404" => "Not found", /* (Nicht gefunden) */
"404_0" => "(None) - File or directory not found", /* (Datei oder Verzeichnis nicht gefunden) */
"404_1" => "Web site not accessible on the requested port", /* (Kein Zugriff auf Website auf angefordertem Port) */
"404_2" => "Web service extension lockdown policy prevents this request", /* (Anforderung wird durch Lockdown-Richtlinie der Webdiensterweiterung verhindert) */
"404_3" => "MIME map policy prevents this request", /* (Anforderung wird durch MIME-Verzeichnisrichtlinie verhindert) */
"405" => "HTTP verb used to access this page is not allowed, method not allowed", /* (Für Zugriff auf diese Seite verwendetes HTTP-Verb ist nicht zulässig, Methode unzulässig) */
"406" => "Client browser does not accept the MIME type of the requested page", /* (Clientbrowser akzeptiert den MIME-Typ der angeforderten Seite nicht) */
"407" => "Proxy authentication required", /* (Proxyauthentifizierung erforderlich) */
"412" => "Precondition failed", /* (Vorbedingung fehlgeschlagen) */
"413" => "Request entity too large", /* (Anforderungseinheit ist zu groß) */
"414" => "Request-URI too long", /* (Anforderungs-URI ist zu lang) */
"415" => "Unsupported media type", /* (Nicht unterstützter Medientyp) */
"416" => "Requested range not satisfiable", /* (Angeforderter Bereich kann nicht erfüllt werden) */
"417" => "Execution failed", /* (Ausführung fehlgeschlagen) */
"423" => "Locked error", /* (Fehler durch Sperre) */

"500" => "Internal server error", /* (Interner Serverfehler) */
"500_12" => "Application is busy restarting on the Web server", /* (Anwendung wird gerade auf dem Webserver neu gestartet) */
"500_13" => "Web server is too busy", /* (Webserver ist ausgelastet) */
"500_15" => "Direct requests for Global.asa are not allowed", /* (Direkte Anforderungen für -Global.asa- nicht zulässig) */
"500_16" => "UNC authorization credentials incorrect", /* (UNC-Anmeldeinformationen falsch) */ 
"500_18" => "URL authorization store cannot be opened", /* (URL-Autorisierungsspeicher kann nicht geöffnet werden) */ 
"500_19" => "Data for this file is configured improperly in the metabase", /* (Daten für diese Datei in der Metabasis fehlerhaft konfiguriert) */
"500_100" => "Internal ASP error", /* (Interner ASP-Fehler) */
"501" => "Header values specify a configuration that is not implemented", /* (Headerwerte geben nicht implementierte Konfiguration an) */
"502" => "Web server received an invalid response while acting as a gateway or proxy", /* (Der Webserver hat, während er als Gateway oder Proxyserver fungierte, eine ungültige Antwort empfangen) */
"502_1" => "CGI application timeout", /* (Zeitüberschreitung der CGI-Anwendung) */
"502_2" => "Error in CGI application", /* (Fehler in CGI-Anwendung) */
"503" => "Service unavailable", /* (Dienst nicht verfügbar) */ 
"504" => "Gateway timeout", /* (Zeitüberschreitung des Gateways) */
"505" => "HTTP version not supported" /* (HTTP-Version wird nicht unterstützt) */
);

sxml_change_attr($xml, "stat", "fail");
$err1 = $xml->addChild("err");
$err1->addAttribute("code", $error_code);
$err1->addAttribute("msg", $error_msg[$error_code]);
$desc1 = $xml->addChild("desc", GeoInfoOTNSG());

if($error_code == 400){
	$notice1 = $xml->addChild("notice"); /* for SmartHub 400-error startdate="2010-05-10T15:00:00+09:00" enddate="2010-05-10T16:00:00+09:00" */ 
$notice1->addAttribute("startdate", get_KoreanHour(0));
$notice1->addAttribute("enddate", get_KoreanHour(24 * 7));
}

$dom = new DOMDocument('1.0');
$dom->preserveWhiteSpace = false;
$dom->formatOutput = true;
$dom->loadXML($xml->asXML());

header("X-Powered-By: ASP.NET", true);
header("Accept-Ranges: bytes", true);
header("X-Powered-By: Servlet/2.5 JSP/2.1", true);
header("Connection: Close", true);
header("Status: 302 Found", true);
// header("Transfer-Encoding: chunked", true);
/* The flush is necessary. Otherwise, Content-Length header will be generated. (we don't write buggy software as samsung do!!!) */
// flush();

if($error_code == 503){
	header("Content-Type: text/html", true);
	echo "<TITLE>ISAPI plug-in Error Message</TITLE><H2>Message from the ISAPI plugin:</H2><P>No backend server available for connection: timed out after 12 seconds or idempotent set to OFF.
		<HR><I>Build date/time: ".gmdate("d M Y H:i:s", filemtime(__FILE__))." GMT<I><P><HR><B>Change Number:</B> <I>".$_SERVER['UNIQUE_ID']."<BR>";
	die();
}

if(!isset($_SERVER['HTTP_TOKEN']) && !isset($_SERVER['HTTP_DUID'])){
	header("Content-Type: text/html", true);
	$ResPage = '<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01//EN" "http://www.w3.org/TR/html4/strict.dtd">
		<HTML><HEAD><TITLE>'.$error_msg[$error_code].'</TITLE>
		<META HTTP-EQUIV="Content-Type" Content="text/html; charset=Windows-1252">
		<STYLE type="text/css">
		BODY { font: 8pt/12pt verdana }
		H1 { font: 13pt/15pt verdana }
		H2 { font: 8pt/12pt verdana }
		A:link { color: red }
		A:visited { color: maroon }
		</STYLE>
		</HEAD><BODY><TABLE width=500 border=0 cellspacing=10><TR><TD>

		<h1>'.SG_EMU.' - '.SG_EMU_VER.'<br></h1>
		<h1>'.$error_msg[$error_code].'</h1>
		<h3>Region: '.GeoInfoOTNSG().'</h3>
		The page you are looking for might have been removed, had its name changed, or is temporarily unavailable.
		<hr>';
	if(file_exists("custerr/de-DE/$error_doc_name.htm")){
		$ResPage = file_exists("custerr/".BrowserLang()."/$error_doc_name.htm") ? file_get_contents("custerr/".BrowserLang()."/$error_doc_name.htm") : file_get_contents("custerr/de-DE/$error_doc_name.htm");
	} else {
		$ResPage = '<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01//EN" "http://www.w3.org/TR/html4/strict.dtd">
			<HTML><HEAD><TITLE>'.$error_msg[$error_code].'</TITLE>
			<META HTTP-EQUIV="Content-Type" Content="text/html; charset=Windows-1252">
			<STYLE type="text/css">
			BODY { font: 8pt/12pt verdana }
			H1 { font: 13pt/15pt verdana }
			H2 { font: 8pt/12pt verdana }
			A:link { color: red }
			A:visited { color: maroon }
			</STYLE>
			</HEAD><BODY><TABLE width=500 border=0 cellspacing=10><TR><TD>

			<h1>'.SG_EMU.' - '.SG_EMU_VER.'<br></h1>
			<h1>'.$error_msg[$error_code].'</h1>
			<h3>Region: '.GeoInfoOTNSG().'</h3>
			The page you are looking for might have been removed, had its name changed, or is temporarily unavailable.
			<hr>
			<p>Please try the following:</p>
			<ul>
			<li>Make sure that the Web site address displayed in the address bar of your browser is spelled and formatted correctly.</li>

			<li>If you reached this page by clicking a link, contact
			the Web site administrator to alert them that the link is incorrectly formatted.
			</li>
			<li>Click the <a href="javascript:history.back(1)">Back</a> button to try another link.</li>
			</ul>
			<h2>HTTP Error '.$error_num.' - '.$error_msg[$error_code].'.<br>Internet Information Services (IIS) ['.SERVER_NAME.']</h2>
			<hr>
			<p>Technical Information (for support personnel)</p>
			<ul>
			<li>Go to <a href="http://go.microsoft.com/fwlink/?linkid=8180">Microsoft Product Support Services</a> and perform a title search for the words <b>HTTP</b> and <b>'.str_replace("_", ".", $error_code).'</b>.</li>

			<li>Open <b>IIS Help</b>, which is accessible in IIS Manager (inetmgr),
				and search for topics titled <b>Web Site Setup</b>, <b>Common Administrative Tasks</b>, and <b>About Custom Error Messages</b>.</li>
				</ul>

				</TD></TR></TABLE></BODY></HTML>';   
	}
	SendPage($ResPage);

} else {
	header("Content-Type: text/xml", true);
	//	SendUnChunked($dom->saveXML());
	SendPage($dom->saveXML());
}
?>
