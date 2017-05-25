#include "video.h"
#include <QKeyEvent>

Video::Video()
{

}

void Video::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Escape) {
        setFullScreen(false);
    } else if (event->key() == Qt::Key_F) {
        if(!this->isFullScreen()) {
            setFullScreen(true);
        }
    }
}

void Video::mouseDoubleClickEvent(QMouseEvent *event)
{
    if(this->isFullScreen()) {
        setFullScreen(false);
    } else {
        setFullScreen(true);
    }

}
