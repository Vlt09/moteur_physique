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

/* constantes utilisées par les struct. de contrôle */
/* -> allocation par blocs                          */
#define _GFL_BLOCSIZE_  32
#define _GFL_MAXBLOC_   32
#define _GFL_NAMESIZE_  15
#define _GFL_INFOSIZE_ 127

/* tailles par defaut de la fenetre gl */
#define _GFL_DFT_PIXHEIGHT_ (GLint)1024
#define _GFL_DFT_PIXWIDTH_  (GLint) 768

/* tailles des bandeaux */
#define _GFL_DFT_DIALWIDTH_ ((GLint)12)

static GLint   xdialwin,ydialwin,cornwin,drawwin,mainwin;
static bool    xdialflag=false, ydialflag=false, cornflag=false;
static char    _WINDNAME_[128]="";

/* tailles de la fenetre : dim. en pixels  */
static GLint   curheight = _GFL_DFT_PIXHEIGHT_; /* */
static GLint   pixheight = _GFL_DFT_PIXHEIGHT_; /* */
static GLint   curwidth  = _GFL_DFT_PIXWIDTH_;  /* */
static GLint   pixwidth  = _GFL_DFT_PIXWIDTH_;  /* */
static GLint   fullwidth,fullheight;

/* PARAMETRES DU BANDEAU LATERAL : position des boutons et scrollbars */
static GLint   Ydialwidth= 0,Xdialheight=0;
static GLint   Xbutpos   = 0,Ybutpos=12;
static GLint   Xbutw     = 0;
static bool    _GFL_AXES_  = false;

/* taille réelle d'un pixel écran -- toujours 1. en 2D */
static double  Xpixsize=1.,Ypixsize=1.;

extern void   gfl_SelectDrawWin(void) { glutSetWindow(drawwin); }
extern int    gfl_GetPixWidth(void) { return curwidth;  }
extern int    gfl_GetPixHeight(void){ return curheight; }

#ifdef _GFL2D_
  /* spécifique 2D : dimensions de la feetre reelle  */
  #define GFLpixtowinX1(x) (double)(Xwmin  + (x)*Xpixsize)
  #define GFLpixtowinY1(y) (double)(Ywmin  + (y)*Ypixsize)
  static double  xdl=-1.,xur=+1.,ydl=-1.,yur=+1.;         // zone de dessin : tailles réelles initiales
  static double  Xwmin=-1.,Xwmax=+1.,Ywmin=-1.,Ywmax=+1.; // tailles réelles de la fenêtre courante
  static double  Xratio =1.,Yratio =1.;   // ratio taille courante / taille initiale
  static double  Xoffset=0.,Yoffset=0.;   // déplacements latéraux
  static double  XcurrOffset = 0, YcurrOffset = 0;// offset générique au total de tous les déplacements latéraux.
  static double zoom_value=1.,zoom_factor=1.05;  // facteur de zoom_value
  /* spécifique 2D : démarrage sur le chargement d'une image  */
  static GFLpixmap* gflrootpix=NULL;
  static char       gflrootname[1024]="";
  static bool       _PANSCAN_    = true;

//static void (*gfl_Bkgf)(void)=NULL;             /*! fonction de background    !*/

  extern void   gfl_SetZoom(double z) { zoom_value = z;   }
  extern double gfl_GetZoom(void)     { return zoom_value;}
  extern double gfl_GetXPixSize(void) { return Xpixsize;  }
  extern double gfl_GetYPixSize(void) { return Ypixsize;  }
  extern double gfl_GetXMin(void)     { return Xwmin;     }
  extern double gfl_GetXMax(void)     { return Xwmax;     }
  extern double gfl_GetYMin(void)     { return Ywmin;     }
  extern double gfl_GetYMax(void)     { return Ywmax;     }
  extern double gfl_GetXRatio(void)   { return Xratio;    }
  extern double gfl_GetYRatio(void)   { return Yratio;    }
  extern double gfl_GetXOffset(void)  { return Xoffset;   }
  extern double gfl_GetYOffset(void)  { return Yoffset;   }
  extern void   gfl_ResetOffset(void) {Xoffset = 0; Yoffset = 0; YcurrOffset = 0; XcurrOffset = 0;}
   // MODIF. 2023.02.08 : pour désactiver zoom / panscan
  extern void gfl_SetPanScanAndZoom(bool FLAG)
  {
    _PANSCAN_ = FLAG;
  }
  
  static void gfl_RescaleWindow(void)
  {  // MODIF. 2023.02.08 : possibilité de désactiver zoom / panscan
    if (!_PANSCAN_) { zoom_value=1.; Xoffset=Yoffset=0; }
    Xratio    = curwidth /(zoom_value*pixwidth );
    Yratio    = curheight/(zoom_value*pixheight);
    Xwmin     = xdl*Xratio-Xoffset;
    Xwmax     = xur*Xratio-Xoffset;
    Ywmin     = ydl*Yratio-Yoffset;
    Ywmax     = yur*Yratio-Yoffset;
    /* taille "réelle" d'un pixel écran */
    Xpixsize  = (Xwmax-Xwmin)/curwidth ;
    Ypixsize  = (Ywmax-Ywmin)/curheight;
  }
#endif

#ifdef _GFL3D_
  static bool    _FILL_       = true ;
  #define GFLpixtowinX1(x) (double)(x)
  #define GFLpixtowinY1(y) (double)(y)
  static double  Xwmin=-1.,Ywmin=-1.; // pour gfl_StaticTextBox
#endif


/** Pas utilisé **/
//static uint    GFL_FPS = 60;
//extern void    gfl_SetFPS(uint fps) { GFL_FPS = fps; }
//extern uint    gfl_GetFPS(void    ) { return GFL_FPS; }

