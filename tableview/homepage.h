#ifndef HOMEPAGE_H
#define HOMEPAGE_H

#include <QWidget>
#include <QTableView>
#include <QStandardItemModel>
#include "itemdelegate.h"
#include <QPushButton>
#include <QVBoxLayout>

class HomePage : public QWidget
{
    Q_OBJECT



signals:

public:
    HomePage(QWidget *parent = 0);
    ~HomePage();

    void releaseQStandaraItemModel(QAbstractItemModel *castmodel);
    void initTableView(QTableView *tableView,QList<ItemData*>datalist,int column);
    void init();

private slots:
    void receiveData(const QModelIndex &index,int type);

private:
    QTableView *imageView;            //tableview
    //QPushButton *AddButton;           //添加按钮后期再实现
    QVBoxLayout *mainLayout;          //主布局垂直布局
    QList<ItemData*>indexlist;        //删除cell然后重新加载cell的辅助变量
};


#endif // HOMEPAGE_H
