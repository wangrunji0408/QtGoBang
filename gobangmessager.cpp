#include "gobangmessager.h"
#include <QMessageBox>
#include <QByteArray>
#include <QDataStream>

const int GoBangMessager::GO;
const int GoBangMessager::NEW_ROUND;
const int GoBangMessager::SET_PLAYER;
const int GoBangMessager::ADMIT_DEFEAT;
const int GoBangMessager::MESSAGE;

GoBangMessager::GoBangMessager(QObject *parent) : QObject(parent)
{

}


void GoBangMessager::createHost()
{
	listener = new QTcpServer(this);
	listener->listen(QHostAddress::Any, port);
	connect(listener, SIGNAL(newConnection()), this, SLOT(newConnection()));
	logger.info() << tr("成功创建主机，端口：") << port;
}

void GoBangMessager::closeHost()
{
	if(listener == nullptr)
	{
		logger.warning() << tr("尝试关闭主机，然而并不是主机");
		return;
	}
	listener->close();
	delete listener;
	listener = nullptr;
	logger.info() << tr("关闭主机");
}

bool GoBangMessager::connectToHost(QHostAddress address)
{
	logger.info() << tr("尝试连接主机：") << address.toString();
	if(listener != nullptr)
	{
		logger.warning() << tr("连接主机失败，已经是主机模式。");
		return false;
	}
	socket = new QTcpSocket;
	socket->connectToHost(address, port);
	connect(socket, SIGNAL(readyRead()), this, SLOT(receiveMessage()));
	connect(socket, &QTcpSocket::connected,
			[=]()
			{
				logger.info() << tr("成功连接主机");
				emit this->connected();
			});
	connect(socket, &QTcpSocket::disconnected,
			[=]()
			{
				logger.info() << tr("与主机断开连接");
				emit this->disconnected();
			});
	if(socket->waitForConnected(1000))
		return true;
	else
	{
		logger.info() << tr("连接超时");
	}
}

void GoBangMessager::disconnect()
{
	if(socket == nullptr)
	{
		logger.warning() << tr("正尝试断开连接，但没有存在的连接");
		return;
	}
	socket->close();
}

void GoBangMessager::sendGo(QPoint p)
{
	QByteArray bytes;
	QDataStream stm(&bytes, QIODevice::ReadWrite);
	stm << GO << p;
	socket->write(bytes);
	logger.info() << tr("本机落子：") << p;
}

void GoBangMessager::sendNewRound()
{
	QByteArray bytes;
	QDataStream stm(&bytes, QIODevice::ReadWrite);
	stm << NEW_ROUND;
	socket->write(bytes);
	logger.info() << tr("本机新开局");
}

void GoBangMessager::sendSetPlayer(int player)
{
	QByteArray bytes;
	QDataStream stm(&bytes, QIODevice::ReadWrite);
	stm << SET_PLAYER << player;
	socket->write(bytes);
	logger.info() << tr("本机设置己方黑白手：") << player;
}

void GoBangMessager::sendAdmitDefeat()
{
	QByteArray bytes;
	QDataStream stm(&bytes, QIODevice::ReadWrite);
	stm << ADMIT_DEFEAT;
	socket->write(bytes);
	logger.info() << tr("本机认输");
}

void GoBangMessager::sendMessage(QString message)
{
	QByteArray bytes;
	QDataStream stm(&bytes, QIODevice::ReadWrite);
	stm << MESSAGE << message;
	socket->write(bytes);
	logger.info() << tr("本机发送消息：") << message;
}

bool GoBangMessager::isHost() const
{
	return listener != nullptr;
}

void GoBangMessager::acceptClient()
{
	if(!waitingForAccept)
	{
		logger.warning() << tr("试图接受客户端连接，然而此时并没有连接请求");
		return;
	}
	waitingForAccept = false;
	connect(socket, SIGNAL(readyRead()), this, SLOT(receiveMessage()));
	connect(socket, &QTcpSocket::disconnected,
			[=]()
			{
				listener->resumeAccepting();
				logger.info() << tr("与客户端断开连接， IP地址：") << socket->peerAddress().toString();
				emit this->disconnected();
			});
	logger.info() << tr("接受对局请求，IP地址：") << socket->peerAddress().toString();
	emit connected();
}

void GoBangMessager::rejectClient()
{
	if(!waitingForAccept)
	{
		logger.warning() << tr("试图拒绝客户端连接，然而此时并没有连接请求");
		return;
	}
	waitingForAccept = false;
	connect(socket, &QTcpSocket::disconnected,
			[=]()
			{
				listener->resumeAccepting();
				logger.info() << tr("与客户端断开连接， IP地址：") << socket->peerAddress().toString();
			});
	logger.info() << tr("拒绝了来自") << socket->peerAddress().toString() << tr("的对局请求");
	disconnect();
}

void GoBangMessager::newConnection()
{
	listener->pauseAccepting();
	socket = listener->nextPendingConnection();
	waitingForAccept = true;
	logger.info() << tr("收到对局请求，来自：") << socket->peerAddress().toString();
	emit newClient(socket->peerAddress());
}

void GoBangMessager::receiveMessage()
{
//	qDebug() << "收到对方信息";
	QDataStream data(socket->readAll());
	int type;
	data >> type;
	if(type == GO)
	{
		QPoint p;
		data >> p;
		logger.info() << tr("对方落子：") << p;
		emit opponentGo(p);
	}
	else if(type == NEW_ROUND)
	{
		logger.info() << tr("对方新开局");
		emit opponentNewRound();
	}
	else if(type == SET_PLAYER)
	{
		int player;
		data >> player;
		player *= -1;
		logger.info() << tr("对方设置黑白手，目前本机为：") << player;
		emit opponentSetPlayer(player);
	}
	else if(type == ADMIT_DEFEAT)
	{
		logger.info() << tr("对方认输");
		emit opponentAdmitDefeat();
	}
	else if(type == MESSAGE)
	{
		QString message;
		data >> message;
		logger.info() << tr("对方向你发送一条消息");
		emit opponentMessage(message);
	}
}
