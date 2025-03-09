/*==================================================================*
 * Version 6 : même chose que version 4 mais en plus récupère les infos
 *             __FILE__,__func__ et __LINE__ via les macros des clônes
 *
 * 1°) compilation du module mem_tracker.o (SANS le header)
 *  $> gcc  -c -Wall src/mem_tracker_05.c -o mem_tracker_05.o
 * 2°) compilation du prog. principal trac_05.o (AVEC le header)
 *  $> gcc  -c -Wall  -include include/mem_tracker_05.h src/main_05.c -o main_05.o
 * 5°) assemblage, édition de lien
 *  $> gcc mem_tracker_05.o main_05.o -o  main_05
 *==================================================================*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <gfl_types.h>
#include <gfl_error.h>


typedef struct
{
  void  *ptr;             /* adresse du bloc      */
  size_t size;            /* taille  du bloc      */
  bool   freed;           /* adresse libérée      */
  /*-------------------------------------------------------------------*/
  /* NOUVEAU dans version 5 :                                          */
  /* champs pour récupérer les infos 'exterieures'                     */
  /* __FILE__,__func__,__LINE__ d'appel aux clônes                     */
  int    aline, fline;    /* lignes    d'appel allocation / libération */
  char  *afile,*ffile;    /* fichiers  d'appel allocation / libération */
  char  *afunc,*ffunc;    /* fonctions d'appel de liberation           */
  /*-------------------------------------------------------------------*/
} gfl_mtcell;

typedef struct
{
  bool        active;        /* traceur on/off         */
  size_t      tabsize;       /* taille de depart       */
  gfl_mtcell* tabcell;       /* la table des cellules  */
  gfl_mtcell* tabend;        /* 1° case libre          */
  int         nbcell;        /* nombre de cellules     */
  /* compteurs d'appel                                 */
  size_t      malloc_count;  /* appels malloc          */
  size_t      calloc_count;  /* appels calloc          */
  size_t      realloc_count; /* appels realloc         */
  size_t      free_count_t;  /* free renvoie true      */
  size_t      free_count_f;  /* free renvoie false     */
  /* données statistiques globales                     */
  size_t      total_a,total_f;
  /*-------------------------------------------------*/
  char*       callfile;
  char*       callfunc;
  int         callline;
  /*-------------------------------------------------*/
 } gfl_mtenv;

/*!******************************************!*/
/*! GESTION DE L'ENVIRONNEMENT               !*/
/*!******************************************!*/

/* LA variable globale indispensable : l'environnement *
 * elle doit être déclarée ici, avant la définition    *
 * des fonctions qui s'en servent                      *
 * il faut prendre soin de l'initialiser à 0 pour être *
 * sûr que rien ne traîne à l'interieur                */
static gfl_mtenv GFL_MTENV = {0};

void  gfl_mtfree  (void* , char*, const char*, int);

static bool _GFL_MTVERB_=false;

extern void gfl_SetMTVerbose(bool flag) { _GFL_MTVERB_=flag; }
extern bool gfl_GetMTVerbose(void)      { return _GFL_MTVERB_; }

/*
static void gfl_mtprint_cell(gfl_mtcell* c)
{
  fprintf(stderr,"@:%p [%zu] [%d] alloc[%s|%02d|%s] free[%s|%02d|%s]\n"
                ,c->ptr, c->size,c->freed,c->afile,c->aline,c->afunc,c->ffile,c->fline,c->ffunc);
}
*/

/*!================= ========================!*/
/*!  bilan final en sortie
 *!==========================================!*/
static void gfl_mtresume(void)
{
  fprintf(stderr,"\n----------------------------------------------\n");
  fprintf(stderr,"FINAL REVIEW\n");
  fprintf(stderr,"allocated memory : %ld octets\n",GFL_MTENV.total_a);
  fprintf(stderr,"freed     memory : %ld octets\n",GFL_MTENV.total_f);
  fprintf(stderr,"ratio            : %.0f%%   \n",(GFL_MTENV.total_f*100.f)/GFL_MTENV.total_a);
  fprintf(stderr,"<malloc>         : %ld calls\n",GFL_MTENV.malloc_count);
  fprintf(stderr,"<calloc>         : %ld calls\n",GFL_MTENV.calloc_count);
  fprintf(stderr,"<free>           : %ld correct calls\n",GFL_MTENV.free_count_t);
  fprintf(stderr,"                 : %ld incorrect calls\n",GFL_MTENV.free_count_f);
  fprintf(stderr,"----------------------------------------------\n");
  // for (gfl_mtcell* c=GFL_MTENV.tabcell; c<GFL_MTENV.tabend; c++) gfl_mtprint_cell(c);
}

