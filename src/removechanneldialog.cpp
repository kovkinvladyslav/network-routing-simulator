#include "removechanneldialog.h"
#include "ui_removechanneldialog.h"
#include <QMessageBox>

RemoveChannelDialog::RemoveChannelDialog(QWidget *parent)
    : QDialog(parent), ui(new Ui::RemoveChannelDialog)
{
    ui->setupUi(this);

    connect(ui->buttonBox, &QDialogButtonBox::accepted, this, &RemoveChannelDialog::accept);
    connect(ui->buttonBox, &QDialogButtonBox::rejected, this, &RemoveChannelDialog::reject);

    connect(ui->comboNodeA, &QComboBox::currentIndexChanged, this, [this]{
        if (getNodeA() && getNodeA() == getNodeB())
            ui->comboNodeB->setCurrentIndex(0);
    });
    connect(ui->comboNodeB, &QComboBox::currentIndexChanged, this, [this]{
        if (getNodeB() && getNodeA() == getNodeB())
            ui->comboNodeA->setCurrentIndex(0);
    });


    connect(ui->comboNodeA, &QComboBox::currentIndexChanged, this, [this]{

        Node* a = getNodeA();
        if (!a) return;

        ui->comboNodeB->clear();
        ui->comboNodeB->addItem("Select neighbor", QVariant::fromValue((void*)nullptr));

        for (auto& [neighbor, props] : a->get_adj()) {
            QString text = QString("Node %1").arg(neighbor->getId());
            ui->comboNodeB->addItem(text, QVariant::fromValue((void*)neighbor));
        }
    });

}


void RemoveChannelDialog::setNodesList(const std::vector<Node*>& nodes)
{
    ui->comboNodeA->clear();
    ui->comboNodeB->clear();

    ui->comboNodeA->addItem("Select node", QVariant::fromValue((void*)nullptr));
    ui->comboNodeB->addItem("Select node", QVariant::fromValue((void*)nullptr));

    for (Node* n : nodes) {
        const QString text = QString("Node %1").arg(n->getId());
        ui->comboNodeA->addItem(text, QVariant::fromValue((void*)n));
        ui->comboNodeB->addItem(text, QVariant::fromValue((void*)n));
    }
}

void RemoveChannelDialog::presetNodes(Node* a, Node* b)
{
    if (a)
        ui->comboNodeA->setCurrentIndex(ui->comboNodeA->findText(QString("Node %1").arg(a->getId())));
    if (b)
        ui->comboNodeB->setCurrentIndex(ui->comboNodeB->findText(QString("Node %1").arg(b->getId())));
}

Node* RemoveChannelDialog::getNodeA() const
{
    return (Node*) ui->comboNodeA->currentData().value<void*>();
}

Node* RemoveChannelDialog::getNodeB() const
{
    return (Node*) ui->comboNodeB->currentData().value<void*>();
}

void RemoveChannelDialog::accept()
{
    if (!getNodeA() || !getNodeB()) {
        QMessageBox::warning(this, "Selection Error", "Please select two nodes.");
        return;
    }
    if (getNodeA() == getNodeB()) {
        QMessageBox::warning(this, "Selection Error", "Please select two different nodes.");
        return;
    }
    QDialog::accept();
}
