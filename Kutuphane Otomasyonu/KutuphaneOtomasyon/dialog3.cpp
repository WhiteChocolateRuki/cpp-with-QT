#include "dialog3.h"
#include "ui_dialog3.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QTableWidgetItem>
#include <QDebug>
#include <QMessageBox>  //bunu unutma!
// şeklinde eklenen değerler yorum açıklması ve modüllerin ne işe yaradığını açıklansın diye eklenmiştir.
Dialog3::Dialog3(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Dialog3)
{
    ui->setupUi(this);
    uyeListele();
    kitapListele();
    oduncListele();
    kitapOduncListele();


    // Stil Ayarları

    // Button stilleri ve ikon
    ui->BtnOduncVer->setIcon(QIcon(":/img/odunc.png"));
    ui->BtnOduncVer->setIconSize(QSize(32, 32));
    ui->BtnOduncVer->setStyleSheet("QPushButton {"
                                   "background-color: #28a745;"
                                   "color: white;"
                                   "font-weight: bold;"
                                   "border-radius: 6px;"
                                   "padding: 6px;"
                                   "}"
                                   "QPushButton:hover {"
                                   "background-color: #218838;"
                                   "}");

    // Table stilleri
    QString tableStyle =
        "QHeaderView::section {"
        "   background-color: #343a40;"
        "   color: white;"
        "   font-weight: bold;"
        "   padding: 4px;"
        "}"
        "QTableWidget {"
        "   gridline-color: #dee2e6;"
        "   background-color: #ffffff;"
        "   alternate-background-color: #f8f9fa;"
        "   selection-background-color: #007bff;"
        "   selection-color: white;"
        "}";

    ui->TblUyeler->setStyleSheet(tableStyle);
    ui->TblKitaplar->setStyleSheet(tableStyle);
    ui->txtKitapOduncDurumu->setStyleSheet(tableStyle);

    // LineEdit stilleri
    QString lineEditStyle =
        "QLineEdit {"
        "   background-color: #f1f1f1;"
        "   border: 1px solid gray;"
        "   border-radius: 4px;"
        "   padding: 4px;"
        "}";

    ui->txtUyeNo->setStyleSheet(lineEditStyle);
    ui->txtKitapNo->setStyleSheet(lineEditStyle);

    // DateEdit stili
    ui->dateEdit->setStyleSheet(lineEditStyle);

    // Başlık font ve rengi
    ui->label->setStyleSheet("color: red; font-weight: bold; font-size: 16px;");



}

Dialog3::~Dialog3()
{
    delete ui;
}

void Dialog3::uyeListele()
{
    QSqlQuery query("SELECT UyeNo, UyeAd, UyeSoyad FROM TBLUye", QSqlDatabase::database());

    ui->TblUyeler->setRowCount(0);
    int satir = 0;
    while (query.next()) {
        ui->TblUyeler->insertRow(satir);
        ui->TblUyeler->setItem(satir, 0, new QTableWidgetItem(query.value("UyeNo").toString()));
        ui->TblUyeler->setItem(satir, 1, new QTableWidgetItem(query.value("UyeAd").toString()));
        ui->TblUyeler->setItem(satir, 2, new QTableWidgetItem(query.value("UyeSoyad").toString()));
        satir++;
    }

    QStringList basliklar = {"Üye No", "Ad", "Soyad"};
    ui->TblUyeler->setHorizontalHeaderLabels(basliklar);
}

void Dialog3::kitapListele()
{
    QSqlQuery query("SELECT KitapNo, Kitap_ad, Kitap_Sayisi FROM TBLKitap", QSqlDatabase::database());

    ui->TblKitaplar->setRowCount(0);
    int satir = 0;
    while (query.next()) {
        ui->TblKitaplar->insertRow(satir);
        ui->TblKitaplar->setItem(satir, 0, new QTableWidgetItem(query.value("KitapNo").toString()));
        ui->TblKitaplar->setItem(satir, 1, new QTableWidgetItem(query.value("Kitap_ad").toString()));
        ui->TblKitaplar->setItem(satir, 2, new QTableWidgetItem(query.value("Kitap_Sayisi").toString()));
        satir++;
    }

    QStringList basliklar = {"Kitap No", "Ad", "Stok"};
    ui->TblKitaplar->setHorizontalHeaderLabels(basliklar);
}
void Dialog3::oduncListele()
{
    QSqlQuery query(QSqlDatabase::database());
    query.prepare("SELECT FK_Uye_No, FK_Kitap_No, Odunc_Alma_Tarihi FROM TBLOduncKitap");

    ui->txtKitapOduncDurumu->setRowCount(0);
    int satir = 0;

    if (query.exec()) {
        while (query.next()) {
            ui->txtKitapOduncDurumu->insertRow(satir);
            ui->txtKitapOduncDurumu->setItem(satir, 0, new QTableWidgetItem(query.value("FK_Uye_No").toString()));
            ui->txtKitapOduncDurumu->setItem(satir, 1, new QTableWidgetItem(query.value("FK_Kitap_No").toString()));
            ui->txtKitapOduncDurumu->setItem(satir, 2, new QTableWidgetItem(query.value("Odunc_Alma_Tarihi").toDate().toString("dd.MM.yyyy")));
            satir++;
        }
        QStringList basliklar = {"Üye No", "Kitap No", "Ödünç Alma Tarihi"};
        ui->txtKitapOduncDurumu->setHorizontalHeaderLabels(basliklar);
    } else {
        qDebug() << "HATA:" << query.lastError().text();
    }
}


