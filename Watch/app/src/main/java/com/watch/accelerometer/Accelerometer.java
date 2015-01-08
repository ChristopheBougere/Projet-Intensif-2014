package com.watch.accelerometer;

import android.app.Notification;
import android.app.Service;
import android.content.BroadcastReceiver;
import android.content.IntentFilter;
import android.content.Context;
import android.content.Intent;
import android.hardware.Sensor;
import android.hardware.SensorEvent;
import android.hardware.SensorEventListener;
import android.hardware.SensorManager;
import android.os.AsyncTask;
import android.os.Bundle;
import android.os.Handler;
import android.os.IBinder;
import android.os.PowerManager.WakeLock;
import android.os.PowerManager;
import android.os.Process;
import android.os.Vibrator;
import android.util.Log;

import org.apache.http.NameValuePair;
import org.apache.http.client.HttpClient;
import org.apache.http.client.entity.UrlEncodedFormEntity;
import org.apache.http.client.methods.HttpPost;
import org.apache.http.impl.client.DefaultHttpClient;
import org.apache.http.message.BasicNameValuePair;

import java.io.IOException;
import java.io.UnsupportedEncodingException;
import java.util.ArrayList;
import java.util.List;

/**
 * Created by Thomas on 07/01/2015.
 */
public class Accelerometer extends Service implements SensorEventListener{

    String urlServer = "http://192.168.5.101/serverApi/actions/sendAlert.php";

    public static final int SCREEN_OFF_RECEIVER_DELAY = 500; // TODO a tester + grande valeurs
    private SensorManager mSensorManager = null;
    private WakeLock mWakeLock = null;

    public static final String TAG = Accelerometer.class.getName();

    private float deltaX = 0, deltaY = 0,deltaZ = 0;
    private float lastX= 0, lastY= 0, lastZ= 0;
    private float vibrateThreshold = 0;
    private int i = 0;
    private Vibrator v;

    /*********************************************************************************************/
    @Override
    public void onCreate() {
        super.onCreate();

        v = (Vibrator) this.getSystemService(Context.VIBRATOR_SERVICE);

        mSensorManager = (SensorManager) getSystemService(SENSOR_SERVICE);
        PowerManager manager = (PowerManager) getSystemService(Context.POWER_SERVICE);
        mWakeLock = manager.newWakeLock(PowerManager.PARTIAL_WAKE_LOCK, TAG);
        registerReceiver(mReceiver, new IntentFilter(Intent.ACTION_SCREEN_OFF));
    }

    private void registerListener() {
        Log.i(TAG, "registerListener().");
        Sensor mAcc = mSensorManager.getDefaultSensor(Sensor.TYPE_ACCELEROMETER);

        mSensorManager.registerListener(this,mAcc,SensorManager.SENSOR_DELAY_NORMAL);
        vibrateThreshold = (float)0.7*mAcc.getMaximumRange();

    }

    private void unregisterListener() {
        Log.i(TAG, "unregisterListener().");

        mSensorManager.unregisterListener(this);
    }

    public BroadcastReceiver mReceiver = new BroadcastReceiver() {
        @Override
        public void onReceive(Context context, Intent intent) {
            Log.i(TAG, "mReceiver().");

            Log.i(TAG, "onReceive(" + intent + ")");
            if (!intent.getAction().equals(Intent.ACTION_SCREEN_OFF)) {
                return;
            }

            Runnable runnable = new Runnable() {
                public void run() {
                    Log.i(TAG, "Runnable executing.");
                    unregisterListener();
                    registerListener();
                }
            };
            new Handler().postDelayed(runnable, SCREEN_OFF_RECEIVER_DELAY);
        }
    };


    public void onAccuracyChanged(Sensor sensor, int accuracy) {
        Log.i(TAG, "onAccuracyChanged().");
    }



    @Override
    public void onDestroy() {
        unregisterReceiver(mReceiver);
        unregisterListener();
        mWakeLock.release();
        stopForeground(true);
    }

    @Override
    public IBinder onBind(Intent intent) {
        return null;
    }

    @Override
    public int onStartCommand(Intent intent, int flags, int startId) {
        Log.i(TAG, "onStartCommand().");
        super.onStartCommand(intent, flags, startId);

        startForeground(Process.myPid(), new Notification());
        registerListener();
        mWakeLock.acquire();
        return START_STICKY;
    }

    public void onSensorChanged(SensorEvent event) {
        deltaX = Math.abs(lastX - event.values[0]);
        deltaY = Math.abs(lastY - event.values[1]);
        deltaZ = Math.abs(lastZ - event.values[2]);

        if (deltaX < 2)
            deltaX = 0;
        if (deltaY < 2)
            deltaY = 0;
        if (deltaZ < 2)
            deltaZ = 0;

        lastX = event.values[0];
        lastY = event.values[1];
        lastZ = event.values[2];

        if ((deltaX > vibrateThreshold) || (deltaY > vibrateThreshold) || (deltaZ > vibrateThreshold)) {
            v.vibrate(50);
            // TODO Start a broadcast pour alerte + sendServeur();
        }

    }

    /*******************************************************************************************************/
    private void sendServeur() {
        new AsyncTask<Void, Void, Void>() {
            @Override
            protected Void doInBackground(Void... params) {
                try {

                    HttpPost httppost = new HttpPost(urlServer);

                    List<NameValuePair> nameValuePairs = new ArrayList<NameValuePair>();

                    nameValuePairs.add(new BasicNameValuePair("user_id", "1"));
                    nameValuePairs.add(new BasicNameValuePair("alert_type", "2"));
                    nameValuePairs.add(new BasicNameValuePair("alert_level", "1"));
                    httppost.setEntity(new UrlEncodedFormEntity(nameValuePairs));

                    HttpClient httpclient = new DefaultHttpClient();

                    httpclient.execute(httppost);

                } catch (UnsupportedEncodingException e) {

                    e.printStackTrace();
                } catch (IOException e) {

                    e.printStackTrace();
                }
                return null;
            }

            @Override
            protected void onPostExecute(Void msg) {
                Log.e("SEND", "Envoyé au serveur.");
            }
        }.execute(null, null, null);
    }


}
