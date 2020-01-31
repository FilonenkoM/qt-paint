#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QSvgGenerator>
#include <QPainter>

#include <QStyleFactory>
#include <QToolButton>
#include <QColorDialog>

#include <QDebug>
#include "sizedialog.h"
#include "thicknessicon.h"
#include <QDomDocument>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->mainToolBar->setStyle(QStyleFactory::create("windows"));

    setupMainToolbar();
}

void MainWindow::setupMainToolbar() {
    QToolButton *saveButton=
                dynamic_cast<QToolButton*>(ui->mainToolBar->widgetForAction(ui->shapesButton));
    saveButton->setPopupMode(QToolButton::InstantPopup);
    saveButton->addAction(ui->actionCreateRectangle);
    saveButton->addAction(ui->actionCreate_Eliipse);
    saveButton->addAction(ui->actionCreateRoundedReact);
    saveButton->addAction(ui->actionLine);
    saveButton->addAction(ui->actionTriangle);


    QToolButton *styleButton=
                dynamic_cast<QToolButton*>(ui->mainToolBar->widgetForAction(ui->actionPenStyle));

    styleButton->setPopupMode(QToolButton::InstantPopup);
    styleButton->addAction(ui->actionThin);
    styleButton->addAction(ui->actionMediumThickness);
    styleButton->addAction(ui->actionThick);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionSave_triggered()
{
    QString newPath = QFileDialog::getSaveFileName(this, tr("Save SVG"),
            tr("SVG files (*.svg)"));
    if (newPath.isEmpty())
           return;
    QSvgGenerator generator;
    generator.setFileName(newPath);
    generator.setSize(ui->displayWidget->image().size());
        generator.setViewBox(QRect(0, 0,
                                   ui->displayWidget->image().size().width(),
                                   ui->displayWidget->image().size().height()));
        generator.setTitle(tr("SVG Generator Example Drawing"));
        generator.setDescription(tr("An SVG drawing created by the SVG Generator "
                                    "Example provided with Qt."));

    QPainter painter;
    painter.begin(&generator);
    ui->displayWidget->paint(painter);
    painter.end();
}

void MainWindow::on_shapesButton_triggered()
{

}

void MainWindow::on_actionNew_triggered()
{
    SizeDialog sizeDialog(this);

    int result = sizeDialog.exec();
    if(result == QDialog::Accepted)
    {
        // TODO implement the ImageOwner class that will keep the value of the image
        // TODO check the correctness of the given values
        SVGImage* image = new SVGImage();
        image->size() = QSize(sizeDialog.width(), sizeDialog.height());
        ui->displayWidget->setImage(image);
        ui->displayWidget->update();
    }
}

void MainWindow::on_actionCreateRectangle_triggered()
{
    ui->displayWidget->addRect();
}

void MainWindow::on_actionCreate_Eliipse_triggered()
{
   ui->displayWidget->addEllipse();
}

void MainWindow::on_actionCreateRoundedReact_triggered()
{
    ui->displayWidget->addRoundedRect();
}

void MainWindow::on_actionThin_triggered()
{
    ui->displayWidget->setThickness(1);
}

void MainWindow::on_actionMediumThickness_triggered()
{
    ui->displayWidget->setThickness(3);

}
void MainWindow::on_actionThick_triggered()
{
    ui->displayWidget->setThickness(5);

}

void MainWindow::on_actionBorder_triggered()
{
    QColorDialog dialog;
    dialog.exec();
    ui->displayWidget->setBorderColor(dialog.selectedColor());
}

void MainWindow::on_actionBrush_triggered()
{
    QColorDialog dialog;
    dialog.exec();
    ui->displayWidget->setBrushColor(dialog.selectedColor());
}

void MainWindow::on_actionOpen_triggered()
{
    SVGImage* image = new SVGImage();
    image->size() = QSize(600, 600);
    ui->displayWidget->setImage(image);
    ui->displayWidget->update();

    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),
                                                    "/home",
                                                    tr("Images (*.svg)"));
    if(fileName.size() > 0) {
        QFile file(fileName);

        QDomDocument doc;
        if (!file.open(QIODevice::ReadOnly) || !doc.setContent(&file))
                return;
        QDomNodeList gList = doc.elementsByTagName("g");
        for (int i = 0; i < gList.size(); i++) {
            QDomNode node = gList.item(i);
            QDomElement pathElement = node.firstChildElement("path");
            QPainterPath path;
            if (!pathElement.isNull()){
                QString d = pathElement.attribute("d");

                QStringList dSolit = d.split(' ');
                qDebug() << dSolit;

                for(int i=0;i<dSolit.size();i++) {
                    QString token = dSolit[i];
                    if(token.size() < 1) continue;
                    if(token.at(0) == 'M') {
                        token.remove(0,1);
                        QStringList coords = token.split(',');
                        path.moveTo(coords[0].toDouble(), coords[1].toDouble());
                    }
                    else if(token.at(0) == 'L') {
                        token.remove(0,1);
                        QStringList coords = token.split(',');
                        path.lineTo(coords[0].toDouble(), coords[1].toDouble());
                    }
                    else if(token.at(0) == 'C') {
                        token.remove(0,1);
                        QStringList coords = token.split(',') << dSolit[++i].split(',') << dSolit[++i].split(',');
                        path.cubicTo(coords[0].toDouble(), coords[1].toDouble(),
                                coords[2].toDouble(), coords[3].toDouble(),
                                coords[4].toDouble(), coords[5].toDouble());
                    }
                }
                QDomElement nodeElement = node.toElement();
                QPen pen;
                if(nodeElement.attribute("stroke") != "none") {
                    pen.setColor(QColor(nodeElement.attribute("stroke")));
                }

                pen.setWidth(nodeElement.attribute("stroke-width").toInt());
                qDebug() << "width " << pen.width();
                QBrush brush;

                if(nodeElement.attribute("fill") != "none") {
                    brush.setColor(QColor(nodeElement.attribute("fill")));
                    brush.setStyle(Qt::SolidPattern);
                }

                qDebug() << path;
                ImageFragment fragment(path, pen, brush);
                ui->displayWidget->image().fragments().push_back(fragment);
            }
        }
    }
    ui->displayWidget->update();
}

void MainWindow::on_actionLine_triggered()
{
    ui->displayWidget->addLine();
}

void MainWindow::on_actionTriangle_triggered()
{
    ui->displayWidget->addTriangle();
}
