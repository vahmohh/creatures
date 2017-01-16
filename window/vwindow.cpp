#include "vwindow.h"

#include <QMatrix4x4>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <QMessageBox>
#include <QDir>

#include "opengl/vopenglshaderprogram.h"

VOpenGLShaderProgram opengl_shader_program;
QMatrix4x4 camera_matrix;
VNeuralNetworkWindow *neural_network_window;

bool save_neural_network = false;

void keyEvent(GLFWwindow *window, int key, int, int action, int);

VWindow::VWindow(QObject *parent) : QObject(parent)
{
    qRegisterMetaType<QHash<uint,VOpenGLFragment> >("QHash<uint,VOpenGLFragment>");
    qRegisterMetaType<VNeuralNetwork>("VNeuralNetwork");

    connect(&_world, SIGNAL(updatedSignal(QHash<uint,VOpenGLFragment>)), this, SLOT(worldUpdatedSlot(QHash<uint,VOpenGLFragment>)));
    connect(&_world, SIGNAL(updatedNeuralNetworkSignal(VNeuralNetwork,VNeuralNetwork)), this, SLOT(worldUpdatedNeuralNetworkSlot(VNeuralNetwork,VNeuralNetwork)));
    connect(&_neural_network_window, SIGNAL(textChangedSignal()), this, SLOT(neuralNetworkWindowTextChangedSlot()));
    connect(&_neural_network_window, SIGNAL(keyPressedSignal(int)), this, SLOT(neuralNetworkWindowKeyPressedSlot(int)));

    neural_network_window = &_neural_network_window;
}

VWindow::~VWindow()
{
    glfwTerminate();
}

void VWindow::show()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwSetKeyCallback(_window = glfwCreateWindow(800, 600, "", NULL, NULL), keyEvent);
    glfwMakeContextCurrent(_window);

    glewExperimental = GL_TRUE;
    glewInit();

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    QMatrix4x4 projection_matrix;

    projection_matrix.perspective(45.0f, 800.0f / 600.0f, 1.0f, 1000.0f);
    camera_matrix.lookAt(QVector3D(0.0f, 5.0f, 20.0f), QVector3D(0.0f, 0.0f, 0.0f), QVector3D(0.0f, 1.0f, 0.0f));

    opengl_shader_program.initialize();
    opengl_shader_program.updateProjectionMatrix(projection_matrix.data());
    opengl_shader_program.updateCameraMatrix(camera_matrix.data());

    bool mode = false;
    if (QDir().exists("nn") && QDir("nn").count() > 2) {
        QMessageBox msgBox;
        msgBox.setText("Do you wish to run the application in the presentation mode?");
        msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        msgBox.setDefaultButton(QMessageBox::No);
        int ret = msgBox.exec();
        mode = ret == QMessageBox::Yes ? true : false;
    }

    _world.initialize(mode);

    _neural_network_window.setMinimumSize(800, 600);
    _neural_network_window.setMaximumSize(800, 600);
    _neural_network_window.setHidden(true);

    qDebug() << "Manual:\rA: Left\rD: Right\rW: Forward\rS: Backward\rN: Save Neural Networks\rM: Show Neural Networks";
}

void VWindow::worldUpdatedSlot(const QHash<uint, VOpenGLFragment> &openglFragmentHash)
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    foreach (VOpenGLFragment opengl_fragment, openglFragmentHash) {

        opengl_shader_program.updateModelMatrix(opengl_fragment.matrix());
        opengl_shader_program.updateType(opengl_fragment.type());

        if (opengl_fragment.type() == VOpenGLFragment::Ground) {
            glBindTexture(GL_TEXTURE_2D, opengl_fragment.getTextureID());
        } else {
            opengl_shader_program.updateColor(opengl_fragment.color().x(), opengl_fragment.color().y(), opengl_fragment.color().z());
        }

        glBindVertexArray(opengl_fragment.getVertexID());
        glDrawElements(GL_TRIANGLES, opengl_fragment.getDrawSize(), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

    }

    glfwSwapBuffers(_window);
    glfwPollEvents();

    if (glfwWindowShouldClose(_window) == GL_TRUE) {
        exit(0);
    }

    if (save_neural_network) {
        _world.saveNeuralNetwork();
        save_neural_network = false;
    }
}

void VWindow::worldUpdatedNeuralNetworkSlot(const VNeuralNetwork &neuralNetwork, const VNeuralNetwork &neuralNetwork2)
{
    _neural_network_window.updateNeuralNetwork(neuralNetwork, neuralNetwork2);
}

void VWindow::neuralNetworkWindowTextChangedSlot()
{
    _world.setNeuralNetworkNumber(_neural_network_window.getNumber());
    _world.setNeuralNetworkNumber2(_neural_network_window.getNumber2());
}

void VWindow::neuralNetworkWindowKeyPressedSlot(int key)
{
    keyEvent(_window, key, 0, GLFW_PRESS, 0);
}

void keyEvent(GLFWwindow *window, int key, int, int action, int)
{
    if (action != GLFW_RELEASE) {

        switch (key) {
        case GLFW_KEY_A:
            camera_matrix.translate(0.7f, 0.0f, 0.0f);
            break;
        case GLFW_KEY_D:
            camera_matrix.translate(-0.7f, 0.0f, 0.0f);
            break;
        case GLFW_KEY_W:
            camera_matrix.translate(0.0f, 0.0f, 0.7f);
            break;
        case GLFW_KEY_S:
            camera_matrix.translate(0.0f, 0.0f, -0.7f);
            break;
        case GLFW_KEY_I:
            camera_matrix.translate(0.0f, -0.7f, 0.0f);
            break;
        case GLFW_KEY_K:
            camera_matrix.translate(0.0f, 0.7f, 0.0f);
            break;
        case GLFW_KEY_N:
            save_neural_network = true;
            break;
        case GLFW_KEY_M:
            if (neural_network_window->isHidden()) {
                neural_network_window->show();
            } else {
                neural_network_window->setHidden(true);
            }
            break;
        case GLFW_KEY_ESCAPE:
            glfwSetWindowShouldClose(window, true);
            break;
        default:
            break;
        }

        opengl_shader_program.updateCameraMatrix(camera_matrix.data());

    }
}
