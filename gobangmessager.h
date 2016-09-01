#ifndef GOBANGMESSAGER_H
#define GOBANGMESSAGER_H

#include <QObject>
#include <QtNetwork/QTcpServer>
#include <QtNetwork/QTcpSocket>
#include <QMessageLogger>
#include <QPoint>

class GoBangMessager : public QObject
{
	Q_OBJECT
public:
	explicit GoBangMessager(QObject *parent = 0);
	void createHost ();
	void closeHost ();
	bool connectToHost (QHostAddress address);
	void disconnect();
//	QHostAddress getLocalAddress() const;
//	QHostAddress getPeerAddress() const;
	void sendGo (QPoint);
	void sendNewRound();
	void sendSetPlayer (int player);
	void sendAdmitDefeat ();
	void sendMessage (QString message);
	bool isHost () const;
	void acceptClient ();
	void rejectClient ();

private slots:
	void newConnection ();
	void receiveMessage ();

signals:
	void connected ();
	void disconnected ();
	void newClient (QHostAddress);
	void opponentGo (QPoint);
	void opponentNewRound ();
	void opponentSetPlayer (int);
	void opponentAdmitDefeat ();
	void opponentMessage (QString);

private:
	const quint16 port = 2333;
	QTcpServer* listener = nullptr;
	QTcpSocket* socket = nullptr;
	QMessageLogger logger;
	bool waitingForAccept = false;

	static const int GO = 1;
	static const int NEW_ROUND = 2;
	static const int SET_PLAYER = 3;
	static const int ADMIT_DEFEAT = 4;
	static const int MESSAGE = 5;
};

#endif // GOBANGMESSAGER_H
