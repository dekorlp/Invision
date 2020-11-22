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
		rtbOutput->setText("Hallo Welt");


		QObject::connect(btnStartStop, SIGNAL(clicked()), this, SLOT(startStopEvent()));
		QObject::connect(btnChangeText, SIGNAL(clicked()), this, SLOT(textChangeEvent()));
	}



	~MainWindow2()
	{

	}



private slots:
	void startStopEvent()
	{
		if (!leftWidget->GetContinousRender())
			leftWidget->SetContinousRender(true);
		else
			leftWidget->SetContinousRender(false);
	}

	void textChangeEvent()
	{
		rtbOutput->setText("dssdsddssdsd");
	}

};

#endif