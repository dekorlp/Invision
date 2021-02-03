/********************************************************************************
** Form generated from reading UI file 'QTDemo_MockupHp6580.ui'
**
** Created by: Qt User Interface Compiler version 5.9.9
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef QTDEMO_MOCKUPHP6580_H
#define QTDEMO_MOCKUPHP6580_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

#include "RenderWidget.h"
#include "renderer/GraphicsFactory.h"

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionNew;
    QAction *actionOpen;
    QWidget *centralwidget;
    QVBoxLayout *verticalLayout;
    QVBoxLayout *verticalLayout_2;
    QHBoxLayout *horizontalLayout_2;
	RenderWidget *leftWidget;
    QVBoxLayout *verticalLayout_5;
    QHBoxLayout *horizontalLayout;
    QPushButton *btnPushButton3;
    QPushButton *btnChangeText;
    QPushButton *btnStartStop;
    QTextBrowser *rtbOutput;
    QMenuBar *menubar;
    QMenu *menuFile;
    QStatusBar *statusbar;

	std::shared_ptr <Invision::IGraphicsEngine> graphicsEngine;

    void setupUi(QMainWindow *MainWindow, std::ofstream *ofstr)
    {

		// initialization Code
		std::vector<const char*> requiredExtensions = { "VK_KHR_surface", "VK_KHR_win32_surface" };


#ifdef NDEBUG
		graphicsEngine = Invision::create_engine(Invision::EngineType::Vulkan);
#else
		graphicsEngine = Invision::Create_engine(Invision::EngineType::Vulkan, ofstr);
#endif

		
		graphicsEngine->Init();

        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(1920, 1080);
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(MainWindow->sizePolicy().hasHeightForWidth());
        MainWindow->setSizePolicy(sizePolicy);
        actionNew = new QAction(MainWindow);
        actionNew->setObjectName(QStringLiteral("actionNew"));
        actionOpen = new QAction(MainWindow);
        actionOpen->setObjectName(QStringLiteral("actionOpen"));
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QStringLiteral("centralwidget"));
        QSizePolicy sizePolicy1(QSizePolicy::Maximum, QSizePolicy::Maximum);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(centralwidget->sizePolicy().hasHeightForWidth());
        centralwidget->setSizePolicy(sizePolicy1);
        verticalLayout = new QVBoxLayout(centralwidget);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        leftWidget = new RenderWidget(centralwidget, graphicsEngine);
        leftWidget->setObjectName(QStringLiteral("leftWidget"));
        leftWidget->setMinimumSize(QSize(100, 100));
        leftWidget->setMaximumSize(QSize(9000, 9000));
		leftWidget->SetContinousRender(true);

        horizontalLayout_2->addWidget(leftWidget);


        verticalLayout_2->addLayout(horizontalLayout_2);

        verticalLayout_5 = new QVBoxLayout();
        verticalLayout_5->setObjectName(QStringLiteral("verticalLayout_5"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        btnPushButton3 = new QPushButton(centralwidget);
        btnPushButton3->setObjectName(QStringLiteral("btnPushButton3"));

        horizontalLayout->addWidget(btnPushButton3);

        btnChangeText = new QPushButton(centralwidget);
        btnChangeText->setObjectName(QStringLiteral("btnChangeText"));
        btnChangeText->setBaseSize(QSize(0, 20));

        horizontalLayout->addWidget(btnChangeText);

        btnStartStop = new QPushButton(centralwidget);
        btnStartStop->setObjectName(QStringLiteral("btnStartStop"));

        horizontalLayout->addWidget(btnStartStop);


        verticalLayout_5->addLayout(horizontalLayout);

        rtbOutput = new QTextBrowser(centralwidget);
        rtbOutput->setObjectName(QStringLiteral("rtbOutput"));
        rtbOutput->setMinimumSize(QSize(0, 150));
        rtbOutput->setMaximumSize(QSize(16777215, 100));

        verticalLayout_5->addWidget(rtbOutput);


        verticalLayout_2->addLayout(verticalLayout_5);

        verticalLayout_2->setStretch(0, 95);
        verticalLayout_2->setStretch(1, 5);

        verticalLayout->addLayout(verticalLayout_2);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QStringLiteral("menubar"));
        menubar->setGeometry(QRect(0, 0, 1920, 21));
        menuFile = new QMenu(menubar);
        menuFile->setObjectName(QStringLiteral("menuFile"));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName(QStringLiteral("statusbar"));
        MainWindow->setStatusBar(statusbar);

        menubar->addAction(menuFile->menuAction());
        menuFile->addAction(actionNew);
        menuFile->addAction(actionOpen);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "QTDemo", Q_NULLPTR));
        actionNew->setText(QApplication::translate("MainWindow", "New", Q_NULLPTR));
        actionOpen->setText(QApplication::translate("MainWindow", "Open", Q_NULLPTR));
        btnPushButton3->setText(QApplication::translate("MainWindow", "PushButton3", Q_NULLPTR));
        btnChangeText->setText(QApplication::translate("MainWindow", "Change Text", Q_NULLPTR));
        btnStartStop->setText(QApplication::translate("MainWindow", "Start/Stop", Q_NULLPTR));
        menuFile->setTitle(QApplication::translate("MainWindow", "File", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // QTDEMO_MOCKUPHP6580_H
