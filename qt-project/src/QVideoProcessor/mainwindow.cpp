#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "VideoItem.h"
#include "PhotoOrTextItem.h"
#include "TimeAxisItem.h"
#include <QSlider>
#include <QTimer>
#include <QLabel>
#include <QLineEdit>
#include <QValidator>
#include <QCheckBox>
#include <QButtonGroup>
#include <QSignalMapper>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    //init
    ui->setupUi(this);
    centralWidget = new QWidget(this);

    globalLayout = new QVBoxLayout ;
    WorkZoneLayout = new QHBoxLayout;
    MaterialLibLayout = new QVBoxLayout ;
    VideoProcessLayout = new QVBoxLayout ;

    commandLine = new FfmpegCommandInterface(this);

    videoProcessionInterface = new VideoProcessInterface(this);
    videoProcessionInterface->backend_VideoProcessWidget->setVisible(false);

    FileManage = new MaterialLibItemManage(this);

    VideoSelection = FileManage->VideoItemView->selectionModel();
    PhotoOrTextSelection = FileManage->PhotoOrTextItemView->selectionModel();

    MainMenuBar = new QMenuBar(this);
    videoPreviewWidget = new QLabel(this,Qt::Widget);
    videoTimeAxis = new QLabel(this, Qt::Widget);
    videoTimeAxis->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    videoTimeAxis->setScaledContents(true);
    textTimeAxis = new QLabel(this, Qt::Widget);
    textTimeAxis->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    textTimeAxis->setScaledContents(true);
    imageTimeAxis = new QLabel(this, Qt::Widget);
    imageTimeAxis->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    imageTimeAxis->setScaledContents(true);
//    videoProcessionInterface->TimeAxisVideoView->label = videoTimeAxis;
//    videoProcessionInterface->TimeAxisImageView->label = imageTimeAxis;
//    videoProcessionInterface->TimeAxisTextView->label = textTimeAxis;

    initMainMenuBar();
    initVideoPreviewWidget();
    initVideoPreviewProcess();
    initTimeAxis();


//    videoProcessionInterface->updateTimeAxisView();
    //Track the selected Video
    connect(VideoSelection, &QItemSelectionModel::currentChanged, this, [=](const QModelIndex &current, const QModelIndex &previous){
        Q_UNUSED(previous);
        if(current.isValid())
        {
            currentVideoIndex = current;
            QStandardItem* aItem = FileManage->VideoItemModel->itemFromIndex(current);
            QVariant rawData = aItem->data();
            VideoItem data = rawData.value<VideoItem>();
            videoProcessionInterface->VideoPreviewPlayer->setSource(data.srcPath);
            videoProcessionInterface->nowPlayedVideoSrcPath = data.srcPath;
            videoProcessionInterface->nowVideoDuration = commandLine->getVideoDuration(data.srcPath);
            videoProcessionInterface->VideoPreviewPlayer->play();
            QTimer time(this);
            time.setInterval(1000);
            videoProcessionInterface->VideoPreviewPlayer->pause();
        }
    });
    //Track the selected photo or text
    connect(PhotoOrTextSelection, &QItemSelectionModel::currentChanged, this, [=](const QModelIndex &current, const QModelIndex &previous){
        Q_UNUSED(previous);
        if(current.isValid())
        {
            currentPhotoOrTextIndex = current;
        }
    });
    //Track the changed position of video Player and draw the pointer following the player's procession
    connect(videoProcessionInterface->VideoPreviewPlayer, &QMediaPlayer::positionChanged, this, [=](qint64 pos){
        QImage image1 = videoProcessionInterface->TimeAxisTextView->OriginImage;
        QColor color(Qt::red);
        color.setAlpha(255);
        QPainter* painter1 = new QPainter(&image1);
        painter1->setPen(QPen(color, 1));
        qreal x = (qreal)image1.width()*(qreal)pos/(qreal)videoProcessionInterface->nowVideoDuration;
        painter1->drawLine(x, 0, x, image1.height());
        textTimeAxis->setPixmap(QPixmap::fromImage(image1));
        delete painter1;

        QImage image2 = videoProcessionInterface->TimeAxisImageView->OriginImage;
        QPainter* painter2 = new QPainter(&image2);
        painter2->setPen(QPen(color, 1));
        x = (qreal)image2.width()*(qreal)pos/(qreal)videoProcessionInterface->nowVideoDuration;
        painter2->drawLine(x, 0, x, image2.height());
        imageTimeAxis->setPixmap(QPixmap::fromImage(image2));
        delete painter2;

        QImage image3 = videoProcessionInterface->TimeAxisVideoView->OriginImage;
        QPainter* painter3 = new QPainter(&image3);
        painter3->setPen(QPen(color, 1));
        x = (qreal)image3.width()*(qreal)pos/(qreal)videoProcessionInterface->nowVideoDuration;
        painter3->drawLine(x, 0, x, image3.height());
        videoTimeAxis->setPixmap(QPixmap::fromImage(image3));
        delete painter3;
    });

    MaterialLibLayout->addWidget(FileManage->PhotoOrTextItemView, 5);
    MaterialLibLayout->addWidget(FileManage->VideoItemView, 5);

    WorkZoneLayout->addLayout(MaterialLibLayout,3);
    WorkZoneLayout->addLayout(VideoProcessLayout,7);

    globalLayout->addWidget(MainMenuBar);
    globalLayout->addLayout(WorkZoneLayout);

    centralWidget->setLayout(globalLayout);
    setCentralWidget(centralWidget);

    videoProcessionInterface->TimeAxisVideoView->updateView(videoTimeAxis);
    videoProcessionInterface->TimeAxisTextView->updateView(textTimeAxis);
    videoProcessionInterface->TimeAxisImageView->updateView(imageTimeAxis);

}

