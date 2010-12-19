using System;
using System.Drawing;
using System.Collections.Generic;
using System.IO;
using System.Drawing.Imaging;

namespace FaceDetector
{
	public static class BitmapFilterExtension
	{
		public static Bitmap Binarizate (this Bitmap input, byte threshold)
		{
			Bitmap binarizedImage = (Bitmap)input.Clone ();
			for (int x = 0; x < binarizedImage.Width; x++) {
				for (int y = 0; y < binarizedImage.Height; y++) {
					Color color = binarizedImage.GetPixel (x, y);
					int red = color.R < threshold ? 0 : 255;
					int green = color.G < threshold ? 0 : 255;
					int blue = color.B < threshold ? 0 : 255;
					binarizedImage.SetPixel (x, y, Color.FromArgb (red, green, blue));
				}
			}
			
			return binarizedImage;
		}

		public static Bitmap MedianFilter (this Bitmap Image, int Size)
		{
			System.Drawing.Bitmap TempBitmap = Image;
			System.Drawing.Bitmap NewBitmap = new System.Drawing.Bitmap (TempBitmap.Width, TempBitmap.Height);
			System.Drawing.Graphics NewGraphics = System.Drawing.Graphics.FromImage (NewBitmap);
			NewGraphics.DrawImage (TempBitmap, new System.Drawing.Rectangle (0, 0, TempBitmap.Width, TempBitmap.Height), new System.Drawing.Rectangle (0, 0, TempBitmap.Width, TempBitmap.Height), System.Drawing.GraphicsUnit.Pixel);
			NewGraphics.Dispose ();
			
			int ApetureMin = -(Size / 2);
			int ApetureMax = (Size / 2);
			for (int x = 0; x < NewBitmap.Width; ++x) {
				for (int y = 0; y < NewBitmap.Height; ++y) {
					List<int> RValues = new List<int> ();
					List<int> GValues = new List<int> ();
					List<int> BValues = new List<int> ();
					for (int x2 = ApetureMin; x2 < ApetureMax; ++x2) {
						int TempX = x + x2;
						if (TempX >= 0 && TempX < NewBitmap.Width) {
							for (int y2 = ApetureMin; y2 < ApetureMax; ++y2) {
								int TempY = y + y2;
								if (TempY >= 0 && TempY < NewBitmap.Height) {
									Color TempColor = TempBitmap.GetPixel (TempX, TempY);
									RValues.Add (TempColor.R);
									GValues.Add (TempColor.G);
									BValues.Add (TempColor.B);
								}
							}
						}
					}
					RValues.Sort ();
					GValues.Sort ();
					BValues.Sort ();
					Color MedianPixel = Color.FromArgb (RValues[RValues.Count / 2], GValues[GValues.Count / 2], BValues[BValues.Count / 2]);
					NewBitmap.SetPixel (x, y, MedianPixel);
				}
			}
			return NewBitmap;
		}

		public static Bitmap Scale (this Bitmap Input, int Width, int Height)
		{
			return new Bitmap (Input, new Size (Width, Height));
		}

		public static Bitmap ConvertToGrayScale (this Bitmap input)
		{
			Bitmap image = (Bitmap)input.Clone ();
			
			for (int x = 0; x < input.Width; x++) {
				for (int y = 0; y < input.Height; y++) {
					Color color = input.GetPixel (x, y);
					int value = (color.R + color.G + color.B) / 3;
					image.SetPixel (x, y, Color.FromArgb (value, value, value));
				}
			}
			
			return image;
		}

		public static Bitmap Difference (this Bitmap input, Bitmap image)
		{
			
			if (input.Width != image.Width || input.Height != image.Height) {
				throw new ArgumentException ("Incompatible input image size", "image");
			}
			
			Bitmap result = new Bitmap (input.Width, input.Height);
			
			for (int x = 0; x < input.Width; x++) {
				for (int y = 0; y < input.Height; y++) {
					Color c1 = input.GetPixel (x, y);
					Color c2 = image.GetPixel (x, y);
					result.SetPixel (x, y, Color.FromArgb (Math.Max (0, c1.R - c2.R), Math.Max (0, c1.G - c2.G), Math.Max (0, c1.B - c2.B)));
				}
			}
			
			return result;
		}