/*= Couleur de fond                                                  =*/
static GLfloat GFL_BKGD=1.0;
extern void    gfl_SetBkGdCol(double BkGd) { GFL_BKGD=(GLfloat)BkGd;  }
extern double  gfl_GetBkGdCol(void       ) { return (double)GFL_BKGD; }

static GLuint pushmode=GLUT_UP;
static GLint  moving;

static bool  _GFL_ANIM_        = false;
static bool  _GFL_RUN_         = false ;
static bool  _GFL_AUTO_DISPLAY_= true ;
static bool  _GFL_FULLSCREEN_  = false;
static bool  _GFL_INFO_       = false;
//static int   _BKGLIST_    = 0xFFFF;

/*= prototypes =*/
static void (*gfl_Init)(void)=NULL;             /*! fonction d'initialisation !*/
static void (*gfl_Ctrl)(void)=NULL;             /*! fonction de contrôle      !*/
static void (*gfl_Anim)(void)=NULL;             /*! fonction d'animation      !*/
static void (*gfl_Evts)(void)=NULL;             /*! fonction d'événements     !*/
static void (*gfl_Draw)(void)=NULL;             /*! fonction de dessin        !*/
static void (*gfl_Exit)(void)=NULL;             /*! fonction de sortie        !*/


/*=======================================*/
/*= FONCTIONS DE GESTION DES EVENEMENTS =*/
/*=======================================*/

/*= automatisation - rarement utilisee =*/
extern void gfl_SetAutoDisplay(bool flag) { _GFL_AUTO_DISPLAY_=flag; }
extern void gfl_ToggleAutoDisplay(void)   { _GFL_AUTO_DISPLAY_=!_GFL_AUTO_DISPLAY_; }
static int _GFL_VIDEO_ON_ = 0;
extern void gfl_AutoStartVideo(void)      { _GFL_RUN_=true; _GFL_VIDEO_ON_=1; }
static bool  _GFL_ENABLE_RTVID_ = false;
extern void gfl_EnableRealTimeVideo(void) { _GFL_ENABLE_RTVID_=true; }
extern void gfl_DisableRealTimeVideo(void) { _GFL_ENABLE_RTVID_=false; }
static bool  _GFL_RTVID_ON_ = false;
static void gfl_RealTimeVideo(void)       { gfl_PlugCapture(_WINDNAME_,0,0,curwidth,curheight); _GFL_RUN_=true; _GFL_RTVID_ON_=true; }
/*= snapshot attache a des touches ('<Ctrl+j>'/'<Ctrl+p>')  =*/
extern void gfl_AutoSnap_jpg(void) { gfl_Snapshot("jpg",_WINDNAME_,curwidth,curheight); }
extern void gfl_AutoSnap_png(void) { gfl_Snapshot("png",_WINDNAME_,curwidth,curheight); }

/*= - Fonction d'initialisation                                                      =*/
extern void gfl_SetInitFunction(void (*f)(void)) { gfl_Init=f; }
/*= - Fonction de contrôle                                                           =*/
extern void gfl_SetCtrlFunction(void (*f)(void)) { gfl_Ctrl=f; }
/*= - DrawFunc   : procedure d'affichage, associee a la fenetre de dessin 'drawwin'  =*/
extern void gfl_SetDrawFunction(void (*f)(void)) { gfl_Draw = f; }
/*= - EventFunc   : procedure de la boucle d'action Gl,                              =*/
extern void gfl_SetEvtsFunction(void (*f)(void)) { gfl_Evts=f; }
/*= - FreeFunc   : procedure de liberation de memoire, appelee a la sortie           =*/
extern void gfl_SetExitFunction(void (*f)(void)) { gfl_Exit = f; }
/*= - AnimFunc   : procedure de la boucle d'action Gl, passee a <glutTimerFunc>      =*/
extern void gfl_SetAnimFunction(void (*f)(void)) { gfl_Anim=f; _GFL_ANIM_=(f!=NULL); }


static int  _GFL_FAFF_         = 1;  /* frequence d'affichage */

#ifdef _USE_GL_IDLE_
  extern void gfl_Stop(void)     { glutIdleFunc(NULL); _GFL_RUN_=!_GFL_RUN_; }
  static uint _GFL_TEMPO_=0;
  extern void gfl_SetDelay(uint usec) { _GFL_TEMPO_=usec; }
  /* par defaut : tous les pas de calcul sont affiches */
  static void _anim_0_(void)
  {
    gfl_Anim();
    glutPostWindowRedisplay(drawwin);
    if (_GFL_TEMPO_>0) usleep(_GFL_TEMPO_);
  }
  
  /* affichage temporise                               */
  static void _anim_F_(void)
  {
    static int t=0;
    gfl_Anim();
    if (t==_GFL_FAFF_) { glutPostWindowRedisplay(drawwin); t=0; }
    t++;
  }
  
  /* regle la frequence d'affichage et selctionne la bonne <idle func>  */
  extern void gfl_SetRefreshFreq(int freq)
  {
    if (_GFL_FAFF_==freq) return;
    _GFL_FAFF_=freq;
    glutIdleFunc((_GFL_ANIM_&&_GFL_RUN_)?(_GFL_FAFF_>1?_anim_F_:_anim_0_):NULL);
  }
