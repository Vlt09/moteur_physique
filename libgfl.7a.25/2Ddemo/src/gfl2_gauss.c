/*!=================================================================!*/
/*!= E.Incerti - eric.incerti@univ-eiffel.fr                       =!*/
/*!= Université Gustave Eiffel                                     =!*/
/*!= Code exemple pour prototypage avec libgfl.7e                  =!*/
/*!=================================================================!*/

#include <gfl.h>

/* tailles initiales de la fenêtre graphique (en pixels)     */
#define WWIDTH  720 
#define WHEIGHT 540

static double m,e,rac2pi;


static void drawfunction(double (*f)(double), double xmin, double xmax, GFLcolor col, double e)
{
  double dx = e*gfl_GetXPixSize();
  double xa=xmin,xb=xa+dx;
  while (xb<=xmax) { gfl_Line(xa,f(xa),xb,f(xb),col,e); xa=xb; xb+=dx; }
}

static double gaussienne(double x)
{
  return exp(-0.5*SQR(x-m)/e)/(e*rac2pi);
}

/*! fonction d'initialisation !*/
static void init(void)
{
  m=0.0;
  e=0.1;
  rac2pi = sqrt(2.*PI);
}

/*! fonction de contrôle      !*/
static void ctrl(void)
{
  gfl_CreateScrollh_d("m",&m,-5.,+5,"");
  gfl_CreateScrollv_d("e",&e, 0.,.5,"");
}

/*! fonction de dessin        !*/
static void draw(void)
{
  gfl_MultiGrad(GFLk,3,1.,GFLwc_a,.1,GFLwb_b,.01,GFLwa_c);
  drawfunction(gaussienne,gfl_GetXMin(),gfl_GetXMax(),GFLr,2);
}

/*!***************************!*/
/*! fonction principale       !*/
/*!***************************!*/
int main(int argc, char **argv)
{
  gfl_InitWindow(*argv,WWIDTH,WHEIGHT);
  /* zone de travail excentrée vers le bas */
  gfl_SetCenteredDrawZone(0.,+2.5,10.,0.);

  gfl_SetInitFunction(init); /*! fonction d'initialisation !*/
  gfl_SetCtrlFunction(ctrl); /*! fonction de contrôle      !*/
  gfl_SetDrawFunction(draw); /*! fonction de dessin        !*/

  return gfl_MainStart();
}
