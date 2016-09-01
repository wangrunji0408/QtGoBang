#include "gridview.h"
#include <QPainter>
#include <QDebug>

GridView::GridView(QWidget *parent) : QWidget(parent)
{
}

void GridView::paintEvent(QPaintEvent *)
{
	setAutoFillBackground(true);
	QPainter painter(this);
	painter.setPen(Qt::black);
	const int LU = manager->getLu();
	for(int i=1; i<=LU; ++i)
	{
		painter.drawLine(getPoint(QPoint(1, i)), getPoint(QPoint(LU, i)));
		painter.drawLine(getPoint(QPoint(i, 1)), getPoint(QPoint(i, LU)));
	}
	painter.setBrush(Qt::black);
	painter.drawEllipse(getPoint(QPoint(8, 8)), 2, 2);
	painter.drawEllipse(getPoint(QPoint(8-4, 8-4)), 2, 2);
	painter.drawEllipse(getPoint(QPoint(8+4, 8+4)), 2, 2);
	painter.drawEllipse(getPoint(QPoint(8-4, 8+4)), 2, 2);
	painter.drawEllipse(getPoint(QPoint(8+4, 8-4)), 2, 2);

	for(int i=1; i<=LU; ++i)
		for(int j=1; j<=LU; ++j)
		{
			int player = manager->get(QPoint(i, j));
			if(player == GoBangManager::NONE)
			{
//				int count = manager->dangerCount(QPoint(i, j));
				if(dangerShow && manager->isDangerPoint(QPoint(i, j)))
				{
					painter.setBrush(Qt::red);
					painter.drawEllipse(getPoint(QPoint(i, j)), 5, 5);
				}
				continue;
			}
			painter.setBrush(player == GoBangManager::BLACK? Qt::black: Qt::white);
			painter.drawEllipse(getPoint(QPoint(i, j)), PIECE_SIZE/2, PIECE_SIZE/2);
		}
	//	painter.drawPoint(getPoint(QPoint((LU+1)/2, (LU+1)/2)));
}

void GridView::mousePressEvent(QMouseEvent *e)
{
	QPoint p = getCoordinateFromMousePos(e->pos());
	if(manager->get(p) == GoBangManager::NONE)
		emit go(p);
	QWidget::mousePressEvent(e);
}

QPoint GridView::getPoint(QPoint p) const
{
	return p * GRID_SIDE;
}

QPoint GridView::getCoordinateFromMousePos(QPoint p) const
{
	QPoint nearest = p / GRID_SIDE;
	return nearest;
}

void GridView::setManager(const GoBangManager *value)
{
	manager = value;
}

void GridView::setDangerShow(bool value)
{
	dangerShow = value;
}
