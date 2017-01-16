QT += widgets

LIBS += -L/usr/lib/x86_64-linux-gnu/ -lGLEW -lglfw -lBulletDynamics -lBulletCollision -lLinearMath

INCLUDEPATH += /usr/include/bullet

SOURCES += \
    main.cpp \
    window/vwindow.cpp \
    opengl/vopenglshaderprogram.cpp \
    opengl/vopenglvertex.cpp \
    opengl/vopenglfragment.cpp \
    world/vworld.cpp \
    creature/vcreature.cpp \
    segment/vsegment.cpp \
    joint/vjoint.cpp \
    utility/vutility.cpp \
    neuralnetwork/vneuralnetwork.cpp \
    neuron/vneuron.cpp \
    layer/vlayer.cpp \
    neuralnetworkwindow/vneuralnetworkwindow.cpp

HEADERS += \
    window/vwindow.h \
    opengl/vopenglshaderprogram.h \
    opengl/vopenglvertex.h \
    opengl/vopenglfragment.h \
    world/vworld.h \
    world/vworldconfiguration.h \
    creature/vcreature.h \
    segment/vsegment.h \
    joint/vjoint.h \
    utility/vutility.h \
    neuralnetwork/vneuralnetwork.h \
    neuron/vneuron.h \
    layer/vlayer.h \
    neuralnetworkwindow/vneuralnetworkwindow.h

RESOURCES += \
    resource/texture.qrc
