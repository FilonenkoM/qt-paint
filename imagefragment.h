#ifndef IMAGEFRAGMENT_H
#define IMAGEFRAGMENT_H

#include <QPen>
#include <QBrush>
#include <QPainterPath>

class ImageFragment
{
public:
    ImageFragment(QPainterPath& path, QPen& pen, QBrush brush);

    inline QPen& pen() { return _pen; }
    inline QBrush& brush() { return _brush; }
    inline QPainterPath& path() { return _path; }

private:
    QPainterPath _path;
    QBrush _brush;
    QPen _pen;
};

#endif // IMAGEFRAGMENT_H
