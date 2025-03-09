/*!===============================================================
  E.Incerti - Universite Gustave Eiffel - eric.incerti@univ-eiffel.fr
       - Librairie GFL - Fonctions de base d'acces public -
                    version 6e - Jan.2022
  ================================================================!*/

#include <gfl_control.h>
/**@brief Scroll-bars (horizontal ou vertical)
 * permet de faire varier un paramètre (var. globale) dans un intervalle [min,max]    */
typedef struct
{
  double  *dprm;         /**@brief pointer to a global var <double>                    */
  int     *iprm;         /**@brief pointer to a global var <int>                       */
  double   cursor;       /**@brief value / position of the cursor (in [0,1])           */
  double   min,max;      /**@brief min/max values for the cursor run (min<cursor<max)  */
  double   factor;       /**@brief scale factor for display                            */
  int      xcurs,ycurs;  /**@brief position of the cursor in the window                */
  int      w;            /**@brief width on display <xdialwin> or <ydialwin>           */
  int      id;           /**@brief scroll id                                           */
  GFLcolor BkGd;         /**@brief background color the slide bar                      */
  char     name[7];      /**@brief name on display                                     */
  char  info[_GFL_INFOSIZE_+1];/**@brief text for information pannel (key '?')   */
} GFLscroll;

#define _SCROLLWIDTH_ 6
static int         _scrollwidth_ = _SCROLLWIDTH_;
static int         _scrollid_=0;

static int         _scrollhnum_=0;
static GFLscroll *_SCROLLH_=NULL,*TheScrollh=NULL;
static double      Xscrollrange;
static GLint       Xscrollend  ;
static GLint       Yscrollpos  = 8;
static GLint       Xscrollstart=72;

static int         _scrollvnum_=0;
static GFLscroll *_SCROLLV_=NULL, *TheScrollv=NULL;
static double      Yscrollrange;
static GLint       Yscrollend;
static GLint       Xscrollpos  =12;
static GLint       Yscrollstart=32;

static GFLcolor    deftbkg = (GFLcolor){0.7,0.9,0.8,0.5};

/*================================================*/
/*=                                              =*/
extern void gfl_SetScrollWidth(int width)
{
  _scrollwidth_ = CLIP(_SCROLLWIDTH_/2,width,_SCROLLWIDTH_*2);
}

/*================================================*/
/*=       RECUPERE LA POSITION DU SCROLL id      =*/
extern double gfl_GetScrollCursor(int id)
{
  GFLscroll *scroll=NULL;

  scroll=_SCROLLH_;
  while (scroll<_SCROLLH_+_scrollhnum_)
  {
    if (scroll->id==id)
    { return scroll->min + scroll->cursor*(scroll->max-scroll->min); }
    scroll++;
  }
  scroll=_SCROLLV_;
  while (scroll<_SCROLLV_+_scrollvnum_)
  {
    if (scroll->id==id)
    { return scroll->min + scroll->cursor*(scroll->max-scroll->min); }
    scroll++;
  }
  gfl_error_(" Scroll [%d] Non Identifie\e[0m (retour 0.0)\n",id);
  return 0.;
}

/*================================================*/
/*=       AJUSTE LA COULEUR DU SCROLL id      =*/
extern void gfl_SetScrollColor(int id, GFLcolor col)
{
  GFLscroll *scroll=NULL;

  scroll=_SCROLLH_;
  while (scroll<_SCROLLH_+_scrollhnum_)
  {
    if (scroll->id==id) { scroll->BkGd = col; return; }
    scroll++;
  }
  scroll=_SCROLLV_;
  while (scroll<_SCROLLV_+_scrollvnum_)
  {
    if (scroll->id==id) { scroll->BkGd = col; return; }
    scroll++;
  }
  gfl_error_(" Scroll [%d] Non Identifie\e[0m (retour 0.0)\n",id);
  return;
}


