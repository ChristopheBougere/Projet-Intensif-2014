package com.gcm;

import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.content.SharedPreferences;
import android.content.pm.PackageInfo;
import android.content.pm.PackageManager;
import android.os.AsyncTask;
import android.os.Bundle;
import android.preference.PreferenceManager;
import android.text.TextUtils;
import android.util.Log;
import android.widget.Toast;

import com.google.android.gms.gcm.GoogleCloudMessaging;

import org.apache.http.HttpResponse;
import org.apache.http.NameValuePair;
import org.apache.http.StatusLine;
import org.apache.http.client.HttpClient;
import org.apache.http.client.entity.UrlEncodedFormEntity;
import org.apache.http.client.methods.HttpGet;
import org.apache.http.client.methods.HttpPost;
import org.apache.http.impl.client.DefaultHttpClient;
import org.apache.http.message.BasicNameValuePair;

import java.io.IOException;
import java.io.UnsupportedEncodingException;
import java.util.ArrayList;
import java.util.List;

import app.Historique;
import app.Home;

/**
 * Created by Thomas on 06/01/2015.
 */
public class Enregistrement extends Activity {
    private static final String ID_USER = "id_user";
    private static final String ID_SERV = "id_serveur";

    String regId;
    String _num;
    GoogleCloudMessaging gcm;
    Context context;
    //public static final String REG_ID = "regId";
    //private static final String APP_VERSION = "appVersion";

    String urlServer = "";

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        context = getApplicationContext();

        SharedPreferences sp = PreferenceManager.getDefaultSharedPreferences(context);
        urlServer = sp.getString("serveur", "Pas de serveur");

       if(urlServer.equals("Pas de serveur") || urlServer.equals("") || urlServer.equals(null)){

           final SharedPreferences prefs = getSharedPreferences(Home.class.getSimpleName(), Context.MODE_PRIVATE);
           SharedPreferences.Editor editor = prefs.edit();
           editor.putString(ID_USER, "");
           editor.putString(ID_SERV, "");
           editor.commit();

           Toast.makeText(context,"Erreur serveur",Toast.LENGTH_LONG).show();

           Enregistrement.this.finish();

       }else {

           registerInBackground();

        /*if (TextUtils.isEmpty(regId)) {
            regId = registerGCM();
        } else {
            Toast.makeText(getApplicationContext(), "Already Registered with GCM Server!", Toast.LENGTH_LONG).show();
        }*/

           sendServeur();

           Intent i = new Intent(Enregistrement.this, Historique.class);
           startActivity(i);
           Enregistrement.this.finish();

       }

    }

    /*public String registerGCM() {
        gcm = GoogleCloudMessaging.getInstance(this);
        regId = getRegistrationId(context);
        } else {
            Toast.makeText(getApplicationContext(), "RegId already available. RegId: " + regId,Toast.LENGTH_LONG).show();
    }

        return regId;
    }*/

    /*private String getRegistrationId(Context context) {
        final SharedPreferences prefs = getSharedPreferences(Home.class.getSimpleName(), Context.MODE_PRIVATE);
        String registrationId = prefs.getString(REG_ID, "");

        if (registrationId.isEmpty()) {
            Log.e("kk", "Registration not found.");
            return "";
        }

        int registeredVersion = prefs.getInt(APP_VERSION, Integer.MIN_VALUE);
        int currentVersion = getAppVersion(context);
        if (registeredVersion != currentVersion) {
            Log.e("kk", "App version changed.");
            return "";
        }
        return registrationId;
    }*/

    /*private static int getAppVersion(Context context) {
        try {
            PackageInfo packageInfo = context.getPackageManager().getPackageInfo(context.getPackageName(), 0);
            return packageInfo.versionCode;
        } catch (PackageManager.NameNotFoundException e) {
            throw new RuntimeException(e);
        }
    }*/

   /* private void storeRegistrationId(Context context, String regId) {
        final SharedPreferences prefs = getSharedPreferences(Home.class.getSimpleName(), Context.MODE_PRIVATE);
        int appVersion = getAppVersion(context);
        Log.e("kk", "Saving regId on app version " + appVersion);

        SharedPreferences.Editor editor = prefs.edit();
        editor.putString(REG_ID, regId);
        editor.putInt(APP_VERSION, appVersion);

        editor.commit();
    }*/

    private String getIdUser(Context context) {
        final SharedPreferences prefs = getSharedPreferences(Home.class.getSimpleName(), Context.MODE_PRIVATE);
        String idUser = prefs.getString(ID_USER, "");
        if (idUser.isEmpty()) {
            Log.e("kk", "No ID_User");
            return "";
        }
        return idUser;
    }

    private void sendServeur(){
        new AsyncTask<Void, Void, Void>() {
            @Override
            protected Void doInBackground(Void... params) {
                try {

                    urlServer = urlServer.concat("?user_id="+getIdUser(context)+"&push_id="+regId);

                    HttpClient httpclient = new DefaultHttpClient();
                    HttpGet httpget = new HttpGet(urlServer);
                    HttpResponse response = httpclient.execute(httpget);

                    Log.e("EnregistrementStatus", "" + response.getStatusLine().getStatusCode());

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
    /**********************************************************************************************/
    private void registerInBackground() {
        new AsyncTask<Void, Void, String>() {
            @Override
            protected String doInBackground(Void... params) {
                String msg = "";

                try {
                    if (gcm == null) {
                        gcm = GoogleCloudMessaging.getInstance(context);
                    }

                    regId = gcm.register(Config.GOOGLE_PROJECT_ID);
                    Log.d("VERIFY", "registerInBackground - regId: " + regId);
                    msg = "Device registered, registration ID=" + regId;

                    //storeRegistrationId(context, regId);

                } catch (IOException ex) {
                    msg = "Error :" + ex.getMessage();
                    Log.d("VERIFY", "Error: " + msg);
                }
                Log.d("VERIFY", "AsyncTask completed: " + msg);
                return msg;
            }

            @Override
            protected void onPostExecute(String msg) {}
        }.execute(null, null, null);
    }
}
