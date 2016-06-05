#ifndef ITEMDELEGATE_H
#define ITEMDELEGATE_H

#include <QStyledItemDelegate>
#include <QModelIndex>
#include <QPoint>
#include <QPainter>

// ItemData 数据
typedef struct{
    int id;
    bool check;
    QString BkPixmap;
    QString ClosePixmap;
}ItemData;

class Itemdelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit Itemdelegate(QObject *parent = 0);
    ~Itemdelegate();

    enum ItemType{                                  //枚举按钮状态
        ItemNormal = 0,
        ItemCheck,
        ItemClose

    };

    enum ButtonState{                               //枚举鼠标状态
        ButtonNormal = 0,
        ButtonHover,
        ButtonPress
    };


    void paint(QPainter * painter,const QStyleOptionViewItem & option,const QModelIndex & index) const;
    bool editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index);

signals:
    void SendData(const QModelIndex &index,int);    //发送数据
    void sendIndex(QModelIndex&,int);               //点击关闭

private:
    QPoint mousePoint;    //鼠标位置
    ItemType PItemType;   //图标状态
    ButtonState PButtonState; //按钮状态

};

#endif // ITEMDELEGATE_H
