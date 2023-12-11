#ifndef MODIF_PRODUIT_H
#define MODIF_PRODUIT_H

#include <QDialog>


namespace Ui
{
class modif_produit;
}

class modif_produit : public QDialog
{
    Q_OBJECT

public:
    explicit modif_produit(QWidget *parent = nullptr);
    QString numPro;
    QString design;
    QString stock;
    QString numero(QString num);
    void getInfo(QString nom, QString stock);
    ~modif_produit();

private slots:
    void on_cancelbtn_clicked();

    void on_confirmbtn_clicked();

private:
    Ui::modif_produit *ui;
};

#endif // MODIF_PRODUIT_H
