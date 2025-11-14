#include "messagestatsdialog.h"
#include "ui_messagestatsdialog.h"

MessageStatsDialog::MessageStatsDialog(
    const std::vector<MessageLogEntry>& log,
    QWidget *parent)
    : QDialog(parent),
    ui(new Ui::MessageStatsDialog),
    model(new QStandardItemModel(this))
{
    ui->setupUi(this);

    model->setHorizontalHeaderLabels({
        "Src","Dst","Mode","Msg Size","Pkt Size",
        "Packets","Service Bytes","Total Time (ms)","Path"
    });

    model->setRowCount(log.size());
    int row = 0;

    for (const auto& e : log) {
        model->setItem(row, 0, new QStandardItem(QString::number(e.srcId)));
        model->setItem(row, 1, new QStandardItem(QString::number(e.dstId)));
        model->setItem(row, 2, new QStandardItem(e.mode));
        model->setItem(row, 3, new QStandardItem(QString::number(e.messageSize)));
        model->setItem(row, 4, new QStandardItem(QString::number(e.packetSize)));
        model->setItem(row, 5, new QStandardItem(QString::number(e.totalPackets)));
        model->setItem(row, 6, new QStandardItem(QString::number(e.serviceBytes)));
        model->setItem(row, 7, new QStandardItem(QString::number(e.totalTime, 'f', 2)));
        model->setItem(row, 8, new QStandardItem(e.pathString));
        row++;
    }

    ui->tableView->setModel(model);
    ui->tableView->resizeColumnsToContents();
}

MessageStatsDialog::~MessageStatsDialog() {
    delete ui;
}
