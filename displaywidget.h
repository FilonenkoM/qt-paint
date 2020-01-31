#ifndef DISPLAYWIDGET_H
#define DISPLAYWIDGET_H

#include <QWidget>
#include "svgimage.h"

class DisplayWidget : public QWidget
{
    Q_OBJECT    
public:
    explicit DisplayWidget(QWidget *parent = nullptr, SVGImage* image = nullptr);
    void paint(QPainter& painter);
    inline void setImage(SVGImage* image) { _image = image; }
    void drawImageEditorView(QPainter&);

    void addRect();
    void addEllipse();
    void addRoundedRect();
    void setThickness(int thickness);
    void setBorderColor(QColor color);
    void setBrushColor(QColor color);
    void addLine();
    void addTriangle();

    bool imageSet() { return _image; }

    inline int xOffset() { return (width() - _image->size().width()) / 2; }
    inline int yOffset() { return (height() - _image->size().height()) / 2; }

signals:

public slots:
    inline SVGImage& image() { return *_image; }

protected:
    void paintEvent(QPaintEvent* event) override;
    void mousePressEvent( QMouseEvent * event ) override;
    void mouseReleaseEvent (QMouseEvent * event ) override;
    void mouseMoveEvent (QMouseEvent * event ) override;


private:
    SVGImage* _image;
    ImageFragment* _currentFragment;

    QPoint _dragBeginPoint;
    vector<QRect> _currentRects;
    int _scalingIndex;

    void paintCurrentFragmentDecorations(QPainter& painter);
    QRect rectAround(QPointF point, int size);
    static int distanceBetween(QPoint first, QPoint second);

    int lineCount = 0;
    int triangleCount = 0;
};

#endif // DISPLAYWIDGET_H
