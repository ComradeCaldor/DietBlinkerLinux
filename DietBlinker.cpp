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
#include <sys/time.h>
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

	int slider_pix_thresh = 120,
	    slider_sum_step = 2,
	    slider_x_crop = 210,
            slider_y_crop = 126,
	    slider_height = 200,
	    slider_width  = 208,
	    slider_time_shut_min = 1,
	    slider_blinks_neded = 3,
	    slider_blinks_delta = 1000,
	    slider_blinks_death_time = 1000;

	namedWindow("options",1);
	createTrackbar("threshold","options",&slider_pix_thresh,300);
	createTrackbar("step","options",&slider_sum_step,20);
	createTrackbar("x_roi","options",&slider_x_crop,400);
	createTrackbar("y_roi","options",&slider_y_crop,400);
	createTrackbar("width","options",&slider_width,400);
	createTrackbar("height","options",&slider_height,400);
	createTrackbar("!!(CHECKME)!! min_Tshut","options",&slider_time_shut_min,10);
	createTrackbar("blinks_neded","options",&slider_blinks_neded,7);
	createTrackbar("blinks_delta","options",&slider_blinks_delta,5000);
	createTrackbar("blinks_death_time","options",&slider_blinks_death_time,10000);

	cv::Mat pic = cv::Mat::zeros(250,250,CV_8UC3);;
	imshow("options", pic);
	//---time----
	
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
	int blink_no = 0;
	int blinks_neded = 3,
	    blinks_delta = 1000,
	    blinks_death_time = 1000;
	long int blink_time[100000];
	long int last_blink;

	bool bSuccess = false;
	Mat frame, frame1;

	Rect myROI(x_crop, y_crop, width, height);

	struct timeval tp;
	gettimeofday(&tp, NULL);
	long int start_ms = tp.tv_sec * 1000 + tp.tv_usec / 1000;
	long int ms = 0;
	long int fps_ms = 0, fps_last = 0;

	while (1)
	{
		frame_no++;
		gettimeofday(&tp, NULL);
		ms = tp.tv_sec * 1000 + tp.tv_usec / 1000;

		if (frame_no%10 == 0)
		{
			fps_last = fps_ms;
			fps_ms   = ms;
			cout << "FPS" << (double)10000/(fps_ms - fps_last) << endl;
		}

		pix_thresh = slider_pix_thresh;
		x_crop = slider_x_crop;
		y_crop = slider_y_crop;
		height = slider_height;
		width  = slider_width;
		time_shut_min = slider_time_shut_min;
		blinks_neded = slider_blinks_neded;
		blinks_delta = slider_blinks_delta;
		blinks_death_time = slider_blinks_death_time;
		if (slider_sum_step > 0)
			sum_step = slider_sum_step;
		myROI = Rect(x_crop, y_crop, width, height);

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
		case 65362:
		{
						y_crop--;
						myROI = Rect(x_crop, y_crop, width, height);
						cout << "Y" << y_crop << endl;
		}
			break;
		case 65364:
		{
						y_crop++;
						myROI = Rect(x_crop, y_crop, width, height);
						cout << "Y" << y_crop << endl;
		}
			break;
		case 65363:
		{
						x_crop++;
						myROI = Rect(x_crop, y_crop, width, height);
						cout << "X" << x_crop << endl;
		}
			break;
		case 65361:
		{
						x_crop--;
						myROI = Rect(x_crop, y_crop, width, height);
						cout << "X" << x_crop << endl;
		}
			break;
		case 50:
		{
				   height++;
				   myROI = Rect(x_crop, y_crop, width, height);
				   cout << "He" << height << endl;
		}
			break;
		case 56:
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

					 //pic = cv::Mat::zeros(250,250,CV_8UC3);
					 char ch[] = "char00000";
					 sprintf(ch, "%d", sum_shut);
					 cv::rectangle(pic, cv::Point(50,25), cv::Point(120, 50), cv::Scalar(0,0,0), -1, CV_AA);
					 cv::putText(pic, "shut",cv::Point(10,35), CV_FONT_HERSHEY_SIMPLEX, 0.5,    cv::Scalar(255),1,5,false);
					 cv::putText(pic, ch,cv::Point(50,35), CV_FONT_HERSHEY_SIMPLEX, 0.5,    cv::Scalar(255),1,8,false);
		}
			break;
		case 111://o
		{
					 sum_open = summa;
					 cout << "OPEN = " << sum_open << endl;

					 //pic = cv::Mat::zeros(250,250,CV_8UC3);
					 char ch[] = "char00000";
					 sprintf(ch, "%d", sum_open);
					 cv::rectangle(pic, cv::Point(50,10), cv::Point(120, 22), cv::Scalar(0,0,0), -1, CV_AA);
					 cv::putText(pic, "open",cv::Point(10,20), CV_FONT_HERSHEY_SIMPLEX, 0.5,    cv::Scalar(255),1,5,false);
					 cv::putText(pic, ch,cv::Point(50,20), CV_FONT_HERSHEY_SIMPLEX, 0.5,    cv::Scalar(255),1,5,false);
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

/*

		blinks_neded = slider_blinks_neded;
		blinks_delta = slider_blinks_delta;
		blinks_death_time = slider_blinks_death_time;

*/
				int ret = system("xdotool key space");
				cout << " SPACE " << endl;

				blink_time[blink_no] = ms;
				blink_no++;

				char ch[] = "char00000";
				sprintf(ch, "%.3f", ((double)ms - start_ms)/1000);
				cv::rectangle(pic, cv::Point(93,65), cv::Point(150, 85), cv::Scalar(0,100,0), -1, CV_AA);
				cv::putText(pic, ch,cv::Point(90,80), CV_FONT_HERSHEY_SIMPLEX, 0.5,    cv::Scalar(255),1,5,false);
				cv::putText(pic, "space",cv::Point(45,80), CV_FONT_HERSHEY_SIMPLEX, 0.5,    cv::Scalar(255),1,5,false);

				//cv::rectangle(pic, cv::Point(50,10), cv::Point(120, 22), cv::Scalar(0,0,0), -1, CV_AA);
				//cv::putText(pic, "open",cv::Point(10,20), CV_FONT_HERSHEY_SIMPLEX, 0.5,    cv::Scalar(255),1,5,false);
				//cv::putText(pic, ch,cv::Point(50,20), CV_FONT_HERSHEY_SIMPLEX, 0.5,    cv::Scalar(255),1,5,false);
			}

			if (time_shut > time_shut_min)
			{
				blink_time[blink_no] = ms;
			}
		}
		else
		{
			 cv::putText(pic, "space",cv::Point(45,80), CV_FONT_HERSHEY_SIMPLEX, 0.5,    cv::Scalar(0),1,5,false);

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
	 imshow("options", pic);
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
