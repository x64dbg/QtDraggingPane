#include "PaneSubWidget.h"
#include "PaneWidget.h"
#include "PaneMeta.h"
#include <QDebug>
#include <QMouseEvent>
#include <QTabBar>
#include <QApplication>
#include <QBitmap>
#include <QPainter>
#include <QPicture>
#include <QSplitter>
#include <QPushButton>



QWidget* findGoodParent(QObject* widget)
{

    while(widget)
    {
        widget = widget->parent();
        if (qobject_cast<PaneSubWidget*>(widget))
        {
            return static_cast<QWidget*>(widget);
        }
        if (qobject_cast<QSplitter*>(widget))
        {
            return static_cast<QWidget*>(widget);
        }
    }
    return 0;
}

// a pane is customized tab widget
PaneSubWidget::PaneSubWidget(QWidget *parent) :
    QTabWidget(parent)
{
    // remember parent
    mParentWidget = parent;
    setParent(mParentWidget);
    setMovable(true);
    setTabsClosable(true);
    setDocumentMode(true);
    tabBar()->installEventFilter(this);

    // nothing is dragable at the moment
    mDragIsPossible=false;
    // blue area to show possible destinations
    mSuggestionArea = new QRubberBand(QRubberBand::Rectangle, this);
    // nice picture for positioning
    mMiddleHintImg = new QLabel(0, Qt::ToolTip );
    mMiddleHintImg->setAttribute(Qt::WA_ShowWithoutActivating);
    mMiddleHintImg->setGeometry(0,0,89,89);
    mMiddleHintImg->setStyleSheet("background-image: url(:/icons/middleHint.png); background-color:transparent;");
    // currently no picture for outside positioning
    mLeftHintImg = new QLabel(0, Qt::ToolTip );
    mLeftHintImg->setAttribute(Qt::WA_ShowWithoutActivating);
    mLeftHintImg->setGeometry(0,0,15,30);
    mLeftHintImg->setStyleSheet(" background-color:red;");
    mRightHintImg = new QLabel(0, Qt::ToolTip );
    mRightHintImg->setAttribute(Qt::WA_ShowWithoutActivating);
    mRightHintImg->setGeometry(0,0,15,30);
    mRightHintImg->setStyleSheet(" background-color:red;");
    mTopHintImg = new QLabel(0, Qt::ToolTip );
    mTopHintImg->setAttribute(Qt::WA_ShowWithoutActivating);
    mTopHintImg->setGeometry(0,0,30,15);
    mTopHintImg->setStyleSheet(" background-color:red;");
    mBottomHintImg = new QLabel(0, Qt::ToolTip );
    mBottomHintImg->setAttribute(Qt::WA_ShowWithoutActivating);
    mBottomHintImg->setGeometry(0,0,30,15);
    mBottomHintImg->setStyleSheet(" background-color:red;");


}

void PaneSubWidget::setIdName(QString n){
    mIdName = n;
}
QString PaneSubWidget::idName() const{
    return "idname: "+mIdName;
}

// FORWARD all events from any subwidget
void PaneSubWidget::onClickSubPane(QWidget *w)
{
    emit clickPane(w);
}
void PaneSubWidget::onDragSubPane(CurrentWidget w)
{
    emit dragPane(w);
}
void PaneSubWidget::onMoveSubPane(bool inTabbar)
{
    emit movePane(inTabbar);
}
void PaneSubWidget::onReleaseSubPane()
{
    emit releasePane();
}


QSplitter* PaneSubWidget::createSplitter(QWidget* A,QWidget* B,QWidget* parent,bool vertical){
    QSplitter *split = new QSplitter(parent);
    split->setChildrenCollapsible(false);
    split->setHandleWidth(10);
    A->setParent(split);
    B->setParent(split);

    split->addWidget(A);    // add new widget "left of old"
    split->addWidget(B);      // add old widget
    B->show();
    A->show();
    return split;
}

