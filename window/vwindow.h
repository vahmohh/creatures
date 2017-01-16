#ifndef VWINDOW_H
#define VWINDOW_H

#include "world/vworld.h"
#include "neuralnetworkwindow/vneuralnetworkwindow.h"

class GLFWwindow;

class VWindow : public QObject
{
    Q_OBJECT

public:
    VWindow(QObject *parent = 0);
    ~VWindow();

    void show();

private:
    GLFWwindow *_window;
    VWorld _world;
    VNeuralNetworkWindow _neural_network_window;

private slots:
    void worldUpdatedSlot(const QHash<uint, VOpenGLFragment> &openglFragmentHash);
    void worldUpdatedNeuralNetworkSlot(const VNeuralNetwork &neuralNetwork, const VNeuralNetwork &neuralNetwork2);
    void neuralNetworkWindowTextChangedSlot();
    void neuralNetworkWindowKeyPressedSlot(int key);
};

#endif // VWINDOW_H