#else
  static int _GFL_TimerFreq_ = 0;  /* Called back when timer expired */
  extern void gfl_SetDelay(uint usec)      { _GFL_TimerFreq_=1+usec; }
  extern void gfl_SetRefreshFreq(int freq) { _GFL_FAFF_ = freq; }
  /* par defaut : tous les pas de calcul sont affiches */
  static void _anim_0_(void)
  {
    gfl_Anim();
    glutPostWindowRedisplay(drawwin);
  }
  /* affichage temporise                               */
  static void _anim_F_(void)
  {
    for (int t=0;t<_GFL_FAFF_; t++) gfl_Anim();
    glutPostWindowRedisplay(drawwin);
  }
  
  static void gfl_Timer(int f)
  {
    if (_GFL_ANIM_&&_GFL_RUN_)
    {
      if (_GFL_FAFF_>1) _anim_F_();
      else          _anim_0_();
      glutPostWindowRedisplay(drawwin);
      glutTimerFunc(_GFL_TimerFreq_, gfl_Timer, f);
    }
  }

  // intégré dans la fonction d'affichage, cet appel permet de faire de l'image par image
  // => affiche une image et s'arrête. la touche "espace" permet de passer à l'étape suivante.
  extern void gfl_Stop(void)     
  { 
    _GFL_RUN_=!_GFL_RUN_;
    // mise a jour de la fonction d'anim
    if (_GFL_ANIM_&&_GFL_RUN_)
    {
      if (_GFL_FAFF_>1) _anim_F_();
      else          _anim_0_();
    }
    glutPostWindowRedisplay(drawwin);
    glutTimerFunc(_GFL_TimerFreq_, gfl_Timer,0);
  }  
#endif


/*=================================================*/
/* Code C séparé - ne peut pas être compilé à part */
/* trop de dépendance aux variables globales       */
#ifdef _GFL2D_
  #include "_gfl_axes2d.c"     // axes et coordonnées (spécifique 2D)
  #include "_gfl_ctrlpnt2d.c"  // Gestion des points 'cliquables' (spécifique 2D)
#endif
#ifdef _GFL3D_
  #include "_gfl_camlight3d.c" // Gestion Caméra/Lumières (spécifique 3D)
#endif
#include "_gfl_keys.c"      // Gestion clavier
#include "_gfl_popup.c"     // Boutons 'popup' (action unique)
#include "_gfl_button.c"    // Boutons on/off
#include "_gfl_exclbut.c"   // groupes de boutons on/off - HP.2022.05
#include "_gfl_switch.c"    // Switch (Boutons on/off)
#include "_gfl_scroll.c"    // ScrollBars
#include "_gfl_mouse.c"     // Gestion souris
#include "_gfl_print.c"     // Gestion texte
/*=================================================*/

#ifdef _GFL3D_
  /*====================================================================*/
  /*=              trace des axes (spécifique 3D)                      =*/
  /*====================================================================*/
  // trace une flèche - Pb sur gestion des faces FRONT/BACK ?
  extern void gfl_Arrow(GLfloat r, GLfloat h, GFLcolor col)
  {
    static GLUquadricObj *qobj=NULL;
    glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
    glPushMatrix();
      if (!qobj) qobj=gluNewQuadric();
      gfl_Material(col,0.2,0.8,0.,0.,0.);
      gluQuadricDrawStyle(qobj,GLU_FILL);
      gluCylinder(qobj,r,r,.8*h,8,1);
      glTranslatef(0.,0.,.8*h);
      glutSolidCone(4.*r,.2*h,4,1);
    glPopMatrix();
    glPolygonMode(_FILL_?GL_FRONT:GL_FRONT_AND_BACK,_FILL_?GL_FILL:GL_LINE);
  }
  
  // variante
  extern void gfl_ArrowVect(GFLpoint O, GFLvector V, float r, float h, GFLcolor col)
  {
    static GLUquadricObj *qobj=NULL;
    glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
  
    double l,theta,phi,sinphi;
    l=gfl_Norm(V);
    phi=acos(V.z/l);
    sinphi=sin(phi);
    theta=(GFLISZERO(sinphi)?0.:(V.y>0?+1.:-1.)*acos(V.x/(l*sinphi)));
  
    glPushMatrix();
      glTranslatef(O.x,O.y,O.z);            /* positionnement   */
      glPushMatrix();
        glRotated(RADTODEG*theta,0.,0.,1.); /* rotation (z) */
        glPushMatrix();
          glRotated(RADTODEG*phi,0.,1.,0.); /* rotation (y) */
          glPushMatrix();
            if (!qobj) qobj=gluNewQuadric();
            gfl_Material(col,0.2,0.8,0.,0.,0.);
            gluQuadricDrawStyle(qobj,GLU_FILL);
            gluCylinder(qobj,r,r,.8*h,8,1);  /* mise à l'échelle */
            glTranslatef(0.,0.,.8*h);
            glutSolidCone(4.*r,.2*h,4,1);
          glPopMatrix();
        glPopMatrix();
      glPopMatrix();
    glPopMatrix();
  
    glPolygonMode(GL_FRONT_AND_BACK,_FILL_?GL_FILL:GL_LINE);
  }
  
  /*= affichage du repere =*/
  extern void gfl_Repere(void)
  {
    static GFLcolor xcol =(GFLcolor){1.0,0.2,0.2,0.5};
    static GFLcolor ycol =(GFLcolor){0.2,1.0,0.2,0.5};
    static GFLcolor zcol =(GFLcolor){0.3,0.3,1.0,0.5};
    glEnable(GL_LIGHTING);
    glPushMatrix();
    glTranslated(_main_cam_.tar->x,_main_cam_.tar->y,_main_cam_.tar->z);
    glPushMatrix();
      gfl_Arrow(0.025,1.0,zcol); /*axe z (bleu) */
      glPushMatrix();
        glRotatef(-90.,1.,0.,0.);
        gfl_Arrow(0.025,1.0,ycol); /* axe y (vert) */
      glPopMatrix();
      glPushMatrix();
        glRotatef(+90.,0.,1.,0.);
        gfl_Arrow(0.025,1.0,xcol); /* axe x (rouge) */
      glPopMatrix();
    glPopMatrix();
    glPopMatrix();
  }
