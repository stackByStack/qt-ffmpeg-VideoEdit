#ifndef FFMPEGCOMMANDINTERFACE_H
#define FFMPEGCOMMANDINTERFACE_H

//#include "mainwindow.h"

#include <QCoreApplication>
#include <QProcess>
#include <QStandardItemModel>

/**
 * @brief The FfmpegCommandInterface class
 * Responsible for process the command line request supported by Ffmpeg
 */
class FfmpegCommandInterface: public QObject
{
    Q_OBJECT
public:
    FfmpegCommandInterface(QWidget* parent);
    qint64 getVideoDuration(QString video);
    qint64 getNowTimeStamp();
    QString addVideoIntoPipeline(QString srcPath, QString tmpSavedPipelineVideo);
    void outputVideo(QString tmpSavedVideo, QStandardItemModel* text, QStandardItemModel* image, QStandardItemModel* video);
    QString addSubtitlesToVideo(QString nowSavedVideoPath, QString subtitleFilePath);
    QString tmpSavedVideoFormat;
    void deleteAllTmpVideo();
    void CropVideo(QString srcPath, QString stHour,QString stMin, QString stSec, QString etHour,QString etMin, QString etSec, QString targetPath);
private:
    QWidget* parent;
    QString appDir;
    QString ffmpegPath;
};

#endif // FFMPEGCOMMANDINTERFACE_H
