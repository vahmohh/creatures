#ifndef VJOINT_H
#define VJOINT_H

#include <bullet/btBulletDynamicsCommon.h>

class VJoint
{
public:
    VJoint();
    VJoint(btRigidBody *rigidBodyA, btRigidBody *rigidBodyB, const btVector3 &positionA, const btVector3 &positionB, const btVector3 &axis, btScalar low, btScalar hight, btDiscreteDynamicsWorld *discreteDynamicsWorld);
    ~VJoint();

    void setRigidBodyA(btRigidBody *rigidBodyA);
    void setRigidBodyB(btRigidBody *rigidBodyB);
    void setPositionA(const btVector3 &positionA);
    void setPositionB(const btVector3 &positionB);
    void setAxis(const btVector3 &axis);
    void setLow(btScalar low);
    void setHight(btScalar hight);

    btRigidBody *rigidBodyA() const;
    btRigidBody *rigidBodyB() const;
    btVector3 positionA() const;
    btVector3 positionB() const;
    btVector3 axis() const;
    btScalar low() const;
    btScalar hight() const;

    void setDiscreteDynamicsWorld(btDiscreteDynamicsWorld *discreteDynamicsWorld);

    void initizlize();
    void reset();
    void angularMotor(btScalar targetVelocity, btScalar maxMotorImpulse);

    btHingeConstraint *_hinge_constraint;

private:
    btRigidBody *_rigid_body_a, *_rigid_body_b;
    btVector3 _position_a, _position_b, _axis;
    btScalar _low, _hight;
    btDiscreteDynamicsWorld *_discrete_dynamics_world;
};

#endif // VJOINT_H
