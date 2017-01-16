#include "vopenglvertex.h"

VOpenGLVertex::VOpenGLVertex()
{

}

VOpenGLVertex::VOpenGLVertex(const QVector3D &position, const QVector3D &normal, const QVector2D &textureCoordinate)
{
    setPosition(position);
    setNormal(normal);
    setTextureCoordinate(textureCoordinate);
}

void VOpenGLVertex::setPosition(const QVector3D &position)
{
    _position = position;
}

void VOpenGLVertex::setNormal(const QVector3D &normal)
{
    _normal = normal;
}

void VOpenGLVertex::setTextureCoordinate(const QVector2D &textureCoordinate)
{
    _texture_coordinate = textureCoordinate;
}

QVector3D VOpenGLVertex::position() const
{
    return _position;
}

QVector3D VOpenGLVertex::normal() const
{
    return _normal;
}

QVector2D VOpenGLVertex::textureCoordinate() const
{
    return _texture_coordinate;
}
