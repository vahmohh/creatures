#ifndef VNEURALNETWORKWINDOW_H
#define VNEURALNETWORKWINDOW_H

#include <QWidget>
#include <QComboBox>

#include "neuralnetwork/vneuralnetwork.h"

class VNeuralNetworkWindow : public QWidget
{
    Q_OBJECT

public:
    VNeuralNetworkWindow(QWidget *parent = 0);

    int getNumber() const;
    int getNumber2() const;

    void updateNeuralNetwork(const VNeuralNetwork &neuralNetwork, const VNeuralNetwork &neuralNetwork2);

protected:
    void showEvent(QShowEvent *);
    void keyPressEvent(QKeyEvent *key);
    void paintEvent(QPaintEvent *);

private:
    VNeuralNetwork _neural_network, _neural_network_2;
    QComboBox _combo_box, _combo_box_2;

signals:
    void textChangedSignal();
    void keyPressedSignal(int key);
};

#endif // VNEURALNETWORKWINDOW_H
