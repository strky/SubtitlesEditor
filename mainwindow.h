#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QMediaPlayer>
#include <QGraphicsVideoItem>
#include <QKeyEvent>
#include <QListWidget>
#include <QMediaPlaylist>
#include "video.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void OnButtonOpenFile(bool);
    void OnButtonPlay(bool);
    void OnSliderPos(int pos);
    void DurationChanged(qint64);
    void PositionChanged(qint64 pos);
    void OnRotationChanged(int angle);
    void OnVolumeChanged(int volume);
    void OnButtonFullScreen(bool);
    void OnButtonMute(bool);
    void OnDoubleClickPlaylist(QModelIndex index);
    void OnStateChanged(QMediaPlayer::State);

protected:
    void keyPressEvent(QKeyEvent *event);
    bool eventFilter(QObject *obj, QEvent *event);

private:
    void SetPlay();
    void SetPause();

    QMediaPlayer mediaPlayer;
    QMediaPlaylist *playlist;
    Video *videoWidget;
    QListWidget *playlistWidget;

    double duration;
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