#endif

/*==================================*/
/*=  RESHAPE                       =*/
/*==================================*/
static  void gfl_CornReshape(int w, int h)
{
  glViewport (0, 0, w, h);
}

static  void gfl_XDialReshape(int w, int h)
{
  glViewport (0, 0, w, h);
  if (_SCROLLH_) gfl_SetUpScrollh();
}

static void gfl_YDialReshape(int w, int h)
{
  glViewport (0, 0, w, h);
  if (_SCROLLV_) gfl_SetUpScrollv();
}

static void gfl_DrawReshape(int w, int h)
{
  glViewport (0, 0, w, h);
  #ifdef _GFL3D_
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(_main_cam_.near,(GLfloat)w/(GLfloat)h,_main_cam_.open,_main_cam_.far );
  #endif
}

static void gfl_MainReshape(int w, int h)
{
  glViewport (0, 0, w, h);
  curwidth  = w-Ydialwidth;
  curheight = h-Xdialheight;

  if (cornflag)
  {
    glutSetWindow(cornwin);
    glutReshapeWindow(Ydialwidth,Xdialheight);
    glutPositionWindow(curwidth,curheight);
    glutShowWindow();
  }
  if (xdialflag)
  {
    glutSetWindow(xdialwin);
    glutReshapeWindow(curwidth,Xdialheight);
    glutPositionWindow(0,curheight);
    glutShowWindow();
  }
  if (ydialflag)
  {
    glutSetWindow(ydialwin);
    glutReshapeWindow(Ydialwidth,curheight);
    glutPositionWindow(curwidth,0);
    glutShowWindow();
  }
  glutSetWindow(drawwin);
  glutReshapeWindow(curwidth,curheight);
  glutPositionWindow(0,0);
  glutShowWindow();
}

