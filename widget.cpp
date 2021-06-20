#include "widget.h"

void Widget::setGroupBox()
{
    groupBox = new QGroupBox(this);
    comboBox = new QComboBox(groupBox);

    QVBoxLayout *vBox = new QVBoxLayout;
    vBox->addWidget(comboBox);
    groupBox->setLayout(vBox);
}

void Widget::setInterface()
{
    drawArea = new QWidget(this);

    this->setGeometry(100, 100, 800, 600);
//    drawArea->resize(this->width()*0.75, this->height());

    QHBoxLayout *hBox = new QHBoxLayout;

    this->setGroupBox();

    hBox->addWidget(drawArea, 2);
    hBox->addWidget(groupBox, -1);

    this->setLayout(hBox);
}

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    this->setInterface();
    this->drawArea->installEventFilter(this);
}

Widget::~Widget()
{
    delete drawArea;
    delete comboBox;
    delete groupBox;
}

bool Widget::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::Paint)
    {
        this->paint();
        return true;
    }
    else
        return QObject::eventFilter(obj, event);
}

void Widget::paint()
{
	Canvas canv(this->drawArea, 2);
    canv.drawAxies();
    canv.setPen(Qt::black);
    canv.drawVoronoyDiagram();
}
