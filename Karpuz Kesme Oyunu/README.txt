Karpuz Kesme Oyunu – Proje Açıklaması
Bu proje, kullanıcı reflekslerini ölçmek ve hızlı karar verme becerisini geliştirmek amacıyla tasarlanmış bir Karpuz Kesme Oyunudur.
Oyun, belirli süre içinde ekranda beliren karpuzları keserek puan toplama üzerine kuruludur.

Oyun Özellikleri
Rastgele Karpuz Üretimi:
Oyun esnasında, konumlar.txt dosyasından okunan konum bilgilerine göre belli aralıklarla rastgele karpuzlar ekrana gelir.

Karpuz Kesme Mekaniği:
Oyuncu, ekrandaki karpuzların üzerine tıkladığında karpuzun kesilmiş hali görüntülenir.
Kesilmiş karpuz görseli kısa bir süre ekranda kaldıktan sonra kaybolur.

Süre Sınırı:
Oyun toplam 30 saniye sürer. Amaç, bu süre zarfında olabildiğince fazla karpuz kesmektir.

Skor ve Bilgi Gösterimi:
Oyun sırasında ekranın üst kısmında;

Kalan süre

Kesilen karpuz sayısı

Kaçırılan karpuz sayısı
bilgileri anlık olarak gösterilir.

Oyun Sonu Skor Kaydı:
30 saniye sonunda oyun biter ve kesilen karpuz sayısı skorlar.txt dosyasına kaydedilir.
Daha önce oynanan oyunlardan alınan skorlar da bu dosyada saklanır.

Skor Karşılaştırması:
Oyun bitiminde, oyuncunun bu oyundaki skorunun önceki en yüksek skoru geçip geçmediği bilgisi ekranda gösterilir.
Ayrıca kesilen ve kaçırılan karpuz sayıları da rapor edilir.

Tam Ekran Oyun Penceresi:
Oyun penceresi bilgisayar ekranı boyutlarında açılır. Arka plan görseli Y=100 konumundan itibaren başlar.

Kaynak Dosyalar:
Projede görseller, sesler ve konumlar.txt / skorlar.txt gibi gerekli dosyalar önceden hazırlanmıştır.
Ayrıca projenin örnek çalışma videosu da mevcuttur.

Çalışma Mantığı
Başlangıç:
Oyun açıldığında, ekrandaki süre sayacı başlar.

Karpuzların Belirmesi:
konumlar.txt dosyasındaki koordinatlara göre karpuzlar belirli aralıklarla rastgele ekranda görünür.

Karpuz Kesme:
Oyuncu, karpuza tıkladığında kesilmiş versiyon görseli ekrana gelir ve kısa süre içinde kaybolur.
Puan artar, kesilen karpuz sayısı güncellenir.

Kaçırılan Karpuzlar:
Karpuz ekrandan kaybolduğunda ve oyuncu kesemediyse "kaçırılan" sayısı artar.

Oyun Bitişi:
30 saniye sonunda oyun durur, skor kaydedilir ve önceki skorlarla karşılaştırma yapılır.

Skor Gösterimi:
Oyuncuya toplam kesilen ve kaçırılan karpuz sayısı, ayrıca maksimum skor durumu gösterilir.

