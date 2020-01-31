#include "sizedialog.h"
#include "ui_sizedialog.h"

SizeDialog::SizeDialog(QWidget *parent) :
    QDialog(parent, Qt::Sheet),
    ui(new Ui::SizeDialog)
{
    ui->setupUi(this);
}

SizeDialog::~SizeDialog()
{
    delete ui;
}
