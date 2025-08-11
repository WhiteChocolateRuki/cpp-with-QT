#include "dialog.h"
#include "ui_dialog.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QTableWidgetItem>
#include <QDebug>
#include <QMessageBox>  //bunu unutma!
// şeklinde eklenen değerler yorum açıklması ve modüllerin ne işe yaradığını açıklansın diye eklenmiştir.
Dialog::Dialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Dialog)
{
    ui->setupUi(this);
    uyeListele();
    // Arka plan rengi ve genel font
    this->setStyleSheet("QDialog { background-color: #f9f9f9; font-family: 'Segoe UI'; font-size: 11pt; }");

    // Tablo stilleri
    ui->tblUyeler->setStyleSheet(
        "QTableWidget {"
        "  background-color: white;"
        "  border: 1px solid #ccc;"
        "  gridline-color: #eee;"
        "  selection-background-color: #a8dadc;"
        "  selection-color: black;"
        "}"
        "QHeaderView::section {"
        "  background-color: #457b9d;"
        "  color: white;"
        "  font-weight: bold;"
        "  border: 1px solid #ddd;"
        "  padding: 4px;"
        "}"
        );

    // LineEdit stilleri
    ui->txtUyeNo->setStyleSheet("QLineEdit { background-color: #e9ecef; padding: 4px; border: 1px solid #ced4da; border-radius: 4px; }");
    ui->txtUyeAd->setStyleSheet("QLineEdit { background-color: white; padding: 4px; border: 1px solid #ced4da; border-radius: 4px; }");
    ui->txtUyeSoyad->setStyleSheet("QLineEdit { background-color: white; padding: 4px; border: 1px solid #ced4da; border-radius: 4px; }");

    // Buton stilleri
    QString butonStili =
        "QPushButton {"
        "  background-color: #1d3557;"
        "  color: white;"
        "  padding: 6px 12px;"
        "  border: none;"
        "  border-radius: 6px;"
        "}"
        "QPushButton:hover {"
        "  background-color: #457b9d;"
        "}"
        "QPushButton:pressed {"
        "  background-color: #1d3557;"
        "  padding-left: 8px;"
        "}";

    ui->btnYeniKayit->setStyleSheet(butonStili);
    ui->btnGuncelle->setStyleSheet(butonStili);
    ui->btnSil->setStyleSheet(butonStili);
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::uyeListele()
{
    QSqlQuery query(QSqlDatabase::database());

    if (!query.exec("SELECT UyeNo, UyeAd, UyeSoyad FROM TBLUye")) {
        qDebug() << "Sorgu hatası:" << query.lastError().text();
        return;
    }

    ui->tblUyeler->setRowCount(0);
    int satir = 0;

    while (query.next()) {
        ui->tblUyeler->insertRow(satir);
        ui->tblUyeler->setItem(satir, 0, new QTableWidgetItem(query.value("UyeNo").toString()));
        ui->tblUyeler->setItem(satir, 1, new QTableWidgetItem(query.value("UyeAd").toString()));
        ui->tblUyeler->setItem(satir, 2, new QTableWidgetItem(query.value("UyeSoyad").toString()));
        satir++;
    }

    QStringList basliklar = {"Üye No", "Ad", "Soyad"};
    ui->tblUyeler->setHorizontalHeaderLabels(basliklar);
}

void Dialog::on_btnYeniKayit_clicked()
{
    QString ad = ui->txtUyeAd->text();
    QString soyad = ui->txtUyeSoyad->text();

    if (ad.isEmpty() || soyad.isEmpty()) {
        QMessageBox::warning(this, "Uyarı", "Lütfen Üye Adı ve Soyadı giriniz.");
        return;
    }

    QSqlQuery query(QSqlDatabase::database());
    query.prepare("INSERT INTO TBLUye (UyeAd, UyeSoyad) VALUES (:ad, :soyad)");
    query.bindValue(":ad", ad);
    query.bindValue(":soyad", soyad);

    if (query.exec()) {
        QMessageBox::information(this, "Başarılı", "Yeni üye eklendi.");
        ui->txtUyeAd->clear();
        ui->txtUyeSoyad->clear();
        uyeListele();
    } else {
        QMessageBox::critical(this, "HATA", "Kayıt eklenemedi!\n" + query.lastError().text());
    }
}

void Dialog::on_tblUyeler_cellClicked(int row, int column)
{
    QString uyeNo   = ui->tblUyeler->item(row, 0)->text();
    QString uyeAd   = ui->tblUyeler->item(row, 1)->text();
    QString uyeSoyad= ui->tblUyeler->item(row, 2)->text();

    ui->txtUyeNo->setText(uyeNo);
    ui->txtUyeAd->setText(uyeAd);
    ui->txtUyeSoyad->setText(uyeSoyad);
}


void Dialog::on_btnGuncelle_clicked()
{
    QString uyeNo = ui->txtUyeNo->text();
    QString ad    = ui->txtUyeAd->text();
    QString soyad = ui->txtUyeSoyad->text();

    if (uyeNo.isEmpty()) {
        QMessageBox::warning(this, "Uyarı", "Güncellenecek bir satır seçmediniz!");
        return;
    }

    if (ad.isEmpty() || soyad.isEmpty()) {
        QMessageBox::warning(this, "Uyarı", "Ad ve Soyad boş olamaz.");
        return;
    }

    QSqlQuery query(QSqlDatabase::database());
    query.prepare("UPDATE TBLUye SET UyeAd = :ad, UyeSoyad = :soyad WHERE UyeNo = :no");
    query.bindValue(":ad", ad);
    query.bindValue(":soyad", soyad);
    query.bindValue(":no", uyeNo);

    if (query.exec()) {
        QMessageBox::information(this, "Başarılı", "Kayıt güncellendi.");
        uyeListele();
        ui->txtUyeNo->clear();
        ui->txtUyeAd->clear();
        ui->txtUyeSoyad->clear();
    } else {
        QMessageBox::critical(this, "Hata", "Güncelleme başarısız!\n" + query.lastError().text());
    }
}


void Dialog::on_btnSil_clicked()
{
    QString uyeNo = ui->txtUyeNo->text();

    if (uyeNo.isEmpty()) {
        QMessageBox::warning(this, "Uyarı", "Lütfen silinecek üyeyi seçiniz.");
        return;
    }

    // Teslim edilmemiş kitap var mı kontrol et
    QSqlQuery kontrol(QSqlDatabase::database());
    kontrol.prepare("SELECT COUNT(*) FROM TBLOduncKitap WHERE FK_Uye_No = :no");
    kontrol.bindValue(":no", uyeNo);

    if (!kontrol.exec()) {
        QMessageBox::critical(this, "HATA", "Kontrol sorgusu çalıştırılamadı!\n" + kontrol.lastError().text());
        return;
    }

    kontrol.next();
    int aktifOdunc = kontrol.value(0).toInt();

    if (aktifOdunc > 0) {
        QMessageBox::critical(this, "Hata", "Bu üye silinemez. Üyenin henüz teslim etmediği kitaplar vardır.");
        return;
    }

    // Kitap teslim edilmişse silme işlemi yap
    QSqlQuery sil(QSqlDatabase::database());
    sil.prepare("DELETE FROM TBLUye WHERE UyeNo = :no");
    sil.bindValue(":no", uyeNo);

    if (sil.exec()) {
        QMessageBox::information(this, "Başarılı", "Üye başarıyla silindi.");
        ui->txtUyeNo->clear();
        ui->txtUyeAd->clear();
        ui->txtUyeSoyad->clear();
        uyeListele();
    } else {
        QMessageBox::critical(this, "HATA", "Silme işlemi başarısız!\n" + sil.lastError().text());
    }
}

