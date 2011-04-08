/* vi: set sw=4 ts=4: */
// for upnp media/device control see: /mtd_rwarea/dlna_web_root/dmr/SamsungMRDesc.xml
//
// http://sourceforge.net/apps/phpbb/samygo/viewtopic.php?f=1&t=250#p4025
// POST /upnp/control/RenderingControl1 HTTP/1.1
// Host: 192.168.0.10:52235
// SOAPAction: "urn:schemas-upnp-org:service:RenderingControl:1#SetMute"
// Accept-Language: LC-ctype=en-us;q=1, LC-ctype=en;q=0.5
// Content-Type: text/xml; charset=utf-8
// Content-Length: 335
//
// <?xml version="1.0"?>
// <s:Envelope xmlns:s="http://schemas.xmlsoap.org/soap/envelope/" s:encodingStyle="http://schemas.xmlsoap.org/soap/encoding/">
// <s:Body><u:SetMute xmlns:u="urn:schemas-upnp-org:service:RenderingControl:1">
// <InstanceID>0</InstanceID>
// <Channel>Master</Channel>
// <DesiredMute>0</DesiredMute></u:SetMute>
// </s:Body>
// </s:Envelope>
//
package com.samygo.samples.arris.SmessenGO;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;

import java.io.IOException;
import java.io.BufferedReader;
import java.io.UnsupportedEncodingException;
import java.io.StringWriter;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.util.Date;
import java.text.SimpleDateFormat;
import java.lang.StringBuilder;

import org.apache.http.HttpEntity;
import org.apache.http.entity.StringEntity;
import org.apache.http.HttpResponse;
import org.apache.http.client.methods.HttpPost;
import org.apache.http.impl.client.DefaultHttpClient;
import org.apache.http.params.HttpParams;
import org.apache.http.params.BasicHttpParams;
import org.apache.http.params.CoreConnectionPNames;

import org.xmlpull.v1.XmlSerializer;

import android.content.SharedPreferences;
import android.preference.PreferenceManager;
import android.provider.ContactsContract.PhoneLookup;
// import com.android.calendar;
// import android.provider.Calendar.*;
import android.database.Cursor;
import android.net.Uri;
import android.os.Bundle;
import android.util.Log;
import android.util.Xml;
// need other fix for some chars in message body?
import android.text.TextUtils;

public class SendToSamy extends BroadcastReceiver {
	private static final String TAG = "SendToSamy: ";
	private String Category;
	private String Action;
	private String Namespace;
	private String MyName;
	private String MyNum;
	private String Number;
	private String Name;
	private String Message;
	private Boolean Mute;
	private Long StartD;
	private Long EndD;

	private static final String METHOD_NAME = "AddMessage"; // RemoveMessage, SetMute,  
	private static final String NAMESPACE = "urn:samsung.com:service:MessageBoxService:1";
	private static final String UPNP_NAMESPACE = "urn:schemas-upnp-org:service:RenderingControl:1";
	private static final String URL = ":52235/PMR/control/MessageBoxService";
	private static final String UPNP_URL = ":52235/upnp/control/RenderingControl1";
	private static final String SDURL = ":52235/pmr/MessageBoxService.xml";

	private String SOAPRequestXML;
	private DefaultHttpClient httpclient;
	private InputStream content;
	private HttpPost httppost;
	private Date date;

	static SimpleDateFormat DATE = new SimpleDateFormat("yyyy-MM-dd");
	static SimpleDateFormat TIME = new SimpleDateFormat("HH:mm:ss");

