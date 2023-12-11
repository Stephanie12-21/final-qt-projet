#include "ajoutbs.h"
#include "ui_ajoutbs.h"
#include "Connection.h"
#include "produit.h"
#include <QSqlQuery>
#include <QMessageBox>
#include <QDate>
#include <QSqlError>

ajoutbs::ajoutbs(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ajoutbs)
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

ajoutbs::~ajoutbs()
{
    delete ui;
}

void ajoutbs::on_numProduit_activated(int index)
{
    QString itemText = ui->numProduit->currentText();
    QStringList parts = itemText.split(" - "); // Utilisation du caractère pipe comme séparateur
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


void ajoutbs::on_ConfirmBSbtn_clicked()
{
    Connection m_db;
    if (!m_db.connOpen())
    {
        qDebug() << "Failed to open database";
    }
    m_db.connOpen();

    qtt = ui->qttBSEdit->text();

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
        QString requete = "INSERT INTO bonSortie (qtSortie, dateSortie, numPro) VALUES(:qtt, :date, :recupId);";
        req.prepare(requete);
        req.bindValue(":qtt", qtt);
        req.bindValue(":date", currentDate.toString("yyyy-MM-dd HH:mm:ss"));
        req.bindValue(":recupId", recupId.toInt());

// pour connaître le stock du produit sélectionné lors de l'ajout du bon d'entrée
        QSqlQuery queries;
        QString com = "SELECT stock FROM produit where numPro =:recupId;";
        queries.prepare(com);
        queries.bindValue(":recupId",recupId);
        queries.exec();
        while (queries.next())
        {
            if (queries.value(0).toInt() - qtt.toInt() <0)
            {
                QMessageBox::warning(this, "avertissement","vous ne pouvez pas effectuer cette opération, votre stock semble insuffisant.");
            }
            else{
                if (req.exec())
                {
                    // Mise à jour du stock dans la table produit
                    QSqlQuery updateQuery;
                    QString updateRequete = "UPDATE produit SET stock = stock - :qtSortie WHERE numPro = :recupId;";
                    updateQuery.prepare(updateRequete);
                    updateQuery.bindValue(":qtSortie", qtt);
                    updateQuery.bindValue(":recupId", recupId.toInt());

                    if (updateQuery.exec())
                    {
                        QMessageBox::information(this, "Succès", "Le bon de sortie a été ajouté avec succès, et le stock a été mis à jour.");
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

}

}


void ajoutbs::on_CancelBSbtn_clicked()
{
    this->close();
}

