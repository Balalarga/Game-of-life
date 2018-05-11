#ifndef GAME_H
#define GAME_H

#include <QObject>
#include <QWidget>
#include <QTimer>

class Game : public QWidget
{
    Q_OBJECT
public:
    explicit Game(int n = 50, QWidget *parent = nullptr);

private:
    QVector<QVector<int>> cell;
    QTimer* timer;
    int mode;

protected:
    void paintEvent(QPaintEvent* e);
    void paintGrid(QPainter& p);
    void paintCells(QPainter& p);
    void mousePressEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
    int alive(int x, int y)const;
    void randWalls();

signals:

public slots:
    void start();
    void action();
    void stop();
    void clear();
    void changeSpeed(int speed);
    void changeSize(int size);
    void changeMode(QString mode);
};

#endif // GAME_H
