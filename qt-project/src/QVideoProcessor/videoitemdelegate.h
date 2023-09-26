#ifndef VIDEOITEMDELEGATE_H
#define VIDEOITEMDELEGATE_H

#include <QStyledItemDelegate>
#include <QPainter>
#include <QSize>

/**
 * @brief The VideoItemDelegate class
 * Responsible for drawing every item in the Material Library
 */
class VideoItemDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    VideoItemDelegate(QObject *parent = nullptr);

    void paint(QPainter *painter, const QStyleOptionViewItem &option,
               const QModelIndex &index) const Q_DECL_OVERRIDE;
    QSize sizeHint(const QStyleOptionViewItem &option,
                   const QModelIndex &index) const Q_DECL_OVERRIDE;

};

#endif // VIDEOITEMDELEGATE_H
