package com.watch.loadingPosology;

import android.app.AlarmManager;
import android.app.PendingIntent;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.text.format.Time;
import android.widget.Toast;

import com.watch.Home;

import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.ObjectInputStream;
import java.util.Calendar;

/**
 * Created by Thomas on 05/01/2015.
 */
public class AlarmBootReceiver extends BroadcastReceiver{
    Alarm _alarm;
    Context _context;
    final int ALARM_ID = 1234567;

    @Override
    public void onReceive(Context context, Intent intent) {
        _context = context;
        loadInfos();
        planification();
    }

  /*************************************************************************************************/
    public void loadInfos(){
        _alarm = null;
        try {
            ObjectInputStream alarmOIS= new ObjectInputStream(_context.openFileInput("alarm.serial"));
            try {
                _alarm = (Alarm) alarmOIS.readObject();
            } finally {
                try {
                    alarmOIS.close();
                } finally {
                    ;
                }
            }
        }catch(FileNotFoundException fnfe){
            _alarm = new Alarm();
            _alarm.setActive(true);
            Time t = new Time();
            t.hour = 2;
            t.minute = 00;
            _alarm.setHour(t);
        }
        catch(IOException ioe) {
            ioe.printStackTrace();
        }
        catch(ClassNotFoundException cnfe) {
            cnfe.printStackTrace();
        }
    }

    private void planification() {
        AlarmManager am = (AlarmManager) _context.getSystemService(Context.ALARM_SERVICE);
        Intent intent = new Intent(_context, AlarmReceiver.class);
        PendingIntent pendingintent = PendingIntent.getBroadcast(_context, ALARM_ID, intent, 0);
        am.cancel(pendingintent);

        Calendar alarm  = Calendar.getInstance();
        alarm.set(Calendar.HOUR_OF_DAY, _alarm.getHour().hour);
        alarm.set(Calendar.MINUTE, _alarm.getHour().minute);
        if(alarm.compareTo(Calendar.getInstance()) == -1) {
            alarm.add(Calendar.DAY_OF_YEAR, 1);
        }

        Calendar cal = Calendar.getInstance();
        alarm.set(Calendar.SECOND, 0);
        cal.set(Calendar.SECOND, 0);
        long diff = alarm.getTimeInMillis() - cal.getTimeInMillis();

        am.set(AlarmManager.RTC_WAKEUP,cal.getTimeInMillis() + diff, pendingintent);
        Toast.makeText(_context, "Alarm set", Toast.LENGTH_SHORT).show();
    }
}
