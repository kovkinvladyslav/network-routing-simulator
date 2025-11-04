#include "createchanneldialog.h"
#include "ui_createchanneldialog.h"

CreateChannelDialog::CreateChannelDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::CreateChannelDialog)
{
    ui->setupUi(this);
    connect(ui->buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(ui->buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);

}

bool CreateChannelDialog::isDuplex()
{
    return ui->Duplex->isChecked();
}

bool CreateChannelDialog::isRandomWeight()
{
    return ui->Type_Half_Duplex->isChecked();
}


