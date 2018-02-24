#include "hello.h"
#include <QtWidgets/QApplication>
#include "Func.h"


int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	hello w;
	w.setWindowTitle(G2U("ÄÚ´æ"));
	w.showMaximized();

	return a.exec();
}


//#include "opencv2/opencv.hpp"  
//#include <iostream>
//
//using namespace cv;
//using namespace std;
//
//int main()
//{
//	QString path("f:\\timg.jpg");
//	Mat srcImage = imread(path.toLocal8Bit().constData());
//	imshow("img", srcImage);
//	Mat grayImage;
//	cvtColor(srcImage, grayImage, CV_BGR2GRAY);
//	threshold(srcImage, grayImage, 128, 200, 3);
//	imshow("img2", grayImage);
//	waitKey(0);
//
//	return 0;
//}
