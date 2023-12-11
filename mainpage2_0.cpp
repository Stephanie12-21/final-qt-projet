#include "mainpage2_0.h"
#include "ui_mainpage2_0.h"
#include "mainwindow.h"
#include"ui_mainwindow.h"
#include <QMessageBox>
#include "ajout_produit.h"
#include "ajoutbe.h"
#include "ajoutbs.h"
#include "Connection.h"
#include "produit.h"
#include "modif_produit.h"
#include "modif_bonentre.h"
#include "modif_bonsortie.h"
#include "qdebug.h"
#include <QSqlQuery>
#include <QMessageBox>
#include <QSqlError>
#include <QStandardItemModel>
#include <QFont>
#include <QTextDocument>
#include <QPrinter>
#include  <QDesktopServices>

MainPage2_0::MainPage2_0(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainPage2_0)
{
    ui->setupUi(this);
    this->setFixedSize(this->size());


        Qt::WindowFlags flags = this->windowFlags();
        flags &= ~Qt::WindowMaximizeButtonHint;
        this->setWindowFlags(flags);
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
                            recupIdBilan = produit.getId();
                            ui->DesignComboBox->addItem(produit.toString());

                        }

                }

        }
    else
        {
            qDebug() << "Database connection failed.";
        }

    TotalNombreProduit();
    setupChart(ui->tableview, 0, 0);
    connect(this, SIGNAL(dataUpdated()), this, SLOT(updateChart()));
    ValeursTotales();
    TotalNombreBE();
    TotalNombreBS();
    afficheProduit(ui->produitTable);
    afficheBonEntree(ui->bonEntreeTable);
    afficheBonSortie(ui->bonSortieTable);
    afficheBilan(ui->MouvementStockTable);
    ui->modif_produitbtn->setDisabled(true);
    ui->sup_produitbtn->setDisabled(true);

    int currentIndex = ui->tabWidget->currentIndex();
    QString num = QString::number(currentIndex);
    qDebug() << num;

}


MainPage2_0::~MainPage2_0()
{
    delete ui;
    delete m_db;
}

QColor MainPage2_0::generateRandomColor()
{
    int red = (qrand() + QDateTime::currentMSecsSinceEpoch()) % 256;
    int green = (qrand() + QDateTime::currentMSecsSinceEpoch() + 100) % 256;
    int blue = (qrand() + QDateTime::currentMSecsSinceEpoch() + 200) % 256;

    return QColor(red, green, blue);
}

void MainPage2_0::setupChart(QTableView *tableview, int row, int column)
{
    QSqlQuery query("SELECT design, stock FROM produit");

    QBarSet *set = new QBarSet("Quantité");

    while (query.next()) {

        *set << query.value(1).toInt();
    }

    QBarSeries *series = new QBarSeries();
    series->append(set);

    QChart *chart = new QChart();
    QFont titleFont;
    titleFont.setPointSize(16);

    chart->setTitleFont(titleFont);
    chart->addSeries(series);
    chart->setTitle("Présentation de l'état des stocks");
    chart->setAnimationOptions(QChart::AllAnimations);
    chart->setPreferredSize(1400, 490);

    QStringList categories;

    QSqlQuery categoryQuery("SELECT design FROM produit");
    while (categoryQuery.next()) {
        categories << categoryQuery.value(0).toString();
    }

    QBarCategoryAxis *axisX = new QBarCategoryAxis();
    axisX->append(categories);
    chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);

    QValueAxis *axisY = new QValueAxis();
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);

    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignBottom);

    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->setFixedSize(1400, 490);

    QStandardItemModel *model = new QStandardItemModel();
    tableview->setModel(model);


    tableview->setEditTriggers(QAbstractItemView::NoEditTriggers);

    model->setItem(row, column, new QStandardItem());
    tableview->setIndexWidget(model->index(row, column), chartView);

    emit dataUpdated();
}

