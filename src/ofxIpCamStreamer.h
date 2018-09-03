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
    ofImage imageLoad;

    ofxIpCamStreamer(){
        cout << cv::getBuildInformation() << endl;
        ipCam.open("rtsp://admin:Punkt123@192.168.0.60", cv::CAP_FFMPEG);
        ipCam.set(CV_CAP_PROP_BUFFERSIZE, 5);
        if(!ipCam.isOpened())
        {
            ofLog() << "ipCam input error\n";
            useLoadedImage = true;
        }
        else ipCam.read(camMat);

        imageLoad.load("camMat_day.jpg");

        
        calibration.load("calibrationCamera.yml");
        if(calibration.isReady()) parameters.add(doUndistort);

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
            if(!useLoadedImage){
                ipCam.read(camMat);
                cvtColor(camMat, camMat, CV_BGR2RGB);
            }
            else toCv(imageLoad).copyTo(camMat);

            if(camMat.rows != outWidth && camMat.cols != outHeight) resize(camMat, camMat, cv::Size(outWidth.get(), outHeight.get()), 0, 0, INTER_CUBIC);
            if(doUndistort && calibration.isReady()) calibration.undistort(camMat, camMat);
            
            if(tryLock()){
            camMat.copyTo(outMat);
            unlock();
            }
        }
    }
    
    void close(){
        stopThread();
    }
    
    
    ofParameter<int> outWidth{"outWidth", 1920, 0, 1920};
    ofParameter<int> outHeight{"outHeight", 1080, 0, 1080};
    ofParameter<bool> doUndistort{"doUndistort", false};
    
    ofParameterGroup parameters{"IP_Cam", outWidth, outHeight};
    
private:
    VideoCapture ipCam;
    cv::Mat camMat, outMat;
    Calibration calibration;
    bool useLoadedImage;
};

#endif /* IpCamStreamer_h */
