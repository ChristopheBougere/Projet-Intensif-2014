package push;

import android.app.Activity;
import android.content.Intent;
import android.net.Uri;
import android.os.Bundle;
import android.util.Log;
import android.webkit.WebChromeClient;
import android.webkit.WebSettings;
import android.webkit.WebView;
import android.webkit.WebViewClient;
import android.widget.MediaController;
import android.widget.VideoView;

import com.gcm.R;

/**
 * Created by Thomas on 06/01/2015.
 */
public class Stream extends Activity {

    private String _url = "";//"http://192.168.5.102:8554";
    private WebView _mWebView;

    @Override

    public void onCreate(Bundle savedInstanceState) {

        super.onCreate(savedInstanceState);
        setContentView(R.layout.stream);

        _url = getIntent().getStringExtra("link");

        Log.e("kkurl", _url);
        Log.e("kkurl", "kkkk :"+_url.equals("http://192.168.5.102:8554"));
        VideoView videoView = (VideoView) findViewById(R.id.myvideoview);
        MediaController mc = new MediaController(this);
        mc.setAnchorView(videoView);
        mc.setMediaPlayer(videoView);
        videoView.setMediaController(mc);
        videoView.setVideoURI(Uri.parse(_url));

        videoView.requestFocus();
        videoView.start();
    }
}
