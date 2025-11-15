#ifndef MESSAGESTATSDIALOG_H
#define MESSAGESTATSDIALOG_H

#include <QDialog>
#include <QStandardItemModel>
#include "messagestats.h"

namespace Ui {
class MessageStatsDialog;
}

class MessageStatsDialog : public QDialog {
    Q_OBJECT
public:
    explicit MessageStatsDialog(const std::vector<MessageLogEntry>& log,
                                QWidget* parent = nullptr);
    ~MessageStatsDialog();

private slots:
    void on_copyButton_clicked();

private:
    QStandardItemModel* model;
    Ui::MessageStatsDialog *ui;
};

#endif // MESSAGESTATSDIALOG_H
