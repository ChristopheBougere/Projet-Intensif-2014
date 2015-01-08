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
import android.widget.Toast;

import com.watch.loadingPosology.AlarmReceiver;

import java.util.Calendar;

public class TakeDrugs extends Activity {
    private PendingIntent _pendingIntent;
    private Button _close;
    final int ALARM_ID = 123456789;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        _close = (Button) findViewById(R.id.button);

        Vibrator vib = (Vibrator) getApplicationContext().getSystemService(Context.VIBRATOR_SERVICE);
        vib.vibrate(3000);

        new CountDownTimer(5000,5000){
            @Override
            public void onTick(long millisUntilFinished) {}

            @Override
            public void onFinish() {
                _close.setVisibility(Button.VISIBLE);

                _close.setOnClickListener(new View.OnClickListener() {
                    @Override
                    public void onClick(View v) {
                        AlarmManager am = (AlarmManager) getSystemService(Context.ALARM_SERVICE);
                        Intent intent = new Intent(TakeDrugs.this, AlarmReceiverDrugsBis.class);
                        PendingIntent pendingintent = PendingIntent.getBroadcast(TakeDrugs.this, ALARM_ID, intent, 0);
                        am.cancel(pendingintent);

                        Calendar calendar = Calendar.getInstance();
                        calendar.setTimeInMillis(System.currentTimeMillis());
                        calendar.add(Calendar.MINUTE, 20);
                        am.set(AlarmManager.RTC_WAKEUP, calendar.getTimeInMillis(), _pendingIntent);

                    }});
            }
        }.start();
    }
}
