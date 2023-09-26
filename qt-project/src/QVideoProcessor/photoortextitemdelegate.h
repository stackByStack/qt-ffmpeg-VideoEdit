#ifndef PHOTOORTEXTITEMDELEGATE_H
#define PHOTOORTEXTITEMDELEGATE_H

#include <QStyledItemDelegate>
#include <QPainter>
#include <QSize>
/**
 * @brief The photoOrTextItemDelegate class
 * Responsible for drawing every item in the Material Library
 */
class photoOrTextItemDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    photoOrTextItemDelegate(QObject *parent = nullptr);
    void paint(QPainter *painter, const QStyleOptionViewItem &option,
               const QModelIndex &index) const Q_DECL_OVERRIDE;
    QSize sizeHint(const QStyleOptionViewItem &option,
                   const QModelIndex &index) const Q_DECL_OVERRIDE;

};

#endif // PHOTOORTEXTITEMDELEGATE_H
