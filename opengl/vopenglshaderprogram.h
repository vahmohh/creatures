#ifndef VOPENGLSHADERPROGRAM_H
#define VOPENGLSHADERPROGRAM_H

#include <GL/glew.h>

class VOpenGLShaderProgram
{
public:
    VOpenGLShaderProgram();

    void initialize();

    void updateProjectionMatrix(GLfloat *matrix);
    void updateModelMatrix(GLfloat *matrix);
    void updateCameraMatrix(GLfloat *matrix);
    void updateType(GLint type);
    void updateColor(GLfloat red, GLfloat green, GLfloat blue);

private:
    GLint _projection_matrix_id, _model_matrix_id, _camera_matrix_id, _type_id, _color_id;
};

#endif // VOPENGLSHADERPROGRAM_H
