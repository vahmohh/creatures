#include "vutility.h"

#include <QThread>
#include <QUuid>
#include <QFile>
#include <QDir>

void VUtility::moveToThread(QObject *object)
{
    object->moveToThread(new QThread());
    object->thread()->start();
}

QString VUtility::uuid()
{
    return QUuid::createUuid().toString();
}

int VUtility::random(int min, int max)
{
    std::random_device random_device;
    std::mt19937 mt19937(random_device());
    std::uniform_int_distribution<int> uniform_int_distribution(min, max);

    return uniform_int_distribution(mt19937);
}

float VUtility::random(float min, float max)
{
    std::random_device random_device;
    std::mt19937 mt19937(random_device());
    std::uniform_real_distribution<float> uniform_real_distribution(min, max);

    return uniform_real_distribution(mt19937);
}

bool VUtility::removeDir(const QString & dirName)
{
    bool result = true;
    QDir dir(dirName);

    if (dir.exists()) {
        Q_FOREACH(QFileInfo info, dir.entryInfoList(QDir::NoDotAndDotDot | QDir::System | QDir::Hidden  | QDir::AllDirs | QDir::Files, QDir::DirsFirst)) {
            if (info.isDir()) {
                result = removeDir(info.absoluteFilePath());
            }
            else {
                result = QFile::remove(info.absoluteFilePath());
            }

            if (!result) {
                return result;
            }
        }
        result = QDir().rmdir(dirName);
    }
    return result;
}