void MainPage2_0::updateChart(QTableView *tableview, int row, int column)
{

    QSqlQuery query("SELECT design, stock FROM produit");

    QBarSet *set = new QBarSet("Quantité");

    while (query.next()) {

        *set << query.value(1).toInt();
    }

    QBarSeries *series = new QBarSeries();
    series->append(set);

    QChart *chart = new QChart();
    QFont titleFont;
    titleFont.setPointSize(16);

    chart->setTitleFont(titleFont);
    chart->addSeries(series);
    chart->setTitle("Présentation de l'état des stocks");
    chart->setAnimationOptions(QChart::AllAnimations);
    chart->setPreferredSize(1400, 490);

    QStringList categories;

    QSqlQuery categoryQuery("SELECT design FROM produit");
    while (categoryQuery.next()) {
        categories << categoryQuery.value(0).toString();
    }

    QBarCategoryAxis *axisX = new QBarCategoryAxis();
    axisX->append(categories);
    chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);

    QValueAxis *axisY = new QValueAxis();
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);

    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignBottom);

    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->setFixedSize(1400, 490);

    QStandardItemModel *model = new QStandardItemModel();
    tableview->setModel(model);


    tableview->setEditTriggers(QAbstractItemView::NoEditTriggers);

    model->setItem(row, column, new QStandardItem());
    tableview->setIndexWidget(model->index(row, column), chartView);
}

void MainPage2_0::on_refreshBtn_clicked()
{
   updateChart(ui->tableview, 0, 0);

}


void MainPage2_0::on_pushButton_3_clicked()
{

    QMessageBox retour;
   retour.setWindowTitle("Question");
   retour.setText("Voulez-vous vraiment vous déconnecter?");
   retour.setStandardButtons(QMessageBox::Yes | QMessageBox::No);


   retour.setButtonText(QMessageBox::Yes, "Oui");
   retour.setButtonText(QMessageBox::No, "Non");


    int result = retour.exec();


    if (result == QMessageBox::Yes)
    {
      this->close();
      MainWindow *mainwindow = new MainWindow();
      mainwindow->show();

    }
    else
    {
       retour.close();
    }
}


//pour la partie PRODUITS

void MainPage2_0::on_ajoutProduitBtn_clicked()
{
    ajout_produit *Aj = new ajout_produit();
    Aj->setModal(true);
    Aj->exec();
    afficheProduit(ui->produitTable);
    TotalNombreProduit();
    updateDesignComboBox();
     emit dataUpdated();

    chartTableView = ui->produitTable;


}

void MainPage2_0::afficheProduit (QTableView *table)
{
    Connection m_db;
    if (!m_db.connOpen())
    {

        qDebug() << "Failed to open database";
    }
    m_db.connOpen();

    model= new QSqlQueryModel();
    model->setQuery("SELECT * FROM produit");
    model->setHeaderData(0,Qt::Horizontal, tr("Numéro du produit"));
    model->setHeaderData(1,Qt::Horizontal, tr("Désignation du produit"));
    model->setHeaderData(2,Qt::Horizontal, tr("Stock"));
    model->setHeaderData(3,Qt::Horizontal, tr("Date d'ajout"));
    model->setHeaderData(4,Qt::Horizontal, tr("Date de modification"));
    table->setStyleSheet("QHeaderView::section {background-color:#76D2D1; font-size:11pt;}");
    table->setModel(model);
    QFont font;
           font.setPointSize(10);
           ui->produitTable->setFont(font);

    ui->modif_produitbtn->setDisabled(true);
    ui->sup_produitbtn->setDisabled(true);



}

void MainPage2_0::on_produitTable_clicked(const QModelIndex &index)
{
    ui->modif_produitbtn->setDisabled(false);
    ui->sup_produitbtn->setDisabled(false);


    int rowSelected= index.row();
    numProduit=ui->produitTable->model()->data(ui->produitTable->model()->index(rowSelected,0)).toString();
    design=ui->produitTable->model()->data(ui->produitTable->model()->index(rowSelected,1)).toString();
    stock=ui->produitTable->model()->data(ui->produitTable->model()->index(rowSelected,2)).toString();



    qDebug() << numProduit+" "+design+" "+stock;


}

void MainPage2_0::on_sup_produitbtn_clicked()
{

    QMessageBox::StandardButton reply;
    QMessageBox confirmationBox(QMessageBox::Question,
                                "Confirmation",
                                "Voulez-vous vraiment supprimer cet élément?",
                                QMessageBox::Yes | QMessageBox::No,
                                this);

    confirmationBox.setButtonText(QMessageBox::Yes, "Oui");
    confirmationBox.setButtonText(QMessageBox::No, "Non");

    reply = static_cast<QMessageBox::StandardButton>(confirmationBox.exec());

    if (reply == QMessageBox::No)
    {
        return;
    }
    Connection m_db;
    if (!m_db.connOpen())
    {
        qDebug() << "Failed to open database";
        return;
    }

    m_db.connOpen();

    QSqlQuery req;
    QString requete = "DELETE FROM produit WHERE numPro=:numero";
    req.prepare(requete);
    req.bindValue(":numero", numProduit);

    if (!req.exec())
    {
        QMessageBox::critical(this, "Erreur", "Échec de l'exécution de la requête DELETE: " + req.lastError().text());
        return;
    }
    QMessageBox::information(this, "Succès", "L'élément avec le numéro " + numProduit + " a été supprimé avec succès.");


    afficheProduit(ui->produitTable);
    TotalNombreProduit();
}

