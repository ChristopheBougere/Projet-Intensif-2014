package com.watch;

import android.app.Activity;
import android.content.Context;
import android.content.SharedPreferences;
import android.os.Bundle;
import android.preference.EditTextPreference;
import android.preference.Preference;
import android.preference.PreferenceFragment;
import android.preference.PreferenceManager;
import android.view.Menu;
import android.view.MenuInflater;
import android.view.MenuItem;

/**
 * Created by Thomas on 08/01/2015.
 */
public class FragmentPreferences extends PreferenceFragment implements SharedPreferences.OnSharedPreferenceChangeListener {
    private static final String ID_SERV = "id_serveur";
    private static final String ID_USER = "id_user";

    private Context _ctx;
    @Override
    public void onCreate(Bundle savedInstanceState) {
        setHasOptionsMenu(true);
        super.onCreate(savedInstanceState);
        addPreferencesFromResource(R.xml.preferences);

        _ctx = getActivity().getApplicationContext();

        SharedPreferences sp = getPreferenceManager().getSharedPreferences();

        EditTextPreference serv = (EditTextPreference) findPreference("serveur");
        serv.setSummary(sp.getString("serveur", "Pas de serveur"));
    }

    @Override
    public void onResume() {
        super.onResume();
        getPreferenceScreen().getSharedPreferences()
                .registerOnSharedPreferenceChangeListener(this);
    }

    @Override
    public void onPause() {
        super.onPause();
        getPreferenceScreen().getSharedPreferences()
                .unregisterOnSharedPreferenceChangeListener(this);
    }


    public void onSharedPreferenceChanged(SharedPreferences sharedPreferences,String key) {
        Preference pref = findPreference(key);
        if (pref instanceof EditTextPreference) {
            EditTextPreference etp = (EditTextPreference) pref;
            pref.setSummary(etp.getText());
        }
    }

    public static class FragmentActivity extends Activity {

        protected void onCreate(Bundle savedInstanceState) {
            super.onCreate(savedInstanceState);
            getFragmentManager().beginTransaction().replace(android.R.id.content, new FragmentPreferences()).commit();
        }
    }

    @Override
    public void onCreateOptionsMenu(Menu menu, MenuInflater inflater){
        inflater.inflate(R.menu.menu_prefs, menu);
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item){
        int id = item.getItemId();
        switch(id){
            case R.id.reinitialize:
                tel_par_defaut();
                suppress_id();
                return true;
        }
        return super.onOptionsItemSelected(item);
    }

    private void tel_par_defaut() {
        SharedPreferences prefs =  PreferenceManager.getDefaultSharedPreferences(getActivity());
        SharedPreferences.Editor editor = prefs.edit();
        editor.putString("serveur", "Pas de serveur");
        editor.commit();
        setPreferenceScreen(null);
        addPreferencesFromResource(R.xml.preferences);
    }

    private void  suppress_id() {
        final SharedPreferences prefs = getActivity().getSharedPreferences(Home.class.getSimpleName(), Context.MODE_PRIVATE);
        SharedPreferences.Editor editor = prefs.edit();
        editor.putString(ID_USER, "");
        editor.putString(ID_SERV, "");
        editor.commit();
    }

}
