#include <iostream>
#include <iomanip>
#include <cmath>
#include "cph2.h"
#include "utils.h"

using namespace std;


CPH2::CPH2(){
    dx = dy = 5;
    i = 0;
    stop = true;
    grid_initialized = false;
    enemy_pos_grid = pVector(3);
    team_pos_grid = pVector(3);
    pGrid = dMatrix(28, vector<double>(36, 0.0));
    tGrid = dMatrix(28, vector<double>(36, 0));
    cout<<"\n\nAMBIENTE CRIADO!\n";
}

double CPH2::iterator(){
    double erro = 0;
    double top, botton, left, right;
    double newPotencial, oldPotencial;
    int i,j;
    for(i=0;i<28;i++)
    {
        for(j=0;j<36;j++)
        {
            if(get_occupancy(i,j))
            {
                oldPotencial = get_potential(i,j);
                top = get_neighborhood(i,j,0);
                botton = get_neighborhood(i,j,1);
                left = get_neighborhood(i,j,2);
                right = get_neighborhood(i,j,3);
                newPotencial = (top+botton+left+right)/4;
                newPotencial = newPotencial + 0.8*(newPotencial-oldPotencial);
                erro = erro + pow((newPotencial - oldPotencial),2);
                set_potential(i,j,newPotencial);
            }
        }
    }
    //cout << "Erro" << " " << erro << endl;
    return erro;
}

double CPH2::get_neighborhood(int i, int j, int k){
   double top,botton,left,right;
   if(i==0)
   {
       top=1;
       botton=get_potential(i+1,j);
   }
   else
   {
       if(i==27)
       {
           botton=1;
           top=get_potential(i-1,j);
       }
       else
       {
           top=get_potential(i-1,j);
           botton=get_potential(i+1,j);
       }
   }
   if(j==0)
   {
       left=1;
       right=get_potential(i,j+1);
   }
   else
   {
       if(j==35)
       {
           right=1;
           left=get_potential(i,j-1);
       }
       else
       {
           left=get_potential(i,j-1);
           right=get_potential(i,j+1);
       }
   }
   switch (k) {
   case 0:
       return top;
       break;
   case 1:
       return botton;
       break;
   case 2:
       return left;
       break;
   case 3:
       return right;
       break;
   default:
       break;
   }
}

int CPH2::get_occupancy(int i, int j){
    if(get_potential(i,j)==1 || get_potential(i,j)==0)
    {
        return 0;
    }
    else
    {
        return 1;
    }
}

double CPH2::get_potential(int i, int j){
    return pGrid[i][j];
}

void CPH2::set_direction(){
    int i,j;

    for(i=0;i<28;i++)
    {
        for(j=0;j<36;j++)
        {
            //cout << "i = " << i << " j = " << j << endl;
            if(get_occupancy(i,j)==1)
                tGrid[i][j] = -atan2(get_neighborhood(i,j,0)-get_neighborhood(i,j,1),get_neighborhood(i,j,2)-get_neighborhood(i,j,3))*180/PI;
            else
                tGrid[i][j] = get_potential(i,j)*1000;
        }
    }
}

double CPH2::get_direction(Point grid){
    return this->tGrid[grid.y][grid.x];
}

void CPH2::set_potential(int i, int j, double aux){
    pGrid[i][j]= aux;
}

void CPH2::init_grid(){
    int i,j;

    for(i=0;i<28;i++)
    {
        for(j=0;j<36;j++)
        {
            if ( i == 0 || i == 27 || j == 0 || j == 1 || j == 2 || j == 33 || j == 34 || j == 35 )
            {
                if ( j == 1 || j == 2 || j == 33 || j == 34 )
                {
                    if (i > 9 && i < 18)
                        pGrid[i][j] = 0.9;
                    else
                        pGrid[i][j] = 1.0;
                }
                else
                    pGrid[i][j] = 1.0;
            }
            else
                pGrid[i][j] = 0.9;
        }
    }
}

void CPH2::print_grid(){
    cout<<"\n\n\nGrid:\n\n";;
    int i,j;
    for(i=0;i<28;i++)
    {
        for(j=0;j<36;j++)
        {
            cout.precision(4);
            cout<<fixed;
            cout<<pGrid[i][j]<<setw(7);
        }
        cout<<"\n\n";
    }
    cout<<"\n\n\nGrid de direcoes:\n\n";
    for(i=0;i<28;i++)
    {
        for(j=0;j<36;j++)
        {
            cout<<tGrid[i][j]<<"\t";
        }
        cout<<"\n\n";
    }
    cout<<"\n\n\n";
}

Point CPH2::convert_C_to_G(Point2d coord){
    Point i;

    coord.x = int(coord.x) + 5;
    coord.y = int(coord.y) + 5;

    if(coord.x / dx != 35){
        i.x = coord.x / dx;
    }else{
        i.x = coord.x / dx - 1;
    }

    if(coord.y / dy != 27){
        i.y = coord.y / dy;
    }else{
        i.y = coord.y / dy - 1;
    }
    return i;
}