void MainPage2_0::on_modif_produitbtn_clicked()
{
    modif_produit *modif = new modif_produit ();
    modif->setModal(true);
    modif->numPro = modif->numero(this->numProduit);
    modif->getInfo(this->design, this->stock);
    modif->exec();
    afficheProduit(ui->produitTable);
    TotalNombreProduit();
}

void MainPage2_0::on_PDFProduitbtn_clicked()
{
    QMessageBox::information(this, "Export en PDF", "L'export en PDF a commencé.");

    QSqlQuery query("SELECT * FROM produit");
    query.exec();

    QTextDocument doc;

    QString htmlContent = "<h1>Liste des produits enregistrés</h1>"
                          "<table style=\"border-collapse: collapse; width:100%;\" border=\"1\">"
                          "<tr>"
                          "<th style=\"border: 1px solid black; width:50%; text-align: center;\">Numéro du produit</th>"
                          "<th style=\"border: 1px solid black; width:50%; text-align: center;\">Désignation du produit</th>"
                          "<th style=\"border: 1px solid black; width:50%; text-align: center;\">Stock actuel du produit</th>"
                          "</tr>";

    while (query.next()) {
        QString numPro = query.value(0).toString();
        QString design = query.value(1).toString();
        QString stock = query.value(2).toString();

        htmlContent += "<tr>"
                       "<td style=\"border: 1px solid black; width:50%; text-align: center;\">" + numPro + "</td>"
                       "<td style=\"border: 1px solid black; width:50%; text-align: center;\">" + design + "</td>"
                       "<td style=\"border: 1px solid black; width:50%; text-align: center;\">" + stock + "</td>"
                       "</tr>";
    }

    htmlContent += "</table>";

    doc.setHtml(htmlContent);

    QPrinter printer;
    printer.setOutputFileName("D://Liste des produits enregistrés.pdf");
    printer.setOutputFormat(QPrinter::PdfFormat);

    doc.print(&printer);

    QMessageBox::information(this, "Export en PDF", "L'export en PDF est terminé.");


        QMessageBox openPdfMessageBox;
        openPdfMessageBox.setText("Voulez-vous ouvrir le PDF récemment exporté ?");


        openPdfMessageBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        openPdfMessageBox.setDefaultButton(QMessageBox::Yes);
        openPdfMessageBox.setButtonText(QMessageBox::Yes, "Oui");
        openPdfMessageBox.setButtonText(QMessageBox::No, "Non");

        int userChoice = openPdfMessageBox.exec();

        if (userChoice == QMessageBox::Yes) {

            QDesktopServices::openUrl(QUrl::fromLocalFile("D://Liste des produits enregistrés.pdf"));
        }
    }

void MainPage2_0::TotalNombreProduit()
{
    Connection m_db;
    if(!m_db.connOpen())
        {
          qDebug() << "Failed to open database";
        }
    else
    {
        m_db.connOpen();
    }

      QSqlQuery query;
      if(query.exec("SELECT SUM(stock) FROM produit "))
          {
                if (query.next())
                    {
                         int totalProduit = query.value(0).toInt();
                         ui->produitCardNumber->display(totalProduit);
                     }
           }
      else
      {
        qDebug() << "Error calculating total entries:" << query.lastError().text();
      }

}