/*!==========================================!*/
/*! libération de l'environnement             *
 *!==========================================!*/
static bool gfl_mtclean_menv(void)
{
  if (GFL_MTENV.tabcell==NULL) return false;
  free(GFL_MTENV.tabcell);
  memset(&GFL_MTENV,0,sizeof(gfl_mtenv)); // pour vraiment nettoyer tout le reste
  return true;
}

/*!==========================================!*/
/*! sortie automatique : bilan et libération  *
 *! passée à <atexit> dans <gfl_mtauto_start>   *
 *!==========================================!*/
extern void gfl_mtauto_exit(void)
{
  /* vidange de la table : libération de tou ce qui traine */
  for (gfl_mtcell* c=GFL_MTENV.tabcell; c<GFL_MTENV.tabend; c++)
  {
    if (c->freed) continue; /* déjà libérée */
    gfl_mtfree(c->ptr,__FILE__,__func__,__LINE__);
    c->ptr   = NULL;
    c->freed = true;
  }
  if (_GFL_MTVERB_) gfl_mtresume();
  gfl_mtclean_menv();
}

/*!==========================================!*/
/*! initialisation de l'environnement         *
 *!==========================================!*/
static bool gfl_mtinit_menv(void)
{
  GFL_MTENV.tabsize = 32;              // taille de départ
  if (!(GFL_MTENV.tabcell=(gfl_mtcell*)calloc(GFL_MTENV.tabsize,sizeof(gfl_mtcell))))
    gfl_error_("\e[33mError initializing L.U.T.\n");

  GFL_MTENV.tabend=GFL_MTENV.tabcell;
  return true;
}

/*!==========================================!*/
/*! démarrage automatique                     *
 *  -----------------------------------------
 * -  elle initialise le traceur au 1° passage
 *    et en profite pour passer la fct de sortie
 *    à <atexit> [ATTENTION : à faire 1 seule fois]
 * *!==========================================!*/
static bool gfl_mtauto_start(void)
{
  /* si déjà activé : on ne fait rien */
  if (GFL_MTENV.active==true) return true;
  /* sinon on active                  */
  GFL_MTENV.active = true;
  if (_GFL_MTVERB_)
  {
    fprintf(stderr,"\e[0m");
    fprintf(stderr,"-----------------------------\n");
    fprintf(stderr,"-- activation automatique -- \n");
    fprintf(stderr,"-----------------------------\n");
  }
  /* la procédure de sortie */
  atexit(gfl_mtauto_exit);
  return gfl_mtinit_menv();
}

/*!==========================================!*/
/*! recherche une adresse dans la table.      *
 *  la cellule est renvoyee par adresse       *
 *  retour :                                  *
 *  0 : le ptr. n'est pas trouvee             *
 *      et found_cell==NULL                   *
 * +1 : tout c'est bien passe                 *
 * -1 : le ptr. a été trouvé, mais l'adresse  *
 *      a déjà été libérée (double free)      *
 *!==========================================!*/
static int gfl_mtfind_cell(void* ptr, gfl_mtcell** found)
{
 *found=NULL;
  if (!ptr || !GFL_MTENV.tabcell)
    return 0;                 /*  0: l'adresse d'entree est NULL, ou la table est vide */

  for (gfl_mtcell* c=GFL_MTENV.tabcell; c<GFL_MTENV.tabend; c++)
  {
    if (c->ptr==ptr)
    {
     *found=c;
      return (c->freed?-1:1);  /* -1: l'adresse d'entree a deja ete liberee             */
    }                          /* +1: l'adresse d'entree a ete trouvee dans la table    */
  }
  return 0;                    /*  0: l'adresse d'entree n'a pas ete trouvee            */
}

/*!============================================!*/
/*! ajoute/recycle une adresse dans la table    *
 * si la table est vide ou si un bloc est plein *
 * pour l'instant on désactive                  *
 * et la 1ere case libre est (re)positionnee    *
 * -> utilisé par {m|c|re}alloc                 *
 *!============================================!*/
