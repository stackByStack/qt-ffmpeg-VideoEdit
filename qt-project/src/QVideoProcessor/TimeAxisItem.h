#ifndef TIMEAXISITEM_H
#define TIMEAXISITEM_H

#include <QMetaType>
//Time Axis Item type
typedef struct
{
    QString type;
    QString srcPath;
    QString value;
    qint64 qStartTime;
    qint64 qEndTime;
    bool TranslationEnabled;
    bool FadeEnabled;
    int x;
    int y;
} timeAxisItem;

Q_DECLARE_METATYPE(timeAxisItem)

#endif // TIMEAXISITEM_H
