#include "createchanneldialog.h"
#include "ui_createchanneldialog.h"
#include <QIntValidator>
#include <QPushButton>
#include <QMessageBox>


CreateChannelDialog::CreateChannelDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::CreateChannelDialog)
{
    ui->setupUi(this);
    connect(ui->buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(ui->buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);

    connect(ui->comboNodeA, &QComboBox::currentIndexChanged, this, [this]{
        if (getNodeA() == getNodeB() && getNodeA() != nullptr) {
            ui->comboNodeB->setCurrentIndex(0);
        }
    });

    connect(ui->comboNodeB, &QComboBox::currentIndexChanged, this, [this]{
        if (getNodeA() == getNodeB() && getNodeB() != nullptr) {
            ui->comboNodeA->setCurrentIndex(0);
        }
    });



    ui->manualWeightGroup->setVisible(false);

}

bool CreateChannelDialog::isDuplex() const
{
    return ui->Duplex->isChecked();
}

bool CreateChannelDialog::isRandomWeight() const
{
    return ui->Weight_Random->isChecked();
}


void CreateChannelDialog::setNodesList(const std::vector<Node*>& nodes)
{
    ui->comboNodeA->clear();
    ui->comboNodeB->clear();

    ui->comboNodeA->addItem("Select node", QVariant::fromValue((void*)nullptr));
    ui->comboNodeB->addItem("Select node", QVariant::fromValue((void*)nullptr));

    for (Node* n : nodes) {
        QString text = QString("Node %1").arg(n->getId());
        ui->comboNodeA->addItem(text, QVariant::fromValue((void*)n));
        ui->comboNodeB->addItem(text, QVariant::fromValue((void*)n));
    }
}

void CreateChannelDialog::presetNodes(Node* a, Node* b)
{
    if (a)
        ui->comboNodeA->setCurrentIndex(ui->comboNodeA->findText(QString("Node %1").arg(a->getId())));
    if (b)
        ui->comboNodeB->setCurrentIndex(ui->comboNodeB->findText(QString("Node %1").arg(b->getId())));
}

Node* CreateChannelDialog::getNodeA() const
{
    return (Node*) ui->comboNodeA->currentData().value<void*>();
}

Node* CreateChannelDialog::getNodeB() const
{
    return (Node*) ui->comboNodeB->currentData().value<void*>();
}

int CreateChannelDialog::getWeight() const
{
    return ui->spinBox->value();
}

void CreateChannelDialog::accept()
{
    if (!getNodeA() || !getNodeB()) {
        QMessageBox::warning(this, "No nodes selected", "Please select both nodes");
        return;
    }

    if (!isRandomWeight()) {
        if (ui->spinBox->value() < 1) {
            QMessageBox::warning(this, "Invalid weight", "Weight must be at least 1.");
            return;
        }
    }

    QDialog::accept();
}



void CreateChannelDialog::on_Weight_Random_toggled(bool checked)
{
    ui->manualWeightGroup->setVisible(!checked);
}



void CreateChannelDialog::on_Weight_Manually_toggled(bool checked)
{
    ui->manualWeightGroup->setVisible(checked);
}

ChannelType CreateChannelDialog::getChannelType() const
{
    return isDuplex() ? ChannelType::Duplex : ChannelType::HalfDuplex;
}

ChannelMode CreateChannelDialog::getChannelMode() const
{
    return (ui->Satelite->isChecked()) ? ChannelMode::Satellite : ChannelMode::Normal;
}


