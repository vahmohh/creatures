#include "vopenglfragment.h"

#include <QImage>

#include "vopenglvertex.h"

VOpenGLFragment::VOpenGLFragment()
{

}

VOpenGLFragment::VOpenGLFragment(Type type, const QVector3D &size, const QVector3D &color, GLfloat *matrix)
{
    setType(type);
    setSize(size);
    setColor(color);
    setMatrix(matrix);

    initialize();
}

void VOpenGLFragment::setType(Type type)
{
    _type = type;
}

void VOpenGLFragment::setSize(const QVector3D &size)
{
    _size = size;
}

void VOpenGLFragment::setColor(const QVector3D &color)
{
    _color = color;
}

void VOpenGLFragment::setMatrix(GLfloat *matrix)
{
    _matrix = matrix;
}

VOpenGLFragment::Type VOpenGLFragment::type() const
{
    return _type;
}

QVector3D VOpenGLFragment::size() const
{
    return _size;
}

QVector3D VOpenGLFragment::color() const
{
    return _color;
}

GLfloat *VOpenGLFragment::matrix() const
{
    return _matrix;
}

GLuint VOpenGLFragment::getVertexID()
{
    return _vertex_id;
}

GLuint VOpenGLFragment::getTextureID()
{
    return _texture_id;
}

GLsizei VOpenGLFragment::getDrawSize()
{
    return _draw_size;
}