/*====================================================================*/
/*=                     panneau d'information                        =*/
/*====================================================================*/
static void gfl_Info(void)
{
  #ifdef _GFL3D_
    glDisable(GL_LIGHTING);
    glDisable(GL_DEPTH_TEST);
  
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0,curwidth,0,curheight,-1.0,1.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glEnable(GL_BLEND);
    glBlendFunc(GL_ONE_MINUS_SRC_ALPHA, GL_SRC_ALPHA);
    gfl_Color4fv(GFLk_b);
    glRecti(10,10,curwidth-10,curheight-10);
    glDisable(GL_BLEND);
  #endif
  
  #ifdef _GFL2D_
  glBlendFunc(GL_ONE_MINUS_SRC_ALPHA, GL_SRC_ALPHA);
  glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
  gfl_Color4fv(GFLk_b);
  glBegin(GL_QUADS);
    glVertex2d(GFLpixtowinX1(        +10),GFLpixtowinY1(         +10));
    glVertex2d(GFLpixtowinX1(        +10),GFLpixtowinY1(curheight-10));
    glVertex2d(GFLpixtowinX1(curwidth-10),GFLpixtowinY1(curheight-10));
    glVertex2d(GFLpixtowinX1(curwidth-10),GFLpixtowinY1(         +10));
  glEnd();
  glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
  #endif

  static int x,y,usery;

  x=20;
  y=curheight-16;
  gfl_SetFontAttributes('m','b','l');
  y-=16; gfl_StaticPrint(x,y,GFLw,"Librairie GFL - version 7a");
  gfl_SetFontAttributes('m','b',0);
  y-=24; gfl_StaticPrint(x,y,GFLw,"_________ACTIONS PREDEFINIES_________");
  gfl_SetFontAttributes('m','n',0);
  y-=16; gfl_StaticPrint(x,y,GFLw,"<ESC>,<Ctrl+q> : quitte le programme proprement");
  y-=16; gfl_StaticPrint(x,y,GFLw,"<?>,<F12> : affiche cette fenetre d'aide");
  y-=16; gfl_StaticPrint(x,y,GFLw,"<Ctrl+f>,<F11> : plein ecran");
  y-=16; gfl_StaticPrint(x,y,GFLw,"<Ctrl+r> : affiche le repere principal");
  y-=16; gfl_StaticPrint(x,y,GFLw,"<Ctrl+w> : inverse la couleur de fond");
  y-=16; gfl_StaticPrint(x,y,GFLw,"<Ctrl+j> : snapshot format JPG");
  y-=16; gfl_StaticPrint(x,y,GFLw,"<Ctrl+p> : snapshot format PNG");
  #ifdef _GFL2D_
    y-=16; gfl_StaticPrint(x,y,GFLw,"[+],[-],[=] : facteurs de zoom_value ([=]:1.)");
  #endif
  #ifdef _GFL3D_
    y-=16; gfl_StaticPrint(x,y,GFLw,"<x/X,y/Y,z/Z> : deplacement (-/+) du _main_spot_            ");  
  #endif
  if (_GFL_ANIM_) { y-=16; gfl_StaticPrint(x,y,GFLw,"<SPACEBAR> : stoppe/relance l'animation "); }
  y-=16;

  usery = y;
  if (_SCROLLH_ || _SCROLLV_ || _BUTTON_ || _SWITCH_ || _EXCL_BUTTON_)
  {
    gfl_SetFontAttributes('m','b',0);
    y-=16; gfl_StaticPrint(x,y,GFLg,"_________CONTROLE UTILISATEUR___________");
    gfl_SetFontAttributes(0,'n',0);
    GFLscroll *scroll=_SCROLLH_;
    while (scroll<_SCROLLH_+_scrollhnum_)
    {
      y-=16;
      gfl_StaticPrint(x,y,GFLc,"Scroll H. <%s> :\t\t%s",scroll->name,strlen(scroll->info)>0?scroll->info:"non renseigne");
      scroll++;
    }
    scroll=_SCROLLV_;
    while (scroll<_SCROLLV_+_scrollvnum_)
    {
      y-=16;
      gfl_StaticPrint(x,y,GFLc,"Scroll V. <%s> :\t\t%s",scroll->name,strlen(scroll->info)>0?scroll->info:"non renseigne");
      scroll++;
    }
    GFLExclBut **exclBut=_EXCL_BUTTON_;
    while (exclBut<_EXCL_BUTTON_+exclbutnum)
    {
      y-=16;
      gfl_StaticPrint(x,y,GFLc,"Button  <%s> :\t\t%s" ,(*exclBut)->name,strlen((*exclBut)->info)>0?(*exclBut)->info:"non renseigne");
      exclBut++;
    }
    GFLbut *but = _BUTTON_;
    while(but < _BUTTON_+butnum){
      y-=16;
      gfl_StaticPrint(x, y, GFLc, "Button <%s>:\t\t%s", but->name, strlen(but->info)>0?but->info:"non renseigne");
      but++;
    }
    GFLpopup *pop=_POPUP_;
    while (pop<_POPUP_+popnum)
    {
      y-=16;
      gfl_StaticPrint(x,y,GFLc,"PopUp  <%s> :\t\t%s" ,pop->name,strlen(pop->info)>0?pop->info:"non renseigne");
      pop++;
    }
    GFLswitch *swt=_SWITCH_;
    while (swt<_SWITCH_+switchnum)
    {
      y-=16;
      gfl_StaticPrint(x,y,GFLc,"Switch  <%s> :\t\t%s",swt->name,strlen(swt->info)>0?swt->info:"non renseigne");
      swt++;
    }
  }
  #ifdef _GFL3D_
    x = 20 + curwidth/2;
    y=curheight-16;
    y-=16;
      gfl_SetFontAttributes(0,'b',0);
    y-=16; gfl_StaticPrint(x,y,GFLw,"_________________CAMERA_________________");
      gfl_SetFontAttributes(0,'n',0);
    y-=16; gfl_StaticPrint(x,y,GFLw,"coord. cart. : [%+.3f,%+.3f,%+.3f]",_main_cam_.pos->x,_main_cam_.pos->y,_main_cam_.pos->z);
    y-=16; gfl_StaticPrint(x,y,GFLw,"coord. spher.: [%+.3f,%+.3f,%+.3f]",_main_cam_.theta ,_main_cam_.phi,_main_cam_.dist);
    y-=16;
      gfl_SetFontAttributes(0,'b',0);
    y-=16; gfl_StaticPrint(x,y,GFLw,"_________________LUMIERE_________________");
      gfl_SetFontAttributes(0,'n',0);
    y-=16; gfl_StaticPrint(x,y,GFLw,"coord. cart. : [%+.3f,%+.3f,%+.3f]",_main_spot_.pos.x,_main_spot_.pos.y,_main_spot_.pos.z);
    y-=16; gfl_StaticPrint(x,y,GFLw,"coord. spher.: [%+.3f,%+.3f,%+.3f]",_main_spot_.theta,_main_spot_.phi  ,_main_spot_.dist );
    y-=16;
  #endif
  x = 20 + curwidth/2;
  y = usery;
  if (keynumb+skeynumb>0)
  {
    gfl_SetFontAttributes(0,'b',0);
    y-=16; gfl_StaticPrint(x,y,GFLg,"_________ACTIONS UTILISATEUR___________");
    gfl_SetFontAttributes(0,'n',0);
    GFLkey *k;
    for (k=_KEY_; k<_KEY_+keynumb; k++)
    {
      y-=16; gfl_StaticPrint(x,y,GFLc,"<%c> : %s",k->key,k->info);
    }
    for (k=_SKEY_; k<_SKEY_+skeynumb; k++)
    {
      y-=16; gfl_StaticPrint(x,y,GFLc,"<%d> : %s",k->key,k->info);
    }
  }
  #ifdef _GFL3D_
    glEnable(GL_LIGHTING);
    glEnable(GL_DEPTH_TEST);
    gfl_DrawReshape(curwidth,curheight);
  #endif
}

/*==================================*/
/*=  DISPLAY                       =*/
/*==================================*/
static __inline__ void gfl_CornDisplay(void)
{
  glutSetWindow (cornwin);
  glLoadIdentity();
  gluOrtho2D (0.,(double)Ydialwidth,0.,(double)Xdialheight);
  glClearColor(.75,.80,.85,1.);
  glClear(GL_COLOR_BUFFER_BIT);
  gfl_DrawInfoSwitch();
  glutSwapBuffers();
}

static __inline__ void gfl_XDialDisplay(void)
{
  glutSetWindow (xdialwin);
  glLoadIdentity();
  gluOrtho2D (0.,(double)curwidth,0.,(double)Xdialheight);
  glClearColor(.75,.80,.85,1.);
  glClear(GL_COLOR_BUFFER_BIT);
  GFLscroll *scroll=_SCROLLH_;
  while (scroll<_SCROLLH_+_scrollhnum_)
  {
    gfl_DrawScrollh(scroll);
    scroll++;
  }
  glutSwapBuffers();
}

