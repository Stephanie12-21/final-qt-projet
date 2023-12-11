// produit.cpp
#include "produit.h"

Produit::Produit(const QString &id, const QString &nom)
    : id(id), nom(nom)
{
}

QString Produit::getId() const
{
    return id;
}

QString Produit::getNom() const
{
    return nom;
}
QString Produit::toString() const {
    return id + " - " + nom;
}

