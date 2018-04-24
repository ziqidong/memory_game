#ifndef MYWINDOW_H
#define MYWINDOW_H

#include <gtkmm.h>
#include <gtkmm/application.h>
#include <vector>  
#include "matrix.h"
#include "buttons.h"
#include "waitthread.h"
#include "timerthread.h"
using namespace std; 

class MyWindow : public Gtk::Window
{
public:
  MyWindow();
    static const int yes_response = -8;
    static const int no_response = -9;
    static const int ok_response = -5;

    Buttons* buttons[16];
    vector<int> buttonfinished; 
    int buttonselected [2];
    Glib::Dispatcher m_Dispatcher;
    Glib::Dispatcher m_Dispatcher_tt;
    WaitThread wt;
    TimerThread tt;
    std::thread* m_WaitThread;
    std::thread* m_TimerThread;
    Gtk::Grid *grid = Gtk::manage(new Gtk::Grid);
    Gtk::Entry *todo_entry = Gtk::manage(new Gtk::Entry());
    Gtk::Button *b_add = Gtk::manage(new Gtk::Button("New Game"));
    Gtk::Button *b_saveEdit = Gtk::manage(new Gtk::Button("blabla"));
    Gtk::Button *b_delete = Gtk::manage(new Gtk::Button("blabla"));
    Glib::RefPtr<Gtk::Adjustment> m_adjustment;
    Gtk::Label *time = Gtk::manage(new Gtk::Label("00:00"));
    void set_all_insensitive(int c1[2], int c2[2]);
    void set_notfinishedbuttons_sensitive();
    void set_button_finished(int c1[2], int c2[2]);
    void new_game();
    void turn_selectedbuttons_back();
    void waitthread_notify();
    void timerthread_notify();
    void on_notification_from_wait_thread();
    void on_notification_from_timer_thread();

  virtual ~MyWindow();

protected:
  //Signal handlers:
  void on_menu_file_new_generic();
  void on_menu_file_quit();

  void on_addEntry_clicked();
  void on_help_clicked();
  void restart();
  void pause();
  void on_delete_clicked();

  //Child widgets:
  Gtk::Box m_Box;
  Gtk::Box d_Box;
  Gtk::Box h1_Box;
  Gtk::Box h2_Box;
  Gtk::Box h3_Box;
  Gtk::Scale m_HScale;
  Gdk::Pixbuf *pixmap;
  std::string iconpath="icon.png";

  Gtk::Grid m_Grid;
  Gtk::ProgressBar m_ProgressBar;
  Gtk::ScrolledWindow m_ScrolledWindow;

  Glib::RefPtr<Gtk::UIManager> m_refUIManager;
  Glib::RefPtr<Gtk::ActionGroup> m_refActionGroup;
};

#endif //MYWINDOW_H