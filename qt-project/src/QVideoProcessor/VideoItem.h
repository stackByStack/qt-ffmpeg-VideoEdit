#ifndef VIDEOITEM_H
#define VIDEOITEM_H

#include <QMetaType>
#include <QPixmap>
// Material Library Item Type
typedef struct {
    QString type;
    QString srcPath;
    QString name;
    QString firstFrame;
} VideoItem;

Q_DECLARE_METATYPE(VideoItem)

#endif // VIDEOITEM_H
