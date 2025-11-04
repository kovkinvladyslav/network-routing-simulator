#ifndef CREATECHANNELDIALOG_H
#define CREATECHANNELDIALOG_H

#include <QDialog>
#include "node.h"

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
    void setNodesList(const std::vector<Node*>& nodes);
    void presetNodes(Node *a, Node *b);
    Node* getNodeA() const;
    Node* getNodeB() const;
    int getWeight() const;
private slots:
    void on_Weight_Random_toggled(bool checked);

    void on_Weight_Manually_toggled(bool checked);

private:
    Ui::CreateChannelDialog *ui;
};

#endif // CREATECHANNELDIALOG_H