void MainPage2_0::on_searchLineProEdit_textChanged(const QString &arg1)
{
    qDebug() << arg1;

    Connection m_db;
    if (!m_db.connOpen())
    {
        qDebug() << "Failed to open database";
        return;
    }


    QSqlQuery requete;


    requete.prepare("SELECT * FROM produit WHERE design LIKE :searchTerm");
    requete.bindValue(":searchTerm", "%" + arg1 + "%");


    if (requete.exec())
    {

        QStandardItemModel *tableau = new QStandardItemModel(0, 5);
        tableau->setHorizontalHeaderLabels(QStringList() << "Numéro du produit" << "Désignation du produit"
                                                         << "Stock" << "Date d'ajout" << "Date de modification");


        int row = 0;
        while (requete.next())
        {
            QString numProduit = requete.value("numPro").toString();
            QString design = requete.value("design").toString();
            int stock = requete.value("stock").toInt();
            QString dateAjout = requete.value("date_colonne").toString();
            QString dateModification = requete.value("dateModifPro").toString();

            tableau->setItem(row, 0, new QStandardItem(numProduit));
            tableau->setItem(row, 1, new QStandardItem(design));
            tableau->setItem(row, 2, new QStandardItem(QString::number(stock)));
            tableau->setItem(row, 3, new QStandardItem(dateAjout));
            tableau->setItem(row, 4, new QStandardItem(dateModification));

            row++;
        }


        ui->produitTable->setModel(tableau);
        ui->produitTable->setStyleSheet("QHeaderView::section {background-color:#76D2D1; font-size:11pt;}");
        QFont font;
        font.setPointSize(10);
        ui->produitTable->setFont(font);


        if (row == 0)
        {
            QMessageBox::warning(this, "Information", "Aucun résultat trouvé pour : " + arg1);
        }
    }
    else
    {

        qDebug() << "Erreur lors de l'exécution de la requête :" << requete.lastError().text();
    }
}



//pour la partie Bon d'entrée

void MainPage2_0::afficheBonEntree(QTableView *bonEntreeTable)
{
    ui->modifBEbtn->setDisabled(true);
    ui->supBEbtn->setDisabled(true);

    Connection m_db;
    if (!m_db.connOpen())
    {
        qDebug() << "Failed to open database";
        return;
    }

    m_db.connOpen();

    modele = new QSqlQueryModel();
    modele->setQuery("SELECT CASE WHEN bon.numBonEntre < 10 THEN 'BE-00' || bon.numBonEntre WHEN bon.numBonEntre >= 10 AND bon.numBonEntre < 100 THEN 'BE-0' || bon.numBonEntre ELSE 'BE-' || bon.numBonEntre END AS formattedNumBonEntre, bon.qtEntre, bon.dateEntre,pro.numPro, pro.design, bon.dateBEModifPro FROM bonEntree bon INNER JOIN produit pro ON bon.numPro = pro.numPro;");

    modele->setHeaderData(0, Qt::Horizontal, tr("Numéro de bon d'entrée"));
    modele->setHeaderData(1, Qt::Horizontal, tr("Quantité d'entrée"));
    modele->setHeaderData(2, Qt::Horizontal, tr("Date d'entrée"));
    modele->setHeaderData(3, Qt::Horizontal, tr("Numéro du produit"));
    modele->setHeaderData(4, Qt::Horizontal, tr("Désignation du produit"));
    modele->setHeaderData(5, Qt::Horizontal, tr("Date de modification"));



    bonEntreeTable->setStyleSheet("QHeaderView::section {background-color:#76D2D1;font-size:11pt;}");
    bonEntreeTable->setModel(modele);

    QFont font;
    font.setPointSize(10);
    bonEntreeTable->setFont(font);
}

void MainPage2_0::on_AjoutBEbtn_clicked()
{
    ajoutBE *ajBE = new ajoutBE();
    ajBE->setModal(true);
    ajBE->exec();
    afficheBonEntree(ui->bonEntreeTable);
    afficheProduit(ui->produitTable);
    afficheBilan(ui->MouvementStockTable);
    ValeursTotales();
    TotalNombreBS();
    TotalNombreBE();
    TotalNombreProduit();
}

void MainPage2_0::on_modifBEbtn_clicked()
{
    modif_bonEntre *modifBE = new modif_bonEntre();
    modifBE-> setModal(true);
    modifBE->getInfo(this->qttBE);
    modifBE->exec();
    afficheBonEntree(ui->bonEntreeTable);
    TotalNombreProduit();
    TotalNombreBE();
    afficheBilan(ui->MouvementStockTable);
    afficheBonSortie(ui->bonSortieTable);
    afficheProduit(ui->produitTable);

}

void MainPage2_0::on_bonEntreeTable_clicked(const QModelIndex &index)
{
    ui->modifBEbtn->setDisabled(false);
    ui->supBEbtn->setDisabled(false);

    int rowSelected = index.row();
    QString numBE = ui->bonEntreeTable->model()->data(ui->bonEntreeTable->model()->index(rowSelected, 0)).toString();
    qttBE = ui->bonEntreeTable->model()->data(ui->bonEntreeTable->model()->index(rowSelected, 1)).toString();
    numProEntr = ui->bonEntreeTable->model()->data(ui->bonEntreeTable->model()->index(rowSelected, 3)).toString();
    designationBE = ui->bonEntreeTable->model()->data(ui->bonEntreeTable->model()->index(rowSelected, 4)).toString();

    QStringList parts = numBE.split("-");
    if (parts.size() == 2) {
        numBonEntree = QString::number(parts[1].toInt());
    }

    qDebug() << "Ligne sélectionnée : " << numBonEntree << " " << designationBE << " " << qttBE << " " << numProEntr;

}

