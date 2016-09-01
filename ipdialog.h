#ifndef IPDIALOG_H
#define IPDIALOG_H

#include <QDialog>
#include <QHostAddress>

namespace Ui {
class IPDialog;
}

class IPDialog : public QDialog
{
	Q_OBJECT

public:
	explicit IPDialog(QWidget *parent = 0);
	~IPDialog();
	QHostAddress getAddress () const;

private:
	Ui::IPDialog *ui;
};

#endif // IPDIALOG_H
