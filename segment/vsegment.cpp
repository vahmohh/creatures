#include "vsegment.h"

VSegment::VSegment()
{

}

VSegment::VSegment(float mass, float friction, const QVector3D &size, const QVector3D &color, btDiscreteDynamicsWorld *discreteDynamicsWorld, const QVector3D &position) : VSegment()
{
    setMass(mass);
    setFriction(friction);
    setSize(size);
    setColor(color);
    setDiscreteDynamicsWorld(discreteDynamicsWorld);

    initialize(position);
}

VSegment::~VSegment()
{
    _discrete_dynamics_world->removeRigidBody(_rigid_body);

    delete _rigid_body->getMotionState();
    delete _rigid_body->getCollisionShape();
    delete _rigid_body;
}

void VSegment::setMass(float mass)
{
    _mass = mass;
}

void VSegment::setFriction(float friction)
{
    _friction = friction;
}

void VSegment::setSize(const QVector3D &size)
{
    _size = size;
}

void VSegment::setColor(const QVector3D &color)
{
    _color = color;
}

float VSegment::mass() const
{
    return _mass;
}

float VSegment::friction() const
{
    return _friction;
}

QVector3D VSegment::size() const
{
    return _size;
}

QVector3D VSegment::color() const
{
    return _color;
}

void VSegment::setDiscreteDynamicsWorld(btDiscreteDynamicsWorld *discreteDynamicsWorld)
{
    _discrete_dynamics_world = discreteDynamicsWorld;
}

float *VSegment::getMatrix()
{
    return _matrix;
}

btRigidBody *VSegment::getRigidBody()
{
    return _rigid_body;
}

float VSegment::getX()
{
    return _transform.getOrigin().x();
}

float VSegment::getZ()
{
    return _transform.getOrigin().z();
}

void VSegment::initialize(const QVector3D &position)
{
    btVector3 local_interia;

    _initialize_position = position;

    _collision_shape = new btBoxShape(btVector3(size().x() / 2.0f, size().y() / 2.0f, size().z() / 2.0f));
    _collision_shape->calculateLocalInertia(mass(), local_interia);

    _rigid_body = new btRigidBody(btRigidBody::btRigidBodyConstructionInfo(mass(), new btDefaultMotionState(btTransform(btQuaternion(0.0f, 0.0f, 0.0f, 1.0f), btVector3(_initialize_position.x(), _initialize_position.y(), _initialize_position.z()))), _collision_shape, local_interia));
    _rigid_body->setActivationState(DISABLE_DEACTIVATION);
    _rigid_body->setFriction(friction());

    _discrete_dynamics_world->addRigidBody(_rigid_body);
}

void VSegment::update()
{
    _rigid_body->getMotionState()->getWorldTransform(_transform);
    _transform.getOpenGLMatrix(_matrix);
}

void VSegment::reset()
{
    _discrete_dynamics_world->removeRigidBody(_rigid_body);

    delete _rigid_body->getMotionState();
    delete _rigid_body->getCollisionShape();
    delete _rigid_body;

    initialize(_initialize_position);
}
