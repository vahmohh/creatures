#include "vneuralnetworkwindow.h"

#include <QPainter>
#include <QKeyEvent>

#include "../world/vworldconfiguration.h"

VNeuralNetworkWindow::VNeuralNetworkWindow(QWidget *parent) : QWidget(parent)
{
    setFocusPolicy(Qt::StrongFocus);
    setWindowFlags(Qt::FramelessWindowHint);

    for (int i = 0; i < CREATURE_NUMBER; ++i) {
        _combo_box.addItem(QString::number(i + 1));
        _combo_box_2.addItem(QString::number(i + 1));
    }

    _combo_box.setParent(this);
    _combo_box.setGeometry(0, 570, 100, 20);
    _combo_box.setCurrentIndex(0);
    _combo_box.show();

    _combo_box_2.setParent(this);
    _combo_box_2.setGeometry(500, 570, 100, 20);
    _combo_box_2.setCurrentIndex(1);
    _combo_box_2.show();

    connect(&_combo_box, SIGNAL(currentIndexChanged(int)), this, SIGNAL(textChangedSignal()));
    connect(&_combo_box_2, SIGNAL(currentIndexChanged(int)), this, SIGNAL(textChangedSignal()));
}

int VNeuralNetworkWindow::getNumber() const
{
    return _combo_box.currentText().toInt();
}

int VNeuralNetworkWindow::getNumber2() const
{
    return _combo_box_2.currentText().toInt();
}

void VNeuralNetworkWindow::updateNeuralNetwork(const VNeuralNetwork &neuralNetwork, const VNeuralNetwork &neuralNetwork2)
{
    _neural_network = neuralNetwork;
    _neural_network_2 = neuralNetwork2;

    update();
}

void VNeuralNetworkWindow::showEvent(QShowEvent *)
{
    emit textChangedSignal();

    setFocus();
}

void VNeuralNetworkWindow::keyPressEvent(QKeyEvent *key)
{
    emit keyPressedSignal(key->key());
}

void VNeuralNetworkWindow::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    QPen pen;

    painter.setRenderHint(QPainter::Antialiasing);

    for (int i = 0; i < _neural_network.getLayerList()->count(); ++i) {
        for (int j = 0; j < 8; ++j) {

            pen.setColor(Qt::red);
            painter.setPen(pen);
            painter.drawEllipse(QPoint(120 * i + 50, 70 * j + 50), 10, 10);

            pen.setColor(Qt::black);
            painter.setPen(pen);

            if (i == 0) {
                painter.drawLine(0, 70 * j + 50, 120 * i + 50, 70 * j + 50);
            } else if (i == _neural_network.getLayerList()->count() - 1) {
                painter.drawLine(120 * i + 50, 70 * j + 50, 120 * i + 120, 70 * j + 50);
            }

            for (int k = 0; k < 8; ++k) {
                if (i < _neural_network.getLayerList()->count() - 1 && _neural_network.getLayer(i + 1)->existConnection(j, k)) {
                    painter.drawLine(120 * i + 50, 70 * j + 50, 120 * (i + 1) + 50, 70 * k + 50);
                }
            }
        }
    }

    for (int i = 0; i < _neural_network_2.getLayerList()->count(); ++i) {
        for (int j = 0; j < 8; ++j) {

            pen.setColor(Qt::red);
            painter.setPen(pen);
            painter.drawEllipse(QPoint(120 * i + 500, 70 * j + 50), 10, 10);

            pen.setColor(Qt::black);
            painter.setPen(pen);

            if (i == 0) {
                painter.drawLine(450, 70 * j + 50, 120 * i + 500, 70 * j + 50);
            } else if (i == _neural_network_2.getLayerList()->count() - 1) {
                painter.drawLine(120 * i + 500, 70 * j + 50, 120 * i + 600, 70 * j + 50);
            }

            for (int k = 0; k < 8; ++k) {
                if (i < _neural_network_2.getLayerList()->count() - 1 && _neural_network_2.getLayer(i + 1)->existConnection(j, k)) {
                    painter.drawLine(120 * i + 500, 70 * j + 50, 120 * (i + 1) + 500, 70 * k + 50);
                }
            }
        }
    }

    painter.drawLine(405, 0, 405, 600);
}
