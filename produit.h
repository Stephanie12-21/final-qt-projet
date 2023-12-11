// produit.h
#ifndef PRODUIT_H
#define PRODUIT_H

#include <QString>

class Produit
{
public:
    Produit() {}
    Produit(const QString &id, const QString &nom);
    QString getId() const;
    QString getNom() const;
    QString toString() const;

private:
    QString id;
    QString nom;
};

#endif // PRODUIT_H
