using System;
using System.Drawing;
using System.Collections.Generic;
using System.IO;
using System.Drawing.Imaging;


namespace FaceDetector
{
	public static class DetectorAlgorithms
	{
		public static double SkinUpperLimit (double red)
		{
			// F1 ( r ) = −1.376r 2 + 1.0743r + 0.2
			return -1.376 * Math.Pow (red, 2) + 1.0743 * red + 0.2;
		}

		public static double SkinLowerLimit (double red)
		{
			// F2 ( r ) = −0.776r 2 + 0.5601r + 0.18
			return -0.776 * Math.Pow (red, 2) + 0.5601 * red + 0.18;
		}

		public static double ConvertFromRGBtoHSI (Color pixel)
		{
			double a = 0.5 * ((pixel.R - pixel.G) + (pixel.R - pixel.B));
			double b = Math.Pow (pixel.R - pixel.G, 2) + (pixel.R - pixel.B) * (pixel.G - pixel.B);
			
			return Math.Acos (a / Math.Sqrt (b));
		}

		public static Bitmap SkinColorBinarization (this Bitmap input)
		{
			Bitmap image = (Bitmap)input.Clone ();
			
			for (int x = 0; x < input.Width; x++) {
				for (int y = 0; y < input.Height; y++) {
					Color pixel = input.GetPixel (x, y);
					double sum = ((double)pixel.R) + ((double)pixel.G) + ((double)pixel.B);
					double r = ((double)pixel.R) / sum;
					double g = ((double)pixel.G) / sum;
					double w = Math.Pow (r - 0.33, 2) + Math.Pow (g - 0.33, 2);
					double radians = ConvertFromRGBtoHSI (pixel);
					double phi = radians * 180.0 / Math.PI;
					double h = (pixel.B <= pixel.G) ? phi : 360.0 - phi;
					
					if ((h > 240 || h <= 35) && (w > 0.001) && (g < SkinUpperLimit (r)) && (g > SkinLowerLimit (r))) {
						image.SetPixel (x, y, Color.White);
					} else {
						image.SetPixel (x, y, Color.Black);
					}
				}
			}
			
			return image;
		}

		public static Bitmap HairColorBinarization (this Bitmap input)
		{
			Bitmap image = (Bitmap)input.Clone ();
			
			for (int x = 0; x < input.Width; x++) {
				for (int y = 0; y < input.Height; y++) {
					Color pixel = input.GetPixel (x, y);
					double intensity = (pixel.R + pixel.G + pixel.B) / 3;
					
					if ((intensity < 80) && ((pixel.B - pixel.G < 15) || (pixel.B - pixel.R < 15))) {
						image.SetPixel (x, y, Color.White);
					} else {
						image.SetPixel (x, y, Color.Black);
					}
				}
			}
			
			return image;
		}

		public static int CountBlackPixels (this Bitmap input, int x0, int y0, int size)
		{
			int count = 0;
			
			for (int x = x0; x < x0 + size; x++) {
				for (int y = y0; y < y0 + size; y++) {
					
					int cordX = getPeriodicCoordinate (x, input.Width);
					int cordY = getPeriodicCoordinate (y, input.Height);
					
					Color pixel = input.GetPixel (cordX, cordY);
					
					if (Color.Black.ToArgb () == pixel.ToArgb ()) {
						count++;
					}
				}
			}
			
			return count;
		}

		public static Bitmap FillWithColor (this Bitmap input, int x0, int y0, int size, Color color)
		{
			for (int x = x0; x < x0 + size; x++) {
				for (int y = y0; y < y0 + size; y++) {
					
					int cordX = getPeriodicCoordinate (x, input.Width);
					int cordY = getPeriodicCoordinate (y, input.Height);
					
					input.SetPixel (cordX, cordY, color);
				}
			}
			
			return input;
		}

		public static int getPeriodicCoordinate (int x, int total)
		{
			if (x < 0)
				return total + x; else if (x >= total)
				return x - total;
			else
				return x;
		}

