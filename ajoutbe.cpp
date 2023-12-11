#include"ajoutbe.h"
#include "ui_ajoutbe.h"
#include"Connection.h"
#include "produit.h"
#include <QSqlQuery>
#include <QMessageBox>
#include <QDate>
#include <QSqlError>




ajoutBE::ajoutBE(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ajoutBE)
{
    ui->setupUi(this);



        Connection m_db;
        if (!m_db.connOpen())
            {
                qDebug() << "Failed to open database";
            }
        else if(m_db.connOpen())
            {
                QSqlQuery query;
                query.prepare("SELECT * FROM produit");

                if (query.exec())
                    {

                        while (query.next())
                            {
                                QString numPro  = query.value(0).toString();
                                QString nomPro = query.value(1).toString();
                                Produit produit(numPro,nomPro);
                                recupId = produit.getId();
                                ui->numProduit->addItem(produit.toString());

                            }
                    }

            }
        else
            {
                qDebug() << "Database connection failed.";
            }

    }


ajoutBE::~ajoutBE()
{
    delete ui;
}


void ajoutBE::on_numProduit_activated(int index)
{
    QString itemText = ui->numProduit->currentText();
    QStringList parts = itemText.split(" - ");
        if (parts.size() == 2)
        {
                recupId = parts[0];
                QString nomPro = parts[1];
                int id = ui->numProduit->itemData(index).toInt();
                qDebug() << "ID: " << id;
                qDebug() << "NumPro: " << recupId;
                qDebug() << "NomPro: " << nomPro;
        }
}


void ajoutBE::on_ConfirmBEbtn_clicked()
{
    Connection m_db;
    if (!m_db.connOpen())
    {
        qDebug() << "Failed to open database";
    }
    m_db.connOpen();

    qtt = ui->qttEdit->text();

    QDateTime currentDate = QDateTime::currentDateTime();
    QString formatteDate = currentDate.toString("yyyy-MM-dd HH:mm:ss");
        qDebug() << "qtt: " << qtt;
        qDebug() << "date: " << currentDate;
        qDebug() << "NumPro: " << recupId;

    if (qtt.isEmpty())
    {
        QMessageBox::warning(this, "Attention", "Veuillez remplir les champs vides ou la date n'est pas valide.");
    }
    else
    {
        QSqlQuery req;
        QString requete = "INSERT INTO bonEntree(qtEntre, dateEntre, numPro) VALUES(:qtt, :date, :recupId);";
        req.prepare(requete);
        req.bindValue(":qtt", qtt);
        req.bindValue(":date", formatteDate);
        req.bindValue(":recupId", recupId.toInt());

        if (req.exec())
        {

            QSqlQuery updateQuery;
            QString updateRequete = "UPDATE produit SET stock = stock + :qtEntre WHERE numPro = :recupId;";
            updateQuery.prepare(updateRequete);
            updateQuery.bindValue(":qtEntre", qtt);
            updateQuery.bindValue(":recupId", recupId.toInt());

            if (updateQuery.exec())
            {
                QMessageBox::information(this, "Succès", "Le bon d'entrée a été ajouté avec succès, et le stock a été mis à jour.");
            }
            else
            {
                QMessageBox::warning(this, "Erreur", "Échec de la mise à jour du stock.");
                qDebug() << updateQuery.lastError();
            }
        }
        else
        {
            QMessageBox::warning(this, "Erreur", "Échec de l'ajout du bon d'entrée.");
            qDebug() << req.lastError();
        }

        m_db.connClose();
        this->close();
    }
}

void ajoutBE::on_CancelBEbtn_clicked()
{
    this->close ();
}

