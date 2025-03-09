/*!==================================================================
  E.Incerti - Universite Gustave Eiffel - eric.incerti@univ-eiffel.fr
       - Librairie GFL - Fonctions de base d'acces public -
                    version 6e - aout 2022
  =================================================================== */
#ifdef __cplusplus
  extern "C" {
#else
  #define _GNU_SOURCE
#endif


/*======================================================*/
/*! POLYGONES           CONVEXES             POLYGONES !*/
/*!           CONVEXES            POLYGONES            !*/
/*! CONVEXES            POLYGONES            CONVEXES  !*/
/*======================================================*/
#include <gfl.h>


/*! avril 2022
 *  Révision du mode d'allocation
 *  => un gros polygône "vide" est créé à la 1° demande de Vertex par l'utilisateur.
 *     il sert de réserve : lorsqu'il faut un nouveau Vertex, on vient piocher dedans
 *     et lorsqu'un Vertex est libéré, il est réintégré dans le stock
 *     Ca serait mieux avec des blocs + petits et des <realloc>, mais le poly stockant
 *     des adresses de Vertex (prev/next), celles-ci sont invalidées en cas de déplacement
 *     du bloc mémoire.
 *  => dans cette version, on part avec un poly de taille 1024 (un peu gros...)
 * */
static GFLpolygon STOCKPOLY=NULL;
static uint       STOCKSIZE=1024;

/*! avril 2022
 * Pour régler, si nécessaire, la taille du stock
 * */
extern void gfl_SetPolygonStockSize(uint n) { STOCKSIZE = n; }

static bool gfl_InitStockPolygon(void)
{
  STOCKPOLY = (GFLvertex*)calloc(STOCKSIZE,sizeof(GFLvertex));
  GFLvertex *v=STOCKPOLY,*lastv=STOCKPOLY+STOCKSIZE;
  if (!v) return false;
  v->next = v+1;
  v++;
  while (v<lastv)
  {
    v->prev=v-1;
    v->next=v+1;
    v++;
  }
  v--;
  v->next = STOCKPOLY;
  STOCKPOLY->prev=v;
  return true;
}

/* obsolète -> garbage collector 
extern void gfl_FreeStockPolygon(void)
{
  if (STOCKPOLY) free(STOCKPOLY);
  STOCKPOLY=NULL;
}*/

/*!-------------------------------------------------------------------------!*/
/*! Equivalent de l'alloc d'un nouveau vertex : pris dans le stock          !*/
/*!-------------------------------------------------------------------------!*/
static GFLvertex* gfl_GetVertex()
{
  if (STOCKPOLY==NULL) gfl_InitStockPolygon();
  GFLvertex *newv=STOCKPOLY->prev;
  newv->prev->next = STOCKPOLY;
  STOCKPOLY->prev = newv->prev;
  memset(newv,0,sizeof(GFLvertex));
  return newv;
}

/*!-------------------------------------------------------------------------!*/
/*! => pas vraiment supprimé : remis dans le stock                          !*/
/*!-------------------------------------------------------------------------!*/
static void gfl_FreeVertex(GFLvertex *v)
{
  memset(v,0,sizeof(GFLvertex));
  v->next = STOCKPOLY;
  v->prev = STOCKPOLY->prev;
  STOCKPOLY->prev->next = v;
  STOCKPOLY->prev = v;
}

/*!-------------------------------------------------------------------------!*/
/*! supprime le vertex *V et renvoie le vertex SUIVANT (*V)->next           !*/
/*!-------------------------------------------------------------------------!*/
extern GFLvertex* gfl_DelVertex(GFLvertex** V)
{
  GFLvertex *tmp=(*V)->next;
  (*V)->prev->next=tmp;
  tmp->prev=(*V)->prev;
  gfl_FreeVertex(*V);
  return tmp;
}

/*!-------------------------------------------------------------------------!*/
/*! crée un nouveau vertex pour le point de coordonnées (x,y)               !*/
/*!-------------------------------------------------------------------------!*/
extern GFLvertex* gfl_Vertex_XY(double x, double y)
{
  GFLvertex* newptr=gfl_GetVertex();
  newptr->s.x = x;
  newptr->s.y = y;
  newptr->next= NULL;
  newptr->prev= NULL;
  return newptr;
}

/*!-------------------------------------------------------------------------!*/
/*! variante : crée un nouveau vertex pour le point P                       !*/
/*!-------------------------------------------------------------------------!*/
extern GFLvertex* gfl_Vertex(GFLpoint P)
{
  GFLvertex* newptr=gfl_GetVertex();
  newptr->s = P;
  newptr->next= NULL;
  newptr->prev= NULL;
  return newptr;
}


static void gfl_SetVertexOpen(GFLvertex *v)
{

  GFLvector u1=gfl_NormalVector2p(v->prev->s,v->s);
  GFLvector u2=gfl_NormalVector2p(v->s,v->next->s);
  v->o = gfl_ProdVect(u1,u2);
}

extern void gfl_SetPolygonOpen(GFLpolygon pol)
{
  GFLvertex *v=pol;
  do
  {
    gfl_SetVertexOpen(v);
  } while ((v=v->next)!=pol);
}

/*!-------------------------------------------------------------------------!*/
/*! insère le point P dans le polygône (crée le vertex automatiquement)     !*/
/*!-------------------------------------------------------------------------!*/
extern void gfl_InsereSommet(GFLpolygon *Pol, GFLpoint P)
{
  if (*Pol==NULL) /* si la liste est vide : 1 seul vertex, bouclé sur lui-même */
  {
    (*Pol) = gfl_Vertex(P);
    (*Pol)->prev = (*Pol)->next = (*Pol);
    (*Pol)->o = 0.;
    return;
  }
  GFLvertex* newptr;
  newptr=gfl_Vertex(P); /* nouveau vertex */
  newptr->prev=(*Pol);
  newptr->next=(*Pol)->next;
  newptr->next->prev=newptr;
  (*Pol)->next=newptr;
}

/*!-------------------------------------------------------------------------!*/
/*! même chose, mais pour un vertex déjà existant (dans une autre polygône) !*/
/*! => il est enlevé de son polyg. d'origine et inséré dans (Pol)           !*/
/*! => si Pol était vide, le nouveau vertex devient le point d'entrée       !*/
/*!    sinon, il est inséré en fin, juste avant (*Pol)                      !*/
/*!-------------------------------------------------------------------------!*/
extern void gfl_InsereVertex(GFLpolygon *Pol, GFLvertex *new_vrtx)
{
  /* v est court-circuité dans son polygône d'origine */
  new_vrtx->prev->next = new_vrtx->next;
  new_vrtx->next->prev = new_vrtx->prev;
  if (*Pol==NULL) /* si la liste est vide : 1 seul vertex, bouclé sur lui-même */
  {
    (*Pol) = new_vrtx;
    (*Pol)->prev = (*Pol)->next = (*Pol);
    (*Pol)->o = 0.;
    return;
  }
  /* cas général : v est inséré en dernier (avant Pol) */
  new_vrtx->prev=(*Pol);
  new_vrtx->next=(*Pol)->next;
  new_vrtx->next->prev=new_vrtx;
  (*Pol)->next=new_vrtx;
}


/*!-------------------------------------------------------------------------!*/
/*! insere le point P (crée le vertex) dans le polygône, après le vertex V  !*/
/*! => si V=NULL, ça démarre un nouveau polygône à 1 vertex ( sommet P)     !*/
/*! => dans tous les cas, la fonction renvoie le nouveau vertex             !*/
/*!-------------------------------------------------------------------------!*/
extern GFLvertex* gfl_InsereApres(GFLvertex* V, GFLpoint P)
{
  GFLvertex* newptr=gfl_Vertex(P);
  if (V)
  {
    newptr->prev=V;
    newptr->next=V->next;
    V->next->prev=newptr;
    V->next=newptr;
  }
  else /* si V==NULL */
  {
    newptr->prev=newptr;
    newptr->next=newptr;
  }
  return newptr;
}

/*!-------------------------------------------------------------------------!*/
/*! même chose, mais pour un vertex déjà existant                           !*/
/*! => il est enlevé de son polyg. d'origine et inséré dans le nouveau      !*/
/*!-------------------------------------------------------------------------!*/
extern GFLvertex* gfl_InsereVertexApres(GFLvertex* prev_vrtx, GFLvertex *new_vrtx)
{
  /* v est court-circuité dans son polygône d'origine */
  new_vrtx->prev->next = new_vrtx->next;
  new_vrtx->next->prev = new_vrtx->prev;
  if (prev_vrtx)
  {
    new_vrtx->prev=prev_vrtx;
    new_vrtx->next=prev_vrtx->next;
    prev_vrtx->next->prev=new_vrtx;
    prev_vrtx->next=new_vrtx;
  }
  else /* si prev_vrtx==NULL */
  {
    new_vrtx->prev=new_vrtx;
    new_vrtx->next=new_vrtx;
  }
  return new_vrtx;
}

/*!-------------------------------------------------------------------------!*/
/*! supprime les sommets doubles (points identiques)                        !*/
/*!-------------------------------------------------------------------------!*/
extern int gfl_CleanPolygon(GFLpolygon Pol)
{
  int n=1;
  GFLvertex *a,*b;

  if (Pol==NULL) return 0;
  a=Pol;
  b=a->next;
  while (b!=Pol)
  {
    if (a->s.x==b->s.x && a->s.y==b->s.y) gfl_DelVertex(&b);
    n++;
    a=b;
    b=a->next;
  }
  return n;
}

/*!-------------------------------------------------------------------------!*/
/*! supprime les sommets alginés                                            !*/
/*!-------------------------------------------------------------------------!*/
extern int gfl_CleanAlignVertex(GFLpolygon Pol)
{
  int n=1;
  GFLvertex *a,*b,*c;

  if (Pol==NULL) return 0;
  a=Pol;
  if (a->next==Pol) return 0;
  b=a->next;
  if (b->next==Pol) return 0;
  c=b->next;
  while (c!=Pol)
  {
    double o = gfl_Orient(a->s,b->s,c->s);
    if (GFLISZERO(o)) gfl_DelVertex(&b);
    n++;
    a=b;
    b=c;
    c=c->next;
  }
  return n;
}


/*!-------------------------------------------------------------------------!*/
/*! Enregistrement en mode texte (ou simple affichage)                      !*/
/*!-------------------------------------------------------------------------!*/
extern void gfl_PrintPolygon(GFLpolygon Pol, FILE* output)
{
  int i,N=0;
  GFLvertex *tmp;
  char    ok;

  tmp=Pol;
  while (tmp!=NULL && tmp->next!=Pol)
  {
    N++;
    tmp=tmp->next;
  }
  ok = (tmp==NULL ? 'n':'o');
  fprintf(stderr,"Polygone %s a %d sommets\n",ok=='o'?"ferme":"ouvert",N);

  if (output==stderr) fprintf(stderr," N=");
  fprintf(output,"%d\n",N);
  i=0;
  tmp=Pol;
  while (i<=N)
  {
    if (output==stderr) fprintf(stderr," s[%2d] : ",i);
    fprintf(output,"%lf %lf\n",tmp->s.x,tmp->s.y);
    tmp=tmp->next;
    i++;
  }
  fprintf(output,"%c\n",ok);
}

/*!-------------------------------------------------------------------------!*/
/*! Lecture sur fichier                                                     !*/
/*!-------------------------------------------------------------------------!*/
extern void gfl_ScanPolygon(GFLpolygon* Pol, FILE* input)
{
  int N,i;
  GFLvertex *tmp;
  double  x,y;
  char    ok;

  if (input==stdin) fprintf(stderr," N=");
  if (1!=fscanf(input,"%d\n",&N)) { fprintf(stderr,"\e[1m<%s>\e[0m cannot read vertex number\n",__func__); return; }
  i=0;
  if (input==stdin) fprintf(stderr," s[%2d] : ",i);
  if (2!=fscanf(input,"%lf %lf\n",&x,&y)) { fprintf(stderr,"\e[1m<%s>\e[0m cannot read vertex coord.\n",__func__); return; }
  *Pol=gfl_Vertex_XY(x,y);

  tmp=*Pol;
  while (++i<N+1)
  {
    if (input==stdin) fprintf(stderr," s[%2d] : ",i);
    if (2!=fscanf(input,"%lf %lf\n",&x,&y)) { fprintf(stderr,"\e[1m<%s>\e[0m cannot read vertex coord.\n",__func__); return; }
    tmp->next=gfl_Vertex_XY(x,y);
    tmp->next->prev=tmp;
    tmp=tmp->next;
  }
  if (input==stdin) fprintf(stderr,"Fermer le GFLpolygon ? ");
  if (fscanf(input,"%c\n",&ok) && (ok=='o' || ok=='0' || ok=='\n'))
  {
    tmp->next=*Pol;
    (*Pol)->prev=tmp;
  }
}

/*!-------------------------------------------------------------------------!*/
/*! Polygône régulier à <deg> sommets                                       !*/
/*! - Centré en <C>, de rayon <r>, oritenté d'un angle <alpha>              !*/
/*!-------------------------------------------------------------------------!*/
extern void gfl_RegulPolygon(GFLpolygon* Pol, uint deg, GFLpoint C, double r, double alpha)
{
  GFLvertex* tmp;
  double apas=2.*PI/deg;

   alpha+=0.5*apas;
  *Pol=gfl_Vertex_XY(C.x+r*cos(alpha),C.y+r*sin(alpha));
  tmp=*Pol;
  while (--deg)
  {
    alpha+=apas;
    tmp->next=gfl_Vertex_XY(C.x+r*cos(alpha),C.y+r*sin(alpha));
    tmp->next->prev=tmp;
    tmp=tmp->next;
  }
  tmp->next=*Pol;
  (*Pol)->prev=tmp;
}

/*!-------------------------------------------------------------------------!*/
/*! Polygône construit sur un tableau de points, fermé ou non               !*/
/*!-------------------------------------------------------------------------!*/
extern void gfl_TabPolygon(GFLpolygon* Pol, GFLpoint S[], uint n, bool closed)
{
  GFLvertex* tmp;
  uint i;

  *Pol=gfl_Vertex(S[0]);
  tmp=*Pol;
  i=0;
  while (++i<n)
  {
    tmp->next=gfl_Vertex(S[i]);
    tmp->next->prev=tmp;
    tmp=tmp->next;
  }
  if (closed)
  {
    tmp->next=*Pol;
  (*Pol)->prev=tmp;
  }
}


/*!-------------------------------------------------------------------------!*/
/*! vérifie si le polygône est convexe                                      !*/
/*!-------------------------------------------------------------------------!*/
extern int gfl_IsConvex(GFLpolygon Pol)
{
  GFLvertex* v=Pol;
  int orient = gfl_Orient(v->s,v->next->s,v->next->next->s);
  do
  {
    v=v->next;
    if (gfl_Orient(v->s,v->next->s,v->next->next->s)!=orient) return 0;
  } while (v->next!=Pol);
  return orient;
}

extern void gfl_DrawPolygon(GFLpolygon Pol, GFLcolor col, uint w)
{
  GFLvertex* tmp=Pol;
  double  xa,ya,xb,yb;

  if (Pol==NULL)
    return;
  if (Pol==Pol->next)
    return gfl_Plot(Pol->s.x,Pol->s.y,col,w);
  if (Pol==Pol->next->next)
    return gfl_Line(Pol->s.x,Pol->s.y,Pol->next->s.x,Pol->next->s.y,col,w);

  xa=tmp->s.x;
  ya=tmp->s.y;
  do
  {
    tmp=tmp->next;
    xb=tmp->s.x;
    yb=tmp->s.y;
    gfl_Line(xa,ya,xb,yb,col,w);
    xa=xb;
    ya=yb;
  }
  while (tmp!=Pol);
}

extern void gfl_DrawSplinePolygon(GFLpolygon Pol)
{
  GFLvertex* tmp=Pol;
  double xa,ya,xb,yb;
  static GFLcolor  col1={0.75,0.50,0.25};
  static GFLcolor  col2={0.12,0.25,0.33};
  int    i=0;
  do
  {
    xa=tmp->s.x; ya=tmp->s.y;
    if (i%3==0)
      gfl_Plot(xa,ya,col1,4);
    else
      gfl_Plot(xa,ya,col2,2);
    i++;
    tmp=tmp->next;
    xb=tmp->s.x; yb=tmp->s.y;
    gfl_LineDft(xa,ya,xb,yb);
  }
  while (tmp!=Pol);
}


extern void gfl_DrawFilledPolygon(GFLpolygon Pol, GFLcolor col)
{
  GFLvertex* tmp=Pol;
  double xa,ya,xb,yb,xc,yc;

  if (Pol==NULL)
    return;
  if (Pol==Pol->next)
    return gfl_Plot(Pol->s.x,Pol->s.y,col,1);
  if (Pol==Pol->next->next)
    return gfl_Line(Pol->s.x,Pol->s.y,Pol->next->s.x,Pol->next->s.y,col,1);

  if (!gfl_IsConvex(Pol)) return gfl_DrawPolygon(Pol,col,1);

  xa=tmp->s.x;
  ya=tmp->s.y;
  tmp=tmp->next;
  do
  {
    xb=tmp->s.x;
    yb=tmp->s.y;
    tmp=tmp->next;
    xc=tmp->s.x;
    yc=tmp->s.y;
    gfl_FillTriangle(xa,ya,xb,yb,xc,yc,col);
  }
  while (tmp!=Pol);
}


extern void gfl_FreePolygon(GFLpolygon* Pol)
{
  GFLvertex* tmp;

  if (*Pol==NULL) return;
  tmp=(*Pol)->next;
  while (tmp!=*Pol)
  {
    (*Pol)->next=tmp->next;
    gfl_DelVertex(&tmp);
    tmp = (*Pol)->next;
  }
  gfl_DelVertex(Pol);
  *Pol=NULL;
}


extern bool gfl_PointDansPolygon(GFLpoint P, GFLpolygon Pol)
{
  /* il faut commencer par determiner */
  /* l'orientation du GFLpolygon        */
  double sgn = GFLorient(Pol->s,Pol->next->s,Pol->next->next->s);
  GFLvertex* tmp=Pol;
  do
  {
    if (sgn*GFLorient(P,tmp->s,tmp->next->s)<0.) return false;
    tmp=tmp->next;
  }
  while (tmp!=Pol);
  return true;
}

/*!-------------------------------------------------------------------------!*/
/*! version alternative                                                     !*/
/*!  cette version renvoie un vertex (*som) :                               !*/
/*!  - NULL si le  vertex v est dans le polygône                            !*/
/*!  - sinon c'est que P est dehors, par rapport à [(*som),(*som)->next]    !*/
/*!-------------------------------------------------------------------------!*/
extern bool gfl_VertexDansPolygon(GFLvertex *v, GFLpolygon Pol, int orient, GFLvertex** som)
{
  GFLvertex* tmp=Pol;
  do
  {
    if (orient*GFLorient(v->s,tmp->s,tmp->next->s)<0.)
    {
     *som = tmp;
      return false;
    }
    tmp=tmp->next;
  }
  while (tmp!=Pol);
 *som=NULL;
  return true;
}

/* une 3° version... */
extern GFLvertex* gfl_PointDansPolygon3(GFLpoint P, GFLpolygon Pol)
{
  double sgn = GFLorient(Pol->s,Pol->next->s,Pol->next->next->s);
  GFLvertex* tmp=Pol;
  do
  {
    if (sgn*GFLorient(P,tmp->s,tmp->next->s)<0.)
      return tmp;
    tmp=tmp->next;
  }
  while (tmp!=Pol);
  return NULL;
}

extern bool gfl_Clipping(GFLpoint *A, GFLpoint *B, GFLpolygon Pol)
{
  GFLpoint   P,Q;
  GFLvertex *S;
  bool boolA = gfl_PointDansPolygon(*A,Pol);
  bool boolB = gfl_PointDansPolygon(*B,Pol);
  bool flag  = false;

  if (boolA && boolB )return true;    /* A et B dedans */

  /* on cherche une premiere intersection*/
  S = Pol;
  while (!flag && S->next!=Pol)
  {
    flag = gfl_SegmentInterSegment(*A,*B,S->s,S->next->s,&P);
    S=S->next;
  }

  if (flag) /* si on en a trouve une */
  {
    if (boolB) { *A=P;  return true; } /* B dedans, donc A dehors */
    if (boolA) { *B=P;  return true; } /* A dedans, donc B dehors */
    flag = false;
    while (!flag)                   /* A et B dehors           */
    { /* on cherche la seconde intersection :
         on est sûr qu'elle existe */
      flag = gfl_SegmentInterSegment(*A,*B,S->s,S->next->s,&Q);
      S=S->next;
    }
    if (GFLpntprodscal(P,*A,Q)<0)
    { *A=P; *B=Q; }
    else
    { *A=Q; *B=P; }
    return true;
  }
  /* si on n'a rien trouve */
  return false;
}

/*= autre version =*/
extern int gfl_Clipping2(GFLpoint* A, GFLpoint* B, GFLpolygon Pol)
{
  GFLpoint   P,Q;
  GFLvertex *S;
  bool boolA = gfl_PointDansPolygon(*A,Pol);
  bool boolB = gfl_PointDansPolygon(*B,Pol);
  bool flag  = false;

  if (boolA && boolB )return 0;    /* A et B dedans */

  /* on cherche une premiere intersection*/
  S = Pol;
  while (!flag && S->next!=Pol)
  {
    flag = gfl_SegmentInterSegment(*A,*B,S->s,S->next->s,&P);
    S=S->next;
  }

  if (flag) /* si on en a trouve une */
  {
    if (boolB) { *A=P;  return 1; } /* B dedans, donc A dehors */
    if (boolA) { *B=P;  return 2; } /* A dedans, donc B dehors */
    flag = false;
    while (!flag)                   /* A et B dehors           */
    { /* on cherche la seconde intersection :
         on est sûr qu'elle existe */
      flag = gfl_SegmentInterSegment(*A,*B,S->s,S->next->s,&Q);
      S=S->next;
    }
    if (GFLpntprodscal(P,*A,Q)<0)
    { *A=P; *B=Q; }
    else
    { *A=Q; *B=P; }
    return 3;
  }
  /* si on n'a rien trouve */
  return -1;
}

extern GFLpolygon gfl_PolygonInterPolygon(GFLpolygon Pol1, GFLpolygon Pol2)
{
  GFLpolygon Inter=NULL;
  GFLpoint   A,B;
  GFLvertex *tmp;

  if (!gfl_IsConvex(Pol1) || !gfl_IsConvex(Pol2)) return NULL;

  tmp=Pol1;
  do
  {
    A = tmp->s;
    B = tmp->next->s;

    switch (gfl_Clipping2(&A,&B,Pol2))
    {
      case 0 : case 1 : Inter=gfl_InsereApres(Inter,A); break;
      case 2 : case 3 : Inter=gfl_InsereApres(Inter,A);
                        Inter=gfl_InsereApres(Inter,B);
                        break;
    }
    tmp =  tmp->next;
  } while (tmp!=Pol1);

  tmp=Pol2;
  do
  {
    if (gfl_PointDansPolygon(tmp->s,Pol1))
    {
      if (Inter==NULL || Inter->next==Inter || Inter->next->next==Inter)
        Inter=gfl_InsereApres(Inter,tmp->s);
      else
        Inter=gfl_InsereApres(gfl_PointDansPolygon3(tmp->s,Inter),tmp->s);
    }
    tmp =  tmp->next;
  } while (tmp!=Pol2);

  return Inter;
}

/*! Clipping d'un polygone sur un cercle :
  *  -1 : les 2 sont disjoints - par d'untersection, pas d'inclusion
  *   0 : le polygone est inclus dans le cercle
  *  +1 : le cercle est inclus dans le polygone
  * n>1 : le polygone intersection a n sommets, certains pouvant
          appartenir au polyg. initial.
                                                                  !*/
extern int gfl_CercleClipPolygon(GFLpoint C, double r, GFLpolygon Poly, GFLpolygon *Clip)
{
  GFLvertex* v=Poly;
  GFLpoint   A,B,I1,I2;
  int       sum=0;
  int       n;

  do
  {
    A=v->s;
    v=v->next;
    B=v->s;
    n=0;
    if (GFLsqrdist(C,A)<r*r)
    { gfl_InsereSommet(Clip,A);
      sum++;
      n=1;
    }
    if (GFLsqrdist(C,B)<r*r && n==1) continue;

    n=gfl_SegmentInterCercle(A,B,C,r,&I1,&I2);
    switch (n)
    {
      case -1 : case 0 : case +1 : break;
      case +2 : gfl_InsereSommet(Clip,I2); sum++;
                break;
      case +3 : gfl_InsereSommet(Clip,I1); sum++;
                break;
      case +4 : gfl_InsereSommet(Clip,I1); sum++;
                gfl_InsereSommet(Clip,I2); sum++;
                break;
    }

  }
  while (v!=Poly);

  /* Il y a au moins 1 intersection */
  if (sum>0) return sum;

  /* Aucune intersection et au moins 1 sommet est dans le cercle */
  /* donc il y sont tous et le poly est inclu dans le cercle */
  if (GFLsqrdist(C,Poly->s)<r*r) return 0;

  /* tous les sommets du polyg. sont hors du cercle :
     - si C est dans le polyg, le cercle y estentierement (return 1)
     - sinon les 2 sont disjoints (return 0) */
  return gfl_PointDansPolygon(C,Poly)?1:-1;
}

extern bool gfl_ConvexHull(GFLpoint *point, int nbpt, GFLpolygon *EC, int *EClen)
{
  if (nbpt<3) return false;
  /* 1) Initialisation sur les 3 premiers points */
  GFLpoint *p=point;
  (*EC) = gfl_InsereApres((*EC),*p);
  gfl_InsereApres((*EC),*(p+1));
  /* Pour le 3°, on teste le triangle <P0,P1,P2> pour savoir si il est direct ou pas
     FONDAMENTAL : ca détermine dans quel sens on tourne */
  if (gfl_Orient(*(p),*(p+1),*(p+2))>=0.)
    gfl_InsereApres((*EC)->next,*(p+2));
  else
    gfl_InsereApres((*EC),*(p+2));
  (*EClen) = 3;
  p+=3;

  GFLvertex *v,*w;
  while (p<point+nbpt)
  {
    v=*EC;
    do
    {
      if (gfl_Orient(*p,v->s,v->next->s)<0.)
      { /* p est hors de l'enveloppe par rapport au segment [v,v.next] */
        /* On insere p entre v et v->next                              */
        v=gfl_InsereApres(v,*p);
        (*EClen)++;
        /* On nettoie ce qu'il y a avant */
        w=v->prev;
        /*! ATTENTION ICI :
         *  la fonction <gfl_DelVertex> renvoie le vertex suivant celui qui est détruit
         *  il faut donc RECULER d'un cran et prendre le <prev>                        !*/
        while (gfl_Orient(w->s,w->prev->s,w->next->s)>0)
        {
          w=gfl_DelVertex(&w)->prev;
          (*EClen)--;
        }
        /* On nettoie ce qu'il y a apres */
        w=v->next;
        while (gfl_Orient(w->s,w->prev->s,w->next->s)>0)
        {
          w=gfl_DelVertex(&w);
          (*EClen)--;
        }
        /* On repositionne le début de liste sur le point inséré */
        *EC=v;
        /* Et on s'en va */
        break;
      }
      /* sinon, on teste le segment suivant */
      v = v->next;
    }
    while(v!=*EC);
    /* Le point était à l'intérieur */
    p++;
  }
  return true;
}
