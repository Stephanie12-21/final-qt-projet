#ifndef MODIF_BONENTRE_H
#define MODIF_BONENTRE_H

#include <QDialog>

#include <QString>

namespace Ui {
class modif_bonEntre;
}

class modif_bonEntre : public QDialog
{
    QString nomPro;


    QString id;
    QString nom;
    QString recupId;
    Q_OBJECT

public:
    explicit modif_bonEntre(QWidget *parent = nullptr);
    QString numBE;
    QString qttBE;
    QString Entre;
    QString oldQtEntre;

    void getInfo(QString qttBE);



    ~modif_bonEntre();

private slots:
    void on_numProduit_activated(const QString &arg1);

    void on_numProduit_activated(int index);

    void on_cancelBEbtn_clicked();

    void on_confirmBEbtn_clicked();
    void updateProductStockForSelectedRow(QString recupId, QString Entre);

private:
    Ui::modif_bonEntre *ui;
};

#endif // MODIF_BONENTRE_H
