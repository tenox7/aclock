
using System;
using System.Collections.Generic;
using System.Linq;
using MonoTouch.Foundation;
using MonoTouch.UIKit;
using MonoTouch.CoreGraphics;

namespace aClockMonoTouch
{
	public class Application
	{
		static void Main (string[] args)
		{
			UIApplication.Main (args);
		}
	}

	// The name AppDelegate is referenced in the MainWindow.xib file.
	public partial class AppDelegate : UIApplicationDelegate
	{
		private static MonoTouch.ObjCRuntime.Selector selector = new MonoTouch.ObjCRuntime.Selector("UpdateTimer");
		private bool ipad;
		CGBitmapContext ctx;
		
		// This method is invoked when the application has loaded its UI and its ready to run
		public override bool FinishedLaunching (UIApplication app, NSDictionary options)
		{
			// If you have defined a view, add it here:
			// window.AddSubview (navigationController.View);
			
			if(UIDevice.CurrentDevice.Model.ToLower().Contains("ipad"))
				ipad = true;
			else
				ipad = false;

			window.AddSubview(imageView);
			
			window.MakeKeyAndVisible ();
			
			NSTimer.CreateScheduledTimer(0.01, this, selector, null, true);
			
			return true;
		}

		// This method is required in iPhoneOS 3.0
		public override void OnActivated (UIApplication application)
		{
		}
		
		public override void ReceiveMemoryWarning (UIApplication application)
		{
			GC.Collect();
		}
		
		[Export ("UpdateTimer")]
		public void UpdateTimer()
		{
			DrawClock();
		}
		
		public void DrawClock()
		{
			if(imageView.Image!=null)
				imageView.Image.Dispose();
			if(ctx!=null)
				ctx.Dispose();
			ctx = new CGBitmapContext(IntPtr.Zero, (int)imageView.Bounds.Width, (int)imageView.Bounds.Height, 8, 4 * (int)imageView.Bounds.Width, CGColorSpace.CreateDeviceRGB(), CGImageAlphaInfo.PremultipliedFirst);
			GC.Collect();
			CGColor red = new CGColor(1f, 0f, 0f, 1f);
			CGColor black = new CGColor(0f, 0f, 0f, 1f);
			DateTime now = DateTime.Now;
			
			int big_hand_width, small_hand_width, big_square_size, small_square_size;
			
			if(ipad)
			{
				big_hand_width = 16;
				small_hand_width = 10;
				big_square_size = 20;
				small_square_size = 10;
			}
			else
			{
				big_hand_width = 8;
				small_hand_width = 5;
				big_square_size = 10;
				small_square_size = 5;
			}
			
			float x, y;
			int r;
			int sXmax = (int)imageView.Bounds.Width;
			int sYmax = (int)imageView.Bounds.Width-big_square_size;
			
			int smax;
			
			if(sXmax<=sYmax)
				smax=sXmax;
			else
				smax=sYmax;
			
			int hand_max = (smax/2)-1;

			int sXcen = sXmax/2;
			int sYcen = sYmax/2;
			
			#region Draw circle
			for(r=0;r<60;r++){
				x=((float)Math.Cos(r*Math.PI/180*6)*hand_max)+sXcen;
				y=((float)Math.Sin(r*Math.PI/180*6)*hand_max)+sYcen;
		
				switch (r) {
					case 0:
					case 5:
					case 10:
					case 15:
					case 20:
					case 25:
					case 30:
					case 35:
					case 40:
					case 45:
					case 50:
					case 55:
						ctx.SetFillColorWithColor(red);
						ctx.FillRect(new System.Drawing.RectangleF(y, x, big_square_size, big_square_size));
						break;
					default:
						ctx.SetFillColorWithColor(black);
						ctx.FillRect(new System.Drawing.RectangleF(y, x, small_square_size, small_square_size));
						break;
				}
			}
			#endregion
			
			#region Draw hour hand
			ctx.SetStrokeColorWithColor(black);
			float r_hour = (float)((30 * (now.Hour%12) + now.Minute*0.5)*(Math.PI / 180));
			ctx.MoveTo(sXcen, sYcen);
			ctx.SetLineWidth(big_hand_width);
			ctx.AddLineToPoint((float)(Math.Sin(r_hour)*(3*hand_max/5)+sYcen), (float)(Math.Cos(r_hour)*(3*hand_max/5)+sXcen));
			ctx.StrokePath();
			#endregion

			#region Draw minute hand
			ctx.SetStrokeColorWithColor(black);
			float r_minute = (float)((6 * now.Minute)*(Math.PI / 180));
			ctx.MoveTo(sXcen, sYcen);
			ctx.SetLineWidth(small_hand_width);
			ctx.AddLineToPoint((float)(Math.Sin(r_minute)*(hand_max-5)+sYcen), (float)(Math.Cos(r_minute)*(hand_max-5)+sXcen));
			ctx.StrokePath();
			#endregion

			#region Draw minute hand
			ctx.SetStrokeColorWithColor(red);
			float r_second = (float)((6 * now.Second)*(Math.PI / 180));
			ctx.MoveTo(sXcen, sYcen);
			ctx.SetLineWidth(5);
			ctx.AddLineToPoint((float)(Math.Sin(r_second)*(hand_max-5)+sYcen), (float)(Math.Cos(r_second)*(hand_max-5)+sXcen));
			ctx.StrokePath();
			#endregion
			
			float aclockX, aclockY, digital_timeX, digital_timeY, font_size;
			
			if(ipad)
			{
				font_size = 32f;
				aclockX = imageView.Bounds.GetMaxX()/2-80;
				aclockY = imageView.Bounds.GetMaxY()-100;
				digital_timeX = imageView.Bounds.GetMaxX()/2-70;
				digital_timeY = imageView.Bounds.GetMaxY()-150;
			}
			else
			{
				font_size = 16f;
				aclockX = imageView.Bounds.GetMaxX()/2-40;
				aclockY = imageView.Bounds.GetMaxY()-50;
				digital_timeX = imageView.Bounds.GetMaxX()/2-35;
				digital_timeY = imageView.Bounds.GetMaxY()-100;
			}

			
			ctx.SetFillColorWithColor(black);
			ctx.SelectFont("American Typewriter", font_size, CGTextEncoding.MacRoman);
			ctx.SetTextDrawingMode(CGTextDrawingMode.Fill);
			ctx.ShowTextAtPoint(aclockX, aclockY, ".:ACLOCK:.");
			
			string digital_time;
			
			if(now.Second%2==0)
				digital_time=String.Format("{0}:{1}:{2}", now.Hour.ToString("00"), now.Minute.ToString("00"), now.Second.ToString("00"));
			else
				digital_time=String.Format("{0} {1} {2}", now.Hour.ToString("00"), now.Minute.ToString("00"), now.Second.ToString("00"));
			
			ctx.SetFillColorWithColor(black);
			ctx.SelectFont("American Typewriter", font_size, CGTextEncoding.MacRoman);
			ctx.SetTextDrawingMode(CGTextDrawingMode.Fill);
			ctx.ShowTextAtPoint(digital_timeX, digital_timeY, digital_time);
			
			imageView.Image = UIImage.FromImage(ctx.ToImage());
			ctx.Dispose();
		}
	}
}