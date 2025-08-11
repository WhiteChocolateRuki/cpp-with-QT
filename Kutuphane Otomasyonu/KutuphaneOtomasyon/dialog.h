#ifndef DIALOG_H
#define DIALOG_H
#include <QDialog>

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = nullptr);
    ~Dialog();

private slots:
    void on_btnYeniKayit_clicked();
    void on_tblUyeler_cellClicked(int row, int column);
    void on_btnGuncelle_clicked();
    void on_btnSil_clicked();
private:
    Ui::Dialog *ui;
    void uyeListele();
};



#endif // DIALOG_H
