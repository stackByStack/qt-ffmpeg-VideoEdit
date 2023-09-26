#ifndef TIMEAXISITEMVIEWWIDGET_H
#define TIMEAXISITEMVIEWWIDGET_H

#include <QWidget>
#include <QStandardItem>
#include <QStandardItemModel>
#include <QLabel>
//#include "TimeAxisItem.h"
/**
 * @brief The TimeAxisItemViewWidget class
 * Responsible For Drawing the View of Each item on Time Axis
 */
class TimeAxisItemViewWidget : public QObject
{
    Q_OBJECT
    Q_PROPERTY(qint64 OverallDuration READ getOverallDuration WRITE setOverallDuration)
public:
    explicit TimeAxisItemViewWidget(QWidget *parent, QStandardItemModel*);

    QStandardItemModel *itemDataModel;
    QImage OriginImage;
    //QLabel* label;

    //void paintEvent(QPaintEvent*) override;
    void updateView(QLabel*);
    qint64 getOverallDuration();
    void setOverallDuration(qint64);
private:
    qint64 OverallDuration;
    QStringList ColorList;
    int nowColor;
};

#endif // TIMEAXISITEMVIEWWIDGET_H
