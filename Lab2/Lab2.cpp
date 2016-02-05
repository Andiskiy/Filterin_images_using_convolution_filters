// Lab2.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <time.h>
#include<windows.h>

using namespace cv;
using namespace std;


struct SUserdata {
	cv::Mat image;
};


void BoxBlur(Mat& img)
{
	Mat dst(img.rows, img.cols, CV_32SC1);
	dst.at<int>(0, 0) = img.at<uchar>(0, 0);
	for (int y = 1; y < img.rows; y++)
		dst.at<int>(y, 0) = img.at<uchar>(y, 0) + dst.at<int>(y - 1, 0);
	for (int x = 1; x < img.cols; x++)
		dst.at<int>(0, x) = img.at<uchar>(0, x) + dst.at<int>(0, x - 1);
	for (int y = 1; y < img.rows; y++)
	{
		for (int x = 1; x < img.cols; x++)
		{
			dst.at<int>(y, x) = img.at<uchar>(y, x) + dst.at<int>(y - 1, x) + dst.at<int>(y, x - 1)
				- dst.at<int>(y - 1, x - 1);
		}
	}
	int rad = 10;
	Mat blured = img.clone();
	for (int y = 0; y < img.rows; y++) {
		for (int x = 0; x < img.cols; x++)
		{
			//blured.at<uchar>(y, x)
			int y0 = y + rad;
			int y1 = y - rad;
			int x0 = x + rad;
			int x1 = x - rad;
			if (y0 >= img.rows)
				y0 = img.rows - 1;
			if (x0 >= img.cols)
				x0 = img.cols - 1;
			if (y1 < 0)
				y1 = 0;
			if (x1 < 0)
				x1 = 0;
			blured.at<uchar>(y, x) = (uchar)((dst.at<int>(y0, x0) - dst.at<int>(y1, x0) - dst.at<int>(y0, x1)
				+ dst.at<int>(y1, x1)) / ((y0 - y1 + 1)*(x0 - x1 + 1)));
		}
	}
	img = blured.clone();
	//imwrite("img.jpg", img);
	//imshow("Реализация приближенного гауссова фильтра", img);
	waitKey();
}


// Эффективная реализация box blur
void integral_transformation(Mat& img)
{
	int start, end;
	start = clock();
	imshow("Оригинал", img);
	Mat dst(img.rows, img.cols, CV_32SC1);
	dst.at<int>(0, 0) = img.at<uchar>(0, 0);
	for (int y = 1; y < img.rows; y++)
		dst.at<int>(y, 0) = img.at<uchar>(y, 0) + dst.at<int>(y - 1, 0);
	for (int x = 1; x < img.cols; x++)
		dst.at<int>(0, x) = img.at<uchar>(0, x) + dst.at<int>(0, x - 1);
	for (int y = 1; y < img.rows; y++)
	{
		for (int x = 1; x < img.cols; x++)
		{
			dst.at<int>(y, x) = img.at<uchar>(y, x) + dst.at<int>(y - 1, x) + dst.at<int>(y, x - 1)
				- dst.at<int>(y - 1, x - 1);
		}
	}
	int rad = 11;
	Mat blured = img.clone();
	for (int y = 0; y < img.rows; y++) {
		for (int x = 0; x < img.cols; x++)
		{
			//blured.at<uchar>(y, x)
			int y0 = y + rad;
			int y1 = y - rad;
			int x0 = x + rad;
			int x1 = x - rad;
			if (y0 >= img.rows)
				y0 = img.rows - 1;
			if (x0 >= img.cols)
				x0 = img.cols - 1;
			if (y1 < 0)
				y1 = 0;
			if (x1 < 0)
				x1 = 0;
			blured.at<uchar>(y, x) = (uchar)((dst.at<int>(y0, x0) - dst.at<int>(y1, x0) - dst.at<int>(y0, x1)
				+ dst.at<int>(y1, x1)) / ((y0 - y1 + 1)*(x0 - x1 + 1)));
		}
	}
	imshow("Эффективная реализация box blur", blured);
	cv::Mat etalon;
	cv::blur(img, etalon, cv::Size(11, 11));
	imshow("diff", etalon - blured);
	end = clock();
	cout << endl << "Время выполнения: " << double(end - start) / CLOCKS_PER_SEC;
	waitKey();

}

