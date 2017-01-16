#ifndef VCREATURE_H
#define VCREATURE_H

#include <QList>

#include "neuralnetwork/vneuralnetwork.h"

class QVector3D;

class btDiscreteDynamicsWorld;

class VSegment;
class VJoint;

class VCreature
{
public:
    VCreature();
    VCreature(uint id, float strength, float speed, btDiscreteDynamicsWorld *discreteDynamicsWorld, const QVector3D &position);
    VCreature(uint id, float strength, float speed, btDiscreteDynamicsWorld *discreteDynamicsWorld, const QVector3D &position, const VNeuralNetwork &neuralNetwork);

    ~VCreature();

    void setID(uint id);
    void setStrength(float strength);
    void setSpeed(float speed);

    uint id() const;
    float strength() const;
    float speed() const;

    void setNeuralNetwork(const VNeuralNetwork &neuralNetwork);
    void setDiscreteDynamicsWorld(btDiscreteDynamicsWorld *discreteDynamicsWorld);

    QList<VSegment *> getSegmentList();
    VNeuralNetwork getNeuralNetwork();
    float getFitness();

    void initialize(const QVector3D &position);
    void initializeNeuralNetwork();
    void resetNeuralNetwork();
    void update();
    void move();
    void reset();

private:
    uint _id;
    float _strength, _speed;
    btDiscreteDynamicsWorld *_discrete_dynamics_world;
    QList <VSegment *> _segment_list;
    QList<VJoint *> _joint_list;
    VNeuralNetwork _neural_network;
    bool _initizlize_neural_network;
};

#endif // VCREATURE_H
