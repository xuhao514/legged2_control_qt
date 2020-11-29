#include "walk_leg.h"

void WalkLegClass::init(Leg &_leg,float _x0,float _y0,float _L,float _H0,float _t_floor,float _tair_tfloor_scale,float _start_scale_pos,float _delayt_tfloor_scale,int _forward)
{
     MaxL=100,MinL=-100;   //最大最小步长
     v_leg=_L/_t_floor;

     leg=&_leg; over_flag=false; fixPointFlag=false;
     prep=false; walk_leg_num=0;
     t_floor=_t_floor;
     tair_tfloor_scale=_tair_tfloor_scale; t_air=t_floor*tair_tfloor_scale;
     start_scale_pos = _start_scale_pos;
     if(start_scale_pos<0.5)  start_tn= 2*start_scale_pos*t_air;
     else start_tn = 2*(start_scale_pos-0.5)*t_floor+t_air;
     delayt_tfloor_scale = _delayt_tfloor_scale;
     alp=0;
     Lst=_L;H0st=_H0;

     bezier_len =12;
     setBezier( _x0, _y0, _L, _H0,tair_tfloor_scale, bezier_len, _forward);
     x=y=0;

     cal_param.delay_t = delayt_tfloor_scale * t_floor;
     cal_param.use_t_long = 1000;
     cal_param.tn_last=cal_param.tn_now=0;
     cal_param.T_now=cal_param.T_last=t_floor+t_air;
     walk_leg_state.is_add_start_tn = false;
     walk_leg_state.in_delay = true;
     walk_leg_state.to_init_pos = true;
     walk_leg_state.walking = false;
     walk_leg_state.in_air_else_floor = false;
     walk_leg_state.start_act =false;
}

void WalkLegClass::move2InitPos(float _use_t_long=1000)
{
    calPos(start_tn);
    cal_param.use_t_long = _use_t_long;
    leg->moveToPos(x,y,z,0,0_use_t_long);
}

//更新腿位置  _dt:调用时间间隔 ms
void WalkLegClass::walkUpdate(float _dt)
{
    calTnNow(_dt);
    leg->update(_dt);
    walk_leg_state.x=leg->get_x();
    walk_leg_state.y=leg->get_y();
}
//_dt:ms
void WalkLegClass::calTnNow(float _dt)
{
    if(walk_leg_state.to_init_pos) //没到初始位置
    {
        cal_param.use_t_long -=_dt;
        if(cal_param.use_t_long<=0) walk_leg_state.to_init_pos = false;
        return;
    }

    if(! walk_leg_state.start_act)
        return;

    if(walk_leg_state.in_delay)
    {
        cal_param.delay_t-=_dt;
        if(cal_param.delay_t<=0) walk_leg_state.in_delay = false;
        return;
    }
    cal_param.T_last = cal_param.T_now;
    cal_param.T_now = t_floor + t_air;
    cal_param.tn_last = cal_param.tn_now;
    cal_param.tn_now = cal_param.tn_last/cal_param.T_last * cal_param.T_now + _dt;    //如果周期突变，保证在周期中时刻的比例不变
    if(!walk_leg_state.is_add_start_tn)    //初始相位差只加一次
    {
      cal_param.tn_now += start_tn ;             //当前腿时刻
      walk_leg_state.is_add_start_tn=true;
    }
    if(cal_param.tn_now>cal_param.T_now) cal_param.tn_now-= cal_param.T_now;  //在周期中的时刻

    if(cal_param.tn_last>=t_air && cal_param.tn_now<t_air)  //判断是否完成一个周期
         over_flag=true;
    else over_flag=false;

    if(over_flag==true) walk_leg_num++;

    if(cal_param.tn_now>=t_floor/2+t_air && cal_param.tn_last<t_floor/2+t_air)
        fixPointFlag=true;
    else fixPointFlag=false;

    walk_leg_state.walking = true;
    calPos(cal_param.tn_now);
    leg->setPos(x,y);
}

