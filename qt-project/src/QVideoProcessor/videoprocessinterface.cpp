#include "videoprocessinterface.h"
#include <QVideoSink>
#include <QVideoFrame>
#include <QPainter>
#include <QAudioDevice>

VideoProcessInterface::VideoProcessInterface(QWidget* parent): parent(parent)
{
    //init
    VideoPreviewPlayer = new QMediaPlayer(parent);
    audioOutput = new QAudioOutput(parent);
    textDataModel = new QStandardItemModel(parent);
    ImageDataModel = new QStandardItemModel(parent);
    VideoDataModel = new QStandardItemModel(parent);
    processedVideoDuration = 0;
    backend_VideoProcessWidget = new VideoPreviewInterface(parent, textDataModel, ImageDataModel, VideoDataModel);

    TimeAxisVideoView = new TimeAxisItemViewWidget(parent,VideoDataModel);
    TimeAxisImageView = new TimeAxisItemViewWidget(parent, ImageDataModel);
    TimeAxisTextView = new TimeAxisItemViewWidget(parent, textDataModel);

    nowVideoDuration = 0;

    connect(this, &VideoProcessInterface::processedVideoDurationChanged, this, [=](qint64 val){
       TimeAxisVideoView->setOverallDuration(val);
       TimeAxisImageView->setOverallDuration(val);
       TimeAxisTextView->setOverallDuration(val);
    });

    VideoPreviewPlayer->setVideoOutput(backend_VideoProcessWidget);
    VideoPreviewPlayer->setAudioOutput(audioOutput);
    audioOutput->setVolume(0.2);

    //init video sink
    backend_videoSink = backend_VideoProcessWidget->videoSink();

}

qint64 VideoProcessInterface::getProcessedVideoDuration()
{
    return processedVideoDuration;
}

void VideoProcessInterface::setProcessedVideoDuration(qint64 num)
{
    processedVideoDuration = num;
    emit processedVideoDurationChanged(num);
}