	@Override
		public void onReceive(Context context, Intent intent) {
			Bundle extras = intent.getExtras();
			date = new Date();
			SharedPreferences prefMgr = PreferenceManager.getDefaultSharedPreferences(context);

			// TODO: read config, and check wlan and samy
			// 	get MyNum and MyName from phone settings?
			if (extras != null) {
				MyNum = "911";
				MyName = "SamyGO";
				Category = extras.getString("category");
				Action = METHOD_NAME;
				Namespace = NAMESPACE;
				Number = extras.getString("caller");
				Name = getDisplayName(context, Number);
				Message = extras.getString("data");
				Mute = true;

				if (Category.startsWith("Schedule")){
					StartD = extras.getLong("beginTime");
					EndD = extras.getLong("endTime");
					Message = "Alert Count: " + extras.getInt("android.intent.extra.ALARM_COUNT");
					Log.i(TAG, "Reminder: " + extras.toString());
				} else if (Category.startsWith("SM")){
					Log.i(TAG, "SMS: " + extras.toString());
				} else if (Category.startsWith("Incoming")){
					Log.i(TAG, "Call State Changed: " + extras.toString());
					// TODO: check setting for mute on call
					/* if extras.getString("state") == "RINGING" -> new incomming call
					   on extras.getString("state") == "IDLE" -> end of call
					   on extras.getString("state") == "OFFHOOK" -> accepted the call or 
					   new outgoing call -> remove message from tv! */
					if (extras.getString("state").startsWith("ID")){
						// unmute tv and exit?
						Action = "SetMute";
						Category = "UPNP";
						Mute = false;
						Namespace = UPNP_NAMESPACE;
						parametrosHttp("http://"+prefMgr.getString("SecondEditTextPref", "10.0.0.1")+UPNP_URL);
						go();
						return;
					} else if (extras.getString("state").startsWith("OFFH")){
						// remove message? else it pop up after call ended
						Action = "RemoveMessage";
						Category = "Incoming Call";
						parametrosHttp("http://"+prefMgr.getString("SecondEditTextPref", "10.0.0.1")+URL);
						go();
						Action = "SetMute";
						Category = "UPNP";
						Namespace = UPNP_NAMESPACE;
						parametrosHttp("http://"+prefMgr.getString("SecondEditTextPref", "10.0.0.1")+UPNP_URL);
						go();
						return;
					}
				} else {
					// reserved/unhandled
					Log.i(TAG, "Incomming Call:");
				}
			}
			// SecondEditTextPref or where you store tv's ip
			parametrosHttp("http://"+prefMgr.getString("SecondEditTextPref", "10.0.0.1")+URL);
			go();
		}

	private String writeXml(){
		XmlSerializer serializer = Xml.newSerializer();
		StringWriter writer = new StringWriter();
		try {
			serializer.setOutput(writer);
			serializer.startDocument("UTF-8", false);
			serializer.setPrefix("s", "http://schemas.xmlsoap.org/soap/envelope/");
			serializer.startTag("", "s:Envelope");
			serializer.attribute("", "s:encodingStyle", "http://schemas.xmlsoap.org/soap/encoding/");
			serializer.startTag("", "s:Body");
			serializer.setPrefix("u", Namespace);
			serializer.startTag("", "u:" + Action);
			if (Category.startsWith("UPN")){
				serializer.startTag("", "InstanceID");
				serializer.text("0");
				serializer.endTag("", "InstanceID");
				serializer.startTag("", "Channel");
				serializer.text("Master");
				serializer.endTag("", "Channel");
				serializer.startTag("", "DesiredMute");
				serializer.text(Mute.toString());
				serializer.endTag("", "DesiredMute");
			} else {
				serializer.startTag("", "MessageType");
				serializer.text("text/xml");
				serializer.endTag("", "MessageType");
				serializer.startTag("", "MessageID");
				serializer.text("can be anything");
				serializer.endTag("", "MessageID");
				serializer.startTag("", "Message");
				serializer.text("<Category>" + Category + "</Category>");
				// where to put ExpireState, here or outside of Message?
				serializer.text("<DisplayType>Maximum</DisplayType>");
				if (Category.startsWith("SM"))
				{
					serializer.text("<ReceiveTime><Date>" + getDate() + "</Date><Time>" + getTime() + "</Time></ReceiveTime>");
					serializer.text("<Receiver><Number>" + MyNum + "</Number><Name>" + MyName + "</Name></Receiver>");
					serializer.text("<Sender><Number>" + Number + "</Number><Name>" + Name + "</Name></Sender>");
					serializer.text("<Body>" + TextUtils.htmlEncode(Message) + "</Body>");
				} else if (Category.startsWith("Incoming"))
				{
					serializer.text("<CallTime><Date>" + getDate() + "</Date><Time>" + getTime() + "</Time></CallTime>");
					serializer.text("<Callee><Number>" + MyNum + "</Number><Name>" + MyName + "</Name></Callee>");
					serializer.text("<Caller><Number>" + Number + "</Number><Name>" + Name + "</Name></Caller>");
				} else if (Category.startsWith("Schedule"))
				{
					serializer.text("<StartTime><Date>" + DATE.format(StartD) + "</Date><Time>" + TIME.format(StartD) + "</Time></StartTime>");
					serializer.text("<Owner><Number>" + MyNum + "</Number><Name>" + MyName + "</Name></Owner>");
					serializer.text("<Subject>SamyGO Reminder</Subject>");
					serializer.text("<EndTime><Date>" + DATE.format(EndD) + "</Date><Time>" + TIME.format(EndD) + "</Time></EndTime>");
					serializer.text("<Location></Location>");
					serializer.text("<Body>" + TextUtils.htmlEncode(Message) + "</Body>");
				} else {
					// reserved
				}
				serializer.endTag("", "Message");
			}
			serializer.endTag("", "u:" + Action);
			serializer.endTag("", "s:Body");
			serializer.endTag("", "s:Envelope");
			serializer.endDocument();
			return writer.toString();
		} catch (Exception e) {
			throw new RuntimeException(e);
		} 
	}

