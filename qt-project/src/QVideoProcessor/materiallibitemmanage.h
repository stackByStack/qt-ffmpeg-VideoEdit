#ifndef MATERIALLIBITEMMANAGE_H
#define MATERIALLIBITEMMANAGE_H

#include "videoitemdelegate.h"
#include "photoortextitemdelegate.h"

#include <QStandardItemModel>
#include <QStandardItem>
#include <QString>
#include <QFileDialog>
#include <QPushButton>
#include <QListView>
#include <QTextEdit>

/**
 * @brief The MaterialLibItemManage class
 * Responsible for Managing components in Material Library
 */
class MaterialLibItemManage : public QObject
{
    Q_OBJECT
public:
    MaterialLibItemManage(QWidget *parent);

public slots:
    void addVideoItem();
    void addTextItem();
    void addImageItem();

public:
    QStandardItemModel* VideoItemModel;
    QStandardItemModel* PhotoOrTextItemModel;

    VideoItemDelegate* VideoItemModelDelegate;
    photoOrTextItemDelegate* PhotoOrTextItemModelDelegate;

    QListView* VideoItemView;
    QListView* PhotoOrTextItemView;

private:
    QWidget* parent;
    QString tmpText;
};

#endif // MATERIALLIBITEMMANAGE_H
