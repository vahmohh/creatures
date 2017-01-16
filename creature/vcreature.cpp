#include "vcreature.h"

#include <QDebug>

#include "segment/vsegment.h"
#include "joint/vjoint.h"

VCreature::VCreature()
{
    _initizlize_neural_network = true;
}

VCreature::VCreature(uint id, float strength, float speed, btDiscreteDynamicsWorld *discreteDynamicsWorld, const QVector3D &position)
{
    _initizlize_neural_network = true;

    setID(id);
    setStrength(strength);
    setSpeed(speed);
    setDiscreteDynamicsWorld(discreteDynamicsWorld);

    initialize(position);
}

VCreature::VCreature(uint id, float strength, float speed, btDiscreteDynamicsWorld *discreteDynamicsWorld, const QVector3D &position, const VNeuralNetwork &neuralNetwork)
{
    _initizlize_neural_network = false;

    setID(id);
    setStrength(strength);
    setSpeed(speed);
    setDiscreteDynamicsWorld(discreteDynamicsWorld);
    setNeuralNetwork(neuralNetwork);

    initialize(position);
}

VCreature::~VCreature()
{
    foreach (VJoint *tmp_joint, _joint_list) {
        delete tmp_joint;
    }

    foreach (VSegment *tmp_segment, _segment_list) {
        delete tmp_segment;
    }
}

void VCreature::setID(uint id)
{
    _id = id;
}

void VCreature::setStrength(float strength)
{
    _strength = strength;
}

void VCreature::setSpeed(float speed)
{
    _speed = speed;
}

uint VCreature::id() const
{
    return _id;
}

float VCreature::strength() const
{
    return _strength;
}

float VCreature::speed() const
{
    return _speed;
}

void VCreature::setNeuralNetwork(const VNeuralNetwork &neuralNetwork)
{
    _neural_network = neuralNetwork;
    _neural_network.reset();
}

void VCreature::setDiscreteDynamicsWorld(btDiscreteDynamicsWorld *discreteDynamicsWorld)
{
    _discrete_dynamics_world = discreteDynamicsWorld;
}

QList<VSegment *> VCreature::getSegmentList()
{
    return _segment_list;
}

VNeuralNetwork VCreature::getNeuralNetwork()
{
    return _neural_network;
}

float VCreature::getFitness()
{
    return _segment_list.at(0)->getX();
}

void VCreature::initialize(const QVector3D &position)
{
    _segment_list.append(new VSegment(1.0f, 0.5f, QVector3D(1.0f, 1.0f, 1.0f), QVector3D(1.0f, 1.0f, .0f), _discrete_dynamics_world, position));
    _segment_list.append(new VSegment(0.1f, 50.0f, QVector3D(0.5f, 0.2f, 0.2f), QVector3D(1.0f, 1.0f, 0.0f), _discrete_dynamics_world, QVector3D(position.x() + 0.75f, position.y(), position.z() + 0.5f)));
    _segment_list.append(new VSegment(0.1f, 50.0f, QVector3D(0.5f, 0.2f, 0.2f), QVector3D(1.0f, 1.0f, 0.0f), _discrete_dynamics_world, QVector3D(position.x() + 0.75f, position.y(), position.z() - 0.5f)));
    _segment_list.append(new VSegment(0.1f, 50.0f, QVector3D(0.5f, 0.2f, 0.2f), QVector3D(1.0f, 1.0f, 0.0f), _discrete_dynamics_world, QVector3D(position.x() - 0.75f, position.y(), position.z() + 0.5f)));
    _segment_list.append(new VSegment(0.1f, 50.0f, QVector3D(0.5f, 0.2f, 0.2f), QVector3D(1.0f, 1.0f, 0.0f), _discrete_dynamics_world, QVector3D(position.x() - 0.75f, position.y(), position.z() - 0.5f)));
    _segment_list.append(new VSegment(0.1f, 50.0f, QVector3D(1.5f, 0.2f, 0.2f), QVector3D(1.0f, 1.0f, 0.0f), _discrete_dynamics_world, QVector3D(position.x() + 1.75f, position.y(), position.z() + 0.5f)));
    _segment_list.append(new VSegment(0.1f, 50.0f, QVector3D(1.5f, 0.2f, 0.2f), QVector3D(1.0f, 1.0f, 0.0f), _discrete_dynamics_world, QVector3D(position.x() + 1.75f, position.y(), position.z() - 0.5f)));
    _segment_list.append(new VSegment(0.1f, 50.0f, QVector3D(1.5f, 0.2f, 0.2f), QVector3D(1.0f, 1.0f, 0.0f), _discrete_dynamics_world, QVector3D(position.x() - 1.75f, position.y(), position.z() + 0.5f)));
    _segment_list.append(new VSegment(0.1f, 50.0f, QVector3D(1.5f, 0.2f, 0.2f), QVector3D(1.0f, 1.0f, 0.0f), _discrete_dynamics_world, QVector3D(position.x() - 1.75f, position.y(), position.z() - 0.5f)));

    _joint_list.append(new VJoint(_segment_list.at(0)->getRigidBody(), _segment_list.at(1)->getRigidBody(), btVector3(0.5f, 0.5f, 0.5f), btVector3(-0.25f, 0.0f, 0.0f), btVector3(0.0f, 0.0f, 1.0f), -M_PI_4, 0.0f, _discrete_dynamics_world));
    _joint_list.append(new VJoint(_segment_list.at(0)->getRigidBody(), _segment_list.at(2)->getRigidBody(), btVector3(0.5f, 0.5f, -0.5f), btVector3(-0.25f, 0.0f, 0.0f), btVector3(0.0f, 0.0f, 1.0f), -M_PI_4, 0.0f, _discrete_dynamics_world));
    _joint_list.append(new VJoint(_segment_list.at(0)->getRigidBody(), _segment_list.at(3)->getRigidBody(), btVector3(-0.5f, 0.5f, 0.5f), btVector3(0.25f, 0.0f, 0.0f), btVector3(0.0f, 0.0f, 1.0f), 0.0f, M_PI_4, _discrete_dynamics_world));
    _joint_list.append(new VJoint(_segment_list.at(0)->getRigidBody(), _segment_list.at(4)->getRigidBody(), btVector3(-0.5f, 0.5f, -0.5f), btVector3(0.25f, 0.0f, 0.0f), btVector3(0.0f, 0.0f, 1.0f), 0.0f, M_PI_4, _discrete_dynamics_world));
    _joint_list.append(new VJoint(_segment_list.at(1)->getRigidBody(), _segment_list.at(5)->getRigidBody(), btVector3(0.25f, 0.0f, 0.0f), btVector3(-0.75f, 0.0f, 0.0f), btVector3(0.0f, 0.0f, 1.0f), 0.0f, M_PI_2, _discrete_dynamics_world));
    _joint_list.append(new VJoint(_segment_list.at(2)->getRigidBody(), _segment_list.at(6)->getRigidBody(), btVector3(0.25f, 0.0f, 0.0f), btVector3(-0.75f, 0.0f, 0.0f), btVector3(0.0f, 0.0f, 1.0f), 0.0f, M_PI_2, _discrete_dynamics_world));
    _joint_list.append(new VJoint(_segment_list.at(3)->getRigidBody(), _segment_list.at(7)->getRigidBody(), btVector3(-0.25f, 0.0f, 0.0f), btVector3(0.75f, 0.0f, 0.0f), btVector3(0.0f, 0.0f, 1.0f), -M_PI_2, 0.0f, _discrete_dynamics_world));
    _joint_list.append(new VJoint(_segment_list.at(4)->getRigidBody(), _segment_list.at(8)->getRigidBody(), btVector3(-0.25f, 0.0f, 0.0f), btVector3(0.75f, 0.0f, 0.0f), btVector3(0.0f, 0.0f, 1.0f), -M_PI_2, 0.0f, _discrete_dynamics_world));

    if (_initizlize_neural_network) {
        _neural_network.setOutputNumber(_joint_list.count() * 2);
        _neural_network.setHiddenLayerNumber(1);
        _neural_network.setHiddenLayerNeuronNumber((_joint_list.count() * 2) + 1);
        _neural_network.initialize();
    }
}

