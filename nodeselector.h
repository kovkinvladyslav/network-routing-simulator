#ifndef NODESELECTOR_H
#define NODESELECTOR_H

#include <QObject>
#include <QGraphicsScene>
#include "node.h"

class NodeSelector : public QObject
{
    Q_OBJECT
public:
    explicit NodeSelector(QGraphicsScene* scene);

signals:
    void twoNodesSelected(Node* first, Node* second);
    void oneNodeSelected(Node* node);

private slots:
    void onSelectionChanged();

private:
    QGraphicsScene* scene;
    Node* last1 = nullptr;
    Node* last2 = nullptr;

};

#endif // NODESELECTOR_H
