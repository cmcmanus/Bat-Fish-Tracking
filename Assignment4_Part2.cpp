/**
	CS585_Assignment4.cpp
	@author: 
	@version: 

	CS585 Image and Video Computing Fall 2014
	Assignment 4: AQUARIUM
*/

#include "stdafx.h"
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

using namespace cv;
using namespace std;

void createBinary(Mat&, Mat&);
void createLabelled(Mat&, Mat&);
void drawCentroids(Mat&, Mat&);

/**
TODO: Read the segmented images of the aquarium dataset and create the following:
	a) A binary image where background pixels are labelled 0, and object pixels are
		labelled 255
	b) A labelled image where background pixels are labelled 0, and object pixels are
		labelled numerically 
	c) A 3-channel image where the object centroids are drawn as red dots on the binary image
*/
int main()
{
	Mat frame, binary, labelled, binary3c;

	frame = imread("Segmentation_Aqua/2014_aq_segmented 01.jpg", CV_LOAD_IMAGE_COLOR);

	namedWindow("Source");
	imshow("Source", frame);

	createBinary(frame, binary);
	namedWindow("Binary");
	imshow("Binary", binary);

	createLabelled(binary, labelled);
	namedWindow("Labelled");
	imshow("Labelled", labelled);

	cvtColor(binary, binary3c, CV_GRAY2BGR);
	drawCentroids(labelled, binary3c);
	namedWindow("Centroids");
	imshow("Centroids", binary3c);

	char key = waitKey(0);
	return 0;
}

void createBinary(Mat& src, Mat& dst){
	dst = Mat::zeros(src.rows, src.cols, CV_8UC1);
	for (int i = 0; i < src.rows; i++){
		for (int j = 0; j < src.cols; j++){
			Vec3b color = src.at<Vec3b>(i, j);
			if (color[2] > color[1] && color[2] > color[0] && color[2] > 70){
				dst.at<uchar>(i, j) = 255;
			}
		}
	}
	vector<vector<Point>> contours;
	vector<Vec4i> hierarcy;
	findContours(dst, contours, hierarcy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);
	drawContours(dst, contours, -1, Scalar(255), -1);
	drawContours(dst, contours, -1, Scalar(0), 1);
}

void createLabelled(Mat& src, Mat& dst){
	int value = 1;
	dst = src.clone();
	for (int i = 0; i < dst.rows; i++){
		for (int j = 0; j < dst.cols; j++){
			if (dst.at<uchar>(i, j) == 255){
				floodFill(dst, Point(j, i), Scalar(value));
				value++;
			}
		}
	}
}

void drawCentroids(Mat& src, Mat& dst){
	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;
	findContours(src, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);
	for (vector<vector<Point>>::iterator it = contours.begin(); it != contours.end(); it++){
		Rect box = boundingRect(*it);
		int sumx = 0;
		int sumy = 0;
		int pixel = 0;
		for (int i = box.x; i < box.x + box.width - 1; i++){
			for (int j = box.y; j < box.y + box.height - 1; j++){
				Vec3b color = dst.at<Vec3b>(j, i);
				if (color[0] == 255 && color[1] == 255 && color[2] == 255){
					sumx += i;
					sumy += j;
					pixel++;
				}
			}
		}
		int xcent = sumx / pixel;
		int ycent = sumy / pixel;
		circle(dst, Point(xcent, ycent), 3, Scalar(0, 0, 255), -1);
	}
}
