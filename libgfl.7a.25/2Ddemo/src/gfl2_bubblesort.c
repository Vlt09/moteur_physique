/*!=================================================================!*/
/*!= E.Incerti - eric.incerti@univ-eiffel.fr                       =!*/
/*!= Université Gustave Eiffel                                     =!*/
/*!= Code exemple pour prototypage avec libgfl.7e                  =!*/
/*!=================================================================!*/

#include <gfl.h>

/* tailles initiales de la fenêtre graphique (en pixels)     */
#define WWIDTH  1024
#define WHEIGHT 512

#define FULLSTEP ((bool)true)
static bool STEPS = FULLSTEP;

/* variables globales : données */
static int      val[WWIDTH];  // valeurs dans [0,WWHEIGHT]
static GFLcolor map[WHEIGHT]; // la couleur correspond à la valeur
static int      N=256,i,j;
static double   barw=0.;
/* nouveau tirage     */
static void reset(void)
{
  int* v=val;
  while (v<val+N)
    *v++ = (int)(rand()%WHEIGHT);
  i=0;
  j=1;
}

/* échange de valeurs */
static void swap(int *T, int i, int j)
{
  int tmp;
  tmp =T[i];
  T[i]=T[j];
  T[j]=tmp;
}

/*! fonction d'initialisation !*/
static void init(void)
{
  srand(getpid());
  gfl_ColorMapHSV360(map,WHEIGHT);
  reset();
}

/*! fonction de contrôle      !*/
static void ctrl(void)
{
  gfl_CreateSwitch("STEPS",&STEPS,"double boucle / boucle principale");
  gfl_CreatePopUp("reset",reset,"reset");
  gfl_CreateScrollv_i("N",&N,1,1+WWIDTH/4,"N");
}

/*! fonction d'événements     !*/
static void evts(void)
{
  static int _N_=0;
  barw=(double)gfl_GetPixWidth()/(double)N;
  if (N==_N_) return;
  reset();
  _N_=N;
}

/*! fonction de dessin        !*/
static void draw()
{
  int* v;
  double x1=-0.5*barw;
  double x2=x1+barw;
  for (v=val; v<val+N; v++)
  {  
    gfl_FillRectangle(x1,0.,x2,*v,map[*v]);
    x1 =x2;
    x2+=barw;
  }
}

/*! fonction d'animation 1     !*/
static void full_steps(void)
{
  /* calcul : une étape du Bubble Sort */
  if (val[i]>val[j]) swap(val,i,j);
  j++;
  
  if (j==N) { i++; j=i+1; }
  /* fin : stop anim */
  if (i==N-1) gfl_Stop();
}

static void main_steps(void)
{
  /* calcul : une étape du Bubble Sort */
  for (int j=i+1; j<N; j++)
    if (val[i]>val[j]) swap(val,i,j);
  i++;
  /* fin : stop anim */
  if (i==N-1) gfl_Stop();
}


static void anim(void)
{
    switch (STEPS)
    {
      case FULLSTEP : full_steps(); break;
      default       : main_steps(); break;
    }
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

  gfl_SetInitFunction(init); /*! fonction d'initialisation !*/
  gfl_SetCtrlFunction(ctrl); /*! fonction de contrôle      !*/
  gfl_SetEvtsFunction(evts); /*! fonction d'événements     !*/
  gfl_SetDrawFunction(draw); /*! fonction de dessin        !*/
  gfl_SetAnimFunction(anim); /*! fonction d'animation      !*/

  return gfl_MainStart();
}