void MainPage2_0::on_supBEbtn_clicked()
{
    // Demander une confirmation avant la suppression
    QMessageBox::StandardButton reply;
    QMessageBox confirmationBox(QMessageBox::Question,
                                "Confirmation",
                                "Voulez-vous vraiment supprimer cet élément?",
                                QMessageBox::Yes | QMessageBox::No,
                                this);

    confirmationBox.setButtonText(QMessageBox::Yes, "Oui");
    confirmationBox.setButtonText(QMessageBox::No, "Non");

    reply = static_cast<QMessageBox::StandardButton>(confirmationBox.exec());

    if (reply == QMessageBox::No) {
        // L'utilisateur a cliqué sur "Non", annuler la suppression
        return;
    }

    // L'utilisateur a cliqué sur "Oui", procéder à la suppression
    Connection m_db;
    if (!m_db.connOpen())
    {
        QMessageBox::critical(this, "Erreur", "Échec de l'ouverture de la base de données");
        return;
    }

    m_db.connOpen();

    QSqlQuery reqs;
    QString requete = "DELETE FROM bonEntree WHERE numBonEntre=:numeroBE";
    reqs.prepare(requete);
    reqs.bindValue(":numeroBE", numBonEntree);

    if (!reqs.exec())
    {
        QMessageBox::critical(this, "Erreur", "Échec de l'exécution de la requête DELETE: " + reqs.lastError().text());
        return;
    }
    QMessageBox::information(this, "Succès", "L'élément avec le numéro " + numBonEntree + " a été supprimé avec succès.");

    // Mise à jour du stock après suppression
    QSqlQuery updateQuery;
    QString updateRequete = "UPDATE produit SET stock = stock - :qtEntre WHERE numPro = :recupId;";
    updateQuery.prepare(updateRequete);
    updateQuery.bindValue(":qtEntre", qttBE);
    updateQuery.bindValue(":recupId", numProEntr.toInt());
    if (!updateQuery.exec())
    {
        QMessageBox::critical(this, "Erreur", "Échec de l'exécution de la requête UPDATE: " + updateQuery.lastError().text());
        return;
    }
    QMessageBox::information(this, "Succès", "La mise à jour du stock du produit a réussi.");

    // Mise à jour des affichages dans les tables
    afficheBonEntree(ui->bonEntreeTable);
    afficheBonSortie(ui->bonSortieTable);
    afficheProduit(ui->produitTable);
    afficheBilan(ui->MouvementStockTable);
    ValeursTotales();
    TotalNombreBE();
    TotalNombreBS();
    TotalNombreProduit();
}

void MainPage2_0::TotalNombreBE()
{
    Connection m_db;
    if(!m_db.connOpen())
        {
          qDebug() << "Failed to open database";
        }
    else
    {
        m_db.connOpen();
    }

      QSqlQuery query;
      if(query.exec("SELECT SUM(qtEntre) FROM bonEntree "))
          {
                if (query.next())
                    {
                         int totalENTREE = query.value(0).toInt();
                         ui->BECardNumber->display(totalENTREE);
                     }
           }
      else
      {
        qDebug() << "Error calculating total entries:" << query.lastError().text();
      }


}


//pour les bons de sortie

void MainPage2_0::on_ajoutBSbtn_clicked()
{
    ajoutbs *ajBS= new ajoutbs();
    ajBS->setModal(true);
    ajBS->exec();

    afficheBonSortie(ui->bonSortieTable);
    afficheProduit(ui->produitTable);
    afficheBilan(ui->MouvementStockTable);
    ValeursTotales();
    TotalNombreBS();
    TotalNombreBE();
    TotalNombreProduit();
}

