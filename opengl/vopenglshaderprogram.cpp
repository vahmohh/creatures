#include "vopenglshaderprogram.h"

VOpenGLShaderProgram::VOpenGLShaderProgram()
{

}

void VOpenGLShaderProgram::initialize()
{
    const GLchar *vertex_shader_source =
            "#version 330 core\n"
            "layout (location = 0) in vec3 _position;\n"
            "layout (location = 1) in vec3 _normal;\n"
            "layout (location = 2) in vec2 _texture_coordinate;\n"
            "out vec3 position;\n"
            "out vec3 normal;\n"
            "out vec2 texture_coordinate;\n"
            "uniform mat4 ProjectionMatrix;\n"
            "uniform mat4 ModelMatrix;\n"
            "uniform mat4 CameraMatrix;\n"
            "void main()\n"
            "{\n"
            "   gl_Position = ProjectionMatrix * CameraMatrix * ModelMatrix * vec4(_position, 1.0f);\n"
            "   position = vec3(ModelMatrix * vec4(_position, 1.0f));\n"
            "   normal = mat3(transpose(inverse(ModelMatrix))) * _normal;\n"
            "   texture_coordinate = _texture_coordinate;\n"
            "}\n";

    const GLchar *fragment_shader_source =
            "#version 330 core\n"
            "in vec3 position;\n"
            "in vec3 normal;\n"
            "in vec2 texture_coordinate;\n"
            "out vec4 color;\n"
            "uniform int Type;\n"
            "uniform vec3 Color;\n"
            "uniform sampler2D Texture;\n"
            "void main()\n"
            "{\n"
            "   if (Type == 0) {\n"
            "       color = texture(Texture, texture_coordinate);\n"
            "   } else {\n"
            "       color = vec4(((0.1f * vec3(1.0f, 1.0f, 1.0f)) + (max(dot(normalize(normal), normalize(vec3(1.0f, 1000.0f, 0.0f) - position)), 0.0f) * vec3(1.0f, 1.0f, 1.0f))) * Color, 1);\n"
            "   }\n"
            "}\n";

    GLuint id, vertex_id, fragment_id;

    glShaderSource(vertex_id = glCreateShader(GL_VERTEX_SHADER), 1, &vertex_shader_source, NULL);
    glShaderSource(fragment_id = glCreateShader(GL_FRAGMENT_SHADER), 1, &fragment_shader_source, NULL);
    glCompileShader(vertex_id);
    glCompileShader(fragment_id);
    glAttachShader(id = glCreateProgram(), vertex_id);
    glAttachShader(id, fragment_id);
    glLinkProgram(id);
    glUseProgram(id);

    _projection_matrix_id = glGetUniformLocation(id, "ProjectionMatrix");
    _model_matrix_id = glGetUniformLocation(id, "ModelMatrix");
    _camera_matrix_id = glGetUniformLocation(id, "CameraMatrix");
    _type_id = glGetUniformLocation(id, "Type");
    _color_id = glGetUniformLocation(id, "Color");
}

void VOpenGLShaderProgram::updateProjectionMatrix(GLfloat *matrix)
{
    glUniformMatrix4fv(_projection_matrix_id, 1, GL_FALSE, matrix);
}

void VOpenGLShaderProgram::updateModelMatrix(GLfloat *matrix)
{
    glUniformMatrix4fv(_model_matrix_id, 1, GL_FALSE, matrix);
}

void VOpenGLShaderProgram::updateCameraMatrix(GLfloat *matrix)
{
    glUniformMatrix4fv(_camera_matrix_id, 1, GL_FALSE, matrix);
}

void VOpenGLShaderProgram::updateType(GLint type)
{
    glUniform1i(_type_id, type);
}

void VOpenGLShaderProgram::updateColor(GLfloat red, GLfloat green, GLfloat blue)
{
    glUniform3f(_color_id, red, green, blue);
}
