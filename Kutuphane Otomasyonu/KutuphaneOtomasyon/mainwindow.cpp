#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "dialog.h"     // Üye
#include "dialog2.h"    // Kitap
#include "dialog3.h"    // Ödünç Alma
#include "dialog4.h"    // Ödünç Teslim
#include <QSqlError>
#include <QMessageBox>
#include <QDebug>
#include <QIcon>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // MSSQL VERİTABANI BAĞLANTISI
    db = QSqlDatabase::addDatabase("QODBC");
    db.setDatabaseName("Driver={SQL Server};Server=RUKIYE;Database=KutuphaneDB;Trusted_Connection=yes;");

    if (db.open()) {
        ui->statusbar->showMessage("Veritabanı bağlantısı başarılı!");
    } else {
        QMessageBox::critical(this, "Hata", "Veritabanına bağlanılamadı:\n" + db.lastError().text());
    }

    //Butonlara ikon ataması
    ui->btnUyeIslemleri->setIcon(QIcon(":/img/user.jpg"));
    ui->btnKitapIslemleri->setIcon(QIcon(":/img/book.jpg"));
    ui->btnOduncAlma->setIcon(QIcon(":/img/odunc_1.jpg"));
    ui->btnOduncTeslim->setIcon(QIcon(":/img/odunc_2.jpg"));

    //İkon boyutları
    QSize ikonBoyut(64, 64);
    ui->btnUyeIslemleri->setIconSize(ikonBoyut);
    ui->btnKitapIslemleri->setIconSize(ikonBoyut);
    ui->btnOduncAlma->setIconSize(ikonBoyut);
    ui->btnOduncTeslim->setIconSize(ikonBoyut);

    // Yazar bilgisini göster
    ui->lblYazar->setText("Yazar: <b style='color:#007acc;'>Rukiye Uçar</b> &nbsp;|&nbsp; <i style='color:gray;'>Versiyon 1.0</i>");
    ui->lblYazar->setAlignment(Qt::AlignRight);
    ui->lblYazar->setStyleSheet("font-size: 10pt; font-family: Segoe UI; padding-right: 12px;");


}

MainWindow::~MainWindow()
{
    db.close();
    delete ui;
}

// Üye İşlemleri
void MainWindow::on_btnUyeIslemleri_clicked()
{
    Dialog dlg;
    dlg.exec();
}

// Kitap İşlemleri
void MainWindow::on_btnKitapIslemleri_clicked()
{
    Dialog2 kitapForm;
    kitapForm.exec();
}

// Ödünç Alma İşlemleri
void MainWindow::on_btnOduncAlma_clicked()
{
    Dialog3 oduncForm;
    oduncForm.exec();
}

// Ödünç Teslim Etme İşlemleri
void MainWindow::on_btnOduncTeslim_clicked()
{
    Dialog4 teslimForm;
    teslimForm.exec();
}