/*================================================*/
/*=               SCROLL HORIZONTAL              =*/
static void gfl_DrawScrollh(GFLscroll *scroll)
{
  int x1,x2,y1,y2;
  if (!scroll) return;
  glEnable(GL_BLEND);
  glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  /* BANDE DE DEFILEMENT */
  x1=(Xscrollstart -scroll->w);
  y2=(scroll->ycurs-scroll->w);
  x2=(Xscrollend   +scroll->w);
  y1=(scroll->ycurs+scroll->w);
  glPolygonMode(GL_FRONT,GL_LINE);
  glLineWidth(1);
  gfl_Color4fv(GFLk_c);
  glBegin(GL_QUADS);
    glVertex2i(x1,y1);
    glVertex2i(x1,y2);
    glVertex2i(x2,y2);
    glVertex2i(x2,y1);
  glEnd();
  glPolygonMode(GL_FRONT,GL_FILL);
  gfl_Color4fv(scroll->BkGd);
  glBegin(GL_QUADS);
    glVertex2i(x1,y1);
    glVertex2i(x1,y2);
    glVertex2i(x2,y2);
    glVertex2i(x2,y1);
  glEnd();
  /* CURSEUR */
  x1=(scroll->xcurs-scroll->w);
  y1=(scroll->ycurs-scroll->w);
  x2=(scroll->xcurs+scroll->w);
  y2=(scroll->ycurs+scroll->w);
  gfl_Color4fv(GFLba_c);
  glBegin(GL_QUADS);
    glVertex2i(x1,y1);
    glVertex2i(x1,y2);
    glVertex2i(x2,y2);
    glVertex2i(x2,y1);
  glEnd();
  
  /* nom et facteur d'échelle */
  glColor4f(GFLwd.r,GFLwd.g,GFLwd.b,1.-GFLwd.a);
  glRasterPos2i((4),(scroll->ycurs-4));
  glutBitmapString(GLUT_BITMAP_HELVETICA_10,(const uchar*)scroll->name);
  /* curseur */
  glRasterPos2i((scroll->xcurs+scroll->w+2),(scroll->ycurs-4));
  char str[16]="";
       if (scroll->dprm) sprintf(str,"%.2e",*scroll->dprm);
  else if (scroll->iprm) sprintf(str,"%d"   ,*scroll->iprm); 
  else                   sprintf(str,"%.2e",scroll->min + scroll->cursor*(scroll->max-scroll->min));
  glutBitmapString(GLUT_BITMAP_HELVETICA_10,(const uchar*)str);
}

extern int gfl_CreateScrollh_d(const char *name, double* prm, double min, double max, const char *info)
{
  GFLscroll *scroll=NULL,*s;
  if (_scrollhnum_>=_GFL_BLOCSIZE_) { gfl_error_(" NOMBRE MAX (%d)ATTEINT retour -1\e[0;0m\n",_scrollhnum_); return -1;}
  if (!_SCROLLH_)
    if (!(_SCROLLH_=(GFLscroll*)calloc(_GFL_BLOCSIZE_,sizeof(GFLscroll)))) 
    { gfl_error_("  ECHEC CREATION retour -1\e[0;0m\n"); return -1; }
  TheScrollh = scroll = _SCROLLH_+_scrollhnum_;
  _scrollhnum_++;
  strncpy(scroll->name,name,6);
  if (info) strncpy(scroll->info,info,127);
  scroll->dprm   = prm;
  scroll->iprm   = NULL;

  int   len=12;
  if (len>Xscrollstart)
  {
    Xscrollstart = len;
    Xscrollrange = Xscrollend-Xscrollstart;
    s=_SCROLLH_;
    while (s<scroll) { s->xcurs  = (int)(Xscrollstart+s->cursor*Xscrollrange); s++; }
  }
  scroll->xcurs  = (int)(Xscrollstart+scroll->cursor*Xscrollrange);
  scroll->ycurs  = Yscrollpos;
  scroll->cursor = (double)(*prm-min)/(double)(max-min);
  scroll->min    = (double)min;
  scroll->max    = (double)max;
  double flen = (max-min);
  scroll->factor = 1.;
       if (flen<    1.0) while ((flen*scroll->factor)<= 10.) scroll->factor*=10.0;
  else if (flen>=1000.0) while ((flen*scroll->factor)>=  1.) scroll->factor*= 0.1;
  scroll->w      = _scrollwidth_;
  scroll->BkGd    = deftbkg;
  /* position en y du prochain scroll */
  Yscrollpos += 2*scroll->w+4;
  Xdialheight  = MAX(Xdialheight,Yscrollpos-scroll->w);
  
  return (scroll->id = _scrollid_++) ;
}

