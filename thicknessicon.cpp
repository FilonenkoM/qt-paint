#include "thicknessicon.h"
#include <QtDebug>
#include <QPainter>

ThicknessIcon::ThicknessIcon(int thickness): QIcon(), _thickness(thickness)
{
    qDebug() << "Hello";

}

void ThicknessIcon::paintEvent(QPaintEvent *event) {
    qDebug() << "Hello";
    QPainter painter;
    painter.setRenderHint(QPainter::Antialiasing);
    paint(painter);
}

void ThicknessIcon::paint(QPainter& painter) {
}
