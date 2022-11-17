#include <iostream>
#include <unistd.h>
#include <functional>
#include <stdio.h>
#include <stdlib.h>
#include <error.h>
#include <errno.h>
#include <memory.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/mman.h>

using namespace std;

struct buffer {
	int index;
};
typedef buffer bufferinfo;

class VideoCapture {
public:
	void TimerTenSec(std::function<void(VideoCapture*, bufferinfo*, void*)> callbackFunc);
	
};

std::function<void(VideoCapture*, bufferinfo*, void*)> mCallback;

void * pixelbuffer;

void collectFrame() {
    if(mCallback)
    	mCallback(this, &bufferinfo, pixelbuffer);
}


void CallBackPrintFrameInfo(VideoCapture*, &bufferinfo, void*) {
	cout << "count=" << endl;
}

void VideoCapture::TimerTenSec (std::function<void(VideoCapture*, bufferinfo*, void*)> callbackFunc) {
    buffer bufferinfo = {};
    int cnt = 10;
    int i = 0;
   
    int fd = ::open("file_path", O_RDWR, 0);

    bufferinfo.index = 3;
    while(i < cnt) {
    	cout << "i=" << i << endl;
	sleep(1);
	i++;
    }

    memset(&bufferinfo, 0, sizeof(bufferinfo));
    bufferinfo.index = 3;

    pixelbuffer = mmap(NULL, 3, PROT_READ| PROT_WRITE, MAP_SHARED, fd, 2);

    mCallback = callbackFunc;
    std::thread mCaptureThread = std::thread([this]() { collectFrame();});

}

int main (int argc, char ** argv) {
	bool ret;

	TimerTenSec(CallBackPrintFrameInfo);

	while (1) {
		cout << "main function progress" << endl;
		sleep(5);
	}
}
