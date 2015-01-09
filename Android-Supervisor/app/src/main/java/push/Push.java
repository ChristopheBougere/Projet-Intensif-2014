package push;

import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.content.SharedPreferences;
import android.net.Uri;
import android.os.Bundle;
import android.preference.Preference;
import android.preference.PreferenceManager;
import android.telephony.PhoneStateListener;
import android.telephony.TelephonyManager;
import android.util.Log;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;
import android.widget.Toast;

import com.gcm.R;

import java.util.ArrayList;

import app.FragmentPreferences;

/**
 * Created by Thomas on 06/01/2015.
 */

public class Push extends Activity {
    private Button _call, _stream, _ignore;
    private ArrayList<String> _val;
    String _tel;
    TextView _alarm,_type;
    Context _context;

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.push);

        Intent i = getIntent();
        _val = i.getStringArrayListExtra("Tab");

        _context = getApplicationContext();

        _call = (Button) findViewById(R.id.Call);
        _stream = (Button) findViewById(R.id.StreamVideo);
        _alarm = (TextView) findViewById(R.id.alarm);
        _ignore = (Button) findViewById(R.id.Ignore);
        _type = (TextView) findViewById(R.id.type);

        _type.setText(_val.get(0));

        String mois = _val.get(1).substring(5, 7);

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

        String heure = _val.get(2).substring(0, 2);
        if(heure.contains(":")){
            heure = "0"+_val.get(2).substring(0, 1);
        }

        String min = _val.get(2).substring(3, 5);
        if(min.contains(":")){
            min = "0"+_val.get(2).substring(3, 4);
        }

        String msg = "Alarme du " + _val.get(1).substring(9, 10)+ " " + mois +
                " "+ _val.get(1).substring(0, 4)+ " à "+  heure +"h"+min+".";

        _alarm.setText(msg);

        // add PhoneStateListener
        PhoneCallListener phoneListener = new PhoneCallListener();
        TelephonyManager telephonyManager = (TelephonyManager) this.getSystemService(Context.TELEPHONY_SERVICE);
        telephonyManager.listen(phoneListener,PhoneStateListener.LISTEN_CALL_STATE);
        SharedPreferences sp = PreferenceManager.getDefaultSharedPreferences(_context);

        _tel = sp.getString("numberPhone", "0000000000");

        if(_tel.equals("0000000000")) {
            _call.setEnabled(false);
        }

        _call.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View arg0) {
                    Intent callIntent = new Intent(Intent.ACTION_CALL);
                    callIntent.setData(Uri.parse("tel:" + _tel));
                    startActivity(callIntent);

            }
        });

        /*************************************************************************************/

        if(!_val.get(3).equals("true")){
            _stream.setEnabled(false);
        }

        _stream.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View arg0) {
                Intent st = new Intent(Push.this,Stream.class);
                st.putExtra("link",_val.get(4));
                startActivity(st);
            }
        });

        _ignore.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View arg0) {
                Push.this.finish();
            }
        });
    }


    /******************************************************************************************/

    //monitor phone call activities
    private class PhoneCallListener extends PhoneStateListener {
        private boolean isPhoneCalling = false;

        @Override
        public void onCallStateChanged(int state, String incomingNumber) {

            if (TelephonyManager.CALL_STATE_RINGING == state) {
            }

            if (TelephonyManager.CALL_STATE_OFFHOOK == state) {
                isPhoneCalling = true;
            }


        }
    }

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