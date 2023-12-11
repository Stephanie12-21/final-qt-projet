#ifndef AJOUTBE_H
#define AJOUTBE_H

#include <QDialog>
#include <QString>


namespace Ui {
class ajoutBE;
}

class ajoutBE : public QDialog
{
    Q_OBJECT

    QString nomPro;
    QString qtt;
    QString date;
    QString id;
    QString nom;
    QString recupId;

public:
    explicit ajoutBE(QWidget *parent = nullptr);

    ~ajoutBE();

private slots:
    void on_numProduit_activated(int index);

     void on_ConfirmBEbtn_clicked();

     void on_CancelBEbtn_clicked();

private:
    Ui::ajoutBE *ui;
};

#endif // AJOUTBE_H
