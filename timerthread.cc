#include "timerthread.h"
#include "mywindow.h"
#include <sstream>
#include <iostream>
#include <string>
#include <chrono>

TimerThread::TimerThread() 
{
}

void TimerThread::do_work(MyWindow* mw)
{
	while(!finish){
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
		if(pause){
		}
		else{
			seconds++;
			if(seconds>59){
				seconds=0;
				minute++;
			}
			if(minute>59){
				seconds=0;
				minute=0;
			}
			if(minute<10){
				sminute = "0" + std::to_string(minute);
			}
			else{
				sminute = std::to_string(minute);
			}
			if(seconds<10){
				sseconds  = "0" + std::to_string(seconds);
			}
			else{
				sseconds = std::to_string(seconds);
			}
			time = sminute + ":" + sseconds;
			mw->timerthread_notify();
		}
	}
}

std::string TimerThread::gettime(){
	return sminute + "m " + sseconds + "s." ;
}

void TimerThread::newgame(){
	seconds = 0;
	minute = 0;
	sminute = "";
	sseconds = "";
	finish = false;
	pause = false;
}