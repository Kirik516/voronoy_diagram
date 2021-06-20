#ifndef WIDGET_H
#define WIDGET_H

#include "canvas.h"
#include <QComboBox>
#include <QEvent>
#include <QHBoxLayout>
#include <QGroupBox>

class Widget : public QWidget
{
    Q_OBJECT

    QWidget *drawArea;
    QComboBox *comboBox;
    QGroupBox *groupBox;

    void setGroupBox();
    void setInterface();

public:
    Widget(QWidget *parent = 0);
    ~Widget();
    virtual bool eventFilter(QObject *obj, QEvent *event);
    void paint();
};

#endif // WIDGET_H