void PaneSubWidget::replaceTab(QWidget* find, QWidget* replace){

    // remember current tabIndex from parent
    const int idx = indexOf(find);

    // add the tab
    const int new_idx = insertTab(idx,replace,"new tab");
    setCurrentIndex(new_idx);
}
void PaneSubWidget::addWidget(QWidget *newWidget, const PanePosition Pos)
{
    if(qobject_cast<PaneSubWidget*>(newWidget)){
        qDebug() << "current wid: "<< qobject_cast<PaneSubWidget*>(newWidget)->idName() << " from parent "<< idName();
        qDebug() << "ask for position "<<Pos;
    }

    // how is your dady?
    newWidget->setParent(this);
    // try to interpret the widget as PaneSubWidget
    PaneSubWidget *paneWidget = qobject_cast<PaneSubWidget*>(newWidget);
    // unfortunetaly I cannot override the "windowTitle" method, so use the cast-result
    QString winTitle= (!paneWidget) ? newWidget->windowTitle() : paneWidget->windowTitle();

    // place widget at correct position
    switch(Pos)
    {
    default:
        qDebug() << Pos;
        qErrnoWarning("not implemented yet");
        break;
    case TabIn:
    {
        // this is easy just add a Tab
        int idx;
        idx = addTab(newWidget,winTitle);
        if(paneWidget)
        {
            // if widget is a PaneSubWidget --> create connections
            connect(paneWidget,SIGNAL(clickPane(QWidget*)),this,SLOT(onClickSubPane(QWidget*)));
            connect(paneWidget,SIGNAL(dragPane(CurrentWidget)),this,SLOT(onDragSubPane(CurrentWidget)));
            connect(paneWidget,SIGNAL(movePane(bool)),this,SLOT(onMoveSubPane(bool)));
            connect(paneWidget,SIGNAL(releasePane()),this,SLOT(onReleaseSubPane()));
        }
        setCurrentIndex(idx);
        break;
    }

    case RightOut:
    {
        // the new widget should be placed left of "this"
        // ----------------------------------------------
        // get parent panesub-object

        QWidget* parent = findGoodParent(this);

        if(qobject_cast<PaneSubWidget*>(parent)){
            // next interesting parent is PaneSubWidget --> use that
            PaneSubWidget* parentPane =  findParentOfType<PaneSubWidget*>(this);
            QWidget* find = parentPane->currentWidget();
            QSplitter *replace = createSplitter(this,newWidget,parentPane);

            parentPane->replaceTab(find,replace);
        }
        if(qobject_cast<QSplitter*>(parent)){
            // TODO :: QSplitter --> PaneSplitter for easier removing
        }
        break;
    }
    }
}


QString PaneSubWidget::windowTitle() const
{
    // we try to override the windowTitle-methode since we want to concatenate all titles by ','
    const unsigned int n = count();
    if( n >1  )
    {
        // there are more than 1 window
        QStringList titles;
        for(unsigned int i=0; i<n; i++)
            titles.append(widget(i)->windowTitle());
        return titles.join(",");

    }
    else
    {
        // just return the name from the child
        return widget(0)->windowTitle();
    }
}


PanePosition PaneSubWidget::dropHint(QPoint pt)
{
    // return blue area from the given QPoint (mostly this will be the cursor position)
    QPoint m = mapToGlobal(QPoint(rect().left()+rect().width()/2,rect().top()+rect().height()/2));

    const int x = cursor().pos().x();
    const int y = cursor().pos().y();

    // middle
    if(abs(x-m.x())<15 && abs(y-m.y())<15)
        return TabIn;
    // horizontal
    if(abs(x-m.x())<45 && abs(y-m.y())<15)
        return (x>m.x() ? RightIn : LeftIn);
    // vertical
    if(abs(y-m.y())<45 && abs(x-m.x())<15)
        return (y>m.y() ? BottomIn : TopIn);

    // horizontal outside
    if((abs(x-(m.x()-rect().width()/2))<15  )&& abs(y-m.y())<15)
        return LeftOut;
    if((abs(x-(m.x()+rect().width()/2))<15  )&& abs(y-m.y())<15)
        return RightOut;

    // vertical outside
    if((abs(y-(m.y()+rect().height()/2))<15  )&& abs(x-m.x())<15)
        return BottomOut;
    if((abs(y-(m.y()-rect().height()/2))<15  )&& abs(x-m.x())<15)
        return TopOut;

    return Hide;

}