static __inline__ void gfl_YDialDisplay(void)
{
  glutSetWindow (ydialwin);
  glLoadIdentity();
  gluOrtho2D (0.,(double)Ydialwidth,0.,(double)curheight);
  glClearColor(.75,.80,.85,1.);
  glClear(GL_COLOR_BUFFER_BIT);
  if (_BUTTON_) gfl_DrawButtons();
  if (_POPUP_ ) gfl_DrawPopUps();
  if (_SWITCH_) gfl_DrawSwitches();
  GFLscroll *scroll=_SCROLLV_;
  while (scroll<_SCROLLV_+_scrollvnum_)
  {
    gfl_DrawScrollv(scroll);
    scroll++;
  }
  glutSwapBuffers();
}

#ifdef _GFL3D_
  static void gfl_LookAt(GFLpoint pos, GFLpoint tar, GFLvector upv)
  {
    return gluLookAt(pos.x,pos.y,pos.z,tar.x,tar.y,tar.z,upv.x,upv.y,upv.z);
  }
#endif

#define BKG_LIST 0xFF
static void gfl_DrawDisplay(void)
{
  /* patch dégeulasse pour éviter 4 appels inutiles à gfl_DrawDisplay() */
  /* ... pas réussi à identifier l'origine de ces appels                */
  static ulint i=0;
  if ((++i<4)) return;

  glutSetWindow (drawwin);
  glClearColor(GFL_BKGD,GFL_BKGD,GFL_BKGD,1.);
  glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  
  #ifdef _GFL2D_
    /*! specifique 2D !*/
    gfl_RescaleWindow();
    glOrtho(Xwmin,Xwmax,Ywmin,Ywmax,-1.,1.);  
  #endif
  
  #ifdef _GFL3D_
    /** sp&cifique 3D **/
    /* -------------------------CAMERA------------------------------------------- */
    gfl_LookAt(*_main_cam_.pos, *_main_cam_.tar, _main_cam_.upv);
  
    /*-------------------------LUMIERE-------------------------------------------
       pour être indépendante de la pos. caméra (i.e. attachée au repère central)
       la lumière doit être déclarée APRES l'appel <gluLookAt>                    */
    glEnable(GL_LIGHTING);
    // mode plein ou fil de fer
    glPolygonMode(GL_FRONT_AND_BACK,_FILL_?GL_FILL:GL_LINE);
    // lumière
    GLfloat _POS[4]={_main_spot_.pos.x,_main_spot_.pos.y,_main_spot_.pos.z,1. };
    glLightfv(GL_LIGHT0,GL_POSITION,_POS);
   #endif

  /*!-----------------!*/
  glPushMatrix();
    if (gfl_Evts) gfl_Evts(); // APPEL A LA FONCTION D'EVENTS  UTILISATEUR
    gfl_Draw();               // APPEL A LA FONCTION DE DESSIN UTILISATEUR
  glPopMatrix();
  /*!-----------------!*/

  if (_GFL_AXES_) 
  {
    #ifdef _GFL2D_
      gfl_AutoGrad(GFLk,GFLwa);
    #endif
    #ifdef _GFL3D_
      gfl_Repere();  
    #endif
  }
 
  if (_GFL_INFO_) gfl_Info();
  glFlush();
  glutSwapBuffers();

  if (_GFL_RTVID_ON_)
  {
    glutPostWindowRedisplay(drawwin);
    gfl_RealTimeFrame();
  }
  else if (_GFL_VIDEO_ON_ && !gfl_FilmFrame()) gfl_Quit();
}

static void gfl_MainDisplay(void)
{
  glutSetWindow(mainwin);
  glLoadIdentity();
  if (cornflag ) gfl_CornDisplay();
  if (xdialflag) gfl_XDialDisplay();
  if (ydialflag) gfl_YDialDisplay();
  gfl_DrawDisplay();
}

extern void gfl_Refresh(void) { gfl_MainDisplay(); }

/*==================================
 OPERATION A REALISER AVANT DE QUITTER
  - fermer la fenetre graphique
  - liberer les diverses listes
  - fabrique le MPEG eventuel
  ==================================*/
extern void gfl_Quit(void)
{
  if (gfl_Exit) gfl_Exit();
  /* plus le Garbage Collector <mtrack> */
  glutDestroyWindow(mainwin);
  if (_GFL_RTVID_ON_) gfl_MakeMpeg4();
  switch (_GFL_VIDEO_ON_)
  {
    case 0 : break;
    case 1 : gfl_MakeMpeg4(); break;
    case 2 : gfl_MakeFlv();   break;
    case 3 : gfl_MakeAvi();   break;
    case 4 : gfl_MakeMpeg();  break;
    case 5 : gfl_FFmpeg();    break;
    default: break;
  }
  gfl_UnplugCapture();
  exit(0);
}


