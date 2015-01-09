package com.gcm;

/**
 * Created by Thomas on 05/01/2015.
 */

public interface Config {

    // used to share GCM regId with application server - using php app server
    static final String APP_SERVER_URL = "http://www.ecole.ensicaen.fr/~bukowski/intensif/send.php?shareRegId=1";

    // GCM server using java
    // static final String APP_SERVER_URL =
    // "http://192.168.1.17:8080/GCM-App-Server/GCMNotification?shareRegId=1";

    // Google Project Number
    static final String GOOGLE_PROJECT_ID = "316451856179";
    static final String MESSAGE_KEY = "m";

    //Phone number

}
