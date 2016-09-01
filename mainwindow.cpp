#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "ipdialog.h"

#include <QPainter>
#include <QRect>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);
	manager = new GoBangManager;
	messager = new GoBangMessager;
	ui->gird->setManager(manager);

	connect(ui->gird, SIGNAL(go(QPoint)), this, SLOT(localGo(QPoint)));
	connect(messager, SIGNAL(connected()), this, SLOT(connected()));
	connect(messager, SIGNAL(disconnected()), this, SLOT(disconnected()));
	connect(messager, SIGNAL(newClient(QHostAddress)), this, SLOT(newClient(QHostAddress)));
	connect(messager, SIGNAL(opponentGo(QPoint)), this, SLOT(remoteGo(QPoint)));
	connect(messager, SIGNAL(opponentNewRound()), this, SLOT(remoteNewRound()));
	connect(messager, SIGNAL(opponentSetPlayer(int)), this, SLOT(remoteSetPlayer(int)));
	connect(messager, SIGNAL(opponentAdmitDefeat()), this, SLOT(remoteAdmitDefeat()));
	connect(messager, SIGNAL(opponentMessage(QString)), this, SLOT(remoteMessage(QString)));
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::showIP()
{
//	ui->localAddress->setText(tr("本机地址：\n") + socket->localAddress().toString() +
	//							  tr("\n对方地址：\n") + socket->peerAddress().toString());
}

void MainWindow::showInfo(const QString &info)
{
	ui->listWidget->addItem(info);
}

void MainWindow::checkStatus()
{
	int status = manager->getStatus();
	if(status == GoBangManager::NONE)
		return;
	if(status == player)
	{
		QMessageBox::information(this, tr("对局结束"), tr("您赢了！"));
		showInfo(tr("对局结束，您赢了！"));
	}
	else if(status == -player)
	{
		QMessageBox::information(this, tr("对局结束"), tr("您输了！"));
		showInfo(tr("对局结束，您输了！"));
	}
	else if(status == GoBangManager::INVALID)
	{
		QMessageBox::warning(this, tr("对局结束"), tr("出错了！"));
		showInfo(tr("对局结束，出错了！"));
	}
	endRound();
}

void MainWindow::newRound()
{
	manager->newRound();
	ui->gird->update();
	ui->gird->setEnabled(player == GoBangManager::BLACK);
	ui->player->setEnabled(false);
	ui->admitDefeat->setEnabled(true);
	ui->newRound->setEnabled(false);
}

void MainWindow::endRound()
{
	ui->gird->setEnabled(false);
	ui->admitDefeat->setEnabled(false);
	ui->player->setEnabled(true);
	ui->newRound->setEnabled(true);
}

void MainWindow::on_disconnect_clicked()
{
	messager->disconnect();
}

void MainWindow::on_connect_clicked()
{
	auto dialog = new IPDialog(this);
	if(dialog->exec() == QDialog::Rejected)
		return;
	bool result = messager->connectToHost(dialog->getAddress());
	if(!result)
	{
		QMessageBox::warning(this, tr("连接失败"), tr("连接主机失败"));
		return;
	}
	ui->create->setEnabled(false);
	ui->connect->setEnabled(false);
}

void MainWindow::connected()
{
	ui->create->setEnabled(false);
	ui->connect->setEnabled(false);
	ui->disconnect->setEnabled(true);
	ui->newRound->setEnabled(true);
	ui->player->setEnabled(true);
	ui->lineEdit->setEnabled(true);
	if(messager->isHost())
		localSetPlayer(GoBangManager::BLACK);
	showInfo(tr("连接建立"));
}

void MainWindow::disconnected()
{
	endRound();
	ui->create->setEnabled(true);
	ui->connect->setEnabled(!messager->isHost());
	ui->disconnect->setEnabled(false);
	ui->newRound->setEnabled(false);
	ui->player->setEnabled(false);
	ui->lineEdit->setEnabled(false);
	QMessageBox::information(this, tr("连接断开"), tr("连接断开"));
	showInfo(tr("连接断开"));
}

void MainWindow::newClient(QHostAddress ip)
{
	int result = QMessageBox::question(this, tr("新的连接请求"), tr("是否接受来自") + ip.toString() + tr("的连接请求？"));
	if(result == QMessageBox::Yes)
	{
		messager->acceptClient();
		showInfo(tr("接受了连接请求"));
	}
	else
	{
		messager->rejectClient();
		showInfo(tr("拒绝了连接请求"));
	}
}

void MainWindow::localGo(QPoint p)
{
	manager->go(p);
	messager->sendGo(p);
	ui->gird->setEnabled(false);
	checkStatus();
}

void MainWindow::localNewRound()
{
	newRound();
	messager->sendNewRound();
	showInfo(tr("您开启了新对局"));
}

void MainWindow::localSetPlayer(int player)
{
	this->player = player;
	messager->sendSetPlayer(player);
	ui->player->setCurrentIndex(player == GoBangManager::BLACK? 0: 1);
	showInfo(tr("您设置了黑白手"));
}

void MainWindow::localAdmitDefeat()
{
	messager->sendAdmitDefeat();
	QMessageBox::information(this, tr("对局结束"), tr("您认输了"));
	showInfo(tr("对局结束，您认输了！"));
	endRound();
}

void MainWindow::localMessage(QString message)
{
	messager->sendMessage(message);
	showInfo(tr("<< ") + message);
}

void MainWindow::remoteGo(QPoint p)
{
	manager->go(p);
	ui->gird->setEnabled(true);
	ui->gird->update();
	checkStatus();
}

void MainWindow::remoteNewRound()
{
	newRound();
	showInfo(tr("对方开启新的对局"));
}

void MainWindow::remoteSetPlayer(int player)
{
	this->player = player;
	ui->player->setCurrentIndex(player == GoBangManager::BLACK? 0: 1);
	showInfo(tr("对方设置了黑白手"));
}

void MainWindow::remoteAdmitDefeat()
{
	endRound();
	QMessageBox::information(this, tr("对局结束"), tr("对方认输了"));
	showInfo(tr("对局结束，对方认输了！"));
}

void MainWindow::remoteMessage(QString message)
{
	showInfo(tr(">> ") + message);
}

void MainWindow::on_player_activated(int index)
{
	localSetPlayer(index == 0? GoBangManager::BLACK: GoBangManager::WHITE);
}

void MainWindow::on_newRound_clicked()
{
	localNewRound();
}

void MainWindow::on_danger_clicked(bool checked)
{
	ui->gird->setDangerShow(checked);
	ui->gird->update();
}

void MainWindow::on_create_clicked(bool checked)
{
	if(checked)
	{
		messager->createHost();
		ui->connect->setEnabled(false);
	}
	else
	{
		messager->closeHost();
		ui->connect->setEnabled(true);
	}
}

void MainWindow::on_admitDefeat_clicked()
{
	localAdmitDefeat();
}

void MainWindow::on_lineEdit_returnPressed()
{
	localMessage(ui->lineEdit->text());
	ui->lineEdit->clear();
}
