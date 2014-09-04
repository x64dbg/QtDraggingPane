#include "PaneWidget.h"
#include "PaneSubWidget.h"
#include "PaneMeta.h"

#include <QSplitter>
#include <QPushButton>
#include <QDebug>

// should be set as central widget
PaneWidget::PaneWidget(QWidget *parent) : QWidget(parent)
{
    // unname topmost widget
    setWindowFlags(windowFlags() | Qt::Tool);
    setWindowTitle(" ");

    // prepare dynamic layout
    mMainLayout = new QVBoxLayout(this);
    mMainLayout->setContentsMargins(0, 0, 0, 0);

    setLayout(mMainLayout);

    mDragHint = new QLabel(0, Qt::ToolTip );
    mDragHint->setAttribute(Qt::WA_ShowWithoutActivating);

    //mMainPane = new PaneSubWidget();


}

void PaneWidget::setPane(PaneSubWidget *widget)
{
    //mMainPane->addWidget(widget);
    mMainPane = widget;
    connect(mMainPane,SIGNAL(clickPane(QWidget*)),this,SLOT(onClickSubPane(QWidget*)));
    connect(mMainPane,SIGNAL(releasePane()),this,SLOT(onReleaseSubPane()));
    connect(mMainPane,SIGNAL(movePane(bool)),this,SLOT(onMoveSubPane(bool)));
    connect(mMainPane,SIGNAL(dragPane(CurrentWidget)),this,SLOT(onDragSubPane(CurrentWidget)));

    connect(this,SIGNAL(displayDropHints(bool)),mMainPane,SLOT(onDisplayDropHints(bool)));
    connect(this,SIGNAL(drawSuggestionArea(PanePosition)),mMainPane,SLOT(onDrawSuggestionArea(PanePosition)));

    mMainLayout->addWidget(mMainPane);

}


void PaneWidget::onMoveSubPane(bool showDragHint)
{

    if( mCurrentWidget.InDragProcess)
    {
        QPoint pos = QCursor::pos();

        // display blue region
        QPoint relativePlaneCursor = mCurrentWidget.Parent->mapFromGlobal(pos);
        mCurrentWidget.Parent->onDrawSuggestionArea(Hide);
        mCurrentWidget.Parent->onDrawSuggestionArea(mCurrentWidget.Parent->dropHint(pos));
        // display icon to
        mCurrentWidget.Parent->onDisplayDropHints(true);

        // show minified version of window
        mDragHint->setText(mCurrentWidget.Title);
        mDragHint->setPixmap(QPixmap::grabWidget(mCurrentWidget.Widget));
        mDragHint->move(pos+ QPoint(1, 1));
        if(showDragHint)
        {
            mDragHint->show();
        }
        else
        {
            mDragHint->hide();
        }
    }
}

void PaneWidget::onClickSubPane(QWidget *w)
{
    // it is possible that the user will drag a pane, so remember important things
}

void PaneWidget::onDragSubPane(CurrentWidget w)
{
    mCurrentWidget = w;
}


void PaneWidget::onReleaseSubPane()
{
    QPoint pos = QCursor::pos();

    mDragHint->hide();

    emit displayDropHints(false);
    emit drawSuggestionArea(Hide);

    if(mCurrentWidget.InDragProcess)
    {
        qobject_cast<PaneSubWidget*>(mCurrentWidget.Parent)->addWidget(mCurrentWidget.Widget,mCurrentWidget.Parent->dropHint(pos));

        mDragHint->hide();
        mCurrentWidget.Parent->onDisplayDropHints(false);
        mCurrentWidget.Parent->onDrawSuggestionArea(Hide);
    }

    mCurrentWidget.InDragProcess=false;
}

