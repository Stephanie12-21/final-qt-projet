#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "ui_mainpage2_0.h"
#include "mainpage2_0.h"

#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setFixedSize(this->size());

        // Cacher le bouton d'agrandissement
        Qt::WindowFlags flags = this->windowFlags();
        flags &= ~Qt::WindowMaximizeButtonHint;
        this->setWindowFlags(flags);
}

MainWindow::~MainWindow()
{
    delete ui;
}

//pour la page de connexion
void MainWindow::on_loginBtn_clicked()
{
    QString username = ui->NameUserEdit->text();
    QString password = ui->PasswordEdit->text();
    if (username == "0-0" && password == "0-0")
    {
        this->hide();
        MainPage2_0 *mainpage2_0 = new MainPage2_0();
        mainpage2_0->show();
    }
    else
    {
       QMessageBox::information(this, "Attention", "Veuillez entrer des informations valides!!!!");
    }

}


void MainWindow::on_cancelLoginBtn_clicked()
{
    QMessageBox retour;
   retour.setWindowTitle("Question");
   retour.setText("Voulez-vous vraiment annuler cette opération?");
   retour.setStandardButtons(QMessageBox::Yes | QMessageBox::No);


   retour.setButtonText(QMessageBox::Yes, "Oui");
   retour.setButtonText(QMessageBox::No, "Non");


    int result = retour.exec();


    if (result == QMessageBox::Yes)
    {
       QApplication::quit();
    }
    else
    {
       retour.close();
    }

}




