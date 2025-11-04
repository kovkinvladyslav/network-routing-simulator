#ifndef CREATECHANNELDIALOG_H
#define CREATECHANNELDIALOG_H

#include <QDialog>

QT_BEGIN_NAMESPACE
namespace Ui {
class CreateChannelDialog;
}
QT_END_NAMESPACE

class CreateChannelDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CreateChannelDialog(QWidget *parent = nullptr);

    bool isDuplex();
    bool isRandomWeight();

private:
    Ui::CreateChannelDialog *ui;
};

#endif // CREATECHANNELDIALOG_H
