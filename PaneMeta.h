#ifndef PANEMETA_H
#define PANEMETA_H
#include <QWidget>
#include <QMainWindow>
#include <QSplitter>



/*
 * TopIn --> stack in widget (creates a multi-tab)
 * BottomIn --> stack in widget (creates a multi-tab)
 * LeftIn --> stack in widget (creates a multi-tab)
 * RightIn --> stack in tab (creates a multi-tab)
 * TabIn --> create tab in tab (creates a multi-tab in tab)
 * LeftOver --> stack over widget
 */
enum PanePosition {TopIn,BottomIn,LeftIn,RightIn,TabIn,Hide,TopOut,LeftOut,BottomOut,RightOut};

template<class T>
T findParentOfType(QObject* widget)
{

    while(widget)
    {
        widget = widget->parent();
        if (qobject_cast<T>(widget))
        {
            return static_cast<T>(widget);
        }
    }
    return 0;
}


template<class T>
int findParentDistance(QObject* widget)
{
    int i=0;
    while(widget)
    {
        qDebug() << "++ classnames "<<widget->metaObject()->className();
        widget = widget->parent();
        i++;
        if (qobject_cast<T>(widget))
        {
            return i;
        }
    }
    return INT_MAX;
}


class PaneSubWidget;
struct CurrentWidget
{
    QWidget* Widget;
    PaneSubWidget* Parent;
    QString Title;
    int Index;
    bool InDragProcess;
    PaneSubWidget *LastPane;
    CurrentWidget()
    {
        InDragProcess=false;
    }
};


#endif // PANEMETA_H
