package com.watch;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.widget.Toast;

import com.watch.loadingPosology.LoadingPosology;

/**
 * Created by Thomas on 06/01/2015.
 */
public class AlarmReceiverDrugs extends BroadcastReceiver {
    @Override
    public void onReceive(Context context, Intent intent) {
        Toast.makeText(context, "Taken treatment !", Toast.LENGTH_LONG).show();
        Intent intentone = new Intent(context.getApplicationContext(), TakeDrugs.class);
        intentone.addFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
        context.startActivity(intentone);
    }
}