		public static Bitmap Quantization (this Bitmap input)
		{
			Bitmap image = (Bitmap)input.Clone ();
			
			for (int x = 0; x < input.Width; x += 5) {
				for (int y = 0; y < input.Height; y += 5) {
					int blackPixels = input.CountBlackPixels (x, y, 5);
					
					Color color = (blackPixels > 12) ? Color.Black : Color.White;
					
					image = image.FillWithColor (x, y, 5, color);
				}
			}
			
			return image;
		}

		public static Bitmap ComponentLabeling (this Bitmap input)
		{
			Bitmap image = (Bitmap) input.Clone ();
			List<Region> list = new List<Region> ();
			
			for (int x = 0; x < image.Width; x++) {
				for (int y = 0; y < image.Height; y++) {
					Color color = image.GetPixel (x, y);
					if (color.ToArgb () == Color.White.ToArgb ()) {
						image.SetPixel (x, y, Color.Black);
						Region area = new Region (list.Count + 1);
						area.AddPoint (new Point (x, y));
						
						List<Point> neighbors = image.getNeighbors (x, y);
						for (int idx = 0; idx < neighbors.Count; idx++) {
							Point p = neighbors[idx];
							neighbors.AddRange (image.getNeighbors (p.X, p.Y));
						}
						
						area.AddPoints (neighbors);
						list.Add (area);
					}
				}
			}
			
			foreach (Region r in list) {
				foreach (Point p in r.Points) {
					Color color = Color.FromArgb (50 * r.Marker, 50 * r.Marker, 50 * r.Marker);
					image.SetPixel (p.X, p.Y, color);
				}
			}
			
			return image;
		}

		public static List<Point> getNeighbors (this Bitmap image, int x, int y)
		{
			List<Point> neighbors = new List<Point> ();
			
			int x_l = (x - 1 >= 0) ? x - 1 : x;
			int x_r = (x + 1 < image.Width) ? x + 1 : x;
			int y_b = (y - 1 >= 0) ? y - 1 : y;
			int y_t = (y + 1 < image.Height) ? y + 1 : y;
			
			int white = Color.White.ToArgb ();
			
			if (image.GetPixel (x, y_b).ToArgb () == white) {
				neighbors.Add (new Point (x, y_b));
				image.SetPixel (x, y_b, Color.Black);
			}
			
			if (image.GetPixel (x, y_t).ToArgb () == white) {
				neighbors.Add (new Point (x, y_t));
				image.SetPixel (x, y_t, Color.Black);
			}
			
			if (image.GetPixel (x_l, y).ToArgb () == white) {
				neighbors.Add (new Point (x_l, y));
				image.SetPixel (x_l, y, Color.Black);
			}
			
			if (image.GetPixel (x_l, y_b).ToArgb () == white) {
				neighbors.Add (new Point (x_l, y_b));
				image.SetPixel (x_l, y_b, Color.Black);
			}
			
			if (image.GetPixel (x_l, y_t).ToArgb () == white) {
				neighbors.Add (new Point (x_l, y_t));
				image.SetPixel (x_l, y_t, Color.Black);
			}
			
			if (image.GetPixel (x_r, y).ToArgb () == white) {
				neighbors.Add (new Point (x_r, y));
				image.SetPixel (x_r, y, Color.Black);
			}
			
			if (image.GetPixel (x_r, y_b).ToArgb () == white) {
				neighbors.Add (new Point (x_r, y_b));
				image.SetPixel (x_r, y_b, Color.Black);
			}
			
			if (image.GetPixel (x_r, y_t).ToArgb () == white) {
				neighbors.Add (new Point (x_r, y_t));
				image.SetPixel (x_r, y_t, Color.Black);
			}
			
			return neighbors;
		}

		public class Region
		{
			public Region (int marker)
			{
				Marker = marker;
			}

			public int Marker { get; private set; }

			public List<Point> Points { get; private set; }

			public void AddPoint (Point p)
			{
				if (Points == null) {
					Points = new List<Point> ();
				}
				
				Points.Add (p);
			}
			
			public void AddPoints(List<Point> points)
			{
				if (Points == null) {
					Points = new List<Point> ();
				}
				
				Points.AddRange(points);
			}
		}
	}
}
