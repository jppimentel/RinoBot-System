#ifndef MOVER_H
#define MOVER_H


#include <QThread>
#include <vector>
#include "robot.h" //Robot class
#include "utils.h" //Utils library
#include "fuzzy.h"
#include "cph.h"
#include "cpo.h"
#include "cph2.h"
#include "cpo2.h"
#include "cpo3.h"
#include "serial.h"

struct Selec{
    CPH *cph;
    CPO *cpo;
    CPH2 *cph2;
    CPO2 *cpo2;
    CPO3 *cpo3;
};

class Mover: public QThread
{
    Q_OBJECT
private:
    pVector enemy_pos_grid; //posicao em grid dos inimigos
    pVector team_pos_grid; //posicao em grid do time
    p2dVector enemy_pos; //posicao em cm dos inimigos
    Point2d ball_pos; //posicao em cm da bola
    Point2d centroid_atk; //posicao em cm do centro da area de atk
    Point2d centroid_def; //posicao em cm do centro da area de def
    pVector def_area;
    pair<double, double> ball_vel;
    vector<pair<float, float> > vels;
    static Serial serial;

    bool stop, mover_initialized; //variavel de controle de thread
    bool team_chang;
    Selector selec_robot; //estrutura de selecao dos robos que vao entrar no fuzzy
    Selec selec_iterador;

protected:
    void msleep(int ms);
    void run(); //rotina que a thread executa
signals:
    void emitRobots(Selector);
public:
    Mover();
    double set_ang(double robot_angle, double angle, double w);
    pair<double, double> potDefense(double katt, double kw, Robot r, Point2d obj, bool correct);
    pair<double, double> defenderGK(Robot r);

    void init_mover();
    void calcula_velocidades(Robot *, CPH *, CPO *, CPH2 *, CPO2 *,CPO3 *, pair<float, float>*);
    void goleiro(Robot r, pair<float, float> *vels);
    double min_function(double, double);
    double max_function(double, double);
    double ajusta_angulo(double);
    void Play();
    bool is_running();
    void Stop();
    bool isStopped() const;
    void team_changed();
    void set_def_area(pVector def_area);
    void set_ball_vel(pair<double, double>);
    void set_to_select(Robot, Robot, Robot);
    void set_to_select_iterador(CPH *, CPO *,CPH2 *,CPO2 *, CPO3 *);
    void set_enemy_pos(p2dVector);
    void set_ball_pos(Point2d);
    void set_centroid_atk(Point2d);
    void set_centroid_def(Point2d);
    Point convert_C_to_G(Point2d);
    ~Mover();
};

#endif // MOVER_H