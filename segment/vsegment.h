#ifndef VSEGMENT_H
#define VSEGMENT_H

#include <QVector3D>

#include <bullet/btBulletDynamicsCommon.h>

class VSegment
{
public:
    VSegment();
    VSegment(float mass, float friction, const QVector3D &size, const QVector3D &color, btDiscreteDynamicsWorld *discreteDynamicsWorld, const QVector3D &position);
    ~VSegment();

    void setMass(float mass);
    void setFriction(float friction);
    void setSize(const QVector3D &size);
    void setColor(const QVector3D &color);

    float mass() const;
    float friction() const;
    QVector3D size() const;
    QVector3D color() const;

    void setDiscreteDynamicsWorld(btDiscreteDynamicsWorld *discreteDynamicsWorld);

    float *getMatrix();
    btRigidBody *getRigidBody();
    float getX();
    float getZ();

    void initialize(const QVector3D &position);
    void update();
    void reset();

private:
    float _mass, _friction, _matrix[16];
    QVector3D _size, _color, _initialize_position;
    btDiscreteDynamicsWorld *_discrete_dynamics_world;
    btRigidBody *_rigid_body;
    btCollisionShape *_collision_shape;
    btTransform _transform;
};

#endif // VSEGMENT_H
