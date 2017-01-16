#ifndef VWORLD_H
#define VWORLD_H

#include <QObject>
#include <QHash>

#include "opengl/vopenglfragment.h"

class btDiscreteDynamicsWorld;

class VCreature;
class VNeuralNetwork;

class VWorld : public QObject
{
    Q_OBJECT

public:
    VWorld(QObject *parent = 0);
    ~VWorld();

    void setNeuralNetworkNumber(int number);
    void setNeuralNetworkNumber2(int number);

    int neuralNetworkNumber();
    int neuralNetworkNumber2();

    void initialize(bool mode);
    void saveNeuralNetwork();

protected:
    void timerEvent(QTimerEvent *);

private:
    btDiscreteDynamicsWorld *_discrete_dynamics_world;
    QHash<uint, VOpenGLFragment> _opengl_fragment_hash;
    QHash<uint, VCreature *> _creature_hash;
    uint _current_time, _generation;
    int _neural_network_number, _neural_network_number_2;
    bool _save_neural_network, _mode;

private slots:
    void startTimerSlot();

signals:
    void startTimerSignal();

    void updatedSignal(const QHash<uint, VOpenGLFragment> &openglFragmentHash);
    void updatedNeuralNetworkSignal(const VNeuralNetwork &neuralNetwork, const VNeuralNetwork &neuralNetwork2);
};

#endif // VWORLD_H
