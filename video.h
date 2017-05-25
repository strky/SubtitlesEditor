#ifndef VIDEO_H
#define VIDEO_H
#include <QVideoWidget>

class Video : public QVideoWidget
{
public:
    Video();
protected:
    void keyPressEvent(QKeyEvent *event);
    void mouseDoubleClickEvent(QMouseEvent *event);
private:


};

#endif // VIDEO_H