void MainPage2_0::afficheBonSortie(QTableView *bonSortieTable)
{
    ui->modifBSbtn->setDisabled(true);
    ui->supBSbtn->setDisabled(true);
    Connection m_db;
    if (!m_db.connOpen())
    {
        qDebug() << "Failed to open database";
    }
    m_db.connOpen();

    modeles = new QSqlQueryModel();
    modeles->setQuery("SELECT CASE WHEN bon.numBonSortie < 10 THEN 'BS-00' || bon.numBonSortie WHEN bon.numBonSortie >= 10 AND bon.numBonSortie < 100 THEN 'BS-0' || bon.numBonSortie ELSE 'BS-' || bon.numBonSortie END AS formattedNumBonSortie, bon.qtSortie, bon.dateSortie, pro.numPro, pro.design, bon.dateBSModifPro  FROM bonSortie bon  INNER JOIN produit pro ON bon.numPro = pro.numPro;");

    modeles->setHeaderData(0, Qt::Horizontal, tr("Numéro de bon de sortie"));
    modeles->setHeaderData(1, Qt::Horizontal, tr("Quantité de sortie"));
    modeles->setHeaderData(2, Qt::Horizontal, tr("Date de sortie"));
    modeles->setHeaderData(3, Qt::Horizontal, tr("Numéro du produit"));
    modeles->setHeaderData(4, Qt::Horizontal, tr("Désignation du produit"));

    bonSortieTable->setModel(modeles);

    // Appliquer le style pour l'entête du tableau
    bonSortieTable->horizontalHeader()->setStyleSheet("QHeaderView::section { background-color:#76D2D1 ; font-size: 11pt; }");


    // Appliquer le style pour le contenu du tableau
    QFont font;
    font.setPointSize(10);
    bonSortieTable->setFont(font);
}

void MainPage2_0::on_bonSortieTable_clicked(const QModelIndex &index)
{
    ui->modifBSbtn->setDisabled(false);
    ui->supBSbtn->setDisabled(false);


    int rowSelected = index.row();
    QString numBS = ui->bonSortieTable->model()->data(ui->bonSortieTable->model()->index(rowSelected, 0)).toString();
    qttBS = ui->bonSortieTable->model()->data(ui->bonSortieTable->model()->index(rowSelected, 1)).toString();
    numProBs = ui->bonSortieTable->model()->data(ui->bonSortieTable->model()->index(rowSelected, 3)).toString();
    designationBS = ui->bonSortieTable->model()->data(ui->bonSortieTable->model()->index(rowSelected, 4)).toString();


    QStringList parts = numBS.split("-");
    if (parts.size() == 2) {
        numBonSortie = QString::number(parts[1].toInt());
    }

    qDebug() << "Ligne sélectionnée : " << numBonSortie << " " << designationBS << " " << qttBS << " " << numProBs;

}

void MainPage2_0::on_supBSbtn_clicked()
{
    QMessageBox::StandardButton reply;
    QMessageBox confirmationBox(QMessageBox::Question,
                                "Confirmation",
                                "Voulez-vous vraiment supprimer cet élément?",
                                QMessageBox::Yes | QMessageBox::No,
                                this);

    confirmationBox.setButtonText(QMessageBox::Yes, "Oui");
    confirmationBox.setButtonText(QMessageBox::No, "Non");

    reply = static_cast<QMessageBox::StandardButton>(confirmationBox.exec());

    if (reply == QMessageBox::No) {
        // L'utilisateur a cliqué sur "Non", annuler la suppression
        return;
    }


    Connection m_db;
    if (!m_db.connOpen())
    {
        QMessageBox::critical(this, "Erreur", "Échec de l'ouverture de la base de données");
        return;
    }

    m_db.connOpen();

    QSqlQuery reqs;
    QString requete = "DELETE FROM bonSortie WHERE numBonSortie=:numeroBS";
    reqs.prepare(requete);
    reqs.bindValue(":numeroBS", numBonSortie);

    if (!reqs.exec())
    {
        QMessageBox::critical(this, "Erreur", "Échec de l'exécution de la requête DELETE: " + reqs.lastError().text());
        return;
    }
    QMessageBox::information(this, "Succès", "L'élément avec le numéro " + numBonSortie + " a été supprimé avec succès.");

    QSqlQuery updateQuery;
    QString updateRequete = "UPDATE produit SET stock = stock + :qtSortie WHERE numPro = :recupId;";
    updateQuery.prepare(updateRequete);
    updateQuery.bindValue(":qtSortie", qttBS);
    updateQuery.bindValue(":recupId", numProBs.toInt());
    if (!updateQuery.exec())
    {
        QMessageBox::critical(this, "Erreur", "Échec de l'exécution de la requête UPDATE: " + updateQuery.lastError().text());
        return;
    }
    QMessageBox::information(this, "Succès", "La mise à jour du stock du produit a réussi.");


    afficheBonEntree(ui->bonEntreeTable);
    afficheBonSortie(ui->bonSortieTable);
    afficheProduit(ui->produitTable);
    afficheBilan(ui->MouvementStockTable);
    ValeursTotales();
    TotalNombreBS();
    TotalNombreBE();
    TotalNombreProduit();
}

