Create database KutuphaneDB
use KutuphaneDB
CREATE TABLE TBLUye (
	UyeNo INT PRIMARY KEY IDENTITY(1,1),
	UyeAd NVARCHAR(100) NOT NULL,
	UyeSoyad NVARCHAR(100) NOT NULL
);

CREATE TABLE TBLKitap (
	KitapNo INT PRIMARY KEY IDENTITY(1,1),
	Kitap_ad NVARCHAR(100) NOT NULL,
	Kitap_Sayisi TINYINT NOT NULL CHECK (Kitap_Sayisi >= 0)
);

CREATE TABLE TBLOduncKitap (
	FK_Uye_No INT,
	FK_Kitap_No INT,
	Odunc_Alma_Tarihi DATETIME NOT NULL,
	PRIMARY KEY (FK_Uye_No, FK_Kitap_No, Odunc_Alma_Tarihi),
	FOREIGN KEY (FK_Uye_No) REFERENCES TBLUye(UyeNo) ON DELETE CASCADE,
	FOREIGN KEY (FK_Kitap_No) REFERENCES TBLKitap(KitapNo) ON DELETE CASCADE
);

CREATE TABLE TBLOduncTeslimEden (
	FK2_Uye_No INT,
	FK2_Kitap_No INT,
	Alma_Tarihi DATETIME NOT NULL,
	Verme_Tarihi DATETIME NOT NULL,
	Borc INT DEFAULT 0 CHECK (Borc >= 0),
	PRIMARY KEY (FK2_Uye_No, FK2_Kitap_No, Alma_Tarihi),
	FOREIGN KEY (FK2_Uye_No) REFERENCES TBLUye(UyeNo) ON DELETE CASCADE,
	FOREIGN KEY (FK2_Kitap_No) REFERENCES TBLKitap(KitapNo) ON DELETE CASCADE
);

INSERT INTO TBLUye (UyeAd, UyeSoyad) VALUES 
('Ali', 'Yýlmaz'),
('Ayþe', 'Kara'),
('Mehmet', 'Demir'),
('Zeynep', 'Aydýn'),
('Can', 'Þahin'),
('Elif', 'Güneþ'),
('Mert', 'Koç'),
('Fatma', 'Çelik'),
('Ahmet', 'Öztürk'),
('Hülya', 'Aslan');


INSERT INTO TBLKitap (Kitap_ad, Kitap_Sayisi) VALUES 
('1984', 3),
('Sefiller', 5),
('Suç ve Ceza', 4),
('Kürk Mantolu Madonna', 2),
('Tutunamayanlar', 1),
('Hayvan Çiftliði', 6),
('Simyacý', 3),
('Beyaz Diþ', 2),
('Saatleri Ayarlama Enstitüsü', 4),
('Ýnce Mehmed', 5);

INSERT INTO TBLOduncKitap (FK_Uye_No, FK_Kitap_No, Odunc_Alma_Tarihi) VALUES 
(1, 1, '2024-05-01'),
(2, 2, '2024-05-02'),
(3, 3, '2024-05-03'),
(4, 4, '2024-05-04'),
(5, 5, '2024-05-05'),
(6, 6, '2024-05-06'),
(7, 7, '2024-05-07'),
(8, 8, '2024-05-08'),
(9, 9, '2024-05-09'),
(10, 10, '2024-05-10');

INSERT INTO TBLOduncTeslimEden (FK2_Uye_No, FK2_Kitap_No, Alma_Tarihi, Verme_Tarihi, Borc) VALUES
(1, 1, '2024-04-20', '2024-04-27', 0),
(2, 2, '2024-04-22', '2024-04-29', 0),
(3, 3, '2024-04-23', '2024-05-02', 5),
(4, 4, '2024-04-25', '2024-05-05', 10),
(5, 5, '2024-04-27', '2024-05-06', 0),
(6, 6, '2024-04-28', '2024-05-08', 3),
(7, 7, '2024-04-29', '2024-05-09', 0),
(8, 8, '2024-05-01', '2024-05-10', 2),
(9, 9, '2024-05-02', '2024-05-11', 0),
(10, 10, '2024-05-03', '2024-05-12', 7);

select * from TBLUye
select * from TBLKitap
select * from TBLOduncKitap
select * from TBLOduncTeslimEden
