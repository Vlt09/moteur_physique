/*!=================================================================!*/
/*!= E.Incerti - eric.incerti@univ-eiffel.fr                       =!*/
/*!= Université Gustave Eiffel                                     =!*/
/*!= Code exemple pour prototypage avec libgfl.7e                  =!*/
/*!=================================================================!*/

#include <gfl.h>

/* tailles initiales de la fenêtre graphique (en pixels)     */
#define WWIDTH  720 
#define WHEIGHT 540

static GFLpoint ctr; /* un point : centre du cercle */
static double   ray; /* un réel  : rayon du cercle  */

/*! fonction d'initialisation !*/
static void init(void)
{
  ctr = (GFLpoint){0.,0.}; /* positionnement de centre */
  ray = 1.;                /* rayon initial            */
}

/*! fonction de contrôle      !*/
static void ctrl(void)
{
  /* le centre devient actif (drag_&_drop), "cliquable" dans un rayon de 10 pixels */
  gfl_SetControlPoint(&ctr,10);
  /* un scrollbar vertical associé à la variable <ray> : varie dans [0.1,5.]       */
  gfl_CreateScrollv_d("ray",&ray,0.1,5.,"rayon du cercle");
}

/*! fonction de dessin        !*/
static void draw(void)
{
  /* le cercle, en rouge, avec une épaisseur de trait de 2 */
  gfl_DrawCircle(ctr,ray,GFLr,2);
  /* ou gfl_Cricle(ctr.x, ctr.y, ray, GFLr, 2); */

  /* le centre, en bleu (5x5 pixels) : point de contrôle     */
  gfl_Plot(ctr.x,ctr.y,GFLb,5);
}

/*!***************************!*/
/*! fonction principale       !*/
/*!***************************!*/
int main(int argc, char **argv)
{
  gfl_InitWindow(*argv,WWIDTH,WHEIGHT);
  /* 2°) définition de la zone de travail en coord. réeelles *
   *     par défaut (pas d'initialisation) la zone 'réelle'  *
   *     est [(0.,0.),(WWIDTH,WHEIGHT)]                      */
  gfl_SetCenteredDrawZone(0.,0.,10.,0.);

  gfl_SetInitFunction(init); /*! fonction d'initialisation !*/
  gfl_SetCtrlFunction(ctrl); /*! fonction de contrôle      !*/
  gfl_SetDrawFunction(draw); /*! fonction de dessin        !*/

  return gfl_MainStart();
}
