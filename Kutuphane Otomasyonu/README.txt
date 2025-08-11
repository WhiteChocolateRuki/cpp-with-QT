# Qt Kütüphane Otomasyon Sistemi

Bu proje, **Qt Framework** kullanılarak geliştirilen bir **Kütüphane Otomasyon Sistemi**dir.  
Amaç; üyelerin, kitapların ve ödünç işlemlerinin yönetimini kolaylaştıran, görsel arayüzlü bir masaüstü uygulaması oluşturmaktır.  
Proje **MSSQL veritabanı** ile bağlantılı çalışır.

---

## Özellikler

###  Üye Yönetimi (`Dialog`)
- Üye **listeleme**
- Yeni üye **ekleme**
- Üye bilgilerini **güncelleme**
- Üye **silme** (Eğer üyenin teslim edilmemiş kitabı varsa silme işlemi engellenir)
- **Veritabanı** üzerinden MSSQL sorguları ile işlem
- **Arayüz özellikleri**:
  - Arka plan rengi
  - Özelleştirilmiş buton stilleri (hover & pressed efektleri)
  - Gelişmiş tablo görünümü
  - Kullanıcı dostu uyarı mesajları (`QMessageBox`)

---

### Kitap Yönetimi (`Dialog2`)
- Kitap **listeleme**
- Yeni kitap **ekleme** (stok miktarı ile)
- Kitap **güncelleme**
- Kitap **silme** (Eğer kitap ödünç verilmişse silme işlemi engellenir)
- Kitabın:
  - **Aktif ödünç durumu** (kimde olduğu ve tarih bilgisi)
  - **Geçmiş ödünç kayıtları** (iade tarihi, borç bilgisi)
- **Arayüz özellikleri**:
  - Farklı renklerde butonlar (Ekle, Güncelle, Sil)
  - Stok giriş kontrolü (negatif sayı engelleme)
  - Tablolarda alternating renkler
  - Kullanıcı dostu hata ve bilgi mesajları

---

###  Ödünç Kitap İşlemleri (`Dialog3`)
- Üye ve kitap listelerini ayrı tablolarda gösterme
- Üye veya kitap seçildiğinde ilgili bilgileri otomatik olarak doldurma
- **Ödünç verme işlemi**
- **Ödünçteki kitapların listesi**
- Arayüz özellikleri:
  - İkonlu butonlar (`:/img/odunc.png`)
  - Modern tablo tasarımı
  - Seçim rengi ve hover efektleri
  - Tarih seçim alanı (`QDateEdit`)

---

## 🛠 Kullanılan Teknolojiler ve Modüller

### **Qt Modülleri**
- `QDialog` → Pencere ve arayüz yönetimi
- `QSqlQuery`, `QSqlError` → MSSQL veritabanı işlemleri
- `QTableWidget`, `QTableWidgetItem` → Veri tabloları
- `QMessageBox` → Kullanıcıya bilgi, uyarı ve hata mesajları
- `QDate` → Tarih formatlama ve görüntüleme
- `QIcon` → Buton ikonları

### **Veritabanı**
- **MSSQL** üzerinde şu tablolar kullanılmıştır:
  - `TBLUye` → Üye bilgileri
  - `TBLKitap` → Kitap bilgileri
  - `TBLOduncKitap` → Teslim edilmemiş ödünç kitaplar
  - `TBLOduncTeslimEden` → Teslim edilmiş kitap geçmişi


