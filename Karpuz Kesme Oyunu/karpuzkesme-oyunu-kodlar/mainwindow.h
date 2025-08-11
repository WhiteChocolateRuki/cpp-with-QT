#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QLabel>
#include <QVector>
#include <QPoint>
#include <QPropertyAnimation>




QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    bool eventFilter(QObject *obj, QEvent *event) override;

private slots:
    void updateGame();               // Süreyi azaltır
    void spawnWatermelon();         // Yeni karpuz ekler

private:
    Ui::MainWindow *ui;

    QTimer *gameTimer;              // Oyun süresi
    QTimer *spawnTimer;             // Karpuz ekleme
    int remainingTime;              // Kalan süre

    int score;                      // Kesilen karpuz
    int missed;                     // Kaçan karpuz

    QVector<QPoint> positions;      // konumlar.txt'den gelen konumlar
    QVector<QLabel*> melons;        // ekrandaki karpuzlar

    void loadPositions();           // konumlar.txt'yi oku
    void updateStats();             // Skor, süre, kaçan güncelle
    void endGame();                 // Oyun sonu işlemleri
};

#endif // MAINWINDOW_H