void CPH2::run(){
    //if(!grid_initialized){
        init_grid();

        grid_initialized = true;
    //}

    /*cout << "Inimigo 1: " << "em x: "<< enemy_pos[0].x << " em y: "<< enemy_pos[0].y << endl;
    cout << "Inimigo 2: " << "em x: "<< enemy_pos[1].x << " em y: "<< enemy_pos[1].y << endl;
    cout << "Inimigo 3: " << "em x: "<< enemy_pos[2].x << " em y: "<< enemy_pos[2].y << endl;
    cout << "Leona: " << "em x: "<< team_pos[0].x << " em y: "<< team_pos[0].y << endl;
    cout << "Gandalf: " << "em x: "<< team_pos[1].x << " em y: "<< team_pos[1].y << endl;
    cout << "Presto: " << "em x: "<< team_pos[2].x << " em y: "<< team_pos[2].y << endl;*/

    for(i = 0; i < 3; ++i){
        if(enemy_pos[i].x > 0 && enemy_pos[i].y > 0){
            enemy_pos_grid[i] = convert_C_to_G(enemy_pos[i]);
            //cout<<"Inimigo "<<enemy_pos_grid[i].x<<" "<<enemy_pos_grid[i].y<<endl;
            set_potential(enemy_pos_grid[i].y, enemy_pos_grid[i].x, 1);
        }else{
            //tratar posição dos inimigos aqui
        }

        if(team_pos[i].x > 0 && team_pos[i].y > 0){
            team_pos_grid[i] = convert_C_to_G(team_pos[i]);
            //cout<<"Amigo "<<team_pos_grid[i].x<<" "<<team_pos_grid[i].y<<endl;
        }else{
            //tratar posição dos miguxos aqui
        }
    }

    //cout<<"Bola "<<ball_pos_grid.x<<" "<<ball_pos_grid.y<<endl;

    if(ball_pos.x > 0 && ball_pos.y > 0){
        if ((ball_pos.y > centroid_def.y+50 || ball_pos.y < centroid_def.y-50) && fabs(ball_pos.x - centroid_def.x) < 70){
            cout << 1 << endl;
            ball_pos_grid = convert_C_to_G(ball_pos);
            if (ball_pos_grid.x > 0 && ball_pos_grid.y > 0){
                cout << 2 << endl;
                cout<<"Meta: "<<ball_pos_grid.x<<" "<<ball_pos_grid.y<<endl;
                set_potential(ball_pos_grid.y, ball_pos_grid.x, 0);
            }
        }else{
            Point2d vec_ball_def = centroid_def - ball_pos;
            double aux = (0.45/150)*euclidean_dist(centroid_def,ball_pos);

            Point2d meta = ball_pos + vec_ball_def*aux;
            //cout << "Meta: " << endl;
            //cout << " x: " << meta.x << " y: " << meta.y << endl;
            if(meta.x > 0 && meta.y > 0){
                 Point meta_pos_grid = convert_C_to_G(meta);
                 //cout<<"Bola "<<ball_pos_grid.x<<" "<<ball_pos_grid.y<<endl;
                 if (meta_pos_grid.x > 0 && meta_pos_grid.y > 0)
                     set_potential(meta_pos_grid.y, meta_pos_grid.x, 0);
            }else{
                //tratar a meta aqui
            }
            meta_aux = meta;
        }
    }else{
        //tratar a bola aqui
    }

    /*if(ball_pos.x > 0 && ball_pos.y > 0){
        ball_pos_grid = convert_C_to_G(ball_pos);
        //cout<<"Bola "<<ball_pos_grid.x<<" "<<ball_pos_grid.y<<endl;
        set_potential(ball_pos_grid.y, ball_pos_grid.x, 0);
    }else{
        //tratar a bola aqui
    }*/


    while(iterator()>1E-6);
    set_direction();

   //print_grid();

    flag_finish_CPH2 = true;
}

bool CPH2::get_flag_finish(){
    return this->flag_finish_CPH2;
}

void CPH2::zera_flag_finish(){
    flag_finish_CPH2 = false;
}


void CPH2::set_enemy_pos(p2dVector enemy_pos){
    this->enemy_pos = enemy_pos;
}

void CPH2::set_team_pos(p2dVector team_pos){
    this->team_pos = team_pos;
}

void CPH2::set_ball_pos(Point2d ball_pos){
    this->ball_pos = ball_pos;
}

void CPH2::set_centroid_atk(Point2d centroid_atk){
    this->centroid_atk = centroid_atk;
}

void CPH2::set_centroid_def(Point2d centroid_def){
    this->centroid_def = centroid_def;
}

Point2d CPH2::get_meta_aux(){
    return this->meta_aux;
}

bool CPH2::isStopped() const
{
    return this->stop;
}

void CPH2::Play(){
    if(isStopped())
        stop = false;
   start();
}

void CPH2::Stop(){
    stop = true;
}

bool CPH2::is_running(){
    return isRunning();
}

void CPH2::msleep(int ms){
    /*struct timespec ts = {ms / 1000, (ms % 1000) * 1000 * 1000};
    nanosleep(&ts, NULL);*/
}

CPH2::~CPH2(){

}