void MainWindow::initMainMenuBar()
{
    //init the menu items
    QMenu* menu1 = new QMenu("文件", this);
    QAction* VideoInput = new QAction("导入视频", this);
    QAction* ImageInput = new QAction("导入图片", this);
    QAction* TextInput = new QAction("导入文字", this);
    QAction* deleteTextOrImage = new QAction("删除素材库元素", this);
    QAction* deleteVideo = new QAction("删除视频", this);
    QAction* VideoOutput = new QAction("导出视频", this);

    QMenu* menu2 = new QMenu("时间轴", this);
    QAction* VideoToPipeline = new QAction("加入视频", this);
    QAction* ImageOrTextToPipeline = new QAction("加入图片/文字", this);
    QAction* previewPipeline = new QAction("预览", this);

    QMenu* menu3 = new QMenu("格式", this);
    QAction* tmpTimeAxisVideoFormat = new QAction("时间轴视频缓存格式设置",this);

    QMenu* menu4 = new QMenu("字幕", this);
    QAction* subtitleAddedToPipeline = new QAction("挂载字幕到流水线", this);

    QMenu* menu5 = new QMenu("裁剪");
    QAction* CropOutputToVideoList = new QAction("导出到视频列表", this);

    //init the function of each QAction

    //Add Video into Material Library
    connect(VideoInput, &QAction::triggered, this, [=](){
            videoProcessionInterface->TimeAxisVideoView->updateView(videoTimeAxis);
            videoProcessionInterface->TimeAxisTextView->updateView(textTimeAxis);
            videoProcessionInterface->TimeAxisImageView->updateView(imageTimeAxis);

            FileManage->addVideoItem();
    });
    //Add Image into Material Library
    connect(ImageInput, &QAction::triggered, FileManage, &MaterialLibItemManage::addImageItem);
    //Add Text into Material Library
    connect(TextInput, &QAction::triggered, FileManage, &MaterialLibItemManage::addTextItem);
    //Delete the specified Item in the Library
    connect(deleteTextOrImage, &QAction::triggered, this, [=](){
        if(currentPhotoOrTextIndex.isValid())
        {
            FileManage->PhotoOrTextItemModel->removeRow(currentPhotoOrTextIndex.row());
            currentPhotoOrTextIndex = QModelIndex();
        }
    });
    //Delete the specified Item in the Library
    connect(deleteVideo, &QAction::triggered, this, [=](){
        if(currentVideoIndex.isValid())
        {
            FileManage->VideoItemModel->removeRow(currentVideoIndex.row());
            currentVideoIndex = QModelIndex();
            qDebug()<<"kkk";
        }
    });
    //Integrate the specified video into Pipeline temporary-saved Video
    connect(VideoToPipeline, &QAction::triggered, this, &MainWindow::addVideoItemToPipeline);
    connect(ImageOrTextToPipeline, &QAction::triggered, this, &MainWindow::addImageOrTextItemToPipeline);
    connect(previewPipeline, &QAction::triggered, this, [=](){
        videoProcessionInterface->VideoPreviewPlayer->setSource(videoProcessionInterface->tmpSavedVideoSrcPath);
        videoProcessionInterface->nowPlayedVideoSrcPath = videoProcessionInterface->tmpSavedVideoSrcPath;
        videoProcessionInterface->nowVideoDuration = commandLine->getVideoDuration(videoProcessionInterface->nowPlayedVideoSrcPath);
        videoProcessionInterface->VideoPreviewPlayer->play();
        QTimer time(this);
        time.setInterval(1000);
        videoProcessionInterface->VideoPreviewPlayer->pause();
    });
    // Output the video to a specified Location
    connect(VideoOutput, &QAction::triggered, this, [=](){
        commandLine->outputVideo(videoProcessionInterface->tmpSavedVideoSrcPath, videoProcessionInterface->textDataModel, videoProcessionInterface->ImageDataModel, videoProcessionInterface->VideoDataModel);
    });
    //Change the format of temporary Video of Pipeline
    connect(tmpTimeAxisVideoFormat, &QAction::triggered, this, [=](){
        QWidget* widget12 = new QWidget();
        QCheckBox* format1 = new QCheckBox("*.mp4",widget12);
        format1->setChecked(true);
        QCheckBox* format2 = new QCheckBox("*.mkv",widget12);
        QButtonGroup* group = new QButtonGroup(widget12);
        group->addButton(format1);
        group->addButton(format2);
        group->setExclusive(true);
        QHBoxLayout* globalLayout = new QHBoxLayout();
        QPushButton* YesButton = new QPushButton("Yes");
        globalLayout->addWidget(format1);
        globalLayout->addWidget(format2);
        globalLayout->addWidget(YesButton);
        widget12->setLayout(globalLayout);

        connect(YesButton, &QPushButton::clicked, this, [=](){
            if(format1->isChecked())
            {
                commandLine->tmpSavedVideoFormat = ".mp4";
            }
            else
            {
                commandLine->tmpSavedVideoFormat = ".mkv";
            }
            widget12->close();
        });
        widget12->show();
    });
    //Integrate the subtitle into pipeline Video.
    connect(subtitleAddedToPipeline, &QAction::triggered, this, [=](){
        QString subtitlePath = QFileDialog::getOpenFileName(this, "选择字幕", QCoreApplication::applicationDirPath(), "Subtitles (*.ass *.srt)");
        if(subtitlePath == "")
        {
            return;
        }
        videoProcessionInterface->tmpSavedVideoSrcPath = commandLine->addSubtitlesToVideo(videoProcessionInterface->tmpSavedVideoSrcPath, subtitlePath);
    });
    connect(CropOutputToVideoList, &QAction::triggered, this, [=](){
        if(currentVideoIndex.isValid())
        {
            QStandardItem* aItem = FileManage->VideoItemModel->itemFromIndex(currentVideoIndex);
            QVariant rawData = aItem->data();
            VideoItem data = rawData.value<VideoItem>();

            QString savedPath = QFileDialog::getSaveFileName(this, "选择保存地址", QCoreApplication::applicationDirPath(), "Video Files (*.mp4 *.mkv)");
            if(savedPath == "")
            {
                return;
            }
            else
            {
                qint64 duration = commandLine->getVideoDuration(data.srcPath);
                QWidget* widget = new QWidget();
                QHBoxLayout* layout = new QHBoxLayout();
                QLabel* label1 = new QLabel("StartTime: ", widget);
                QLabel* label2 = new QLabel("EndTime: ", widget);
                QLineEdit* stHour = new QLineEdit(widget);
                QLineEdit* stMin = new QLineEdit(widget);
                QLineEdit* stSec = new QLineEdit(widget);
                QLineEdit* etHour = new QLineEdit(widget);
                QLineEdit* etMin = new QLineEdit(widget);
                QLineEdit* etSec = new QLineEdit(widget);
                QLabel* label3 = new QLabel(":",this);
                QLabel* label4 = new QLabel(":",this);
                QLabel* label5 = new QLabel(":",this);
                QLabel* label6 = new QLabel(":",this);
                layout->addWidget(label1);
                layout->addWidget(stHour);
                layout->addWidget(label3);
                layout->addWidget(stMin);
                layout->addWidget(label4);
                layout->addWidget(stSec);
                layout->addWidget(label2);
                layout->addWidget(etHour);
                layout->addWidget(label5);
                layout->addWidget(etMin);
                layout->addWidget(label6);
                layout->addWidget(etSec);
                QLabel* wrongInput = new QLabel("Wrong Time Input", widget);
                wrongInput->hide();
                bool* isCorrectTime = new bool;
                *isCorrectTime =false;
                QPushButton* YesButton = new QPushButton("Yes", widget);
                layout->addWidget(wrongInput);
                layout->addWidget(YesButton);

                connect(stHour, &QLineEdit::textChanged, this, [=](){
                    qint64 startT = 1000 * (stHour->text().toInt() * 3600 + stMin->text().toInt() * 60 + stSec->text().toInt());
                    qint64 endT = 1000 * (etHour->text().toInt() * 3600 + etMin->text().toInt() * 60 + etSec->text().toInt());
                    if(startT<=endT&&endT<=duration)
                    {
                        *isCorrectTime = true;
                        wrongInput->hide();
                    }
                    else
                    {
                        *isCorrectTime = false;
                        wrongInput->show();
                    }
                });
                connect(etHour, &QLineEdit::textChanged, this, [=](){
                    qint64 startT = 1000 * (stHour->text().toInt() * 3600 + stMin->text().toInt() * 60 + stSec->text().toInt());
                    qint64 endT = 1000 * (etHour->text().toInt() * 3600 + etMin->text().toInt() * 60 + etSec->text().toInt());
                    if(startT<=endT&&endT<=duration)
                    {
                        *isCorrectTime = true;
                        wrongInput->hide();
                    }
                    else
                    {
                        *isCorrectTime = false;
                        wrongInput->show();
                    }
                });
                connect(stMin, &QLineEdit::textChanged, this, [=](){
                    qint64 startT = 1000 * (stHour->text().toInt() * 3600 + stMin->text().toInt() * 60 + stSec->text().toInt());
                    qint64 endT = 1000 * (etHour->text().toInt() * 3600 + etMin->text().toInt() * 60 + etSec->text().toInt());
                    if(startT<=endT&&endT<=duration&&stMin->text().toInt()<=59&&stMin->text().toInt()>=0)
                    {
                        *isCorrectTime = true;
                        wrongInput->hide();
                    }
                    else
                    {
                        *isCorrectTime = false;
                        wrongInput->show();
                    }
                });
                connect(etMin, &QLineEdit::textChanged, this, [=](){
                    qint64 startT = 1000 * (stHour->text().toInt() * 3600 + stMin->text().toInt() * 60 + stSec->text().toInt());
                    qint64 endT = 1000 * (etHour->text().toInt() * 3600 + etMin->text().toInt() * 60 + etSec->text().toInt());
                    if(startT<=endT&&endT<=duration&&etMin->text().toInt()<=59&&etMin->text().toInt()>=0)
                    {
                        *isCorrectTime = true;
                        wrongInput->hide();
                    }
                    else
                    {
                        *isCorrectTime = false;
                        wrongInput->show();
                    }
                });
                connect(stSec, &QLineEdit::textChanged, this, [=](){
                    qint64 startT = 1000 * (stHour->text().toInt() * 3600 + stMin->text().toInt() * 60 + stSec->text().toInt());
                    qint64 endT = 1000 * (etHour->text().toInt() * 3600 + etMin->text().toInt() * 60 + etSec->text().toInt());
                    if(startT<=endT&&endT<=duration&&stSec->text().toInt()<=59&&stSec->text().toInt()>=0)
                    {
                        *isCorrectTime = true;
                        wrongInput->hide();
                    }
                    else
                    {
                        *isCorrectTime = false;
                        wrongInput->show();
                    }
                });
                connect(etSec, &QLineEdit::textChanged, this, [=](){
                    qint64 startT = 1000 * (stHour->text().toInt() * 3600 + stMin->text().toInt() * 60 + stSec->text().toInt());
                    qint64 endT = 1000 * (etHour->text().toInt() * 3600 + etMin->text().toInt() * 60 + etSec->text().toInt());
                    if(startT<=endT&&endT<=duration&&etSec->text().toInt()<=59&&etSec->text().toInt()>=0)
                    {
                        *isCorrectTime = true;
                        wrongInput->hide();
                    }
                    else
                    {
                        *isCorrectTime = false;
                        wrongInput->show();
                    }
                });
                connect(YesButton, &QPushButton::clicked, this, [=](){
                    if(*isCorrectTime)
                    {
                        widget->close();
                        commandLine->CropVideo(data.srcPath,stHour->text(),stMin->text(),stSec->text(),etHour->text(),etMin->text(),etSec->text(),savedPath);
                        VideoItem item1 = data;
                        item1.srcPath = savedPath;
                        QStringList l1 = savedPath.split("/");
                        item1.name = l1[l1.size()-1];
                        QStandardItem* item2 = new QStandardItem;
                        item2->setData(QVariant::fromValue(item1), Qt::UserRole+1);
                        FileManage->VideoItemModel->appendRow(item2);
                    }
                });
                widget->setLayout(layout);
                widget->show();
            }
        }
    });

    // init the menus
    menu1->addAction(VideoInput);
    menu1->addAction(ImageInput);
    menu1->addAction(TextInput);
    menu1->addAction(VideoOutput);
    menu1->addAction(deleteTextOrImage);
    menu1->addAction(deleteVideo);

    menu2->addAction(ImageOrTextToPipeline);
    menu2->addAction(VideoToPipeline);
    menu2->addAction(previewPipeline);

    menu3->addAction(tmpTimeAxisVideoFormat);

    menu4->addAction(subtitleAddedToPipeline);

    menu5->addAction(CropOutputToVideoList);

    MainMenuBar->addMenu(menu1);
    MainMenuBar->addMenu(menu2);
    MainMenuBar->addMenu(menu3);
    MainMenuBar->addMenu(menu4);
    MainMenuBar->addMenu(menu5);

}

