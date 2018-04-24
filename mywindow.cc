#include "mywindow.h"
#include <gtkmm/stock.h>
#include <iostream>

MyWindow::MyWindow()
: m_Box(Gtk::ORIENTATION_VERTICAL),
  d_Box(Gtk::ORIENTATION_VERTICAL),
  h1_Box(Gtk::ORIENTATION_HORIZONTAL),
  h2_Box(Gtk::ORIENTATION_HORIZONTAL),
  h3_Box(Gtk::ORIENTATION_HORIZONTAL),
  m_ProgressBar(),
  m_adjustment( Gtk::Adjustment::create(0, 0, 100, 1, 1)),
  m_HScale(m_adjustment, Gtk::ORIENTATION_HORIZONTAL),
  wt(),
  tt(),
  m_WaitThread(nullptr),
  m_TimerThread(nullptr),
  m_Dispatcher(),
  m_Dispatcher_tt()
{
  set_title("Jeu Memoire");
  set_default_size(1200, 600);
  set_icon_from_file(iconpath);


  add(m_Box);

  m_refActionGroup = Gtk::ActionGroup::create(); //为顶部工具栏创建菜单和和动作


  m_refActionGroup->add(Gtk::Action::create("GameMenu", "Game"));   //顶部菜单栏添加file选项，第一个参数对应ustring ui_info中的FileMenu, 第二个参数file为顶栏中该选项名称
 //File选项包含两个选项，FileNew和FileQuit
  m_refActionGroup->add(Gtk::Action::create("GameNew", Gtk::Stock::NEW));       //FileNew包含三个子选项
  m_refActionGroup->add(Gtk::Action::create("GameQuit", Gtk::Stock::QUIT), sigc::mem_fun(*this, &MyWindow::on_menu_file_quit)); //退出选项
   //FileNew中的三个子选项
  m_refActionGroup->add(Gtk::Action::create("GameEasy", Gtk::Stock::NEW, "Easy", "Create a new file"), sigc::mem_fun(*this, &MyWindow::on_menu_file_new_generic));
  m_refActionGroup->add(Gtk::Action::create("GameMedium", Gtk::Stock::NEW, "Medium", "Create a new foo"), sigc::mem_fun(*this, &MyWindow::on_menu_file_new_generic));
  m_refActionGroup->add(Gtk::Action::create("GameHard", Gtk::Stock::NEW, "Hard", "Create a new goo"), sigc::mem_fun(*this, &MyWindow::on_menu_file_new_generic));

  m_refActionGroup->add(Gtk::Action::create("RecordMenu", "Record"));   //顶部菜单栏添加Edit选项,第一个参数对应ustring ui_info中的EditMenu, 第二个参数edit为顶栏中该选项名称
  m_refActionGroup->add(Gtk::Action::create("RecordAll", "AllRecord"));

  m_refActionGroup->add( Gtk::Action::create("HelpMenu", "Help") );   //顶部菜单栏添加帮助菜单,第一个参数对应ustring ui_info中的HelpMenu, 第二个参数Help为顶栏中该选项名称
  m_refActionGroup->add( Gtk::Action::create("Help", Gtk::Stock::HELP), sigc::mem_fun(*this, &MyWindow::on_help_clicked) );
  m_refActionGroup->add( Gtk::Action::create("About", Gtk::Stock::ABOUT), sigc::mem_fun(*this, &MyWindow::on_help_clicked) );

  m_refActionGroup->add( Gtk::Action::create("Start", Gtk::Stock::MEDIA_PLAY), sigc::mem_fun(*this, &MyWindow::restart) );
  m_refActionGroup->add( Gtk::Action::create("Quit", Gtk::Stock::QUIT), sigc::mem_fun(*this, &MyWindow::on_menu_file_quit) );
  m_refActionGroup->add( Gtk::Action::create("Pause", Gtk::Stock::MEDIA_PAUSE), sigc::mem_fun(*this, &MyWindow::pause) );

  m_Dispatcher.connect(sigc::mem_fun(*this, &MyWindow::on_notification_from_wait_thread));
  m_Dispatcher_tt.connect(sigc::mem_fun(*this, &MyWindow::on_notification_from_timer_thread));

  m_refUIManager = Gtk::UIManager::create();
  m_refUIManager->insert_action_group(m_refActionGroup);    //将设置好的m_refActionGroup工具栏添加到UI中
  add_accel_group(m_refUIManager->get_accel_group());
  //顶栏布局文件
  Glib::ustring ui_info =
        "<ui>"
        "  <menubar name='MenuBar'>"
        "    <menu action='GameMenu'>"
        "      <menu action='GameNew'>"
        "        <menuitem action='GameEasy'/>"
        "        <menuitem action='GameMedium'/>"
        "        <menuitem action='GameHard'/>"
        "      </menu>"
        "      <separator/>"
        "      <menuitem action='GameQuit'/>"
        "    </menu>"
        "    <menu action='RecordMenu'>"
        "      <menuitem action='RecordAll'/>"
        "    </menu>"
        "    <menu action='HelpMenu'>"
        "      <menuitem action='Help'/>"
        "      <menuitem action='About'/>"
        "    </menu>"
        "  </menubar>"
        "  <toolbar  name='ToolBar'>"
        "    <toolitem action='Start'/>"
        "    <toolitem action='Pause'/>"
        "    <toolitem action='Quit'/>"
        "  </toolbar>"
        "</ui>";
  try
  {
    m_refUIManager->add_ui_from_string(ui_info);    //添加布局文件
  }
  catch(const Glib::Error& ex)
  {
    std::cerr << "building menus failed: " <<  ex.what();
  }

  Gtk::Widget* pMenubar = m_refUIManager->get_widget("/MenuBar");   
  Gtk::Widget* pToolbar = m_refUIManager->get_widget("/ToolBar") ;   
  if(pMenubar){
    m_Box.pack_start(*pMenubar, Gtk::PACK_SHRINK);    //将顶部工具栏在界面中显示出来
  }
  if(pToolbar){
    m_Box.pack_start(*pToolbar, Gtk::PACK_SHRINK);    //将顶部工具栏下面的两个图片在界面中显示出来
  }
//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
    m_Box.add(d_Box);   //添加d_box到主盒子中
    d_Box.add(h2_Box);  //h2_box负责显示进度条那一栏的box, 将其添加到d_box中
    h2_Box.set_vexpand(false);

    d_Box.pack_start(m_ProgressBar, Gtk::PACK_SHRINK);
    m_ProgressBar.set_text("Game done");
    m_ProgressBar.set_show_text();
    // h2_Box.pack_start(cell);   //添加滚动条到h2box中

    h2_Box.pack_start(*time, false, true, 40); 
    b_add->signal_clicked().connect(sigc::mem_fun(*this, &MyWindow::new_game));
    h2_Box.pack_start(*b_add, false, true, 5);    //添加新建任务的按钮

    //add Button which saves editing
    // b_saveEdit->signal_clicked().connect(sigc::mem_fun(*this, &MyWindow::onSaveEdit_clicked));
    b_saveEdit->set_sensitive(false);
    h2_Box.pack_start(*b_saveEdit, false, true, 5);   //添加Save Edit 的按钮

    // b_delete->signal_clicked().connect(sigc::mem_fun(*this, &MyWindow::on_delete_clicked));
    b_delete->set_sensitive(false);
    h2_Box.pack_start(*b_delete, false, true, 5);    //添加delete 的按钮
//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
  m_Grid.set_row_spacing(2);                          //图片行数
  m_Grid.set_column_spacing(8);                   //图片列数
  m_Grid.set_halign(Gtk::ALIGN_CENTER);    //水平居中
  m_ScrolledWindow.set_border_width(10);
  m_ScrolledWindow.set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);      //只在必要时显示滚动窗口 ：
  m_ScrolledWindow.add(m_Grid);   ////将m_Grid添加到一个滚动窗口中                   

  for(int i = 0; i < 8; i++)    //列数
  {
     for(int j = 0; j < 2; j++)   //行数
     {
        // sprintf(buffer, "button (%d,%d)\n", i, j);
        // auto pButton = Gtk::manage(new Gtk::ToggleButton(buffer));
        Buttons* bt = new Buttons(j, i);
        bt->mw = this;
        bt->button.set_sensitive(false);
        buttons[j*8 + i] = bt;
        m_Grid.attach(bt->button, i, j, 1, 1);    //添加按键
     }
  }

    d_Box.add(h3_Box);
    h3_Box.set_vexpand(true);
    h3_Box.pack_start(m_ScrolledWindow, true, true, 5);
    show_all_children();
}

