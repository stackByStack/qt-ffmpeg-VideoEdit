#include "ffmpegcommandinterface.h"
#include "TimeAxisItem.h"
#include <QDateTime>
#include <QWidget>
#include <QTextEdit>
#include <QLayout>
#include <QFileDialog>
#include <QLabel>
#include <QButtonGroup>
#include <QCheckBox>
#include <QPushButton>


FfmpegCommandInterface::FfmpegCommandInterface(QWidget* parent): parent(parent),appDir(QCoreApplication::applicationDirPath())
{
    ffmpegPath = QCoreApplication::applicationDirPath()+"/ffmpeg-5.1-full_build-shared/bin/ffmpeg.exe";
    tmpSavedVideoFormat = ".mp4";
}
/**
 * @brief FfmpegCommandInterface::getVideoDuration
 * @param video: Path of video
 * @return qint64 Duration of the video
 */
qint64 FfmpegCommandInterface::getVideoDuration(QString video)
{
    if(video == "")
    {
        return 0;
    }
    QProcess process;
    QStringList list;
    list<<"-v"<<"error";
    list<<"-select_streams"<<"v:0";
    list<<"-show_entries"<<"stream=duration";
    list<<"-of"<<"default=noprint_wrappers=1:nokey=1";
    list<<video;
    process.start(appDir + "/ffmpeg-5.1-full_build-shared/bin/ffprobe.exe", list);

    process.waitForFinished();
    process.waitForReadyRead();

    //Read the duration data
    QByteArray rawData = process.readAll();
    QString tmp = rawData;
    tmp = tmp.mid(0, tmp.size()-1-1);
    qint64 res = tmp.toDouble() * 1000;
    return res;
}
/**
 * @brief FfmpegCommandInterface::getNowTimeStamp
 * @return Unique Time Stamp To Mark the File
 */
qint64 FfmpegCommandInterface::getNowTimeStamp()
{
    return QDateTime::currentMSecsSinceEpoch();
}
/**
 * @brief FfmpegCommandInterface::addVideoIntoPipeline
 * @param srcPath
 * @param tmpSavedPipelineVideo
 * @return new tmpSavedPipelineVideo Path
 */
QString FfmpegCommandInterface::addVideoIntoPipeline(QString srcPath, QString tmpSavedPipelineVideo)
{
    if(tmpSavedPipelineVideo == "")
    {
        QString tmpSavedPath = QCoreApplication::applicationDirPath() + "/tmp" + "/" + "tmp_" + QString::number(getNowTimeStamp()) + tmpSavedVideoFormat;
        QStringList list;
        list<<"-i"<<srcPath;
        list<<"-filter_complex";
        list<<"[0:0][0:1] concat=n=1:v=1:a=1[v][a]";
        list<<"-map"<<"[v]";
        list<<"-map"<<"[a]";
        list<<tmpSavedPath;

        QProcess* process = new QProcess;
        //SHow the Process
        QWidget* widget = new QWidget();
        QTextEdit* edit = new QTextEdit();
        edit->setReadOnly(true);
        QHBoxLayout* lay = new QHBoxLayout();
        lay->addWidget(edit);
        widget->setLayout(lay);
        process->setReadChannel(QProcess::StandardOutput);
        process->setProcessChannelMode(QProcess::MergedChannels);
        connect(process, &QProcess::readyRead, this, [=](){
            widget->show();
            edit->append(process->readAll());
        });
        connect(process, &QProcess::finished, this, [=](){
            widget->close();
            //viewWidget->setOverallDuration(getVideoDuration(tmpSavedPath));
            delete process;
            delete widget;
        });
        qDebug()<<list;

        process->start(QCoreApplication::applicationDirPath()+"/ffmpeg-5.1-full_build-shared/bin/ffmpeg.exe",list);

        return tmpSavedPath;
    }
    else
    {
        QString tmpSavedPath = QCoreApplication::applicationDirPath() + "/tmp" + "/" + "tmp_" + QString::number(getNowTimeStamp()) + ".mp4";
        QStringList list;
        list<<"-i"<<tmpSavedPipelineVideo;
        list<<"-i"<<srcPath;
        list<<"-filter_complex";
        list<<"[0:0][0:1] [1:0][1:1] concat=n=2:v=1:a=1[v][a]";
        list<<"-map"<<"[v]";
        list<<"-map"<<"[a]";
        list<<tmpSavedPath;

        QProcess* process = new QProcess;
        //Show the process
        QWidget* widget = new QWidget();
        QTextEdit* edit = new QTextEdit();
        edit->setReadOnly(true);
        QHBoxLayout* lay = new QHBoxLayout();
        lay->addWidget(edit);
        widget->setLayout(lay);
        process->setReadChannel(QProcess::StandardOutput);
        process->setProcessChannelMode(QProcess::MergedChannels);
        connect(process, &QProcess::readyRead, this, [=](){
            widget->show();
            edit->append(process->readAll());
        });
        connect(process, &QProcess::finished, this, [=](){
            widget->close();
            delete process;
            delete widget;
        });
        //qDebug()<<list;
        process->start(QCoreApplication::applicationDirPath()+"/ffmpeg-5.1-full_build-shared/bin/ffmpeg.exe",list);

        return tmpSavedPath;
    }
}

