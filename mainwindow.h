#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtNetwork/QTcpServer>
#include <QtNetwork/QTcpSocket>
#include <QMessageLogger>
#include "gobangmessager.h"
#include "gobangmanager.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();

private slots:
	// Button
	void on_disconnect_clicked();
	void on_connect_clicked();
	// Connect
	void connected ();
	void disconnected ();
	void newClient (QHostAddress ip);
	// Local
	void localGo (QPoint p);
	void localNewRound ();
	void localSetPlayer (int player);
	void localAdmitDefeat ();
	void localMessage (QString message);
	// Remote
	void remoteGo (QPoint p);
	void remoteNewRound ();
	void remoteSetPlayer (int player);
	void remoteAdmitDefeat ();
	void remoteMessage (QString message);
	// Show
	void showIP ();
	void showInfo (const QString &info);
	// Round
	void checkStatus ();
	void newRound ();
	void endRound ();


	void on_player_activated(int index);
	void on_newRound_clicked();
	void on_danger_clicked(bool checked);
	void on_create_clicked(bool checked);
	void on_admitDefeat_clicked();

	void on_lineEdit_returnPressed();

private:
	Ui::MainWindow *ui;
	GoBangManager* manager;
	GoBangMessager* messager;
	QMessageLogger logger;
	int player;
};

#endif // MAINWINDOW_H
