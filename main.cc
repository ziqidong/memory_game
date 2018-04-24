#include "mywindow.h"
#include <gtkmm/application.h>

int main(int argc, char *argv[])
{
  Glib::RefPtr<Gtk::Application> app = Gtk::Application::create(argc, argv, "utm.LuchFilip");

  MyWindow window;

  //Shows the window and returns when it is closed.
  return app->run(window);
}
//g++ -std=c++11 mywindow.cc buttons.cc matrix.cc waitthread.cc main.cc -o main `pkg-config gtkmm-3.0 --cflags --libs`