/********************************************************************************
** Form generated from reading UI file 'dialog.ui'
**
** Created by: Qt User Interface Compiler version 5.12.8
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DIALOG_H
#define UI_DIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Dialog
{
public:
    QGridLayout *gridLayout;
    QVBoxLayout *verticalLayout_3;
    QHBoxLayout *horizontalLayout_3;
    QListWidget *lwLog;
    QListWidget *lwUsers;
    QHBoxLayout *horizontalLayout_2;
    QPlainTextEdit *pteMessage;
    QVBoxLayout *verticalLayout_2;
    QVBoxLayout *verticalLayout;
    QLineEdit *lePort;
    QLineEdit *leHost;
    QPushButton *pbStartStop;
    QHBoxLayout *horizontalLayout;
    QPushButton *pbSend;
    QSpacerItem *horizontalSpacer;
    QCheckBox *cbToAll;

    void setupUi(QWidget *Dialog)
    {
        if (Dialog->objectName().isEmpty())
            Dialog->setObjectName(QString::fromUtf8("Dialog"));
        Dialog->resize(540, 414);
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(Dialog->sizePolicy().hasHeightForWidth());
        Dialog->setSizePolicy(sizePolicy);
        Dialog->setMinimumSize(QSize(540, 414));
        Dialog->setMaximumSize(QSize(540, 414));
        gridLayout = new QGridLayout(Dialog);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        lwLog = new QListWidget(Dialog);
        lwLog->setObjectName(QString::fromUtf8("lwLog"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(lwLog->sizePolicy().hasHeightForWidth());
        lwLog->setSizePolicy(sizePolicy1);
        lwLog->setSelectionRectVisible(false);

        horizontalLayout_3->addWidget(lwLog);

        lwUsers = new QListWidget(Dialog);
        lwUsers->setObjectName(QString::fromUtf8("lwUsers"));
        QSizePolicy sizePolicy2(QSizePolicy::Minimum, QSizePolicy::Fixed);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(lwUsers->sizePolicy().hasHeightForWidth());
        lwUsers->setSizePolicy(sizePolicy2);
        lwUsers->setSelectionMode(QAbstractItemView::MultiSelection);
        lwUsers->setSortingEnabled(true);

        horizontalLayout_3->addWidget(lwUsers);


        verticalLayout_3->addLayout(horizontalLayout_3);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        pteMessage = new QPlainTextEdit(Dialog);
        pteMessage->setObjectName(QString::fromUtf8("pteMessage"));

        horizontalLayout_2->addWidget(pteMessage);

        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        lePort = new QLineEdit(Dialog);
        lePort->setObjectName(QString::fromUtf8("lePort"));

        verticalLayout->addWidget(lePort);

        leHost = new QLineEdit(Dialog);
        leHost->setObjectName(QString::fromUtf8("leHost"));

        verticalLayout->addWidget(leHost);

        pbStartStop = new QPushButton(Dialog);
        pbStartStop->setObjectName(QString::fromUtf8("pbStartStop"));
        pbStartStop->setCheckable(true);
        pbStartStop->setChecked(false);

        verticalLayout->addWidget(pbStartStop);


        verticalLayout_2->addLayout(verticalLayout);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        pbSend = new QPushButton(Dialog);
        pbSend->setObjectName(QString::fromUtf8("pbSend"));

        horizontalLayout->addWidget(pbSend);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        cbToAll = new QCheckBox(Dialog);
        cbToAll->setObjectName(QString::fromUtf8("cbToAll"));
        cbToAll->setChecked(true);
        cbToAll->setTristate(false);

        horizontalLayout->addWidget(cbToAll);


        verticalLayout_2->addLayout(horizontalLayout);


        horizontalLayout_2->addLayout(verticalLayout_2);


        verticalLayout_3->addLayout(horizontalLayout_2);


        gridLayout->addLayout(verticalLayout_3, 0, 0, 1, 1);


        retranslateUi(Dialog);

        QMetaObject::connectSlotsByName(Dialog);
    } // setupUi

    void retranslateUi(QWidget *Dialog)
    {
        Dialog->setWindowTitle(QApplication::translate("Dialog", "Server", nullptr));
        pteMessage->setPlainText(QString());
        lePort->setText(QApplication::translate("Dialog", "1234", nullptr));
        leHost->setText(QApplication::translate("Dialog", "127.0.0.1", nullptr));
        pbStartStop->setText(QApplication::translate("Dialog", "Stop server", nullptr));
        pbSend->setText(QApplication::translate("Dialog", "Send To All", nullptr));
        cbToAll->setText(QApplication::translate("Dialog", "To All", nullptr));
#ifndef QT_NO_SHORTCUT
        cbToAll->setShortcut(QString());
#endif // QT_NO_SHORTCUT
    } // retranslateUi

};

namespace Ui {
    class Dialog: public Ui_Dialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DIALOG_H
