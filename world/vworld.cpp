#include "vworld.h"

#include <QMatrix4x4>
#include <QFile>
#include <QDir>

#include "vworldconfiguration.h"
#include "creature/vcreature.h"
#include "segment/vsegment.h"
#include "utility/vutility.h"

VWorld::VWorld(QObject *parent) : QObject(parent)
{
    _discrete_dynamics_world = new btDiscreteDynamicsWorld(new btCollisionDispatcher(new btDefaultCollisionConfiguration()), new btDbvtBroadphase(), new btSequentialImpulseConstraintSolver(), new btDefaultCollisionConfiguration());
    _current_time = 0;
    _generation = 1;
    _neural_network_number = 1;
    _neural_network_number_2 = 2;
    _save_neural_network = false;

    connect(this, SIGNAL(startTimerSignal()), this, SLOT(startTimerSlot()));

    VUtility::moveToThread(this);
}

VWorld::~VWorld()
{
    foreach (VCreature *tmp_creature, _creature_hash) {
        delete tmp_creature;
    }

    delete _discrete_dynamics_world->getDispatcher();
    delete _discrete_dynamics_world->getBroadphase();
    delete _discrete_dynamics_world->getConstraintSolver();
    delete _discrete_dynamics_world;
}

void VWorld::setNeuralNetworkNumber(int number)
{
    _neural_network_number = number;

    if (_creature_hash.contains(_neural_network_number) && _creature_hash.contains(_neural_network_number_2)) {
        emit updatedNeuralNetworkSignal(_creature_hash.value(_neural_network_number)->getNeuralNetwork(), _creature_hash.value(neuralNetworkNumber2())->getNeuralNetwork());
    } else {
        emit updatedNeuralNetworkSignal(VNeuralNetwork(), VNeuralNetwork());
    }
}

void VWorld::setNeuralNetworkNumber2(int number)
{
    _neural_network_number_2 = number;

    if (_creature_hash.contains(_neural_network_number) && _creature_hash.contains(_neural_network_number_2)) {
        emit updatedNeuralNetworkSignal(_creature_hash.value(neuralNetworkNumber())->getNeuralNetwork(), _creature_hash.value(neuralNetworkNumber2())->getNeuralNetwork());
    } else {
        emit updatedNeuralNetworkSignal(VNeuralNetwork(), VNeuralNetwork());
    }
}

int VWorld::neuralNetworkNumber()
{
    return _neural_network_number;
}

int VWorld::neuralNetworkNumber2()
{
    return _neural_network_number_2;
}

void VWorld::initialize(bool mode)
{
    _mode = mode;

    _discrete_dynamics_world->setGravity(btVector3(0.0f, -9.8f, 0.0f));
    _discrete_dynamics_world->addRigidBody(new btRigidBody(btRigidBody::btRigidBodyConstructionInfo(0.0f, new btDefaultMotionState(btTransform(btQuaternion(0.0f, 0.0f, 0.0f, 1.0f), btVector3(0.0f, 0.0f, 0.0f))), new btStaticPlaneShape(btVector3(0.0f, 1.0f, 0.0f), 1.0f), btVector3(0.0f, 0.0f, 0.0f))));

    _opengl_fragment_hash.insertMulti(0, VOpenGLFragment(VOpenGLFragment::Ground, QVector3D(500.0f, 0.0f, 500.0f), QVector3D(), (new QMatrix4x4())->data()));

    if (mode == false) {
        for (int i = 0; i < CREATURE_NUMBER; ++i) {

            uint creature_id = i + 1;

            _creature_hash.insert(creature_id, new VCreature(creature_id, 3.0f, 2.0f, _discrete_dynamics_world, QVector3D(0.0f, 1.0f, i * 10.0f)));

            foreach (VSegment *tmp_segment, _creature_hash[creature_id]->getSegmentList()) {
                _opengl_fragment_hash.insertMulti(creature_id, VOpenGLFragment(VOpenGLFragment::Segment, tmp_segment->size(), tmp_segment->color(), tmp_segment->getMatrix()));
            }

        }
    } else {

        QDir recoredDir("nn");
        QStringList allFiles = recoredDir.entryList(QDir::NoDotAndDotDot | QDir::System | QDir::Hidden  | QDir::AllDirs | QDir::Files, QDir::DirsFirst);

        for (int i = 0; i < QDir("nn").count() - 2; ++i) {

            //
            QFile file("nn/" + allFiles.at(i));
            file.open(QIODevice::ReadOnly);
            QDataStream in(&file);    // read the data serialized from the file
            VNeuralNetwork nn;
            in >> nn;           // extract "the answer is" and 42
            //

            uint creature_id = i + 1;

            _creature_hash.insert(creature_id, new VCreature(creature_id, 3.0f, 2.0f, _discrete_dynamics_world, QVector3D(0.0f, 1.0f, i * 10.0f), nn));

            foreach (VSegment *tmp_segment, _creature_hash[creature_id]->getSegmentList()) {
                _opengl_fragment_hash.insertMulti(creature_id, VOpenGLFragment(VOpenGLFragment::Segment, tmp_segment->size(), tmp_segment->color(), tmp_segment->getMatrix()));
            }

        }
    }


    emit startTimerSignal();
}

