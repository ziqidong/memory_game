#include "buttons.h"
#include <iostream>
#include "mywindow.h"
Matrix* Buttons::matrix =  new Matrix();

Buttons::Buttons()
{
}

Buttons::Buttons(int row, int clonumn)
{
	this->row = row;
	this->clonumn = clonumn;
	button.add_pixlabel("icon.png", "");
	button.signal_clicked().connect(sigc::mem_fun(*this, &Buttons::on_button_clicked ));
}

Buttons::~Buttons()
{
}

void Buttons::on_button_clicked()
{
	if(matrix->button_clicked_nb==0){
		trunover();
		matrix->button_clicked_nb++;
		matrix->clicked_bt1[0] = row;
		matrix->clicked_bt1[1] = clonumn;
	}
	else if(matrix->button_clicked_nb==1){
		if(! (matrix->clicked_bt1[0] == row && matrix->clicked_bt1[1] == clonumn)){
			trunover();
			matrix->button_clicked_nb--;
			matrix->clicked_bt2[0] = row;
			matrix->clicked_bt2[1] = clonumn;
			if(matrix->mat[(matrix->clicked_bt1[0])*8 + matrix->clicked_bt1[1]] ==  matrix->mat[(matrix->clicked_bt2[0]) *8 + matrix->clicked_bt2[1]]){
				mw->set_button_finished(matrix->clicked_bt1, matrix->clicked_bt2);
				matrix->button_clicked_nb=0;
			}
			else{
				mw->set_all_insensitive(matrix->clicked_bt1, matrix->clicked_bt2);
			}
		}
	}
	else if(matrix->button_clicked_nb==2){
		trunback();
		button.set_sensitive(false);
		matrix->button_clicked_nb++;
	}
	else if(matrix->button_clicked_nb==3){
		trunback();
		matrix->button_clicked_nb=0;
		mw->set_notfinishedbuttons_sensitive();
	}
}

void Buttons::trunover(){
	button.remove();
	button.add_pixlabel(matrix->getPosition(row, clonumn), "");
}
void Buttons::trunback(){
	button.remove();
	button.add_pixlabel("icon.png", "");
}


