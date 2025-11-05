#ifndef REMOVECHANNELDIALOG_H
#define REMOVECHANNELDIALOG_H

#include <QDialog>
#include "node.h"

namespace Ui {
class RemoveChannelDialog;
}

class RemoveChannelDialog : public QDialog
{
    Q_OBJECT
public:
    explicit RemoveChannelDialog(QWidget *parent = nullptr);
    void setNodesList(const std::vector<Node*>& nodes);
    void presetNodes(Node* a, Node* b);
    Node* getNodeA() const;
    Node* getNodeB() const;

protected:
    void accept() override;

private:
    Ui::RemoveChannelDialog *ui;
};

#endif // REMOVECHANNELDIALOG_H
