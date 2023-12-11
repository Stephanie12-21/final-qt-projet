#include "modif_bonentre.h"
#include "ui_modif_bonentre.h"
#include "Connection.h"
#include "produit.h"
#include <QSqlQuery>
#include <QDate>
#include <QMessageBox>
#include <QSqlError>

modif_bonEntre::modif_bonEntre(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::modif_bonEntre)
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

modif_bonEntre::~modif_bonEntre()
{
    delete ui;
}

void modif_bonEntre::getInfo(QString qttBE)
{

    ui->qttEdit->setText(qttBE);
}

void modif_bonEntre::on_cancelBEbtn_clicked()
{
    this->close();
}

void modif_bonEntre::on_numProduit_activated(int index)
{
    QString itemText = ui->numProduit->currentText();
    QStringList parts = itemText.split(" - ");
    if (parts.size() == 2)
    {
        recupId = parts[0];

        oldQtEntre = ui->numProduit->itemData(index, Qt::UserRole).toString();
        QString nomPro = parts[1];
        int id = recupId.toInt();
        qDebug() << "ID: " << id;
        qDebug() << "NumPro: " << recupId;
        qDebug() << "NomPro: " << nomPro;
    }
}

void modif_bonEntre::on_confirmBEbtn_clicked()
{
    {
        Connection m_db;
        if (!m_db.connOpen())
        {
            qDebug() << "Failed to open database";
        }

        Entre = ui->qttEdit->text();

        QDateTime currentDate = QDateTime::currentDateTime();
        QString formatteDate = currentDate.toString("yyyy-MM-dd HH:mm:ss");

        qDebug() << "qtt: " << Entre;
        qDebug() << "date: " << currentDate;
        qDebug() << "NumPro: " << recupId;

        if (Entre.isEmpty())
        {
            QMessageBox::warning(this, "Attention", "Veuillez remplir les champs vides.");
        }
        else
        {
            QSqlQuery req;
            QString requete = "UPDATE bonEntree SET qtEntre = :qtt, dateBEModifPro = :date WHERE numPro = :recupId;";
            req.prepare(requete);
            req.bindValue(":qtt", Entre);
            req.bindValue(":date", formatteDate);
            req.bindValue(":recupId", recupId.toInt());

            if (req.exec())
            {
                QMessageBox::information(this, "Succès", "Le bon d'entrée a été modifié avec succès.");

            }
            else
            {
                QMessageBox::warning(this, "Erreur", "Échec de la modification du bon d'entrée.");
                qDebug() << req.lastError();
            }

            m_db.connClose();
            this->close();
        }
    }
}