/**
 * @brief MainWindow::initVideoPreviewProcess:
 * Prepare for preview the video
 */
void MainWindow::initVideoPreviewProcess()
{
    //Draw the pixmap or text on the pipeline Video. Or just play the origin video.
    connect(videoProcessionInterface->backend_videoSink, &QVideoSink::videoFrameChanged, this, [=](const QVideoFrame& frame){
        if(frame.isValid())
        {
            QVideoFrame cloneFrame(frame);
            cloneFrame.map(QVideoFrame::ReadOnly);
            QImage::Format format = QVideoFrameFormat::imageFormatFromPixelFormat(cloneFrame.pixelFormat());

            QImage image(cloneFrame.bits(0), cloneFrame.width(), cloneFrame.height(), format);

            QPainter* painter = new QPainter(&image);
            painter->save();

            painter->setPen(QPen(Qt::black));
            painter->setFont(QFont("Microsoft Yahei", 10));

            if(videoProcessionInterface->nowPlayedVideoSrcPath == videoProcessionInterface->tmpSavedVideoSrcPath)
            {
                int rows = videoProcessionInterface->textDataModel->rowCount();
                int cols = videoProcessionInterface->textDataModel->columnCount();
                for(int i=0;i<rows;i++)
                {
                    for(int j=0;j<cols;j++)
                    {
                        QStandardItem* item = videoProcessionInterface->textDataModel->item(i,j);
                        if(item)
                        {
                            QVariant rawData = item->data();
                            timeAxisItem data = rawData.value<timeAxisItem>();

                            QRectF rect(data.x, data.y, image.width(), 20);
                            if((videoProcessionInterface->VideoPreviewPlayer->position()>=data.qStartTime)
                                &&(videoProcessionInterface->VideoPreviewPlayer->position()<=data.qEndTime))
                            {
                                painter->drawText(rect, data.value);
                            }
                        }
                    }
                }
            }
            painter->restore();

            painter->save();

            if(videoProcessionInterface->nowPlayedVideoSrcPath == videoProcessionInterface->tmpSavedVideoSrcPath)
            {
                int rows = videoProcessionInterface->ImageDataModel->rowCount();
                int cols = videoProcessionInterface->ImageDataModel->columnCount();
                for(int i=0;i<rows;i++)
                {
                    for(int j=0;j<cols;j++)
                    {
                        QStandardItem* item = videoProcessionInterface->ImageDataModel->item(i,j);
                        if(item)
                        {
                            QVariant rawData = item->data();
                            timeAxisItem data = rawData.value<timeAxisItem>();

                            if((videoProcessionInterface->VideoPreviewPlayer->position()>=data.qStartTime)
                                &&(videoProcessionInterface->VideoPreviewPlayer->position()<=data.qEndTime))
                            {
                                painter->drawImage(data.x, data.y, QImage(data.srcPath));
                            }
                        }
                    }
                }
            }
            painter->restore();

            videoPreviewWidget->setPixmap(QPixmap::fromImage(image));
        }

    });
}

