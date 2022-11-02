#include <iostream>
#include <unistd.h>

using namespace std;

void CallBackPrintFrameInfo(int count) {
	cout << "count=" << count << endl;
}

void TimerTenSec (void (*callbackFunc)(int)) {
    int cnt = 10;
    int i = 0;
    while(i < cnt) {
    	cout << "i=" << i << endl;
	sleep(1);
	i++;
    }
    callbackFunc(i);
}

int main (int argc, char ** argv) {
	bool ret;

	TimerTenSec(CallBackPrintFrameInfo);

	while (1) {
		cout << "main function progress" << endl;
		sleep(5);
	}
}
