package com.watch;

import android.app.Activity;
import android.app.AlarmManager;
import android.app.PendingIntent;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.content.SharedPreferences;
import android.nfc.NdefMessage;
import android.nfc.NdefRecord;
import android.nfc.NfcAdapter;
import android.nfc.Tag;
import android.nfc.tech.Ndef;
import android.os.AsyncTask;
import android.os.Bundle;
import android.preference.PreferenceManager;
import android.text.format.Time;
import android.util.Log;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;
import android.widget.Toast;

import com.watch.accelerometer.Accelerometer;
import com.watch.loadingPosology.Alarm;
import com.watch.loadingPosology.AlarmReceiver;
import com.watch.loadingPosology.LoadingPosology;

import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.ObjectInputStream;
import java.io.UnsupportedEncodingException;
import java.util.Arrays;
import java.util.Calendar;

/**
 * Created by Thomas on 06/01/2015.
 */
public class Home extends Activity {
    public static final String MIME_TEXT_PLAIN = "text/plain";
    public static final String TAG = "NfcDemo";
    private NfcAdapter mNfcAdapter;


    private Context _context;
    private TextView _text;
    public Alarm _alarm = null;
    final int ALARM_ID = 1234567;
    private static final String ID_USER = "id_user";
    public static final int HOUR = 2;
    public static final int MIN = 00;

    @Override

    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        _context = getApplicationContext();
        setContentView(R.layout.todonfc);

        startService(new Intent(this, Accelerometer.class));

        _text = (TextView) findViewById(R.id.textView);

        mNfcAdapter = NfcAdapter.getDefaultAdapter(this);