/*==============================================================================*/
/*= Fonction d'initialisation de la fenetre et mise en place des parametres Gl =*/
/*= windname : nom de la fenetre                                               =*/
/*= w, h : largeur/hauteur de la denetre (en pixel)                            =*/
/*==============================================================================*/
extern void gfl_InitWindow(char* windname, int w, int h)
{
  /* juste pour que <glutInit> soit content... */
  int   argc=1;
  char* argv[]={0};

  strncpy(_WINDNAME_,windname,127);
  /* initialisation des parametres gl */
  glutInit(&argc,argv);
  glutInitWindowSize(w,h);
  glutInitWindowPosition(0,0);

  pixwidth = curwidth  = w;
  pixheight= curheight = h;

  //glutInitDisplayMode(GLUT_RGBA|GLUT_DOUBLE|GLUT_ALPHA|GLUT_DEPTH|GLUT_STENCIL);

  #ifdef _GFL2D_
    /*! specifique 2D - zone graphique reelle par defaut !*/
    gfl_SetWindowCoord(0,0,w,h);  // MODIF. 2023.02.08
  #endif
  #ifdef _GFL3D_
    /*! specifique 3D - init Caméra / Lumière !*/
    /* parametres camera */
    _main_cam_.pos   = &cam_default_pos;
    _main_cam_.tar   = &cam_default_tar;
    _main_cam_.vis   = gfl_Vector2p(*_main_cam_.pos ,*_main_cam_.tar);
    _main_cam_.upv.x =   0.0;
    _main_cam_.upv.y =   0.0;
    _main_cam_.upv.z =   1.0;
    _main_cam_.hrz   = gfl_ProdVect(_main_cam_.upv,_main_cam_.vis);
    _main_cam_.hrz.y =   0.0;
    _main_cam_.hrz.z =   0.0;
    _main_cam_.theta =   0.25*PI;
    _main_cam_.phi   =   0.25*PI;
    _main_cam_.dist  =  10.0;
    _main_cam_.near  =  20.0,
    _main_cam_.far   = 100.0,
    _main_cam_.open  =   0.1;
  
    /* parametres lumiere */
    _main_spot_.theta= 0.25*PI;
    _main_spot_.phi  = 0.25*PI;
    _main_spot_.dist = 10.;
    gfl_SetSpotSteps(0.125,0.125,0.125);
    gfl_SetLightSpheric(0.25*PI,+0.25*PI,10.);
    gfl_SetLightAmbient (1.,1.,1.);
    gfl_SetLightDiffuse (1.,1.,1.);
    gfl_SetLightSpecular(1.,1.,1.);
  #endif
}

#ifdef _GFL2D_
  /*=======================================================*/
  /*=     GESTION DE LA FENETRE EN COORDONNES REELLES     =*/
  /*!          SPECIFIQUE 2D - N'EXISTE PAS EN 3D         !*/
  /*=======================================================*/
  extern bool gfl_SetWindowCoord(double xmin, double ymin, double xmax, double ymax)
  {
    if (xmin>=xmax || ymin>=ymax)
    { gfl_error_(" (xmin=%f,ymin=%f,xmax=%f,ymax=%f)> : Mauvaises coordonnees\e[0m\n",xmin,ymin,xmax,ymax); return false;}
  
    /* limites initiales de la fenetre reelle */
    Xwmin = xdl = xmin;
    Xwmax = xur = xmax;
    Ywmin = ydl = ymin;
    Ywmax = yur = ymax;
    gfl_RescaleWindow();
    return true;
  }
  
  /* 2025/01/14 - variantes un peu plus explicite  */
  extern bool gfl_SetDrawZone(double xmin, double ymin, double width, double height )
  {
    if (width<=0. && height<=0.)
    { gfl_error_(" at least one dimension (width or height) must be stritly positive\n\
                   a negative or null value implies aspect ratio 1:1 bases the positive value\e[0m\n"); return false;}
    double whratio =(double)pixwidth/(double)pixheight;
    return gfl_SetWindowCoord(xmin,ymin,
                              xmin+(width >0.?width :height/whratio),
                              ymin+(height>0.?height:width*whratio));
  }
  
  /* =========================================================================================== *
   * [jan.2024] - variante à la fonction d'initialisation - e centre et les dimensions           *
   * =========================================================================================== */
  extern bool gfl_SetCenteredDrawZone(double xctr, double yctr, double width, double height )
  {
    if (width<=0. && height<=0.)
    { gfl_error_(" at least one dimension (width or height) must be stritly positive\n\
                   a negative or null value implies aspect ratio 1:1 bases the positive value\e[0m\n"); return false; }
    double whratio =(double)pixwidth/(double)pixheight;
    double h= 0.5*(height>0.?height:width/whratio );
    double w= 0.5*(width >0.?width :height*whratio);
    return gfl_SetWindowCoord(xctr-w,yctr-h,xctr+w,yctr+h);
  }
  
  /* =========================================================================================== *
   * [oct.2024] - variante à la fonction d'initialisation - ouvre direct sur un image            *
   * =========================================================================================== */
  extern GFLpixmap* gfl_InitImage(char *imgfname, char **pathname, char **rootname, char **extname)
  {
    if (!gfl_AnyToPixmap(&gflrootpix,imgfname)) gfl_fopenfail_(imgfname,"r");
  
    sprintf(gflrootname,"%s",imgfname);
  
    char* sep=strrchr(gflrootname,'/');
   *pathname = gflrootname;
    if (sep) *rootname=sep+1;
   *sep=0;
    sep=strrchr(*rootname,'.');
   *sep=0;
    if (sep) *extname=sep+1;
  
    int height = MIN(gflrootpix->height, 800);
    height = MAX(height,512);
    int width  = MIN(gflrootpix->width ,1600);
    width  = MAX(width ,512);
  
    gfl_InitWindow(*rootname,width,height);
    gfl_SetCenteredDrawZone(0.,0.,width,height);
    return gflrootpix;
  }
#endif

