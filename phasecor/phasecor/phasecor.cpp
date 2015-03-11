// phasecor.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

using namespace cv;

int main(int, char* [])
{
    VideoCapture video(0);
    Mat frame, curr, prev, curr64f, prev64f, hann;
    int key = 0;
	Point2d	loc;

    do
    {
        video >> frame;
        cvtColor(frame, curr, COLOR_RGB2GRAY);

        if(prev.empty())
        {
            prev = curr.clone();
            createHanningWindow(hann, curr.size(), CV_64F);
        }

        prev.convertTo(prev64f, CV_64F);
        curr.convertTo(curr64f, CV_64F);

        Point2d shift = phaseCorrelate(prev64f, curr64f, hann);
        double radius = cv::sqrt(shift.x*shift.x + shift.y*shift.y);

		loc.x= loc.x + shift.x;
		loc.y= loc.y + shift.y;

		printf("%lf %lf %lf %lf %lf\r\n", radius, shift.x, shift.y, loc.x, loc.y);
        if(radius > 1)
        {
			radius= radius*10;
            // draw a circle and line indicating the shift direction...
            Point center(curr.cols >> 1, curr.rows >> 1);
            cv::circle(frame, center, (int)radius, cv::Scalar(0, 255, 0), 1, CV_AA);
            cv::line(frame, center, Point(center.x + (int)shift.x*10, center.y + (int)shift.y*10), cv::Scalar(0, 255, 0),1, CV_AA);
        }

        imshow("phase shift", frame);
        key = waitKey(2);

        prev = curr.clone();
    } while((char)key != 27); // Esc to exit...

    return 0;
}

