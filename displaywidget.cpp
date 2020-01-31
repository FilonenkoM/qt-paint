#include "displaywidget.h"
#include <QPainter>

#include <QtDebug>
#include <QMouseEvent>
#include <QPointF>

#include <cmath>

DisplayWidget::DisplayWidget(QWidget *parent, SVGImage* image)
    : QWidget(parent),
      _image(image),
      _currentFragment(0),
      _dragBeginPoint(QPoint(-1,-1)),
      _scalingIndex(-1)
{
}

void DisplayWidget::paintEvent(QPaintEvent * /* event */)
{
    QPainter painter;
    painter.begin(this);
    painter.setRenderHint(QPainter::Antialiasing);
    paint(painter);
    painter.end();
}

void DisplayWidget::paint(QPainter &painter) {

    if(_image) {
        // TODO set the window size corresponding to the size of the image drawn
        painter.setClipRect(0, 0, _image->size().width(), _image->size().height());
        drawImageEditorView(painter);
//        painter.translate(xOffset(), yOffset());
        _image->paint(painter);

        if(_currentFragment) paintCurrentFragmentDecorations(painter);
    }

}

void DisplayWidget::drawImageEditorView(QPainter& painter) {
    painter.setPen((Qt::NoPen));
    painter.fillRect(rect(), Qt::white);
}

void DisplayWidget::addRect() {
    // TODO implement the memorizing mechanism (StateOwner)
    if(imageSet()) {
        QPainterPath path;
        path.addRect(0,  0, 100, 100);
        QPen pen;
        pen.setBrush(Qt::darkBlue);
        ImageFragment fragment(path, pen, Qt::NoBrush);
        _image->fragments().push_back(fragment);
        _currentFragment = nullptr;
        update();
    }
}

void DisplayWidget::addEllipse() {
    if(imageSet()) {
        QPainterPath path;
        path.addEllipse(_image->size().width() / 2 - 50,  _image->size().height() / 2 - 50, 100, 100);
        QPen pen;
        pen.setBrush(Qt::darkBlue);
        ImageFragment fragment(path, pen, Qt::NoBrush);
        _image->fragments().push_back(fragment);
        _currentFragment = nullptr;
        update();
    }
}

void DisplayWidget::addRoundedRect() {
    if(imageSet()) {
        QPainterPath path;
        path.addRoundRect(_image->size().width() / 2 - 50,  _image->size().height() / 2 - 50, 100, 100, 10, 10);
        QPen pen;
        pen.setBrush(Qt::darkBlue);
        ImageFragment fragment(path, pen, Qt::NoBrush);
        _image->fragments().push_back(fragment);
        _currentFragment = nullptr;

        update();
    }
}

void DisplayWidget::mousePressEvent(QMouseEvent *event) {
    static QPainterPath* path = nullptr;
    static QPainterPath* trianglePath = nullptr;

    _dragBeginPoint = event->pos();

//    qDebug() << "pos " << event->pos();
    for(int i=0;i<_currentRects.size();i++)
    {
        QRect& r = _currentRects[i];
//        qDebug() << r;

//        if(r.contains(QPoint(event->pos().x() - xOffset(), event->pos().y() - yOffset()))) {
          if(r.contains(QPoint(event->pos().x(), event->pos().y()))) {
            _scalingIndex = i;
            qDebug() << _scalingIndex;
            break;
        }
    }

    if(lineCount) {
        if(lineCount == 2) {
            path = new QPainterPath();
            path->moveTo(event->x(), event->y());
        }
        else {
            path->lineTo(event->x(), event->y());
            QPen pen(Qt::black);
            ImageFragment f(*path, pen, QBrush(Qt::black));
            _image->fragments().push_back(f);
            qDebug() << path->boundingRect();
        }
        lineCount --;
    }
    if(triangleCount) {
        if(triangleCount == 3) {
            trianglePath = new QPainterPath();
            trianglePath->moveTo(event->x(), event->y());
        }
        else if(triangleCount == 1) {
            trianglePath->lineTo(event->x(), event->y());
            trianglePath->closeSubpath();
            QPen pen(Qt::black);
            ImageFragment f(*trianglePath, pen, QBrush(Qt::NoBrush));
            _image->fragments().push_back(f);
        }
        else {
            trianglePath->lineTo(event->x(), event->y());
        }
        triangleCount --;
    }
}