/*==================================*/
/* FONCTION PRINCIPALE              */
/*==================================*/
extern int gfl_MainStart(void)
{
  pixwidth = curwidth  = pixwidth -Ydialwidth;
  pixheight= curheight = pixheight-Xdialheight;

  if ((mainwin=glutCreateWindow(_WINDNAME_)) == GL_FALSE)
  { gfl_error_(" erreur creation de la fenetre <mainwin>\e[0m\n"); exit(1); }

  // CREATION DE LA FENETRE DE DESSIN
  if ((drawwin=glutCreateSubWindow(mainwin,0,0,curwidth,curheight)) == GL_FALSE)
  { gfl_error_(" erreur creation de la fenetre <drawwin>\e[0m\n"); exit(1); }

  glutInitDisplayMode(GLUT_RGBA|GLUT_DOUBLE|GLUT_DEPTH);

  /* attribution des fonctions principales */
  glutMouseFunc        (gfl_DrawMouseClic       );
  glutMotionFunc       (gfl_DrawMouseMove       );
  glutKeyboardFunc     (gfl_Keyboard            );
  glutSpecialFunc      (gfl_Special             );
  glutPassiveMotionFunc(gfl_DrawPassiveMouseMove);
  glutReshapeFunc      (gfl_DrawReshape         );
  glutDisplayFunc      (gfl_DrawDisplay         );
  
  #ifdef _GFL2D_
    glEnable(GL_BLEND);
    glBlendFunc (GL_ONE_MINUS_SRC_ALPHA, GL_SRC_ALPHA);
  #endif
  
  #ifdef _GFL3D_
    /* initialisation des parametres 3D */
    glClearColor(0.,0.,0.,0.);
    glEnable(GL_LIGHT0);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_NORMALIZE);
  
    gfl_SetLightSpheric(_main_spot_.theta, _main_spot_.phi, _main_spot_.dist);
    GLfloat _POS[4]={_main_spot_.pos.x,_main_spot_.pos.y,_main_spot_.pos.z,1. };
    glLightfv(GL_LIGHT0,GL_POSITION      ,_POS);
    glLightfv(GL_LIGHT0,GL_AMBIENT       ,(GLfloat*)&_main_spot_.ambi);
    glLightfv(GL_LIGHT0,GL_DIFFUSE       ,(GLfloat*)&_main_spot_.diff);
    glLightfv(GL_LIGHT0,GL_SPECULAR      ,(GLfloat*)&_main_spot_.spec);
  
    glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT  ,(GLfloat*)&_main_spot_.ambi);
    glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE  ,(GLfloat*)&_main_spot_.diff);
    glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR ,(GLfloat*)&_main_spot_.spec);
    glMaterialf (GL_FRONT_AND_BACK,GL_SHININESS,0.);

    /* spécial 3D - gestion des faces */
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glPolygonMode(GL_FRONT,(_FILL_?GL_FILL:GL_LINE));
    gfl_SetBkGdCol(0.);
  #endif

  // APPEL DES FONCTIONS DE DEMARRAGE (init, ctrl, evts)
  if (gfl_Init) gfl_Init();
  if (gfl_Ctrl) gfl_Ctrl();
  if (gfl_Evts) gfl_Evts();
  
  // CREATION DU MENU CLIC-DROIT
  gfl_SubMenu();

  // CREATION DES FENETRES DE CONTROLE, SI NECESSAIRE
  if (_SCROLLH_)
  { // bandeau horizontal (si presence scrollbars horiz)
    xdialflag=true;
    if ((xdialwin=glutCreateSubWindow(mainwin,0,0,curwidth,Xdialheight)) == GL_FALSE)
    { gfl_error_(" erreur creation de la fenetre <xdialwin>\e[0m\n"); exit(1); }
    glutReshapeFunc(gfl_XDialReshape);
    glutDisplayFunc(gfl_XDialDisplay);
    glutMouseFunc(gfl_XDialMouseClic);
    glutMotionFunc(gfl_XDialMouseMove);
  }
  if (_SCROLLV_ || _SWITCH_ || _BUTTON_ || _POPUP_)
  { // bandeau vertical (si presence scrollbars vert. ou boutons)
    ydialflag=true;
    if ((ydialwin=glutCreateSubWindow(mainwin,0,0,Ydialwidth,curheight)) == GL_FALSE)
    { gfl_error_(" erreur creation de la fenetre <ydialwin>\e[0m\n"); exit(1); }
    glutReshapeFunc(gfl_YDialReshape);
    glutDisplayFunc(gfl_YDialDisplay);
    glutMouseFunc(gfl_YDialMouseClic);
    glutMotionFunc(gfl_YDialMouseMove);
    // SWITCH, BUTTON, POPUP
    if (_SWITCH_)
    {
      gfl_InitSwitchOn();
      gfl_InitSwitchOff();
    }
    if (_BUTTON_)
    {
      gfl_InitButOn();
      gfl_InitButOff();
    }
    if (_POPUP_)
    {
      gfl_InitPopOn();
      gfl_InitPopOff();
    }
  }
  if (xdialflag && ydialflag)
  { // coin bas/gauche "?"
    cornflag=true;
    if ((cornwin=glutCreateSubWindow(mainwin,0,0,Ydialwidth,Xdialheight)) == GL_FALSE)
    { gfl_error_(" erreur creation de la fenetre <cornwin>\e[0m\n"); exit(1); }
    glutReshapeFunc(gfl_CornReshape);
    glutDisplayFunc(gfl_CornDisplay);
    glutMouseFunc(gfl_CornMouseClic);
    gfl_InitCornSwitchOn();
    gfl_InitCornSwitchOff();
  }

  // FENETRE PRINCIPALE
  curwidth +=Ydialwidth;
  curheight+=Xdialheight;

  glutSetWindow(mainwin);
  glutDisplayFunc(gfl_MainDisplay);
  glutReshapeFunc(gfl_MainReshape);
  glutReshapeWindow(curwidth,curheight);
  
#ifndef _USE_GL_IDLE_
  /** NOUVEAUTE VERSION .7a : REMPLACE APPEL à <glutIdleFunc> **/
  glutTimerFunc(_GFL_TimerFreq_, gfl_Timer, 0);     // First timer call immediately
#endif
  // INITIALISATION DES TABLES TRIGO
  //gfl_InitTrigonometric();

  /*= LANCEMENT BOUCLE PRINCIPALE OpenGl =*/
  glutMainLoop();
  return 1;
}

#ifdef __cplusplus
  }
#endif