MyWindow::~MyWindow()
{
}

void MyWindow::on_addEntry_clicked() {
}

void MyWindow::on_menu_file_quit()
{
         Gtk::MessageDialog quitDialog("Are you sure you want to Quit?",
                                        false,
                                        Gtk::MESSAGE_ERROR,
                                        Gtk::BUTTONS_YES_NO,
                                        true);
            quitDialog.set_title("Quit Dialog");
            int result = quitDialog.run();
        switch(result) {
            case(yes_response):
                hide();
                break;
            case(no_response):
                quitDialog.hide();
                break;
            }
}

void MyWindow::on_menu_file_new_generic()
{
}

void MyWindow::restart()
{
  tt.pause = false;
}

void MyWindow::pause()
{
  tt.pause = true;
}

void MyWindow::on_delete_clicked()
{
}

void MyWindow::on_help_clicked()
{
    Gtk::MessageDialog dlg("Help", false,
            Gtk::MESSAGE_INFO, Gtk::BUTTONS_OK, true);
    dlg.set_title("Help Dialog");
    dlg.set_secondary_text("Add items to your TODO list by writing the task in the entry box, and then press Ctrl+S or button Add. ");
    dlg.run();
}

void MyWindow::set_all_insensitive(int c1[2], int c2[2]){
  for(int i=0 ; i<16 ; i++){
    // if(i==c1[0]*8 + c1[1] || i==c2[0]*8 + c2[1])continue;
    buttons[i]->button.set_sensitive(false);
  }
  buttonselected[0] = c1[0]*8 + c1[1];
  buttonselected[1] = c2[0]*8 + c2[1];

  if (m_WaitThread)
  {
    std::cout << "Can't start a worker thread while another one is running." << std::endl;
  }
  else
  {
    m_WaitThread = new std::thread(
      [this]
      {
        wt.do_work(this);
      });
  }
}

