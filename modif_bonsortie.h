#ifndef MODIF_BONSORTIE_H
#define MODIF_BONSORTIE_H

#include <QDialog>

#include <QString>

namespace Ui {
class modif_bonsortie;
}

class modif_bonsortie : public QDialog
{
        QString nomPro;


        QString id;
        QString nom;
        QString recupId;

    Q_OBJECT

public:
    explicit modif_bonsortie(QWidget *parent = nullptr);
        QString numBS;
        QString qttBS;
        QString oldQtSortie;
        QString newQtSortie;
        void getInfo(QString qttBS);
    ~modif_bonsortie();

private slots:
        void on_numProduit_activated(int index);

        void on_cancelBSbtn_clicked();

        void on_confirmBSbtn_clicked();

private:
    Ui::modif_bonsortie *ui;
};

#endif // MODIF_BONSORTIE_H
