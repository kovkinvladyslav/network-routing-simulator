#include "nodeselector.h"

NodeSelector::NodeSelector(QGraphicsScene *scene)
    : QObject(scene), scene(scene)
{
    connect(scene, &QGraphicsScene::selectionChanged,
            this, &NodeSelector::onSelectionChanged);
}

void NodeSelector::onSelectionChanged()
{
    const auto &items = scene->selectedItems();

    if (items.size() == 2)
        emit twoNodesSelected(qgraphicsitem_cast<Node*>(items[0]),
                              qgraphicsitem_cast<Node*>(items[1]));
    else if (items.size() == 1)
        emit oneNodeSelected(qgraphicsitem_cast<Node*>(items[0]));
}
