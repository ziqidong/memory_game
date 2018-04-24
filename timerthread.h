#ifndef GTKMM_TIMERTHREAD_H
#define GTKMM_TIMERTHREAD_H

#include "gtkmm.h"
#include <thread>
#include <string.h>

class MyWindow;

class TimerThread
{
public:
  TimerThread();
  void do_work(MyWindow* mw);
  std::string gettime();
  void newgame();

std::string time = "";
std::string sminute = "";
std::string sseconds = "";
int minute = 0;
int seconds = 0;
bool finish = false;
bool pause = false;
};

#endif // GTKMM_TIMERTHREAD_H