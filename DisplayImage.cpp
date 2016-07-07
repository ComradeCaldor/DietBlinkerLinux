#include <opencv2/core/core.hpp>
#include <iostream>

#include <cv.h>
#include <highgui.h>
#include <stdlib.h>
#include <stdio.h>

#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <iostream>
#include <string>
#include <time.h>
//#include <chrono>

using namespace cv;
using namespace std;

void get_input(int* sum_shut, int* sum_open, short* ishow_screen, short* iexit, short* x_crop, short* width, short* y_crop, short* height, short* time_shut_min);

int sum(cv::Mat image, short pix_thresh, short sum_step)
{
	int sum(0);
	for (int y(0); y < image.rows; y += sum_step)
	{

		for (int x(0); x < 3 * image.cols; x += sum_step)
		{
			int tmp = image.at<unsigned char>(y, x);
			if (tmp < pix_thresh)
			{
				sum += tmp;
				image.at<unsigned char>(y, x) = 255;
			}
		}
		//cout << endl;
	}
	//getchar();
	return sum;
}
/*
void draw_cross(cv::Mat image)
{
}*/

/*
Mat thresh(cv::Mat image, short pix_thresh)
{
for (int y(0); y < image.rows; y += 1)
{
;
for (int x(0); x < image.cols; x += 1)
{
int tmp = image.at<unsigned char>(y, x);
//if (tmp < pix_thresh) image.at<unsigned char>(y, x) = 250;
}
//cout << endl;
}
//getchar();
return image;
}
*/
int main()
{
	short index_camera = 0;
	short ishow_screen = 1;
	short iexit = 0;
	short time_shut_min = 3;
	short pix_thresh = 120;
	short multy = 5;

	short x_crop = 210,
		width = 208,
		y_crop = 126,
		height = 200;

	//---time----
	
		string time_output;
		int blink_no = 0;
		time_t timer;
		time_t now;
		struct tm y2k = { 0 };
		double seconds;

		y2k.tm_hour = 0;   y2k.tm_min = 0; y2k.tm_sec = 0;
		y2k.tm_year = 100; y2k.tm_mon = 0; y2k.tm_mday = 1;

		time(&timer);  /* get current time; same as: timer = time(NULL)  */

		seconds = difftime(timer, mktime(&y2k));

		printf("%.f seconds since January 1, 2000 in the current timezone\n", seconds);
//		unsigned long  start = chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now().time_since_epoch()).count(); //chrono::milliseconds
//		cout << start <<endl;
	

	printf(" Controls: \n   s/o                = shut/open \n   c                  = camera \n   up/down/left/right = center crop \n   num 4/8/6/2        = width/heght \n   z                  = console \n   num 7/9            = threshold -/+ \n   num 1/3            = delay -/+ \n   ESC                = Adios! \n\n");

	cv::VideoCapture stream1(index_camera);//CV_CAP_ANY);   //0 is the id of video device.0 if you have only one camera.

	if (!stream1.isOpened())
	{ //check if video device has been initialised
		printf("cannot open camera");
		getchar();
	}

	//stream1.set(CV_CAP_PROP_FRAME_HEIGHT,600); //does'nt work
	cout << "camera : " << index_camera << endl;
	cout << "current resolution :" << stream1.get(CV_CAP_PROP_FRAME_WIDTH) << "x" << stream1.get(CV_CAP_PROP_FRAME_HEIGHT) << endl;

	stream1.set(CV_CAP_PROP_AUTO_EXPOSURE, 0);

	int frame_no = 0;
	double fps = -1;
	char c_input;

	int k = 0;
	int summa = 0;
	int sum_shut = 0;
	int sum_open = 0;
	short time_shut = 0;
	short sum_step = 4;

	bool bSuccess = false;
	Mat frame, frame1;

	Rect myROI(x_crop, y_crop, width, height);

	while (1)
	{
		/*
		time(&now);  // get current time; same as: timer = time(NULL)  
		seconds = difftime(now, timer);
		printf("%.f seconds since January 1, 2000 in the current timezone\n", seconds);
		*/

		bSuccess = stream1.read(frame);
		/*
		if (!bSuccess) //if not success, break loop
		{
		cout << "Cannot read the frame from video file" << endl;
		getchar();
		break;
		}*/

		//cout << c_input << endl;
		if ( k != -1 )
			cout << k << endl;

		//frame = thresh(frame(myROI), pix_thresh);
		// äîáàâî÷íàÿ âåëè÷èíà 
		//		int add = argc >= 7 ? atoi(argv[6]) : 200;

		//cvAddS(frame, cvScalar(add), frame);
		//threshold(frame, frame, pix_thresh, 255, 4);//4
		summa = sum(frame(myROI), pix_thresh, sum_step);
		//cout << "SUM = " << sum(frame(myROI)) << endl;

		//c_input = cvWaitKey(33);
		k = cvWaitKey(33);
		switch (k)
		{
		case 122:
		{
					get_input(&sum_shut, &sum_open, &ishow_screen, &iexit, &x_crop, &width, &y_crop, &height, &time_shut_min);//dfaef(*par, *par1);
					myROI = Rect(x_crop, y_crop, width, height);
					cout << "x" << x_crop << "," << width << "  y" << y_crop << "," << height << endl;
		}
			break;
		case 2490368:
		{
						y_crop--;
						myROI = Rect(x_crop, y_crop, width, height);
						cout << "Y" << y_crop << endl;
		}
			break;
		case 2621440:
		{
						y_crop++;
						myROI = Rect(x_crop, y_crop, width, height);
						cout << "Y" << y_crop << endl;
		}
			break;
		case 2555904:
		{
						x_crop++;
						myROI = Rect(x_crop, y_crop, width, height);
						cout << "X" << x_crop << endl;
		}
			break;
		case 2424832:
		{
						x_crop--;
						myROI = Rect(x_crop, y_crop, width, height);
						cout << "X" << x_crop << endl;
		}
			break;
		case 56:
		{
				   height++;
				   myROI = Rect(x_crop, y_crop, width, height);
				   cout << "He" << height << endl;
		}
			break;
		case 50:
		{
				   height--;
				   myROI = Rect(x_crop, y_crop, width, height);
				   cout << "He" << height << endl;
		}
			break;
		case 54:
		{
				   width++;
				   myROI = Rect(x_crop, y_crop, width, height);
				   cout << "Wi" << width << endl;
		}
			break;
		case 52:
		{
				   width--;
				   myROI = Rect(x_crop, y_crop, width, height);
				   cout << "Wi" << width << endl;
		}
			break;
		case 115://s
		{
					 sum_shut = summa;
					 cout << "SHUT = " << sum_shut << endl;
		}
			break;
		case 111://o
		{
					 sum_open = summa;
					 cout << "OPEN = " << sum_open << endl;
		}
			break;
		case 99://c
		{
					if (ishow_screen == 1)
						ishow_screen = 2;
					else if (ishow_screen == 2)
						ishow_screen = 3;
					else if (ishow_screen == 3)
						ishow_screen = 0;
					else ishow_screen = 1;
		}
			break;
		case 55://num7
		{
					pix_thresh--;
					cout << "Threshold = " << pix_thresh << endl;
		}
			break;
		case 57://num9
		{
					pix_thresh++;
					cout << "Threshold = " << pix_thresh << endl;
		}
			break;
		case 27://ESC
		{
					cout << "!Hasta luego1" << endl;
					return 0;
		}
			break;
		case 49://num1
		{
					time_shut_min--;
					cout << "Time_shut_min = " << time_shut_min << endl;
		}
			break;
		case 51://num3
		{
					time_shut_min++;
					cout << "Time_shut_min = " << time_shut_min << endl;
		}
			break;
		case 113://q
			{
				if (sum_step > 1)
					sum_step--;
				cout << "Sum_step = " << sum_step << endl;
			}
				break;
		case 119://w
			{
				sum_step++;
				cout << "Sum_step = " << sum_step << endl;
			}
				break;
		default:
			break;
		}//switch(k)

		if (abs(summa - sum_open) > multy * abs(summa - sum_shut))
		{
			//cout <<  "Ts" << time_shut  << "difffer" << endl;
			time_shut++;
			if (time_shut == time_shut_min)
			{
				//keybd_event(VK_SPACE, 0, 0, 0);
				//keybd_event(VK_SPACE, 0, KEYEVENTF_KEYUP, 0);
				cout << " SPACE " << endl;
			}

			if (time_shut == 1)
			{
				;//unsigned long now = chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now().time_since_epoch()).count();
				//cout << "time_ms[" << blink_no << "] = " << now - start << ";" << endl;
			}
		}
		else
		{
			//cout << time_shut << endl;
			if (time_shut > 0)
			{
				;//cout << "time_dura[" << blink_no << "] = " << time_shut << ";" << endl;
				//blink_no++;
			}
			time_shut = 0;
		}

		//cout << "ss = " << sum_shut << "\t" << " so = " << sum_open << endl;
		//if (iexit == 1) break;
		//if (c_input == 27) break; // If ESC then break;

		//frame_no++;

		//SetImageROI(frame, cvRect(20, 20, 100, 1000));
		//ishow_screen == 1
		switch (ishow_screen)
		{
		case 1:
		{
				  imshow("MyVideo", frame);
		}
			break;
		case 2:
		{
				  Mat frame_cropped = frame(myROI);
				  imshow("MyVideo", frame_cropped);
		}
			break;
		case 3:
		{
				  frame(myROI) += 0.3 * 255;
				  imshow("MyVideo", frame);
		}
		case 0:
		{
				  ;
		}
			break;
		default:
		{
				   cout << "Error:" << endl << "screen == " << ishow_screen << endl;
		}
			break;
		}  //switch (ishow_screen)
	}  //while(1)
	return 0;
}

