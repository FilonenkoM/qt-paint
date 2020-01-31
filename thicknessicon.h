#ifndef THICKNESSICON_H
#define THICKNESSICON_H

#include <QObject>
#include <QWidget>
#include <QIcon>

class ThicknessIcon : public QIcon
{
   int _thickness;
protected:
   void paintEvent(QPaintEvent* event);
public:
    explicit ThicknessIcon(int thickness = 10);
   void paint(QPainter& painter);

};

#endif // THICKNESSICON_H