void VOpenGLFragment::initialize()
{
    QVector<VOpenGLVertex> opengl_vertex_list;
    QVector<GLuint> index_list;

    if (type() == Ground) {

        for (int i = 0; i < size().x(); ++i) {
            for (int j = 0; j < size().z(); ++j) {
                opengl_vertex_list.append(VOpenGLVertex(QVector3D((i + 0.5f) - (size().x() / 2.0f), 1.0f, (j - 0.5f) - (size().z() / 2.0f)), QVector3D(0.0f, 1.0f, 0.0f), QVector2D(1.0f, 0.0f)));
                opengl_vertex_list.append(VOpenGLVertex(QVector3D((i + 0.5f) - (size().x() / 2.0f), 1.0f, (j + 0.5f) - (size().z() / 2.0f)), QVector3D(0.0f, 1.0f, 0.0f), QVector2D(1.0f, 1.0f)));
                opengl_vertex_list.append(VOpenGLVertex(QVector3D((i - 0.5f) - (size().x() / 2.0f), 1.0f, (j + 0.5f) - (size().z() / 2.0f)), QVector3D(0.0f, 1.0f, 0.0f), QVector2D(0.0f, 1.0f)));
                opengl_vertex_list.append(VOpenGLVertex(QVector3D((i - 0.5f) - (size().x() / 2.0f), 1.0f, (j - 0.5f) - (size().z() / 2.0f)), QVector3D(0.0f, 1.0f, 0.0f), QVector2D(0.0f, 0.0f)));
            }
        }

        for (int i = 0; i < size().x() * size().z(); ++i) {
            index_list.append(i * 4);
            index_list.append((i * 4) + 2);
            index_list.append((i * 4) + 1);
            index_list.append(i * 4);
            index_list.append((i * 4) + 3);
            index_list.append((i * 4) + 2);
        }

        QImage image;

        image.load(":/texture/floor.png");

        glGenTextures(1, &_texture_id);
        glBindTexture(GL_TEXTURE_2D, _texture_id);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image.width(), image.height(), 0, GL_RGB, GL_UNSIGNED_BYTE, image.convertToFormat(QImage::Format_RGB888).bits());
        glGenerateMipmap(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, 0);

        _draw_size = size().x() * size().z() * 6;

    } else {

        opengl_vertex_list.append(VOpenGLVertex(QVector3D(0.5f * size().x(), 0.5f * size().y(), 0.5f * size().z()), QVector3D(0.0f, 0.0f, 1.0f), QVector2D(0.0f, 0.0f)));
        opengl_vertex_list.append(VOpenGLVertex(QVector3D(0.5f * size().x(), -0.5f * size().y(), 0.5f * size().z()), QVector3D(0.0f, 0.0f, 1.0f), QVector2D(0.0f, 0.0f)));
        opengl_vertex_list.append(VOpenGLVertex(QVector3D(-0.5f * size().x(), -0.5f * size().y(), 0.5f * size().z()), QVector3D(0.0f, 0.0f, 1.0f), QVector2D(0.0f, 0.0f)));
        opengl_vertex_list.append(VOpenGLVertex(QVector3D(-0.5f * size().x(), 0.5f * size().y(), 0.5f * size().z()), QVector3D(0.0f, 0.0f, 1.0f), QVector2D(0.0f, 0.0f)));
        opengl_vertex_list.append(VOpenGLVertex(QVector3D(-0.5f * size().x(), 0.5f * size().y(), -0.5f * size().z()), QVector3D(0.0f, 0.0f, -1.0f), QVector2D(0.0f, 0.0f)));
        opengl_vertex_list.append(VOpenGLVertex(QVector3D(-0.5f * size().x(), -0.5f * size().y(), -0.5f * size().z()), QVector3D(0.0f, 0.0f, -1.0f), QVector2D(0.0f, 0.0f)));
        opengl_vertex_list.append(VOpenGLVertex(QVector3D(0.5f * size().x(), -0.5f * size().y(), -0.5f * size().z()), QVector3D(0.0f, 0.0f, -1.0f), QVector2D(0.0f, 0.0f)));
        opengl_vertex_list.append(VOpenGLVertex(QVector3D(0.5f * size().x(), 0.5f * size().y(), -0.5f * size().z()), QVector3D(0.0f, 0.0f, -1.0f), QVector2D(0.0f, 0.0f)));
        opengl_vertex_list.append(VOpenGLVertex(QVector3D(-0.5f * size().x(), 0.5f * size().y(), 0.5f * size().z()), QVector3D(-1.0f, 0.0f, 0.0f), QVector2D(0.0f, 0.0f)));
        opengl_vertex_list.append(VOpenGLVertex(QVector3D(-0.5f * size().x(), -0.5f * size().y(), 0.5f * size().z()), QVector3D(-1.0f, 0.0f, 0.0f), QVector2D(0.0f, 0.0f)));
        opengl_vertex_list.append(VOpenGLVertex(QVector3D(-0.5f * size().x(), -0.5f * size().y(), -0.5f * size().z()), QVector3D(-1.0f, 0.0f, 0.0f), QVector2D(0.0f, 0.0f)));
        opengl_vertex_list.append(VOpenGLVertex(QVector3D(-0.5f * size().x(), 0.5f * size().y(), -0.5f * size().z()), QVector3D(-1.0f, 0.0f, 0.0f), QVector2D(0.0f, 0.0f)));
        opengl_vertex_list.append(VOpenGLVertex(QVector3D(0.5f * size().x(), 0.5f * size().y(), -0.5f * size().z()), QVector3D(1.0f, 0.0f, 0.0f), QVector2D(0.0f, 0.0f)));
        opengl_vertex_list.append(VOpenGLVertex(QVector3D(0.5f * size().x(), -0.5f * size().y(), -0.5f * size().z()), QVector3D(1.0f, 0.0f, 0.0f), QVector2D(0.0f, 0.0f)));
        opengl_vertex_list.append(VOpenGLVertex(QVector3D(0.5f * size().x(), -0.5f * size().y(), 0.5f * size().z()), QVector3D(1.0f, 0.0f, 0.0f), QVector2D(0.0f, 0.0f)));
        opengl_vertex_list.append(VOpenGLVertex(QVector3D(0.5f * size().x(), 0.5f * size().y(), 0.5f * size().z()), QVector3D(1.0f, 0.0f, 0.0f), QVector2D(0.0f, 0.0f)));
        opengl_vertex_list.append(VOpenGLVertex(QVector3D(0.5f * size().x(), 0.5f * size().y(), -0.5f * size().z()), QVector3D(0.0f, 1.0f, 0.0f), QVector2D(0.0f, 0.0f)));
        opengl_vertex_list.append(VOpenGLVertex(QVector3D(0.5f * size().x(), 0.5f * size().y(), 0.5f * size().z()), QVector3D(0.0f, 1.0f, 0.0f), QVector2D(0.0f, 0.0f)));
        opengl_vertex_list.append(VOpenGLVertex(QVector3D(-0.5f * size().x(), 0.5f * size().y(), 0.5f * size().z()), QVector3D(0.0f, 1.0f, 0.0f), QVector2D(0.0f, 0.0f)));
        opengl_vertex_list.append(VOpenGLVertex(QVector3D(-0.5f * size().x(), 0.5f * size().y(), -0.5f * size().z()), QVector3D(0.0f, 1.0f, 0.0f), QVector2D(0.0f, 0.0f)));
        opengl_vertex_list.append(VOpenGLVertex(QVector3D(0.5f * size().x(), -0.5f * size().y(), 0.5f * size().z()), QVector3D(0.0f, -1.0f, 0.0f), QVector2D(0.0f, 0.0f)));
        opengl_vertex_list.append(VOpenGLVertex(QVector3D(0.5f * size().x(), -0.5f * size().y(), -0.5f * size().z()), QVector3D(0.0f, -1.0f, 0.0f), QVector2D(0.0f, 0.0f)));
        opengl_vertex_list.append(VOpenGLVertex(QVector3D(-0.5f * size().x(), -0.5f * size().y(), -0.5f * size().z()), QVector3D(0.0f, -1.0f, 0.0f), QVector2D(0.0f, 0.0f)));
        opengl_vertex_list.append(VOpenGLVertex(QVector3D(-0.5f * size().x(), -0.5f * size().y(), 0.5f * size().z()), QVector3D(0.0f, -1.0f, 0.0f), QVector2D(0.0f, 0.0f)));

        for (int i = 0; i < 6; ++i) {
            index_list.append(i * 4);
            index_list.append((i * 4) + 2);
            index_list.append((i * 4) + 1);
            index_list.append(i * 4);
            index_list.append((i * 4) + 3);
            index_list.append((i * 4) + 2);
        }

        _draw_size = 36;

    }

    GLuint buffer_id;

    glGenVertexArrays(1, &_vertex_id);
    glBindVertexArray(_vertex_id);
    glGenBuffers(1, &buffer_id);
    glBindBuffer(GL_ARRAY_BUFFER, buffer_id);
    glBufferData(GL_ARRAY_BUFFER, opengl_vertex_list.count() * sizeof(VOpenGLVertex), opengl_vertex_list.constData(), GL_STATIC_DRAW);
    glGenBuffers(1, &buffer_id);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer_id);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_list.count() * sizeof(GLuint), index_list.constData(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VOpenGLVertex), (void *) 0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VOpenGLVertex), (void *) (3 * sizeof(GLfloat)));
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(VOpenGLVertex), (void *) (6 * sizeof(GLfloat)));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}