void MainWindow::onCurrentVideoSelectionChaned(const QModelIndex &, const QModelIndex &)
{

}

/**
 * @brief MainWindow::initVideoPreviewWidget:
 * init the view of player, including the video frame and relative tools.
 */
void MainWindow::initVideoPreviewWidget()
{
    QString* durationTime = new QString;
    QString* positionTime = new QString;
    QLabel* posAndDuration = new QLabel("00:00/00:00",this);
    QVBoxLayout* VideoPlayer = new QVBoxLayout;
    QHBoxLayout* videoPlayerTool = new QHBoxLayout;

    QPushButton* PlayButton = new QPushButton(QIcon(QCoreApplication::applicationDirPath()+"/built-inMaterial/STARTPLAY.png"), "", this);
    connect(PlayButton, &QPushButton::clicked, this, [=](){
        if(videoProcessionInterface->VideoPreviewPlayer->playbackState() == QMediaPlayer::StoppedState||videoProcessionInterface->VideoPreviewPlayer->playbackState() == QMediaPlayer::PausedState)
        {
            PlayButton->setIcon(QIcon(QCoreApplication::applicationDirPath()+"/built-inMaterial/PAUSEPLAY.png"));
            videoProcessionInterface->VideoPreviewPlayer->play();
        }
        else
        {
            PlayButton->setIcon(QIcon(QCoreApplication::applicationDirPath()+"/built-inMaterial/STARTPLAY.png"));
            videoProcessionInterface->VideoPreviewPlayer->pause();
        }
    });
    QPushButton* StopButton = new QPushButton(QIcon(QCoreApplication::applicationDirPath()+"/built-inMaterial/STOPPLAY.png"), "", this);
    connect(StopButton, &QPushButton::clicked, this, [=](){
        PlayButton->setIcon(QIcon(QCoreApplication::applicationDirPath()+"/built-inMaterial/STARTPLAY.png"));
        videoProcessionInterface->VideoPreviewPlayer->stop();
    });
    QSlider* videoPlaySlider = new QSlider(Qt::Horizontal, this);
    videoPlaySlider->setMinimum(0);
    videoPlaySlider->setMaximum(0);
    connect(videoProcessionInterface->VideoPreviewPlayer, &QMediaPlayer::positionChanged, this, [=](qint64 position){
        videoPlaySlider->setSliderPosition(position);

        int secs = position / 1000;
        int mins = secs / 60;
        secs = secs % 60;
        *positionTime = QString::asprintf("%02d:%02d", mins, secs);
        posAndDuration->setText(*positionTime + "/" + *durationTime);

    });
    connect(videoProcessionInterface->VideoPreviewPlayer, &QMediaPlayer::durationChanged, this, [=](qint64 duration){
        videoPlaySlider->setMaximum(duration);

        int secs = duration / 1000;
        int mins = secs / 60;
        secs = secs % 60;

        *durationTime = QString::asprintf("%02d:%02d", mins, secs);
        posAndDuration->setText(*positionTime + "/" + *durationTime);

    });
    connect(videoPlaySlider, &QSlider::valueChanged, videoProcessionInterface->VideoPreviewPlayer, &QMediaPlayer::setPosition);

    QPushButton* audioButtion = new QPushButton(QIcon(QCoreApplication::applicationDirPath()+"/built-inMaterial/AUDIO.png"), "", this);
    QSlider* audioSlider = new QSlider(Qt::Horizontal, this);
    audioSlider->setMinimum(0);
    audioSlider->setMaximum(100);
    audioSlider->setValue((float)((float)videoProcessionInterface->audioOutput->volume()*(float)(100.0)));
    connect(audioSlider, &QSlider::valueChanged, this, [=](int value){
        videoProcessionInterface->audioOutput->setVolume((float)(value)/(float)(audioSlider->maximum()));
    });

    connect(audioButtion, &QPushButton::clicked, this, [=](){
        videoProcessionInterface->audioOutput->setVolume(0);
        audioSlider->setValue(0);
    });
    videoPlayerTool->addWidget(PlayButton,1);
    videoPlayerTool->addWidget(StopButton,1);
    videoPlayerTool->addWidget(videoPlaySlider, 50);
    videoPlayerTool->addWidget(posAndDuration,5);
    videoPlayerTool->addWidget(audioButtion, 1);
    videoPlayerTool->addWidget(audioSlider, 5);
    VideoPlayer->addWidget(videoPreviewWidget,90);
    VideoPlayer->addLayout(videoPlayerTool, 10);
    VideoProcessLayout->addLayout(VideoPlayer,7);


}
/**
 * @brief MainWindow::initTimeAxis:
 * init the View of Time Axis
 */
