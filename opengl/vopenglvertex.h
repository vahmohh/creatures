#ifndef VOPENGLVERTEX_H
#define VOPENGLVERTEX_H

#include <QVector3D>
#include <QVector2D>

class VOpenGLVertex
{
public:
    VOpenGLVertex();
    VOpenGLVertex(const QVector3D &position, const QVector3D &normal, const QVector2D &textureCoordinate);

    void setPosition(const QVector3D &position);
    void setNormal(const QVector3D &normal);
    void setTextureCoordinate(const QVector2D &textureCoordinate);

    QVector3D position() const;
    QVector3D normal() const;
    QVector2D textureCoordinate() const;

private:
    QVector3D _position, _normal;
    QVector2D _texture_coordinate;
};

#endif // VOPENGLVERTEX_H
