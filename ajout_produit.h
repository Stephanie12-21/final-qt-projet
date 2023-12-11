#ifndef AJOUT_PRODUIT_H
#define AJOUT_PRODUIT_H

#include <QDialog>
#include <QString>


namespace Ui {
class ajout_produit;
}

class ajout_produit : public QDialog
{
    Q_OBJECT

public:
    explicit ajout_produit(QWidget *parent = nullptr);
    QString design;
    QString dateAjout;
    QString stock;
    ~ajout_produit();

private slots:
    void on_cancelbtn_clicked();

    void on_confirmbtn_clicked();

private:
    Ui::ajout_produit *ui;
};

#endif // AJOUT_PRODUIT_H
