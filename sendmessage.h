#ifndef SENDMESSAGE_H
#define SENDMESSAGE_H

#include <QDialog>
#include <vector>
#include "node.h"

namespace Ui {
class SendMessage;
}

class SendMessage : public QDialog
{
    Q_OBJECT

public:
    explicit SendMessage(const std::vector<Node*>& nodes, QWidget *parent = nullptr);
    ~SendMessage();

    Node* getSource() const;
    Node* getDestination() const;
    int getMessageSizeBytes() const;
    int getPacketSizeBytes() const;
    bool isDatagramMode() const;
    void accept();

private:
    Ui::SendMessage *ui;
    std::vector<Node*> availableNodes;
};

#endif // SENDMESSAGE_H