// Üyeye tıklanınca txtUyeNo'ya yaz
void Dialog3::on_TblUyeler_cellClicked(int row, int /*column*/)
{
    QString uyeNo = ui->TblUyeler->item(row, 0)->text();
    ui->txtUyeNo->setText(uyeNo);
}

// Kitaba tıklanınca txtKitapNo'ya yaz
void Dialog3::on_TblKitaplar_cellClicked(int row, int /*column*/)
{
    QString kitapNo = ui->TblKitaplar->item(row, 0)->text();
    ui->txtKitapNo->setText(kitapNo);
}

void Dialog3::on_BtnOduncVer_clicked()
{
    QString uyeNo = ui->txtUyeNo->text();
    QString kitapNo = ui->txtKitapNo->text();
    QString tarih = ui->dateEdit->date().toString("yyyy-MM-dd");

    // Boş alan kontrolü
    if (uyeNo.isEmpty() || kitapNo.isEmpty()) {
        QMessageBox::warning(this, "Uyarı", "Lütfen Üye ve Kitap seçiniz.");
        return;
    }

    QSqlQuery kontrol(QSqlDatabase::database());

    // Üye hâlihazırda bu kitabı aldıysa (teslim etmediyse) engelle
    kontrol.prepare("SELECT * FROM TBLOduncKitap WHERE FK_Uye_No = :uye AND FK_Kitap_No = :kitap");
    kontrol.bindValue(":uye", uyeNo);
    kontrol.bindValue(":kitap", kitapNo);
    kontrol.exec();

    if (kontrol.next()) {
        QMessageBox::information(this, "Bilgi", "Bu üye bu kitabın bir tanesini şu an ödünç almış, tekrar ödünç verilemez!");
        return;
    }

    // Kitabın stokta olup olmadığını kontrol et
    QSqlQuery stokSorgu(QSqlDatabase::database());
    stokSorgu.prepare(R"(
        SELECT Kitap_Sayisi FROM TBLKitap WHERE KitapNo = :kitap
    )");
    stokSorgu.bindValue(":kitap", kitapNo);
    stokSorgu.exec();

    int stok = 0;
    if (stokSorgu.next()) {
        stok = stokSorgu.value("Kitap_Sayisi").toInt();
    }

    // Bu kitaptan stokta kaç tane ödünç verildi?
    QSqlQuery oduncSayi(QSqlDatabase::database());
    oduncSayi.prepare(R"(
        SELECT COUNT(*) FROM TBLOduncKitap WHERE FK_Kitap_No = :kitap
    )");
    oduncSayi.bindValue(":kitap", kitapNo);
    oduncSayi.exec();
    oduncSayi.next();
    int oduncVerilen = oduncSayi.value(0).toInt();

    if (stok <= oduncVerilen) {
        QMessageBox::critical(this, "HATA", "Stokta bu kitaptan kalmamış! Ödünç verilemez.");
        return;
    }

    //Ekleme işlemi
    QSqlQuery ekle(QSqlDatabase::database());
    ekle.prepare(R"(
        INSERT INTO TBLOduncKitap (FK_Uye_No, FK_Kitap_No, Odunc_Alma_Tarihi)
        VALUES (:uye, :kitap, :tarih)
    )");
    ekle.bindValue(":uye", uyeNo);
    ekle.bindValue(":kitap", kitapNo);
    ekle.bindValue(":tarih", tarih);

    if (ekle.exec()) {
        QMessageBox::information(this, "Başarılı", "Ödünç verme işlemi başarıyla tamamlandı.");

        // Kitap stoğunu 1 azalt
        QSqlQuery guncelle(QSqlDatabase::database());
        guncelle.prepare("UPDATE TBLKitap SET Kitap_Sayisi = Kitap_Sayisi - 1 WHERE KitapNo = :kitap");
        guncelle.bindValue(":kitap", kitapNo);
        if (!guncelle.exec()) {
            QMessageBox::warning(this, "Stok Uyarısı", "Kitap stoğu güncellenemedi!\n" + guncelle.lastError().text());
        }

        kitapOduncListele(); // tabloyu güncelle
        kitapListele();      // kitap listesi stokla güncellensin
    } else {
        QMessageBox::critical(this, "HATA", "Kayıt eklenemedi!\n" + ekle.lastError().text());
    }
}


void Dialog3::kitapOduncListele()
{
    QSqlQuery query("SELECT FK_Uye_No, FK_Kitap_No, Odunc_Alma_Tarihi FROM TBLOduncKitap", QSqlDatabase::database());

    ui->txtKitapOduncDurumu->setRowCount(0);
    int satir = 0;
    while (query.next()) {
        ui->txtKitapOduncDurumu->insertRow(satir);
        ui->txtKitapOduncDurumu->setItem(satir, 0, new QTableWidgetItem(query.value(0).toString()));
        ui->txtKitapOduncDurumu->setItem(satir, 1, new QTableWidgetItem(query.value(1).toString()));
        ui->txtKitapOduncDurumu->setItem(satir, 2, new QTableWidgetItem(query.value(2).toDate().toString("dd.MM.yyyy")));
        satir++;
    }
}


