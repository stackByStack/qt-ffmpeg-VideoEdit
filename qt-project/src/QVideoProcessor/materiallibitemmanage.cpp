#include "materiallibitemmanage.h"
#include "VideoItem.h"
#include "PhotoOrTextItem.h"
#include <QCoreApplication>
#include <QStringList>
#include <QLineEdit>
#include <QLayout>
#include <QPushButton>
/**
 * @brief MaterialLibItemManage::MaterialLibItemManage:
 * Init the model and match the model with the delegate method and model view
 *
 * @param parent
 */
MaterialLibItemManage::MaterialLibItemManage(QWidget *parent): parent(parent)
{
    VideoItemModel = new QStandardItemModel(parent);
    PhotoOrTextItemModel = new QStandardItemModel(parent);

    VideoItemModelDelegate = new VideoItemDelegate(parent);
    PhotoOrTextItemModelDelegate = new photoOrTextItemDelegate(parent);

    VideoItemView = new QListView(parent);
    PhotoOrTextItemView = new QListView(parent);

    VideoItemView->setItemDelegate(VideoItemModelDelegate);
    PhotoOrTextItemView->setItemDelegate(PhotoOrTextItemModelDelegate);

    VideoItemView->setModel(VideoItemModel);
    PhotoOrTextItemView->setModel(PhotoOrTextItemModel);

    connect(VideoItemModel,&QStandardItemModel::rowsInserted, this , [&](){
        VideoItemView->setModel(VideoItemModel);
    });
    connect(PhotoOrTextItemModel,&QStandardItemModel::rowsInserted, this , [&](){
        PhotoOrTextItemView->setModel(PhotoOrTextItemModel);;
    });

}

void MaterialLibItemManage::addVideoItem()
{
    QString srcPath = QFileDialog::getOpenFileName(parent, "选择视频", QCoreApplication::applicationDirPath(), "Video (*.mp4 *.mkv)");
    if(srcPath == "")
    {
        return;
    }
    QStandardItem *item = new QStandardItem;
    VideoItem vItem;
    QStringList list = srcPath.split("/");
    vItem.name = list[list.length()-1];
    vItem.srcPath = srcPath;
    vItem.type = "Video";
    vItem.firstFrame = QCoreApplication::applicationDirPath()+"/built-inMaterial/VIDEO.png";
    item->setData(QVariant::fromValue(vItem), Qt::UserRole+1);
    VideoItemModel->appendRow(item);
}

void MaterialLibItemManage::addImageItem()
{
    QString srcPath = QFileDialog::getOpenFileName(parent, "选择图片", QCoreApplication::applicationDirPath(), "Image Files (*.png)");
    if(srcPath == "")
    {
        return;
    }
    QStandardItem *item = new QStandardItem;
    photoOrTextItem vItem;
    QStringList list = srcPath.split("/");
    vItem.name = list[list.length()-1];
    vItem.srcPath = srcPath;
    vItem.type = "Image";
    item->setData(QVariant::fromValue(vItem), Qt::UserRole+1);

    PhotoOrTextItemModel->appendRow(item);
}

void MaterialLibItemManage::addTextItem()
{

    QWidget* inputText = new QWidget();
    QLineEdit* lineEdit = new QLineEdit(inputText);
    QPushButton* button1 = new QPushButton("确定",inputText);
    QHBoxLayout* layout = new QHBoxLayout();
    layout->addWidget(lineEdit);
    layout->addWidget(button1);
    inputText->setLayout(layout);


    connect(button1, &QPushButton::clicked, this ,[=](){
        tmpText = lineEdit->text();
        inputText->close();
        photoOrTextItem vItem;
        QStandardItem *item = new QStandardItem;

        vItem.srcPath = QCoreApplication::applicationDirPath()+"/built-inMaterial/TEXT.png";
        vItem.type = "Text";
        vItem.name = tmpText;

        item->setData(QVariant::fromValue(vItem), Qt::UserRole+1);

        item->setData(QVariant::fromValue(vItem), Qt::UserRole+1);

        PhotoOrTextItemModel->appendRow(item);
    }, Qt::QueuedConnection);


    inputText->show();
}
