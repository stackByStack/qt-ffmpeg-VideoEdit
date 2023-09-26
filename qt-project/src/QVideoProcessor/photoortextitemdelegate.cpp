#include "photoortextitemdelegate.h"
#include <QPainter>
#include "PhotoOrTextItem.h"
#include <QRectF>
#include <QRect>
#include <QPainterPath>
#include <QPixmap>
#include <QImage>
#include <QPen>
#include <QFont>

photoOrTextItemDelegate::photoOrTextItemDelegate(QObject *parent) : QStyledItemDelegate(parent)
{

}

void photoOrTextItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if(index.isValid())
    {
        painter->save();
        QVariant var = index.data(Qt::UserRole + 1);
        photoOrTextItem itemData = var.value<photoOrTextItem>();

        QRectF rect;
        rect.setX(option.rect.x());
        rect.setY(option.rect.y());
        rect.setWidth(option.rect.width()-1);
        rect.setHeight(option.rect.height()-1);
        //if the focus is on the item, then the item will be painted gray.
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

        QRectF iconRect = QRect(rect.left()+5,rect.top()+5,200,200);
        QRectF nameRect = QRect(iconRect.right()+5, iconRect.top(), rect.width()-10-iconRect.width(), 20);

        painter->drawImage(iconRect, QImage(itemData.srcPath));
        painter->setPen(QPen(Qt::black));
        painter->setFont(QFont("Microsoft Yahei",10));
        painter->drawText(nameRect, itemData.name);

        painter->restore();


    }
}

QSize photoOrTextItemDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(index);
    return QSize(option.rect.width(), 300);
}
