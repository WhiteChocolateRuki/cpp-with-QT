#include "dialog4.h"
#include "ui_dialog4.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>
#include <QTableWidgetItem>
#include <QDebug>
#include <QDate>
// şeklinde eklenen değerler yorum açıklması ve modüllerin ne işe yaradığını açıklansın diye eklenmiştir.
Dialog4::Dialog4(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Dialog4)
{
    ui->setupUi(this);
    oduncAlinanlariListele();
    teslimEdilenleriListele();

    // Stil ayarları
    this->setStyleSheet("background-color: #f9f9f9;");

    // Başlık fontu (label_4...label_8 dahil olacak şekilde hepsi)
    QFont titleFont("Arial", 11, QFont::Bold);
    ui->label_4->setFont(titleFont);
    ui->label_5->setFont(titleFont);
    ui->label_6->setFont(titleFont);
    ui->label_7->setFont(titleFont);
    ui->label_8->setFont(titleFont);

    // Tablo kenarlıkları
    ui->txtKitapOduncDurumu->setStyleSheet("border: 1px solid gray; background-color: white;");
    ui->txtKitapGecmisOduncDurumu->setStyleSheet("border: 1px solid gray; background-color: white;");

    // Buton tasarımı
    ui->BtnOduncuVer->setStyleSheet(
        "QPushButton {"
        "  background-color: #2ecc71;"
        "  color: white;"
        "  font-weight: bold;"
        "  padding: 6px;"
        "  border-radius: 4px;"
        "}"
        "QPushButton:hover {"
        "  background-color: #27ae60;"
        "}"
        );

    // LineEdit’ler gri zeminli
    ui->txtUyeNo->setStyleSheet("background-color: #eeeeee; padding: 2px;");
    ui->txtKitapNo->setStyleSheet("background-color: #eeeeee; padding: 2px;");

    // DateEdit stil
    ui->dateEdit->setStyleSheet("background-color: white; padding: 2px;");
}

Dialog4::~Dialog4()
{
    delete ui;
}

//Ödünç alınan kitapları listele
void Dialog4::oduncAlinanlariListele()
{
    QSqlQuery query("SELECT FK_Uye_No, FK_Kitap_No, Odunc_Alma_Tarihi FROM TBLOduncKitap", QSqlDatabase::database());

    ui->txtKitapOduncDurumu->setRowCount(0);
    int row = 0;
    while (query.next()) {
        ui->txtKitapOduncDurumu->insertRow(row);
        ui->txtKitapOduncDurumu->setItem(row, 0, new QTableWidgetItem(query.value(0).toString()));
        ui->txtKitapOduncDurumu->setItem(row, 1, new QTableWidgetItem(query.value(1).toString()));
        ui->txtKitapOduncDurumu->setItem(row, 2, new QTableWidgetItem(query.value(2).toDate().toString("dd.MM.yyyy")));
        row++;
    }
}

//Teslim edilen kitapları listele
void Dialog4::teslimEdilenleriListele()
{
    QSqlQuery query("SELECT FK2_Uye_No, FK2_Kitap_No, Alma_Tarihi, Verme_Tarihi, Borc FROM TBLOduncTeslimEden", QSqlDatabase::database());

    ui->txtKitapGecmisOduncDurumu->setRowCount(0);
    int row = 0;
    while (query.next()) {
        ui->txtKitapGecmisOduncDurumu->insertRow(row);
        for (int i = 0; i < 5; ++i) {
            ui->txtKitapGecmisOduncDurumu->setItem(row, i, new QTableWidgetItem(query.value(i).toString()));
        }
        row++;
    }
}

//Tabloya tıklandığında textbox'lara verileri getir
void Dialog4::on_txtKitapOduncDurumu_cellClicked(int row, int)
{
    QString uyeNo   = ui->txtKitapOduncDurumu->item(row, 0)->text();
    QString kitapNo = ui->txtKitapOduncDurumu->item(row, 1)->text();

    ui->txtUyeNo->setText(uyeNo);
    ui->txtKitapNo->setText(kitapNo);
}

