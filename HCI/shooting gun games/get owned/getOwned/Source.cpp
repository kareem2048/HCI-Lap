#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "iostream"
#include <thread>
using namespace cv;
using namespace std;
class theif {
	int locx;
	int locy;
	Mat img = imread("theif.png",0);
	//int hp for later
public:
	Mat getimg() {
		return this->img;
	}
	void setlocx(int locx) {
		this->locx = locx;
	
	}
	void setlocy(int locy){
		this->locy = locy;
	}
	int getlocx(){
		return this->locx;
	}
	int getlocy() {
		return this->locy;
	}
};

Mat image;
string winName = "desplay window";
Rect button, button2;
void Startgame() {
	Mat frame;
	double minVal;
	double maxVal;
	Point minLoc;
	Point maxLoc;
	Mat gray;
	int level = 1;
	int score = 0;
	vector<theif>myvector;
	VideoCapture cap(0);
	int width = cap.get(3);
	int	height = cap.get(4);
	theif the;
	
	for (int i = 0; i < level; i++) {
		the.setlocx(rand() % (width - 75 + 1));
		the.setlocy(rand() % (height- 75 + 1));
		myvector.push_back(the);

	}
	if (!cap.isOpened())
	{
		cout << "Cannot open the web cam" << endl;
	}
	while (true)
	{
		bool bSuccess = cap.read(frame);
		if (!bSuccess)
		{
			cout << "Cannot read a frame from video stream" << endl;
			break;
		}
		cvtColor(frame, gray, CV_BGR2GRAY);
		minMaxLoc(gray, &minVal, &maxVal, &minLoc, &maxLoc);
		Mat dark;
		threshold(gray, dark, 255, 255, 3);
		Mat theifimg = myvector[0].getimg();
		for (int i = 0; i < myvector.size(); i++) {
			theifimg.copyTo(dark(Rect(myvector[i].getlocx(), myvector[i].getlocy(), theifimg.cols, theifimg.rows)));
		}	//	cvtColor(frame, frame, CV_BGR2GRAY);
		string livesStr = " score "+ to_string(score)+" level" +to_string(level);
		putText(dark,
			livesStr,
			Point(150, 75), // Coordinates
			FONT_HERSHEY_PLAIN, // Font
			2.0, // Scale. 2.0 = 2x bigger
			Scalar(255, 0, 0), // Color
			1, // Thickness
			CV_AA);
		if (maxVal > 240) {
			circle(dark, maxLoc, 5, (0, 0, 255), 2);
			for (int i = 0; i < myvector.size(); i++) {
				if (maxLoc.x < (myvector[i].getlocx() + (.5 * 75)) && maxLoc.x >(myvector[i].getlocx() - (.5 * 75)) &&
					maxLoc.y < (myvector[i].getlocy() + (.5 * 75)) && maxLoc.y >(myvector[i].getlocy() - (.5 * 75))) {
					myvector.erase(myvector.begin() + i);
					score++;
				}
			}
		}
		imshow("displaywindow", dark);
		if (myvector.size() == 0) {
			level += 1;
			for (int i = 0; i < level; i++) {
				the.setlocx(rand() % (width - 75 + 1));
				the.setlocy(rand() % (height - 75 + 1));
				myvector.push_back(the);

			}
		}
		if (waitKey(30) == 27||level==10)
		{
			cout << "esc key is pressed by user" <<score << endl;
			break;
		}
	}
	waitKey(0);

}
void callBackFunc(int event, int x, int y, int flags, void* userdata)
{
	if (y > 500 && y < 550)
	{
		if (event == EVENT_LBUTTONDOWN)
		{
			if (button.contains(Point(x, y)))
			{
				cout << "Clicked!" << endl;
				rectangle(image, button, Scalar(0, 0, 255), 2);
				Startgame();
			}
		}
		if (event == EVENT_LBUTTONUP)
		{
			rectangle(image, button, Scalar(200, 200, 200), 2);
		}

		imshow(winName, image);
		waitKey(1);
	}if (y > 600 && y < 650) {
		if (event == EVENT_LBUTTONDOWN)
		{
			if (button2.contains(Point(x, y)))
			{
				cout << "Clicked!" << endl;
				rectangle(image, button2, Scalar(0, 0, 255), 2);
			}
		}
		if (event == EVENT_LBUTTONUP)
		{
			rectangle(image, button2, Scalar(200, 200, 200), 2);
		}
		waitKey(1);
	}
}

int main()
{

	image = imread("newback.png", CV_LOAD_IMAGE_COLOR);
	string buttonText("play as cop");
	string buttonText2("play as theif");
	button = Rect(0, 500, image.cols, 50);
	button2 = Rect(0, 600, image.cols, 50);
	image(button) = Vec3b(200, 200, 200);
	image(button2) = Vec3b(200, 200, 200);
	putText(image, buttonText, Point(button.width*0.4, 500 + button.height*0.5), FONT_HERSHEY_PLAIN, 1, Scalar(0, 0, 0));
	putText(image, buttonText2, Point(button.width*0.4, 600 + button.height*0.5), FONT_HERSHEY_PLAIN, 1, Scalar(0, 0, 0));
	namedWindow(winName, CV_WINDOW_AUTOSIZE);
	namedWindow(winName);
	setMouseCallback(winName, callBackFunc);
	imshow(winName, image);
	waitKey();
	return 0;
}
// TODO 
//1/ start menu
//2/  create class draw shooting (cops// thieives)
//3/  git poistion of lazer and remove them when lazer touch 
//4/ add 3 points in class (head/body/chest) to kill
//5/ add by random and lvls 
//6/ add theives mode 
// how we gona work we will make windows as if  its wall  when  we touch postion relative in screen the target vanish 
/*
// capture lazer for later
Mat image;
double minVal;
double maxVal;
Point minLoc;
Point maxLoc;
Mat gray;
VideoCapture cap(0);
if (!cap.isOpened())
{
cout << "Cannot open the web cam" << endl;
return -1;
}
while (true)
{
bool bSuccess = cap.read(image);
if (!bSuccess)
{
cout << "Cannot read a frame from video stream" << endl;
break;
}
cvtColor(image, gray, CV_BGR2GRAY);
minMaxLoc(gray, &minVal, &maxVal, &minLoc, &maxLoc);
circle(image, maxLoc, 5, (255, 0, 0), 2);
imshow("displaywindow", image);
if (waitKey(30) == 27)
{
cout << "esc key is pressed by user" << endl;
break;
}
}
waitKey(0);
return 0;*/