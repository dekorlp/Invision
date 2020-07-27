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


		QObject::connect(pushButton, SIGNAL(clicked()), this, SLOT(startStopEvent()));
		QObject::connect(pushButton_3, SIGNAL(clicked()), this, SLOT(textChangeEvent()));
	}



	~MainWindow2()
	{
		
	}



private slots:
	void startStopEvent()
	{
		if (!widget->GetContinousRender())
			widget->SetContinousRender(true);
		else
			widget->SetContinousRender(false);
	}

	void textChangeEvent()
	{
		textBrowser->setText("dssdsddssdsd");
	}

};

#endif
