#include "TimeAxisItem.h"
#include "timeaxisitemviewwidget.h"
#include <QPainter>
#include <QImage>
#include <QBrush>
#include <QColor>
#include <QRectF>

TimeAxisItemViewWidget::TimeAxisItemViewWidget(QWidget *parent, QStandardItemModel* model)
    : itemDataModel(model)
{
    Q_UNUSED(parent);
    OverallDuration = 0;
    nowColor = 0;

    ColorList.append("#8b4513");
    ColorList.append("#778899");
    ColorList.append("#4169e1");
    ColorList.append("#de3163");
    ColorList.append("#8ce600");
    ColorList.append("#6633cc");
    ColorList.append("#704214");
    ColorList.append("#ffd700");
    ColorList.append("#73e68c");
    ColorList.append("#b85798");
}


void TimeAxisItemViewWidget::updateView(QLabel* label)
{
    int rows = itemDataModel->rowCount();
    int columns = itemDataModel->columnCount();
    qreal width1 = label->width();
    qreal height1 = label->height();

    QImage image(width1,height1,QImage::Format_RGBA8888);
    image.fill(QColor(Qt::white));

    QPainter* painter = new QPainter(&image);

    for(int i = 0; i < rows; i++)
    {
        for(int j = 0; j < columns; j++)
        {
            //painter->setPen(QPen(QBrush(QColor(ColorList[nowColor])),40));

            QStandardItem* item = itemDataModel->item(i,j);
            if(item)
            {
                QVariant rawData = item->data();
                timeAxisItem data = rawData.value<timeAxisItem>();

                qreal aleft = width1 * (qreal)data.qStartTime/(qreal)OverallDuration;
                qreal atop = 0;
                qreal aheight = height1;
                qreal aWidth = width1 * (qreal)(data.qEndTime-data.qStartTime)/(qreal)OverallDuration;

                QRectF rect(aleft, atop, aWidth, aheight);
                QColor color(ColorList[nowColor]);
                nowColor++;
                nowColor%=10;
                color.setAlpha(50);
                painter->setPen(QPen(QBrush(color),100));
                painter->drawRect(rect);

                painter->setPen(QPen(Qt::black));
                painter->setFont(QFont("Microsoft Yahei", 10));
                painter->drawText(rect,data.value);

            }

        }
    }
    OriginImage = image;
    label->setPixmap(QPixmap::fromImage(image));
    delete painter;
}

qint64 TimeAxisItemViewWidget::getOverallDuration()
{
    return OverallDuration;
}

void TimeAxisItemViewWidget::setOverallDuration(qint64 val)
{
    OverallDuration = val;
}
