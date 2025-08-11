//Rukiye Uçar
//21100011057
//Karpuz kesildikten sonra ses çıkaran ses efekti eklenmiştir. Sounds klasörü bunun içindir.


#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFile>
#include <QTextStream>
#include <QRandomGenerator>
#include <QMessageBox>
#include <QScreen>
#include <QGuiApplication>
#include <QPalette>
#include <QBrush>
#include <QPixmap>
#include <QTimer>
#include <QMouseEvent>
#include <QSoundEffect>



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , remainingTime(30)
    , score(0)
    , missed(0)
{
    ui->setupUi(this);
    setWindowTitle("Ruki-cute QT Watermelonnnn!");

    setFixedSize(QGuiApplication::primaryScreen()->size());

    ui->lblScore->setText("Skor: 0");
    ui->lblMissed->setText("Kaçan: 0");
    ui->lblTime->setText("Süre: 30");

    loadPositions();

    gameTimer = new QTimer(this);
    connect(gameTimer, &QTimer::timeout, this, &MainWindow::updateGame);
    gameTimer->start(1000);

    spawnTimer = new QTimer(this);
    connect(spawnTimer, &QTimer::timeout, this, &MainWindow::spawnWatermelon);
    spawnTimer->start(1500);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::loadPositions()
{
    QFile file("C:/Users/ucarr/Documents/mainwindow/konumlar.txt");
    if (file.open(QIODevice::ReadOnly)) {
        QTextStream in(&file);
        while (!in.atEnd()) {
            int x, y;
            in >> x >> y;
            positions.append(QPoint(x, y));
        }
        file.close();
    }
}

void MainWindow::spawnWatermelon()
{
    if (positions.isEmpty()) {
        return;
    }

    int index = QRandomGenerator::global()->bounded(positions.size());
    QPoint pos = positions.at(index);

    QLabel *melon = new QLabel(this);
    melon->setPixmap(QPixmap("C:/Users/ucarr/Documents/mainwindow/images/1.png").scaled(80, 80));
    melon->setGeometry(QRect(pos.x(), pos.y(), 80, 80));
    melon->setAttribute(Qt::WA_DeleteOnClose);
    melon->show();

    QPropertyAnimation *anim = new QPropertyAnimation(melon, "geometry");
    anim->setDuration(3000);
    anim->setStartValue(QRect(pos.x(), pos.y(), 80, 80));
    anim->setEndValue(QRect(pos.x(), height(), 80, 80));
    anim->start(QAbstractAnimation::DeleteWhenStopped);

    melon->installEventFilter(this);
    melons.push_back(melon);

    QTimer::singleShot(3000, this, [=]() {
        if (melon->isVisible()) {
            melon->hide();
            missed++;
            updateStats();
        }
    });

    /*
    //karpuz kaçınca ekrana karpuz kaçtı uyarısı verilecekti ama çalışmadı. ui ekranından lblWarning ise kaldırıldı.
    QTimer::singleShot(3000, this, [=]() {
        if (karpuzEtiketi->isVisible()) {
            karpuzEtiketi->hide();
            kacirilan++;
            skorGuncelle();

            ui->lblWarning->setText("Karpuz kaçtı!");
            ui->lblWarning->setStyleSheet("color: red; font-weight: bold;");
            ui->lblWarning->setVisible(true);

            QTimer::singleShot(1000, this, [=]() {
                ui->lblWarning->clear();
                ui->lblWarning->setVisible(false);
            });
        }
    });
    */
}

bool MainWindow::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::MouseButtonPress) {
        QLabel *label = qobject_cast<QLabel*>(obj);
        if (label) {
            label->setPixmap(QPixmap("C:/Users/ucarr/Documents/mainwindow/images/2.png").scaled(80, 80));
            score++;
            updateStats();

            QSoundEffect *effect = new QSoundEffect(this);
            effect->setSource(QUrl("qrc:/sounds/kilic.wav"));
            effect->setVolume(0.5);
            effect->play();

            QTimer::singleShot(1000, label, &QLabel::hide);
            return true;
        }
    }
    return QMainWindow::eventFilter(obj, event);
}

void MainWindow::updateGame()
{
    remainingTime--;

    ui->lblTime->setText("Süre: " + QString::number(remainingTime));

    if (remainingTime == 0) {
        gameTimer->stop();
        spawnTimer->stop();
        endGame();
    }
}

void MainWindow::updateStats()
{
    ui->lblScore->setText("Skor: " + QString::number(score));
    ui->lblMissed->setText("Kaçan: " + QString::number(missed));
}

void MainWindow::endGame()
{
    int maxSkor = 0;

    QFile file("C:/Users/ucarr/Documents/mainwindow/skorlar.txt");

    if (file.open(QIODevice::ReadWrite | QIODevice::Text)) {
        QTextStream in(&file);
        while (!in.atEnd()) {
            int s;
            in >> s;
            maxSkor = qMax(maxSkor, s);
        }

        QTextStream out(&file);
        out << "\n" << score;
        file.close();
    }

    QString message;

    if (score > maxSkor) {
        message = "Tebrikler! Yeni rekor: " + QString::number(score);
    } else {
        message = "Oyun Bitti! Maksimum skoru Geçemediniz!\nSkorunuz: "+QString::number(score) +"\nMaksimum skor: " + QString::number(maxSkor) +"\nKaçan karpuz: " + QString::number(missed);
    }

    QMessageBox::information(this, "Oyun Bitti", message);
}

