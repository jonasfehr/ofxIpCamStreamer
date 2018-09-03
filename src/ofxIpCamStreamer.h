//
//  IpCamStreamer.h
//  openCV
//
//  Created by Jonas Fehr on 09/08/2018.
//

#ifndef IpCamStreamer_h
#define IpCamStreamer_h

#include "ofMain.h"
#include "ofxCv.h"

using namespace cv;
using namespace ofxCv;

class ofxIpCamStreamer : public ofThread{
public:
    
    ofxIpCamStreamer(){
//        cout << cv::getBuildInformation() << endl;
        ipCam.open("rtsp://admin:Punkt123@192.168.0.60", cv::CAP_FFMPEG);
        ipCam.set(CV_CAP_PROP_BUFFERSIZE, 5);
        if(!ipCam.isOpened())
        {
            std::cout << "Input error\n";
            return -1;
        }
        ipCam.read(camMat);

        ofImage imageLoad;
        imageLoad.load("camMat_day.jpg");
        toCv(imageLoad).copyTo(camMat);

        
//        calibration.load("calibrations/calibrationCamera.yml");

        startThread();
        
        outMat = Mat::zeros(1920,1080, CV_8UC3);
    }
    
    ~ofxIpCamStreamer(){
        stopThread();
    }
    
    void setup(){


    }
    
    void start(){
        startThread();
    }
    
    void stop(){
        stopThread();
    }
    
    cv::Mat get(){
        lock();
        cv::Mat mat = outMat;
        unlock();

        return mat;
    }
    
    void draw(){
        ofxCv::drawMat(this->get(), 0,0);
    }
    
    void threadedFunction() {
        while(isThreadRunning()) {
            ipCam.read(camMat);
//            resize(camMat, camMat, cv::Size(1280, 720), 0, 0, INTER_CUBIC);
            cvtColor(camMat, camMat, CV_BGR2RGB);
            if(calibration.isReady()) calibration.undistort(camMat, camMat);
            
            if(tryLock()){
            camMat.copyTo(outMat);
            unlock();
            }
        }
    }
    
    void close(){
        stopThread();
    }
    
private:
    VideoCapture ipCam;
    cv::Mat camMat, outMat;
    Calibration calibration;
};

#endif /* IpCamStreamer_h */
