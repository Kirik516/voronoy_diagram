#ifndef CANVAS_H
#define CANVAS_H

#include <cstdlib>
#include <ctime>
#include <QtCore/qmath.h>
#include <QPainter>
#include <QPaintEngine>
#include <QPointF>

#define DEBUG_MODE

#ifdef DEBUG_MODE
#include <QDebug>
#endif

class Canvas : public QPainter
{
//    Q_OBJECT

    struct LineParams
    {
        double a, b, c;
    };

    int pointCount;

    double angleBetwin(QPointF &a, QPointF &b)
    {
        return qAcos((a.x()*b.x() + a.y()*b.y())/
                     (qSqrt(a.x()*a.x() + a.y()*a.y())*
                     qSqrt(b.x()*b.x() + b.y()*b.y())));
    }
    QPointF getScreenCoords(QPointF &p) {return QPointF(p.x(), -p.y());}
    QPointF getScreenCoords(const QPointF &p) {return QPointF(p.x(), -p.y());}
    QPointF makeRandPoint()
    {
        double x = -this->device()->width()/2.0f +
                    ((double)rand()/(double)RAND_MAX)*this->device()->width();
        double y = -this->device()->height()/2.0f +
                    ((double)rand()/(double)RAND_MAX)*this->device()->height();
        return QPointF(x, y);
    }
    QVector<QVector<QPointF> > *makeMidlPointsSet(QVector<QPointF> &poins);
    bool determinePointLocation(const QPointF &point, LineParams &params)
    {
        if ((params.a*point.x() + params.b*point.y() + params.c) > 0)
            return true;
        return false;
    }
    QPolygonF createHalfPlain(LineParams &params);
    QPolygonF &sortPolygonVertesis(QPolygonF &pol);
    QVector<QPolygonF> cutPolygonByLine(LineParams lp, QPolygonF poly);

public:
    explicit Canvas(QPaintDevice *device = 0, int pointCount = 2);
    void drawAxies();
    void drawVoronoyDiagram();

};

#endif // CANVAS_H
