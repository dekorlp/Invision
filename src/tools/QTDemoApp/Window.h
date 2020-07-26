/********************************************************************************
** Form generated from reading UI file 'designerN17780.ui'
**
** Created by: Qt User Interface Compiler version 5.9.9
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef DESIGNERN17780_H
#define DESIGNERN17780_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

#include "RenderWidget.h"

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
	QWidget *centralwidget;
	QWidget *verticalLayoutWidget;
	QVBoxLayout *verticalLayout;
	//QWidget *widget;
	RenderWidget *widget;
	QTextBrowser *textBrowser;
	QHBoxLayout *horizontalLayout;
	QPushButton *pushButton_2;
	QPushButton *pushButton_3;
	QPushButton *pushButton;
	QMenuBar *menubar;
	QStatusBar *statusbar;

	void setupUi(QMainWindow *MainWindow)
	{
		if (MainWindow->objectName().isEmpty())
			MainWindow->setObjectName(QStringLiteral("MainWindow"));
		MainWindow->resize(800, 600);
		centralwidget = new QWidget(MainWindow);
		centralwidget->setObjectName(QStringLiteral("centralwidget"));
		verticalLayoutWidget = new QWidget(centralwidget);
		verticalLayoutWidget->setObjectName(QStringLiteral("verticalLayoutWidget"));
		verticalLayoutWidget->setGeometry(QRect(10, 30, 721, 501));
		verticalLayout = new QVBoxLayout(verticalLayoutWidget);
		verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
		verticalLayout->setSizeConstraint(QLayout::SetDefaultConstraint);
		verticalLayout->setContentsMargins(0, 0, 0, 0);
		widget = new RenderWidget(verticalLayoutWidget);
		widget->setObjectName(QStringLiteral("widget"));
		widget->setMaximumSize(QSize(16777215, 300));
		widget->SetContinousRender(true);

		verticalLayout->addWidget(widget);

		textBrowser = new QTextBrowser(verticalLayoutWidget);
		textBrowser->setObjectName(QStringLiteral("textBrowser"));
		textBrowser->setMaximumSize(QSize(16777215, 100));

		verticalLayout->addWidget(textBrowser);

		horizontalLayout = new QHBoxLayout();
		horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
		horizontalLayout->setSizeConstraint(QLayout::SetDefaultConstraint);
		pushButton_2 = new QPushButton(verticalLayoutWidget);
		pushButton_2->setObjectName(QStringLiteral("pushButton_2"));

		horizontalLayout->addWidget(pushButton_2);

		pushButton_3 = new QPushButton(verticalLayoutWidget);
		pushButton_3->setObjectName(QStringLiteral("pushButton_3"));

		horizontalLayout->addWidget(pushButton_3);

		pushButton = new QPushButton(verticalLayoutWidget);
		pushButton->setObjectName(QStringLiteral("pushButton"));

		horizontalLayout->addWidget(pushButton);


		verticalLayout->addLayout(horizontalLayout);

		MainWindow->setCentralWidget(centralwidget);
		menubar = new QMenuBar(MainWindow);
		menubar->setObjectName(QStringLiteral("menubar"));
		menubar->setGeometry(QRect(0, 0, 800, 21));
		MainWindow->setMenuBar(menubar);
		statusbar = new QStatusBar(MainWindow);
		statusbar->setObjectName(QStringLiteral("statusbar"));
		MainWindow->setStatusBar(statusbar);

		retranslateUi(MainWindow);
		
		QMetaObject::connectSlotsByName(MainWindow);
	} // setupUi

	void retranslateUi(QMainWindow *MainWindow)
	{
		MainWindow->setWindowTitle(QApplication::translate("MainWindow", "FirstApp", Q_NULLPTR));
		pushButton_2->setText(QApplication::translate("MainWindow", "PushButton", Q_NULLPTR));
		pushButton_3->setText(QApplication::translate("MainWindow", "PushButton", Q_NULLPTR));
		pushButton->setText(QApplication::translate("MainWindow", "PushButton", Q_NULLPTR));
	} // retranslateUi

};

namespace Ui {
	class MainWindow : public Ui_MainWindow
	{


	};
} // namespace Ui

QT_END_NAMESPACE

#endif // DESIGNERN17780_H
