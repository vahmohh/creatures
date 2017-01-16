#include "vneuralnetwork.h"

#include <QDataStream>
#include <QDebug>

#include "utility/vutility.h"

VNeuralNetwork::VNeuralNetwork()
{

}

VNeuralNetwork::VNeuralNetwork(int outputNumber, int hiddenLayerNumber, int hiddenLayerNeuronNumber, QList<VLayer> layerList)
{
    _output_number = outputNumber;
    _hidden_layer_number = hiddenLayerNumber;
    _hidden_layer_neuron_number = hiddenLayerNeuronNumber;
    _layer_list = layerList;
}

void VNeuralNetwork::setOutputNumber(int outputNumber)
{
    _output_number = outputNumber;
}

void VNeuralNetwork::setHiddenLayerNumber(int hiddenLayerNumber)
{
    _hidden_layer_number = hiddenLayerNumber;
}

void VNeuralNetwork::setHiddenLayerNeuronNumber(int hiddenLayerNeuronNumber)
{
    _hidden_layer_neuron_number = hiddenLayerNeuronNumber;
}

void VNeuralNetwork::setLayerList(QList<VLayer> layerList)
{
    _layer_list = layerList;
}

int VNeuralNetwork::outputNumber() const
{
    return _output_number;
}

int VNeuralNetwork::hiddenLayerNumber() const
{
    return _hidden_layer_number;
}

int VNeuralNetwork::hiddenLayerNeuronNumber() const
{
    return _hidden_layer_neuron_number;
}

QList<VLayer> *VNeuralNetwork::getLayerList()
{
    return &_layer_list;
}

VLayer *VNeuralNetwork::getLayer(int number)
{
    return &_layer_list[number];
}

void VNeuralNetwork::initialize()
{
    _layer_list.clear();
    _layer_list.append(VLayer(outputNumber(), 1.0f));

    for (int i = 0; i < hiddenLayerNumber(); ++i) {
        _layer_list.append(VLayer(hiddenLayerNeuronNumber(), 0.0f));
    }

    _layer_list.append(VLayer(outputNumber(), 0.0f));

    for (int i = 1; i < _layer_list.count(); ++i) {
        for (int j = 0; j < _layer_list.at(i - 1).neuronNumber(); ++j) {
            for (int k = 0; k < _layer_list.at(i).neuronNumber(); ++k) {
                if (VUtility::random(0.0f, 1.0f) > 0.7f) {
                    _layer_list[i].addConnection(j, k, VUtility::random(-1.0f, 1.0f));
                }
            }
        }
    }
}

void VNeuralNetwork::reset()
{
    QList<float> neuron_value_list;

    for (int i = 0; i < outputNumber(); ++i) {
        neuron_value_list.append(1.0f);
    }

    _layer_list[0].setNeuronValueList(neuron_value_list);
}

QList<float> VNeuralNetwork::run()
{
    QList<float> output;

    for (int i = 1; i < _layer_list.count(); ++i) {
        _layer_list[i].update(_layer_list[i - 1].getValueMap());
    }

    foreach (float tmp_value, _layer_list.last().getValueMap()) {
        output.append(tmp_value);
    }

    _layer_list[0].setNeuronValueList(output);

    return output;
}

void VNeuralNetwork::mutation(float rate)
{
    // Mutation Connection

    for (int i = 1; i < _layer_list.count(); ++i) {
        for (int j = 0; j < _layer_list.at(i - 1).neuronNumber(); ++j) {
            for (int k = 0; k < _layer_list.at(i).neuronNumber(); ++k) {
                if (VUtility::random(0.0f, 1.0f) < (float) rate / _layer_list.at(i).neuronNumber()) {
                    if (_layer_list[i].existConnection(j, k) == true) {
                        _layer_list[i].removeConnection(j, k);
                    } else {
                        _layer_list[i].addConnection(j, k, VUtility::random(-1.0f, 1.0f));
                    }
                }
            }
        }
    }

    // Mutation Weight Value

    for (int i = 1; i < _layer_list.count(); ++i) {
        for (int j = 0; j < _layer_list.at(i - 1).neuronNumber(); ++j) {
            for (int k = 0; k < _layer_list.at(i).neuronNumber(); ++k) {
                if (VUtility::random(0.0f, 1.0f) < rate) {
                    if (_layer_list[i].existConnection(j, k) == true) {
                        _layer_list[i].alterConnection(j, k, VUtility::random(-1.0f, 1.0f));
                    }
                }
            }
        }
    }
}

