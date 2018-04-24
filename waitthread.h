#ifndef GTKMM_WaitThread_H
#define GTKMM_WaitThread_H

#include "gtkmm.h"
#include <thread>

class MyWindow;

class WaitThread
{
public:
  WaitThread();
  void do_work(MyWindow* mw);
};

#endif // GTKMM_WaitThread_H