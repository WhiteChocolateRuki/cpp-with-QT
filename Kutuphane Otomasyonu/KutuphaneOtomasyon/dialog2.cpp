#include "dialog2.h"
#include "ui_dialog2.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QTableWidgetItem>
#include <QDebug>
#include <QDate>
#include <QMessageBox>  //bunu unutma!
// şeklinde eklenen değerler yorum açıklması ve modüllerin ne işe yaradığını açıklansın diye eklenmiştir.
Dialog2::Dialog2(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Dialog2)
{
    ui->setupUi(this);
    kitapListele();  //Kitapları yükle

    // Arayüz Stilleri
    ui->btnYeniKayit->setStyleSheet("background-color: #4CAF50; color: white; font-weight: bold; padding: 5px;");
    ui->btnGuncelle->setStyleSheet("background-color: #2196F3; color: white; font-weight: bold; padding: 5px;");
    ui->btnSil->setStyleSheet("background-color: #f44336; color: white; font-weight: bold; padding: 5px;");

    ui->txtKitapAd->setStyleSheet("background-color: white; border: 1px solid gray; padding: 2px;");
    ui->txtKitapNo->setStyleSheet("background-color: #f0f0f0; border: 1px solid gray; padding: 2px;");
    ui->txtStok->setStyleSheet("background-color: white; border: 1px solid gray; padding: 2px;");

    ui->tblUyeler->setStyleSheet("QTableWidget { background-color: #ffffff; alternate-background-color: #f9f9f9; }"
                                 "QHeaderView::section { background-color: #dddddd; font-weight: bold; }");

    ui->txtKitapOduncDurumu->setStyleSheet("QTableWidget { background-color: #ffffff; }"
                                           "QHeaderView::section { background-color: #f2f2f2; font-weight: bold; }");

    ui->txtKitapGecmisOduncDurumu->setStyleSheet("QTableWidget { background-color: #ffffff; }"
                                                 "QHeaderView::section { background-color: #f2f2f2; font-weight: bold; }");

}

Dialog2::~Dialog2()
{
    delete ui;
}

// Kitapları MSSQL'den çekip tabloya aktar
void Dialog2::kitapListele()
{
    QSqlQuery query(QSqlDatabase::database());

    if (!query.exec("SELECT KitapNo, Kitap_ad, Kitap_Sayisi FROM TBLKitap")) {
        qDebug() << "Sorgu hatası:" << query.lastError().text();
        return;
    }

    ui->tblUyeler->setRowCount(0);
    int satir = 0;

    while (query.next()) {
        ui->tblUyeler->insertRow(satir);
        ui->tblUyeler->setItem(satir, 0, new QTableWidgetItem(query.value("KitapNo").toString()));
        ui->tblUyeler->setItem(satir, 1, new QTableWidgetItem(query.value("Kitap_ad").toString()));
        ui->tblUyeler->setItem(satir, 2, new QTableWidgetItem(query.value("Kitap_Sayisi").toString()));
        satir++;
    }

    QStringList basliklar = {"Kitap No", "Kitap Adı", "Stok"};
    ui->tblUyeler->setHorizontalHeaderLabels(basliklar);
}

void Dialog2::on_tblUyeler_cellClicked(int row, int /*column*/)
{
    QString kitapNo  = ui->tblUyeler->item(row, 0)->text();
    QString kitapAd  = ui->tblUyeler->item(row, 1)->text();
    QString kitapStok= ui->tblUyeler->item(row, 2)->text();

    ui->txtKitapNo->setText(kitapNo);
    ui->txtKitapAd->setText(kitapAd);
    ui->txtStok->setText(kitapStok);

    //txtKitapOduncDurumu (TableWidget) temizle ve kolon hazırla
    ui->txtKitapOduncDurumu->clearContents();
    ui->txtKitapOduncDurumu->setRowCount(0);
    ui->txtKitapOduncDurumu->setColumnCount(3);
    QStringList kolonlar1 = {"Uye_No", "Kitap_No", "Odunc_Alma_Tarihi"};
    ui->txtKitapOduncDurumu->setHorizontalHeaderLabels(kolonlar1);

    QSqlQuery odunc(QSqlDatabase::database());
    odunc.prepare(R"(
        SELECT FK_Uye_No, FK_Kitap_No, Odunc_Alma_Tarihi
        FROM TBLOduncKitap
        WHERE FK_Kitap_No = :kitapNo
    )");
    odunc.bindValue(":kitapNo", kitapNo);

    if (odunc.exec()) {
        int row = 0;
        while (odunc.next()) {
            ui->txtKitapOduncDurumu->insertRow(row);
            ui->txtKitapOduncDurumu->setItem(row, 0, new QTableWidgetItem(odunc.value(0).toString()));
            ui->txtKitapOduncDurumu->setItem(row, 1, new QTableWidgetItem(odunc.value(1).toString()));
            ui->txtKitapOduncDurumu->setItem(row, 2, new QTableWidgetItem(odunc.value(2).toDate().toString("dd.MM.yyyy")));
            row++;
        }
    } else {
        qDebug() << "Ödünç sorgusu hatası:" << odunc.lastError().text();
    }

    //txtKitapGecmisOduncDurumu (TableWidget) temizle ve kolon hazırla
    ui->txtKitapGecmisOduncDurumu->clearContents();
    ui->txtKitapGecmisOduncDurumu->setRowCount(0);
    ui->txtKitapGecmisOduncDurumu->setColumnCount(5);
    QStringList kolonlar2 = {"Uye_No", "Kitap_No", "kitap_sayisi", "Alma_Tarihi", "Borc"};
    ui->txtKitapGecmisOduncDurumu->setHorizontalHeaderLabels(kolonlar2);

    QSqlQuery gecmis(QSqlDatabase::database());
    gecmis.prepare(R"(
        SELECT FK2_Uye_No, FK2_Kitap_No, 1 AS kitap_sayisi, Alma_Tarihi, Borc
        FROM TBLOduncTeslimEden
        WHERE FK2_Kitap_No = :kitapNo
        ORDER BY Alma_Tarihi DESC
    )");
    gecmis.bindValue(":kitapNo", kitapNo);

    if (gecmis.exec()) {
        int row = 0;
        while (gecmis.next()) {
            ui->txtKitapGecmisOduncDurumu->insertRow(row);
            ui->txtKitapGecmisOduncDurumu->setItem(row, 0, new QTableWidgetItem(gecmis.value(0).toString()));
            ui->txtKitapGecmisOduncDurumu->setItem(row, 1, new QTableWidgetItem(gecmis.value(1).toString()));
            ui->txtKitapGecmisOduncDurumu->setItem(row, 2, new QTableWidgetItem(gecmis.value(2).toString()));
            ui->txtKitapGecmisOduncDurumu->setItem(row, 3, new QTableWidgetItem(gecmis.value(3).toDate().toString("dd.MM.yyyy")));
            ui->txtKitapGecmisOduncDurumu->setItem(row, 4, new QTableWidgetItem(gecmis.value(4).toString()));
            row++;
        }
    } else {
        qDebug() << "Geçmiş ödünç sorgusu hatası:" << gecmis.lastError().text();
    }
}