void DisplayWidget::mouseMoveEvent(QMouseEvent *event) {

    static QPoint previous(-1, -1);
    if(previous.x() == -1) previous = _dragBeginPoint;

    if(_currentFragment && _scalingIndex != -1) {
        double width = _currentFragment->path().boundingRect().width();
        double height = _currentFragment->path().boundingRect().height();
        double xFactor = (event->x() - _currentFragment->path().boundingRect().left()) / width;
        double yFactor = (event->y() - _currentFragment->path  ().boundingRect().top()) / height;
        double xRevFactor = (_currentFragment->path().boundingRect().right() - event->x()) / width;
        double yRevFactor = (_currentFragment->path().boundingRect().bottom() - event->y()) / height;

        switch(_scalingIndex) {
        case 0:{
            double right = _currentFragment->path().boundingRect().right();
            double top = _currentFragment->path().boundingRect().top();

            QTransform transform;
            transform.scale(xRevFactor, yFactor);
            _currentFragment->path() = transform.map(_currentFragment->path());

            double newRight = _currentFragment->path().boundingRect().right();
            double newTop = _currentFragment->path().boundingRect().top();

            _currentFragment->path().translate(right - newRight, top - newTop);
            break;
        }
        case 1: {
            double left = _currentFragment->path().boundingRect().left();
            double top = _currentFragment->path().boundingRect().top();

            QTransform transform;
            transform.scale(xFactor, yFactor);
            _currentFragment->path() = transform.map(_currentFragment->path());

            double newLeft = _currentFragment->path().boundingRect().left();
            double newTop = _currentFragment->path().boundingRect().top();

            _currentFragment->path().translate(left - newLeft, top - newTop);
            break;
        }
        case 2:{
            double right = _currentFragment->path().boundingRect().right();
            double bottom = _currentFragment->path().boundingRect().bottom();

            QTransform transform;
            transform.scale(xRevFactor, yRevFactor);
            _currentFragment->path() = transform.map(_currentFragment->path());

            double newRight = _currentFragment->path().boundingRect().right();
            double newBottom = _currentFragment->path().boundingRect().bottom();

            _currentFragment->path().translate(right - newRight, bottom - newBottom);
            break;
        }
        case 3: {
            double left = _currentFragment->path().boundingRect().left();
            double bottom = _currentFragment->path().boundingRect().bottom();

            QTransform transform;
            transform.scale(xFactor, yRevFactor);
            _currentFragment->path() = transform.map(_currentFragment->path());

            double newLeft = _currentFragment->path().boundingRect().left();
            double newBottom = _currentFragment->path().boundingRect().bottom();

            _currentFragment->path().translate(left - newLeft, bottom - newBottom);
            break;
        }

        case 4: {
            double bottom = _currentFragment->path().boundingRect().bottom();

            QTransform transform;
            transform.scale(1, yRevFactor);
            _currentFragment->path() = transform.map(_currentFragment->path());

            double newBottom = _currentFragment->path().boundingRect().bottom();

            _currentFragment->path().translate(0, bottom - newBottom);
            break;
        }
        case 5: {
            double top = _currentFragment->path().boundingRect().top();

            QTransform transform;
            transform.scale(1, yFactor);
            _currentFragment->path() = transform.map(_currentFragment->path());

            double newTop = _currentFragment->path().boundingRect().top();

            _currentFragment->path().translate(0, top - newTop);
            break;
        }
        case 6:{
            double right = _currentFragment->path().boundingRect().right();

            QTransform transform;
            transform.scale(xRevFactor, 1);
            _currentFragment->path() = transform.map(_currentFragment->path());

            double newRight = _currentFragment->path().boundingRect().right();

            _currentFragment->path().translate(right - newRight, 0);
            break;
        }
        case 7: {
            double left = _currentFragment->path().boundingRect().left();

            QTransform transform;
            transform.scale(xFactor, 1);
            _currentFragment->path() = transform.map(_currentFragment->path());

            double newLeft = _currentFragment->path().boundingRect().left();

            _currentFragment->path().translate(left - newLeft, 0);
            break;
        }
        }
    }
    else if(_image and _dragBeginPoint.x() != -1 and _currentFragment and _scalingIndex == -1) {
        int distance = DisplayWidget::distanceBetween(_dragBeginPoint, event->pos());
        if(distance > 5) {
           _currentFragment->path().translate(event->x() - previous.x(), event->y() - previous.y());
        }
            previous = event->pos();
    }

    update();
}

