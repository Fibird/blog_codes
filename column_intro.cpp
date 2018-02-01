// column_intro.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;
using namespace std;

int main(int argc, char **argv)
{
	if (argc < 2)
	{
		cout << "No arguments found!" << endl;
		return -1;
	}
	//cout << argc << endl;
	float frame_wd = 1110, frame_hg = 170;
	Mat frame = Mat::zeros(frame_hg, frame_wd, CV_8U);
	
	Mat scale_mat = Mat::zeros(2, 3, CV_32FC1);
	float scale_factor;
	Mat img, rst;
	// the distance of every image move in loop
	float pitch = 0.0;

	for (int i = 0; i < argc - 1; i++)
	{
		img = imread(argv[i + 1], CV_LOAD_IMAGE_GRAYSCALE);
		// get scale factor
		scale_factor = frame_hg / img.size().height;
		// get scale matrix
		scale_mat.at<float>(0) = scale_factor;
		scale_mat.at<float>(4) = scale_factor;
		// get size of result
		Size sz(img.size().width * scale_factor,
			img.size().height * scale_factor);
		warpAffine(img, rst, scale_mat, sz);

		if (pitch + rst.size().width < frame.size().width)
		{
			rst.copyTo(frame(Rect(pitch, 0, rst.size().width, frame_hg)));
			pitch += rst.size().width;
		}
		else
		{
			rst(Rect(0, 0, frame_wd - pitch, frame_hg)).
				copyTo(frame(Rect(pitch, 0, frame_wd - pitch, frame_hg)));
		}
	}

	// write the result into file
	imwrite("result.png", frame);
    return 0;
}

