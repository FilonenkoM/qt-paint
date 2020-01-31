#ifndef SIZEDIALOG_H
#define SIZEDIALOG_H

#include <QDialog>
#include "ui_sizedialog.h"

namespace Ui {
class SizeDialog;
}

class SizeDialog : public QDialog
{
    Q_OBJECT

private:
    Ui::SizeDialog *ui;

public:
    explicit SizeDialog(QWidget *parent = nullptr);
    ~SizeDialog();
    inline int width() { return ui->widthSpinBox->value(); }
    inline int height() { return ui->heightSpinBox->value(); }
};

#endif // SIZEDIALOG_H