//Сепарабельность гауссова фильтра
void Gaussian_filter(Mat& img)
{
	int start, end;
	start = clock();

	Mat dst = img.clone();
	Mat image = img.clone();
	imshow("Original", img);

	int height = 3;
	int width = 1;	
	int rad = 1;

	
	Mat Gx = Mat::zeros(width, height, CV_8UC1);
	Mat Gy = Mat::zeros(height, width, CV_8UC1);

	Gx.at<uchar>(0, 0) = 1;
	Gx.at<uchar>(0, 1) = 2;
	Gx.at<uchar>(0, 2) = 1;
	Gy.at<uchar>(0, 0) = 1;
	Gy.at<uchar>(1, 0) = 2;
	Gy.at<uchar>(2, 0) = 1;

	float sum_kernel = 1 / 16;

	copyMakeBorder(img, dst, rad, rad, rad, rad, 0);

	for (int x = rad; x < img.cols - rad; x++)
		for (int y = rad; y < img.rows - rad; y++)
		{
			double sum = 0;
			for (int l = -rad; l <= rad; l++)
			{
				sum = sum + img.at<uchar>(y, x - l)*Gx.at<uchar>(0, rad + l);
			}
			if ( (sum/4) > 255 )
			{
				image.at<uchar>(y, x) = 255; 
			}
			else
			{
				image.at<uchar>(y, x) = sum / 4;
			}
		}

	for (int x = rad; x < img.cols - rad; x++)
		for (int y = rad; y < img.rows - rad; y++)
		{
			double sum = 0;
			for (int k = -rad; k <= rad; k++)
			{
				sum = sum + image.at<uchar>(y - k, x)*Gy.at<uchar>(rad + k, 0);
			}
			if ((sum / 4) > 255)
			{ 
				dst.at<uchar>(y, x) = 255; 
			}
			else
			{ 
				dst.at<uchar>(y, x) = sum / 4;
			}
		}

	Mat result = dst(Rect(rad, rad, img.cols, img.rows));
//	imshow("Разность", result - img);
	imshow("Сепарабельность гауссова фильтра", result);
	end = clock();
	cout << endl << "Время выполнения: " << double(end - start) / CLOCKS_PER_SEC;
	waitKey();
}


//Реализация приближенного гауссова фильтра
void approximateGaussianFilter(Mat& img)
{
	int start, end;
	start = clock();

	int i = 0;
	while (i < 3)
	{
		BoxBlur(img);
		i++;
	}
	imshow("Реализация приближенного гауссова фильтра", img);
	end = clock();
	cout << endl << "Время выполнения: " << double(end - start) / CLOCKS_PER_SEC;
	waitKey();
}

//Наивная реализация свертки
void convolution_image(Mat& img)
{
	int start, end;
	start = clock();

	//imshow("Original", img);
	int height = 3;
	int width = 3;
	int image_width = img.cols;
	int image_height = img.rows;
	int rad = 1;

	Mat image = img;
	Mat dst = image.clone();
	
	/*kernel.at<float>(0, 0) = 1;
	kernel.at<float>(0, 1) = 2;
	kernel.at<float>(0, 2) = 1;
	kernel.at<float>(1, 0) = 2;
	kernel.at<float>(1, 1) = 2;
	kernel.at<float>(1, 2) = 2;
	kernel.at<float>(2, 0) = 1;
	kernel.at<float>(2, 1) = 2;
	kernel.at<float>(2, 2) = 1;*/

	Mat kernel = (cv::Mat_<float>(3, 3) <<  1, 2, 1,
											2, -12, 2,
											1, 2, 1);
	
	float sum_kernel = 0;
	for (int i = 0; i < kernel.cols; i++)
		for (int j = 0; j < kernel.rows; j++)
		{
			sum_kernel = sum_kernel + kernel.at<float>(j, i);
		}

	copyMakeBorder(img, dst, rad, rad, rad, rad, 0);

	for (int x = rad; x < image_width - rad; x++)
		for (int y = rad; y < image_height - rad; y++)
		{
			double sum = 0;
			for (int k = -rad; k <= rad; k++)
				for (int l = -rad; l <= rad; l++)
				{
					sum = sum + image.at<uchar>(y + k, x + l)*kernel.at<float>(rad + k, rad + l);
				}
			if ((sum / sum_kernel) > 255)
			{
				dst.at<uchar>(y, x) = 255; 
			}
			else
			{ 
				dst.at<uchar>(y, x) = sum / sum_kernel; 
			}
		}

	Mat result = dst(Rect(rad, rad, img.cols, img.rows));

	//imshow("Разность", result - img);
	imshow("Наивная реализация свертки", result);
	end = clock();
	cout << endl << "Время выполнения: " << double(end - start) / CLOCKS_PER_SEC;
	waitKey();

}

