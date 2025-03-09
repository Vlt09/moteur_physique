/*!=================================================================!*/
/*!= E.Incerti - eric.incerti@univ-eiffel.fr                       =!*/
/*!= Université Gustave Eiffel                                     =!*/
/*!= Code exemple pour prototypage avec libgfl.7e                  =!*/
/*!=================================================================!*/

#include <gfl.h>

/* tailles initiales de la fenêtre graphique (en pixels)     */
#define WWIDTH  720 
#define WHEIGHT 720

static int n,p;

/*! fonction d'initialisation !*/
static void init(void)
{
  n = 5;
  p = 4;
}

/*! fonction de contrôle      !*/
static void ctrl(void)
{
  gfl_CreateScrollv_i("n",&n,3,12,"n petales");
  gfl_CreateScrollv_i("p",&p,1, 8,"2^p cercles par petale");
}

/*! fonction de dessin        !*/
static void draw(void)
{
  int    i;
  double pas=1./n,r=24.*pas,R=8.;
  double s,xc,yc;
  GFLcolor col;

  for (s=1.;s>0.;s-=1./(1<<p))
    for (i=0;i<n;i++)
    {
      xc = R*s*gfl_Idxcos((i*720)/n);
      yc = R*s*gfl_Idxsin((i*720)/n);
      col= gfl_hsva_rgba_4f(1.-s,s,1.,0.);
      gfl_FillCircle(xc,yc,r*s,col);
    }
}


/*!***************************!*/
/*! fonction principale       !*/
/*!***************************!*/
int main(int argc, char **argv)
{
  gfl_InitWindow(*argv,WWIDTH,WHEIGHT);
  gfl_SetCenteredDrawZone(0.,0.,25.,0.);
  
  gfl_SetInitFunction(init); /*! fonction d'initialisation !*/
  gfl_SetCtrlFunction(ctrl); /*! fonction de controle      !*/
  gfl_SetDrawFunction(draw); /*! fonction de dessin        !*/

  return gfl_MainStart();
}
