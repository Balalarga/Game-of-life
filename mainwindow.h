#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "game.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSlider>
#include <QPushButton>
#include <QSpinBox>
#include <QComboBox>
#include <QLabel>

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    explicit MainWindow(int n = 40, QWidget *parent = 0);
    ~MainWindow();

private:
    Game* game;
    QPushButton*   startBtn;
    QPushButton*   clearBtn;
    QPushButton*    stopBtn;
    QSlider*    speedSlider;
    QSpinBox*    sizeSpinBx;
    QComboBox*  modeComboBx;
    QVBoxLayout*   mainLout;
    QVBoxLayout*   menuLout;
    QHBoxLayout*  buttnLout;
    QHBoxLayout* changeLout;
};

#endif // MAINWINDOW_H
