/*!==================================================================
  E.Incerti - Universite Gustave Eiffel - eric.incerti@univ-eiffel.fr
       - Librairie GFL - Fonctions de base d'acces public -
                    version 6e - aout 2022
  ===================================================================!*/

/*============================================*/
/*=   GESTIONS DES POINTS DE CONTROLE REELS  =*/
/*============================================*/
/**@brief describes a mouse controlable point. */
typedef struct
{
  GFLpoint* add; /**@brief the address of the point structure to be controled */
  int       ray; /**@brief the range at witch the point may be conroled. */
} GFLctrlpt;
  

/*=  Les points de controles sont ranges dans un tableau semi-dynamique automatique  =*/
static int        nbblocs=0;   // nombre de blocs d'allocation (<_GFL_BLOCSIZE_>)
static GFLctrlpt* CTRLPTS=NULL;// tableau des <ctrlpt>
static int        nbcpt=0;     // nbre total de points de controle
static GFLpoint*  CPT=NULL;    // adresse du point courant selectionne
static GFLpoint   CPTpos;      // sa position

/*= revoie <true> si il existe au moins un point 'cliquable' =*/
/*= utilite douteuse.....                                    =*/
extern bool gfl_GetCpt() { return nbcpt?true:false;}

/*= ajoute le point d'adresse <P> dans la liste
    en lui attribuant le 'rayon' de detection <ray> (pixels) =*/
extern bool gfl_SetControlPoint(GFLpoint* P, int  ray)
{
  if (nbblocs>=_GFL_MAXBLOC_) gfl_error_(" nombre max (%d) de point de controle atteint\e0;0m\n",nbcpt);

  if (nbcpt%_GFL_BLOCSIZE_==0)
  {
    CTRLPTS=(GFLctrlpt*)realloc(CTRLPTS,(nbblocs+1)*_GFL_BLOCSIZE_*sizeof(GFLctrlpt));
    if (!CTRLPTS) return false;
    memset(CTRLPTS+nbblocs*_GFL_BLOCSIZE_,0,_GFL_BLOCSIZE_*sizeof(GFLctrlpt));
    nbblocs++;
  }
  CTRLPTS[nbcpt].add = P;
  CTRLPTS[nbcpt].ray = ray;
  nbcpt++;
  return true;
}

/*= Renvoie l'adresse du point de controle selectionne       =*/
extern GFLpoint* gfl_GetControlPoint(void)
{ return (pushmode==GLUT_UP)||(CPT==NULL)?NULL:(CPT); }

/*= Renvoie l'adresse du point de controle selectionne       =*/
extern GFLpoint* gfl_GetControlPoint2(GFLpoint *old)
{ return (pushmode==GLUT_UP)||(CPT==NULL)?NULL:(*old=CPTpos,CPT); }

/*= Annule le deplacement du point de controle selectionne   =*/
extern void gfl_ReplaceControlPoint(void)
{ *CPT=CPTpos; }

/*= Supprime le point de controle                            =*/
extern void gfl_CleanControlPoint(GFLpoint* pt)
{
  int n=0;
  while(n<nbcpt)
  {
    if (CTRLPTS[n].add==pt)
    {
      nbcpt--;
      CTRLPTS[n].add=CTRLPTS[nbcpt].add; CTRLPTS[nbcpt].add=NULL;
      CTRLPTS[n].ray=CTRLPTS[nbcpt].ray; CTRLPTS[nbcpt].ray=0;
      return;
    }
    n++;
  }
}

/*= Libere la liste des points de controle                   =*/
extern void gfl_CleanAllControlPoint(void)
{
  if (CTRLPTS) { free(CTRLPTS); CTRLPTS=NULL; }
  nbblocs=0;
  nbcpt=0;
  CPT=NULL;
}
