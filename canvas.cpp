#include "canvas.h"

QVector<QVector<QPointF> > *Canvas::makeMidlPointsSet(QVector<QPointF> &poins)
{

    QVector< QVector<QPointF> > *midlSet = new QVector< QVector<QPointF> >;
    for (int i = 0; i < pointCount; i++)
    {
        midlSet->append(QVector<QPointF>());
        for (int j = 0; j < pointCount; j++)
            if (j != i)
                (*midlSet)[i].append((poins[i] + poins[j])/2.0f);
    }

    return midlSet;
}

QPolygonF Canvas::createHalfPlain(Canvas::LineParams &params)
{
    const double c = 1;
    QPolygonF tmp;
    if (this->determinePointLocation
            (
                QPointF
                (
                    -this->device()->width()/2.0f + c,
                    this->device()->height()/2.0f - c
                ),
                params
            )
        )
        tmp << QPointF
               (
                   -this->device()->width()/2.0f + c,
                   this->device()->height()/2.0f - c
                );

    if (this->determinePointLocation
            (
                QPointF
                (
                    -this->device()->width()/2.0f + c,
                    -this->device()->height()/2.0f + c
                ),
                params
            )
        )
        tmp << QPointF
               (
                   -this->device()->width()/2.0f + c,
                   -this->device()->height()/2.0f + c
                );

    if (this->determinePointLocation
            (
                QPointF
                (
                    this->device()->width()/2.0f - c,
                    -this->device()->height()/2.0f + c
                ),
                params
            )
        )
        tmp << QPointF
               (
                   this->device()->width()/2.0f - c,
                   -this->device()->height()/2.0f + c
                );

    if (this->determinePointLocation
            (
                QPointF
                (
                    this->device()->width()/2.0f - c,
                    this->device()->height()/2.0f - c
                ),
                params
            )
        )
        tmp << QPointF
               (
                   this->device()->width()/2.0f - c,
                   this->device()->height()/2.0f - c
                );

    double y0, y1;
    y0 = -(params.a/params.b)*(-this->device()->width()/2.0f + c) -
            (params.c/params.b);
    y1 = -(params.a/params.b)*(this->device()->width()/2.0f - c) -
            (params.c/params.b);

    if ((-this->device()->height()/2.0f + c) <= y0 &&
            (this->device()->height()/2.0f - c) >= y0)
        tmp << QPointF(-this->device()->width()/2.0f + c, y0);
    if ((-this->device()->height()/2.0f + c) <= y1 &&
            (this->device()->height()/2.0f - c) >= y1)
        tmp << QPointF(this->device()->width()/2.0f - c, y1);

    double x0, x1;
    x0 = -(params.b/params.a)*(-this->device()->height()/2.0f + c) -
            (params.c/params.a);
    x1 = -(params.b/params.a)*(this->device()->height()/2.0f - c) -
            (params.c/params.a);

    if ((-this->device()->width()/2.0f + c) <= x0 &&
            (this->device()->width()/2.0f - c) >= x0)
        tmp << QPointF(x0, -this->device()->height()/2.0f + c);
    if ((-this->device()->width()/2.0f + c) <= x1 &&
            (this->device()->width()/2.0f - c) >= x1)
        tmp << QPointF(x1, this->device()->height()/2.0f - c);

    return tmp;
}

QPolygonF &Canvas::sortPolygonVertesis(QPolygonF &pol)
{
#ifdef DEBUG_MODE
    qDebug() << "sorting polygons>";
#endif // DEBUG_MODE

    int k = 0;
    double max;
    QPointF corePnt, basePnt;
    QPointF baseVec, itrVec;
    QPolygonF tmpPol1, tmpPol2 = pol;

    basePnt = tmpPol2.first();
    tmpPol1 << tmpPol2.first();
    tmpPol2.remove(0);
    for (int i = 0; i < pol.size(); i++)
        corePnt += pol[i];
    corePnt /= (double)pol.size();

    baseVec = basePnt - corePnt;
    for (int i = 2; i < pol.size(); i++)
    {
        max = 2 * 3.1415926;
        for (int j = 0; j < tmpPol2.size(); j++)
        {
            itrVec = tmpPol2[j] - corePnt;
            if (this->angleBetwin(baseVec, itrVec)< max &&
                    (baseVec.x()*itrVec.y() - itrVec.x()*baseVec.y()) < 0)
            {
                max = this->angleBetwin(baseVec, itrVec);
                k = j;
            }
        }
#ifdef DEBUG_MODE
        qDebug() << "pol2 size:" << tmpPol2.size() << "k:" << k;
#endif // DEBUG_MODE
        tmpPol1 << tmpPol2[k];
        basePnt = tmpPol2[k];
        tmpPol2.remove(k);
        baseVec = basePnt - corePnt;
    }
    tmpPol1 << tmpPol2.first();

    pol = tmpPol1;
    return pol;
}