void PaneSubWidget::onDisplayDropHints(bool visible)
{
    // this will draw the nice little artistic picture
    if(visible)
    {

        QPoint p = mapToGlobal(QPoint(rect().left()+rect().width()/2-mMiddleHintImg->width()/2,rect().top()+rect().height()/2-mMiddleHintImg->height()/2));
        mMiddleHintImg->move(p);
        mMiddleHintImg->show();

        QPoint middlePt = mapToGlobal(QPoint(rect().left()+rect().width()/2,rect().top()+rect().height()/2));

        QPoint left = middlePt - QPoint(rect().width()/2,mLeftHintImg->height()/2);
        QPoint right = middlePt + QPoint(rect().width()/2-mRightHintImg->width(),-mRightHintImg->height()/2);
        QPoint top = middlePt - QPoint(mTopHintImg->width()/2,rect().height()/2);
        QPoint bottom = middlePt + QPoint(-0.5*mBottomHintImg->width(),rect().height()/2-mBottomHintImg->height());


        mLeftHintImg->move(left);
        mRightHintImg->move(right);
        mTopHintImg->move(top);
        mBottomHintImg->move(bottom);

        mLeftHintImg->show();
        mRightHintImg->show();
        mTopHintImg->show();
        mBottomHintImg->show();
    }
    else
    {
        mMiddleHintImg->hide();
        mLeftHintImg->hide();
        mRightHintImg->hide();
        mTopHintImg->hide();
        mBottomHintImg->hide();
    }
}

void PaneSubWidget::onDrawSuggestionArea(PanePosition pos)
{
    // draw the blue area on screen that indicates the possible target position
    QRect rec;


    switch(pos)
    {
    case TopIn:
        rec = QRect(0,tabBar()->height(),rect().width(),rect().height()/2-tabBar()->height());
        break;
    case BottomIn:
        rec = QRect(0,rect().height()/2,rect().width(),rect().height()/2);
        break;
    case LeftIn:
        rec = QRect(0,tabBar()->height(),rect().width()/2,rect().height()-tabBar()->height());
        break;
    case RightIn:
        rec = QRect(rect().width()/2,tabBar()->height(),rect().width()/2,rect().height()-tabBar()->height());
        break;
    case TabIn:
        rec = QRect(0,tabBar()->height(),rect().width(),rect().height()-tabBar()->height());
        break;
    case LeftOut:
        rec = QRect(0,0,15,rect().height());
        break;
    case RightOut:
        rec = QRect(rect().width()-15,0,15,rect().height());
        break;
    case TopOut:
        rec = QRect(0,0,rect().width(),15);
        break;
    case BottomOut:
        rec = QRect(0,rect().height()-15,rect().width(),15);
        break;
    default:
    case Hide:
        mSuggestionArea->hide();
        return;
    }
    mSuggestionArea->setGeometry(rec);
    mSuggestionArea->show();
}

bool PaneSubWidget::eventFilter(QObject *object, QEvent *event)
{
    // handle all events
    if (object == tabBar())
    {
        if (event->type() == QEvent::MouseButtonPress && qApp->mouseButtons() == Qt::LeftButton)
        {
            // mouse button was pressed, so keep attention
            startDragging=true;
        }
        else if (event->type() == QEvent::MouseMove)
        {
            // EVENT: move pane
            // the mouse is moving, just check wazzup
            if (!tabBar()->rect().contains(static_cast<QMouseEvent*>(event)->pos()))
            {
                // the left mouse button was pressed down --> good sign for dragging
                if(startDragging)
                {
                    // bugfixing Qt (which will activate another tab and move that in addition)
                    QMouseEvent* releaseEvent = new QMouseEvent(QEvent::MouseButtonRelease,static_cast<QMouseEvent*>(event)->pos(),Qt::LeftButton, Qt::LeftButton, 0);
                    qApp->sendEvent(tabBar(), releaseEvent);
                    // get active widget that was clicked
                    QWidget* widget = currentWidget();

                    const int idx = indexOf(widget);
                    // just remember some information
                    CurrentWidget mCurrentWidget;
                    mCurrentWidget.Parent = qobject_cast<PaneSubWidget*>(this);
                    mCurrentWidget.Widget = widget;
                    mCurrentWidget.Index = idx;
                    mCurrentWidget.Title = tabText(idx);
                    mCurrentWidget.InDragProcess=true;
                    mCurrentWidget.LastPane = this;
                    // remove the tab
                    removeTab(idx);
                    // tell everybody the news
                    emit dragPane(mCurrentWidget);
                    // we do NOT start, we currently DO dragging
                    startDragging=false;
                    isDragging=true;
                }
            }
            if(isDragging)
                emit movePane(true);
            return false;
        }
        // EVENT: release pane
        else if (event->type() == QEvent::MouseButtonRelease)
        {
            // the user releases the mouse button
            emit releasePane();
        }
    }
    return QTabWidget::eventFilter(object, event);
}
