#ifndef GRIDVIEW_H
#define GRIDVIEW_H

#include <QWidget>
#include <QPaintEvent>
#include "gobangmanager.h"

class GridView : public QWidget
{
	Q_OBJECT
public:
	explicit GridView(QWidget *parent = 0);
	void paintEvent(QPaintEvent *e) override;
	void mousePressEvent(QMouseEvent *e) override;
	void drawGrid ();
	QPoint getPoint (QPoint pos) const;
	QPoint getCoordinateFromMousePos (QPoint p) const;
	void setManager(const GoBangManager *value);
	void setDangerShow(bool value);

signals:
	void go (QPoint);

private:
	const GoBangManager* manager;
	const int GRID_SIDE = 30;
	const int PIECE_SIZE = GRID_SIDE * 0.8;
	bool dangerShow = false;
};

#endif // GRIDVIEW_H