void VCreature::initializeNeuralNetwork()
{
    _neural_network.initialize();
}

void VCreature::resetNeuralNetwork()
{
    _neural_network.reset();
}

void VCreature::update()
{
    foreach (VSegment *tmp_segment, _segment_list) {
        tmp_segment->update();
    }
}

void VCreature::move()
{
    QList<float> output_list = _neural_network.run();

    for (int i = 0; i < _joint_list.count(); ++i) {
        _joint_list[i]->angularMotor(speed() * output_list.at(i * 2), strength() * ((output_list.at(i * 2) + 1.0f) * 0.5f));
    }
}

void VCreature::reset()
{
    for (int i = 0; i < _joint_list.count(); ++i) {
        _joint_list[i]->_hinge_constraint->enableMotor(false);
    }

    foreach (VSegment *segment, _segment_list) {
        segment->reset();
    }

    _joint_list.at(0)->setRigidBodyA(_segment_list.at(0)->getRigidBody());
    _joint_list.at(0)->setRigidBodyB(_segment_list.at(1)->getRigidBody());
    _joint_list.at(1)->setRigidBodyA(_segment_list.at(0)->getRigidBody());
    _joint_list.at(1)->setRigidBodyB(_segment_list.at(2)->getRigidBody());
    _joint_list.at(2)->setRigidBodyA(_segment_list.at(0)->getRigidBody());
    _joint_list.at(2)->setRigidBodyB(_segment_list.at(3)->getRigidBody());
    _joint_list.at(3)->setRigidBodyA(_segment_list.at(0)->getRigidBody());
    _joint_list.at(3)->setRigidBodyB(_segment_list.at(4)->getRigidBody());
    _joint_list.at(4)->setRigidBodyA(_segment_list.at(1)->getRigidBody());
    _joint_list.at(4)->setRigidBodyB(_segment_list.at(5)->getRigidBody());
    _joint_list.at(5)->setRigidBodyA(_segment_list.at(2)->getRigidBody());
    _joint_list.at(5)->setRigidBodyB(_segment_list.at(6)->getRigidBody());
    _joint_list.at(6)->setRigidBodyA(_segment_list.at(3)->getRigidBody());
    _joint_list.at(6)->setRigidBodyB(_segment_list.at(7)->getRigidBody());
    _joint_list.at(7)->setRigidBodyA(_segment_list.at(4)->getRigidBody());
    _joint_list.at(7)->setRigidBodyB(_segment_list.at(8)->getRigidBody());

    foreach (VJoint *joint, _joint_list) {
        joint->reset();
    }
}
