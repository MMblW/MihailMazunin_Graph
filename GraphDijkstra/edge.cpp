#include "edge.h"
#include "graphvertex.h"

#include <qmath.h>
#include <QPainter>

Edge::Edge(graphVertex *sourceNode, graphVertex *destNode)
    : arrowSize(8)
{
    setFlag(ItemIsSelectable);
    source = sourceNode;
    dest = destNode;
    source->addEdge(this);
    dest->addEdge(this);
    adjust();
}

Edge::~Edge()
{
    source->removeEdge(this);
    dest->removeEdge(this);
    int node1 = source->getNum(), node2 = dest->getNum();
    graph->eraseEdge(node1, node2);
}

graphVertex *Edge::sourceNode() const
{
    return source;
}

graphVertex *Edge::destNode() const
{
    return dest;
}

void Edge::setContent(int num)
{
    content = QString::number(num);
}

void Edge::adjust()
{
    if (!source || !dest)
        return;

    QLineF line(mapFromItem(source, 0, 0), mapFromItem(dest, 0, 0));
    qreal length = line.length();

    prepareGeometryChange();

    if (length > qreal(20.)) {
        QPointF edgeOffset((line.dx() * 10) / length, (line.dy() * 10) / length);
        sourcePoint = line.p1() + edgeOffset;
        destPoint = line.p2() - edgeOffset;
    } else {
        sourcePoint = destPoint = line.p1();
    }
}

void Edge::setGraph(Graph *graph)
{
    this->graph = graph;
}

QRectF Edge::boundingRect() const
{
    if (!source || !dest)
        return QRectF();

    qreal penWidth = 3;
    qreal extra = (penWidth + arrowSize) / 2.0;

    return QRectF(sourcePoint,
                  QSizeF(destPoint.x() - sourcePoint.x(), destPoint.y() - sourcePoint.y()))
        .normalized().adjusted(-extra, -extra, extra, extra);
}

void Edge::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    if (!source || !dest)
        return;

    QLineF line(sourcePoint, destPoint);
    if (qFuzzyCompare(line.length(), qreal(0.)))
        return;
    painter->setPen(QPen(Qt::black, 2, Qt::SolidLine));
    painter->drawLine(line);
    painter->drawText((sourcePoint*0.8+destPoint)/2, content);
    double angle = std::atan2(-line.dy(), line.dx());

    QPointF sourceArrowP1 = sourcePoint + QPointF(sin(angle + M_PI / 3) * arrowSize,
                                                  cos(angle + M_PI / 3) * arrowSize);
    QPointF sourceArrowP2 = sourcePoint + QPointF(sin(angle + M_PI - M_PI / 3) * arrowSize,
                                                  cos(angle + M_PI - M_PI / 3) * arrowSize);

    painter->setBrush(Qt::black);
    painter->drawPolygon(QPolygonF() << line.p1() << sourceArrowP1 << sourceArrowP2);

}

