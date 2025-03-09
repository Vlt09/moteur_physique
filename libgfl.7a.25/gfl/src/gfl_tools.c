/*!===============================================================
  E.Incerti - Universite Gustave Eiffel - eric.incerti@univ-eiffel.fr
       - Librairie GFL - Fonctions de base d'acces public -
                    version 6e - aout 2022
  ================================================================!*/
#ifdef __cplusplus
  extern "C" {
#else
  #define _GNU_SOURCE
#endif

#include <gfl.h>

/*====================================================================*/
/*                 quelques gestionnaires d'horloges                  */
/*====================================================================*/

static GFLclock calc={0,0,0,0,0,0}, /* timer de calcul  */
                real={0,0,0,0,0,0}; /* timer tps reel   */

static char proc_timer[512]="";
static char real_timer[512]="";

/* Chrono "Processeur" : determine le temps de calcul */
extern char* gfl_ProcTimer(void)
{
  static bool   init=true;
  static long   clickpersec=0;
  static struct tms start,time;
  static long   click=0;
  double tps;

  /* initialisation d'un nouveau compteur */
  if (init)
  {
    memset(&calc,0,sizeof(GFLclock));
    clickpersec = sysconf(_SC_CLK_TCK);
    times(&start);
    init=false;
  }
  /* reprise d'un compteur */
  else if (!clickpersec)
  {
    clickpersec = sysconf(_SC_CLK_TCK);
    /* nombre de sec. du compteur a continuer */
    click=((calc.hh*60+calc.mm)*60+calc.ss)*clickpersec;
  }

  times(&time);
  /* nombre de click ecoulees depuis le dernier comptage */
  click+=(time.tms_utime-start.tms_utime);
  start=time;

  /* nombre de click ecoulees depuis le dernier comptage */
  tps=(double)click/(double)clickpersec;

  calc.mm  = (int)(tps/60.);
  calc.ss  = (int)(tps-60.*calc.mm);
  calc.hh  = (int)(calc.mm/60.);
  calc.mm -= (int)(60.*calc.hh);
  sprintf(proc_timer,"proc. : %02d:%02d:%02d",calc.hh,calc.mm,calc.ss);
  return proc_timer;
}

/* Chrono temps "reel" */
extern char* gfl_Chrono(void)
{
  static bool   init=true;
  static struct timeval t;
  static int    sec=0,usec=0,s=0,u=0;

  if (init)
  {
    gettimeofday(&t,NULL);
    sec =(int)t.tv_sec;
    usec=(int)t.tv_usec;
    memset(&real,0,sizeof(GFLclock));
    init=false;
    return NULL;
  }
  gettimeofday(&t,NULL);
  s=(int)t.tv_sec - sec;
  u=(int)t.tv_usec-usec;

  sec =(int)t.tv_sec;
  usec=(int)t.tv_usec;

  if (u<0) {s++; u=1-u;}

  real.mm  = (int)(s/60.);
  real.ss  = (int)(s-60.*real.mm);
  real.hh  = (int)(real.mm/60.);
  real.mm -= (int)(60.*real.hh);
  sprintf(real_timer,"real. : %02d:%02d:%02d",real.hh,real.mm,real.ss);
  return real_timer;
}

/* recupere l'horloge courante */
extern void gfl_GetTime(GFLclock* clock)
{
  static struct timeval t;
  static struct tm *tps;
  static long   elapsed;

  elapsed=time(NULL);
  tps=localtime(&elapsed);
  gettimeofday(&t,NULL);

  clock->hh  = (uint)tps->tm_hour;
  clock->mm  = (uint)tps->tm_min;
  clock->ss  = (uint)tps->tm_sec;
  clock->ms  = (uint)(t.tv_usec/1000);
  clock->cs  = (uint)(clock->ms/10);
  clock->ds  = (uint)(clock->cs/10);
}

/*====================================================================*/
/*                   quelques generateurs aleatoires                  */
/*====================================================================*/
//static bool   _GFLRANDINIT_=false;
static double _GFLRANDCAL_=1./(1.+RAND_MAX);

/* ------------------------------------------------------------------ */
/* val. aleatoire dans [root-p% , root+p%]                            */
/* Exp. gfl_Rand_Percent(2.,10.) => [1.8,2.2]                         */
/* ------------------------------------------------------------------ */
extern double gfl_Rand_Percent(double root, double percent)
{
  //if (!_GFLRANDINIT_) { srand(getpid()); _GFLRANDINIT_=true; }
  return root*(1.+percent*(1.-2.*rand()*_GFLRANDCAL_));
}

/* ------------------------------------------------------------------ */
/* val. aleatoire dans [root-delta , root+delta]                      */
/* Exp. gfl_Rand_Percent(2.,10.) => [-8.,+12.]                        */
/* ------------------------------------------------------------------ */
extern double gfl_Rand_Delta(double root, double delta)
{
  //if (!_GFLRANDINIT_) { srand(getpid()); _GFLRANDINIT_=true; }
  return root+delta*(1.-2.*rand()*_GFLRANDCAL_);
}

/* ------------------------------------------------------------------ */
/* val. aleatoire dans [min , max]                                    */
/* Exp. gfl_Rand_Percent(2.,10.) => [2.,10.]                          */
/* ------------------------------------------------------------------ */
extern double gfl_Rand_MinMax(double min, double max)
{
  return gfl_Rand_Delta((min+max)/2.,(max-min)/2.);
}



/*====================================================================*/
/*                  fonctions trigo adaptees                          */
/*====================================================================*/

/* fonctions trigo approchees : piochent dans un tableau de 720 val.  */
/* moins precis, mais plus rapide pour une utilisation haute freq.    */
static GLdouble* gfl_cos_ = NULL;
static GLdouble* gfl_sin_ = NULL;
/*!
static int step[]={1,2,3,4,5,6,8,9,10,12,15,16,18,20,24,30,36,40,45,48,60,72,80,90,120,144,180,240,360,720};
!*/

/* libération des tables prédéfinies de cos. et sin. */
/* appelée par <atexit> via <gfl_InitTrigonometric>  */
/* -> inutilisé : c'est le Garbage Collector qui libère
static void gfl_FreeTrigonometric(void)
{
  if (gfl_cos_) free(gfl_cos_);
  gfl_cos_=NULL;
  if (gfl_sin_) free(gfl_sin_);
  gfl_sin_=NULL;
}
*/

static bool _GFL_TRIGO_=false;
/* creation des tables prédéfinies de cos. et sin.         */
static bool gfl_InitTrigonometric(void)
{
  int i;
  GLdouble pas=PI/360;
  if (!(gfl_cos_=(GLdouble*)calloc((720),sizeof(GLdouble)))) gfl_allocfail_((720)*sizeof(GLdouble));
  if (!(gfl_sin_=(GLdouble*)calloc((720),sizeof(GLdouble)))) gfl_allocfail_((720)*sizeof(GLdouble));

  GLdouble *c=gfl_cos_;
  GLdouble *s=gfl_sin_;
  for (i=0;i<720;++i) { *c++=cos(i*pas); *s++=sin(i*pas); }
  /* libération automatique en sortie  */
  //atexit(gfl_FreeTrigonometric); // libéré par le Garbage Collector
  return _GFL_TRIGO_=true;
}

/* les fct cos et sin sur tableau, en indice [0,720], en radians [0,2PI], puis en degres [0,360] */
extern double gfl_Idxcos(int i) 
{
  if (!_GFL_TRIGO_) gfl_InitTrigonometric();
  return gfl_cos_[i%720]; 
}

extern double gfl_Idxsin(int i) 
{ 
  if (!_GFL_TRIGO_) gfl_InitTrigonometric();
  return gfl_sin_[i%720]; 
}

#define _radtodeg_ 114.591559026
extern double gfl_Radcos(double rad) 
{ 
  if (!_GFL_TRIGO_) gfl_InitTrigonometric();
  return gfl_cos_[((int)(fabs(rad)*_radtodeg_))%720]; 
}
extern double gfl_Radsin(double rad) 
{ 
  if (!_GFL_TRIGO_) gfl_InitTrigonometric();
  return (rad<0.?-1.:+1.)*gfl_sin_[((int)(fabs(rad)*_radtodeg_))%720]; 
}

extern double gfl_Degcos(double deg) 
{ 
  if (!_GFL_TRIGO_) gfl_InitTrigonometric();
  return gfl_cos_[((int)(fabs(deg)*2.))%720]; 
}

extern double gfl_Degsin(double deg) 
{ 
  if (!_GFL_TRIGO_) gfl_InitTrigonometric();
  return (deg<0.?-1.:+1.)*gfl_sin_[((int)(fabs(deg)*2.))%720]; 
}

#ifdef __cplusplus
  }
#endif
