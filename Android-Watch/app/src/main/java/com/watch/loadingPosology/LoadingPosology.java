package com.watch.loadingPosology;

import android.app.Activity;
import android.app.AlarmManager;
import android.app.PendingIntent;
import android.content.Context;
import android.content.Intent;
import android.content.SharedPreferences;
import android.os.AsyncTask;
import android.os.Bundle;
import android.os.PowerManager;
import android.preference.PreferenceManager;
import android.text.format.Time;
import android.util.Log;
import android.view.Menu;
import android.view.MenuItem;
import android.widget.ProgressBar;
import android.widget.TextView;
import android.widget.Toast;

import com.watch.AlarmReceiverDrugs;
import com.watch.FragmentPreferences;
import com.watch.Home;
import com.watch.R;

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
import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.ObjectInputStream;
import java.util.Calendar;
import java.util.HashSet;


/**
 * Created by Thomas on 05/01/2015.
 */
public class LoadingPosology extends Activity {
    HashSet<String> _hours = new HashSet<String>();
    private static final String ID_USER = "id_user";
    String urlString = "";
    private ProgressBar _progress;
    private TextView _error;
    private boolean _flag = false;
    Context _context;
    Alarm _alarm;

    public static final int H = 17;
    public static final int M = 46;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.posology);
        _progress = (ProgressBar) findViewById(R.id.progressBar);
        _error = (TextView) findViewById(R.id.error);

        _context = getApplicationContext();

        SharedPreferences sp = PreferenceManager.getDefaultSharedPreferences(_context);
        urlString = sp.getString("serveur", "Pas de serveur");

        Log.e("KKLOAD",urlString);
        if(urlString.equals("Pas de serveur") || urlString.equals("")|| urlString.equals(null)){
            suppress_id();
            Toast.makeText(_context,"Veuillez vous appairez de nouveau",Toast.LENGTH_LONG).show();

            LoadingPosology.this.finish();

        }else {

            urlString = urlString.concat("getPosology.php?user_id="+getIdUser(this));
            Log.e("KKLOAD2",urlString);
            new Treatment().execute();
        }
    }

    private String getIdUser(Context context) {
        final SharedPreferences prefs = getSharedPreferences(Home.class.getSimpleName(), Context.MODE_PRIVATE);
        String idUser = prefs.getString(ID_USER, "");
        if (idUser.isEmpty()) {
            Log.e("kk", "No ID_User");
            return "";
        }
        return idUser;
    }


    /******************************************************************************************/
    private class Treatment extends AsyncTask<Void, Void, Void> {
        @Override
        protected Void doInBackground(Void... params) {
            PowerManager pm = (PowerManager)LoadingPosology.this.getSystemService(Context.POWER_SERVICE);
            PowerManager.WakeLock wl = pm.newWakeLock(PowerManager.PARTIAL_WAKE_LOCK, "wake");
            wl.setReferenceCounted(false);
            wl.acquire();

            //get JSON Data
            readAndParseJSON();

            //Create alarms
            for (String hour : _hours) { 
                int hou = Integer.parseInt(hour.substring(0, 2));
                int minute = Integer.parseInt(hour.substring(3, 5));
                loadInfos(hou, minute);
                planification();
            }

            /*
            //For test
            loadInfos(H, M);
            planification();
            */
            wl.release();

            _flag = true;
            return null;
        }

        protected void onPostExecute(Void handler) {
            _progress.setVisibility(ProgressBar.GONE);
            if(_flag) {

                _error.setText("Le chargement a bien été effectué.");

            }else{

                _error.setText("Une erreur de réseau a empeché la mise à jour.");

            }
        }
    }

    /*************************************************************************************************/

    public void loadInfos(int hour, int min){
        try {
            ObjectInputStream alarmOIS= new ObjectInputStream(openFileInput("alarm.serial"));
            _alarm = (Alarm) alarmOIS.readObject();
            alarmOIS.close();
        }catch(FileNotFoundException fnfe){
            _alarm = new Alarm();
            _alarm.setActive(true);
            Time t = new Time();
            t.hour = hour;
            t.minute = min;
            _alarm.setHour(t);
        }catch(IOException ioe) {
            ioe.printStackTrace();
        }catch(ClassNotFoundException cnfe) {
            cnfe.printStackTrace();
        }
    }

    private void planification() {
        AlarmManager am = (AlarmManager) getSystemService(Context.ALARM_SERVICE);

        Intent intent = new Intent(this, AlarmReceiverDrugs.class);
        final int _id = (int) System.currentTimeMillis();
        PendingIntent pendingintent = PendingIntent.getBroadcast(this, _id, intent, 0);

        am.cancel(pendingintent);

        Calendar clock  = Calendar.getInstance();
        clock.set(Calendar.HOUR_OF_DAY, _alarm.getHour().hour);
        clock.set(Calendar.MINUTE, _alarm.getHour().minute);

        if(clock.compareTo(Calendar.getInstance()) == -1) {
            clock.add(Calendar.DAY_OF_YEAR, 1);
        }

        Calendar cal = Calendar.getInstance();
        clock.set(Calendar.SECOND, 0);
        cal.set(Calendar.SECOND, 0);
        long diff = clock.getTimeInMillis() - cal.getTimeInMillis();

        am.set(AlarmManager.RTC_WAKEUP,cal.getTimeInMillis() + diff, pendingintent);

    }

   /*************************************************************************************************/
    public void readAndParseJSON() {
        try {
            JSONArray jArray = getJSONFromUrl(urlString);

            if(jArray != null) {
                for (int i = 0; i < jArray.length(); i++) {
                    JSONObject drug = jArray.getJSONObject(i);

                    JSONArray Jposology = drug.getJSONArray("posology");

                    for (int j = 0; j < Jposology.length(); j++) {
                        JSONObject h = Jposology.getJSONObject(j);
                        _hours.add(h.getString("hour"));
                    }
                }

                Log.e("KK",_hours.toString());

                _flag = true;

            }else{
                _flag = false;
            }

        } catch (Exception e) {
            _flag = false;
            e.printStackTrace();
        }
    }

    public JSONArray getJSONFromUrl(String url) {
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

                content.close();

            } else {
                _flag = false;
                Log.e("KK", "Failed to download file");
            }

        } catch (ClientProtocolException e) {
            e.printStackTrace();
        } catch (IOException e) {
            e.printStackTrace();
        }

        JSONArray jarray = null;

        // Parse String to JSON object
        try {
            jarray = new JSONArray( builder.toString());
        } catch (JSONException e) {
            Log.e("JSON Parser", "Error parsing data " + e.toString());
        }

        // return JSON Object
        return jarray;

    }

    /**********************************************************************************************/

    private void  suppress_id() {
        final SharedPreferences prefs = getSharedPreferences(Home.class.getSimpleName(), Context.MODE_PRIVATE);
        SharedPreferences.Editor editor = prefs.edit();
        editor.putString(ID_USER, "");
        editor.commit();
    }

    /******************************************************************************************/
    /**************                          MENU                                 **************/
    /******************************************************************************************/

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        getMenuInflater().inflate(R.menu.menu_main, menu);
        return true;
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        switch (item.getItemId()) {
            case R.id.menu_settings:

                Intent intent = new Intent(this, FragmentPreferences.FragmentActivity.class);
                startActivity(intent);

                return true;
            default:
                return super.onOptionsItemSelected(item);
        }
    }

}
