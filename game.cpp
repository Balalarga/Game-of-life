#include "game.h"
#include <QPainter>
#include <QMouseEvent>
#include <unistd.h>
#include <cstdlib>
#include <ctime>

enum {ALIVE = 1, DEAD = 0, WALL = -1,
      ClassicMode, WallMode, CyclesMode, SecWallMode};

Game::Game(int n, QWidget *parent) : QWidget(parent),
    cell(n, QVector<int>(n,DEAD)),
    timer(new QTimer(this)),
    mode(ClassicMode)
{
    connect(timer, SIGNAL(timeout()), this, SLOT(action()));
    srand(time(0));
    timer->setInterval(50);
}
//Buttons
void Game::start(){
    if(!timer->isActive())
        timer->start();
}
void Game::stop(){
    timer->stop();
}
void Game::clear(){ 
    stop();
    for(int x = 0; x < cell.size(); x++){
        for(int y = 0; y < cell[x].size(); y++)
            cell[x][y] = DEAD;
    }
    if(mode == WallMode || mode == SecWallMode)
        randWalls();
    update();
}

//Main logic
void Game::action(){
    int nextCell[cell.size()][cell[0].size()];
    int notChanged = 0;
    for(int i=0; i < cell.size(); i++)
        for(int j=0; j < cell[0].size(); j++){
            nextCell[i][j] = alive(i, j);
            if(nextCell[i][j] == cell[i][j])
                notChanged++;
        }
    if(notChanged == cell.size()*cell[0].size()) {
        stop();
        return;
    }
    for(int x=0; x < cell.size(); x++){
        for(int y=0; y < cell[x].size(); y++){
            cell[x][y] = nextCell[x][y];
        }
    }
    update();
}
int Game::alive(int x, int y)const{
    if(cell[x][y] == WALL)
        return WALL;
    int count = 0;
    int dx[]{x+1,x-1,x  ,x  ,x+1,x-1,x-1,x+1};
    int dy[]{y  ,y  ,y+1,y-1,y-1,y+1,y-1,y+1};
    for(int i = 0; i < 8; i++){
        if(dx[i] == cell.size())
            dx[i] = 0;
        if(dx[i] < 0)
            dx[i] = cell.size()-1;
        if(dy[i] == cell[0].size())
            dy[i] = 0;
        if(dy[i] < 0)
            dy[i] = cell[0].size()-1;
        if(cell[dx[i]][dy[i]] == WALL){
            continue;
        }else
            count += cell[dx[i]][dy[i]];
    }
    if(mode == ClassicMode || mode == WallMode)
        if (((cell[x][y] == ALIVE) && (count == 2)) || (count == 3))
            return ALIVE;
    if(mode == CyclesMode)
        if(count%2)
            return ALIVE;
    if(mode == SecWallMode)
        if(count == 2 || count == 3 && cell[x][y] == ALIVE)
            return ALIVE;
    return DEAD;
}
void Game::randWalls(){
    for(int i = 0; i < cell.size()*cell[0].size()/9;i++){
        int x = rand()%cell.size();
        int y = rand()%cell[0].size();
        cell[x][y] = WALL;
    }
}

//Changing params
void Game::changeSpeed(int speed){
    timer->setInterval(speed);
}
void Game::changeSize(int size)
{
    int temp[cell.size()][cell[0].size()];
    for(int i = 0; i < cell.size(); i++)
        for(int j = 0; j < cell[0].size(); j++)
            temp[i][j] = cell[i][j];
    cell = QVector<QVector<int>>(size, QVector<int>(size, 0));
    if(size > cell.size())
        for(int i = 0; i < cell.size(); i++)
            for(int j = 0; j < cell[0].size(); j++)
                cell[i][j] = temp[i][j];
    else for(int i = 0; i < size; i++)
        for(int j = 0; j < size; j++)
            cell[i][j] = temp[i][j];
    update();
}
void Game::changeMode(QString mode)
{
    emit stop();
    if(this->mode == WallMode)
        for(int i = 0; i < cell.size(); i++)
            for(int j = 0; j < cell[0].size(); j++)
                if(cell[i][j] == -1)
                    cell[i][j] = 0;
    if(mode == "Classic")
        this->mode = ClassicMode;
    else if(mode == "Cycles")
        this->mode = CyclesMode;
    else if(mode == "Walls"){
        this->mode = WallMode;
        randWalls();
    }else if(mode == "Walls 2"){
        this->mode = SecWallMode;
        randWalls();
    }
    update();
}

//Graphics
void Game::paintEvent(QPaintEvent *e)
{
    QPainter p(this);
    paintGrid(p);
    paintCells(p);
}
void Game::mouseMoveEvent(QMouseEvent *e){
    double xn = double(width())/cell.size();
    double yn = double(height())/cell[0].size();
    int x = e->x()/xn;
    int y = e->y()/yn;
    if(x >= cell.size() || y >= cell.size() ||
            y < 0            || x < 0         )
        return;

    if(!cell[x][y])
        cell[x][y] = 1;
    update();
}
void Game::mousePressEvent(QMouseEvent *e){
    double xn = double(width())/cell.size();
    double yn = double(height())/cell[0].size();
    int x = e->x()/xn;
    int y = e->y()/yn;
    if(e->button() == Qt::LeftButton)
        cell[x][y] = cell[x][y]==0 ? 1: 0;
    else if(e->button() == Qt::RightButton && mode == WallMode)
        cell[x][y] = -1;
    update();
}
void Game::paintGrid(QPainter &p){
    double xn = double(width())/cell.size();
    double yn = double(height())/cell[0].size();
    QColor color = QColor(255, 255, 255, 30);
    p.setPen(color);
    for(double k = xn; k <= width(); k += xn)
        p.drawLine(k, 0, k, height());
    for(double k = yn; k <= height(); k += yn)
        p.drawLine(0, k, width(), k);
}
void Game::paintCells(QPainter &p)
{
    double xn = double(width())/cell.size();
    double yn = double(height())/cell[0].size();
    for(int x = 0; x < cell.size(); x++)
        for(int y = 0; y < cell[x].size();y++){
            QColor color((x+y)*(250/2/cell.size()),255,255-(x+y)*(250/2/cell.size()));
            if(cell[x][y] == DEAD){
                color = QColor(0,0,0,100);
            }
            if(cell[x][y] == WALL)
                color = QColor(Qt::darkGray);
            qreal left = (qreal)(xn*(x+1)-xn);
            qreal top  = (qreal)(yn*(y+1)-yn);
            QRectF rec(left, top, (qreal)xn, (qreal)yn);
            p.fillRect(rec, QBrush(color));
        }
}
