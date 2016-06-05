#include "homepage.h"
#include <QFileDialog>
#include <QDebug>
#include <QHeaderView>
HomePage::HomePage(QWidget *parent)
    : QWidget(parent)
{

    setMinimumSize(485,425);
    setMaximumSize(485,425);

    //setWindowFlags(Qt::FramelessWindowHint);

    //AddButton = new QPushButton("Add");
    mainLayout = new QVBoxLayout(this);
    imageView = new QTableView;
    mainLayout->addWidget(imageView);

    //mainLayout->addWidget(AddButton);
    mainLayout->setMargin(5);      //控件与窗口之间的间隔
    //mainLayout->setSpacing(100);控件之间的间隔

    imageView->verticalHeader()->hide();   //需要包含hearview头文件
    imageView->horizontalHeader()->hide();




    init();
}

HomePage::~HomePage()
{

}

void HomePage::releaseQStandaraItemModel(QAbstractItemModel *castmodel)
{
    QStandardItemModel *model = static_cast<QStandardItemModel*>(castmodel);//获取视图的模型
    if(model)                                                               //视图模型存在那么全部删除
    {
        while(model->rowCount())                                            //总的行数回收
        {
            model->removeRow(0);
        }
        delete model;
    }
}

void HomePage::initTableView(QTableView *tableView, QList<ItemData *> datalist, int column)
{
    QStandardItemModel *model = static_cast<QStandardItemModel*>(tableView->model());  //获取视图模型
    releaseQStandaraItemModel(model);                                                  //释放其空间
    model = new QStandardItemModel;

    tableView->setSelectionMode(QAbstractItemView::ExtendedSelection);                 //允许选中多行
    tableView->setShowGrid(false);                                                     //不显示网格

    int width = tableView->width();                                                    //获取tableview宽度
    int row = datalist.size()/column;                                                  //初始化4列,行数=list长度/列数
    if(datalist.size()%column)                                                         //不够一行的放在新的一行
    {
        row+=1;
    }

    model->setColumnCount(column);                                                     //设置column列
    model->setRowCount(row);                                                           //设置row行
    tableView->setModel(model);                                                        //设置模型
    tableView->setMouseTracking(true);                                                 //允许鼠标跟踪

    Itemdelegate *PItemDelegate = new Itemdelegate;
    connect(PItemDelegate,SIGNAL(SendData(QModelIndex,int)),this,SLOT(receiveData(QModelIndex,int)));//设置信号槽


    tableView->setItemDelegate(PItemDelegate);                                         //设置委托

    for(int j = 0; j < row; ++j){
        tableView->setRowHeight(j,150);                                       //设置行高度
    for(int i = 0; i < column; ++i)
    {
        tableView->setColumnWidth(i,150);                                    //设置列宽度
        int indexOfList = column*j+i;                                                  //总共的item数
        if(indexOfList < datalist.size())       //判断，当遍历完item之后
        {
            model->setData(model->index(j,i),(unsigned int)datalist.at(indexOfList),Qt::UserRole);
            model->item(j,i)->setEnabled(false);//禁止编辑
        }
        else
        {
            model->setData(model->index(j,i),NULL,Qt::UserRole);
            model->item(j,i)->setEnabled(false);//禁止编辑
        }
    }
    }
}

void HomePage::init()
{
   QList<ItemData*>datalist;                              //容器

   QDir pdir = QDir::current();                           //获取当前工作目录路径
   QStringList filters,Prelease,Pimage;
   Prelease<<"release";
   Pimage<<"image";
   filters << "*.png" << "*.jpg" << "*.bmp" << "*.gif";   //筛选规则
   //pdir.setNameFilters(filters);                        //设置名字筛选规则
   pdir.setNameFilters(Prelease);
   //QFileInfoList plist = pdir.entryInfoList();          //获得完整文件目录信息
   pdir.setFilter(QDir::Dirs|QDir::NoDotAndDotDot);       //设置文件筛选规则



   QDir UrlFirst = QDir(pdir.entryInfoList().at(0).filePath()); //遍历子文件
   qDebug()<<pdir.entryInfoList().at(0).filePath()<<"11111";

   UrlFirst.setNameFilters(Pimage);
   UrlFirst.setFilter(QDir::Dirs|QDir::NoDotAndDotDot);


   if(UrlFirst.entryInfoList().size()==0)
   {
       qDebug()<<"relesea/image is no photo ";

       return ;
   }
   QDir UrlSecond = QDir(UrlFirst.entryInfoList().at(0).filePath());//遍历子文件

   if(UrlSecond.entryInfoList().size()==0)
   {
       qDebug()<<"urlsecond is no image";
       return;
   }
   qDebug()<<UrlSecond.entryInfoList().at(0).filePath()<<"2222";
   UrlSecond.setNameFilters(filters);

   QFileInfoList plist = UrlSecond.entryInfoList();

   if(!plist.size())                                    //判断指针是否为空
       return;

    for(int i=0;i<plist.size();i++)
    {
        ItemData *data = new ItemData;                  //新的item
        data->id = i;
        data->check = false;
        data->BkPixmap = plist.at(i).filePath();        //新的背景图片,自动填充
        qDebug()<<data->BkPixmap;
        data->ClosePixmap = ":/image/close_hover.png";  //关闭按钮都设置一样的
        datalist.append(data);                          //追加到容器里
    }
    indexlist = datalist;
    initTableView(imageView,datalist,4);                //初始化tabaleview
}

void HomePage::receiveData(const QModelIndex &index, int type)
{
  //强制转换获取数据
  ItemData *data = (ItemData*)index.data(Qt::UserRole).toUInt();

  //判断指针是否存在
  if(data)
  {
      qDebug()<<"receiveData sucessfully!";                   //双击cell然后获取pixmap的路径可以任意
      int row = index.row();
      int column = index.column();
      ItemData pdata;
      pdata = *indexlist.at(row*4+column);
      qDebug()<<pdata.BkPixmap;
  }

  //判断按钮状态
  switch(type)
  {
  case Itemdelegate::ItemClose://删除item
  {
      qDebug()<<"closeBtn";


      //这里需要作出删除删除！！！！！！！！
      int row = index.row();
      int column = index.column();
      indexlist.removeAt(row*4+column);
      initTableView(imageView,indexlist,4);

      break;
  }
  case Itemdelegate::ItemCheck:
  {
      if(data->check)
      {

          data->check = false;
      }
      else
      {
          data->check = true;
      }
      //设置视图数据
      imageView->model()->setData(index,(unsigned int )data,Qt::UserRole);
      break;
  }
  default:
      break;
  }
}

