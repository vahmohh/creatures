#ifndef VNEURON_H
#define VNEURON_H

#include <QHash>
#include <QMap>

class VNeuron
{
public:
    VNeuron();
    VNeuron(float value);
    VNeuron(float value, QMap<uint, float> connection_map);

    void setValue(float value);

    float value() const;

    QMap<uint, float> getConnectionMap();

    void addConnection(uint neuronNumber, float weight);
    void alterConnection(uint neuronNumber, float weight);
    void removeConnection(uint neuronNumber);
    float getConnection(uint neuronNumber);
    bool existConnection(uint neuronNumber);

    void update(const QMap<uint, float> &valueMap);

private:
    float _value;
    QMap<uint, float> _connection_map;
};

QDataStream &operator<<(QDataStream &out, VNeuron &neuron);
QDataStream &operator>>(QDataStream &in, VNeuron &neuron);

#endif // VNEURON_H