extern int gfl_CreateScrollh_i(const char *name, int* prm, int min, int max, const char *info)
{
  GFLscroll *scroll=NULL,*s;  
  if (_scrollhnum_>=_GFL_BLOCSIZE_) { gfl_error_(" NOMBRE MAX (%d)ATTEINT retour -1\e[0;0m\n",_scrollhnum_); return -1;}
  if (!_SCROLLH_)
    if (!(_SCROLLH_=(GFLscroll*)calloc(_GFL_BLOCSIZE_,sizeof(GFLscroll)))) 
    { gfl_error_("  ECHEC CREATION retour -1\e[0;0m\n"); return -1; }
  TheScrollh = scroll = _SCROLLH_+_scrollhnum_;
  _scrollhnum_++;
  strncpy(scroll->name,name,6);
  if (info) strncpy(scroll->info,info,127);
  scroll->dprm   = NULL;
  scroll->iprm   = prm;

  int   len=12;
  if (len>Xscrollstart)
  {
    Xscrollstart = len;
    Xscrollrange = Xscrollend-Xscrollstart;
    s=_SCROLLH_;
    while (s<scroll) { s->xcurs  = (int)(Xscrollstart+s->cursor*Xscrollrange); s++; }
  }
  scroll->xcurs  = (int)(Xscrollstart+scroll->cursor*Xscrollrange);
  scroll->ycurs  = Yscrollpos;
  scroll->cursor = (double)(*prm-min)/(double)(max+0.99-min);
  scroll->min    = (double)min;
  scroll->max    = (double)max+0.99;
  double flen = (max-min);
  scroll->factor = 1.;
  if (flen>10000.) while ((flen*scroll->factor)>=1.) scroll->factor*=0.1;
  scroll->w      = _scrollwidth_;
  scroll->BkGd   = deftbkg;
  /* position en y du prochain scroll */
  Yscrollpos += 2*scroll->w+4;
  Xdialheight  = MAX(Xdialheight,Yscrollpos-scroll->w);
  
  return (scroll->id=_scrollid_++);
}

extern int gfl_CreateAnonymousScrollh(const char *name, double init, const char *info)
{
  GFLscroll *scroll=NULL,*s;
  if (_scrollhnum_>=_GFL_BLOCSIZE_) { gfl_error_(" NOMBRE MAX (%d)ATTEINT retour -1\e[0;0m\n",_scrollhnum_); return -1;}
  if (!_SCROLLH_)
    if (!(_SCROLLH_=(GFLscroll*)calloc(_GFL_BLOCSIZE_,sizeof(GFLscroll)))) 
    { gfl_error_("  ECHEC CREATION retour -1\e[0;0m\n"); return -1; }
  TheScrollh = scroll = _SCROLLH_+_scrollhnum_;
  _scrollhnum_++;
  strncpy(scroll->name,name,6);
  if (info) strncpy(scroll->info,info,127);
  scroll->dprm   = NULL;
  scroll->iprm   = NULL;
  int   len=12;
  if (len>Xscrollstart)
  {
    Xscrollstart = len;
    Xscrollrange = Xscrollend-Xscrollstart;
    s=_SCROLLH_;
    while (s<scroll) { s->xcurs  = (int)(Xscrollstart+s->cursor*Xscrollrange); s++; }
  }
  scroll->xcurs  = (int)(Xscrollstart+scroll->cursor*Xscrollrange);
  scroll->ycurs  = Yscrollpos;
  scroll->cursor = CLIP(0.,init,1.);
  scroll->min    = 0.;
  scroll->max    = 1.;
  scroll->w      = _scrollwidth_;
  scroll->BkGd    = deftbkg;
  /* position en y du prochain scroll */
  Yscrollpos += 2*scroll->w+4;
  Xdialheight  = MAX(Xdialheight,Yscrollpos-scroll->w);

  return (scroll->id = _scrollid_++);
}