void MainWindow::initTimeAxis()
{
    QVBoxLayout* timeAxisLayout = new QVBoxLayout();

    timeAxisLayout->addWidget(textTimeAxis);
    timeAxisLayout->addWidget(imageTimeAxis);
    timeAxisLayout->addWidget(videoTimeAxis);


    VideoProcessLayout->addLayout(timeAxisLayout,3);
}

/**
 * @brief MainWindow::addImageOrTextItemToPipeline
 */
void MainWindow::addImageOrTextItemToPipeline()
{
    qint64 durationTime = commandLine->getVideoDuration(videoProcessionInterface->tmpSavedVideoSrcPath);


    QWidget* inputWidget = new QWidget();

    QIntValidator* intValidator = new QIntValidator;
    QIntValidator* secValidator = new QIntValidator;
    intValidator->setBottom(0);
    secValidator->setRange(0,59);

    QLabel* xLabel = new QLabel("x: ", inputWidget);
    QLineEdit* xVal = new QLineEdit(inputWidget);
    xVal->setValidator(intValidator);
    QLabel* yLabel = new QLabel("y: ", inputWidget);
    QLineEdit* yVal = new QLineEdit(inputWidget);
    yVal->setValidator(intValidator);
    QHBoxLayout* xyInputLayout = new QHBoxLayout();
    xyInputLayout->addWidget(xLabel);
    xyInputLayout->addWidget(xVal);
    xyInputLayout->addWidget(yLabel);
    xyInputLayout->addWidget(yVal);

    //Set the detail info like the location and duration.
    QLabel* stLabel = new QLabel("StartTime: ", inputWidget);
    QLineEdit* stValMin = new QLineEdit(inputWidget);
    stValMin->setValidator(intValidator);
    QLabel* dash1 = new QLabel("-", inputWidget);
    QLineEdit* stValSec = new QLineEdit(inputWidget);
    stValSec->setValidator(secValidator);
    QLabel* etLabel = new QLabel("EndTime: ", inputWidget);
    QLineEdit* etValMin = new QLineEdit(inputWidget);
    etValMin->setValidator(intValidator);
    QLabel* dash2 = new QLabel("-", inputWidget);
    QLineEdit* etValSec = new QLineEdit(inputWidget);
    etValSec->setValidator(secValidator);
    QLabel* warning = new QLabel("Time Input Error!", inputWidget);
    warning->hide();
    bool* isCorrectTime = new bool;
    QHBoxLayout* timeLayout = new QHBoxLayout();
    timeLayout->addWidget(stLabel);
    timeLayout->addWidget(stValMin);
    timeLayout->addWidget(dash1);
    timeLayout->addWidget(stValSec);
    timeLayout->addWidget(etLabel);
    timeLayout->addWidget(etValMin);
    timeLayout->addWidget(dash2);
    timeLayout->addWidget(etValSec);
    timeLayout->addWidget(warning);

    QPushButton* YesButton = new QPushButton("Yes", inputWidget);

    QCheckBox* isFadeEnabled = new QCheckBox("淡入淡出(图片类型强制使用):", inputWidget);
    QCheckBox* isTranslationEnabled = new QCheckBox("平移", inputWidget);

    QHBoxLayout* checkSpecialEffect = new QHBoxLayout();
    checkSpecialEffect->addWidget(isFadeEnabled);
    checkSpecialEffect->addWidget(isTranslationEnabled);

    QVBoxLayout* leftLayout = new QVBoxLayout();
    leftLayout->addLayout(xyInputLayout);
    leftLayout->addLayout(timeLayout);
    leftLayout->addLayout(checkSpecialEffect);

    QHBoxLayout* globalLayout = new QHBoxLayout();
    globalLayout->addLayout(leftLayout);
    globalLayout->addWidget(YesButton);
    inputWidget->setLayout(globalLayout);

    connect(stValMin, &QLineEdit::textChanged, this, [=](){
        int stmin = stValMin->text().toInt();
        int stsec = stValSec->text().toInt();
        int etmin = etValMin->text().toInt();
        int etsec = etValSec->text().toInt();
        qint64 startT = 1000 * (stmin * 60 + stsec);
        qint64 endT = 1000 * (etmin * 60 + etsec);
        if(startT<=endT&&endT<=durationTime)
        {
            *isCorrectTime = true;
            warning->hide();
        }
        else
        {
            *isCorrectTime = false;
            warning->show();
        }

    });
    connect(stValSec, &QLineEdit::textChanged, this, [=](){
        int stmin = stValMin->text().toInt();
        int stsec = stValSec->text().toInt();
        int etmin = etValMin->text().toInt();
        int etsec = etValSec->text().toInt();
        qint64 startT = 1000 * (stmin * 60 + stsec);
        qint64 endT = 1000 * (etmin * 60 + etsec);
        if(startT<=endT&&endT<=durationTime)
        {
            *isCorrectTime = true;
            warning->hide();
        }
        else
        {
            *isCorrectTime = false;
            warning->show();
        }

    });
    connect(etValMin, &QLineEdit::textChanged, this, [=](){
        int stmin = stValMin->text().toInt();
        int stsec = stValSec->text().toInt();
        int etmin = etValMin->text().toInt();
        int etsec = etValSec->text().toInt();
        qint64 startT = 1000 * (stmin * 60 + stsec);
        qint64 endT = 1000 * (etmin * 60 + etsec);
        if(startT<=endT&&endT<=durationTime)
        {
            *isCorrectTime = true;
            warning->hide();
        }
        else
        {
            *isCorrectTime = false;
            warning->show();
        }

    });
    connect(etValSec, &QLineEdit::textChanged, this, [=](){
        int stmin = stValMin->text().toInt();
        int stsec = stValSec->text().toInt();
        int etmin = etValMin->text().toInt();
        int etsec = etValSec->text().toInt();
        qint64 startT = 1000 * (stmin * 60 + stsec);
        qint64 endT = 1000 * (etmin * 60 + etsec);
        if(startT<=endT&&endT<=durationTime)
        {
            *isCorrectTime = true;
            warning->hide();
        }
        else
        {
            *isCorrectTime = false;
            warning->show();
        }

    });

    connect(YesButton, &QPushButton::clicked, this, [=](){
        if(*isCorrectTime)
        {
            timeAxisItem item;
            QStandardItem* aItem = FileManage->PhotoOrTextItemModel->itemFromIndex(currentPhotoOrTextIndex);
            QVariant rawData = aItem->data();
            photoOrTextItem data = rawData.value<photoOrTextItem>();

            item.type = data.type;
            item.srcPath = data.srcPath;
            item.value = data.name;
            item.x = xVal->text().toInt();
            item.y = yVal->text().toInt();

            if(isTranslationEnabled->isChecked())
            {
                item.TranslationEnabled = true;
            }
            if(item.type == "image" || isFadeEnabled->isChecked())
            {
                item.FadeEnabled = true;
            }

            int stmin = stValMin->text().toInt();
            int stsec = stValSec->text().toInt();
            int etmin = etValMin->text().toInt();
            int etsec = etValSec->text().toInt();
            qint64 startT = 1000 * (stmin * 60 + stsec);
            qint64 endT = 1000 * (etmin * 60 + etsec);
            item.qStartTime = startT;
            item.qEndTime = endT;
            QStandardItem *pItem = new QStandardItem;
            pItem->setData(QVariant::fromValue(item),Qt::UserRole+1);
            if(item.type=="Image")
            {
                videoProcessionInterface->ImageDataModel->appendRow(pItem);
                videoProcessionInterface->TimeAxisVideoView->updateView(videoTimeAxis);
                videoProcessionInterface->TimeAxisImageView->updateView(imageTimeAxis);
                videoProcessionInterface->TimeAxisTextView->updateView(textTimeAxis);

            }
            else
            {
                videoProcessionInterface->textDataModel->appendRow(pItem);
                videoProcessionInterface->TimeAxisVideoView->updateView(videoTimeAxis);
                videoProcessionInterface->TimeAxisImageView->updateView(imageTimeAxis);
                videoProcessionInterface->TimeAxisTextView->updateView(textTimeAxis);

            }
            inputWidget->close();
        }
    });
    inputWidget->show();
}
/**
 * @brief MainWindow::addVideoItemToPipeline
 */
