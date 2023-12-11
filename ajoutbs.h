#ifndef AJOUTBS_H
#define AJOUTBS_H

#include <QDialog>
#include <QString>

namespace Ui {
class ajoutbs;
}

class ajoutbs : public QDialog
{
    Q_OBJECT

    QString nomPro;
    QString qtt;
    QString date;
    QString id;
    QString nom;
    QString recupId;

public:
    explicit ajoutbs(QWidget *parent = nullptr);
    ~ajoutbs();

private slots:
    void on_numProduit_activated(int index);

    void on_ConfirmBSbtn_clicked();

    void on_CancelBSbtn_clicked();

private:
    Ui::ajoutbs *ui;
};

#endif // AJOUTBS_H

