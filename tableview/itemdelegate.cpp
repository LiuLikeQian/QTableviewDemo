#include "itemdelegate.h"
#include <QDebug>
#include <QApplication>
#include <QPainter>
#include <QMouseEvent>

Itemdelegate::Itemdelegate(QObject *parent):
 QStyledItemDelegate(parent)
{

}

Itemdelegate::~Itemdelegate()
{
    qDebug()<<"delegate is over";
}

//重写paint
void Itemdelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    //获取数据
    ItemData *Data = (ItemData *)index.data(Qt::UserRole).toUInt();
    if(!Data)
    {
        qDebug()<<"return";
        return ;

    }

    //QStyleOptionViewItem ViewOption(option);                        //创建视图子对象
    //initStyleOption(&ViewOption,index);                             //初始化选项索引值

    //if(option.state.testFlag(QStyle::State_HasFocus))
    //    ViewOption.state = ViewOption.state ^ QStyle::State_HasFocus;//判断子对象是否激活，【已测试】激活才可用


    QRect rect;
    rect.setX(option.rect.x()+2);                                     //162  320 140  140
    rect.setY(option.rect.y());
    rect.setHeight(option.rect.height()-35);                          //这样的算法不是很好但是达到目前好看点，可以根据个人需求在优化
    rect.setWidth(option.rect.width()-35);                            //36-54，鼠标移动到索引描绘框框

    QColor ColorFrame(213,213,213,255);
    painter->setPen(ColorFrame);

    if((option.state & QStyle::State_Selected))
    {
        painter->drawRect(rect);
    }
    else if(option.state & QStyle::State_MouseOver)
    {
        painter->drawRect(rect);
    }
    else
    {
        //painter->drawRect(rect);
    }

    QRect BKRect(option.rect.x()+20,option.rect.y()+20,80,80);
    painter->drawPixmap(BKRect,QPixmap(Data->BkPixmap));               //画出背景图

    //qDebug()<<Data->BkPixmap;                                        //测试输出背景图路径[绝对路径，工作目录的路径]

//
    QRect CloseBtnRect(option.rect.x()+option.rect.width()-50,option.rect.y()+9,10,10);//closeIcon区域


    QStyleOptionButton button;                                       //closeBtn绘制


    button.rect = QRect(CloseBtnRect);                               //设置closeBtn区域

    button.state |= QStyle::State_Enabled;                           //closeBtn激活
    button.iconSize = QSize(20,20);                                  //设置大小

    if(button.rect.contains(mousePoint))                             //鼠标移动的状态去显示closeBtn[图都是一张没改]
    {
        switch(PButtonState)
        {
        case ButtonNormal:
            break;
        case ButtonHover:
            button.state |=QStyle::State_MouseOver;
            button.icon = QIcon(Data->ClosePixmap);
        break;
        case ButtonPress:
            button.state |=QStyle::State_Sunken;
            button.icon = QIcon(Data->ClosePixmap);
        break;
        }
    }
    else
    {
        button.icon = QIcon(Data->ClosePixmap);                   //默认情况下显示
    }
    //画出closeBtn
    QApplication::style()->drawControl(QStyle::CE_PushButtonLabel, &button, painter,NULL);
//
}

bool Itemdelegate::editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index)
{

    bool BRepaint = false;                                        //返回状态

    Q_UNUSED(model);                                              //忽略某些警告

    ItemData *Data = (ItemData*)index.data(Qt::UserRole).toUInt();//获取数据

    if(!Data)                                                     //判断是否成功获取数据
    {
        return BRepaint;
    }

    PButtonState = ButtonNormal;                                  //按钮状态

    QMouseEvent *Pevent = static_cast<QMouseEvent *>(event);      //将事件转换鼠标事件并方便获取鼠标位置
    mousePoint = Pevent->pos();


//
    QRect ButtonRect(option.rect.x()+option.rect.width()-50,option.rect.y()+9,10,10);//作用区域


    QStyleOptionButton button;
    button.rect = QRect(ButtonRect);                              //设置Button作用域

    if(button.rect.contains(mousePoint))                          //设置状态
    {
        PItemType = ItemClose;
    }
    else
    {
        QApplication::restoreOverrideCursor();
        PItemType = ItemCheck;
    }
//

    switch(event->type())                                         //根据不用的鼠标状态做出对应的回复
    {
       case QEvent::MouseMove:
    {
        if(ItemClose==PItemType){                                 //鼠标点击
            PButtonState = ButtonPress;
        }
        break;
    }
    case QEvent::MouseButtonRelease:                              //鼠标松开
    {
        if(ItemCheck == PItemType)
        {
            BRepaint = true;
        }
        emit SendData(index,(int )PItemType);
        break;
    }
    default:
        break;
    }
    return BRepaint;
}