	public void parametrosHttp(String url) {
		this.httpclient = new DefaultHttpClient();
		this.httppost = new HttpPost(url);
		this.SOAPRequestXML = new String();

		HttpParams params = new BasicHttpParams();                     
		params.setIntParameter(CoreConnectionPNames.SO_TIMEOUT, 5000);
		params.setIntParameter(CoreConnectionPNames.CONNECTION_TIMEOUT, 10000);
		this.httppost.setParams(params);

		SOAPRequestXML = writeXml();
	}

	// http://mycontainer.wordpress.com/2008/01/15/java-convert-inputstream-to-string/
	private String inputStreamToString(InputStream in) throws IOException {
		BufferedReader bufferedReader = new BufferedReader(new InputStreamReader(in), 8*1024);
		StringBuilder stringBuilder = new StringBuilder();
		String line = null;

		while ((line = bufferedReader.readLine()) != null) {
			stringBuilder.append(line + "\n");
		}

		bufferedReader.close();
		return stringBuilder.toString();
	}

	public int go(){
		try {
			StringEntity se = new StringEntity(SOAPRequestXML,"UTF-8");
			se.setContentType("text/xml");

			httppost.setHeader("SOAPACTION", Namespace + "#" + Action);
			/** Assign the POST data to the entity */
			httppost.setEntity(se);

			/** Perform the actual HTTP POST */
			HttpResponse response = httpclient.execute(httppost);
			HttpEntity entity = response.getEntity();
			content = entity.getContent();
			Log.i(TAG, "HTTP Answer: " + inputStreamToString(content));
		} catch (UnsupportedEncodingException uee) {
			// Woops
		} catch (IOException ioe) {
			// Woops
		} catch(IllegalStateException ise) {
			// woops
		}
		return 1;
	}

	public String getDisplayName(Context context, String caller) {
		String DpName = context.getString(android.R.string.unknownName);
		Uri uri = Uri.withAppendedPath(PhoneLookup.CONTENT_FILTER_URI, 
				Uri.encode(caller));
		Cursor cur = context.getContentResolver().query(uri,
				new String[] { PhoneLookup.DISPLAY_NAME },
				null, null, null );
		if ( cur.moveToFirst() ) {
			int nameIdx = cur.getColumnIndex(PhoneLookup.DISPLAY_NAME);
			DpName = cur.getString(nameIdx);
		}
		cur.close();
		return DpName;
	}

	public String getDate() {
		return DATE.format(date);
	}
	public String getTime() {
		return TIME.format(date);
	}
}