/*================================================*/
/*=                SCROLL VERTICAL               =*/
static __inline__ void gfl_DrawScrollv(GFLscroll *scroll)
{
  int x1,x2,y1,y2;
  if (!scroll) return;
  glEnable(GL_BLEND);
  glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  /* BANDE DE DEFILEMENT */
  x1=(scroll->xcurs-scroll->w);
  y1=(Yscrollend   +scroll->w);
  x2=(scroll->xcurs+scroll->w);
  y2=(Yscrollstart -scroll->w);
  glPolygonMode(GL_FRONT,GL_LINE);
  glLineWidth(1);
  gfl_Color4fv(GFLwb_c);
  glBegin(GL_QUADS);
    glVertex2d(x1,y1);
    glVertex2d(x1,y2);
    glVertex2d(x2,y2);
    glVertex2d(x2,y1);
  glEnd();
  glPolygonMode(GL_FRONT,GL_FILL);
  gfl_Color4fv(scroll->BkGd);
  glBegin(GL_QUADS);
    glVertex2d(x1,y1);
    glVertex2d(x1,y2);
    glVertex2d(x2,y2);
    glVertex2d(x2,y1);
  glEnd();
  /* CURSEUR */
  x1=(scroll->xcurs-scroll->w);
  y1=(scroll->ycurs+scroll->w);
  x2=(scroll->xcurs+scroll->w);
  y2=(scroll->ycurs-scroll->w);
  gfl_Color4fv(GFLba_c);
  glBegin(GL_QUADS);
    glVertex2d(x1,y1);
    glVertex2d(x1,y2);
    glVertex2d(x2,y2);
    glVertex2d(x2,y1);
  glEnd();
  
  /* nom et facteur d'échelle */
  glColor4f(GFLwd.r,GFLwd.g,GFLwd.b,1.-GFLwd.a);
  int len=glutBitmapLength(GLUT_BITMAP_HELVETICA_10,(const uchar*)scroll->name);
  glRasterPos2d((scroll->xcurs-0.5*len),(Yscrollstart-18));
  glutBitmapString(GLUT_BITMAP_HELVETICA_10,(const uchar*)scroll->name);
  /* curseur */
  char str[16]="";
       if (scroll->dprm) sprintf(str,"%.2e",*scroll->dprm);
  else if (scroll->iprm) sprintf(str,"%d",*scroll->iprm); 
  else                   sprintf(str,"%.2e",scroll->min + scroll->cursor*(scroll->max-scroll->min));
  len=glutBitmapLength(GLUT_BITMAP_HELVETICA_10,(const uchar*)str);
  glRasterPos2d((scroll->xcurs-0.5*len), (scroll->ycurs+scroll->w+2));
  glutBitmapString(GLUT_BITMAP_HELVETICA_10,(const uchar*)str);
}

extern int gfl_CreateScrollv_d(const char *name, double* prm, double min, double max, const char *info)
{
  GFLscroll *scroll=NULL;
  if (_scrollvnum_>=_GFL_BLOCSIZE_) { gfl_error_(" NOMBRE MAX (%d)ATTEINT retour -1\e[0;0m\n",_scrollvnum_); return -1;}
  if (!_SCROLLV_)
    if (!(_SCROLLV_=(GFLscroll*)calloc(_GFL_BLOCSIZE_,sizeof(GFLscroll)))) 
    { gfl_error_("  ECHEC CREATION retour -1\e[0;0m\n"); return -1; }

  TheScrollv = scroll = _SCROLLV_+_scrollvnum_;
  _scrollvnum_++;
  strncpy(scroll->name,name,6);
  if (info) strncpy(scroll->info,info,127);
  scroll->dprm   = prm;
  scroll->iprm   = NULL;
  scroll->xcurs  = Xscrollpos;
  scroll->ycurs  = Yscrollstart;
  scroll->cursor = (double)(*prm-min)/(double)(max-min);
  scroll->min    = (double)min;
  scroll->max    = (double)max;
  double flen = (max-min);
  scroll->factor = 1.;
       if (flen<    1.0) while ((flen*scroll->factor)<=   1.) scroll->factor*=10.;
  else if (flen>=1000.0) while ((flen*scroll->factor)>= 100.) scroll->factor*=0.1;
  scroll->w      = _scrollwidth_;
  scroll->BkGd    = deftbkg;
  /* position en y du prochain scroll */
  Xscrollpos += 2*scroll->w+4;
  Ydialwidth   = MAX(Ydialwidth,Xscrollpos-scroll->w+4);
  /* POUR LES BOUTONS */
  Xbutpos      = Ydialwidth/2;
  Xbutw        = Ydialwidth/2-2;
  
  return (scroll->id = _scrollid_++);
}

