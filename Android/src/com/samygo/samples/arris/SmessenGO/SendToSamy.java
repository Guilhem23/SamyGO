package com.samygo.samples.arris.SmessenGO;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;

import java.io.IOException;
import java.io.UnsupportedEncodingException;
import java.io.StringWriter;
import java.util.Date;
import java.text.SimpleDateFormat;

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
	private String MyName;
	private String MyNum;
	private String Number;
	private String Name;
	private String Message;
	private Long StartD;
	private Long EndD;

	private static final String SOAP_ACTION = "urn:samsung.com:service:MessageBoxService:1#AddMessage";
	private static final String METHOD_NAME = "AddMessage";
	private static final String NAMESPACE = "urn:samsung.com:service:MessageBoxService:1";
	private static final String URL = ":52235/PMR/control/MessageBoxService";
	private static final String SDURL = ":52235/pmr/MessageBoxService.xml";

	private String SOAPRequestXML;
	private DefaultHttpClient httpclient;
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
				Number = extras.getString("caller");
				Name = getDisplayName(context, Number);
				Message = extras.getString("data");

				if (Category.startsWith("Schedule")){
					StartD = extras.getLong("beginTime");
					EndD = extras.getLong("endTime");
					Message = "Alert Count: " + extras.getInt("android.intent.extra.ALARM_COUNT");
					Log.i(TAG, "Alert: " + extras.toString());
				} else if (Category.startsWith("SM")){
					Log.i(TAG, "SMS: " + extras.toString());
				} else {
					// TODO: check setting for mute on call
					// if extras.getString("state") == "RINGING" -> start of call
					// on extras.getString("state") == "IDLE" -> end of call
					Log.i(TAG, "Call: " + extras.toString());
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
			serializer.setPrefix("u", NAMESPACE);
			serializer.startTag("", "u:AddMessage");
			serializer.startTag("", "MessageType");
			serializer.text("text/xml");
			serializer.endTag("", "MessageType");
			serializer.startTag("", "MessageID");
			serializer.text("can be anything");
			serializer.endTag("", "MessageID");
			serializer.startTag("", "Message");
			serializer.text("<Category>" + Category + "</Category>");
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
			} else {
				serializer.text("<StartTime><Date>" + DATE.format(StartD) + "</Date><Time>" + TIME.format(StartD) + "</Time></StartTime>");
				serializer.text("<Owner><Number>" + MyNum + "</Number><Name>" + MyName + "</Name></Owner>");
				serializer.text("<Subject>SamyGO Reminder</Subject>");
				serializer.text("<EndTime><Date>" + DATE.format(EndD) + "</Date><Time>" + TIME.format(EndD) + "</Time></EndTime>");
				serializer.text("<Location></Location>");
				serializer.text("<Body>" + TextUtils.htmlEncode(Message) + "</Body>");
			}
			serializer.endTag("", "Message");
			serializer.endTag("", "u:AddMessage");
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

	public int go(){
		try {
			StringEntity se = new StringEntity(SOAPRequestXML,"UTF-8");
			se.setContentType("text/xml");

			httppost.setHeader("SOAPACTION",SOAP_ACTION);
			/** Assign the POST data to the entity */
			httppost.setEntity(se);

			/** Perform the actual HTTP POST */
			HttpResponse response = httpclient.execute(httppost);
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
