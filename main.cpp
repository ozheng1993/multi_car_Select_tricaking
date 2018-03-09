//ou zheng
#include <opencv2/core/utility.hpp>
#include <opencv2/tracking.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
//C
#include <stdio.h>
//C++
#include <iostream>
#include <sstream>
#include <ctime>
#include <fstream>
using namespace std;
using namespace cv;
Mat frame; //current frame mat
Mat frameMat; //current frame mat
Mat Roi;
int frameCount=0;
//speed var
vector<double> preX;
vector<double> preY;
vector<double> lastX;
vector<double> lastY;
vector<double> speed;
vector<double> speedX;
vector<double> speedY;
vector<double> speedCounter;
vector<double> speedPreAvg;
vector<double> speedAvg;
bool fromCenter=false;
//cover unit
double pixelToMeterfinal=0;
double pixelToMeter1=0;
double pixelToMeter2=0;
double pixelToMeter3=0;
double pixelToMeter4=0;
//real length
double reallMeter1=4;
double reallMeter2=4;
double reallMeter3=4;
double reallMeter4=4;
//pixel lentg
int rModifer=100;
int gModifer=0;
int bModifer=10;
 vector< vector<Point> >  roiPoint;
///

    // container of the tracked objects
    vector<Rect2d> rectsObjects;
    vector<Rect> rects;
    vector<Ptr<Tracker>> algorithms;
    void CallBackFunc(int event, int x, int y, int flags, void* userdata)
    {
        if  ( event == EVENT_LBUTTONDOWN )
        {
            cout << "Left button of the mouse is clicked - position (" << x << ", " << y << ")" << endl;
            cv::circle(frameMat,
                       cv::Point(x,y),
                   3,
                     cv::Scalar(255,0,0),
                     -1,
                     LINE_8
                     );
            
            roiPoint.push_back(vector<Point>());
            roiPoint[0].push_back( Point(x,y));
            cout<<roiPoint[0]<<endl;

        }
        else if  ( event == EVENT_RBUTTONDOWN )
        {
            cout << "Right button of the mouse is clicked - position (" << x << ", " << y << ")" << endl;
        }
        else if  ( event == EVENT_MBUTTONDOWN )
        {
            cout << "Middle button of the mouse is clicked - position (" << x << ", " << y << ")" << endl;
        }
        else if ( event == EVENT_MOUSEMOVE )
        {
            cout << "Mouse move over the window - position (" << x << ", " << y << ")" << endl;
        }
        
    }


