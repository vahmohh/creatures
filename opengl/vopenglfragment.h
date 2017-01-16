#ifndef VOPENGLFRAGMENT_H
#define VOPENGLFRAGMENT_H

#include <QVector3D>

#include <GL/glew.h>

class VOpenGLFragment
{
public:
    enum Type {
        Ground,
        Segment
    };

    VOpenGLFragment();
    VOpenGLFragment(Type type, const QVector3D &size, const QVector3D &color, GLfloat *matrix);

    void setType(Type type);
    void setSize(const QVector3D &size);
    void setColor(const QVector3D &color);
    void setMatrix(GLfloat *matrix);

    Type type() const;
    QVector3D size() const;
    QVector3D color() const;
    GLfloat *matrix() const;

    GLuint getVertexID();
    GLuint getTextureID();
    GLsizei getDrawSize();

    void initialize();

private:
    Type _type;
    QVector3D _size, _color;
    GLfloat *_matrix;
    GLuint _vertex_id, _texture_id;
    GLsizei _draw_size;
};

#endif // VOPENGLFRAGMENT_H