static bool gfl_mtnew_cell(void* ptr, size_t size)
{
  gfl_mtcell* c=NULL;

  if (0==gfl_mtfind_cell(ptr,&c))  /* si l'adresse n'a pas ete trouvee */
  {
    /* si la table est pleine : on désactive et on s'arrête */
    if (GFL_MTENV.nbcell>=GFL_MTENV.tabsize)
    {
      GFL_MTENV.active = false;
      gfl_error_("L.U.T. full -> deactivates\n");
    }
    /* sinon on incrémente -> une cellule de plus */
    c=GFL_MTENV.tabend;
    GFL_MTENV.tabend++;
    GFL_MTENV.nbcell++;
  }
  /* dans tous les cas, on rempli la case selectionnee */
  c->ptr =ptr;
  c->size=size;
  c->freed =false;
  /* fichier/fonction/ligne d'appel */
  c->afile = GFL_MTENV.callfile;
  c->afunc = GFL_MTENV.callfunc;
  c->aline = GFL_MTENV.callline;
  /* statistiques */
  if (ptr) GFL_MTENV.total_a += size;
  return true;
}

#define _MT_LINE_OF_FILE_ (fprintf(stderr,"\e[34m in file <%s> func <%s> line <%03d>  -- \e[0m"\
                                         ,GFL_MTENV.callfile,GFL_MTENV.callfunc,GFL_MTENV.callline))

static void gfl_mtblank(int n)
{
  for (int i=0;i<n;i++) fprintf(stderr," ");
}

static void gfl_mtprint_context(char* funcname, int call)
{
  static char* callfile=NULL;
  static char* callfunc=NULL;

  if (strncmp(GFL_MTENV.callfunc,"gfl_",4)==0) fprintf(stderr,"\e[31m");
  else                                         fprintf(stderr,"\e[32m");
  if (callfile != GFL_MTENV.callfile)
  {
    callfile = GFL_MTENV.callfile;
    fprintf(stderr,"| %-32s",callfile);
  }
  else gfl_mtblank(34);
  if (callfunc != GFL_MTENV.callfunc)
  {
    callfunc = GFL_MTENV.callfunc;
    fprintf(stderr,"| %-32s",callfunc);
  }
  else gfl_mtblank(34);
  fprintf(stderr,"| l.%03d | #%02d | %s"
                ,GFL_MTENV.callline,call,funcname);
}


/*!**********************************************!*/
/*! FONCTIONS CLONES : SEULES FONCTIONS EXTERNES !*/
/*!**********************************************!*/


/*!==========================================!*/
/*! malloc                                    *
 *!==========================================!*/
extern void* gfl_mtmalloc(size_t size, char* _file_, const char* _func_, int _line_)
{
  /* activation auto au 1° appel                                  */
  /* si l'initialisation échoue : comportement normal de <malloc> */
  if (gfl_mtauto_start()==false) { void *ptr=malloc(size); return ptr; }

  /* transmission du contexte */
  GFL_MTENV.callfile = _file_;
  GFL_MTENV.callfunc = (char*)_func_; /* cast pour faire taire un warning */
  GFL_MTENV.callline = _line_;

  void *ptr=malloc(size);
  GFL_MTENV.malloc_count++;

  if (!ptr) { gfl_mtprint_context("malloc",(int)(GFL_MTENV.malloc_count));
              fprintf(stderr,"(%zu) --> echec\e[0m\n",size);
              return NULL;
            }
  if (!gfl_mtnew_cell(ptr,size)) // table pleine, traceur désactivé
    return ptr;
  if (_GFL_MTVERB_) 
  { 
    gfl_mtprint_context("malloc",(int)(GFL_MTENV.malloc_count));
    fprintf(stderr,"(%zu)->%p\e[0m\n",size,ptr);
  }
  return ptr;
}

/*!==========================================!*/
/*! calloc                                    *
 *!==========================================!*/
extern void* gfl_mtcalloc(size_t nmemb, size_t size, char* _file_, const char* _func_, int _line_)
{
  /* activation auto au 1° appel                                  */
  /* si l'initialisation échoue : comportement normal de <malloc> */
  if (gfl_mtauto_start()==false) { void *ptr=calloc(nmemb,size); return ptr; }

  /* transmission du contexte */
  GFL_MTENV.callfile = _file_;
  GFL_MTENV.callfunc = (char*)_func_; /* cast pour faire taire un warning */
  GFL_MTENV.callline = _line_;

  void *ptr=calloc(nmemb,size);
  GFL_MTENV.calloc_count++;

  if (!ptr) { gfl_mtprint_context("calloc",(int)(GFL_MTENV.calloc_count));
              fprintf(stderr,"(%zu) --> fail\e[0m\n",size*nmemb);
              return NULL;
            }
  if (!gfl_mtnew_cell(ptr,size*nmemb)) // table pleine, traceur désactivé
    return ptr;
  if (_GFL_MTVERB_) 
  {
    gfl_mtprint_context("calloc",(int)(GFL_MTENV.calloc_count));
    fprintf(stderr,"(%zu)->%p\e[0m\n",size*nmemb,ptr);
  }
  return ptr;
}

