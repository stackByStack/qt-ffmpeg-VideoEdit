#ifndef VIDEOPROCESSINTERFACE_H
#define VIDEOPROCESSINTERFACE_H

#include "videopreviewinterface.h"
#include "timeaxisitemviewwidget.h"


#include <QVideoWidget>
#include <QMediaPlayer>
#include <QAudioOutput>
#include <QVideoSink>

/**
 * @brief The VideoProcessInterface class
 * Responsible to process the original video
 */
class VideoProcessInterface: public QObject
{
    Q_OBJECT
    Q_PROPERTY(qint64 processedVideoDuration READ getProcessedVideoDuration WRITE setProcessedVideoDuration NOTIFY processedVideoDurationChanged)
public:
    VideoProcessInterface(QWidget* parent);
    QMediaPlayer* VideoPreviewPlayer;
    //QMediaPlayer* backend_VideoProcess;

    QAudioOutput* audioOutput;

    //QVideoWidget* videoPreviewWidget;
    VideoPreviewInterface* backend_VideoProcessWidget;

    //QVideoSink* videoPreviewSink;
    QVideoSink* backend_videoSink;

    QStandardItemModel* textDataModel;
    QStandardItemModel* ImageDataModel;
    QStandardItemModel* VideoDataModel;

    QString nowPlayedVideoSrcPath;
    QString tmpSavedVideoSrcPath;

    qint64 nowVideoDuration;

    qint64 getProcessedVideoDuration();
    void setProcessedVideoDuration(qint64);
    TimeAxisItemViewWidget* TimeAxisVideoView;
    TimeAxisItemViewWidget* TimeAxisTextView;
    TimeAxisItemViewWidget* TimeAxisImageView;



    //void updateTimeAxisView();

private:
    qint64 processedVideoDuration;
    QWidget* parent;

public: signals:
    void processedVideoDurationChanged(qint64);

};

#endif // VIDEOPROCESSINTERFACE_H