void VWorld::saveNeuralNetwork()
{
    _save_neural_network = true;
}

void VWorld::timerEvent(QTimerEvent *)
{
    _discrete_dynamics_world->stepSimulation(1.0f);

    foreach (VCreature *tmp_creature, _creature_hash) {
        tmp_creature->update();
    }

    if (++_current_time > 100 && _current_time % 100 == 0) {
        foreach (VCreature *tmp_creature, _creature_hash) {
            tmp_creature->move();
        }
    }

    if (_save_neural_network) {
        if (QDir().exists("nn")) {
            VUtility::removeDir("nn");
        }
        QDir().mkdir("nn");

        int i = 0;
        foreach (VCreature *tmp_creature, _creature_hash) {
            QFile file("nn/generation_" + QString::number(_generation) + "_number_" + QString::number(++i) + ".nn");
            file.open(QIODevice::WriteOnly);
            QDataStream out(&file);
            VNeuralNetwork n = tmp_creature->getNeuralNetwork();
            out << n;
        }
        _save_neural_network = false;
        qDebug() << "Neural Networks Saved Successfuly";
    }

    if (_current_time % 6000 == 0) {

        QMap<float, VCreature *> creature_map;

        foreach (VCreature *tmp_creature, _creature_hash) {
            creature_map.insert(tmp_creature->getFitness(), tmp_creature);
        }

        foreach (VCreature *tmp_creature, _creature_hash) {
            tmp_creature->reset();
        }

        _current_time = 0;

        if (_mode == false) {

            int counter = 0;

            QList<VCreature *> best_creature_list;

            foreach (float tmp_id, creature_map.keys()) {
                if (++counter <= creature_map.count() / 2) {
                    creature_map.value(tmp_id)->initializeNeuralNetwork();
                } else {
                    best_creature_list.append(creature_map.value(tmp_id));
                }
            }

            for (int i = 0; i < best_creature_list.count(); i += 2) {

                //best_creature_list.at(i)->setNeuralNetwork(best_creature_list.at(i)->getNeuralNetwork());
                QList<VNeuralNetwork> neural_network_list = VNeuralNetwork::crossover(best_creature_list.at(i)->getNeuralNetwork(), best_creature_list.at(i + 1)->getNeuralNetwork());

                best_creature_list.at(i)->setNeuralNetwork(neural_network_list.at(0));

                if (i != best_creature_list.count() - 2) {
                    best_creature_list.at(i + 1)->setNeuralNetwork(neural_network_list.at(1));
                }
            }

            QList<VNeuralNetwork> neural_network_list = VNeuralNetwork::crossover(_creature_hash.value(1)->getNeuralNetwork(), _creature_hash.value(2)->getNeuralNetwork());

            _creature_hash.value(1)->setNeuralNetwork(neural_network_list.at(0));
            _creature_hash.value(2)->setNeuralNetwork(neural_network_list.at(1));


            if (_creature_hash.contains(neuralNetworkNumber()) && _creature_hash.contains(neuralNetworkNumber2())) {
                emit updatedNeuralNetworkSignal(_creature_hash.value(neuralNetworkNumber())->getNeuralNetwork(), _creature_hash.value(neuralNetworkNumber2())->getNeuralNetwork());
            } else {
                emit updatedNeuralNetworkSignal(VNeuralNetwork(), VNeuralNetwork());
            }

            qDebug() << "Generation: " << _generation++ << "  Maximum Fitness: " << creature_map.lastKey() << "  " << creature_map.last()->id() << "th Individual";


        } else {

            qDebug() << "Generation: Presentation  Maximum Fitness: " << creature_map.lastKey() << "  " << creature_map.last()->id() << "th Individual";

        }
    }

    if (_current_time % 5 == 0) {
        emit updatedSignal(_opengl_fragment_hash);
    }
}

void VWorld::startTimerSlot()
{
    startTimer(SIMULATION_STEP_TIMEOUT);
}
