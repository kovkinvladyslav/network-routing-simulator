/********************************************************************************
** Form generated from reading UI file 'createchanneldialog.ui'
**
** Created by: Qt User Interface Compiler version 6.9.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CREATECHANNELDIALOG_H
#define UI_CREATECHANNELDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAbstractButton>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_CreateChannelDialog
{
public:
    QVBoxLayout *verticalLayout;
    QGroupBox *groupBox;
    QHBoxLayout *horizontalLayout_3;
    QRadioButton *Type_Half_Duplex;
    QRadioButton *Duplex;
    QGroupBox *groupBox_2;
    QHBoxLayout *horizontalLayout;
    QRadioButton *Weight_Random;
    QRadioButton *Weight_Manually;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *CreateChannelDialog)
    {
        if (CreateChannelDialog->objectName().isEmpty())
            CreateChannelDialog->setObjectName("CreateChannelDialog");
        CreateChannelDialog->resize(400, 300);
        verticalLayout = new QVBoxLayout(CreateChannelDialog);
        verticalLayout->setObjectName("verticalLayout");
        groupBox = new QGroupBox(CreateChannelDialog);
        groupBox->setObjectName("groupBox");
        horizontalLayout_3 = new QHBoxLayout(groupBox);
        horizontalLayout_3->setObjectName("horizontalLayout_3");
        Type_Half_Duplex = new QRadioButton(groupBox);
        Type_Half_Duplex->setObjectName("Type_Half_Duplex");
        Type_Half_Duplex->setChecked(true);

        horizontalLayout_3->addWidget(Type_Half_Duplex);

        Duplex = new QRadioButton(groupBox);
        Duplex->setObjectName("Duplex");
        Duplex->setChecked(false);

        horizontalLayout_3->addWidget(Duplex);


        verticalLayout->addWidget(groupBox);

        groupBox_2 = new QGroupBox(CreateChannelDialog);
        groupBox_2->setObjectName("groupBox_2");
        horizontalLayout = new QHBoxLayout(groupBox_2);
        horizontalLayout->setObjectName("horizontalLayout");
        Weight_Random = new QRadioButton(groupBox_2);
        Weight_Random->setObjectName("Weight_Random");
        Weight_Random->setChecked(true);

        horizontalLayout->addWidget(Weight_Random);

        Weight_Manually = new QRadioButton(groupBox_2);
        Weight_Manually->setObjectName("Weight_Manually");

        horizontalLayout->addWidget(Weight_Manually);


        verticalLayout->addWidget(groupBox_2);

        buttonBox = new QDialogButtonBox(CreateChannelDialog);
        buttonBox->setObjectName("buttonBox");
        buttonBox->setStandardButtons(QDialogButtonBox::StandardButton::Cancel|QDialogButtonBox::StandardButton::Ok);

        verticalLayout->addWidget(buttonBox);


        retranslateUi(CreateChannelDialog);

        QMetaObject::connectSlotsByName(CreateChannelDialog);
    } // setupUi

    void retranslateUi(QDialog *CreateChannelDialog)
    {
        CreateChannelDialog->setWindowTitle(QCoreApplication::translate("CreateChannelDialog", "Dialog", nullptr));
        groupBox->setTitle(QCoreApplication::translate("CreateChannelDialog", "Choose channel type:", nullptr));
        Type_Half_Duplex->setText(QCoreApplication::translate("CreateChannelDialog", "Half-Duplex", nullptr));
        Duplex->setText(QCoreApplication::translate("CreateChannelDialog", "Duplex", nullptr));
        groupBox_2->setTitle(QCoreApplication::translate("CreateChannelDialog", "Choose how to set a weight:", nullptr));
        Weight_Random->setText(QCoreApplication::translate("CreateChannelDialog", "Random", nullptr));
        Weight_Manually->setText(QCoreApplication::translate("CreateChannelDialog", "Manually", nullptr));
    } // retranslateUi

};

namespace Ui {
    class CreateChannelDialog: public Ui_CreateChannelDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CREATECHANNELDIALOG_H