//Свёртка с помощью преобразования Фурье
void FourierTransform(Mat& img )
{
	int start, end;
	start = clock();

	int Rows = 3;
	int Cols = 3;
	
	Mat dst = img.clone();
	Mat image;

	Mat kernel = Mat::zeros(Rows, Cols, CV_32F);
	kernel.at<float>(0, 0) = 1;
	kernel.at<float>(0, 1) = 2;
	kernel.at<float>(0, 2) = 1;
	kernel.at<float>(1, 0) = 2;
	kernel.at<float>(1, 1) = 4;
	kernel.at<float>(1, 2) = 2;
	kernel.at<float>(2, 0) = 1;
	kernel.at<float>(2, 1) = 2;
	kernel.at<float>(2, 2) = 1;

	Mat complexImg = Mat::zeros(Size(dst.cols + kernel.cols, dst.rows + kernel.rows), CV_32FC2);
	Mat complexImg2 = Mat::zeros(Size(dst.cols + kernel.cols, dst.rows + kernel.rows), CV_32FC2);

	int fromTo[] = { 0, 0, 1, 1 };

	copyMakeBorder(dst, dst, 0, kernel.rows, 0, kernel.cols, 0);
	copyMakeBorder(kernel, kernel, 0, img.rows, 0, img.cols, 0);

	//Фурье для ядра 
	kernel.convertTo(kernel, CV_32FC1, 1.0 / 255.0);
	mixChannels(&kernel, 1, &complexImg, 1, fromTo, 1);
	dft(complexImg, complexImg);

	//Фурье для картинки 
	dst.convertTo(dst, CV_32FC1, 1.0 / 255.0);
	mixChannels(&dst, 1, &complexImg2, 1, fromTo, 1);
	dft(complexImg2, complexImg2);

	//Перемножаем спектры 
	mulSpectrums(complexImg, complexImg2, image, 0);
	dft(image, image, DFT_INVERSE | DFT_REAL_OUTPUT);
	normalize(image, image, 0, 1, CV_MINMAX);

	Mat result = image(Rect(0, 0, img.cols, img.rows));
	imshow("Original", img);
	imshow("Свёртка с помощью преобразования Фурье", result);
	end = clock();
	cout << endl << "Время выполнения: " << double(end - start) / CLOCKS_PER_SEC;
	waitKey();
}



int _tmain(int argc, _TCHAR* argv[])
{
	setlocale(LC_ALL, "RUS");
	Mat img;
	img = imread("1.png", CV_LOAD_IMAGE_GRAYSCALE);	
	
	SUserdata userdata;
	userdata.image = img;


	int task;
	cout << "1 - Реализация равномерного фильтра" << endl;
	cout << "2 - Реализация гауссова фильтра пользуясь его сепарабельностью" << endl;
	cout << "3 - Реализация приближенного гауссова фильтра" << endl;
	cout << "4 - Реализация свертки одноканального изображения с заданным ядром" << endl;
	cout << "5 - Реализация сверточного фильтра с помощью преобразования Фурье" << "\n\n";
	cout << "Выберите задание: ";
	cin >> task;
	switch (task)
	{
	case 1:	
		integral_transformation(userdata.image);
		break;
		
	case 2:
		Gaussian_filter(userdata.image);
		break;

	case 3:
		approximateGaussianFilter(userdata.image);
		break;

	case 4:
		convolution_image(userdata.image);
		break;
	
	case 5:
		FourierTransform(userdata.image);
		break;

	default:
		break;
	}
	return 0;
}



