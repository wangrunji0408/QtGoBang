#ifndef GOBANGMANAGER_H
#define GOBANGMANAGER_H

#include <QPoint>
#include <QVector>

class GoBangManager
{
public:
	static const int LU = 15;
	static const int BLACK = 1;
	static const int WHITE = -1;
	static const int NONE = 0;
	static const int INVALID = 2;
public:
	GoBangManager();
	void newRound ();
	int getLu () const;
	bool inRange (const QPoint &p) const;
	int get (const QPoint &p) const;
	void go (const QPoint &p);
	void back ();
	bool isDangerPoint (const QPoint &p) const;
	int dangerCount (const QPoint &p) const;
	int getStatus () const;

private:
	void lineCount (QPoint p, int player, int dx, int dy, int &len, bool &border) const;

private:
	int nowPlayer;
	int winner;
	int v[LU + 2][LU + 2];
	QVector<QPoint> pointList;
};

//const int GoBangManager::LU;

#endif // GOBANGMANAGER_H
