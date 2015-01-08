package com.watch.loadingPosology;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.widget.Toast;


/**
 * Created by Thomas on 05/01/2015.
 */
public class AlarmReceiver extends BroadcastReceiver{

    @Override
    public void onReceive(Context context, Intent intent) {
        try {
            Toast.makeText(context, "Loading treatment...",Toast.LENGTH_LONG).show();
            Intent intentone = new Intent(context.getApplicationContext(), LoadingPosology.class);
            intentone.addFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
            context.startActivity(intentone);

        } catch (Exception r) {
            Toast.makeText(context, "Error...",Toast.LENGTH_SHORT).show();
            r.printStackTrace();
        }
    }



}