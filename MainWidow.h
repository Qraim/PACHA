//
// Created by qraim on 03/04/23.
//

#ifndef PACHA_MAINWIDOW_H
#define PACHA_MAINWIDOW_H

#include  <QApplication>
#include <QPushButton>
#include <QComboBox>
#include <QLineEdit>
#include <QHBoxLayout>
#include <QWidget>
#include <QLabel>
#include <QKeyEvent>
#include <QShortcut>


class MainWindow : public QWidget {
Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);

private:

    QComboBox comboBox1;
    QComboBox comboBox2;

    QLineEdit debit;
    QLineEdit vitesse;
    QLineEdit longueur;
    QLineEdit denivele;

    QHBoxLayout layout;
    QLabel resultLabel;
    QLineEdit readOnlyLine;
    QLineEdit V;

    QPushButton switchButton;
    QPushButton button;

    void updateSecondComboBox(int index);
    float calculer();
    std::map<float,float> gettableau();
    void onButtonClicked();
    void updateButtonState();
    void onSwitchButtonClicked();
    void onReturnPressed();

    float calculdebitvitesse();
    float calcullongueurdeniv();

    void Gettuyau(float diametre);

    void keyPressEvent(QKeyEvent *event);

    void focusPreviousInput();

    void focusNextInput();

};


#endif //PACHA_MAINWIDOW_H
