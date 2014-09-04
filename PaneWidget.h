#ifndef PANEWIDGET_H
#define PANEWIDGET_H

#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>
#include "PaneMeta.h"
#include <QRubberBand>



class PaneWidget : public QWidget
{
    Q_OBJECT

    QVBoxLayout* mMainLayout;
    PaneSubWidget *mMainPane;
    QLabel *mDragHint;
    CurrentWidget mCurrentWidget;


public:
    explicit PaneWidget(QWidget *parent = 0);

    void setPane(PaneSubWidget *widget);
protected:

signals:
    void displayDropHints(bool visible);
    void drawSuggestionArea(PanePosition pos);
public slots:
    void onClickSubPane(QWidget *w);
    void onMoveSubPane( bool showDragHint);
    void onReleaseSubPane();
    void onDragSubPane(CurrentWidget w);
};

#endif // PANEWIDGET_H
