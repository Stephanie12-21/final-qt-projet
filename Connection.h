#ifndef CONNECTION_H
#define CONNECTION_H

#include <QtSql/QSqlDatabase>
#include "qdebug.h"

class Connection{
public:

    QSqlDatabase mydb;

    // POUR FERMER LA BASE DE DONNEES

    void connClose() {
        mydb.close();
        mydb.removeDatabase(QSqlDatabase::defaultConnection);
    }

    // pour ouvrir la base de données

    bool connOpen() {
        mydb = QSqlDatabase::addDatabase("QSQLITE");
        mydb.setDatabaseName("D://PROJET-GESTION-DES-STOCKS-le-vrai5.0--main//PROJET-GESTION-DES-STOCKS-le-vrai5.0--main//gestion_stock.db");

        if (!mydb.open())
        {
            qDebug() << "Échec d'ouverture de la base de données" << mydb.databaseName();
            return false;
        }
        else
        {
            qDebug() << "Connexion réussie à la base de données" << mydb.databaseName();
            return true;
        }
    }
};

#endif // CONNECTION_H
