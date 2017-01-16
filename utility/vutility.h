#ifndef VUTILITY_H
#define VUTILITY_H

class QObject;
class QString;

class VUtility
{
public:
    static void moveToThread(QObject *object);
    static QString uuid();
    static int random(int min, int max);
    static float random(float min, float max);
    static bool removeDir(const QString & dirName);
};

#endif // VUTILITY_H
