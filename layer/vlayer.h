#ifndef VLAYER_H
#define VLAYER_H

#include <QList>

#include "neuron/vneuron.h"

class VLayer
{
public:
    VLayer();
    VLayer(uint neuronNumber, float value = 0.0f);
    VLayer(int neuron_number, QList<VNeuron> neuron_list);

    void setNeuronNumber(int neuronNumber);
    void setNeuronValueList(QList<float> neuronValueList);

    int neuronNumber() const;

    QList<VNeuron> *getNeuronList();
    QMap<uint, float> getValueMap();

    void initialize(float value);

    void addConnection(uint sourceNeuronNumber, uint destinationNeuronNumber, float weight);
    void alterConnection(uint sourceNeuronNumber, uint destinationNeuronNumber, float weight);
    void removeConnection(uint sourceNeuronNumber, uint destinationNeuronNumber);
    float getConnection(uint sourceNeuronNumber, uint destinationNeuronNumber);
    bool existConnection(uint sourceNeuronNumber, uint destinationNeuronNumber);

    void update(const QMap<uint, float> &valueMap);

private:
    int _neuron_number;
    QList<VNeuron> _neuron_list;
};

QDataStream &operator<<(QDataStream &out, VLayer &layer);
QDataStream &operator>>(QDataStream &in, VLayer &layer);

#endif // VLAYER_H