/**
 * @brief FfmpegCommandInterface::outputVideo
 * @param tmpSavedVideo
 * @param textDataModel
 * @param imageDataModel
 * @param videoDataModel
 * integrate the data into the video and output the video.
 */
void FfmpegCommandInterface::outputVideo(QString tmpSavedVideo, QStandardItemModel* textDataModel, QStandardItemModel* imageDataModel, QStandardItemModel* videoDataModel)
{
    Q_UNUSED(videoDataModel);

    QString outputVideoPath = QFileDialog::getSaveFileName(parent, "选择保存地址", QCoreApplication::applicationDirPath(), "Video Files (*.mp4 *.mkv)");
    if(outputVideoPath == "")
    {
        return;
    }
    //Layout for attaining the detail data.
    QWidget* widget1 = new QWidget;
    QLabel* label1 = new QLabel("分辨率", widget1);
    QLabel* label2 = new QLabel("码率", widget1);
    QButtonGroup* group1 = new QButtonGroup(widget1);
    QButtonGroup* group2 = new QButtonGroup(widget1);
    QCheckBox* box1 = new QCheckBox("1080P 1920*1080");
    box1->setChecked(true);
    QCheckBox* box2 = new QCheckBox("720P 1280*720");
    QCheckBox* box3 = new QCheckBox("480P 640*480");
    QCheckBox* box4 = new QCheckBox("500Kbps");
    box4->setChecked(true);
    QCheckBox* box5 = new QCheckBox("2Mbps");
    QCheckBox* box6 = new QCheckBox("5Mbps");
    QPushButton* YesButton = new QPushButton("确认", widget1);

    group1->addButton(box1);
    group1->addButton(box2);
    group1->addButton(box3);
    group2->addButton(box4);
    group2->addButton(box5);
    group2->addButton(box6);
    group1->setExclusive(true);
    group2->setExclusive(true);

    QHBoxLayout* layout1 = new QHBoxLayout();
    QHBoxLayout* layout2 = new QHBoxLayout();
    QVBoxLayout* left = new QVBoxLayout();
    QHBoxLayout* global = new QHBoxLayout();

    layout1->addWidget(label1);
    layout1->addWidget(box1);
    layout1->addWidget(box2);
    layout1->addWidget(box3);
    layout2->addWidget(label2);
    layout2->addWidget(box4);
    layout2->addWidget(box5);
    layout2->addWidget(box6);

    left->addLayout(layout1);
    left->addLayout(layout2);

    global->addLayout(left);
    global->addWidget(YesButton);
    widget1->setLayout(global);

    connect(YesButton, &QPushButton::clicked, this, [=](){
        QString videoSize;
        QString vrate;
        QString bufsize;
        QString maxrate;
        widget1->close();

        if(box1->isChecked())
        {
            videoSize = "1920*1080";
        }
        else if(box2->isChecked())
        {
            videoSize = "1280*720";
        }
        else if(box3->isChecked())
        {
            videoSize = "640*480";
        }

        if(box4->isChecked())
        {
            vrate = "500k";
            bufsize = "500k";
            maxrate = "1000k";
        }
        else if(box5->isChecked())
        {
            vrate = "2000k";
            bufsize = "2000k";
            maxrate = "2500k";
        }
        else if(box6->isChecked())
        {
            vrate = "5000k";
            bufsize = "5000k";
            maxrate = "6000k";
        }

        QStringList list;
        list<<"-i"<<tmpSavedVideo;

        //image

        qint64 durationSecs = getVideoDuration(tmpSavedVideo) / 1000;

        int imageRowsNum = imageDataModel->rowCount();
        int imageColsNum = imageDataModel->columnCount();

        for(int i=0;i<imageRowsNum;i++)
        {
            for(int j=0;j<imageColsNum;j++)
            {
                QStandardItem* aItem = imageDataModel->item(i,j);
                QVariant rawData = aItem->data();
                timeAxisItem data = rawData.value<timeAxisItem>();
                list<<"-loop"<<"1"<<"-t"<<QString::number(durationSecs)<<"-i"<<data.srcPath;
            }
        }

        //stream
        //image
        list<<"-filter_complex";

        QString nowStream = "[0:v]";
        QString filter_complex;

        for(int i=0;i<imageRowsNum;i++)
        {
            QStandardItem* aItem = imageDataModel->item(i,0);
            QVariant rawData = aItem->data();
            timeAxisItem data = rawData.value<timeAxisItem>();

            QString tmp1 = QString("[%1:v]fade=t=in:st=%2:d=3:alpha=1[v_i_%1_1]; [v_i_%1_1]fade=t=out:st=%3:d=3:alpha=1[v_i_%1_2]; ").arg(i+1).arg(data.qStartTime/1000).arg(data.qEndTime/1000);
            QString tmp2 = nowStream + QString("[v_i_%1_2]overlay=x=%2:y=%3[out_i_%1]; ").arg(i+1).arg(data.x).arg(data.y);
            if(data.TranslationEnabled)
            {
                tmp2 = nowStream + QString("[v_i_%1_2]overlay=x='if(gte(t,%2),if(lte(t,%3),if(lte(t,%2+3),(t-%2)*%4/3,%4),NAN),NAN)':y='if(gte(t,%2),if(lte(t,%3),%5,NAN),NAN)'[out_i_%1]; ").arg(i+1).arg(data.qStartTime/1000).arg(data.qEndTime/1000+3).arg(data.x).arg(data.y);
            }

            filter_complex += tmp1 + tmp2;

            nowStream = QString("[out_i_%1]").arg(i+1);
        }



        //text
        int textRowsNum = textDataModel->rowCount();
        //int textColsNum = textDataModel->columnCount();

        for(int i=0;i<textRowsNum;i++)
        {
            QStandardItem* aItem = textDataModel->item(i,0);
            QVariant rawData = aItem->data();
            timeAxisItem data = rawData.value<timeAxisItem>();

            QString tmp1 = nowStream + QString("drawtext=fontsize=50:fontfile=Dengl.ttf:text='%1':x=%2:y=%3:enable='between(t,%4,%5)'").arg(data.value).arg(data.x).arg(data.y).arg(data.qStartTime/1000).arg(data.qEndTime/1000);
            if(data.TranslationEnabled)
            {
                tmp1 = nowStream + QString("drawtext=fontsize=50:fontfile=Dengl.ttf:text='%1':x='if(gte(t,%4),if(lte(t,%5),if(lte(t,%4+3),(t-%4)*%2/3,%2),NAN),NAN)':y=%3:enable='between(t,%4,%5)'").arg(data.value).arg(data.x).arg(data.y).arg(data.qStartTime/1000).arg(data.qEndTime/1000);
            }
            if(data.FadeEnabled)
            {
                tmp1 += QString(":alpha='if(lt(t,%1),0,if(lt(t,%1+3),(t-%1)/3,if(lt(t,%2-3),1,if(lt(t,%2),(3-(t-%2+3))/3,0))))'").arg(data.qStartTime/1000).arg(data.qEndTime/1000);
            }

            tmp1 += QString("[out_t_%1]").arg(i+1) + "; ";
            filter_complex += tmp1;

//            qDebug()<<tmp1;
//            qDebug()<<filter_complex;

            nowStream = QString("[out_t_%1]").arg(i+1);
        }

        QString tmp3 = nowStream + "[0:a]concat=n=1:v=1:a=1[v][a]";
        filter_complex += tmp3;
        list<<filter_complex;

        list<<"-map"<<"[v]";
        list<<"-map"<<"[a]";

        list<<"-s"<<videoSize;

        list<<"-b:v"<<vrate;
        list<<"-bufsize"<<bufsize;
        list<<"-maxrate"<<maxrate;

        list<<outputVideoPath;

        //execute
        QProcess* process = new QProcess;
        //Show the process
        QWidget* widget = new QWidget();
        QTextEdit* edit = new QTextEdit();
        edit->setReadOnly(true);
        QHBoxLayout* lay = new QHBoxLayout();
        lay->addWidget(edit);
        widget->setLayout(lay);
        process->setReadChannel(QProcess::StandardOutput);
        process->setProcessChannelMode(QProcess::MergedChannels);
        connect(process, &QProcess::readyRead, this, [=](){
            widget->show();
            edit->append(process->readAll());
        });
        connect(process, &QProcess::finished, this, [=](){
            widget->close();
            //viewWidget->setOverallDuration(getVideoDuration(tmpSavedPath));
            delete process;
            delete widget;
        });

        //qDebug()<<list;
        process->start(QCoreApplication::applicationDirPath()+"/ffmpeg-5.1-full_build-shared/bin/ffmpeg.exe",list);

    });

    widget1->show();
}

