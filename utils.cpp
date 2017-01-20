#include <iostream>
#include <vector>
#include <cmath>
#include <Eigen/Dense>
#include "utils.h"

using namespace std;
using namespace Eigen;

double euclidean_dist(Point p, Point q)
{
    Point diff = p - q;

    return sqrt(diff.x*diff.x - diff.y*diff.y);
}

double angle_two_points(Point p, Point q)
{
    double theta = p.ddot(q)/(sqrt(p.ddot(p)) * sqrt(q.ddot(q)));

    return acos(theta) * 180.0 / PI;
}

pair<Matrix3d, Vector3d> kalman_filter(Vector3d pos_cam, Vector2d v_w, Vector3d last_pos , double dt, Matrix3d last_P){
    double v = v_w[0], w = v_w[1], ds = v * dt, dw = w * dt;
    double b = 0.055; //Distância entre eixos do carrinho 'CHECK'
    double dsr = (ds + b)/(2 * dw), dsl = (ds - b)/(2 * dw);
    double kr = 9, kl = 9;
    Vector3d prediction, pos_predicted, pos_current;
    Matrix2d covar;
    Matrix3d P = last_P, C, R, I, fp, P_predicted, K;
    MatrixXd fdrl(3, 2);
    pair<Matrix3d, Vector3d> res;

    C = MatrixXd::Identity(3,3);
    I = MatrixXd::Identity(3,3);

    R << 9.23765359168e-07,0                ,0            ,
         0                ,7.93803166352e-07,0            ,
         0                ,0                ,6.14071137584;

    //Kalman Extended
    prediction << ds*cos((last_pos(2,0)+dw/2)*PI/180),
                  ds*sin((last_pos(2,0)+dw/2)*PI/180),
                  dw;

    pos_predicted = last_pos + prediction;

    covar << kr*abs(dsr),0          ,
             0          ,kl*abs(dsl);

    fp << 1, 0, -ds*sin((last_pos(2,0)+dw/2)*PI/180),
          0, 1, ds*cos((last_pos(2,0)+dw/2)*PI/180) ,
          0, 0, 1                                   ;

    fdrl << 1/2*cos((last_pos(2,0)+dw/2)*PI/180)-ds/(2*b)*sin((last_pos(2,0)+dw/2)*PI/180),  1/2*cos((last_pos(2,0)+dw/2)*PI/180)+ds/(2*b)*sin((last_pos(2,0)+dw/2)*PI/180),
            1/2*sin((last_pos(2,0)+dw/2)*PI/180)+ds/(2*b)*cos((last_pos(2,0)+dw/2)*PI/180),  1/2*sin((last_pos(2,0)+dw/2)*PI/180)-ds/(2*b)*cos((last_pos(2,0)+dw/2)*PI/180),
            1/b                                                                           , -1/b                                                                           ;

    P_predicted = (fp * P * fp.transpose()) + (fdrl * covar * fdrl.transpose());
    K = P_predicted * C.transpose() * ( C * P_predicted * C.transpose() + R).inverse();
    pos_current = pos_predicted + K * (pos_cam - C * pos_predicted);
    P = (I - K * C) * P_predicted;

    res.first = P;
    res.second = pos_current;

    return res;
}
