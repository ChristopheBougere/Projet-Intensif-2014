package com.watch;

import android.app.Activity;
import android.app.AlarmManager;
import android.app.PendingIntent;
import android.content.Context;
import android.content.Intent;
import android.os.Bundle;
import android.os.CountDownTimer;
import android.os.Vibrator;
import android.view.View;
import android.widget.Button;
import java.util.Calendar;

/**
 * Created by Thomas on 08/01/2015.
 */
public class TakeDrugsBis extends Activity {
    private Button _close;
    final int ALARM_ID = 123456789;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.takebis);

        _close = (Button) findViewById(R.id.button);

        _close.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                AlarmManager am = (AlarmManager) getSystemService(Context.ALARM_SERVICE);
                Intent intent = new Intent(TakeDrugsBis.this, ReminderAlarm.class);
                PendingIntent pendingintent = PendingIntent.getBroadcast(TakeDrugsBis.this, ALARM_ID, intent, 0);
                am.cancel(pendingintent);

                Calendar cal = Calendar.getInstance();
                //20 min = 1200000
                am.set(AlarmManager.RTC_WAKEUP,cal.getTimeInMillis() + 1200000, pendingintent);
                TakeDrugsBis.this.finish();

            }});

        Vibrator vib = (Vibrator) getApplicationContext().getSystemService(Context.VIBRATOR_SERVICE);
        vib.vibrate(3000);

        new CountDownTimer(5000,5000){
            @Override
            public void onTick(long millisUntilFinished) {}

            @Override
            public void onFinish() {
                _close.setVisibility(Button.VISIBLE);
            }
        }.start();
    }
}
