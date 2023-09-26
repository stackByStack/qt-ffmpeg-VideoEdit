#ifndef PHOTOORTEXTITEM_H
#define PHOTOORTEXTITEM_H

#include <QMetaType>
// Material Library item type
typedef struct
{
    QString type;
    QString srcPath;
    QString name;
} photoOrTextItem;

Q_DECLARE_METATYPE(photoOrTextItem)

#endif // PHOTOORTEXTITEM_H
