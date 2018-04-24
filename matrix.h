#ifndef MATRIX_H
#define MATRIX_H

#include <gtkmm/window.h>
#include <gtkmm/button.h>
class Matrix
{
public:
  Matrix();
  virtual ~Matrix();
  std::string getPosition(int row, int column);

  int button_clicked_nb=0;
  int clicked_bt1[2];
  int clicked_bt2[2];
  int mat [16];
};

#endif //MATRIX_H