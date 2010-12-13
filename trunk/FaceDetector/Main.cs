using System;
using System.Drawing;
using System.Collections.Generic;
using System.IO;

//using System;
//using System.Collections.Generic;
//using System.ComponentModel;
//using System.Data;
//using System.Drawing;
//using System.Drawing.Drawing2D;
//using System.Windows.Forms;
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
			// DirectoryInfo dir = new DirectoryInfo("/home/lhanusiak/input");
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
				Bitmap skin_step1 = ((Bitmap)Bitmap.FromFile (file.FullName)).SkinColorBinarization ();
				skin_step1.Save ("/home/lhanusiak/Desktop/msc_output/skin_s1_" + file.Name);
				Console.WriteLine("Skin Quantization");
				Bitmap skin_step2 = skin_step1.Quantization();
				skin_step2.Save ("/home/lhanusiak/Desktop/msc_output/skin_s2_" + file.Name);
				Console.WriteLine("Component Labeling");
				Bitmap skin_step3 = skin_step2.ComponentLabeling();
				skin_step3.Save ("/home/lhanusiak/Desktop/msc_output/skin_s3_" + file.Name);
				
				Console.WriteLine("Detecting hair in " + file.FullName);
				Bitmap hair_step1 = ((Bitmap)Bitmap.FromFile (file.FullName)).HairColorBinarization ();
				hair_step1.Save ("/home/lhanusiak/Desktop/msc_output/hair_s1_" + file.Name);
				Console.WriteLine("Hair Quantization");
				hair_step1.Quantization().Save ("/home/lhanusiak/Desktop/msc_output/hair_s2_" + file.Name);
			}			
		}
	}
}