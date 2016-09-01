#include "gobangmanager.h"
#include <stdexcept>
#include <cstring>
#include <QDebug>

const int GoBangManager::LU;
const int GoBangManager::BLACK;
const int GoBangManager::WHITE;
const int GoBangManager::NONE;
const int GoBangManager::INVALID;

GoBangManager::GoBangManager()
{
	newRound();
}

void GoBangManager::newRound()
{
	memset(v, 0, sizeof(v));
	nowPlayer = BLACK;
	winner = NONE;
	pointList.clear();
}

int GoBangManager::getLu() const
{
	return LU;
}

bool GoBangManager::inRange(const QPoint &p) const
{
	return p.x() >= 1 && p.x() <= LU && p.y() >= 1 && p.y() <= LU;
}

int GoBangManager::get(const QPoint &p) const
{
	if(!inRange(p))
		return INVALID;
	return v[p.x()][p.y()];
}

void GoBangManager::go(const QPoint &p)
{
	if(!inRange(p))
		throw std::out_of_range("");
	int &pv = v[p.x()][p.y()];
	if(pv != NONE)
		throw std::runtime_error("落子位置已经有子");
	pv = nowPlayer;
	pointList.push_back(p);
	nowPlayer *= -1;
	winner = getStatus();
}

void GoBangManager::back()
{
	if(pointList.empty())
		return;
	QPoint p = pointList.back();
	pointList.pop_back();
	v[p.x()][p.y()] = NONE;
	nowPlayer *= -1;
	winner = getStatus();
}

int GoBangManager::getStatus() const
{
	bool b[3] = {0}, *win = b + 1;
	int count;
	// 水平
	for(int i=1; i<=LU; ++i)
	{
		count = 0;
		for(int j=1; j<=LU; ++j)
		{
			const int &vv = v[i][j];
			if(vv == v[i][j-1])
			{
				count++;
				if(count == 5 && vv != 0)
					win[vv] = true;
			}
			else
				count = 1;
		}
	}
	// 竖直
	for(int j=1; j<=LU; ++j)
	{
		count = 0;
		for(int i=1; i<=LU; ++i)
		{
			const int &vv = v[i][j];
			if(vv == v[i][j-1])
			{
				count++;
				if(count == 5 && vv != 0)
					win[vv] = true;
			}
			else
				count = 1;
		}
	}
	// */
	for(int i=5; i<=LU; ++i)
	{
		count = 0;
		for(int j=1; j<=i; ++j)
		{
			const int &vv = v[i+1-j][j];
			if(vv == v[i+1-(j-1)][j-1])
			{
				count++;
				if(count == 5 && vv != 0)
					win[vv] = true;
			}
			else
				count = 1;
		}
	}
	// /*
	for(int i=5; i<=LU; ++i)
	{
		count = 0;
		for(int j=1; j<=i; ++j)
		{
			const int &vv = v[LU+1 - (i+1-j)][LU+1 - j];
			if(vv == v[LU+1 - (i+1-(j-1))][LU+1 - (j-1)])
			{
				count++;
				if(count == 5 && vv != 0)
					win[vv] = true;
			}
			else
				count = 1;
		}
	}
	// \*
	for(int i=5; i<=LU; ++i)
	{
		count = 0;
		for(int j=1; j<=i; ++j)
		{
			const int &vv = v[i+1-j][LU+1 - j];
			if(vv == v[i+1-(j-1)][LU+1 - (j-1)])
			{
				count++;
				if(count == 5 && vv != 0)
					win[vv] = true;
			}
			else
				count = 1;
		}
	}
	// *\ 0
	for(int i=5; i<=LU; ++i)
	{
		count = 0;
		for(int j=1; j<=i; ++j)
		{
			const int &vv = v[LU+1 - (i+1-j)][j];
			if(vv == v[LU+1 - (i+1-(j-1))][j-1])
			{
				count++;
				if(count == 5 && vv != 0)
					win[vv] = true;
			}
			else
				count = 1;
		}
	}
	if(win[BLACK] && win[WHITE])
		return INVALID;
	if(win[BLACK])
		return BLACK;
	if(win[WHITE])
		return WHITE;
	else
		return NONE;
}

bool GoBangManager::isDangerPoint(const QPoint &p) const
{
	return dangerCount(p) >= 2;
}

int GoBangManager::dangerCount(const QPoint &p) const
{
	if(!inRange(p))
		return 0;
	if(v[p.x()][p.y()] != NONE)
		return 0;

	int dangerCount = 0;
	int dx[] = {1, 0, 1, 1};
	int dy[] = {0, 1, 1, -1};
	for(int i=0; i<4; ++i)
	{
		int len0 = 1, border0 = 0;
		int len; bool border;
		lineCount(p, -nowPlayer, dx[i], dy[i], len, border);	len0 += len, border0 += (int)border;
		lineCount(p, -nowPlayer, -dx[i], -dy[i], len, border);	len0 += len, border0 += (int)border;
//		if((len0 == 3 && border == 0) || (len0 == 4 && border == 1))
		dangerCount += std::max(0, len0 - border0 - 2);
	}
	return dangerCount;
}

void GoBangManager::lineCount(QPoint p0, int player, int dx, int dy, int &len, bool &border) const
{
	len = 0; border = false;
	auto dp = QPoint(dx, dy);
	for(QPoint p = p0 + dp; ; p += dp)
	{
		int vv = v[p.x()][p.y()];	// 不会越界
		if(vv == player)
			++len;
		else
		{
			border = !inRange(p) || (vv == -player);
			return;
		}
	}
}