extern int gfl_CreateScrollv_i(const char *name, int* prm, int min, int max, const char *info)
{
  GFLscroll *scroll=NULL;
  if (_scrollvnum_>=_GFL_BLOCSIZE_) { gfl_error_(" NOMBRE MAX (%d)ATTEINT retour -1\e[0;0m\n",_scrollvnum_); return -1;}
  if (!_SCROLLV_)
    if (!(_SCROLLV_=(GFLscroll*)calloc(_GFL_BLOCSIZE_,sizeof(GFLscroll)))) 
    { gfl_error_("  ECHEC CREATION retour -1\e[0;0m\n"); return -1; }

  TheScrollv = scroll = _SCROLLV_+_scrollvnum_;
  _scrollvnum_++;
  strncpy(scroll->name,name,6);
  if (info) strncpy(scroll->info,info,127);
  scroll->dprm   = NULL;
  scroll->iprm   = prm;
  scroll->xcurs  = Xscrollpos;
  scroll->ycurs  = Yscrollstart;
  scroll->cursor = (double)(*prm-min)/(double)(max+0.99-min);
  scroll->min    = (double)min;
  scroll->max    = (double)max+0.99;
  double flen = (max-min);
  scroll->factor = 1.;
  if (flen>=1000.) while ((flen*scroll->factor)>= 100.) scroll->factor*=0.1;
  scroll->w      = _scrollwidth_;
  scroll->BkGd   = deftbkg;
  /* position en y du prochain scroll */
  Xscrollpos += 2*scroll->w+4;
  Ydialwidth   = MAX(Ydialwidth,Xscrollpos-scroll->w+4);
  /* POUR LES BOUTONS */
  Xbutpos      = Ydialwidth/2;
  Xbutw        = Ydialwidth/2-2;
  return (scroll->id = _scrollid_++);
}

extern int gfl_CreateAnonymousScrollv(const char *name, double init, const char *info)
{
  GFLscroll *scroll=NULL;
  if (_scrollvnum_>=_GFL_BLOCSIZE_) { gfl_error_(" NOMBRE MAX (%d)ATTEINT retour -1\e[0;0m\n",_scrollvnum_); return -1;}
  if (!_SCROLLV_)
    if (!(_SCROLLV_=(GFLscroll*)calloc(_GFL_BLOCSIZE_,sizeof(GFLscroll)))) 
    { gfl_error_("  ECHEC CREATION retour -1\e[0;0m\n"); return -1; }

  TheScrollv = scroll = _SCROLLV_+_scrollvnum_;
  _scrollvnum_++;
  strncpy(scroll->name,name,6);
  if (info) strncpy(scroll->info,info,127);
  scroll->dprm   = NULL;
  scroll->iprm   = NULL;
  scroll->xcurs  = Xscrollpos;
  scroll->ycurs  = Yscrollstart;
  scroll->cursor = CLIP(0.,init,1.);
  scroll->min    = 0.;
  scroll->max    = 1.;
  scroll->w      = _scrollwidth_;
  scroll->BkGd    = deftbkg;
  /* position en y du prochain scroll */
  Xscrollpos += 2*scroll->w+4;
  Ydialwidth   = MAX(Ydialwidth,Xscrollpos-scroll->w+4);
  /* POUR LES BOUTONS */
  Xbutpos      = Ydialwidth/2;
  Xbutw        = Ydialwidth/2-2;

  return (scroll->id = _scrollid_++);
}

/*================================================*/
/*=         REDIMENSIONNEMENT HORIZONTAL         =*/
static __inline__ void gfl_SetUpScrollh(void)
{
  Xscrollend   = curwidth - _scrollwidth_;
  Xscrollrange = Xscrollend-Xscrollstart;

  GFLscroll *scroll=_SCROLLH_;
  while (scroll<_SCROLLH_+_scrollhnum_)
  {
    scroll->xcurs  = (int)(Xscrollstart+scroll->cursor*Xscrollrange);
    scroll++;
  }
}

/*================================================*/
/*=         REDIMENSIONNEMENT VERTICTAL          =*/
static __inline__ void gfl_SetUpScrollv(void)
{
  Yscrollend   = curheight - Ybutpos - _scrollwidth_;
  Yscrollrange = Yscrollend-Yscrollstart;
  GFLscroll *scroll=_SCROLLV_;
  while (scroll<_SCROLLV_+_scrollvnum_)
  {
    scroll->ycurs  = (int)(Yscrollstart+scroll->cursor*Yscrollrange);
    scroll++;
  }
}

/*================================================*/
/*=        LIBERATION DE TOUS LES SCROLLS        =*/
/* obsolète -> garbage collector
static __inline__ void gfl_FreeScrolls()
{
  if (_SCROLLH_) { free(_SCROLLH_); TheScrollh=_SCROLLH_=NULL; }
  if (_SCROLLV_) { free(_SCROLLV_); TheScrollv=_SCROLLV_=NULL; }
}
*/
