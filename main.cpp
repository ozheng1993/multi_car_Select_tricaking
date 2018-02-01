#include <opencv2/core/utility.hpp>
#include <opencv2/tracking.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
//#include <time.h>
//#include "opencv2/imgcodecs.hpp"
//#include "opencv2/imgcodecs.hpp"
//#include "opencv2/imgproc.hpp"
//#include <opencv2/video.hpp>
//#include <opencv2/core/ocl.hpp>
//#include "opencv2/opencv_modules.hpp"
//#include "opencv2/core.hpp"
//#include "opencv2/opencv.hpp"
//#include "opencv2/video/background_segm.hpp"
//
//#include "opencv2/core.hpp"
//#include "opencv2/imgproc/types_c.h"

//C
#include <stdio.h>
//C++
#include <iostream>
#include <sstream>
#include <ctime>
using namespace std;
using namespace cv;
Mat frame; //current frame mat
Mat frameMat; //current frame mat
Mat Roi;
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

// container of the tracked objects
vector<Rect2d> rectsObjects;
vector<Rect> rects;
vector<Ptr<Tracker>> algorithms;

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
    // declares all required variables
    //create GUI windows
//    namedWindow("tracker");
    //  create the tracker
    MultiTracker trackers;
    // set input video
    std::string video = argv[1];
    VideoCapture cap(video);
    // create a tracker object