void MyWindow::set_notfinishedbuttons_sensitive(){
  for(int i=0 ; i<16 ; i++){
    bool btfinished = false;
    std::vector<int>::iterator it;
    for(it = buttonfinished.begin(); it != buttonfinished.end(); it++){
    	if(i == *it){
    		btfinished = true;
    		break;
    	}
    }
    if(btfinished == false){
    	buttons[i]->button.set_sensitive(true);
    }
  }
}

 void MyWindow::set_button_finished(int c1[2], int c2[2]){
   buttonfinished.push_back(c1[0]*8 + c1[1]);
   buttonfinished.push_back(c2[0]*8 + c2[1]);
   buttons[c1[0]*8 + c1[1]]->button.set_sensitive(false);
   buttons[c2[0]*8 + c2[1]]->button.set_sensitive(false);
   m_ProgressBar.set_fraction(buttonfinished.size()/16.0);
   if(buttonfinished.size()==16){
            m_ProgressBar.set_fraction(0.0);
            tt.finish = true;
            // sleep(0.5);
            on_notification_from_timer_thread();
            time->set_text("00:00");
            //------------------------------------------------------
            Gtk::MessageDialog winDialog("You win, your record is " + tt.gettime(),  false, Gtk::MESSAGE_INFO, Gtk::BUTTONS_OK, true);
            winDialog.set_title("congratulation !");
            int result = winDialog.run();
            winDialog.hide();
            b_add->set_sensitive(true);
   }
 }

void MyWindow::new_game(){
  b_add->set_sensitive(false);
  tt.newgame();
  for(int i = 0; i< 16; i++){
    buttons[i]->trunback();
    buttons[i]->button.set_sensitive(true);
  }
  (buttons[0]->matrix)->button_clicked_nb=0;
  vector <int>().swap(buttonfinished);
  if (m_TimerThread)
  {
    std::cout << "Can't start a worker thread while another one is running." << std::endl;
  }
  else
  {
    tt.finish = false;
    m_TimerThread = new std::thread(
      [this]
      {
        tt.do_work(this);
      });
  }
}

 void MyWindow::turn_selectedbuttons_back(){
    for(int j=0 ; j<16; j++){
        if(buttonselected[0] == j){
          buttons[j]->trunback();
        }
      }
      for(int j=0 ; j<16; j++){
        if(buttonselected[1] == j){
          buttons[j]->trunback();
        }
      }
 }

void MyWindow::waitthread_notify(){
  m_Dispatcher.emit();
  turn_selectedbuttons_back();
  set_notfinishedbuttons_sensitive();
}

void MyWindow::on_notification_from_wait_thread()
{
    if (m_WaitThread->joinable())
      m_WaitThread->join();
    delete m_WaitThread;
    m_WaitThread = nullptr;
}

void MyWindow::timerthread_notify(){
  // m_Dispatcher_tt.emit();
  time->set_text(tt.time);
}

void MyWindow::on_notification_from_timer_thread()
{
    if (m_TimerThread->joinable())
      m_TimerThread->join();
    delete m_TimerThread;
    m_TimerThread = nullptr;
}