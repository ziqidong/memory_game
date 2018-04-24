#ifndef BUTTONS_H
#define BUTTONS_H

#include <gtkmm/window.h>
#include <gtkmm/button.h>
#include "matrix.h"

class MyWindow;

class Buttons
{
public:
  Buttons();
  Buttons(int row, int clonumn);
  virtual ~Buttons();
  void on_button_clicked();
  void trunover();
  void trunback();
  void a();

  static Matrix* matrix;
  MyWindow* mw;
  int state = 0;
  int row;
  int clonumn;
  Gtk::Button button;
};

#endif //GTKMM_EXAMPLE_BUTTONS_H