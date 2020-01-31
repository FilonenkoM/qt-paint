#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_actionSave_triggered();

    void on_shapesButton_triggered();

    void on_actionNew_triggered();

    void on_actionCreateRectangle_triggered();



    void on_actionCreate_Eliipse_triggered();

    void on_actionCreateRoundedReact_triggered();

    void on_actionThin_triggered();

    void on_actionMediumThickness_triggered();

    void on_actionThick_triggered();

    void on_actionBorder_triggered();

    void on_actionBrush_triggered();

    void on_actionOpen_triggered();

    void on_actionLine_triggered();

    void on_actionTriangle_triggered();

private:
    Ui::MainWindow *ui;
    void setupMainToolbar();

};

#endif // MAINWINDOW_H
