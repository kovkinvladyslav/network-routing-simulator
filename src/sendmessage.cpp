#include "sendmessage.h"
#include "ui_sendmessage.h"
#include <QMessageBox>

SendMessage::SendMessage(const std::vector<Node*>& nodes, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::SendMessage)
    , availableNodes(nodes)
{
    ui->setupUi(this);

    for (Node* n : availableNodes) {
        ui->SourceNode->addItem(QString::number(n->getId()));
        ui->comboNodeB->addItem(QString::number(n->getId()));
    }

    ui->msgSizeTypeInp->addItems({"Bytes", "KB", "MB"});
    ui->msgSizeInput->setRange(1, 1000000);

    ui->pkgTypeInp->addItems({"Bytes", "KB"});
    ui->pkgSzieInp->setRange(24, 65535);

    ui->radioButton->setChecked(true);

    connect(ui->buttonBox, &QDialogButtonBox::accepted, this, &SendMessage::accept);
    connect(ui->buttonBox, &QDialogButtonBox::rejected, this, &SendMessage::reject);

    connect(ui->SourceNode, &QComboBox::currentIndexChanged, this, [this]{
        if (ui->SourceNode->currentText() == ui->comboNodeB->currentText())
            ui->comboNodeB->setCurrentIndex(0);
    });
    connect(ui->comboNodeB, &QComboBox::currentIndexChanged, this, [this]{
        if (ui->SourceNode->currentText() == ui->comboNodeB->currentText())
            ui->SourceNode->setCurrentIndex(0);
    });

}

SendMessage::~SendMessage()
{
    delete ui;
}

Node* SendMessage::getSource() const
{
    int id = ui->SourceNode->currentText().toInt();
    for (Node* n : availableNodes)
        if (n->getId() == id) return n;
    return nullptr;
}

Node* SendMessage::getDestination() const
{
    int id = ui->comboNodeB->currentText().toInt();
    for (Node* n : availableNodes)
        if (n->getId() == id) return n;
    return nullptr;
}

int SendMessage::getMessageSizeBytes() const
{
    int size = ui->msgSizeInput->value();
    QString unit = ui->msgSizeTypeInp->currentText();

    if (unit == "KB") size *= 1024;
    if (unit == "MB") size *= 1024 * 1024;

    return size;
}

int SendMessage::getPacketSizeBytes() const
{
    int size = ui->pkgSzieInp->value();
    QString unit = ui->pkgTypeInp->currentText();

    if (unit == "KB") size *= 1024;

    return size;
}

bool SendMessage::isDatagramMode() const
{
    return ui->radioButton->isChecked();
}

void SendMessage::accept()
{
    if (getSource() == getDestination()) {
        QMessageBox::warning(this, "Selection Error", "Source and Destination must be different.");
        return;
    }
    QDialog::accept();
}

void SendMessage::presetNodes(Node* a, Node* b)
{
    if (a)
        ui->SourceNode->setCurrentIndex(ui->SourceNode->findText(QString::number(a->getId())));
    if (b)
        ui->comboNodeB->setCurrentIndex(ui->comboNodeB->findText(QString::number(b->getId())));
}


