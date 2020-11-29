//#pragma once
#ifndef __LEG_H
#define __LEG_H

#include "utils.h"

class Leg
{
public:
	void init();
     void update(float _dt);
    //设定关节角度
    void setJointAng(float _c1,float _c2,float _c3,float _c4,float _c5);
    //设定关节弧度
    void setJointrRad(float _c1,float _c2,float _c3,float _c4,float _c5);
    //设定末端坐标
    void setPos(float _x,float _y,float _z,float _P,float _Y);
    //设定速度运动到目标位置 _use_t_long:移动使用时长/ms：
    void moveToPos(float _target_x,float _target_y,float _target_z,float _target_P,float _target_Y,float _use_t_long);
     LegAng getAng();
     //单位  度
     float get_c1(){return c1*180/pi;};
     float get_c2(){return c2*180/pi;};
     float get_c3(){return c3*180/pi;};
     float get_c4(){return c4*180/pi;};
     float get_c5(){return c5*180/pi;};
     float get_x(){return x;};
     float get_y(){return y;};
     float get_z(){return z;};
     float get_R(){return R;};
     float get_P(){return P;};
     float get_Y(){return Y;};

private:
    double c1, c2, c3, c4, c5;          //对应 弧度
	RPY rpy;                           //足底角度 正解解算值 
    LegAng ang;
    double x, y, z,R,P,Y;               //足底曲线坐标  设定值  m 弧度
    float c10, c20, c30,c40,c50;        //上电初始弧度

	//机构参数
	float L1, L2, L3,L4,L5;   //杆长
	DH dh1, dh2, dh3,dh4,dh5;   //DH参数
	TrMatrix tr1, tr2, tr3, tr4, tr5;//转化矩阵
	TrMatrix pos_tr;   //正解求得矩阵
   //正解  由求(x,y,z)
	void fk_5(float _c1, float _c2, float _c3, float _c4, float _c5);
	//逆解 由(x,y,z,P,Y)求(c1,c2,c3)
    bool ik_5(double _x, double _y, double _z, double _P, double _Y);


    float target_x, target_y,target_z,target_P,target_Y; //目标位置
    float start_x, start_y,start_z,start_P,start_Y;  //设定目标位置时的起始位置
	float use_t_long;    //运动到目标时长  ms
	float use_t;   //移动已经使用时长

	bool set_move_to_pos;
};
#endif
