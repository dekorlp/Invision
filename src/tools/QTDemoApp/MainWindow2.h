#ifndef MAINWINDOW2_H
#define MAINWINDOW2_H

// For rebuilding QT moc File: moc MainWindow2.h -o MainWindow2.moc

#include "Window.h"

class MainWindow2 : public QMainWindow, public Ui::MainWindow
{

	Q_OBJECT

public:

	MainWindow2(QMainWindow *parent = 0)
	{
		setupUi(this);
		textBrowser->setText("Hallo Welt");


		connect(pushButton, SIGNAL(clicked()), this, SLOT(pushedEvent()));
	}



	~MainWindow2()
	{

	}



private slots:
	void pushedEvent()
	{
		textBrowser->setText("dssdsddssdsd");
		int test = 0;

	}

};

#endif
