#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QTime>
#include <QGridLayout>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    qApp->installEventFilter(this);

    videoWidget = new Video();
    ui->view->hide();
    mediaPlayer.setVideoOutput(videoWidget);
    ui->gridLayout->addWidget(videoWidget,0,0,1,6);

    playlistWidget = new QListWidget();
    ui->gridLayout->addWidget(playlistWidget,0,6,1,1);
    playlistWidget->setMaximumWidth(130);

    playlist = new QMediaPlaylist;
    mediaPlayer.setPlaylist(playlist);


    ui->buttonPlay->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
    ui->buttonMute->setIcon(style()->standardIcon(QStyle::SP_MediaVolume));
    ui->buttonPlay->setEnabled(false);
    ui->sliderVolume->setValue(80);

    connect(ui->buttonOpenFile, SIGNAL(clicked(bool)),
            this, SLOT(OnButtonOpenFile(bool)));
    connect(ui->buttonPlay, SIGNAL(clicked(bool)),
            this, SLOT(OnButtonPlay(bool)));
    connect(ui->buttonFullScreen, SIGNAL(clicked(bool)),
            this, SLOT(OnButtonFullScreen(bool)));
    connect(ui->buttonMute, SIGNAL(clicked(bool)),
            this, SLOT(OnButtonMute(bool)));

    connect(ui->sliderPos, SIGNAL(sliderMoved(int)),
            this, SLOT(OnSliderPos(int)));
    connect(ui->sliderVolume, SIGNAL(valueChanged(int)),
            this, SLOT(OnVolumeChanged(int)));

    connect(&mediaPlayer, SIGNAL(positionChanged(qint64)),
            SLOT(PositionChanged(qint64)));
    connect(&mediaPlayer, SIGNAL(durationChanged(qint64)),
            SLOT(DurationChanged(qint64)));
    connect(&mediaPlayer, SIGNAL(stateChanged(QMediaPlayer::State)),
            this, SLOT(OnStateChanged(QMediaPlayer::State)));

    connect(playlistWidget, SIGNAL(doubleClicked(QModelIndex)),
            this, SLOT(OnDoubleClickPlaylist(QModelIndex)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::OnButtonOpenFile(bool)
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Movie"),QDir::homePath());

    if (!fileName.isEmpty()) {
       // mediaPlayer.setMedia(QUrl::fromLocalFile(fileName));
        playlist->addMedia(QUrl::fromLocalFile(fileName));
        //playlist->addMedia(QUrl("http://example.com/movie1.mp4"));
        playlistWidget->addItem(fileName);
        ui->buttonPlay->setEnabled(true);
    }
}

void MainWindow::OnButtonPlay(bool)
{
    switch(mediaPlayer.state()) {
    case QMediaPlayer::PlayingState:
        SetPause();
        break;
    default:
        SetPlay();
        break;
    }
}

void MainWindow::OnSliderPos(int pos)
{
    mediaPlayer.setPosition(pos);
}

void MainWindow::DurationChanged(qint64 duration)
{
    ui->sliderPos->setMaximum(duration);
    this->duration = duration;

    QTime timeDuration(0,0,0);
    timeDuration = timeDuration.addMSecs(duration);
    ui->labelTime->setText("00:00:00/" + timeDuration.toString("hh:mm:ss"));
}

void MainWindow::PositionChanged(qint64 pos)
{
    ui->sliderPos->setValue(pos);

    QTime timeCurrent(0,0,0);
    timeCurrent = timeCurrent.addMSecs(pos);
    QTime timeDuration(0,0,0);
    timeDuration = timeDuration.addMSecs(duration);
    ui->labelTime->setText(timeCurrent.toString("hh:mm:ss") + "/" +
                           timeDuration.toString("hh:mm:ss"));
}

void MainWindow::OnRotationChanged(int angle)
{
    //qDebug() << videoItem->boundingRect();
    //videoWidget->sett
//    videoItem->setTransformOriginPoint(videoItem->boundingRect().width()/2,
//                                       videoItem->boundingRect().height()/2);
    //    videoItem->setRotation(angle);
}

void MainWindow::OnVolumeChanged(int volume)
{
    mediaPlayer.setVolume(volume);
}

void MainWindow::OnButtonFullScreen(bool)
{
    videoWidget->setFullScreen(true);
    //ui->view->setWindowState(Qt::WindowFullScreen);
//    this->setWindowState(Qt::WindowFullScreen);
    //    ui->view->fitInView(videoItem, Qt::IgnoreAspectRatio);
}

void MainWindow::OnButtonMute(bool)
{
    if(mediaPlayer.isMuted()) {
        mediaPlayer.setMuted(false);
        ui->buttonMute->setIcon(style()->standardIcon(QStyle::SP_MediaVolume));
    } else {
        mediaPlayer.setMuted(true);
        ui->buttonMute->setIcon(style()->standardIcon(QStyle::SP_MediaVolumeMuted));
    }
}

void MainWindow::OnDoubleClickPlaylist(QModelIndex index)
{
    mediaPlayer.setMedia(QUrl::fromLocalFile(index.data().toString()));
    playlist->setCurrentIndex(index.row());
    SetPlay();
}

void MainWindow::OnStateChanged(QMediaPlayer::State status)
{
    if(status == QMediaPlayer::EndOfMedia) {
        playlistWidget->setCurrentRow(playlistWidget->currentRow() + 1);

        mediaPlayer.setMedia(QUrl::fromLocalFile(
             playlistWidget->item(playlistWidget->currentRow())->text()));
    }

}

bool MainWindow::eventFilter(QObject *obj, QEvent *event)
{
    //qDebug() << "udalost";
    if (event->type() == QEvent::KeyPress)
    {
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
           //qDebug() << "key " << keyEvent->key() << "from" << obj;
        switch (keyEvent->key()) {
        case Qt::Key_Left:
        {
           qDebug() << "key left";
           mediaPlayer.setPosition(mediaPlayer.position() - 5000);
           break;
        }
        case Qt::Key_Right:
        {
           qDebug() << "key right";
           mediaPlayer.setPosition(mediaPlayer.position() + 5000);

           break;
        }
        default:
           break;
        }

    }
    return QMainWindow::eventFilter(obj, event);
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    qDebug() << event->key();
    if(event->key() == Qt::Key_Escape) {
        videoWidget->setFullScreen(false);
    }
}

void MainWindow::SetPlay()
{
    mediaPlayer.play();
    ui->buttonPlay->setIcon(style()->standardIcon(QStyle::SP_MediaPause));
}

void MainWindow::SetPause()
{
    mediaPlayer.pause();
    ui->buttonPlay->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
}
//konec

