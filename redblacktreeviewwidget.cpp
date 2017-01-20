#include "redblacktreeviewwidget.h"
#include "ui_redblacktreeviewwidget.h"

using namespace std;

QPainter *uniPainter;
int radius = 10;

void drawRNode(QPainter &p, int x, int y, int r, QString s)
{
    //draw circle
    p.setPen(QPen(Qt::black, 2, Qt::SolidLine, Qt::RoundCap));
    p.setBrush(QBrush(Qt::red, Qt::SolidPattern));
    p.drawEllipse(x, y, 2 * r, 2 * r);
    //draw text
    p.setPen(QPen(Qt::white, 2, Qt::SolidLine, Qt::RoundCap));
    p.drawText(QRect(x, y, 2 * r, 2 * r), Qt::AlignCenter, s);
}

void drawBNode(QPainter &p, int x, int y, int r, QString s)
{
    //draw circle
    p.setPen(QPen(Qt::black, 2, Qt::SolidLine, Qt::RoundCap));
    p.setBrush(QBrush(Qt::black, Qt::SolidPattern));
    p.drawEllipse(x, y, 2 * r, 2 * r);
    //draw text
    p.setPen(QPen(Qt::white, 2, Qt::SolidLine, Qt::RoundCap));
    p.drawText(QRect(x, y, 2 * r, 2 * r), Qt::AlignCenter, s);
}

void drawFindRstCircle(QPainter &p, int x, int y, int r)
{
    //draw circle
    p.setPen(QPen(Qt::yellow, 4, Qt::SolidLine, Qt::RoundCap));
    p.setBrush(Qt::transparent);
    p.drawEllipse(x, y, 2 * r, 2 * r);
}

void eraseTree(QPainter &p)
{
    p.setPen(QPen(Qt::white, 2, Qt::SolidLine, Qt::RoundCap));
    p.setBrush(QBrush(Qt::white, Qt::SolidPattern));
    p.drawRect(0, 0, 2048, 2048);
}

void drawLinkLine(QPainter &p, int x1, int y1, int x2, int y2, int r)
{
    //draw line
    p.setPen(QPen(Qt::blue, 2, Qt::SolidLine, Qt::RoundCap));
    p.setBrush(QBrush(Qt::blue, Qt::SolidPattern));
    p.drawLine(x1 + r, y1 + r, x2 + r, y2 + r);
}

void visit(const BinNode<int> &node)
{
    //cout << node.data << "  level: " << node.level << endl;
    //draw line
    if(!IsRoot(node))
        drawLinkLine(*uniPainter, node.parent->x, node.parent->y, node.x, node.y, radius);
    //draw node
    if(node.color == RB_BLACK)
        drawBNode(*uniPainter, node.x, node.y, radius, QString::number(node.data));
    else
        drawRNode(*uniPainter, node.x, node.y, radius, QString::number(node.data));
    //debug
    cout << "node:" << node.data << " x, y: " << node.x <<", " << node.y << endl;
}

RedblackTreeViewWidget::RedblackTreeViewWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RedblackTreeViewWidget),
    isFindRst(false)
{
    ui->setupUi(this);
}

RedblackTreeViewWidget::~RedblackTreeViewWidget()
{
    delete ui;
}

void RedblackTreeViewWidget::setMargins(int l, int s, int m, int lm)
{
    this->levelMargin = l;
    this->sibMargin = s;
    this->margin = m;
    this->leftMargin = lm;
}

void RedblackTreeViewWidget::paintEvent(QPaintEvent *e)
{
    Q_UNUSED(e);

    QPainter painter(this);
    uniPainter = &painter;
    painter.setRenderHint(QPainter::Antialiasing, true);
    //clear
    eraseTree(*uniPainter);
    //update
    rb.updatePosition(this->levelMargin, this->sibMargin, this->margin, radius, this->leftMargin);
    //draw
    rb.travPost(visit);
    //find?
    if(this->isFindRst)
        drawFindRstCircle(*uniPainter, this->findRst->x, this->findRst->y, radius);
}


void RedblackTreeViewWidget::setRadius(int r)
{
    radius = r;
}
