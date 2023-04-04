#include "MainWidow.h"
#include <map>
#include <cmath>
#include <iostream>
#include <QShortcut>

MainWindow::MainWindow(QWidget *parent) : QWidget(parent), button("Calculer", this) {
    setFixedSize(500, 310);

    comboBox1.setFixedWidth(80);
    comboBox2.setFixedWidth(80);
    debit.setFixedWidth(60);
    vitesse.setFixedWidth(60);
    longueur.setFixedWidth(60);
    denivele.setFixedWidth(60);
    resultLabel.setFixedWidth(20);
    readOnlyLine.setFixedWidth(400);
    V.setFixedWidth(400);
    switchButton.setFixedWidth(80);


    comboBox1.addItem("PEBD");
    comboBox1.addItem("PEHD");
    comboBox1.addItem("PVC");


    QGridLayout *gridLayout = new QGridLayout;

    // Matériau et Pression
    gridLayout->addWidget(new QLabel("Matériau : "), 0, 0);
    gridLayout->addWidget(&comboBox1, 0, 1);
    gridLayout->addWidget(new QLabel("Pression : "), 1, 0);
    gridLayout->addWidget(&comboBox2, 1, 1);

    // Débit et Vitesse
    gridLayout->addWidget(new QLabel("Débit : "), 2, 0);
    gridLayout->addWidget(&debit, 2, 1);
    gridLayout->addWidget(new QLabel("m³/h"), 2, 2);
    gridLayout->addWidget(new QLabel("Vitesse : "), 3, 0);
    gridLayout->addWidget(&vitesse, 3, 1);
    gridLayout->addWidget(new QLabel("m/s"), 3, 2);

    // Longueur et Dénivelé
    gridLayout->addWidget(new QLabel("Longueur : "), 4, 0);
    gridLayout->addWidget(&longueur, 4, 1);
    gridLayout->addWidget(new QLabel("m"), 4, 2);
    gridLayout->addWidget(new QLabel("Dénivelé : "), 5, 0);
    gridLayout->addWidget(&denivele, 5, 1);
    gridLayout->addWidget(new QLabel("mm"), 5, 2);

    // Bouton Changer en haut à droite
    switchButton.setText("Changer");
    switchButton.setFixedWidth(80);
    gridLayout->addWidget(&switchButton, 0, 3);

    // Bouton Calculer au milieu en bas
    button.setFixedWidth(80);
    button.setEnabled(false);
    gridLayout->addWidget(&button, 6, 1);

    // Résultat sous le bouton Calculer
    gridLayout->addWidget(new QLabel("Résultat (en mm):"), 7, 0);
    gridLayout->addWidget(&resultLabel, 7, 1);

    readOnlyLine.setReadOnly(true);
    readOnlyLine.setFixedWidth(400);
    gridLayout->addWidget(&readOnlyLine, 8, 0, 1, 4);

    V.setReadOnly(true);
    V.setFixedWidth(400);
    gridLayout->addWidget(&V, 9, 0, 1 ,4);


    setLayout(gridLayout);

    button.setEnabled(false);

    V.setReadOnly(true);
    V.setVisible(false);

    // Set visibility of the new widgets to false initially
    longueur.setVisible(false);
    denivele.setVisible(false);

    connect(&comboBox1, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MainWindow::updateSecondComboBox);

    connect(&button, &QPushButton::clicked, this, &MainWindow::onButtonClicked);

    connect(&debit, &QLineEdit::returnPressed, this, &MainWindow::onReturnPressed);
    connect(&longueur, &QLineEdit::returnPressed, this, &MainWindow::onReturnPressed);
    connect(&vitesse, &QLineEdit::returnPressed, this, &MainWindow::onReturnPressed);
    connect(&denivele, &QLineEdit::returnPressed, this, &MainWindow::onReturnPressed);

    connect(&debit, &QLineEdit::textChanged, this, &MainWindow::updateButtonState);
    connect(&vitesse, &QLineEdit::textChanged, this, &MainWindow::updateButtonState);
    connect(&longueur, &QLineEdit::textChanged, this, &MainWindow::updateButtonState);
    connect(&denivele, &QLineEdit::textChanged, this, &MainWindow::updateButtonState);
    // Connect the switch button
    connect(&switchButton, &QPushButton::clicked, this, &MainWindow::onSwitchButtonClicked);

    updateSecondComboBox(comboBox1.currentIndex());
    updateButtonState();

}