void MainWindow::addVideoItemToPipeline()
{
    if(!currentVideoIndex.isValid())
    {
        return;
    }
    videoProcessionInterface->tmpSavedVideoSrcPath = commandLine->addVideoIntoPipeline(videoProcessionInterface->nowPlayedVideoSrcPath,videoProcessionInterface->tmpSavedVideoSrcPath);
    qDebug()<<videoProcessionInterface->tmpSavedVideoSrcPath;
    timeAxisItem item;
    QStandardItem* aItem = FileManage->VideoItemModel->itemFromIndex(currentVideoIndex);
    QVariant rawData = aItem->data();
    VideoItem data = rawData.value<VideoItem>();

    item.type = data.type;
    item.srcPath = data.srcPath;
    item.value = data.name;
    item.x = 0;
    item.y = 0;

    item.qStartTime = videoProcessionInterface->TimeAxisVideoView->getOverallDuration();
    item.qEndTime = commandLine->getVideoDuration(videoProcessionInterface->nowPlayedVideoSrcPath) + item.qStartTime;

    videoProcessionInterface->TimeAxisVideoView->setOverallDuration(item.qEndTime);
    videoProcessionInterface->TimeAxisImageView->setOverallDuration(item.qEndTime);
    videoProcessionInterface->TimeAxisTextView->setOverallDuration(item.qEndTime);

    QStandardItem *pItem = new QStandardItem;
    pItem->setData(QVariant::fromValue(item),Qt::UserRole+1);

    videoProcessionInterface->VideoDataModel->appendRow(pItem);
    videoProcessionInterface->TimeAxisVideoView->updateView(videoTimeAxis);
    videoProcessionInterface->TimeAxisImageView->updateView(imageTimeAxis);
    videoProcessionInterface->TimeAxisTextView->updateView(textTimeAxis);

}

MainWindow::~MainWindow()
{
    delete ui;
}

