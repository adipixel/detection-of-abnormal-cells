using namespace std;

#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;

//converts the rgb image to binary
Mat convertBinary(Mat img)
{
	Mat binImg = Mat(img.rows,img.cols,img.type());

	for(int i =0; i<img.rows; i++)
		for(int j=0; j<img.cols; j++)
		{
			cv::Vec3b intensity = img.at<cv::Vec3b>(i,j);

			if(intensity.val[0] > intensity.val[2] && intensity.val[0] > intensity.val[1])
			{
				if(intensity.val[0] < 210)
				{
					//make all values 0
					binImg.at<cv::Vec3b>(i,j).val[0] = 0;
					binImg.at<cv::Vec3b>(i,j).val[1] = 0;
					binImg.at<cv::Vec3b>(i,j).val[2] = 0;
				}
				else
				{
					binImg.at<cv::Vec3b>(i,j).val[0] = 255;
					binImg.at<cv::Vec3b>(i,j).val[1] = 255;
					binImg.at<cv::Vec3b>(i,j).val[2] = 255;
				}
			}
			else
			{
				binImg.at<cv::Vec3b>(i,j).val[0] = 0;
				binImg.at<cv::Vec3b>(i,j).val[1] = 0;
				binImg.at<cv::Vec3b>(i,j).val[2] = 0;
			}
		}

	return binImg;
}

//erosion - morphological operation
Mat erosion(Mat img,Mat strel)
{
	Mat tempImg =  Mat(img.rows,img.cols,img.type(),Scalar(0,0,0,0));

	for (int i=0; i<=img.rows-1; i++)
		for (int j=0; j<=img.cols-1; j++)
		{
			int validFlag = true;
			for (int m=0; m<=strel.rows-1; m++)
				for (int n=0; n<=strel.cols-1; n++)
				{
					if((int)img.at<cv::Vec3b>(i+m,j+n)[0] != (int)strel.at<int>(m,n))
					{
						validFlag = false;
					}
				}
				if (validFlag)  
				{
					tempImg.at<Vec3b>(i,j)[0] = 255;
					tempImg.at<Vec3b>(i,j)[1] = 255;
					tempImg.at<Vec3b>(i,j)[2] = 255;
				}
      	}

	return tempImg;
}

//dilation - morphological operation
Mat dilation(Mat img, Mat strel)
{
	Mat outImg = Mat(img.rows,img.cols,img.type(),Scalar(0,0,0,0));
	for (int i=0; i<=img.rows-1; i++)
	{
		for (int j=0; j<=img.cols-1; j++) 
		{
			if ((int)img.at<cv::Vec3b>(i,j)[0] == 255) 
			{     
				for (int m=0; m<=strel.rows-1; m++)
					for (int n=0; n<=strel.cols-1; n++)
					{
          				outImg.at<cv::Vec3b>(i+m, j+n)[0] = (int)strel.at<int>(m,n);
						outImg.at<cv::Vec3b>(i+m, j+n)[1] = (int)strel.at<int>(m,n);
						outImg.at<cv::Vec3b>(i+m, j+n)[2] = (int)strel.at<int>(m,n);
					}
			}
		}
	}
	return outImg;
}

void label(Mat& img,int i,int j,int r,int g,int b)
{
		cv::Vec3b intensity = img.at<cv::Vec3b>(i,j);
		int o_r = intensity.val[0];
		if(i < img.rows  && j < img.cols &&  i >= 0 && j >= 0 && o_r == 255 )
		{
			cv::Vec3b intensity = img.at<cv::Vec3b>(i,j);

			intensity.val[0] = r;
			intensity.val[1] = g;
			intensity.val[2] = b;

			img.at<cv::Vec3b>(i,j) = intensity;
				
			label(img,i+1,j,r,g,b);
			label(img,i,j+1,r,g,b);
			label(img,i,j-1,r,g,b);
			label(img,i-1,j,r,g,b);
		}
}

//counts connected components (4 connected)
Mat fourConn (Mat& img)
{
	int r = 0, g = 0, b = 0;
	int cells=0;
	for(int i =0;i<img.rows ;i++)
		for(int j=0;j<img.cols;j++)
			{
					cv::Vec3b intensity = img.at<cv::Vec3b>(i,j);
					int o_r = intensity.val[0];
					int o_g = intensity.val[1];
					int o_b = intensity.val[2];

				if(o_r == 255)
					{
						label(img,i,j,r,g,b);
						r = r + 100;
						g = g + 70;
						b = b + 30;

						cells++;
					}
			}

	cout<<"Number of abnormal cells = " << cells;
		
	return img;
}


//driver function
int main()
{
	//reading input
	Mat img = imread("input.bmp");
	imshow("Input - Medical Image",img);
	waitKey(0);

	//converting to binary 
	int size = img.rows * img.cols;;
	blur(img,img,Size(5,5));
	Mat binaryImage =  convertBinary(img);
	imshow("After thresholding",binaryImage);
	imwrite("thresholded.bmp",binaryImage);
	waitKey(0);

	// applying OPEN morphological operation
	// part 1 - erosion
	Mat strel = (Mat_<int>(9,9) << 255,255,255,255,255,255,255,255,255,
	255,255,255,255,255,255,255,255,255,
	255,255,255,255,255,255,255,255,255,
	255,255,255,255,255,255,255,255,255,
	255,255,255,255,255,255,255,255,255,
	255,255,255,255,255,255,255,255,255,
	255,255,255,255,255,255,255,255,255,
	255,255,255,255,255,255,255,255,255,
	255,255,255,255,255,255,255,255,255
	);
	Mat outImg = erosion(binaryImage,strel);
	imshow("After Erosion",outImg);
	imwrite("Erorded.bmp",outImg);
	waitKey(0);

	//part 2 - dilation
	Mat strel1 = (Mat_<int>(6,6) << 255,255,255,255,255,255,
	255,255,255,255,255,255,
	255,255,255,255,255,255,
	255,255,255,255,255,255,
	255,255,255,255,255,255,
	255,255,255,255,255,255);
	outImg = dilation(outImg,strel1);
	imshow("After Dilation",outImg);
	imwrite("Dilated.bmp",outImg);
	waitKey(0);

	//counting connected components
	outImg = fourConn(outImg);
	imshow("Component mark and count",outImg);
	imwrite("componentmark.bmp",outImg);
	waitKey(0);

	return 0;
}