		public static Bitmap RedAndBlueDifference (this Bitmap input)
		{
			Bitmap image = (Bitmap)input.Clone ();
			
			for (int x = 0; x < image.Width; x++) {
				for (int y = 0; y < image.Height; y++) {
					Color color = image.GetPixel (x, y);
					int value = Math.Abs (color.R - color.B);
					image.SetPixel (x, y, Color.FromArgb (value, value, value));
				}
			}
			
			return image;
		}

		public static Bitmap RedAndGreenDifference (this Bitmap input)
		{
			Bitmap image = (Bitmap)input.Clone ();
			
			for (int x = 0; x < image.Width; x++) {
				for (int y = 0; y < image.Height; y++) {
					Color color = image.GetPixel (x, y);
					int value = Math.Abs (color.R - color.G);
					image.SetPixel (x, y, Color.FromArgb (value, value, value));
				}
			}
			
			return image;
		}

		public static Bitmap RGBDifference (this Bitmap input)
		{
			Bitmap image = (Bitmap)input.Clone ();
			
			for (int x = 0; x < image.Width; x++) {
				for (int y = 0; y < image.Height; y++) {
					Color color = image.GetPixel (x, y);
					int value = (int)(Math.Abs (2 * color.R - color.G - color.B) * 0.5);
					image.SetPixel (x, y, Color.FromArgb (value, value, value));
				}
			}
			
			return image;
		}

		public static Bitmap Normalize (this Bitmap input)
		{
			Bitmap image = (Bitmap)input.Clone ();
			byte min = image.GetPixel (0, 0).R;
			byte max = image.GetPixel (0, 0).R;
			
			for (int x = 0; x < image.Width; x++) {
				for (int y = 0; y < image.Height; y++) {
					Color pixel = image.GetPixel (x, y);
					if (pixel.R < min) {
						min = pixel.R;
					} else if (pixel.R > max) {
						max = pixel.R;
					}
				}
			}
			
			for (int x = 0; x < image.Width; x++) {
				for (int y = 0; y < image.Height; y++) {
					Color pixel = image.GetPixel (x, y);
					int value = 255 * (pixel.R - min) / (max - min);
					image.SetPixel (x, y, Color.FromArgb (value, value, value));
				}
			}
			
			//I2*=255(I2-MIN)/(MAX-MIN)
			
			return image;
		}

		public static Bitmap RhombErode (this Bitmap input)
		{
			return Erode (input, new short[,] { { 0, 1, 0 }, { 1, 1, 1 }, { 0, 1, 0 } });
		}

		public static Bitmap SquareErode (this Bitmap input)
		{
			return Erode (input, new short[,] { { 1, 1, 1 }, { 1, 1, 1 }, { 1, 1, 1 } });
		}

		public static Bitmap Erode (this Bitmap input, short[,] mask)
		{
			Bitmap image = (Bitmap)input.Clone ();
			
			short size = 0;
			foreach (short i in mask) {
				if (i == 1) {
					size++;
				}
			}
			
			for (int x = 0; x < input.Width; x++) {
				for (int y = 0; y < input.Height; y++) {
					if (x + 2 < input.Width && y + 2 < input.Height) {
						int[] values = new int[size];
						int count = 0;
						
						for (short column = 0; column < 3; column++) {
							for (short row = 0; row < 3; row++) {
								if (mask[column, row] == 1) {
									values[count++] = input.GetPixel (x + column, y + row).R;
								}
							}
						}
						
						Array.Sort (values);
						image.SetPixel (x + 1, y + 1, Color.FromArgb (values[0], values[0], values[0]));
					}
				}
			}
			
			return image;
		}
	}
}