void MainWindow::onSwitchButtonClicked() {
    bool isVisible = longueur.isVisible();

    vitesse.setVisible(isVisible);
    longueur.setVisible(!isVisible);
    denivele.setVisible(!isVisible);

    debit.setFocus();

    updateButtonState();
}

void MainWindow::updateSecondComboBox(int index) {
    comboBox2.clear(); // On efface le contenu pour le remplacer

    switch (index) {
        case 0: // PEBD
            comboBox2.addItem("6");
            break;
        case 1: // PEHD
            comboBox2.addItem("6");
            comboBox2.addItem("10");
            comboBox2.addItem("16");
            break;
        case 2: // PVC
            comboBox2.addItem("8");
            comboBox2.addItem("10");
            comboBox2.addItem("16");
            break;
        default:
            break;
    }
}

float MainWindow::calculer(){

    bool isVisible = longueur.isVisible();

    if(!isVisible){
        V.setVisible(false);
        return calculdebitvitesse();
    } else {
        return calcullongueurdeniv();
    }

}
float MainWindow::calcullongueurdeniv(){

    float k = 831743.11;
    float a=1.75;
    float b =-4.75;

    float debits = (debit.text().toFloat()*1000)/3600;
    float deniveles = denivele.text().toFloat();
    float longueurs = longueur.text().toFloat();

    float vitesses = (debits/3600) / ((pow((deniveles/2000),2))*M_PI);
    float diametre = k*pow(debits,a)*pow(deniveles,b)*longueurs;

    V.setVisible(true);

    std::string str="";
    if(vitesses > 2){
        V.setStyleSheet("color: red;  background-color: white");
        str = "La vitesse est trop élevé ( " + std::to_string(vitesses) + " m/s)";
    } else {
        str = "La vitesse sera de " + std::to_string(vitesses) + " m/s";
    }

    V.setText(QString::fromStdString(str));

    Gettuyau(diametre);

    return diametre;

}

float MainWindow::calculdebitvitesse(){

    int debits = debit.text().toInt();
    int vitesses = vitesse.text().toInt(); // Récupération des variables

    float diametre = sqrt((4*debits)/(M_PI*vitesses))*1000; // formule pour le calcul du diametre

    Gettuyau(diametre);

    return diametre;

}

std::map<float, float> MainWindow::gettableau() { // Générateur du tableau en fonction des entrées utilisateur avec ( diametre exterieur, diametre interieur )
    QString str1 = comboBox1.currentText();
    QString str2 = comboBox2.currentText();
    std::map<float,float> tableau;
    if (str1 == "PEBD") {
        tableau = {
                {16, 12},
                {20, 15.4},
                {25, 19.4},
                {32,24.8}
        };
    } else if (str1 == "PEHD") {
        if(str2=="6"){
            tableau = {
                    {25, 21},
                    {32, 28},
                    {40, 35.2},
                    {50,44},
                    {63,55.4},
                    {75,66},
                    {90,79.2}
            };
        } else if (str2=="10"){
            tableau = {
                    {25, 20.4},
                    {32, 27.2},
                    {40, 34},
                    {50, 42.6},
                    {63, 53.6},
                    {75, 63.8},
                    {90, 76.6},
                    {110, 96.8},
                    {125, 110.2},
                    {140, 123.4},
                    {160, 141},
                    {200, 176.2},
                    {250, 220.2},
                    {315, 277.6},
                    {355, 312.8},
                    {400, 352.6},
                    {450, 396.6},
                    {500, 440.6},
                    {560, 493.6},
                    {630, 555.2},
                    {710, 625.8}
            };
        } else {
            tableau = {
                    {25, 19},
                    {32, 24.8},
                    {40, 31},
                    {50, 40.8},
                    {63, 51.4},
                    {75, 58.2},
                    {90, 73.6},
                    {110, 90},
                    {125, 102.2},
                    {140, 114.6},
                    {160, 130.8},
                    {200, 163.6},
                    {250, 204.6},
                    {315, 257.8},
                    {355, 290.6},
                    {400, 327.4}
            };
        }
    } else if (str1 == "PVC") {
        if(str2=="8"){
            tableau = {
                    {25, 21.2},
                    {32, 28.4},
                    {40, 36.4},
                    {50,46}
            };
        } else if (str2=="10"){
            tableau = {
                    {25, 25},
                    {32, 32},
                    {40, 40},
                    {50, 50},
                    {63, 63},
                    {75, 69},
                    {90, 83},
                    {110, 101.4},
                    {125, 116.2},
                    {140, 130.2},
                    {160, 148.8},
                    {200, 186.2},
                    {250, 232.6},
                    {315, 293.2}
            };

        } else {
            tableau = {
                    {90, 85},
                    {110, 105.2},
                    {125, 133.8},
                    {140, 153},
                    {160, 191.2},
                    {200, 239},
                    {250, 301.2},
                    {315, 293.2}
            };
        }
    }
    return tableau;
}

