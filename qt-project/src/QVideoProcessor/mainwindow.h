#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "materiallibitemmanage.h"
#include "videoprocessinterface.h"
#include "ffmpegcommandinterface.h"

#include <QMainWindow>
#include <QLayout>
#include <QGroupBox>
#include <QSplitter>
#include <QListView>
#include <QListWidget>
#include <QMenu>
#include <QLabel>
#include <QMenuBar>
#include <QAction>
#include <QItemSelectionModel>
#include <QVideoFrame>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE
/**
 * @brief The MainWindow class
 * Responsible for layout and controlling global component
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    QVBoxLayout* globalLayout;
    QHBoxLayout* WorkZoneLayout;
    QVBoxLayout* MaterialLibLayout;
    QVBoxLayout* VideoProcessLayout;

//    QGroupBox* PhotoOrTextBox;
//    QGroupBox* VideoBox;
//    QGroupBox* VideoPreview;
//    QGroupBox* TimeAxis;

    QSplitter* MaterialLibSplitter;
    QSplitter* VideoProcessSplitter;
    QSplitter* BetweenLibAndProcessSplitter;

    QWidget* centralWidget;

    MaterialLibItemManage* FileManage;
    QItemSelectionModel* VideoSelection;
    QItemSelectionModel* PhotoOrTextSelection;

    VideoProcessInterface* videoProcessionInterface;

    QMenuBar *MainMenuBar;

    QLabel *videoPreviewWidget;
    QLabel *videoTimeAxis;
    QLabel *textTimeAxis;
    QLabel *imageTimeAxis;

    QModelIndex currentVideoIndex;
    QModelIndex currentPhotoOrTextIndex;
    QModelIndex currentTimeAxisVideoIndex;

    FfmpegCommandInterface* commandLine;

public slots:
    void addVideoItemToPipeline();
    void addImageOrTextItemToPipeline();
    //void addTextItemToPipeline();

private:
    void initMainMenuBar();
    void initVideoPreviewWidget();
    void initVideoPreviewProcess();
    void initTimeAxis();
    void onCurrentVideoSelectionChaned(const QModelIndex &, const QModelIndex &);
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
