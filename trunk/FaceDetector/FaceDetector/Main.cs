using System;
using System.Drawing;
using System.Collections.Generic;
using System.IO;
using System.Drawing.Imaging;

namespace FaceDetector
{
	class MainClass
	{
		private static ImageCodecInfo getEncoderInfo (string mimeType)
		{
			// Get image codecs for all image formats
			ImageCodecInfo[] codecs = ImageCodecInfo.GetImageEncoders ();
			
			// Find the correct image codec
			for (int i = 0; i < codecs.Length; i++)
				if (codecs[i].MimeType == mimeType)
					return codecs[i];
			return null;
		}

		public static void Main (string[] args)
		{
			//DirectoryInfo dir = new DirectoryInfo("/home/lhanusiak/input");
			DirectoryInfo dir = new DirectoryInfo ("/home/lhanusiak/msc_input");
			FileInfo[] jpgFiles = dir.GetFiles ("*.jpg");
			
			// // Encoder parameter for image quality
			//
			// EncoderParameter qualityParam = new EncoderParameter(Encoder.Quality, 100);
			//
			// // Jpeg image codec
			// ImageCodecInfo jpegCodec = getEncoderInfo("image/jpeg");
			//
			// EncoderParameters encoderParams = new EncoderParameters(1);
			// encoderParams.Param[0] = qualityParam;
						
			foreach (FileInfo file in jpgFiles) {
				Console.WriteLine ("Reading file: " + file.FullName);
				
				Console.WriteLine("Detecting skin in " + file.FullName);
				Bitmap inputBmp = (Bitmap) Bitmap.FromFile (file.FullName);
				Bitmap skin_step1 = inputBmp.SkinColorBinarization ();
				//skin_step1.Save ("/home/lhanusiak/Desktop/msc_output/skin_s1_" + file.Name);
				Console.WriteLine("Skin quantization");
				Bitmap skin_step2 = skin_step1.Quantization();
				//skin_step2.Save ("/home/lhanusiak/Desktop/msc_output/skin_s2_" + file.Name);
				Console.WriteLine("Skin component labeling");
				List<MarkerRegion> skinRegions = skin_step2.ComponentLabeling();
				//Bitmap skin_step3 = inputBmp.DrawFaceMarker(face);
				//skin_step3.Save ("/home/lhanusiak/Desktop/msc_output/skin_s3_" + file.Name);
				//skinRegions.Sort ();
				//skinRegions[skinRegions.Count - 1].Points.ForEach (delegate(Point p) { inputBmp.SetPixel (p.X, p.Y, Color.Red); });
				//inputBmp.DrawRegionMarker(skinRegions[skinRegions.Count - 1], Color.Red).Save("/home/lhanusiak/Desktop/msc_output/skin_marker_" + file.Name);
				
				Console.WriteLine("Detecting hair in " + file.FullName);
				Bitmap hair_step1 = ((Bitmap)Bitmap.FromFile (file.FullName)).HairColorBinarization ();
				//hair_step1.Save ("/home/lhanusiak/Desktop/msc_output/hair_s1_" + file.Name);
				Console.WriteLine("Hair quantization");
				Bitmap hair_step2 = hair_step1.Quantization();
				//hair_step2.Save ("/home/lhanusiak/Desktop/msc_output/hair_s2_" + file.Name);
				Console.WriteLine("Hair component labeling");
				List<MarkerRegion> hairRegions = hair_step2.ComponentLabeling();
				
				foreach( MarkerRegion skin in skinRegions){
					Face f = new Face(skin);
					
					
				}
				
			}			
		}
	}
}