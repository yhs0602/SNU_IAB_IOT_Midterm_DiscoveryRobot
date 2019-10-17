package com.kyhsgeekcode.snu_iab_iot_controllerapp;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.RadioButton;
import android.widget.RadioGroup;
import android.widget.Toast;

import java.net.URI;
import java.net.URISyntaxException;
import java.util.Arrays;

import tech.gusavila92.websocketclient.WebSocketClient;

public class MainActivity extends AppCompatActivity implements View.OnClickListener, RadioGroup.OnCheckedChangeListener {
    private static final String TAG = "IOT_Main";
    WebsocketStreamVideoView videoView;
    Button btStraight, btStop, btBack, btLeft, btRight;
    EditText etIP;
    Button btConnect;
    RadioGroup rgResolution;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        etIP = findViewById(R.id.editTextIP);
        rgResolution = findViewById(R.id.rgResolutions);
        rgResolution.setOnCheckedChangeListener(this);
        btConnect = findViewById(R.id.buttonConnect);
        btConnect.setOnClickListener(this);
        videoView = findViewById(R.id.wsvideo);
        btStraight = findViewById(R.id.buttonStraight);
        btStop = findViewById(R.id.buttonStop);
        btBack = findViewById(R.id.buttonBack);
        btLeft = findViewById(R.id.buttonLeft);
        btRight = findViewById(R.id.buttonRight);
        btStraight.setOnClickListener(this);
        btBack.setOnClickListener(this);
        btStop.setOnClickListener(this);
        btLeft.setOnClickListener(this);
        btRight.setOnClickListener(this);
    }
    @Override
    public void onClick(View v) {
        int id = v.getId();
        switch (id)
        {
            case R.id.buttonConnect:
                String strIP = etIP.getText().toString();
                String uriStr = "ws://"+strIP+":81";
                videoView.setUriStr(uriStr);
                Toast.makeText(this, "Connected",Toast.LENGTH_SHORT).show();
                break;
            default:
                videoView.controlBot(id);
        }
    }

    @Override
    public void onCheckedChanged(RadioGroup group, int checkedId) {
        videoView.setResolution(((String)((RadioButton)findViewById(checkedId)).getText()));
    }
}
