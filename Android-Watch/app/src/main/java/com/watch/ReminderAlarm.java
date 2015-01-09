package com.watch;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.SharedPreferences;
import android.os.AsyncTask;
import android.os.PowerManager;
import android.preference.PreferenceManager;
import android.util.Log;
import android.widget.ProgressBar;
import android.widget.Toast;

import org.apache.http.HttpEntity;
import org.apache.http.HttpResponse;
import org.apache.http.StatusLine;
import org.apache.http.client.ClientProtocolException;
import org.apache.http.client.HttpClient;
import org.apache.http.client.methods.HttpGet;
import org.apache.http.impl.client.DefaultHttpClient;
import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;

/**
 * Created by Thomas on 06/01/2015.
 */
public class ReminderAlarm extends BroadcastReceiver {
    private static final String ID_USER = "id_user";
    String urlString = "";
    private boolean bool = false;
   private Context _context;

    @Override
    public void onReceive(Context context, Intent intent) {
        Log.e("KK","ReminderAlarm");

        _context = context;
        SharedPreferences sp = PreferenceManager.getDefaultSharedPreferences(_context);
        urlString = sp.getString("serveur", "Pas de serveur");

        if(!urlString.equals("Pas de serveur")) {
            urlString = urlString.concat("AreDrugsTaken.php?user_id=" + getIdUser(_context));
            new Treatment().execute();
        }

    }

    private String getIdUser(Context context) {
        final SharedPreferences prefs = _context.getSharedPreferences(Home.class.getSimpleName(), Context.MODE_PRIVATE);
        String idUser = prefs.getString(ID_USER, "");
        if (idUser.isEmpty()) {
            Log.e("kk", "No ID_User");
            return "";
        }
        return idUser;
    }

    private class Treatment extends AsyncTask<Void, Void, Void> {
        @Override
        protected Void doInBackground(Void... params) {

            JSONObject take = getJSONFromUrl(urlString);

            if(take != null) {
                try {
                    bool = take.getBoolean("taken");
                } catch (JSONException e) {
                    e.printStackTrace();
                }
            }
            return null;
        }

        protected void onPostExecute(Void handler) {
            if(!bool){
                Toast.makeText(_context, "Prise traitement (Rappel)", Toast.LENGTH_LONG).show();
                Intent intentone = new Intent(_context.getApplicationContext(), TakeDrugsBis.class);
                intentone.addFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
                _context.startActivity(intentone);
            }
        }
    }

    public JSONObject getJSONFromUrl(String url) {
        StringBuilder builder = new StringBuilder();

        HttpClient client = new DefaultHttpClient();
        HttpGet httpGet = new HttpGet(url);

        try {
            HttpResponse response = client.execute(httpGet);
            StatusLine statusLine = response.getStatusLine();
            int statusCode = statusLine.getStatusCode();

            if (statusCode == 200) {
                HttpEntity entity = response.getEntity();
                InputStream content = entity.getContent();
                BufferedReader reader = new BufferedReader(new InputStreamReader(content));

                String line;
                while ((line = reader.readLine()) != null) {
                    builder.append(line);
                }

            } else {
                Log.e("KK", "Failed to download file");
            }

        } catch (ClientProtocolException e) {
            e.printStackTrace();
        } catch (IOException e) {
            e.printStackTrace();
        }

        String val = builder.toString();

        JSONObject jObj = null;
        try {
            jObj = new JSONObject(val);
        } catch (JSONException e) {
            Log.e("JSON Parser", "Error parsing data " + e.toString());
        }
        // return JSON String
        return jObj;
    }
}

