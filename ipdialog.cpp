#include "ipdialog.h"
#include "ui_ipdialog.h"
#include <QPushButton>

IPDialog::IPDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::IPDialog)
{
	ui->setupUi(this);
	const QString s[4][3] = {{"7", "8", "9"}, {"4", "5", "6"}, {"1", "2", "3"}, {":", "0", "<-"}};
	for(int i=0; i<4; ++i)
		for(int j=0; j<3; ++j)
		{
			QString str = s[i][j];
			auto button = new QPushButton (str, ui->widget);
			ui->gridLayout->addWidget(button, i, j);
			if(str != "<-")
				connect(button, &QPushButton::clicked,
					[=]() {ui->lineEdit->setText(ui->lineEdit->text() + str);}
					);
			else
				connect(button, &QPushButton::clicked,
					[=]() {ui->lineEdit->setText(ui->lineEdit->text().remove(ui->lineEdit->text().length()-1, 1));}
					);
		}
}

IPDialog::~IPDialog()
{
	delete ui;
}

QHostAddress IPDialog::getAddress() const
{
	return QHostAddress(ui->lineEdit->text());
}
