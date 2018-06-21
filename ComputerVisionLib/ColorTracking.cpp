#include "ColorTracking.h"
#include <OSAPI/Process/OSAPISemaphore.h>
#include <CDPSystem/Application/Application.h>

#include <opencv2/highgui.hpp>

using namespace std;
using namespace cv;
using namespace ComputerVisionLib;

static int H_MIN = 124;
static int H_MAX = 184;
static int S_MIN = 66;
static int S_MAX = 138;
static int V_MIN = 0;
static int V_MAX = 256;
//default capture width and height
const int FRAME_WIDTH = 640;
const int FRAME_HEIGHT = 480;
//max number of objects to be detected in frame
const int MAX_NUM_OBJECTS=50;
//minimum and maximum object area
const int MIN_OBJECT_AREA = 40*40;
const int MAX_OBJECT_AREA = FRAME_HEIGHT*FRAME_WIDTH/1.5;
//names that will appear at the top of each window
const string windowName = "Original Image";
const string windowName1 = "HSV Image";
const string windowName2 = "Thresholded Image";
const string windowName3 = "After Morphological Operations";
const string trackbarWindowName = "Trackbars";

void on_trackbar( int, void* )
{
    //This function gets called whenever a
    // trackbar position is changed
}

std::string intToString(int number){


    std::stringstream ss;
    ss << number;
    return ss.str();
}

void createTrackbars(){
    //create window for trackbars

    namedWindow(trackbarWindowName,0);
    //create trackbars and insert them into window
    //3 parameters are: the address of the variable that is changing when the trackbar is moved(eg.H_LOW),
    //the max value the trackbar can move (eg. H_HIGH),
    //and the function that is called whenever the trackbar is moved(eg. on_trackbar)
    //                                  ---->    ---->     ---->
    createTrackbar( "H_MIN", trackbarWindowName, &H_MIN, H_MAX, on_trackbar );
    createTrackbar( "H_MAX", trackbarWindowName, &H_MAX, H_MAX, on_trackbar );
    createTrackbar( "S_MIN", trackbarWindowName, &S_MIN, S_MAX, on_trackbar );
    createTrackbar( "S_MAX", trackbarWindowName, &S_MAX, S_MAX, on_trackbar );
    createTrackbar( "V_MIN", trackbarWindowName, &V_MIN, V_MAX, on_trackbar );
    createTrackbar( "V_MAX", trackbarWindowName, &V_MAX, V_MAX, on_trackbar );
}

void morphOps(Mat &thresh){

    //create structuring element that will be used to "dilate" and "erode" image.
    //the element chosen here is a 3px by 3px rectangle

    Mat erodeElement = getStructuringElement( MORPH_RECT,Size(3,3));
    //dilate with larger element so make sure object is nicely visible
    Mat dilateElement = getStructuringElement( MORPH_RECT,Size(8,8));

    erode(thresh,thresh,erodeElement);
    erode(thresh,thresh,erodeElement);

    dilate(thresh,thresh,dilateElement);
    dilate(thresh,thresh,dilateElement);
}

void drawObject(int x,int y,Mat &frame){

    cv::circle(frame,cv::Point(x,y),10,cv::Scalar(0,0,255));
    cv::putText(frame,intToString(x)+ " , " + intToString(y),cv::Point(x,y+20),1,1,Scalar(0,255,0));
}

Point trackFilteredObject(Mat threshold, Mat &cameraFeed){

    int x = 0, y = 0;

    Mat temp;
    threshold.copyTo(temp);
    //these two vectors needed for output of findContours
    vector< vector<Point> > contours;
    vector<Vec4i> hierarchy;
    //find contours of filtered image using openCV findContours function
    findContours(temp,contours,hierarchy,RETR_CCOMP,CHAIN_APPROX_SIMPLE );
    //use moments method to find our filtered object
    bool objectFound = false;
    if (hierarchy.size() > 0)
    {
        int numObjects = hierarchy.size();
        //if number of objects greater than MAX_NUM_OBJECTS we have a noisy filter
        if (numObjects < MAX_NUM_OBJECTS)
        {
            for (int index = 0; index >= 0; index = hierarchy[index][0])
            {
                Moments moment = moments((cv::Mat)contours[index]);
                double area = moment.m00;

                //if the area is less than 20 px by 20px then it is probably just noise
                //if the area is the same as the 3/2 of the image size, probably just a bad filter
                //we only want the object with the largest area so we safe a reference area each
                //iteration and compare it to the area in the next iteration.
                if (area > MIN_OBJECT_AREA)
                {
                    x = moment.m10/area;
                    y = moment.m01/area;

                    objectFound = true;
                }
                else
                    objectFound = false;
            }
            //let user know you found an object
            if (objectFound)
                drawObject(x,y,cameraFeed); //draw object location on screen
        }
        else
            putText(cameraFeed,"TOO MUCH NOISE! ADJUST FILTER",Point(0,50),1,2,Scalar(0,0,255),2);
    }
    return Point(x,y);
}

void ColorTracking::Main()
{
    //Matrix to store each frame of the webcam feed
    Mat cameraFeed;
    Mat threshold;
    Mat HSV;

    //store image to matrix
    capture.read(cameraFeed);
    //convert frame from BGR to HSV colorspace
    cvtColor(cameraFeed,HSV,COLOR_BGR2HSV);

    if (calibrationMode == true){
        //if in calibration mode, we track objects based on the HSV slider values.
        cvtColor(cameraFeed,HSV,COLOR_BGR2HSV);
        inRange(HSV,Scalar(H_MIN,S_MIN,V_MIN),Scalar(H_MAX,S_MAX,V_MAX),threshold);
        morphOps(threshold);
        imshow(windowName2,threshold);
        Point p = trackFilteredObject(threshold,cameraFeed);
        {
            OSAPIMutexLocker locker(GetMemberAccessMutex(), "Main thread member access");
            xCoord = p.x;
            yCoord = p.y;
        }
    }

    cv::imshow(windowName,cameraFeed);

    //delay 30ms so that screen can refresh.
    //image will not appear without this waitKey() command
    cv::waitKey(30);
    Application::RunInMainThread([&] { Main(); });
}

ColorTracking::ColorTracking()
{
}

ColorTracking::~ColorTracking()
{
}

void ColorTracking::Create(const char* fullName)
{
    CDPComponent::Create(fullName);
    xCoord.Create("xCoord",this);
    yCoord.Create("yCoord",this);

}

void ColorTracking::CreateModel()
{
    CDPComponent::CreateModel();

    RegisterStateProcess("Null", (CDPCOMPONENT_STATEPROCESS)&ColorTracking::ProcessNull, "Initial Null state");
}

void ColorTracking::Configure(const char* componentXML)
{
    CDPComponent::Configure(componentXML);
}

void ColorTracking::ProcessNull()
{
}

void ColorTracking::Activate()
{
    Application::RunInMainThread([&]
    {
        if(calibrationMode){
            //create slider bars for HSV filtering
            createTrackbars();
        }
        //video capture object to acquire webcam feed
        //open capture object at location zero (default location for webcam)
        capture.open(0);
        //set height and width of capture frame
        capture.set(CAP_PROP_FRAME_WIDTH,FRAME_WIDTH);
        capture.set(CAP_PROP_FRAME_HEIGHT,FRAME_HEIGHT);
        //start an infinite loop where webcam feed is copied to cameraFeed matrix
        //all of our operations will be performed within this loop

        Application::RunInMainThread([&] { Main(); });
    });

    CDPComponent::Activate();
}
