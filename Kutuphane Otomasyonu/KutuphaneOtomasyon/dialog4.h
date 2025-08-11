#ifndef DIALOG4_H
#define DIALOG4_H
#include <QDialog>

namespace Ui {
class Dialog4;
}

class Dialog4 : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog4(QWidget *parent = nullptr);
    ~Dialog4();

private slots:
    void on_txtKitapOduncDurumu_cellClicked(int row, int column);
    void on_BtnOduncuVer_clicked();

private:
    Ui::Dialog4 *ui;
    void oduncAlinanlariListele();
    void teslimEdilenleriListele();
};

#endif // DIALOG4_H
