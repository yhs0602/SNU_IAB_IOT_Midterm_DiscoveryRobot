package com.kyhsgeekcode.snu_iab_iot_controllerapp;

import android.content.Context;
import android.content.res.TypedArray;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.graphics.drawable.Drawable;
import android.text.TextPaint;
import android.util.AttributeSet;
import android.util.Log;
import android.view.View;

import java.net.URI;
import java.net.URISyntaxException;
import java.util.Arrays;

import tech.gusavila92.websocketclient.WebSocketClient;

/**
 * TODO: document your custom view class.
 */
public class WebsocketStreamVideoView extends View {
    private static final String TAG = "WebSockVideoView";

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


    public void setUriStr(String uriStr) {
        createWebSocketClient(uriStr);
    }
}
