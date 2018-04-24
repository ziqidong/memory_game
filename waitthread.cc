#include "waitthread.h"
#include "mywindow.h"
#include <sstream>
#include <chrono>

WaitThread::WaitThread() 
{
}

void WaitThread::do_work(MyWindow* mw)
{
    std::this_thread::sleep_for(std::chrono::milliseconds(1500));
    mw->waitthread_notify();
}