void MainPage2_0::on_modifBSbtn_clicked()
{
    modif_bonsortie *modBS = new modif_bonsortie();
    modBS->setModal(true);
    modBS->getInfo(this->qttBS);
    modBS->exec();
    afficheBonSortie(ui->bonSortieTable);
    afficheBilan(ui->MouvementStockTable);
    afficheBonEntree(ui->bonEntreeTable);
       TotalNombreProduit();
       TotalNombreBS();
       afficheBilan(ui->MouvementStockTable);
       afficheProduit(ui->produitTable);

}

void MainPage2_0::TotalNombreBS()
{
    Connection m_db;
    if(!m_db.connOpen())
        {
          qDebug() << "Failed to open database";
        }
    else
    {
        m_db.connOpen();
    }

      QSqlQuery query;
      if(query.exec("SELECT SUM(qtSortie) FROM bonSortie "))
          {
                if (query.next())
                    {
                         int totalSORTIE = query.value(0).toInt();
                         ui->BSCardNumber->display(totalSORTIE);
                     }
           }
      else
      {
        qDebug() << "Error calculating total entries:" << query.lastError().text();
      }


}


//pour la partie bilan
void MainPage2_0::on_DesignComboBox_activated(int index)
{
    Q_UNUSED(index);
    QString itemText = ui->DesignComboBox->currentText();
    QStringList parts = itemText.split(" - ");

    if (parts.size() == 2)
    {
        recupIdBilan = parts[0];
        QString nomPro = parts[1];


        QSqlQuery stockQuery;
        stockQuery.prepare("SELECT stock FROM produit WHERE numPro = :recupIdBilan");
        stockQuery.bindValue(":recupIdBilan", recupIdBilan);

        if (stockQuery.exec() && stockQuery.next())
        {
            int stockActuel = stockQuery.value(0).toInt();
            ui->StockCompteurEdit->display(stockActuel);

            qDebug() << "ID: " << recupIdBilan;
            qDebug() << "NomPro: " << nomPro;
            qDebug() << "Stock Actuel: " << stockActuel;
        }
        else
        {
            qDebug() << "Failed to retrieve stock information.";
        }
    }
}

void MainPage2_0::updateDesignComboBox()
{

    ui->DesignComboBox->clear();


    QSqlQuery query("SELECT numPro, design FROM produit");


    while (query.next())
    {
        QString numPro = query.value(0).toString();
        QString design = query.value(1).toString();
        ui->DesignComboBox->addItem(numPro + " - " + design);
    }
}

void MainPage2_0::on_DateFin_dateChanged(const QDate &date1)
{
    qDebug() << "Date de fin sélectionnée : " << date1.toString("dd/MM/yyyy");
}

void MainPage2_0::on_DateDebut_dateChanged(const QDate &date2)
{
       qDebug() << "Date de début sélectionnée : " << date2.toString("dd/MM/yyyy");
}

void MainPage2_0::afficheBilan(QTableView *MouvementStockTable)
{

    Connection m_db;

    if (!m_db.connOpen())
    {
        qDebug() << "Failed to open database";
    }
    else
    {

        models = new QSqlQueryModel();
        models->setQuery("SELECT 'BE-' || numBonEntre AS numBon, numPro, qtEntre AS entree, 0 AS sortie, dateEntre AS date "
                            "FROM bonEntree "
                            "UNION ALL "
                            "SELECT 'BS-' || numBonSortie AS numBon, numPro, 0 AS entree, qtSortie AS sortie, dateSortie AS date "
                            "FROM bonSortie "
                            "ORDER BY date");

        models->setHeaderData(0, Qt::Horizontal, tr("Numéro de bon"));
        models->setHeaderData(1, Qt::Horizontal, tr("Numéro de produit"));
        models->setHeaderData(2, Qt::Horizontal, tr("Entrée"));
        models->setHeaderData(3, Qt::Horizontal, tr("Sortie"));
        models->setHeaderData(4, Qt::Horizontal, tr("Date"));

        MouvementStockTable->setStyleSheet("QHeaderView::section {background-color:#76D2D1; font-size:11pt;}");
        MouvementStockTable->horizontalHeader()->sectionResizeMode(QHeaderView::ResizeToContents);
        MouvementStockTable->setModel(models);

        QFont font;
        font.setPointSize(10);
        MouvementStockTable->setFont(font);
    }
 ValeursTotales();
}