/**
 * @brief FfmpegCommandInterface::addSubtitlesToVideo :
 * integrate the subtitle into the TimeAxis temporary-saved video, and replace it with the new video.
 * @param nowSavedVideoPath
 * @param subtitleFilePath
 * @return new tmpSavedVideoPath
 */
QString FfmpegCommandInterface::addSubtitlesToVideo(QString nowSavedVideoPath, QString subtitleFilePath)
{
    QStringList list;
    list<<"-i"<<nowSavedVideoPath;
    list<<"-vf"<<"subtitles=\\'"+subtitleFilePath+"\\ '";

    QString tmpSavedPath = QCoreApplication::applicationDirPath() + "/tmp" + "/" + "tmp_" + QString::number(getNowTimeStamp()) + tmpSavedVideoFormat;
    list<<tmpSavedPath;
    //qDebug()<<list;

    QProcess* process = new QProcess;

    QWidget* widget = new QWidget();
    QTextEdit* edit = new QTextEdit();
    edit->setReadOnly(true);
    QHBoxLayout* lay = new QHBoxLayout();
    lay->addWidget(edit);
    widget->setLayout(lay);
    process->setReadChannel(QProcess::StandardOutput);
    process->setProcessChannelMode(QProcess::MergedChannels);
    connect(process, &QProcess::readyRead, this, [=](){
        widget->show();
        edit->append(process->readAll());
    });
    connect(process, &QProcess::finished, this, [=](){
        widget->close();
        //viewWidget->setOverallDuration(getVideoDuration(tmpSavedPath));
        delete process;
        delete widget;
    });

    process->start(QCoreApplication::applicationDirPath()+"/ffmpeg-5.1-full_build-shared/bin/ffmpeg.exe",list);

    return tmpSavedPath;
}
/**
 * @brief FfmpegCommandInterface::CropVideo:
 * crop video from the rules specified by parameters, and save it in a location specified by the user
 * @param srcPath
 * @param stHour
 * @param stMin
 * @param stSec
 * @param etHour
 * @param etMin
 * @param etSec
 * @param targetPath
 */