void get_input(int* sum_shut, int* sum_open, short* ishow_screen, short* iexit, short* x_crop, short* width, short* y_crop, short* height, short* time_shut_min)
{
	printf("123\n");

	string  c;
	double a;

	printf("SumShut SumOpen SCreen XCrop YCrop WIdth HEight TimeShutmin \n| ss | so | sc | xc | yc | wi | he | ts |\n Your choise is: ");

	getline(cin, c);
	
	if (c == "ss")
	{
		cout << "val: ";
		cin >> a;
		*sum_shut = (short)a;
		cout << "Now sum_shut = " << *sum_shut << endl;
	}
	else
	if (c == "so")
	{
		cout << "val: ";
		cin >> a;
		*sum_open = (short)a;
		cout << "Now sum_open = " << *sum_open << endl;
	}
	else
	if (c == "sc")
	{
		cout << "val: ";
		cin >> a;
		*ishow_screen = (short)a;
		cout << "Now screen parameter = " << *ishow_screen << endl;
	}
	else if (c == "xc")
	{
		cout << "val: ";
		cin >> a;
		*x_crop = (short)a;
		cout << "Now x_crop = " << (int)*x_crop << endl;//a << endl;
	}
	else if (c == "yc")
	{
		cout << "val: ";
		cin >> a;
		*y_crop = (short)a;
		cout << "Now y_crop = " << (int)*y_crop << endl;
	}
	else if (c == "wi")
	{
		cout << "val: ";
		cin >> a;
		*width = (short)a;
		cout << "Now width = " << (int)*width << endl;
	}
	else if (c == "he")
	{
		cout << "val: ";
		cin >> a;
		*height = (short)a;
		cout << "Now height = " << (int)*height << endl;
	}
	else if (c == "ts")
	{
		cout << "val: ";
		cin >> a;
		*time_shut_min = (short)a;
		cout << "Time shut = " << (int)*time_shut_min << endl;
	}
	else if (c == "exit")
	{
		*iexit = 1;
		cout << " !Hasta luego! " << endl;
	}
	else
		cout << endl << "You've entered:" << c << endl;

	printf(" Controls: \n   s/o                = shut/open \n   c                  = camera \n   up/down/left/right = center crop \n   num 4/8/6/2        = width/heght \n   z                  = console \n   num 7/9            = threshold -/+ \n   num 1/3            = delay -/+ \n   ESC                = Adios! \n\n");
}
