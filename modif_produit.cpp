#include "modif_produit.h"
#include "ui_modif_produit.h"
#include "Connection.h"
#include <QSqlQuery>
#include <QDate>
#include <QMessageBox>

modif_produit::modif_produit(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::modif_produit)
{
    ui->setupUi(this);
}

modif_produit::~modif_produit()
{
    delete ui;
}
QString modif_produit::numero(QString num)
{
    return num;
}

void modif_produit::getInfo(QString nom, QString stock)
{
  ui->designEdit->setText(nom);
  ui->stockEdit->setText(stock);
}

void modif_produit::on_cancelbtn_clicked()
{
    this->close();
}


void modif_produit::on_confirmbtn_clicked()
{
    Connection m_db;
    if (!m_db.connOpen())
        {
            qDebug() << "Failed to open database";
        }
    m_db.connOpen();

    design = ui->designEdit->text();
    stock = ui->stockEdit->text();
    QDateTime currentDate = QDateTime::currentDateTime();
    QString formatteDate =  currentDate.toString("yyyy-MM-dd HH:mm:ss");
    qDebug() << "design: " << design;
    qDebug() << "stock " << stock;
    qDebug() << "date: " <<  currentDate;

    if(design.isEmpty() || stock.isEmpty())
        {
            qDebug() << "Veuillez remplir tous les champs";

        }
    else
    {
            QSqlQuery requ;
            QString verifProd = "select count(*) from produit where design = :design";
            requ.prepare(verifProd);
            requ.bindValue(":design",design);
            requ.exec();
        if(requ.next())
        {


                    QSqlQuery req;
                    QString requete = "update produit set design = :design, stock = :stock, dateModifPro = :date where numPro = :num";
                    req.prepare(requete);
                    req.bindValue(":design", design);
                    req.bindValue(":stock", stock);
                    req.bindValue(":num", numPro);
                    req.bindValue(":date", formatteDate);


                    if(req.exec())
                    {
                         QMessageBox::information(this,"information","La modification du produit "+design+" est un véritable succès");

                    }
                     else
                    {
                          QMessageBox::information(this,"information","La modification du produit "+design+" est un malheureusement un échec");
                    }

                    m_db.connClose();
                    this->close();


       }
    }
}