/*!==========================================!*/
/*! calloc                                    *
 *!==========================================!*/
extern void* gfl_mtrealloc(void * ptr, size_t size, char* _file_, const char* _func_, int _line_)
{
  /* activation auto au 1° appel                                  */
  /* si l'initialisation échoue : comportement normal de <malloc> */
  if (gfl_mtauto_start()==false) { return ptr=realloc(ptr,size); }

  /* transmission du contexte */
  GFL_MTENV.callfile = _file_;
  GFL_MTENV.callfunc = (char*)_func_; /* cast pour faire taire un warning */
  GFL_MTENV.callline = _line_;

  gfl_mtcell *c=NULL;
  switch (gfl_mtfind_cell(ptr,&c))
  {
    case -1 :  /* l'adresse a deja ete liberee */
      GFL_MTENV.free_count_f++;
      fprintf(stderr,"(%p) \e[1;31m ERROR illegal address\e[0m\n",ptr);
      return NULL;
    case +1 :
      GFL_MTENV.total_a -=c->size;
      break;
    default : /* l'adresse existe et le bloc peut être realloue */
      break;
  }
  ptr=realloc(ptr,size);
  GFL_MTENV.realloc_count++;

  if (!ptr) { gfl_mtprint_context("realloc",(int)(GFL_MTENV.calloc_count));
              fprintf(stderr,"(%zu) --> fail\e[0m\n",size);
              return NULL;
            }
  if (!gfl_mtnew_cell(ptr,size)) // table pleine, traceur désactivé
    return ptr;
  if (_GFL_MTVERB_) 
  {
    gfl_mtprint_context("realloc",(int)(GFL_MTENV.realloc_count));
    if (c) fprintf(stderr,"%p (%zu) -> %p (%zu)\e[0m\n",c->ptr,c->size,ptr,size);
    else   fprintf(stderr," NULL -> %p (%zu)\e[0m\n",ptr,size);
  }
  return ptr;
}

/*!==========================================!*/
/*! free                                      *
 *!==========================================!*/
extern void gfl_mtfree(void* ptr, char* _file_, const char* _func_, int _line_)
{
  /* activation auto au 1° appel                                  */
  /* si l'initialisation échoue : comportement normal de <malloc> */
  if (gfl_mtauto_start()==false) { free(ptr); return; }

  /* transmission du contexte */
  GFL_MTENV.callfile = _file_;
  GFL_MTENV.callfunc = (char*)_func_; /* cast pour faire taire un warning */
  GFL_MTENV.callline = _line_;

  int  n = (int)(GFL_MTENV.free_count_t+GFL_MTENV.free_count_f);
  if (_GFL_MTVERB_) gfl_mtprint_context("free",n+1);
  if (ptr==NULL)  /* free(NULL) */
  {
    GFL_MTENV.free_count_f++;
    fprintf(stderr,"(%p) \e[1;31m ERROR free(NULL)\e[0m\n" ,ptr);
    return;
  }
  gfl_mtcell *c=NULL;
  switch (gfl_mtfind_cell(ptr,&c))
  {
    case  0 : /* l'adresse n'existe pas dans la table : free illegal */
      GFL_MTENV.free_count_f++;
      fprintf(stderr,"(%p) \e[1;31m ERROR illegal address\e[0m\n",ptr);
      return;
    case -1 :  /* l'adresse a deja ete liberee */
      GFL_MTENV.free_count_f++;
      fprintf(stderr,"(%p) \e[1;31m ERROR double free\e[0m\n",ptr);
      return;
  }
  /* cas general : l'adresse existe et le bloc peut être libere */
  GFL_MTENV.total_f+=c->size;

  /* fichier/fonction/ligne d'appel */
  c->ffile = GFL_MTENV.callfile;
  c->ffunc = GFL_MTENV.callfunc;
  c->fline = GFL_MTENV.callline;
  c->freed = true; /* la cellule est marquee "liberee" */

  GFL_MTENV.free_count_t++;
  if (_GFL_MTVERB_) fprintf(stderr,"(%p)\e[0m\n",ptr);
  free(ptr);
}