void Dialog2::on_btnYeniKayit_clicked()
{
    QString kitapAd = ui->txtKitapAd->text().trimmed();
    QString stokStr = ui->txtStok->text().trimmed();

    // Kontroller
    if (kitapAd.isEmpty() || stokStr.isEmpty()) {
        QMessageBox::warning(this, "Hata", "Gerekli alanları doldurunuz!");
        return;
    }

    bool ok;
    int stok = stokStr.toInt(&ok);
    if (!ok || stok < 0) {
        QMessageBox::warning(this, "Hata", "Stok sayısı geçerli bir sayı olmalıdır.");
        return;
    }

    // Veritabanına ekle
    QSqlQuery query(QSqlDatabase::database());
    query.prepare("INSERT INTO TBLKitap (Kitap_ad, Kitap_Sayisi) VALUES (:ad, :stok)");
    query.bindValue(":ad", kitapAd);
    query.bindValue(":stok", stok);

    if (query.exec()) {
        QMessageBox::information(this, "Başarılı", "Yeni kitap başarıyla eklendi.");
        ui->txtKitapAd->clear();
        ui->txtStok->clear();
        kitapListele();  // tabloyu güncelle
    } else {
        QMessageBox::critical(this, "Veritabanı Hatası", query.lastError().text());
    }
}




void Dialog2::on_btnGuncelle_clicked()
{
    QString kitapNo  = ui->txtKitapNo->text().trimmed();
    QString kitapAd  = ui->txtKitapAd->text().trimmed();
    QString stokStr  = ui->txtStok->text().trimmed();

    if (kitapNo.isEmpty() || kitapAd.isEmpty() || stokStr.isEmpty()) {
        QMessageBox::warning(this, "Hata", "Lütfen tüm alanları doldurunuz!");
        return;
    }

    bool ok;
    int stok = stokStr.toInt(&ok);
    if (!ok || stok < 0) {
        QMessageBox::warning(this, "Hata", "Stok değeri geçerli bir sayı olmalıdır.");
        return;
    }

    QSqlQuery query(QSqlDatabase::database());
    query.prepare("UPDATE TBLKitap SET Kitap_ad = :ad, Kitap_Sayisi = :stok WHERE KitapNo = :no");
    query.bindValue(":ad", kitapAd);
    query.bindValue(":stok", stok);
    query.bindValue(":no", kitapNo);

    if (query.exec()) {
        QMessageBox::information(this, "Güncelleme", "Kitap bilgileri güncellendi.");
        kitapListele();
    } else {
        QMessageBox::critical(this, "Hata", "Güncelleme başarısız!\n" + query.lastError().text());
    }
}



#include <QMessageBox>  // Uyarılar için

void Dialog2::on_btnSil_clicked()
{
    QString kitapNo = ui->txtKitapNo->text();

    if (kitapNo.isEmpty()) {
        QMessageBox::warning(this, "Uyarı", "Lütfen silinecek kitabı seçiniz!");
        return;
    }

    //Önce kitap ödünçte mi kontrol edelim:
    QSqlQuery kontrol(QSqlDatabase::database());
    kontrol.prepare("SELECT COUNT(*) FROM TBLOduncKitap WHERE FK_Kitap_No = :no");
    kontrol.bindValue(":no", kitapNo);
    kontrol.exec();

    if (kontrol.next() && kontrol.value(0).toInt() > 0) {
        QMessageBox::critical(this, "Hata", "Bu kitap silinemez! Bir üyeye ödünç verilmiştir.");
        return;
    }

    // Silme işlemi:
    QSqlQuery sil(QSqlDatabase::database());
    sil.prepare("DELETE FROM TBLKitap WHERE KitapNo = :no");
    sil.bindValue(":no", kitapNo);

    if (sil.exec()) {
        QMessageBox::information(this, "Başarılı", "Kitap başarıyla silindi.");
        kitapListele();  // tabloyu güncelle
        ui->txtKitapNo->clear();
        ui->txtKitapAd->clear();
        ui->txtStok->clear();
        ui->txtKitapOduncDurumu->clear();
        ui->txtKitapGecmisOduncDurumu->clear();
    } else {
        QMessageBox::critical(this, "Hata", "Kitap silinemedi!\n" + sil.lastError().text());
    }
}


