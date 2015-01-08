package com.gcm;

import android.app.IntentService;
import android.app.Notification;
import android.app.NotificationManager;
import android.app.PendingIntent;
import android.content.Context;
import android.content.Intent;
import android.os.Bundle;
import android.os.SystemClock;
import android.support.v4.app.NotificationCompat;
import android.util.Log;

import com.google.android.gms.gcm.GoogleCloudMessaging;

import org.json.JSONException;
import org.json.JSONObject;

import java.util.ArrayList;

import app.Historique;
import push.Push;

/**
 * Created by Thomas on 05/01/2015.
 */
public class GCMNotificationIntentService extends IntentService {

    public static final int NOTIFICATION_ID = 1;
    private NotificationManager mNotificationManager;
    private ArrayList<String> _values = new ArrayList<>();

    public GCMNotificationIntentService() {
        super("GcmIntentService");
    }

    public static final String TAG = "GCMNotificationIntentService";

    @Override
    protected void onHandleIntent(Intent intent) {
        Bundle extras = intent.getExtras();
        GoogleCloudMessaging gcm = GoogleCloudMessaging.getInstance(this);

        String messageType = gcm.getMessageType(intent);

        if (!extras.isEmpty()) {
            if (GoogleCloudMessaging.MESSAGE_TYPE_SEND_ERROR .equals(messageType)) {
                sendNotification("Send error: " + extras.toString());
            } else if (GoogleCloudMessaging.MESSAGE_TYPE_DELETED
                    .equals(messageType)) {
                sendNotification("Deleted messages on server: "+ extras.toString());
            } else if (GoogleCloudMessaging.MESSAGE_TYPE_MESSAGE
                    .equals(messageType)) {

                for (int i = 0; i < 3; i++) {
                    Log.i(TAG,
                            "Working... " + (i + 1) + "/5 @ "
                                    + SystemClock.elapsedRealtime());
                    try {
                        Thread.sleep(5000);
                    } catch (InterruptedException e) {
                    }

                }
                Log.i(TAG, "Completed work @ " + SystemClock.elapsedRealtime());

                JSONread(extras.get(Config.MESSAGE_KEY).toString());

                String mois = _values.get(1).substring(5, 7);

                if(mois.equals("01")){
                    mois = "Janvier";
                }else if(mois.equals("02")){
                    mois = "Fevrier";
                }else if(mois.equals("03")){
                    mois = "Mars";
                }else if(mois.equals("04")){
                    mois = "Avril";
                }else if(mois.equals("05")){
                    mois = "Mai";
                }else if(mois.equals("06")){
                    mois = "Juin";
                }else if(mois.equals("07")){
                    mois = "Juillet";
                }else if(mois.equals("08")){
                    mois = "Aout";
                }else if(mois.equals("09")){
                    mois = "Septembre";
                }else if(mois.equals("10")){
                    mois = "Octobre";
                }else if(mois.equals("11")){
                    mois = "Novembre";
                }else if(mois.equals("12")){
                    mois = "Decembre";
                }

                String msg = _values.get(0) + " le " + _values.get(1).substring(9, 10)+ " " + mois +
                        " "+ _values.get(1).substring(0, 4)+ " à "+  _values.get(2).substring(0, 2)
                        +"h"+_values.get(2).substring(3, 5);

                sendNotification(msg);
                Log.i(TAG, "Received: " + extras.toString());
            }
        }
        GcmBroadcastReceiver.completeWakefulIntent(intent);
    }

    private void sendNotification(String msg) {

        mNotificationManager = (NotificationManager) this.getSystemService(Context.NOTIFICATION_SERVICE);
        Intent i = new Intent(this, Push.class);
        i.putStringArrayListExtra("Tab",_values);

        PendingIntent contentIntent = PendingIntent.getActivity(this, 0,i, 0);
        NotificationCompat.Builder mBuilder = new NotificationCompat.Builder(
                this).setSmallIcon(R.drawable.gcm_cloud)
                .setContentTitle("Care4You")
                .setStyle(new NotificationCompat.BigTextStyle().bigText(msg))
                .setAutoCancel(true)
                .setDefaults(Notification.DEFAULT_LIGHTS| Notification.DEFAULT_VIBRATE| Notification.DEFAULT_SOUND)
                .setContentText(msg);

        mBuilder.setContentIntent(contentIntent);
        mNotificationManager.notify(NOTIFICATION_ID, mBuilder.build());

    }


    private void JSONread(String msg){

        try {
            JSONObject reader = new JSONObject(msg);
            _values.add(reader.getString("message"));
            _values.add(reader.getString("date"));
            _values.add(reader.getString("hour"));

            boolean stream = reader.getBoolean("is_stream_available");
            _values.add(Boolean.toString(stream));
            if(stream){
                _values.add(reader.getString("stream_addr"));
            }

        } catch (JSONException e) {
            e.printStackTrace();
        }
    }



}
