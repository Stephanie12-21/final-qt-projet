#ifndef MAINPAGE2_0_H
#define MAINPAGE2_0_H
#include <QMainWindow>
#include <QTableView>
#include <QSqlQueryModel>
#include <QString>
#include  <QLCDNumber>
#include "Connection.h"
#include <QVBoxLayout>
#include <QtCharts/QChartView>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QValueAxis>
#include <QStandardItemModel>
#include <QColor>
QT_CHARTS_USE_NAMESPACE

namespace Ui
{
class MainPage2_0;
}

class MainPage2_0 : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainPage2_0(QWidget *parent = nullptr);
    QString column1;
    QString column2;
    QString column3;
    QString column4;
    QString nomTable;
    QString recupIdBilan;



    void afficheProduit (QTableView *table);
    QSqlQueryModel *model;
    QString numProduit;
    QString design;
    QString stock;




    void afficheBonEntree(QTableView *bonEntreeTable);
    QSqlQueryModel *modele;
    QString numBonEntree;
    QString numProEntr;
    QString qttBE;
    QString designationBE;


    void afficheBonSortie (QTableView *bonSortieTable);
    QSqlQueryModel *modeles;
    QString numBonSortie;
    QString qttBS;
    QString numProBs;
    QString designationBS;

    void afficheBilan (QTableView *MouvementStockTable);
    QSqlQueryModel *models;
    QString numPro;


    QSqlQueryModel *Modely;

    QString onglet;

    QTableView *chartTableView;
    ~MainPage2_0();


signals:
    void dataUpdated();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_3_clicked();

    void on_ajoutProduitBtn_clicked();

    void on_produitTable_clicked(const QModelIndex &index);

    void on_sup_produitbtn_clicked();

    void on_modif_produitbtn_clicked();

    void on_ajoutBEbtn_clicked();

    void on_ajoutBSbtn_clicked();

    void on_searchEdit_textChanged(const QString &arg1);

    void on_tabWidget_currentChanged(int index);


    void on_modifBEbtn_clicked();

    void on_supBEbtn_clicked();

    void on_bonEntreeTable_clicked(const QModelIndex &index);

    void on_modifBSbtn_clicked();

    void on_bonSortieTable_activated(const QModelIndex &index);

    void on_supBSbtn_clicked();

    void on_bonSortieTable_clicked(const QModelIndex &index);

    void on_DesignComboBox_activated(int index);

    void on_DateFin_dateChanged(const QDate &date2);

    void on_DateDebut_dateChanged(const QDate &date1);

    void ValeursTotales();


    void updateDesignComboBox();

    void updateTotalValues();

    void on_rechercheButton_clicked();

    void on_cancelBilanBtn_clicked();


    void on_AjoutBEbtn_clicked();

    void on_PDFProduitbtn_clicked();

    void on_PDFBilanbtn_clicked();

    void TotalNombreProduit();

    void TotalNombreBE();

    void TotalNombreBS();

    void setupChart(QTableView *tableview, int row, int column);

     void updateChart(QTableView *tableview, int row, int column);

     void on_refreshBtn_clicked();

     void on_searchbtn_clicked();

     void on_searchbtn_2_clicked();

     void on_searchbtn_3_clicked();

     void on_searchLineProEdit_textChanged(const QString &arg1);

     void on_searchLineBEEdit_textChanged(const QString &arg1);

     void on_ProduitcomboboxBE_activated(int index);

     QColor generateRandomColor();

private:
    Ui::MainPage2_0 *ui;
     Connection *m_db;
};

#endif // MAINPAGE2_0_H
