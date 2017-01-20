#ifndef REDBLACKTREEVIEWWIDGET_H
#define REDBLACKTREEVIEWWIDGET_H

#include <QWidget>
#include <QPainter>
#include <QString>
#include <string>
#include "redblack.h"
#include "binnode.h"

using namespace std;

namespace Ui {
class RedblackTreeViewWidget;
}

class RedblackTreeViewWidget : public QWidget
{
    Q_OBJECT

public:
    explicit RedblackTreeViewWidget(QWidget *parent = 0);
    ~RedblackTreeViewWidget();
    friend class MainWindow;
    void setMargins(int l, int s, int m, int lm);
    void setIsFindRst(bool b) {isFindRst = b;}
    void setFindRst(BinNodePosi(int) r) {findRst = r;}
    void setRadius(int r);

protected:
    void paintEvent(QPaintEvent *e);

private:
    Ui::RedblackTreeViewWidget *ui;
    RedBlack<int> rb;
    int levelMargin;
    int sibMargin;
    int margin;
    int leftMargin;
    bool isFindRst;
    BinNodePosi(int) findRst;
};

#endif // REDBLACKTREEVIEWWIDGET_H