void DisplayWidget::mouseReleaseEvent(QMouseEvent *event) {
    _scalingIndex = -1;
    if(_image) {
//        int x = event->pos().x() - xOffset();
//        int y = event->pos().y() - yOffset();
        int x = event->pos().x();
        int y = event->pos().y();
//        if(QRect(xOffset(), yOffset(), _image->size().width(), _image->size().height()).contains(event->pos().x(), event->pos().y())) {
            _currentFragment = image().findClosestFragment(QPoint(x, y));
//         }

       update();

    }
}

void DisplayWidget::paintCurrentFragmentDecorations(QPainter& painter) {
        QRectF boundingRect = _currentFragment->path().boundingRect();
        int esize = 10;
        painter.setBrush(Qt::darkBlue);

        _currentRects.clear();

        _currentRects.push_back(rectAround( boundingRect.bottomLeft(), esize));
        _currentRects.push_back(rectAround(boundingRect.bottomRight(), esize));
        _currentRects.push_back(rectAround(boundingRect.topLeft(), esize));
        _currentRects.push_back(rectAround(boundingRect.topRight(), esize));

        _currentRects.push_back(rectAround(QPointF(boundingRect.left() + boundingRect.width() / 2, boundingRect.top()), esize));
        _currentRects.push_back(rectAround(QPointF(boundingRect.left() + boundingRect.width() / 2, boundingRect.bottom()), esize));
        _currentRects.push_back(rectAround(QPointF(boundingRect.left(), boundingRect.bottom() - boundingRect.height() / 2), esize));
        _currentRects.push_back(rectAround(QPointF(boundingRect.right(), boundingRect.bottom() - boundingRect.height() / 2), esize));

        for(QRect& r: _currentRects)
            painter.drawEllipse(r);
}

QRect DisplayWidget::rectAround(QPointF point, int size)
{
    return ((QRect(
            point.x() - size / 2,
            point.y() - size / 2,
            size,
            size)));
}

int DisplayWidget::distanceBetween(QPoint first, QPoint second) {
    return std::sqrt((first.x() - second.x()) * (first.x() - second.x()) + (first.y() - second.y()) * (first.y() - second.y()));
}

void DisplayWidget::setThickness(int thickness) {
    if(_image and _currentFragment) _currentFragment->pen().setWidth(thickness);
    update();
}

void DisplayWidget::setBorderColor(QColor color) {
    if(_image and _currentFragment) _currentFragment->pen().setColor(color);
    update();
}

void DisplayWidget::setBrushColor(QColor color) {
    qDebug() << "hello";
    if(_image and _currentFragment) {
        _currentFragment->brush().setStyle(Qt::SolidPattern);
        _currentFragment->brush().setColor(color);
    }
    update();
}

void DisplayWidget::addLine() {
    if(_image) {
        lineCount = 2;
    }
}

void DisplayWidget::addTriangle() {
    if(_image) {
        triangleCount = 3;
    }
}


