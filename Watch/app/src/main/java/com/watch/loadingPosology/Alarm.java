package com.watch.loadingPosology;

/**
 * Created by Thomas on 05/01/2015.
 */

import android.text.format.Time;

import java.io.Serializable;

public class Alarm implements Serializable{
    private static final long serialVersionUID = 1L;
    private Time _hour;
    private boolean _active = true;

    public Time getHour() {
        return _hour;
    }

    public void setHour(Time hour) {
        _hour = hour;
    }

    public boolean isActive() {
        return _active;
    }

    public void setActive(boolean active) {
        _active = active;
    }
}