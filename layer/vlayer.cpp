#include "vlayer.h"

#include <QDataStream>

#include "utility/vutility.h"

VLayer::VLayer()
{

}

VLayer::VLayer(uint neuronNumber, float value)
{
    setNeuronNumber(neuronNumber);

    initialize(value);
}

VLayer::VLayer(int neuron_number, QList<VNeuron> neuron_list)
{
    _neuron_number = neuron_number;
    _neuron_list = neuron_list;
}

void VLayer::setNeuronNumber(int neuronNumber)
{
    _neuron_number = neuronNumber;
}

void VLayer::setNeuronValueList(QList<float> neuronValueList)
{
    for (int i = 0; i < neuronNumber(); ++i) {
        _neuron_list[i].setValue(neuronValueList.at(i));
    }
}

int VLayer::neuronNumber() const
{
    return _neuron_number;
}

QList<VNeuron> *VLayer::getNeuronList()
{
    return &_neuron_list;
}

QMap<uint, float> VLayer::getValueMap()
{
    QMap<uint, float> value_map;

    for (int i = 0; i < _neuron_list.count(); ++i) {
        value_map.insert(i, _neuron_list[i].value());
    }

    return value_map;
}

void VLayer::initialize(float value)
{
    for (int i = 0; i < neuronNumber(); ++i) {
        _neuron_list.insert(i, VNeuron(value));
    }
}

void VLayer::addConnection(uint sourceNeuronNumber, uint destinationNeuronNumber, float weight)
{
    _neuron_list[destinationNeuronNumber].addConnection(sourceNeuronNumber, weight);
}

void VLayer::alterConnection(uint sourceNeuronNumber, uint destinationNeuronNumber, float weight)
{
    _neuron_list[destinationNeuronNumber].alterConnection(sourceNeuronNumber, weight);
}

void VLayer::removeConnection(uint sourceNeuronNumber, uint destinationNeuronNumber)
{
    _neuron_list[destinationNeuronNumber].removeConnection(sourceNeuronNumber);
}

float VLayer::getConnection(uint sourceNeuronNumber, uint destinationNeuronNumber)
{
    return _neuron_list[destinationNeuronNumber].getConnection(sourceNeuronNumber);
}

bool VLayer::existConnection(uint sourceNeuronNumber, uint destinationNeuronNumber)
{
    return _neuron_list[destinationNeuronNumber].existConnection(sourceNeuronNumber);
}

void VLayer::update(const QMap<uint, float> &valueMap)
{
    for (int i = 0; i < _neuron_list.count(); ++i) {
        _neuron_list[i].update(valueMap);
    }
}

QDataStream &operator<<(QDataStream &out, VLayer &layer)
{
    out
            << layer.neuronNumber();

    QList<VNeuron> *neuron_list = layer.getNeuronList();

    for (int i = 0; i < neuron_list->count(); ++i) {
        VNeuron neuron = neuron_list->at(i);
        out << neuron;
    }

    return out;
}

QDataStream &operator>>(QDataStream &in, VLayer &layer)
{
    int neuron_number;
    QList<VNeuron> neuron_list;

    in
            >> neuron_number;

    for (int i = 0; i < neuron_number; ++i) {
        VNeuron neuron;
        in >> neuron;
        neuron_list.append(neuron);
    }

    layer = VLayer(neuron_number, neuron_list);

    return in;
}
