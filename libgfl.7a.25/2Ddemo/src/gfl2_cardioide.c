/*!=================================================================!*/
/*!= E.Incerti - eric.incerti@univ-eiffel.fr                       =!*/
/*!= Université Gustave Eiffel                                     =!*/
/*!= Code exemple pour prototypage avec libgfl.7e                  =!*/
/*!=================================================================!*/

#include <gfl.h>

/* tailles initiales de la fenêtre graphique (en pixels)     */
#define WWIDTH  720 
#define WHEIGHT 540

static double a,pas;
static int    nbpts;

static GFLpoint cardioide(double a, double theta)
{
  double c= a*(1.+cos(theta));
  double x=c*cos(theta);
  double y=c*sin(theta);
  return (GFLpoint){x,y};
}

/*! fonction d'initialisation !*/
static void init(void)
{
  a=1.;
  nbpts = 10;
  pas = (2.*PI)/nbpts;
}

/*! fonction de contrôle      !*/
static void ctrl(void)
{
  gfl_CreateScrollh_d("a",&a    , 0.,10.,"rayon     ");
  gfl_CreateScrollh_i("n",&nbpts, 3 ,50 ,"résolution");
}

/*! fonction d'événements     !*/
static void evts(void)
{
  /* centrage automatique de la fenêtre sur le centre de la courbe */
  gfl_SetCenteredDrawZone(a,0.,10.,0.);
  /* si la résolution (npts) est modifiée, *
   * il faut recalculer le pas             */
  pas = (2.*PI)/nbpts;
}

/*! fonction de dessin        !*/
static void draw(void)
{
  gfl_Axes();
  GFLpoint P = cardioide(a,0.0),Q;
  gfl_Print(P.x,P.y,GFLr,"(%.2lf,%.2lf)",P.x,P.y);
  for (double th=pas; th<2*PI; th+=pas)
  {
    Q = cardioide(a,th);
    gfl_DrawLine(P,Q,GFLr,2);
    P=Q;
  }
  // pour fermer
  Q = cardioide(a,0.0);
  gfl_DrawLine(P,Q,GFLr,2);
}

/*!***************************!*/
/*! fonction principale       !*/
/*!***************************!*/
int main(int argc, char **argv)
{
  gfl_InitWindow(*argv,WWIDTH,WHEIGHT);
  /* définition de la zone de travail en coordonnées réelles */
  gfl_SetCenteredDrawZone(0.,0.,10.,0.);

  gfl_SetInitFunction(init); /*! fonction d'initialisation !*/
  gfl_SetCtrlFunction(ctrl); /*! fonction de contrôle      !*/
  gfl_SetEvtsFunction(evts); /*! fonction d'événements     !*/
  gfl_SetDrawFunction(draw); /*! fonction de dessin        !*/

  return gfl_MainStart();
}
