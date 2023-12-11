#include "modif_bonsortie.h"
#include "ui_modif_bonsortie.h"
#include "Connection.h"
#include "produit.h"
#include <QSqlQuery>
#include <QDate>
#include <QMessageBox>
#include <QSqlError>

modif_bonsortie::modif_bonsortie(QWidget*parent) :
    QDialog(parent),
    ui(new Ui::modif_bonsortie)
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

modif_bonsortie::~modif_bonsortie()
{
    delete ui;
}
void modif_bonsortie::getInfo(QString qttBS)
{

    ui->qttEdit->setText(qttBS);
}

void modif_bonsortie::on_numProduit_activated(int index)
{
    QString itemText = ui->numProduit->currentText();
    QStringList parts = itemText.split(" - ");
    if (parts.size() == 2)
    {
        recupId = parts[0];
        // Ajoutez cette ligne pour récupérer la quantité associée à l'ID
        oldQtSortie = ui->numProduit->itemData(index, Qt::UserRole).toString();
        QString nomPro = parts[1];
        int id = recupId.toInt(); // Vous avez déjà recupId comme QString, pas besoin de la convertir à nouveau
        qDebug() << "ID: " << id;
        qDebug() << "NumPro: " << recupId;
        qDebug() << "NomPro: " << nomPro;
    }
}

void modif_bonsortie::on_cancelBSbtn_clicked()
{
    this->close();
}

void modif_bonsortie::on_confirmBSbtn_clicked()
{
    Connection m_db;
    if (!m_db.connOpen())
    {
        qDebug() << "Échec d'ouverture de la base de données";
    }

    m_db.connOpen();
    newQtSortie = ui->qttEdit->text();

    QDateTime currentDate = QDateTime::currentDateTime();
    QString formatteDate = currentDate.toString("yyyy-MM-dd HH:mm:ss");

    qDebug() << "qtt : " << newQtSortie;
    qDebug() << "date : " << currentDate;
    qDebug() << "NumPro : " << recupId;

    if (newQtSortie.isEmpty())
    {
        QMessageBox::warning(this, "Attention", "Veuillez remplir les champs vides.");
    }
    else
    {
        QSqlQuery req;

        QString requete = "UPDATE bonSortie SET qtSortie = :qtt, dateBSModifPro = :date WHERE numPro = :recupId;";
        req.prepare(requete);
        req.bindValue(":qtt", newQtSortie);
        req.bindValue(":date", formatteDate);
        req.bindValue(":recupId", recupId.toInt());

        if (req.exec())
        {
            QMessageBox::information(this, "Succès", "Le bon de sortie a été modifié avec succès.");
        }
        else
        {
            QMessageBox::warning(this, "Erreur", "Échec de la modification du bon de sortie.");
            qDebug() << req.lastError();
        }

        m_db.connClose();
        this->close();
    }
}