//    Ptr<Tracker> tracker;
    cap>>frame;
     Rect Rec(0, 500, 1900, 380);
     rectangle(frame, Rec, Scalar(255), 10, 8, 0);
    //Select area described by REC and result write to the Roi
    frame = frame(Rec);
    frameMat=frame;
    // Change the background from white to black, since that will help later to extract
    // better results during the use of Distance Transform

    // Show output image
    imshow("Black Background Image", frameMat);
    
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
//    for()
//    //objects.push_back(rects);
//    //algorithms=TrackerKCF;
//
//     trackers.add(TrackerKCF;,frame,rects);
    //roi=selectROI("tracker",frame);
    // initialize the tracker
    //tracker->init(frame,roi);
    //        //Specify a vector of rectangles (ROI)
    //        vector<Rect2d> rects;
    //        bool fromCenter = false;
    // The selected rectangles are in
    
    

    ///

    // perform the tracking process
    printf("Start the tracking process, press ESC to quit.\n");
    //
    for(;;)
    {
        //roi=selectROI("tracker",frame);
        cap>>frame;
        Rect Rec(0, 500, 1900, 380);
        rectangle(frame, Rec, Scalar(255), 1, 8, 0);
        //Select area described by REC and result write to the Roi
        frame = frame(Rec);
        for( int x = 0; x < frame.rows; x++ ) {
            for( int y = 0; y < frame.cols; y++ )
            {
                if (
                    frame.at<Vec3b>(x, y)[0]<= 80&&frame.at<Vec3b>(x, y)[0]>= 20&&
                    frame.at<Vec3b>(x, y)[1]<= 80&&frame.at<Vec3b>(x, y)[1]>= 20&&
                    frame.at<Vec3b>(x, y)[2]<= 80&&frame.at<Vec3b>(x, y)[2]>= 20
                    
                    )
                {
                    frame.at<Vec3b>(x, y)[0] = 0;
                    frame.at<Vec3b>(x, y)[1] = 225;
                    frame.at<Vec3b>(x, y)[2] = 225;
                }
            }
        }
//        for(int i=0;i<rects.size();i++)
//        {
//            //cout<<"car:"<<i<<" x: "<<rects[i].x<<" y: "<<rects[i].y<<endl;
//             //rectangle( frame, rects[i], Scalar( 255, 0, 0 ), 2, 1 );
//        }
       
//        //get the frame number and write it on the current frame
//        stringstream ss;
//        stringstream fps;
//        rectangle(frame, cv::Point(10, 2), cv::Point(450,20),
//                  cv::Scalar(255,255,255), -1);
//        ss << cap.get(CAP_PROP_POS_FRAMES);
//        fps << cap.get(CAP_PROP_FPS);
//        string frameNumberString = ss.str();
//        string fpsNumberString = fps.str();
//        putText(frame, frameNumberString.c_str(), cv::Point(15, 15),
//                FONT_HERSHEY_SIMPLEX, 0.5 , cv::Scalar(255,0,0));
//        putText(frame, fpsNumberString.c_str(), cv::Point(55, 15),
//                FONT_HERSHEY_SIMPLEX, 0.5 , cv::Scalar(255,0,0));
        // stop the program if no more images
        if(frame.rows==0 || frame.cols==0)
            break;
        // update the tracking result
       // trackers->update(frame,roi);
      
        // draw the tracked object
        cout<<"tracker size"<<trackers.getObjects().size()<<endl;
        
        //rectangle(frame, cv::Point(10, 2), cv::Point(450,20*trackers.getObjects().size()),
               //   cv::Scalar(255,255,255), -1);
        
        for(unsigned i=0;i<trackers.getObjects().size();i++)
        {
            
            
            
            vector<double>dist(trackers.getObjects().size());
            
            if(i!=0)
            {
                
          
                
//                    dist[i]=sqrt ((trackers.getObjects()[0].x-trackers.getObjects()[i].x)*(trackers.getObjects()[0].x-trackers.getObjects()[i].x)+(trackers.getObjects()[0].y-trackers.getObjects()[i].y)*(trackers.getObjects()[0].y-trackers.getObjects()[i].y));
             dist[i]=sqrt ((trackers.getObjects()[0].x-trackers.getObjects()[i].x)*(trackers.getObjects()[0].x-trackers.getObjects()[i].x));

                
                    
                     string distanceInfo="(x only)dist between target and "+to_string(i)+" is "+to_string(dist[i]);
                cv::line(frame,
                      cv::Point(trackers.getObjects()[0].x+trackers.getObjects()[0].width/2,trackers.getObjects()[0].y+trackers.getObjects()[0].height/2)
                     , cv::Point(trackers.getObjects()[i].x+trackers.getObjects()[i].width/2,trackers.getObjects()[i].y+trackers.getObjects()[i].height/2),
                      cv::Scalar(255,0,0),
                     1,
                         LINE_8
                         );
                    cv::putText(frame,distanceInfo,
                                cv::Point(10, 20+i*12), // Coordinates
                                cv::FONT_HERSHEY_COMPLEX_SMALL, // Font
                                0.5, // Scale. 2.0 = 2x bigger
                                cv::Scalar(0,0,0), // Color
                                1); // Anti-alias // show image with the tracked object
                   // cout<<"dist between target and "<<i<<" is "<<dist[i]<<endl;
                
               
            }
            
            
            
            
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
                cout<<"speed: "<<i<<" : "<<speedAvg[i]<<endl;
            }
            
            if(speedAvg[i]!=0)
            {
                
                string location="X: "+to_string((int)lastX[i])+" Y: "+to_string((int)lastY[i]);
                string speed="car:"+to_string(i)+" | speed "+to_string(speedAvg[i]);
                string carInfo="width: "+to_string((int)(trackers.getObjects()[i].width-20))+" px"+"| height "+to_string((int)(trackers.getObjects()[i].height-20))+"px ";
                
                if(i==0)
                {
                    rectangle( frame, trackers.getObjects()[i], Scalar( 0, 255, 0 ), 0.01, 1 );
//                    cv::putText(frame,speed,
//                                cv::Point(trackers.getObjects()[i].x+20,trackers.getObjects()[i].y+20), // Coordinates
//                                cv::FONT_HERSHEY_COMPLEX_SMALL, // Font
//                                0.5, // Scale. 2.0 = 2x bigger
//                                cv::Scalar(0,255,0), // Color
//                                1); // Anti-alias // show image with the tracked object
                }
                else
                {
                    rectangle( frame, trackers.getObjects()[i], Scalar( 255, 255, 0 ), 0.01, 1 );
//                    cv::putText(frame,speed,
//                                cv::Point(trackers.getObjects()[i].x+20,trackers.getObjects()[i].y+20), // Coordinates
//                                cv::FONT_HERSHEY_COMPLEX_SMALL, // Font
//                                0.5, // Scale. 2.0 = 2x bigger
//                                cv::Scalar(255,0,0), // Color
//                                1); // Anti-alias // show image with the tracked object
                    
                    cv::putText(frame,speed,
                                cv::Point(400, 20+i*12), // Coordinates
                                cv::FONT_HERSHEY_COMPLEX_SMALL, // Font
                                0.5, // Scale. 2.0 = 2x bigger
                                cv::Scalar(0,0,0), // Color
                                1); // Anti-alias // show image with the tracked object
//
                }
              
                
//                cv::putText(frame,location,
//                            cv::Point(trackers.getObjects()[i].x+20,trackers.getObjects()[i].y+30), // Coordinates
//                            cv::FONT_HERSHEY_COMPLEX_SMALL, // Font
//                            0.5, // Scale. 2.0 = 2x bigger
//                            cv::Scalar(255,0,0), // Color
//                            1); // Anti-alias // show image with the tracked object
//
//                cv::putText(frame,carInfo,
//                            cv::Point(trackers.getObjects()[i].x+20,trackers.getObjects()[i].y+20), // Coordinates
//                            cv::FONT_HERSHEY_COMPLEX_SMALL, // Font
//                            0.5, // Scale. 2.0 = 2x bigger
//                            cv::Scalar(255,0,0), // Color
//                            1); // Anti-alias // show image with the tracked object
            }
            //rectangle( Roi, trackers.getObjects()[i], Scalar( 0, 255, 0 ), 0.25, 1 );
            //cout<<"tracker: "<<i<<"x:"<<trackers.getObjects()[i].x<<i<<"y:"<<trackers.getObjects()[i].y<<endl;
        }
        
        
        
        
        
        
        
        
        
         trackers.update(frame);
        
        imshow("tracker",frame);
        // Show output image
        imshow("Black Background Image", frameMat);
        // draw the tracked object
        //rectangle( frame, roi, Scalar( 255, 0, 0 ), 2, 1 );
        // show image with the tracked object
       
        
        //quit on ESC button
        if(waitKey(1)==27)break;
    }
    //delete capture object
   cap.release();
return 0;
}