void Dialog4::on_BtnOduncuVer_clicked()
{
    QString uyeNo = ui->txtUyeNo->text();
    QString kitapNo = ui->txtKitapNo->text();
    QDate vermeTarihi = ui->dateEdit->date();

    // alma tarihini bul
    QSqlQuery findQuery(QSqlDatabase::database());
    findQuery.prepare("SELECT Odunc_Alma_Tarihi FROM TBLOduncKitap WHERE FK_Uye_No = :uye AND FK_Kitap_No = :kitap");
    findQuery.bindValue(":uye", uyeNo);
    findQuery.bindValue(":kitap", kitapNo);

    if (!findQuery.exec() || !findQuery.next()) {
        QMessageBox::warning(this, "Hata", "Alma tarihi bulunamadı!");
        return;
    }

    QDate almaTarihi = findQuery.value(0).toDate();
    int gunFarki = almaTarihi.daysTo(vermeTarihi);
    int borc = (gunFarki > 15) ? (gunFarki - 15) * 4 : 0;

    // ➤ Teslim zaten yapıldıysa tekrar yapılmasın
    QSqlQuery kontrolTeslim(QSqlDatabase::database());
    kontrolTeslim.prepare(R"(
        SELECT COUNT(*) FROM TBLOduncTeslimEden
        WHERE FK2_Uye_No = :uye AND FK2_Kitap_No = :kitap AND Alma_Tarihi = :alma
    )");
    kontrolTeslim.bindValue(":uye", uyeNo);
    kontrolTeslim.bindValue(":kitap", kitapNo);
    kontrolTeslim.bindValue(":alma", almaTarihi.toString("yyyy-MM-dd"));

    if (kontrolTeslim.exec() && kontrolTeslim.next()) {
        if (kontrolTeslim.value(0).toInt() > 0) {
            QMessageBox::warning(this, "Hata", "Bu teslim işlemi zaten yapılmış. Tekrar kaydedilemez!");
            return;
        }
    }

    // teslim tablosuna ekle
    QSqlQuery insertQuery(QSqlDatabase::database());
    insertQuery.prepare(R"(
        INSERT INTO TBLOduncTeslimEden (FK2_Uye_No, FK2_Kitap_No, Alma_Tarihi, Verme_Tarihi, Borc)
        VALUES (:uye, :kitap, :alma, :verme, :borc)
    )");
    insertQuery.bindValue(":uye", uyeNo);
    insertQuery.bindValue(":kitap", kitapNo);
    insertQuery.bindValue(":alma", almaTarihi.toString("yyyy-MM-dd"));
    insertQuery.bindValue(":verme", vermeTarihi.toString("yyyy-MM-dd"));
    insertQuery.bindValue(":borc", borc);

    if (!insertQuery.exec()) {
        QMessageBox::critical(this, "Hata", "Teslim kaydı yapılamadı:\n" + insertQuery.lastError().text());
        return;
    }

    // ödünç kaydını sil
    QSqlQuery deleteQuery(QSqlDatabase::database());
    deleteQuery.prepare("DELETE FROM TBLOduncKitap WHERE FK_Uye_No = :uye AND FK_Kitap_No = :kitap");
    deleteQuery.bindValue(":uye", uyeNo);
    deleteQuery.bindValue(":kitap", kitapNo);
    deleteQuery.exec();

    // kitap stoğunu 1 artır
    QSqlQuery guncelleStok(QSqlDatabase::database());
    guncelleStok.prepare("UPDATE TBLKitap SET Kitap_Sayisi = Kitap_Sayisi + 1 WHERE KitapNo = :kitap");
    guncelleStok.bindValue(":kitap", kitapNo);
    if (!guncelleStok.exec()) {
        QMessageBox::warning(this, "Uyarı", "Stok güncellenemedi!\n" + guncelleStok.lastError().text());
    }

    QMessageBox::information(this, "Başarılı", "Teslim işlemi tamamlandı!");

    oduncAlinanlariListele();
    teslimEdilenleriListele();

    ui->txtUyeNo->clear();
    ui->txtKitapNo->clear();
}



