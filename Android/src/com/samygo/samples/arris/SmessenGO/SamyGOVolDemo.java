/* vi: set sw=4 ts=4: */

package com.samples;

import android.app.Activity;
import android.os.Bundle;

import android.graphics.drawable.*;

import android.graphics.*;
import android.graphics.PorterDuff.Mode;
import android.graphics.Bitmap.*;
import android.graphics.drawable.shapes.*;
import android.content.res.Resources;
import android.graphics.Shader.*;
import android.view.View;
import android.view.View.OnTouchListener;
import android.view.MotionEvent;
import android.widget.*;

import java.text.*;
import java.util.*;

public class SamyGOVolDemo extends Activity
{
	private static final DecimalFormat DF = (DecimalFormat) NumberFormat.getInstance(Locale.GERMAN);
	private static final NumberFormat NF = NumberFormat.getInstance(Locale.GERMAN);

	public AnimationDrawable VolAnimation;
	private ImageView mVolBut;

	/** Called when the activity is first created. */
	@Override
		public void onCreate(Bundle savedInstanceState)
		{
			super.onCreate(savedInstanceState);
			setContentView(R.layout.main);
			// Load the ImageView that will host the animation and
			// set its background to our AnimationDrawable XML resource.
			mVolBut = (ImageView)findViewById(R.id.vol_but);
			mVolBut.setBackgroundResource(R.drawable.vol_animation);

			// Get the background, which has been compiled to an AnimationDrawable object.
			VolAnimation = (AnimationDrawable) mVolBut.getBackground();

			mVolBut.setOnTouchListener(new OnTouchListener() {
				public boolean onTouch(View v, MotionEvent event) {
					switch (event.getAction()) {
						case MotionEvent.ACTION_DOWN: {
														  VolAnimation.setVisible(true, true);
														  v.invalidate();
														  break;
						}
						case MotionEvent.ACTION_MOVE: {
														  doVol(v, event);
														  //					  v.invalidate();
														  break;
						} 
						case MotionEvent.ACTION_UP: {
														VolAnimation.setVisible(true, true);
														break;
						}
					}
					return true;	
				}
			});

		}

	/** Inform the view that the window focus has changed. */
	@Override
		public void onWindowFocusChanged(boolean hasFocus) {
			super.onWindowFocusChanged(hasFocus);
			if(hasFocus)
				VolAnimation.start();
		}

	private int doVol(View v, MotionEvent event){
		final int historySize = event.getHistorySize();
		final int pointerCount = event.getPointerCount();
		final double xfact = v.getWidth();
		final double yfact = v.getHeight();
		double x;
		double y;
		double w;
		double ang;

		if(VolAnimation.isRunning())
			VolAnimation.stop();

		Resources res = getResources();

		Bitmap blendX = BitmapFactory.decodeResource(res, R.drawable.cir_bg_progress_p);
		BitmapFactory.Options bfo = new BitmapFactory.Options();
		bfo.inSampleSize = 5;
		Bitmap base = Bitmap.createBitmap(blendX.getWidth(), blendX.getHeight(), blendX.getConfig());
		Bitmap blend = blendX.copy(Config.ARGB_8888, true);

		Paint mTextPaint = new Paint(Paint.ANTI_ALIAS_FLAG);
		mTextPaint.setTextSize((int)(base.getHeight() * 0.15));
		mTextPaint.setTextAlign(Paint.Align.CENTER);
		mTextPaint.setColor(Color.WHITE);

		Canvas canvas = new Canvas();
		Paint mArcPaint = new Paint(Paint.ANTI_ALIAS_FLAG);

		x = event.getX() / xfact * 2 - 1;
		y = 1 - event.getY() / yfact * 2;
		w = x / y;
		if(y < 0){
			ang = 180 + Math.toDegrees(Math.atan(w));
		} else {
			if(x < 0){
				ang = 360 + Math.toDegrees(Math.atan(w));
			} else {
				ang = Math.toDegrees(Math.atan(w));
			}
		}
		long mVolPerc = Math.round(ang / 3.6);
		ang = Math.round(mVolPerc * 3.6);

		Bitmap bmOverlay = base.copy(Config.ARGB_8888, true);
		ArcShape mShape = new ArcShape((float)-90, (float)ang);
		mArcPaint.setXfermode(new PorterDuffXfermode(Mode.SRC_OVER));
		mArcPaint.setShader(new BitmapShader(blend, TileMode.CLAMP, TileMode.CLAMP));
		canvas.setBitmap(bmOverlay);
		mShape.resize(base.getWidth(), base.getHeight());
		mShape.draw(canvas, mArcPaint);

		canvas.drawText(DF.format(mVolPerc), (float)(base.getWidth() * 0.5), (float)(base.getHeight() * 0.75), mTextPaint);

		mVolBut.setImageBitmap(bmOverlay);
		return (int)mVolPerc;
	}

}
