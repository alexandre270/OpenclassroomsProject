#include "widget.h"
#include "ui_widget.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QtDebug>
#include <QVBoxLayout>
#include <QTableWidget>
#include <QComboBox>

Widget::Widget(QWidget *parent)
    : QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    // Connexion à la base de données
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL"); // Utilisation du driver MySQL
    db.setHostName("localhost");                          // Serveur MySQL (localhost par défaut)
    db.setDatabaseName("FOOT");                           // Nom de la base de données
    db.setUserName("etudiant");                           // Nom de l'utilisateur
    db.setPassword("Passciel");                           // Mot de passe de l'utilisateur

    // Vérification de la connexion
    if (!db.open()) {
        qDebug() << "Échec de la connexion à la base de données : " << db.lastError().text();
        return; // Si la connexion échoue, on quitte la fonction
    }

    // Requête SQL pour récupérer les données de la table "jeu"
    QSqlQuery query("SELECT * FROM jeu");

    // Vérifier si la requête a réussi
    if (!query.exec()) {
        qDebug() << "Erreur lors de l'exécution de la requête : " << query.lastError().text();
        db.close();
        return;
    }

    // Création du QTableWidget
    tableWidget = new QTableWidget(this);
    tableWidget->setColumnCount(4);  // 4 colonnes : ID, Nom, Club, Note
    tableWidget->setHorizontalHeaderLabels({"ID", "Nom", "Club", "Note"});

    int row = 0; // Variable pour indexer les lignes du tableau

    // Parcourir les résultats de la requête et remplir le tableau
    while (query.next()) {
        QString ID = query.value(0).toString();         // Colonne 0 : ID
        QString Nom = query.value(1).toString();        // Colonne 1 : Nom
        QString Club = query.value(2).toString();       // Colonne 2 : Club

        // Ajouter une nouvelle ligne dans le tableau
        tableWidget->insertRow(row);

        // Ajouter les données dans les colonnes ID, Nom et Club
        tableWidget->setItem(row, 0, new QTableWidgetItem(ID));
        tableWidget->setItem(row, 1, new QTableWidgetItem(Nom));
        tableWidget->setItem(row, 2, new QTableWidgetItem(Club));

        // Ajouter un QComboBox dans la colonne "Note" (index 3)
        QComboBox *comboBox = new QComboBox();
        for (int i = 1; i <= 5; ++i) {
            comboBox->addItem(QString::number(i));  // Ajouter les options 1 à 5
        }
        comboBox->setCurrentIndex(0);  // Sélectionner par défaut la note 1
        tableWidget->setCellWidget(row, 3, comboBox);

        // Passer à la ligne suivante
        row++;
    }

    // Ajouter le QTableWidget au layout
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(tableWidget);
    setLayout(layout);

    db.close(); // Fermer la connexion à la base de données
}

Widget::~Widget()
{
    delete ui;
    delete tableWidget;  // Libérer la mémoire de tableWidget
}
