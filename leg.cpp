#include "leg.h"

void Leg::init()
{
	L1 = 0.5; L2 = 0.7; L3 = 0.8; L4 = 0.7; L5 = 0.5;
	dh1 = { 0,L1,0,pi / 2 }; dh2 = { 0,0,L2,0 }; dh3 = { 0,0,L3,0 };
	dh4 = { 0,0,L4,pi/2 }; dh5 = { 0,0,L5,0 };

    c10=c20= c30=c40=c50=0;
    fk_5(c10,c20,c30,c40,c50);
}

void Leg::update(float _dt)
{
    if(set_move_to_pos)
    {
        if(use_t>=use_t_long)  //到达目标
            set_move_to_pos = false;
        use_t+=_dt;
        setPos(Lerp(start_x,target_x,use_t/use_t_long) , Lerp(start_y,target_y,use_t/use_t_long),Lerp(start_z,target_z,use_t/use_t_long),
               Lerp(start_P,target_P,use_t/use_t_long), Lerp(start_Y,target_Y,use_t/use_t_long) );
    }
}

 void Leg::moveToPos(float _target_x,float _target_y,float _target_z,float _target_P,float _target_Y,float _use_t_long)
 {

     set_move_to_pos = true;
     target_x = _target_x; target_y = _target_y;target_z = _target_z; target_P = _target_P;target_Y = _target_Y;
     use_t_long = _use_t_long;
     use_t = 0;
     start_x = x; start_y = y;start_z = z; start_P = P; start_Y = Y;
 }

//设定关节角度
void Leg::setJointAng(float _c1,float _c2,float _c3,float _c4,float _c5)
{
    setJointrRad(_c1*pi/180,_c2*pi/180,_c3*pi/180,_c4*pi/180,_c5*pi/180);
}
//设定关节弧度
void Leg::setJointrRad(float _c1,float _c2,float _c3,float _c4,float _c5)
{
    fk_5(_c1,_c2,_c3,_c4,_c5);
    c1=_c1;c2=_c2;c3=_c3;c4=_c4;c5=_c5;
}
//设定末端坐标
void Leg::setPos(float _x,float _y,float _z,float _P,float _Y)
{
    if(ik_5(_x,_y,_z,_P,_Y))
    {
        x=_x;y=_y;z=_z; P=_P; Y=_Y;
    }
}

LegAng Leg::getAng()
{
    ang.c1 = c1*180/pi;
    ang.c2 = c2*180/pi;
    ang.c3 = c3*180/pi;
    ang.c4 = c4*180/pi;
    ang.c5 = c5*180/pi;
    return  ang;
}

void Leg::fk_5(float _c1, float _c2, float _c3, float _c4, float _c5)
{
	dh1.theta = _c1;  dh2.theta = _c2;  dh3.theta = _c3; dh4.theta = _c4;  dh5.theta = _c5;
	Dh2Tr(&dh1, &tr1); Dh2Tr(&dh2, &tr2); Dh2Tr(&dh3, &tr3);  Dh2Tr(&dh4, &tr4); Dh2Tr(&dh5, &tr5);
	//pos_tr = TrxTr(tr1, tr2);
	//printfTr(&pos_tr);
	pos_tr = TrxTr( TrxTr( TrxTr( TrxTr( tr1, tr2), tr3) , tr4 ) , tr5);
	printfTr(&pos_tr);
	x = pos_tr.tr[0][3]; y = pos_tr.tr[1][3]; z = pos_tr.tr[2][3];
	Tr2Rpy(&pos_tr, &rpy);
}

bool Leg::ik_5(double _x, double _y, double _z , double _P ,double _Y)
{
    double _c1, _c2, _c3,_c4,_c5;
    _c1 = atan((_y - L5 * cos(_P)*sin(_Y)) / (_x - L5 * cos(_P)*cos(_Y)));
    _c5 = asin((_x*sin(_c1) - _y * cos(_c1)) / L5);

    R = asin(sin(_P)*sin(_c5) / cos(_P) / cos(_c5));

    double sin234 = -sin(_P) / cos(_c5);
    double cos234 = -cos(_P)*cos(R);
    double m1 = _z - L1 + L5 * sin(_P) - L4 * sin234;
    double n1 = _x * cos(_c1) - L5 * (cos(_P)*cos(_Y)*cos(_c1) + cos(_P)*sin(_Y)*sin(_c1)) + _y * sin(_c1) - L4 * cos234;

	double m2 = 2 * m1*L2;
	double n2 = 2 * n1*L2;
	double k = m1 *m1 + n1 * n1 + L2 *L2  - L3 *L3;

	double _det = 4 * n2 *n2 * k *k - 4 * (k *k - m2 * m2)*(m2 *m2 + n2 * n2);  //防止sqrt内小于0  可能会小于一点点
	if (_det < 0) _det = 0;
    _c2 = acos((2 * n2*k + sqrt(_det)) / (2 * (m2 *m2 + n2 *n2)));
    double _sin_val = ((m1 - L2 * sin(_c2)) / L3);   //防止asin内大于1或者小于-1  可能会大于一点点
	if (_sin_val > 1) _sin_val = 1;
	if (_sin_val < -1) _sin_val = -1;
    _c3 = asin(_sin_val) - _c2;
    _c4 = acos(-cos(_P)*cos(R)) - _c2 - _c3;

    if (isNan(_c1) || isNan(_c2) ||isNan(_c3) || isNan(_c4) || isNan(_c5))
       return false;

   c1 = _c1; c2 = _c2; c3 = _c3; c4 = _c4; c5 = _c5;
   return true;
}
