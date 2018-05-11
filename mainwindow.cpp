#include "mainwindow.h"

MainWindow::MainWindow(int n,QWidget *parent) :
    QWidget  (parent),
    game       (new Game(n)),
    mainLout   (new QVBoxLayout(this)),
    menuLout   (new QVBoxLayout),
    buttnLout  (new QHBoxLayout),
    changeLout (new QHBoxLayout),
    startBtn   (new QPushButton("Start")),
    clearBtn   (new QPushButton("Clear")),
    stopBtn    (new QPushButton("Stop")),
    speedSlider(new QSlider(Qt::Horizontal)),
    sizeSpinBx (new QSpinBox),
    modeComboBx(new QComboBox)
{
    resize(620,600);
    speedSlider->setInvertedAppearance(true);
    speedSlider->setInvertedControls(true);
    speedSlider->setRange(20, 400);
    speedSlider->setValue(125);
    sizeSpinBx->setValue(n);
    modeComboBx->addItem("Classic");
    modeComboBx->addItem("Cycles");
    modeComboBx->addItem("Walls");
    modeComboBx->addItem("Walls 2");

    connect(startBtn, SIGNAL(clicked()), game, SLOT(start()));
    connect(stopBtn, SIGNAL(clicked()), game, SLOT(stop()));
    connect(clearBtn, SIGNAL(clicked()), game, SLOT(clear()));
    connect(speedSlider, SIGNAL(sliderMoved(int)), game, SLOT(changeSpeed(int)));
    connect(sizeSpinBx, SIGNAL(valueChanged(int)), game, SLOT(changeSize(int)));
    connect(modeComboBx, SIGNAL(activated(QString)), game, SLOT(changeMode(QString)));

    mainLout->addWidget(modeComboBx);
    mainLout->addWidget(game);
    mainLout->addLayout(menuLout);

    menuLout->addLayout(buttnLout);
    menuLout->addLayout(changeLout);

    buttnLout->addWidget(startBtn);
    buttnLout->addWidget(clearBtn);
    buttnLout->addWidget(stopBtn);

    changeLout->addWidget(speedSlider);
    changeLout->addWidget(sizeSpinBx);
    changeLout->setSpacing(50);
}

MainWindow::~MainWindow()
{
}