void WalkLegClass::calPos(float _t_now)
{
    if(_t_now<t_air)                  //曲线段  由wx12 wy12控制
    {
        walk_leg_state.in_air_else_floor = true;
        Bezier(&x,&y,wx12,wy12,bezier_len+1,_t_now/t_air);
    }
    else if(_t_now<=t_floor+t_air)    //直线段
    {
        walk_leg_state.in_air_else_floor = false;
        //if(t_floor>WalkTeMin)
        Bezier(&x,&y,wx2,wy2,2,(_t_now-t_air)/t_floor);	    //均匀的直线
        //else                                                                         // 因为电机速度达不到要求 所以直线两端点数密集  //MIT的论文里有描述原因  有空看下
        //    Bezier(&x,&y,wx,wy ,6,(tn_now-t_air)/t_floor);	     //两头密中间稀的直线    目前好像还不稳定
    }

   if(alp!=0) Rotate(&x,&y,L5/2,0,forward*alp);  //围绕腿的原点旋转

   walk_leg_state.tn = _t_now;
}

void WalkLegClass::startAct(bool _start)
{
     walk_leg_state.start_act = _start;
    if(! walk_leg_state.start_act)
        walk_leg_state.walking = false;
}

void WalkLegClass::reStart()  //有问题 没找到
{
    cal_param.delay_t = delayt_tfloor_scale * t_floor;
    cal_param.use_t_long = 1000;
    cal_param.tn_last=cal_param.tn_now=0;
    cal_param.T_now=cal_param.T_last=t_floor+t_air;
    walk_leg_state.is_add_start_tn = false;
    walk_leg_state.in_delay = true;
    walk_leg_state.to_init_pos = true;
    walk_leg_state.walking = false;
    walk_leg_state.in_air_else_floor = false;
    walk_leg_state.start_act =false;

    move2InitPos(1000);

}

void WalkLegClass::set_t_floor(float _t_floor)
{
    t_floor = _t_floor;
    t_air = t_floor * tair_tfloor_scale;
}

// 返回速度 m/s    speed：速度    _te：Te值
float WalkLegClass::setLegSpeed(float speed,float _t_floor )
{
    t_floor=_t_floor; t_air=tair_tfloor_scale*_t_floor;

    float dspeed=speed-v_leg;
    L+=dspeed*_t_floor ;
    if(L<MinL)  L=MinL;
    if(L>MaxL)  L=MaxL;

    Lst=L;    //更新设定的步长
    setBezier(x0,y0,L,H0,tair_tfloor_scale,bezier_len,forward);   //更新贝赛尔点

    v_leg=L/_t_floor;
    return v_leg;
}

//设置贝赛尔曲线的控制点
void WalkLegClass::setBezier(float _x0,float _y0,float _L,float _H0,float _tair_tfloor_scale,int _bezierLen,int _forward)
{
     x0=_x0;y0=_y0;L=_L;H0=_H0;
     bezier_len=_bezierLen;forward=_forward;
     if(L>MaxL) L=MaxL;
     if(L<MinL) L=MinL;
     float dL;  //贝赛尔曲线水平段至曲线段的控制长度
     int i=0;
     float k = 2048.0f/1859;  //高度系数
    dL=L/12 *_tair_tfloor_scale;
    wx12[0]=L/2;         wx12[1]=L/2+dL;              wx12[2]=L/2+2*dL;      wx12[3]=L/2+3*dL;
    wx12[4]=L/2+4*dL;    wx12[5]=-L/2+4*dL;           wx12[6]=-L/2-4*dL;     wx12[7]=-L/2-4*dL;
    wx12[8]=-L/2-3*dL;   wx12[9]=-L/2-2*dL;           wx12[10]=-L/2-dL;      wx12[11]=-L/2;
    wy12[0]=0;           wy12[1]=0;                   wy12[2]=0;             wy12[3]=-H0*k;
    wy12[4]=-H0*k;       wy12[5]=-H0*k;               wy12[6]=-H0*k;;        wy12[7]=-H0*k;
    wy12[8]=-H0*k;       wy12[9]=0;                   wy12[10]=0;            wy12[11]=0;

    wx2[0]=-L/2;  wx2[1]=L/2;
    wy2[0]=0;     wy2[1]=0;

    for(i=0;i<13;i++)
    {
      wx12[i]+=x0;
      wy12[i]+=y0;
    }

    wx2[0]=-L/2+x0;  wx2[1]=L/2+x0;
    wy2[0]=y0;       wy2[1]=y0;

    if(forward==-1)                                      //关于中心对称
    {
        for(i=0;i<12;i++)
        {
            wx12[i]=-wx12[i];
        }
        wx2[0]=-wx2[0];  wx2[1]=-wx2[1];
    }
}
