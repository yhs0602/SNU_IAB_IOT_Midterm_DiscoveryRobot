package com.kyhsgeekcode.snu_iab_iot_controllerapp;

import android.content.Context;
import android.content.res.TypedArray;
import android.graphics.Bitmap;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.graphics.drawable.Drawable;
import android.text.TextPaint;
import android.util.AttributeSet;
import android.util.Log;
import android.view.View;
import android.widget.RadioButton;

import java.net.URI;
import java.net.URISyntaxException;
import java.util.Arrays;
import java.util.HashMap;
import java.util.Map;

import tech.gusavila92.websocketclient.WebSocketClient;

/**
 * Streams video via websocket, from ESP32 board AP server
 */
public class WebsocketStreamVideoView extends View {
    private static final String TAG = "WebSockVideoView";

    int ln=0; //the line #, when sent partially
    int flag;
    private String camera_ip;
    int xres, yres;
    Bitmap targetBitmap;
    Canvas targetCanvas;
    byte [] imgData;
    public WebsocketStreamVideoView(Context context) {
        super(context);
        init(null, 0);
    }

    public WebsocketStreamVideoView(Context context, AttributeSet attrs) {
        super(context, attrs);
        init(attrs, 0);
    }

    public WebsocketStreamVideoView(Context context, AttributeSet attrs, int defStyle) {
        super(context, attrs, defStyle);
        init(attrs, defStyle);
    }

    private void init(AttributeSet attrs, int defStyle) {
        // Load attributes
        final TypedArray a = getContext().obtainStyledAttributes(
                attrs, R.styleable.WebsocketStreamVideoView, defStyle, 0);

        // Set up a default TextPaint object

        // Update TextPaint and text measurements from attributes
        invalidateTextPaintAndMeasurements();
    }

    private void invalidateTextPaintAndMeasurements() {
//        mTextHeight = fontMetrics.bottom;
    }

    @Override
    protected void onDraw(Canvas canvas) {
        super.onDraw(canvas);

        // TODO: consider storing these as member variables to reduce
        // allocations per draw cycle.
        int paddingLeft = getPaddingLeft();
        int paddingTop = getPaddingTop();
        int paddingRight = getPaddingRight();
        int paddingBottom = getPaddingBottom();

        int contentWidth = getWidth() - paddingLeft - paddingRight;
        int contentHeight = getHeight() - paddingTop - paddingBottom;

        // Draw the text.
//        canvas.drawText(mExampleString,
//                paddingLeft + (contentWidth - mTextWidth) / 2,
//                paddingTop + (contentHeight + mTextHeight) / 2,
//                mTextPaint);
//
//        // Draw the example drawable on top of the text.
//        if (mExampleDrawable != null) {
//            mExampleDrawable.setBounds(paddingLeft, paddingTop,
//                    paddingLeft + contentWidth, paddingTop + contentHeight);
//            mExampleDrawable.draw(canvas);
//        }
    }

    private WebSocketClient webSocketClient;

    private void createWebSocketClient(String struri) {
        URI uri;
        try {
            uri = new URI(struri);
        }
        catch (URISyntaxException e) {
            e.printStackTrace();
            return;
        }

        webSocketClient = new WebSocketClient(uri) {
            @Override
            public void onOpen() {
                Log.v(TAG,"onOpen");
                //webSocketClient.send("Hello, World!");
            }

            @Override
            public void onTextReceived(String message) {
                Log.v(TAG,"onTextReceived:"+message);
            }

            @Override
            public void onBinaryReceived(byte[] data) {
                Log.v(TAG,"onBinaryReceived:"+ Arrays.toString(data));
                if(data.length == 1)
                {
                    flag = data[0];
                    switch(data[0])
                    {
                        case (byte)0xaa:
                            ln = 1;
                            break;
                        case (byte)0xFF:
                            Log.v(TAG,"Last block");
                            break;
                        case (byte) 0x11:
                            Log.v(TAG, "CAM IP");
                    }
                } else {
                    if(flag == 0x11)
                    {
                        camera_ip = new String(data);
                        flag = 0;
                    } else {
                        Display(data, flag);
                    }
                }
            }

            @Override
            public void onPingReceived(byte[] data) {
                Log.v(TAG,"onPingReceived:"+Arrays.toString(data));
            }

            @Override
            public void onPongReceived(byte[] data) {
                Log.v(TAG,"onPongReceived:"+Arrays.toString(data));
            }

            @Override
            public void onException(Exception e) {
                Log.e(TAG,e.getMessage());
            }

            @Override
            public void onCloseReceived() {
                Log.v(TAG,"onCloseReceived");
            }
        };

        webSocketClient.setConnectTimeout(10000);
        webSocketClient.setReadTimeout(60000);
        //webSocketClient.addHeader("arduino", "http://developer.example.com");
        webSocketClient.enableAutomaticReconnection(5000);
        webSocketClient.connect();
    }

    private void Display(byte[] pixels, int flag) {
        //alert('display');
        int i = 0;
        for(int y=0; y < yres; y++) {
            for(int x=0; x < xres; x++)
            {
                i = (y * xres + x) << 1;
                int pixel16 = (0xffff & pixels[i]) | ((0xffff & pixels[i+1]) << 8);
                //비트맵에 하나하나 그린다.
                //RGBA
                imgData[ln+0] = (byte)(((((pixel16 >> 11) & 0x1F) * 527) + 23) >> 6);
                imgData[ln+1] = (byte)(((((pixel16 >> 5) & 0x3F) * 259) + 33) >> 6);
                imgData[ln+2] = (byte)((((pixel16 & 0x1F) * 527) + 23) >> 6);
                imgData[ln+3] = (byte)((0xFFFFFFFF) & 255);
                ln += 4;
            }
        }

        if (flag == 0xFF) { // last block
            ln = 0;
            targetBitmap = Bitmap.createBitmap(imgData,resx,resy,)
            postInvalidate();
//            ctx.putImageData(imgData,0,0);
            webSocketClient.send("next-frame");
        }
    }


    public void setUriStr(String uriStr) {
        createWebSocketClient(uriStr);
    }

    public void controlBot(int id) {
        String cmd = id2Command.get(id);
        if(cmd !=null)
        {
            webSocketClient.send(cmd);
            Log.v(TAG, "Sent "+cmd + "for "+id);
        }
        else
            Log.e(TAG, "command was null; id="+id);
    }
    private static final Map<Integer, String> id2Command = new HashMap<>();
    static {
        id2Command.put(R.id.buttonBack,"motor_back");
        id2Command.put(R.id.buttonStraight,"motor_straight");
        id2Command.put(R.id.buttonStop,"motor_stop");
        id2Command.put(R.id.buttonRight,"motor_turnright");
        id2Command.put(R.id.buttonLeft,"motor_turnleft");

    }

    public void setResolution(String text) {
        switch (text)
        {
            case "QQQ-VGA":
                xres = 80;
                yres = 60;

                webSocketClient.send("canvas-QQQ-VGA");
                break;
            case "QQ-VGA":
                xres = 160;
                yres = 120;
                webSocketClient.send("canvas-QQ-VGA");
                break;
            case "Q-VGA":
                xres = 320;
                yres = 120;
                webSocketClient.send("canvas-Q-VGA");
                break;
        }
    }
}
