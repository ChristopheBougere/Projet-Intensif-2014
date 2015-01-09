package app;

import android.app.Activity;
import android.app.ProgressDialog;
import android.content.Context;
import android.content.Intent;
import android.content.SharedPreferences;
import android.os.AsyncTask;
import android.os.Bundle;
import android.preference.PreferenceManager;
import android.util.Log;
import android.view.Menu;
import android.view.MenuItem;
import android.widget.ListView;
import android.widget.ProgressBar;
import android.widget.TextView;

import com.gcm.R;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import java.util.ArrayList;
import java.util.HashMap;

/**
 * Created by Thomas on 06/01/2015.
 */
public class Historique extends Activity {
    private static final String ID_USER = "id_user";
    JSONObject _jObject;
    JSONArray _jArray;
    ListView listview;
    ListViewAdapter adapter;
    ArrayList<HashMap<String, String>> _array;
    static String DATE = "date";
    static String TYPE = "type";
    static String HOUR = "hour";
    static String LEVEL = "alert_level";
    Context _context;

    TextView _infos;
    ProgressBar _pBar;

    String urlString = "";

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        setContentView(R.layout.historique_view);
        _context = getApplicationContext();

        SharedPreferences sp = PreferenceManager.getDefaultSharedPreferences(_context);
        urlString = sp.getString("serveur", "Pas de serveur");

        if( getIdUser(Historique.this.getApplicationContext()).equals("")){
            Intent i = new Intent(this,Home.class);
            startActivity(i);
            Historique.this.finish();

        }else{
            _infos = (TextView) findViewById(R.id.infos);
            _pBar = (ProgressBar) findViewById(R.id.pBar);
            new DownloadJSON().execute();
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

    /***************************************************************************************************/

    private class DownloadJSON extends AsyncTask<Void, Void, Void> {
        private boolean _flag = false;
        private boolean _empty = false;


        @Override
        protected Void doInBackground(Void... params) {
            _array = new ArrayList<>();

            if(urlString.equals("Pas de serveur") || urlString.equals("")|| urlString.equals(null)){
                _flag = false;

            }else {
                _jArray = JSONfunctions.getJSONFromUrl(urlString.concat("getAlerts.php?user_id="+getIdUser(Historique.this.getApplicationContext())));

                if (_jArray == null) {
                    _flag = false;

                } else {

                    if (_jArray.length() == 0) {
                        _empty = true;
                        _flag = true;

                    } else {
                        try {
                            for (int i = 0; i < _jArray.length(); i++) {
                                HashMap<String, String> map = new HashMap<String, String>();
                                _jObject = _jArray.getJSONObject(i);

                                // Retrive JSON Objects
                                map.put("type", Integer.toString(_jObject.getInt("type")));
                                map.put("date", _jObject.getString("date"));
                                map.put("hour", _jObject.getString("hour"));
                                map.put("alert_level", Integer.toString(_jObject.getInt("alert_level")));

                                // Set the JSON Objects into the array
                                _array.add(map);
                                _flag = true;
                                _empty = false;
                            }

                        } catch (JSONException e) {
                            Log.e("Error", e.getMessage());
                            e.printStackTrace();
                        }
                    }
                }
            }
            return null;
        }

        @Override
        protected void onPostExecute(Void args) {
            _pBar.setVisibility(ProgressBar.GONE);

            if(_flag) {
                if(_empty){
                    _infos.setVisibility(TextView.VISIBLE);
                    _infos.setText("Aucune alerte en mémoire.");
                }else {
                    listview = (ListView) findViewById(R.id.listview);
                    adapter = new ListViewAdapter(Historique.this, _array);
                    listview.setAdapter(adapter);
                }
            }else{
                _infos.setVisibility(TextView.VISIBLE);
                _infos.setText("Erreur de connexion.");
            }
        }
    }

    /***************************************************************************************************/

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
