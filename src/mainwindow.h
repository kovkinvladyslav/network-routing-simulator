// mainwindow.h
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "graph.h"
#include "messagesimulator.h"
#include "nodeselector.h"
#include "edgecontroller.h"
#include "djkstrastepper.h"
#include "messagestats.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_actionZoom_In_triggered();
    void on_actionZoom_Out_triggered();
    void on_actionAddNode_triggered();
    void on_actionAddConnection_triggered();
    void on_actionDelete_Connection_triggered();
    void on_actionDelete_Node_triggered();
    void onNodeSelected(Node* node);
    void on_NodeON_toggled(bool checked);
    void on_NodeOFF_toggled(bool checked);
    void updateRouting();
    void on_actionArrange_Graph_triggered();

    void on_actionRouting_Next_triggered();

    void on_actionRouting_Start_triggered();

    void on_actionRouting_Reset_triggered();

    void on_actionLSR_Run_triggered();

    void on_actionLSR_Start_triggered();


    void on_actionLSR_Next_Step_triggered();

    void on_actionLSR_Reset_2_triggered();

    void on_actionSend_Message_triggered();

    void on_actionView_Logs_triggered();

    void on_actionClear_triggered();

    void on_actionTest_Same_Pck_different_Msg_Size_triggered();

    void on_actionDuplex_triggered();

    void on_actionHalf_Duplex_triggered();

    void on_actionRandom_triggered();

    void on_actionTest_dif_Pck_same_Msg_triggered();

    void on_actionConvert_to_duplex_triggered();

    void on_actionConvert_to_half_duplex_triggered();

    void on_actionConvert_to_random_triggered();

private:
    Ui::MainWindow *ui;
    Graph* graph;
    NodeSelector* selector;
    EdgeController* edgeController;
    Node* currentNode = nullptr;
    DijkstraStepper* stepper = nullptr;
    Node* routingSource = nullptr;
    RouteMetric currentMetric = RouteMetric::EffectiveCost;
    void updateLSRControls();
    void updateInspector(Node* node);
    void resetInspector();
    void removeEdgeBetween(Node* a, Node* b);
    void removeAllEdges(Node* node);
    QVector<Node*> selectedNodes() const;
    void updateTopologyDB(Node* r);
    void updateHighlight();
    std::vector<MessageLogEntry> messageLog;
    void updateDijkstraState(bool isInitial);
    DijkstraStep currentStep;
    void on_actionRouting_AutoRun_triggered();
    void logResult(Node* src, Node* dst,
                               int msgSize, int pktSize, bool datagram, const MessageSimulationResult& res);
    void generate_topology(ChannelType type = ChannelType::Duplex, bool is_random = false);
};

#endif // MAINWINDOW_H