        if(!getIdUser(this).equals("")) { //Tout est OK, deja enregistré
            _text.setVisibility(TextView.INVISIBLE);

            /*loadInfos();
            planification();*/

            Intent goLoading = new Intent(this, LoadingPosology.class);
            startActivity(goLoading);
            finish();

        }else{

            if (mNfcAdapter == null) {
                // Stop here, we definitely need NFC
                Toast.makeText(this, "This device doesn't support NFC.", Toast.LENGTH_LONG).show();
                finish();
                return;

            }

            if (!mNfcAdapter.isEnabled()) {
                Toast.makeText(this, "NFC Off.", Toast.LENGTH_LONG).show();
                finish();
            }

            handleIntent(getIntent());

        }



    }

    /******************************************************************************************/
    /**************                   GET AND REGISTER IDUSER                    **************/
    /******************************************************************************************/

    private String getIdUser(Context context) {
        final SharedPreferences prefs = getSharedPreferences(Home.class.getSimpleName(), Context.MODE_PRIVATE);
        String idUser = prefs.getString(ID_USER, "");
        if (idUser.isEmpty()) {
            Log.e("kk", "No ID_User");
            return "";
        }
        return idUser;
    }

    private void storeRegistrationId(Context context, String IdUser) {
        final SharedPreferences prefs = getSharedPreferences(Home.class.getSimpleName(), Context.MODE_PRIVATE);
        SharedPreferences.Editor editor = prefs.edit();
        editor.putString(ID_USER, IdUser);
        editor.commit();
    }

    /******************************************************************************************/
    /**************                   CREATE ALARM                               **************/
    /******************************************************************************************/

    public void loadInfos(){
        try {
            ObjectInputStream alarmOIS= new ObjectInputStream(openFileInput("alarm.serial"));
            _alarm = (Alarm) alarmOIS.readObject();
            alarmOIS.close();
        }catch(FileNotFoundException fnfe){
            _alarm = new Alarm();
            _alarm.setActive(true);
            Time t = new Time();
            t.hour = HOUR;
            t.minute = MIN;
            _alarm.setHour(t);
        }catch(IOException ioe) {
            ioe.printStackTrace();
        }catch(ClassNotFoundException cnfe) {
            cnfe.printStackTrace();
        }
    }

    private void planification() {
        AlarmManager am = (AlarmManager) getSystemService(Context.ALARM_SERVICE);
        Intent intent = new Intent(this, AlarmReceiver.class);
        PendingIntent pendingintent = PendingIntent.getBroadcast(this, ALARM_ID, intent, 0);

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
        Toast.makeText(this, "Alarm set.", Toast.LENGTH_SHORT).show();
    }

    /******************************************************************************************/
    /**************                          NFC                                 **************/
    /******************************************************************************************/

    private void handleIntent(Intent intent) {
        String action = intent.getAction();
        if (NfcAdapter.ACTION_NDEF_DISCOVERED.equals(action)) {

            String type = intent.getType();
            if (MIME_TEXT_PLAIN.equals(type)) {

                Tag tag = intent.getParcelableExtra(NfcAdapter.EXTRA_TAG);
                new NdefReaderTask().execute(tag);

            } else {
                Log.d(TAG, "Wrong mime type: " + type);
            }

        } else if (NfcAdapter.ACTION_TECH_DISCOVERED.equals(action)) {
            // In case we would still use the Tech Discovered Intent
            Tag tag = intent.getParcelableExtra(NfcAdapter.EXTRA_TAG);
            String[] techList = tag.getTechList();
            String searchedTech = Ndef.class.getName();

            for (String tech : techList) {
                if (searchedTech.equals(tech)) {
                    new NdefReaderTask().execute(tag);
                    break;
                }
            }
        }
    }

    @Override
    protected void onNewIntent(Intent intent) {
		/*
		 * This method gets called, when a new Intent gets associated with the current activity instance.
		 * Instead of creating a new activity, onNewIntent will be called. For more information have a look
		 * at the documentation.
		 *
		 * In our case this method gets called, when the user attaches a Tag to the device.
		 */
        handleIntent(intent);
    }

    @Override
    protected void onResume() {
        super.onResume();

		/*
		 * It's important, that the activity is in the foreground (resumed). Otherwise
		 * an IllegalStateException is thrown.
		 */
        setupForegroundDispatch(this, mNfcAdapter);
    }

    @Override
    protected void onPause() {
		/*
		 * Call this before onPause, otherwise an IllegalArgumentException is thrown as well.
		 */
        stopForegroundDispatch(this, mNfcAdapter);

        super.onPause();
    }


    public static void setupForegroundDispatch(final Activity activity, NfcAdapter adapter) {
        final Intent intent = new Intent(activity.getApplicationContext(), activity.getClass());
        intent.setFlags(Intent.FLAG_ACTIVITY_SINGLE_TOP);

        final PendingIntent pendingIntent = PendingIntent.getActivity(activity.getApplicationContext(), 0, intent, 0);

        IntentFilter[] filters = new IntentFilter[1];
        String[][] techList = new String[][]{};

        // Notice that this is the same filter as in our manifest.
        filters[0] = new IntentFilter();
        filters[0].addAction(NfcAdapter.ACTION_NDEF_DISCOVERED);
        filters[0].addCategory(Intent.CATEGORY_DEFAULT);
        try {
            filters[0].addDataType(MIME_TEXT_PLAIN);
        } catch (IntentFilter.MalformedMimeTypeException e) {
            throw new RuntimeException("Check your mime type.");
        }

        adapter.enableForegroundDispatch(activity, pendingIntent, filters, techList);
    }

    public static void stopForegroundDispatch(final Activity activity, NfcAdapter adapter) {
        adapter.disableForegroundDispatch(activity);
    }

    private class NdefReaderTask extends AsyncTask<Tag, Void, String> {

        @Override
        protected String doInBackground(Tag... params) {
            Tag tag = params[0];

            Ndef ndef = Ndef.get(tag);
            if (ndef == null) {
                // NDEF is not supported by this Tag.
                return null;
            }

            NdefMessage ndefMessage = ndef.getCachedNdefMessage();

            NdefRecord[] records = ndefMessage.getRecords();
            for (NdefRecord ndefRecord : records) {
                if (ndefRecord.getTnf() == NdefRecord.TNF_WELL_KNOWN && Arrays.equals(ndefRecord.getType(), NdefRecord.RTD_TEXT)) {
                    try {
                        return readText(ndefRecord);
                    } catch (UnsupportedEncodingException e) {
                        Log.e(TAG, "Unsupported Encoding", e);
                    }
                }
            }

            return null;
        }

        private String readText(NdefRecord record) throws UnsupportedEncodingException {
            byte[] payload = record.getPayload();
            String textEncoding = ((payload[0] & 128) == 0) ? "UTF-8" : "UTF-16"; // Get the Text Encoding
            int languageCodeLength = payload[0] & 0063;             // Get the Language Code
            // String languageCode = new String(payload, 1, languageCodeLength, "US-ASCII"); e.g. "en"
            // Get the Text
            return new String(payload, languageCodeLength + 1, payload.length - languageCodeLength - 1, textEncoding);
        }

        @Override
        protected void onPostExecute(String result) {
            if (result != null) {
                _text.setText("Appariement effectué");
                storeRegistrationId(_context, result);

                /*loadInfos();
                planification();*/

                Intent goLoading = new Intent(Home.this, LoadingPosology.class);
                startActivity(goLoading);
                Home.this.finish();
            }
        }
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