double drwaLine(Mat frameMat, int x1, int y1, int x2, int y2)
{
    cv::line(frameMat,
             cv::Point(x1,y1)
             ,
             cv::Point(x2,y2),
             cv::Scalar(255,0,0),
             1,
             LINE_8
             );
    double lineDist=sqrt(pow((x1-x2),2)-pow((y1-y2),2));
    cout<<lineDist<<endl;
    return lineDist;
    
}
Mat resizeWindow(Mat frame)
{
    Roi=frame;
    Mat src=Roi;
    Mat dst=Roi;
    int new_w=0;
    int new_h=0;
    new_w=src.cols;
    new_h=src.rows;
    Rect rectROI(0,0,new_w,new_h);
    Mat mask(new_h, new_w, CV_8UC1, cv::Scalar(0));
    Point P1(0,210);
    Point P1_0(300,175);
    Point P1_1(600,145);
    Point P1_2(1300,145);
    Point P2(1920,160);
    Point P3(1920,250);
    Point P3_1(1300,220);
    Point P3_2(900,250);
    Point P3_3(600,250);
    Point P3_4(300,300);
    Point P4(0,350);
    vector< vector<Point> >  co_ordinates;
    co_ordinates.push_back(vector<Point>());
    co_ordinates[0].push_back(P1);
    co_ordinates[0].push_back(P1_0);
    co_ordinates[0].push_back(P1_1);
    co_ordinates[0].push_back(P1_2);
    co_ordinates[0].push_back(P2);
    co_ordinates[0].push_back(P3);
    co_ordinates[0].push_back(P3_1);
    co_ordinates[0].push_back(P3_2);
    co_ordinates[0].push_back(P3_3);
    co_ordinates[0].push_back(P3_4);
    co_ordinates[0].push_back(P4);
    drawContours( mask,roiPoint,0, Scalar(255),CV_FILLED, 8 );
    Mat srcROI=src(rectROI);
    Mat dstROI=dst(rectROI);
    Mat dst1;
    srcROI.copyTo(dst1,mask);
    return dst1;
}
int keyboard; //input from keyboard
void processVideo(char* videoFilename);
int main( int argc, char** argv ){
    if(argc<2){
        cout<<
        " Usage: tracker <video_name>\n"
        " examples:\n"
        " example_tracking_kcf Bolt/img/%04d.jpg\n"
        " example_tracking_kcf faceocc2.webm\n"
        << endl;
        return 0;
    }
       namedWindow("My Window", CV_WINDOW_AUTOSIZE);
       namedWindow("tracker", CV_WINDOW_AUTOSIZE);
     //namedWindow("My Window", 1);
    //create the tracker
    MultiTracker trackers;
    // set input video
    std::string video = argv[1];
    VideoCapture cap(video);
// skip frame function
//    Ptr<Tracker> tracker;
//    for( int x = 0; x < 5000; x++ ) {
    //cap>>frame;
    //}
    //open csv file
    ofstream outfile;
    outfile.open("./result.csv");
    outfile<<"time"<<"," << "id"<<","<<"x"<<","<<"y"<<","<<"speed"<<",";
    cap>>frame;
    
     resize(frame, frame, cv::Size(), 0.5, 0.5);
   // frame= resizeWindow(frame);
    //    resize(frame, frame, cv::Size(), 0.5, 0.5);
    //Rect Rec(0, 500, 1900, 380);
    //rectangle(frame, Rec, Scalar(255), 10, 8, 0);
    //Select area described by REC and result write to the Roi
    //frame = frame(Rec);
    frameMat=frame;
    //Create a window
    
    //set the callback function for any mouse event
    setMouseCallback("My Window", CallBackFunc, NULL);
    //show the image
  
     cout<<"please select ROI"<<endl;
    rectangle(frameMat, cv::Point(10, 2), cv::Point(450,20),
              cv::Scalar(255,255,255), -1);
    string displayInfor="please select ROI by left click mouse,esc to cancle";
    putText(frame, displayInfor.c_str(), cv::Point(15, 15),
            FONT_HERSHEY_SIMPLEX, 0.5 , cv::Scalar(255,0,0));
    for(;;)
    {
        imshow("My Window", frameMat);
        if(waitKey(1)==27)
        {
            roiPoint[0].pop_back();
            cv::circle(frameMat,
                       cv::Point(roiPoint[0][roiPoint[0].size()].x,roiPoint[0][roiPoint[0].size()].y),
                       3,
                       cv::Scalar(0,255,0),
                       -1,
                       LINE_8
                       );
            
            cout<<"esc pressed"<<roiPoint[0][0].x<<endl;
            
        }
        else if(waitKey(1)==13)
        {
            cout<<"key predded"<<endl;
            frame= resizeWindow(frame);
            break;
        }
    }
    
    
    
//    //filtering image
//    for( int x = 0; x < frame.rows; x++ ) {
//        for( int y = 0; y < frame.cols; y++ )
//        {
//            if (
//                frame.at<Vec3b>(x, y)[0]<= 80&&frame.at<Vec3b>(x, y)[0]>= 20&&
//                frame.at<Vec3b>(x, y)[1]<= 80&&frame.at<Vec3b>(x, y)[1]>= 20&&
//                frame.at<Vec3b>(x, y)[2]<= 80&&frame.at<Vec3b>(x, y)[2]>= 20
//
//                )
//            {
//                frame.at<Vec3b>(x, y)[0] +=rModifer;
//                frame.at<Vec3b>(x, y)[1] +=gModifer;
//                frame.at<Vec3b>(x, y)[2]+=bModifer;
//            }
//        }
//    }
    //cvtColor( frame, frame, CV_BGR2GRAY );
    //threshold( frame, frame, 127, 255,1 );
    //draw line
    ///line1
   displayInfor="choose kcf object";
    putText(frame, displayInfor.c_str(), cv::Point(15, 15),
            FONT_HERSHEY_SIMPLEX, 0.5 , cv::Scalar(255,0,0));
    double line1x1=991;
    double line1y1=460;
    double line1x2=1020;
    double line1y2=457;
    //line2
    double line2x1=1000;
    double line2y1=491;
    double line2x2=1029;
    double line2y2=490;
    ///line3
    double line3x1=1003;
    double line3y1=525;
    double line3x2=1033;
    double line3y2=523;
    //line4
    double line4x1=1010;
    double line4y1=559;
    double line4x2=1039;
    double line4y2=557;
    drwaLine(frameMat,line1x1,line1y1,line1x2,line1y2);
    drwaLine(frameMat,line2x1,line2y1,line2x2,line2y2);
    drwaLine(frameMat,line3x1,line3y1,line3x2,line3y2);
    drwaLine(frameMat,line4x1,line4y1,line4x2,line4y2);
    pixelToMeterfinal=drwaLine(frameMat,line1x1,line1y1,line1x2,line1y2)/reallMeter1;
    cout<<"real"<<pixelToMeterfinal<<endl;
    selectROIs("tracker",frame,rects,fromCenter);
    cout<<rects.size()<<endl;
    for(int i=0;i<rects.size();i++)
    {
        preX.push_back(0);
        preY.push_back(0);
        lastX.push_back(0);
        lastY.push_back(0);
        speed.push_back(0);
        speedX.push_back(0);
        speedY.push_back(0);
        speedCounter.push_back(0);
        speedAvg.push_back(0);
        speedPreAvg.push_back(0);
        cout<<"car:"<<i<<" x: "<<rects[i].x<<" y: "<<rects[i].y<<endl;
       // algorithms.push_back(TrackerKCF::create());
        rectsObjects.push_back(rects[i]);
        algorithms.push_back(TrackerKCF::create());
    }
    trackers.add(algorithms,frame,rectsObjects);
//     perform the tracking process
//    printf("Start the tracking process, press ESC to quit.\n");
//
//
//
//    for(;;)
//    {
//    if(waitKey(1)==27)break;
//        cout<<"wait for roi"<<endl;
//    }
//
    for(;;)
    {
        //string displayInfor="kcf start";
        putText(frame, displayInfor.c_str(), cv::Point(15, 15),
                FONT_HERSHEY_SIMPLEX, 0.5 , cv::Scalar(255,0,0));
        //roi=selectROI("tracker",frame);
        cout<<roiPoint[0]<<endl;
        cap>>frame;
        resize(frame, frame, cv::Size(), 0.5, 0.5);
       frame= resizeWindow(frame);
        // resize(frame, frame, cv::Size(), 0.5, 0.5);
        //resize(frame, frame, cv::Size(), 0.5,0.5);
        //Rect Rec(0, 500, 1900, 380);
        //rectangle(frame, Rec, Scalar(255), 1, 8, 0);
        //Select area described by REC and result write to the Roi
//        frame = frame(Rec);
//        if(waitKey(1)==27)
//        {
//            vector<Rect> rect2s;
//            // container of the tracked objects
//            vector<Rect2d> rectsObject2s;
//            vector<Ptr<Tracker>> algorithm2s;
//            cout<<"stop<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<"<<endl;
//            selectROIs("tracker",frame,rect2s,fromCenter);
//            for(int i=0;i<rect2s.size();i++)
//            {
//                preX.push_back(0);
//                preY.push_back(0);
//                lastX.push_back(0);
//                lastY.push_back(0);
//                speed.push_back(0);
//                speedX.push_back(0);
//                speedY.push_back(0);
//                speedCounter.push_back(0);
//                speedAvg.push_back(0);
//                speedPreAvg.push_back(0);
//                cout<<"car:"<<i<<" x: "<<rect2s[i].x<<" y: "<<rect2s[i].y<<endl;
//                // algorithms.push_back(TrackerKCF::create());
//                rectsObject2s.push_back(rect2s[i]);
//                algorithm2s.push_back(TrackerKCF::create());
//            }
//            trackers.add(algorithm2s,frame,rectsObject2s);
//        }
        stringstream ss;
           stringstream st;
        stringstream fps;
        rectangle(frame, cv::Point(10, 2), cv::Point(450,20),
                  cv::Scalar(255,255,255), -1);
        ss << cap.get(CAP_PROP_POS_FRAMES);
        fps << cap.get(CAP_PROP_FPS);
        st << cap.get( CAP_PROP_POS_MSEC);
        string frameNumberString = ss.str();
        string fpsNumberString = fps.str();
        string timeNumberString = st.str();
        double timeFrame=stod(timeNumberString)/1000+40;
      //  string timeNumberString = st.str();
        putText(frame, frameNumberString.c_str(), cv::Point(15, 15),
                FONT_HERSHEY_SIMPLEX, 0.5 , cv::Scalar(255,0,0));
        putText(frame, fpsNumberString.c_str(), cv::Point(70, 15),
                FONT_HERSHEY_SIMPLEX, 0.5 , cv::Scalar(255,0,0));
        putText(frame, to_string(timeFrame), cv::Point(190, 15),
                FONT_HERSHEY_SIMPLEX, 0.5 , cv::Scalar(255,0,0));
        // stop the program if no more images
        if(frame.rows==0 || frame.cols==0)
            break;
        frameCount++;
        if(frameCount==6)
        {
            frameCount=0;
        for(unsigned i=0;i<trackers.getObjects().size();i++)
        {
           //cout<<"tracker speed: "<<i<<"i"<<endl;
            if(lastX[i]==0&&lastY[i]==0)
            {
                //cout<<"init speed: "<<i<<"i"<<endl;
                // cout<<"first time : "<<i<<endl;
                lastX[i]=trackers.getObjects()[i].x;
                lastY[i]=trackers.getObjects()[i].y;
                speed[i]=0;
            }
            else
            {
                //cout<<"cal speed: "<<i<<"i"<<endl;
                speedCounter[i]+=1;
                preX[i]=trackers.getObjects()[i].x;
                preY[i]=trackers.getObjects()[i].y;
                speedX[i]=(preX[i]-lastX[i]);
                speedY[i]=(preY[i]-lastY[i]);
                lastX[i]=preX[i];
                lastY[i]= preY[i];
                speed[i]=(sqrt(speedX[i]*speedX[i]+speedY[i]*speedY[i]));
                speedPreAvg[i]+=speed[i];
                speedAvg[i]=speedPreAvg[i]/ speedCounter[i];
            }
            string location="X: "+to_string((int)lastX[i])+" Y: "+to_string((int)lastY[i]);
            string speedinfo="car:"+to_string(i)+" | Raw speed "+to_string(speed[i])+" | Avg speed "+to_string(speedAvg[i]*pixelToMeterfinal*59.97/6);
            string carInfo="width: "+to_string((int)(trackers.getObjects()[i].width-20))+" px"+"| height "+to_string((int)(trackers.getObjects()[i].height-20))+"px ";
                if(preY[i]<1700)
                {
                    if(speed[i]==0)
                    {
                         outfile<<endl;
                        outfile<<to_string(timeFrame)<<"," << i<<","<<preX[i]<<","<<preY[i]<<","<<"lost"<<",";
                    }
                    else
                    {
                         outfile<<endl;
                        outfile<<to_string(timeFrame)<<"," << i<<","<<preX[i]<<","<<preY[i]<<","<<speedAvg[i]*pixelToMeterfinal*59.97/6<<",";
                    }

                }
            
                    rectangle( frame, trackers.getObjects()[i], Scalar( 0, 255, 0 ), 0.01, 1 );

                    cv::putText(frame,to_string(i),
                                cv::Point(trackers.getObjects()[i].x+40,trackers.getObjects()[i].y+20), // Coordinates
                                cv::FONT_HERSHEY_COMPLEX_SMALL, // Font
                                1, // Scale. 2.0 = 2x bigger
                                cv::Scalar(0,255,0), // Color
                                1); // Anti-alias // show image with the tracked object
        }
        }
        trackers.update(frame);
        imshow("tracker",frame);
        //quit on ESC button
      //  if(waitKey(1)==27)break;
    }
    //delete capture object
   cap.release();
    outfile.close();
return 0;
}
