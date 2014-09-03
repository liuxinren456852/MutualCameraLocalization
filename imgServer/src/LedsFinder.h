#ifndef _LEDSFINDER_H_
#define _LEDSFINDER_H_

#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/io/zero_copy_stream_impl.h>
#include "proto/img.pb.h"
#include "proto/command.pb.h"
#include "lodepng.h"
#include "lz4.h"
#include <time.h>
#include <stdlib.h> //calloc
#include <stdio.h>
#include <vector>
#include <sys/socket.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <unistd.h>
#include <netdb.h> 
#include <string>
#include <iostream>
#include <sstream>
#include <cstdlib> // exit()
#include <pthread.h>

#include "Camera.h"

#ifdef __arm__
    #include <arm_neon.h>
#endif


namespace img_server
{
class LedsFinder
{
private:
    Camera camera;
    long time;

    // TCP
    static const int BUFFER = 1024;
    static const int WIDTH = 640;
    static const int HEIGHT = 480;
    bool max_;
    char comBuffer[BUFFER];
    struct sockaddr_in ClientAddress;
    int comSocket;  
    int socketFileDescriptor; 

    // Threading
    pthread_t imgGathering;
    pthread_mutex_t proprietyMux; 
    volatile bool recording;
    
    void checkForErrorTCP(int errorCode, const char* errorMessage);
    long getTimeInMilliseconds();
    dotCapture::Command* getCommand();
    dotCapture::Img* dataToProto(long timestamp, int timestamp_microsec, unsigned char* data, long unsigned int size);

    static void* callLoopRecordingFunction(void *arg) { return ((LedsFinder*)arg)->loopRecording(); }

    void* loopRecording();

    void configServerParameter(int port);
    void waitingForClient();
    void waitingForCommandFromClient();
    void sendProto(dotCapture::Img* message);

public:
    LedsFinder(int port, int threshold, float shutter, int brightness, int exposure, float gain);
    ~LedsFinder();
    void startServerLoop();
};
}
#endif

