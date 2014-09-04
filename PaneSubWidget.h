#ifndef PANE_H
#define PANE_H
#include "PaneMeta.h"
#include <QTabWidget>
#include <QLabel>
#include <QRubberBand>

class PaneSubWidget : public QTabWidget
{
    Q_OBJECT

    bool mDragIsPossible;
    QLabel *mMiddleHintImg;
    QLabel *mLeftHintImg,*mRightHintImg,*mTopHintImg,*mBottomHintImg;
    QRubberBand* mSuggestionArea;
    QWidget *mParentWidget;
    bool mDrawDropHints;
    bool startDragging;
    bool isDragging;
    QString mIdName;

public:
    explicit PaneSubWidget(QWidget *parent = 0);
    PanePosition dropHint(QPoint pt);
    void addWidget(QWidget *widget, const PanePosition Pos=TabIn);
    QString windowTitle() const;
    void setIdName(QString n);
    QString idName() const;
    QSplitter *createSplitter(QWidget *A, QWidget *B, QWidget *parent, bool vertical=false);
    void replaceTab(QWidget *find, QWidget *replace);
signals:

    void clickPane(QWidget *widget);
    void movePane(bool showDragHint);
    void dragPane(CurrentWidget);
    void releasePane();

public slots:
    void onDisplayDropHints(bool visible);
    void onDrawSuggestionArea(PanePosition pos);
    void onClickSubPane(QWidget *w);
    void onDragSubPane(CurrentWidget w);
    void onMoveSubPane(bool inTabbar);
    void onReleaseSubPane();
protected:
    virtual bool eventFilter(QObject *object, QEvent *event);


};

#endif // PANETAB_H