QVector<QPolygonF> Canvas::cutPolygonByLine(Canvas::LineParams lp,
                                            QPolygonF poly)
{
    double xa, ya, wa;
    LineParams innerLp;
    QVector<double> U1, U2;
    QPolygonF p;
    for (int i = 1; i < poly.size(); i++)
    {
        U1.append(lp.a*poly[i - 1].x() + lp.b*poly[i - 1].y() + lp.c);
        U2.append(lp.a*poly[i].x() + lp.b*poly[i].y() + lp.c);
        if (!(!(U1[i - 1] > 0) && !(U2[i - 1] > 0)))
        {
            if (!(U1[i - 1] < 0) && !(U2[i - 1] < 0))
            {
                p.append(poly[i - 1]);
                p.append(poly[i]);
            }
            else
            {
                innerLp.a = poly[i - 1].y() - poly[i].y();
                innerLp.b = poly[i - 1].x() - poly[i].x();
                innerLp.c = poly[i - 1].x()*poly[i].y() -
                        poly[i - 1].y()*poly[i].x();

                xa = -(innerLp.b*lp.c - lp.b*lp.c);
                ya = -(innerLp.a*lp.c - lp.a*lp.c);
                wa = -(innerLp.a*lp.b - lp.a*innerLp.b);
            }
        }
    }
}

Canvas::Canvas(QPaintDevice *device, int pointCount) :
    QPainter(device),
    pointCount(pointCount)
{
#ifdef DEBUG_MODE
    qDebug() << "constructor>";
    if (this->device() != NULL)
    {
        qDebug() << "widget size:";
        qDebug() << this->device()->width();
        qDebug() << this->device()->height();
    }
#endif // DEBUG_MODE
}

void Canvas::drawAxies()
{
    this->translate(this->device()->width()/2.0f,
                    this->device()->height()/2.0f);

    this->setPen(Qt::red);
    this->drawLine(this->getScreenCoords(QPointF(0, 0)),
                this->getScreenCoords(QPointF(100, 0)));

    this->setPen(Qt::yellow);
    this->drawLine(this->getScreenCoords(QPointF(0, 0)),
                this->getScreenCoords(QPointF(0, 100)));
}

void Canvas::drawVoronoyDiagram()
{
    srand(time(NULL));
    qDebug() << "drawVoronoyDiagram>";
    QVector<QPointF> poins;
    for (int i = 0; i < pointCount; i++)
        poins.append(this->makeRandPoint());
    qDebug() << "poins randomized";

    QVector< QVector<QPointF> > *midlSet;
    midlSet = this->makeMidlPointsSet(poins);
    qDebug() << "midl poins set";

    QVector< QVector<LineParams> > linesParams;
    for (int i = 0; i < midlSet->size(); i++)
    {
        linesParams.append(QVector<LineParams>());
        for (int j = 0; j < (*midlSet)[i].size(); j++)
        {
            linesParams[i].append(LineParams());
            linesParams[i][j].a = poins[i].x() - (*midlSet)[i][j].x();
            linesParams[i][j].b = poins[i].y() - (*midlSet)[i][j].y();
            linesParams[i][j].c = -linesParams[i][j].a*(*midlSet)[i][j].x()
                                    -linesParams[i][j].b*(*midlSet)[i][j].y();
        }
    }
    qDebug() << "lines params made";

    QVector< QVector<QPolygonF> > poligonsSet;
    for (int i = 0; i < linesParams.size(); i++)
    {
#ifdef DEBUG_MODE
                qDebug() << "point: " << i;
#endif // DEBUG_MODE
        poligonsSet.append(QVector<QPolygonF>());
        for (int j = 0; j < linesParams[i].size(); j++)
        {
            poligonsSet[i].append(this->createHalfPlain(linesParams[i][j]));
#ifdef DEBUG_MODE
            qDebug() << "polygon: " << j;
            for (int k = 0; k < poligonsSet[i][j].size(); k++)
                qDebug() << "vertex: " << k << poligonsSet[i][j][k];
#endif // DEBUG_MODE
        }
    }

    for (int i = 0; i < poligonsSet.size(); i++)
        for (int j = 0; j < poligonsSet[i].size(); j++)
            this->sortPolygonVertesis(poligonsSet[i][j]);

#ifdef DEBUG_MODE
                qDebug() << "drawing>>>>>>";
#endif // DEBUG_MODE
    for (int i = 0; i < linesParams.size(); i++)
    {
        this->drawEllipse(this->getScreenCoords(poins[i]), 1, 1);
#ifdef DEBUG_MODE
        qDebug() << "point: " << i << " x: " << poins[i].x()
                 << "y: " << poins[i].y();
#endif // DEBUG_MODE
        for (int j = 0; j < linesParams[i].size(); j++)
        {
#ifdef DEBUG_MODE
                qDebug() << "polygon: " << j;
#endif // DEBUG_MODE
            for (int k = 0; k < poligonsSet[i][j].size(); k++)
            {
#ifdef DEBUG_MODE
                qDebug() << "vertex: " << k << poligonsSet[i][j][k];
#endif // DEBUG_MODE
                poligonsSet[i][j][k] =
                        this->getScreenCoords(poligonsSet[i][j][k]);
            }
            this->drawConvexPolygon(poligonsSet[i][j]);
        }
    }

    this->setPen(Qt::blue);
    QPolygonF t;
    t << getScreenCoords(QPointF(20, 20))
      << getScreenCoords(QPointF(20, -20))
      << getScreenCoords(QPointF(-20, -20))
      << getScreenCoords(QPointF(-20, 20));
    t.clear();
    t << getScreenCoords(QPointF(20, 20))
      << getScreenCoords(QPointF(20, -20))
      << getScreenCoords(QPointF(-20, 20))
      << getScreenCoords(QPointF(-20, -20));
    this->drawConvexPolygon(t);
    this->setPen(Qt::green);
    this->drawLine(getScreenCoords(QPointF(-100, -289)),
                                getScreenCoords(QPointF(100, -289)));
}
