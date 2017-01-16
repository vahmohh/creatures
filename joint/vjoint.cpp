#include "vjoint.h"

VJoint::VJoint()
{

}

VJoint::VJoint(btRigidBody *rigidBodyA, btRigidBody *rigidBodyB, const btVector3 &positionA, const btVector3 &positionB, const btVector3 &axis, btScalar low, btScalar hight, btDiscreteDynamicsWorld *discreteDynamicsWorld)
{
    setRigidBodyA(rigidBodyA);
    setRigidBodyB(rigidBodyB);
    setPositionA(positionA);
    setPositionB(positionB);
    setAxis(axis);
    setLow(low);
    setHight(hight);
    setDiscreteDynamicsWorld(discreteDynamicsWorld);

    initizlize();
}

VJoint::~VJoint()
{
    _discrete_dynamics_world->removeConstraint(_hinge_constraint);

    delete _hinge_constraint;
}

void VJoint::setRigidBodyA(btRigidBody *rigidBodyA)
{
    _rigid_body_a = rigidBodyA;
}

void VJoint::setRigidBodyB(btRigidBody *rigidBodyB)
{
    _rigid_body_b = rigidBodyB;
}

void VJoint::setPositionA(const btVector3 &positionA)
{
    _position_a = positionA;
}

void VJoint::setPositionB(const btVector3 &positionB)
{
    _position_b = positionB;
}

void VJoint::setAxis(const btVector3 &axis)
{
    _axis = axis;
}

void VJoint::setLow(btScalar low)
{
    _low = low;
}

void VJoint::setHight(btScalar hight)
{
    _hight = hight;
}

btRigidBody *VJoint::rigidBodyA() const
{
    return _rigid_body_a;
}

btRigidBody *VJoint::rigidBodyB() const
{
    return _rigid_body_b;
}

btVector3 VJoint::positionA() const
{
    return _position_a;
}

btVector3 VJoint::positionB() const
{
    return _position_b;
}

btVector3 VJoint::axis() const
{
    return _axis;
}

btScalar VJoint::low() const
{
    return _low;
}

btScalar VJoint::hight() const
{
    return _hight;
}

void VJoint::setDiscreteDynamicsWorld(btDiscreteDynamicsWorld *discreteDynamicsWorld)
{
    _discrete_dynamics_world = discreteDynamicsWorld;
}

void VJoint::initizlize()
{
    (_hinge_constraint = new btHingeConstraint(*rigidBodyA(), *rigidBodyB(), positionA(), positionB(), axis(), axis()))->setLimit(low(), hight());
    _discrete_dynamics_world->addConstraint(_hinge_constraint, true);
}

void VJoint::reset()
{
    _discrete_dynamics_world->removeConstraint(_hinge_constraint);

    delete _hinge_constraint;

    initizlize();
}

void VJoint::angularMotor(btScalar targetVelocity, btScalar maxMotorImpulse)
{
    _hinge_constraint->enableAngularMotor(true, targetVelocity, maxMotorImpulse);
}