void MainWindow::onButtonClicked() {
    float result = calculer();
    resultLabel.setText(QString::number(result)); // Affichage du résultat
}

void MainWindow::updateButtonState() {
    bool debitHasText = !debit.text().isEmpty();
    bool vitesseHasText = !vitesse.text().isEmpty();
    bool longueurHasText = !longueur.text().isEmpty();
    bool deniveleHasText = !denivele.text().isEmpty();

    // Check which mode is active and enable the button accordingly
    if (debit.isVisible() && vitesse.isVisible()) {
        button.setEnabled(debitHasText && vitesseHasText);
    } else {
        button.setEnabled(longueurHasText && deniveleHasText && debitHasText);
    }
}


void MainWindow::onReturnPressed() {
    if (button.isEnabled()) {
        float result = calculer();
        resultLabel.setText(QString::number(result)); // Affichage du résultat
        return;
    }

    bool isVisible = longueur.isVisible();

    if(debit.hasFocus() && !isVisible){
        vitesse.setFocus();
        return;
    } else if (debit.hasFocus() && isVisible){
        longueur.setFocus();
        return;
    }

    if (longueur.hasFocus()) {
        denivele.setFocus();
    }
}

void MainWindow::Gettuyau(float diametre) {
    std::pair<int, float> closestDuo;
    float minValue = std::numeric_limits<float>::max();

    std::map<float,float> tableau = gettableau(); // récupération du tableau de diametres

    for (const auto& duo : tableau) { // ittération dans le tableau pour trouver le bon diametre
        if (duo.second > diametre && duo.second < minValue) {
            minValue = duo.second;
            closestDuo = duo;
            break; // On coupe la boucle quand on a trouvé
        }
    }

    // Maintenant, closestDuo contient le duo le plus proche et supérieur au diamètre
    std::string str = "";
    if(closestDuo.first==0 && closestDuo.second==0){
        str = "calcul invalide, pas de tuyau capable de supporter";
    } else {
        str = "Il faut un tuyau de " + comboBox1.currentText().toStdString() + " en " + comboBox2.currentText().toStdString() + " bar, de " + std::to_string(closestDuo.first) + "mm";
    }
    readOnlyLine.setText(QString::fromStdString(str));
}

void MainWindow::keyPressEvent(QKeyEvent *event) {
    if (event->key() == Qt::Key_Control) {
        switchButton.click();
    }
        // Gérer la combinaison de touches Shift + Up && Shift + Down
    else if (event->modifiers() & Qt::ShiftModifier && event->key() == Qt::Key_Up) {
        focusPreviousInput();
    } else if (event->modifiers() & Qt::ShiftModifier && event->key() == Qt::Key_Down) {
        focusNextInput();
    } else {
        QWidget::keyPressEvent(event);
    }
}

void MainWindow::focusPreviousInput() {

    bool isVisible = longueur.isVisible();

    if (denivele.hasFocus()) {
        longueur.setFocus();
        return;
    } else if (longueur.hasFocus() || vitesse.hasFocus()) {
        debit.setFocus();
        return;
    }
}

void MainWindow::focusNextInput() {

    bool isVisible = longueur.isVisible();

    if (debit.hasFocus() && !isVisible) {
        vitesse.setFocus();
        return;
    } else if (debit.hasFocus() && isVisible){
        longueur.setFocus();
        return;
    } else if(longueur.hasFocus()) {
        denivele.setFocus();
    }

}


