#include "videoitemdelegate.h"
#include <QPainter>
#include "VideoItem.h"
#include <QRectF>
#include <QRect>
#include <QPainterPath>
#include <QPixmap>
#include <QImage>
#include <QPen>
#include <QFont>


VideoItemDelegate::VideoItemDelegate(QObject *parent) : QStyledItemDelegate(parent)
{

}

void VideoItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if(index.isValid())
    {
        painter->save();
        QVariant var = index.data(Qt::UserRole + 1);
        VideoItem itemData = var.value<VideoItem>();

        QRectF rect;
        rect.setX(option.rect.x());
        rect.setY(option.rect.y());
        rect.setWidth(option.rect.width()-1);
        rect.setHeight(option.rect.height()-1);

        QPainterPath path;
        path.moveTo(rect.topRight());
        path.lineTo(rect.topLeft());
        path.quadTo(rect.topLeft(), rect.topLeft());
        path.lineTo(rect.bottomLeft());
        path.quadTo(rect.bottomLeft(), rect.bottomLeft());
        path.lineTo(rect.bottomRight());
        path.quadTo(rect.bottomRight(), rect.bottomRight());
        path.lineTo(rect.topRight());
        path.quadTo(rect.topRight(), rect.topRight());

        if (option.state.testFlag(QStyle::State_MouseOver)) {
            painter->setPen(QPen(QColor("#ebeced")));
            painter->setBrush(QColor("#ebeced"));
            painter->drawPath(path);
        }
        if (option.state.testFlag(QStyle::State_Selected)) {
            painter->setPen(QPen(QColor("#e3e3e5")));
            painter->setBrush(QColor("#e3e3e5"));
            painter->drawPath(path);
        }

        QRectF iconRect = QRect(rect.left()+5,rect.top()+5,100,100);
        QRectF nameRect = QRect(iconRect.right()+5, iconRect.top(), rect.width()-10-iconRect.width(), 20);

        painter->drawImage(iconRect, QImage(itemData.firstFrame));
        painter->setPen(QPen(Qt::black));
        painter->setFont(QFont("Microsoft Yahei",10));
        painter->drawText(nameRect, itemData.name);

        painter->restore();

    }
}

QSize VideoItemDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(index);
    return QSize(option.rect.width(), 300);
}