void FfmpegCommandInterface::CropVideo(QString srcPath, QString stHour,QString stMin, QString stSec, QString etHour,QString etMin, QString etSec, QString targetPath)
{
    qint64 startT = (stHour.toInt() * 3600 + stMin.toInt() * 60 + stSec.toInt());
    qint64 endT = (etHour.toInt() * 3600 + etMin.toInt() * 60 + etSec.toInt());
    qint64 duration = endT - startT;

    QStringList list;
    list<<"-i"<<srcPath;
    list<<"-ss"<<QString::number(startT);
    list<<"-t"<<QString::number(duration);
    list<<targetPath;

    QProcess* process = new QProcess;

    QWidget* widget = new QWidget();
    QTextEdit* edit = new QTextEdit();
    edit->setReadOnly(true);
    QHBoxLayout* lay = new QHBoxLayout();
    lay->addWidget(edit);
    widget->setLayout(lay);
    process->setReadChannel(QProcess::StandardOutput);
    process->setProcessChannelMode(QProcess::MergedChannels);
    connect(process, &QProcess::readyRead, this, [=](){
        widget->show();
        edit->append(process->readAll());
    });
    connect(process, &QProcess::finished, this, [=](){
        widget->close();
        //viewWidget->setOverallDuration(getVideoDuration(tmpSavedPath));
        delete process;
        delete widget;
    });

    process->start(QCoreApplication::applicationDirPath()+"/ffmpeg-5.1-full_build-shared/bin/ffmpeg.exe",list);

}

//Unfinished Func
void FfmpegCommandInterface::deleteAllTmpVideo()
{
    QProcess* process = new QProcess;
    QString path = QDir::toNativeSeparators(QCoreApplication::applicationDirPath() + "/tmp" + "/*.mp4");

    process->execute("del " + path);
    process->waitForStarted();
    process->waitForFinished();
}
