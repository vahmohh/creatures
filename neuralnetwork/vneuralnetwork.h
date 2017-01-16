#ifndef VNEURALNETWORK_H
#define VNEURALNETWORK_H

#include "layer/vlayer.h"

class VNeuralNetwork
{
public:
    VNeuralNetwork();
    VNeuralNetwork(int outputNumber, int hiddenLayerNumber, int hiddenLayerNeuronNumber, QList<VLayer> layerList);

    void setOutputNumber(int outputNumber);
    void setHiddenLayerNumber(int hiddenLayerNumber);
    void setHiddenLayerNeuronNumber(int hiddenLayerNeuronNumber);
    void setLayerList(QList<VLayer> layerList);

    int outputNumber() const;
    int hiddenLayerNumber() const;
    int hiddenLayerNeuronNumber() const;

    QList<VLayer> *getLayerList();
    VLayer *getLayer(int number);

    void initialize();
    void reset();
    QList<float> run();
    void mutation(float rate);

    static QList<VNeuralNetwork> crossover(const VNeuralNetwork &neuralNetworkA, const VNeuralNetwork &neuralNetworkB);
    static VNeuralNetwork clone(VNeuralNetwork &neuralNetwork);

private:
    int _output_number, _hidden_layer_number, _hidden_layer_neuron_number;
    QList<VLayer> _layer_list;
};

QDataStream &operator<<(QDataStream &out, VNeuralNetwork &neuralNetwork);
QDataStream &operator>>(QDataStream &in, VNeuralNetwork &neuralNetwork);

#endif // VNEURALNETWORK_H
