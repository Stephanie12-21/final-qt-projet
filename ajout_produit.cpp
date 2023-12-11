#include "ajout_produit.h"
#include "ui_ajout_produit.h"
#include "Connection.h"
#include <QSqlQuery>
#include <QMessageBox>
#include <QDate>

ajout_produit::ajout_produit(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ajout_produit)
    {
        ui->setupUi(this);
    }

ajout_produit::~ajout_produit()
    {
        delete ui;
    }

void ajout_produit::on_cancelbtn_clicked()
    {
        this->close();
    }


void ajout_produit::on_confirmbtn_clicked()
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
                 QMessageBox::information(this, "information","Veuillez remplir tous les champs avant de confirmer");

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
                        if(requ.value(0).toInt() > 0)
                            {
                              QMessageBox::warning(this,"warning", "Le produit  "+design+" est déjà saisi");
                            }
                        else
                                {

                                    QSqlQuery req;
                                    QString requete = "INSERT INTO produit(design,stock, date_colonne) VALUES(:design,:stock,:date);";
                                    req.prepare(requete);
                                    req.bindValue(":design",design);
                                    req.bindValue(":date", formatteDate);
                                    req.bindValue(":stock",stock);


                                    if (req.exec())
                                       {
                                           QMessageBox::information(this, "information", "L'ajout du produit " + design + " est un succès");

                                       }
                                     else
                                    {
                                        QMessageBox::information(this, "information","L'ajout du produit "+design+" est un échec");
                                    }

                                    m_db.connClose();
                                    this->close();

                                }

                    }
            }



    }

