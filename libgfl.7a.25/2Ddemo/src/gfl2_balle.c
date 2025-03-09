/*!=================================================================!*/
/*!= E.Incerti - eric.incerti@univ-eiffel.fr                       =!*/
/*!= Université Gustave Eiffel                                     =!*/
/*!= Code exemple pour prototypage avec libgfl.7e                  =!*/
/*!=================================================================!*/

#include <gfl.h>

/* tailles initiales de la fenêtre graphique (en pixels)     */
#define WWIDTH  720 
#define WHEIGHT 720

/* variables géométriques */
static GFLpoint  ctr; /* un point   : centre du cercle    */
static double    ray; /* un réel    : rayon du cercle     */
static GFLvector vit; /* un vecteur : vitesse de la balle */

/* variables de contrôle */
static bool FILLDISK = false;
static int  tempo = 10; 

/*! fonction d'initialisation !*/
static void init(void)
{
  /* les objets de la scène */
  ctr=(GFLpoint ){-4.,+1.};
  vit=(GFLvector){0.02,0.012};
  ray=1.;
  FILLDISK=false; /* au départ, le cercle est 'vide' */
}

/*! fonction de contrôle      !*/
static void ctrl(void)
{
  /* le centre devient actif (drag_&_drop), "cliquable" dans un rayon de 10 pixels */
  gfl_SetControlPoint(&ctr,10);
  /* un scrollbar horizontal associé à la variable <ray> : varie dans [0.1,5.]     */
  gfl_CreateScrollh_d("ray",&ray,0.1,5.,"rayon du cercle");
  /* un scrollbar vertical associé à la variable <tempo> : varie dans [0,5000]     */
  gfl_CreateScrollv_i("tempo",&tempo,0,5000,"temporisation (micro-sec.)");
  /* un bouton on/off pour 'remplir/vider' le cercle                               */
  gfl_CreateSwitch("F.",&FILLDISK,"Disque plein ou pas");
}

/*! fonction de gestion des événements !*/
static void evts(void)
{
  gfl_SetDelay(tempo); /* réglage de la temporisation (micro-secondes) */
}


/*! fonction de dessin        !*/
static void draw()
{
  switch (FILLDISK)
  {
    case true : gfl_DrawFillCircle(ctr,ray,GFLo);
    default   : gfl_DrawCircle(ctr,ray,GFLr,2);
  }
}

/*! fonction d'animation      !*/
static void anim()
{
  /* on veut que le cercle se déplace tout seul à la vitesse <vit> *
   * et rebondisse sur les bords de la fenêtre                     */
  /* 1°) le cercle se déplace */
  ctr.x+=vit.x;
  ctr.y+=vit.y;
  /* 2°) si il est sorti de la fenêtre, on inverse la vitesse */
  if (ctr.x<gfl_GetXMin()+ray || ctr.x>gfl_GetXMax()-ray) vit.x=-vit.x;
  if (ctr.y<gfl_GetYMin()+ray || ctr.y>gfl_GetYMax()-ray) vit.y=-vit.y;
}


/*!***************************!*/
/*! fonction principale       !*/
/*!***************************!*/
int main(int argc, char **argv)
{
  /* 1°) creation de la fenetre - titre et tailles (pixels)  */
  gfl_InitWindow(*argv,WWIDTH,WHEIGHT);
  /* 2°) définition de la zone de travail en coord. réeelles *
   *     par défaut (pas d'initialisation) la zone 'réelle'  *
   *     est [(0.,0.),(WWIDTH,WHEIGHT)]                      */
  gfl_SetCenteredDrawZone(0.,0.,10.,0.);

  gfl_SetInitFunction(init); /*! fonction d'initialisation !*/
  gfl_SetCtrlFunction(ctrl); /*! fonction de contrôle      !*/
  gfl_SetEvtsFunction(evts); /*! fonction d'événements     !*/
  gfl_SetDrawFunction(draw); /*! fonction de dessin        !*/
  gfl_SetAnimFunction(anim); /*! fonction d'animation      !*/

  return gfl_MainStart();
}
