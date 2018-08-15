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
        
        startThread();
    }
    
    ~ofxIpCamStreamer(){
        stopThread();
    }
    
    void setup(){


    }
    
    cv::Mat get(){
        lock();
        cv::Mat mat = outMat;
        unlock();

        return mat;
    }
    
    void draw(){
        ofxCv::drawMat(camMat, 0,0);
    }
    
    void threadedFunction() {
        while(isThreadRunning()) {
            ipCam.read(camMat);
            resize(camMat, camMat, cv::Size(1280, 720), 0, 0, INTER_CUBIC);
            cvtColor(camMat, camMat, CV_BGR2RGB);
            
            if(tryLock()){
            outMat = camMat;
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
};

#endif /* IpCamStreamer_h */