void MainPage2_0::ValeursTotales()
{
    Connection m_db;
    if(!m_db.connOpen())
        {
          qDebug() << "Failed to open database";
        }
    else
    {
        m_db.connOpen();
    }

      QSqlQuery query;
      if(query.exec("SELECT SUM(qtEntre) FROM bonEntree"))
          {
                if (query.next())
                    {
                         int totalEntrees = query.value(0).toInt();
                         ui->TotalEntree->display(totalEntrees);
                     }
           }
      else
      {
        qDebug() << "Error calculating total entries:" << query.lastError().text();
      }
      if (query.exec("SELECT SUM(qtSortie) FROM bonSortie"))
                {
                  if (query.next())
                  {
                      int totalSorties = query.value(0).toInt();
                      ui->TotalSortie->display(totalSorties);
                  }
              }
            else
              {
                  qDebug() << "Error calculating total exits:" << query.lastError().text();
              }
}

void MainPage2_0::updateTotalValues()
{

    int totalEntree = 0;
    int totalSortie = 0;

    for (int row = 0; row < Modely->rowCount(); ++row)
    {
        QModelIndex indexEntree = Modely->index(row, 2);
        QModelIndex indexSortie = Modely->index(row, 3);

        totalEntree += indexEntree.data().toInt();
        totalSortie += indexSortie.data().toInt();
    }

    ui->TotalEntree->display(totalEntree);
    ui->TotalSortie->display(totalSortie);
}

void MainPage2_0::on_cancelBilanBtn_clicked()
{
    afficheBilan(ui->MouvementStockTable);
}

void MainPage2_0::on_rechercheButton_clicked()
{
    Connection m_db;
    if (!m_db.connOpen())
    {
        qDebug() << "Failed to open database";
    }
    m_db.connOpen();

    QString numPro = ui->DesignComboBox->currentText().split(" - ").at(0);
//    QString selectedYear = ui->YearComboBox->currentText();
//    QString selectedMonth = ui->MonthsComboBox->currentText();
    QString dateDebut = ui->DateDebut->date().toString("yyyy-MM-dd");
    QString dateFin = ui->DateFin->date().toString("yyyy-MM-dd");

    // Build the SQL query with filters based on the selected values
    QString queryStr =  "SELECT 'BE-' || numBonEntre AS numBon, numPro, qtEntre AS entree, 0 AS sortie, dateEntre AS date "
                                          "FROM bonEntree "
                                          "WHERE numPro = :numPro "
                                          "AND dateEntre BETWEEN :dateDebut AND :dateFin "
                                          "UNION ALL "
                                          "SELECT 'BS-' || numBonSortie AS numBon,numPro, 0 AS entree, qtSortie AS sortie, dateSortie AS date "
                                          "FROM bonSortie "
                                          "WHERE numPro = :numPro "
                                          "AND dateSortie BETWEEN :dateDebut AND :dateFin "
                                          "ORDER BY date";


    QSqlQuery query;
    query.prepare(queryStr);
    query.bindValue(":numPro", numPro);
//    query.bindValue(":selectedYear", selectedYear);
//    query.bindValue(":selectedMonth", selectedMonth);
    query.bindValue(":dateDebut", dateDebut);
    query.bindValue(":dateFin", dateFin);

     qDebug()<<" :numPro"<< numPro;
     qDebug()<<":dateDebut"<<dateDebut ;
     qDebug()<<":dateFin"<< dateFin;

     if (query.exec())
     {
         Modely = new QSqlQueryModel();
         Modely->setQuery(query);
         Modely->setHeaderData(0, Qt::Horizontal, tr("Numéro de bon"));
         Modely->setHeaderData(1, Qt::Horizontal, tr("Numéro de produit"));
         Modely->setHeaderData(2, Qt::Horizontal, tr("Entrée"));
         Modely->setHeaderData(3, Qt::Horizontal, tr("Sortie"));
         Modely->setHeaderData(4, Qt::Horizontal, tr("Date"));

         ui->MouvementStockTable->setStyleSheet("QHeaderView::section {background-color:#76D2D1; font-size:11pt;}");
         ui->MouvementStockTable->horizontalHeader()->sectionResizeMode(QHeaderView::ResizeToContents);
         ui->MouvementStockTable->setModel(Modely);

         QFont font;
         font.setPointSize(10);
         ui->MouvementStockTable->setFont(font);
     }
     else
     {
         qDebug() << "Error executing query:" << query.lastError().text();
     }
updateTotalValues();
}

