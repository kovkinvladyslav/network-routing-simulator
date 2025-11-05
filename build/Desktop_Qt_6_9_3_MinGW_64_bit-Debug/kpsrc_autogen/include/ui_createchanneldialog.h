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
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_CreateChannelDialog
{
public:
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout_2;
    QVBoxLayout *verticalLayout_2;
    QLabel *label;
    QComboBox *comboNodeA;
    QVBoxLayout *verticalLayout_3;
    QLabel *label_2;
    QComboBox *comboNodeB;
    QGroupBox *groupBox;
    QHBoxLayout *horizontalLayout_3;
    QRadioButton *Type_Half_Duplex;
    QRadioButton *Duplex;
    QGroupBox *groupBox_3;
    QHBoxLayout *horizontalLayout_4;
    QRadioButton *Normal;
    QRadioButton *Satelite;
    QGroupBox *groupBox_2;
    QGridLayout *gridLayout;
    QRadioButton *Weight_Random;
    QRadioButton *Weight_Manually;
    QGroupBox *manualWeightGroup;
    QHBoxLayout *horizontalLayout;
    QLabel *label_3;
    QSpinBox *spinBox;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *CreateChannelDialog)
    {
        if (CreateChannelDialog->objectName().isEmpty())
            CreateChannelDialog->setObjectName("CreateChannelDialog");
        CreateChannelDialog->resize(400, 452);
        QSizePolicy sizePolicy(QSizePolicy::Policy::Preferred, QSizePolicy::Policy::MinimumExpanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(CreateChannelDialog->sizePolicy().hasHeightForWidth());
        CreateChannelDialog->setSizePolicy(sizePolicy);
        verticalLayout = new QVBoxLayout(CreateChannelDialog);
        verticalLayout->setObjectName("verticalLayout");
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName("horizontalLayout_2");
        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName("verticalLayout_2");
        label = new QLabel(CreateChannelDialog);
        label->setObjectName("label");

        verticalLayout_2->addWidget(label);

        comboNodeA = new QComboBox(CreateChannelDialog);
        comboNodeA->setObjectName("comboNodeA");

        verticalLayout_2->addWidget(comboNodeA);


        horizontalLayout_2->addLayout(verticalLayout_2);

        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setObjectName("verticalLayout_3");
        label_2 = new QLabel(CreateChannelDialog);
        label_2->setObjectName("label_2");

        verticalLayout_3->addWidget(label_2);

        comboNodeB = new QComboBox(CreateChannelDialog);
        comboNodeB->setObjectName("comboNodeB");

        verticalLayout_3->addWidget(comboNodeB);


        horizontalLayout_2->addLayout(verticalLayout_3);


        verticalLayout->addLayout(horizontalLayout_2);

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

        groupBox_3 = new QGroupBox(CreateChannelDialog);
        groupBox_3->setObjectName("groupBox_3");
        horizontalLayout_4 = new QHBoxLayout(groupBox_3);
        horizontalLayout_4->setObjectName("horizontalLayout_4");
        Normal = new QRadioButton(groupBox_3);
        Normal->setObjectName("Normal");
        Normal->setChecked(true);

        horizontalLayout_4->addWidget(Normal);

        Satelite = new QRadioButton(groupBox_3);
        Satelite->setObjectName("Satelite");

        horizontalLayout_4->addWidget(Satelite);


        verticalLayout->addWidget(groupBox_3);

        groupBox_2 = new QGroupBox(CreateChannelDialog);
        groupBox_2->setObjectName("groupBox_2");
        gridLayout = new QGridLayout(groupBox_2);
        gridLayout->setObjectName("gridLayout");
        Weight_Random = new QRadioButton(groupBox_2);
        Weight_Random->setObjectName("Weight_Random");
        Weight_Random->setChecked(true);

        gridLayout->addWidget(Weight_Random, 0, 0, 1, 1);

        Weight_Manually = new QRadioButton(groupBox_2);
        Weight_Manually->setObjectName("Weight_Manually");

        gridLayout->addWidget(Weight_Manually, 0, 1, 1, 1);


        verticalLayout->addWidget(groupBox_2);

        manualWeightGroup = new QGroupBox(CreateChannelDialog);
        manualWeightGroup->setObjectName("manualWeightGroup");
        QSizePolicy sizePolicy1(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(manualWeightGroup->sizePolicy().hasHeightForWidth());
        manualWeightGroup->setSizePolicy(sizePolicy1);
        horizontalLayout = new QHBoxLayout(manualWeightGroup);
        horizontalLayout->setObjectName("horizontalLayout");
        label_3 = new QLabel(manualWeightGroup);
        label_3->setObjectName("label_3");
        sizePolicy.setHeightForWidth(label_3->sizePolicy().hasHeightForWidth());
        label_3->setSizePolicy(sizePolicy);

        horizontalLayout->addWidget(label_3);

        spinBox = new QSpinBox(manualWeightGroup);
        spinBox->setObjectName("spinBox");
        spinBox->setMinimum(1);
        spinBox->setMaximum(2147483647);

        horizontalLayout->addWidget(spinBox);


        verticalLayout->addWidget(manualWeightGroup);

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
        label->setText(QCoreApplication::translate("CreateChannelDialog", "Node A", nullptr));
        label_2->setText(QCoreApplication::translate("CreateChannelDialog", "Node B", nullptr));
        groupBox->setTitle(QCoreApplication::translate("CreateChannelDialog", "Choose channel type:", nullptr));
        Type_Half_Duplex->setText(QCoreApplication::translate("CreateChannelDialog", "Half-Duplex", nullptr));
        Duplex->setText(QCoreApplication::translate("CreateChannelDialog", "Duplex", nullptr));
        groupBox_3->setTitle(QCoreApplication::translate("CreateChannelDialog", "Choose channel mode:", nullptr));
        Normal->setText(QCoreApplication::translate("CreateChannelDialog", "Normal", nullptr));
        Satelite->setText(QCoreApplication::translate("CreateChannelDialog", "Satelite", nullptr));
        groupBox_2->setTitle(QCoreApplication::translate("CreateChannelDialog", "Choose how to set a weight:", nullptr));
        Weight_Random->setText(QCoreApplication::translate("CreateChannelDialog", "Random", nullptr));
        Weight_Manually->setText(QCoreApplication::translate("CreateChannelDialog", "Manually", nullptr));
        manualWeightGroup->setTitle(QCoreApplication::translate("CreateChannelDialog", "Input weight:", nullptr));
        label_3->setText(QCoreApplication::translate("CreateChannelDialog", "Weight =", nullptr));
    } // retranslateUi

};

namespace Ui {
    class CreateChannelDialog: public Ui_CreateChannelDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CREATECHANNELDIALOG_H
