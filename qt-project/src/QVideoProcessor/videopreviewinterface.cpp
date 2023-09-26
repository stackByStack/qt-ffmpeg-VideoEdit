#include "videopreviewinterface.h"
#include <QPainter>
#include <QPoint>

VideoPreviewInterface::VideoPreviewInterface(QWidget* parent, QStandardItemModel* textDataModel, QStandardItemModel* ImageDataModel, QStandardItemModel* VideoDataModel): QVideoWidget(parent), parent(parent),
    textDataModel(textDataModel), ImageDataModel(ImageDataModel), VideoDataModel(VideoDataModel)
{

}
