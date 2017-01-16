#include "vneuron.h"

#include <QDataStream>
#include <QDebug>

VNeuron::VNeuron()
{
    setValue(0.0f);
}

VNeuron::VNeuron(float value)
{
    setValue(value);
}

VNeuron::VNeuron(float value, QMap<uint, float> connection_map)
{
    _value = value;
    _connection_map = connection_map;
}

void VNeuron::setValue(float value)
{
    _value = value;
}

float VNeuron::value() const
{
    return _value;
}

QMap<uint, float> VNeuron::getConnectionMap()
{
    return _connection_map;
}

void VNeuron::addConnection(uint neuronNumber, float weight)
{
    _connection_map.insert(neuronNumber, weight);
}

void VNeuron::alterConnection(uint neuronNumber, float weight)
{
    _connection_map[neuronNumber] = weight;
}

void VNeuron::removeConnection(uint neuronNumber)
{
    if (_connection_map.contains(neuronNumber)) {
        _connection_map.remove(neuronNumber);
    }
}

float VNeuron::getConnection(uint neuronNumber)
{
    return _connection_map.value(neuronNumber);
}

bool VNeuron::existConnection(uint neuronNumber)
{
    return _connection_map.contains(neuronNumber);
}

void VNeuron::update(const QMap<uint, float> &valueMap)
{
    _value = 0;

    foreach (uint tmp_key, _connection_map.keys()) {
        _value += _connection_map.value(tmp_key) * valueMap.value(tmp_key);
    }

    _value = tanh(_value);
}

QDataStream &operator<<(QDataStream &out, VNeuron &neuron)
{
    out
            << neuron.value()
            << neuron.getConnectionMap();

    return out;
}

QDataStream &operator>>(QDataStream &in, VNeuron &neuron)
{
    float value;
    QMap<uint, float> connection_map;

    in
            >> value
            >> connection_map;

    neuron = VNeuron(value, connection_map);

    return in;
}
