#ifndef VIDEOPREVIEWINTERFACE_H
#define VIDEOPREVIEWINTERFACE_H

#include <QVideoWidget>
#include <QStandardItemModel>
/**
 * @brief The VideoPreviewInterface class
 * Responsible for storing the material added to the TimeAxis Video.
 */
class VideoPreviewInterface : public QVideoWidget
{
    Q_OBJECT
public:
    VideoPreviewInterface(QWidget* parent, QStandardItemModel* textDataModel, QStandardItemModel* ImageDataModel, QStandardItemModel* VideoDataModel);

    //void paintEvent(QPaintEvent*) Q_DECL_OVERRIDE;

    QString nowPlayedVideoSrcPath;
    QString tmpSavedVideoSrcPath;

private:
    QWidget* parent;
    QStandardItemModel* textDataModel;
    QStandardItemModel* ImageDataModel;
    QStandardItemModel* VideoDataModel;

};

#endif // VIDEOPREVIEWINTERFACE_H
