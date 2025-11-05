/********************************************************************************
** Form generated from reading UI file 'removechanneldialog.ui'
**
** Created by: Qt User Interface Compiler version 6.9.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_REMOVECHANNELDIALOG_H
#define UI_REMOVECHANNELDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAbstractButton>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_RemoveChannelDialog
{
public:
    QVBoxLayout *verticalLayout;
    QGroupBox *groupBox;
    QHBoxLayout *horizontalLayout;
    QComboBox *comboNodeA;
    QComboBox *comboNodeB;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *RemoveChannelDialog)
    {
        if (RemoveChannelDialog->objectName().isEmpty())
            RemoveChannelDialog->setObjectName("RemoveChannelDialog");
        RemoveChannelDialog->resize(400, 154);
        verticalLayout = new QVBoxLayout(RemoveChannelDialog);
        verticalLayout->setObjectName("verticalLayout");
        groupBox = new QGroupBox(RemoveChannelDialog);
        groupBox->setObjectName("groupBox");
        horizontalLayout = new QHBoxLayout(groupBox);
        horizontalLayout->setObjectName("horizontalLayout");
        comboNodeA = new QComboBox(groupBox);
        comboNodeA->setObjectName("comboNodeA");

        horizontalLayout->addWidget(comboNodeA);

        comboNodeB = new QComboBox(groupBox);
        comboNodeB->setObjectName("comboNodeB");

        horizontalLayout->addWidget(comboNodeB);


        verticalLayout->addWidget(groupBox);

        buttonBox = new QDialogButtonBox(RemoveChannelDialog);
        buttonBox->setObjectName("buttonBox");
        buttonBox->setStandardButtons(QDialogButtonBox::StandardButton::Cancel|QDialogButtonBox::StandardButton::Ok);

        verticalLayout->addWidget(buttonBox);


        retranslateUi(RemoveChannelDialog);

        QMetaObject::connectSlotsByName(RemoveChannelDialog);
    } // setupUi

    void retranslateUi(QDialog *RemoveChannelDialog)
    {
        RemoveChannelDialog->setWindowTitle(QCoreApplication::translate("RemoveChannelDialog", "Dialog", nullptr));
        groupBox->setTitle(QCoreApplication::translate("RemoveChannelDialog", "Choose channel to remove:", nullptr));
    } // retranslateUi

};

namespace Ui {
    class RemoveChannelDialog: public Ui_RemoveChannelDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_REMOVECHANNELDIALOG_H
