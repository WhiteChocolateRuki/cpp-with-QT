****Kütüphane Otomasyon Sistemi****

Bu proje, kütüphane üyeleri, kitaplar ve ödünç işlemlerinin yönetimini sağlayan bir Kütüphane Otomasyon Sistemidir.
Qt ile MSSQL veritabanı bağlantısı kullanılarak geliştirilmiş olup; üye kayıt işlemleri, kitap işlemleri, ödünç alma ve ödünç iade etme modüllerini içermektedir.

Proje Amacı
Kütüphanede bulunan tüm kitapların ve üye bilgilerinin dijital ortamda yönetilmesi,
ödünç alma / teslim işlemlerinin kaydedilmesi, stok ve borç bilgilerinin otomatik olarak güncellenmesini sağlamaktır.

Veritabanı Yapısı
Proje MSSQL üzerinde aşağıdaki tablolarla çalışmaktadır:

üye

uye_no (int, otomatik)

uye_ad (text)

uye_soyad (text)

kitap

kitap_no (int, otomatik)

kitap_ad (text)

kitap_sayisi (int)

odunc_alinan

uye_no (int)

kitap_no (int)

odunc_alma_tarihi (text)

odunc_teslim_edilen

uye_no (int)

kitap_no (int)

alma_tarihi (text)

verme_tarihi (text)

borc (int)

Modüller ve Özellikler
1. Giriş Ekranı
Uygulama açıldığında veritabanına bağlantı yapılır.

Bağlantı durumu statusBar üzerinde gösterilir.

4 ana buton:

Üye İşlemleri

Kitap İşlemleri

Ödünç Alma

Ödünç Teslim Etme

2. Üye İşlemleri
Listeleme: Mevcut üyeler tablo üzerinde listelenir.

Yeni Kayıt: Ad ve soyad girilerek yeni üye eklenir.

Güncelleme: Seçili üyenin bilgileri değiştirilebilir.

Silme: Üyenin teslim etmediği kitap varsa silme işlemi engellenir.

Boş alan kontrolü yapılır, hatalı girişlerde uyarı verilir.

3. Kitap İşlemleri
Listeleme: Kütüphanedeki tüm kitaplar görüntülenir.

Detay Gösterme: Seçilen kitabın şu an kimde olduğu ve geçmişte kimlerin aldığı listelenir.

Yeni Kayıt: Kitap adı ve stok sayısı girilerek yeni kitap eklenir.

Güncelleme: Kitap adı veya stok bilgisi değiştirilebilir.

Silme: Kitap ödünçteyse silme işlemi engellenir.

4. Ödünç Alma İşlemleri
Üyeler ve kitaplar ayrı tablolarda listelenir.

Seçilen üye ve kitap bilgileri giriş alanlarına aktarılır.

Kontroller:

Aynı kitabı teslim edilmeden tekrar ödünç veremez.

Stok kontrolü yapılır, stoğu 0 olan kitap ödünç verilemez.

Ödünç alma tarihi kayıt altına alınır.

5. Ödünç Teslim Etme İşlemleri
Mevcut ödünç alınan kitaplar ve teslim edilenler ayrı tablolarda listelenir.

Teslim tarihi girilerek kitap iade edilir.

Borç Hesaplama:

15 gün içinde iade: Borç 0 TL

15 günden sonra: Gün başına 4 TL borç eklenir.

Teslim edilen kitap stok sayısına eklenir.

Çalışma Mantığı
Giriş ekranı üzerinden işlem seçilir.

İlgili modül açılır ve veritabanındaki bilgiler tabloya yüklenir.

Yapılan işlemler anında veritabanına kaydedilir.

Güncellemeler sonrası tablolar otomatik yenilenir.

Ödünç/teslim işlemlerinde kurallar ve stok kontrolleri otomatik yapılır.