QList<VNeuralNetwork> VNeuralNetwork::crossover(const VNeuralNetwork &neuralNetworkA, const VNeuralNetwork &neuralNetworkB)
{
    QList<VNeuralNetwork> neural_network_list;

    VNeuralNetwork neural_network_a, neural_network_b;

    neural_network_a = neuralNetworkA;
    neural_network_b = neuralNetworkB;

    for (int i = 1; i < neural_network_a.getLayerList()->count(); ++i) {
        for (int j = 0; j <  neural_network_a.getLayerList()->at(i - 1).neuronNumber(); ++j) {
            for (int k = 0; k < neural_network_a.getLayerList()->at(i).neuronNumber() / 2; ++k) {

                neural_network_a.getLayer(i)->removeConnection(j, k);

                if (neural_network_b.getLayer(i)->existConnection(j, k) == true) {
                    neural_network_a.getLayer(i)->addConnection(j, k, neural_network_b.getLayer(i)->getConnection(j, k));
                }

                //if (neural_network_a.getLayer(i)->existConnection(j, k) == false && neural_network_b.getLayer(i)->existConnection(j, k) == true) {
                //    neural_network_a.getLayer(i)->addConnection(j, k, neural_network_b.getLayer(i)->getConnection(j, k));
                //}
            }
        }
    }

    for (int i = 1; i < neural_network_b.getLayerList()->count(); ++i) {
        for (int j = 0; j <  neural_network_b.getLayerList()->at(i - 1).neuronNumber(); ++j) {
            for (int k = 0; k < neural_network_b.getLayerList()->at(i).neuronNumber() / 2; ++k) {

                neural_network_b.getLayer(i)->removeConnection(j, k);

                if (neural_network_a.getLayer(i)->existConnection(j, k) == true) {
                    neural_network_b.getLayer(i)->addConnection(j, k, neural_network_a.getLayer(i)->getConnection(j, k));
                }

                //if (neural_network_b.getLayer(i)->existConnection(j, k) == false && neural_network_a.getLayer(i)->existConnection(j, k) == true) {
                //    neural_network_b.getLayer(i)->addConnection(j, k, neural_network_a.getLayer(i)->getConnection(j, k));
                //}
            }
        }
    }

    neural_network_a.mutation(0.01f);
    neural_network_b.mutation(0.01f);

    neural_network_list.append(neural_network_a);
    neural_network_list.append(neural_network_b);

    return neural_network_list;
}

VNeuralNetwork VNeuralNetwork::clone(VNeuralNetwork &neuralNetwork)
{
    VNeuralNetwork new_neural_network = VNeuralNetwork(neuralNetwork.outputNumber(), neuralNetwork.hiddenLayerNumber(), neuralNetwork.hiddenLayerNeuronNumber(), *neuralNetwork.getLayerList());;

    return new_neural_network;
}

QDataStream &operator<<(QDataStream &out, VNeuralNetwork &neuralNetwork)
{
    out
            << neuralNetwork.outputNumber()
            << neuralNetwork.hiddenLayerNumber()
            << neuralNetwork.hiddenLayerNeuronNumber();

    QList<VLayer> *layer_list = neuralNetwork.getLayerList();

    for (int i = 0; i < layer_list->count(); ++i) {
        VLayer layer = layer_list->at(i);
        out << layer;
    }

    return out;
}

QDataStream &operator>>(QDataStream &in, VNeuralNetwork &neuralNetwork)
{
    int output_number, hidden_layer_number, hidden_layer_neuron_number;
    QList<VLayer> layer_list;

    in
            >> output_number
            >> hidden_layer_number
            >> hidden_layer_neuron_number;

    for (int i = 0; i < hidden_layer_number + 2; ++i) {
        VLayer layer;
        in >> layer;
        layer_list.append(layer);
    }

    neuralNetwork = VNeuralNetwork(output_number, hidden_layer_number, hidden_layer_neuron_number, layer_list);

    return in;
}
