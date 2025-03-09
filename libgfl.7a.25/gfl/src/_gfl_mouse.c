/*!===============================================================
  E.Incerti - Universite Gustave Eiffel - eric.incerti@univ-eiffel.fr
       - Librairie GFL - Fonctions de base d'acces public -
                    version 6e - aout 2022
  ================================================================!*/

/*====================================================================*/
/*=      Gestion des 'clic' souris                                   =*/
/*====================================================================*/
static bool     _LEFT_CLIC_ = false;
static GFLpoint _CLIC_POS_  = {0.};
static GFLpoint _MOUSE_POS_ = {0.}; /*position en temps réel de la souris.*/
static GLint startx,starty; /* La position du début de mouvenent avec clic enfoncé (quel qu'il soit)*/
static GLint winmsx,winmsy;

static void (*MouseMoveAction)(GFLpoint) = NULL;

extern GFLpoint* gfl_GetClic(void)
{
 if (!_LEFT_CLIC_) return NULL;
 _LEFT_CLIC_ = false;
 return &_CLIC_POS_;
}

extern GFLpoint *gfl_GetClicPixPos(int *x, int *y)
{
  if (!_LEFT_CLIC_) return NULL;
 _LEFT_CLIC_ = false;
  *x=startx, *y=starty;
 return &_CLIC_POS_;
}

static __inline__ void gfl_CornMouseClic(int button, int state, int x, int y)
{
  glutSetWindow(cornwin);
  if (button!=MOUSE_LEFT) return;
  if (state == MOUSE_PRESSED)
  {
    pushmode=MOUSE_PRESSED;
    moving=false;
    return;
  }
  if (pushmode!=MOUSE_PRESSED) return;
  _GFL_INFO_ =!_GFL_INFO_ ;
  glutPostRedisplay();
  glutShowWindow();
  glutSetWindow(mainwin);
  glutShowWindow();
  return;
}

static __inline__ void gfl_XDialMouseClic(int button, int state, int x, int y)
{
  glutSetWindow(xdialwin);
  if (button!=MOUSE_LEFT) return;
  if (state == MOUSE_PRESSED)
  {
    pushmode=MOUSE_PRESSED;
    moving=false;
    return;
  }
  if (pushmode!=MOUSE_PRESSED) return;
  /*! clic Scrollbar!*/
  GFLscroll *scroll=_SCROLLH_;
  y = Xdialheight-y;
  while (scroll<_SCROLLH_+_scrollhnum_ && pushmode==MOUSE_PRESSED)
  {
    if (abs((int)scroll->ycurs-y)<scroll->w && x>(Xscrollstart-2*scroll->w) && x<(Xscrollend  +2*scroll->w))
    {
      moving = true+button;
      pushmode=MOUSE_RELEASED;
      TheScrollh=scroll;
      TheScrollh->xcurs  = CLIP(Xscrollstart,x,Xscrollend);
      TheScrollh->cursor = (double)(TheScrollh->xcurs-Xscrollstart)/(double)Xscrollrange;
      if (TheScrollh->dprm)
         *TheScrollh->dprm   = (double)(TheScrollh->min + TheScrollh->cursor*(TheScrollh->max-TheScrollh->min));
      else
      if (TheScrollh->iprm)
         *TheScrollh->iprm   = (   int)(TheScrollh->min + TheScrollh->cursor*(TheScrollh->max-TheScrollh->min));
      glutPostRedisplay();
      glutShowWindow();
      glutSetWindow(mainwin);
      glutShowWindow();
      return;
    }
    scroll++;
  }
  return;
}

static __inline__ void gfl_YDialMouseClic(int button, int state, int x, int y)
{
  glutSetWindow(ydialwin);
  startx = x;
  starty = y;
  if (button==MOUSE_MIDDLE) { moving=true+button; return; }
  if (button!=MOUSE_LEFT  ) return;

  if (state == MOUSE_PRESSED)
  {
    pushmode=MOUSE_PRESSED;
    moving=false;
    return;
  }
  if (pushmode!=MOUSE_PRESSED) return;
  /*! clic boutons!*/
  if (gfl_SelectPopUp(x,y) || gfl_SelectSwitch(x,y) || gfl_SelectButton(x,y) || gfl_SelectExclButton(x, y))
  {
    if (ThePopUp && ThePopUp->on)
    {
      ThePopUp->action();
      ThePopUp->on=false;
    }
    ThePopUp =NULL;
    ThePopUp=NULL;
    glutPostRedisplay();
    glutShowWindow(); glutSetWindow(mainwin); glutShowWindow();
    return;
  }
  /*! clic Scrollbar!*/
  y = curheight-y;
  GFLscroll *scroll=_SCROLLV_;
  while (scroll<_SCROLLV_+_scrollvnum_ && pushmode==MOUSE_PRESSED)
  {
    if (abs((int)scroll->xcurs-x)<scroll->w && y>(Yscrollstart-2*scroll->w)
                                            && y<(Yscrollend  +2*scroll->w) )
    {
      moving = true+button;
      pushmode=MOUSE_RELEASED;
      TheScrollv=scroll;
      TheScrollv->ycurs = CLIP(Yscrollstart,y,Yscrollend);
      TheScrollv->cursor= (double)(TheScrollv->ycurs-Yscrollstart)/(double)Yscrollrange;
      if (TheScrollv->dprm)
         *TheScrollv->dprm   = (double)(TheScrollv->min + TheScrollv->cursor*(TheScrollv->max-TheScrollv->min));
      else
      if (TheScrollv->iprm)
         *TheScrollv->iprm   = (   int)(TheScrollv->min + TheScrollv->cursor*(TheScrollv->max-TheScrollv->min));
      glutPostRedisplay();
      glutShowWindow(); glutSetWindow(mainwin); glutShowWindow();
        return;
    }
    scroll++;
  }
  return;
}

static __inline__ void gfl_DrawMouseClic(int button, int state, int x, int y)
{
  glutSetWindow(drawwin);
  winmsx = x;
  winmsy = curheight-y;
  startx = x;
  starty = y;
  switch(button)
  {
    case WHEEL_SCROLL_UP   : /* zoom avant */
      #ifdef _GFL2D_
        zoom_value*=zoom_factor;  
      #endif
      #ifdef _GFL3D_
        _main_cam_.dist*=1.05;
        _main_cam_.pos->x *=1.05;
        _main_cam_.pos->y *=1.05;
        _main_cam_.pos->z *=1.05;
      #endif
      glutPostRedisplay(); 
      break;
    case WHEEL_SCROLL_DOWN : /* zoom arrière */
      #ifdef _GFL2D_
        zoom_value/=zoom_factor;  
      #endif
      #ifdef _GFL3D_
        _main_cam_.dist/=1.05;
        _main_cam_.pos->x /=1.05;
        _main_cam_.pos->y /=1.05;
        _main_cam_.pos->z /=1.05;
      #endif
      glutPostRedisplay(); 
      break;
    #ifdef _GFL2D_
      case MOUSE_MIDDLE:
      {
        if (state == MOUSE_PRESSED)
        {
          moving = false;
          return;
        }
        /* HP - mai 2022 : correction panscan */
        XcurrOffset = Xoffset;
        YcurrOffset = Yoffset;
        moving=true+button; return;
      }
      case MOUSE_LEFT :
      {
        if (state == MOUSE_RELEASED) _LEFT_CLIC_=true;
        else if (state == MOUSE_PRESSED)
        { /* HP - mai 2022 : correction panscan */
          pushmode= MOUSE_PRESSED;
          moving  = false;
          _LEFT_CLIC_ = false;
          CPT     = NULL; //buffer Pt. de Contrôle vidé.
          return;
        }
        if (pushmode==MOUSE_PRESSED)
        {
          y = curheight - y;
          _CLIC_POS_.x= GFLpixtowinX1(x);
          _CLIC_POS_.y= GFLpixtowinY1(y);
            /* point de controle ? */
            int    n=0;
            while (n<nbcpt)
            {
              if (fabs(CTRLPTS[n].add->x -_CLIC_POS_.x)<=fabs(CTRLPTS[n].ray*Xpixsize/Xratio) &&
                  fabs(CTRLPTS[n].add->y -_CLIC_POS_.y)<=fabs(CTRLPTS[n].ray*Ypixsize/Yratio)  )
              {
                CPT=CTRLPTS[n].add;
                CPTpos=*CPT;
                pushmode=MOUSE_RELEASED;
                moving = true+button;
                return;
              }
              n++;
            }
          // 23/10/2022 : ajout pour prendre en compte le clic => doit réafficher
          glutPostRedisplay();
        }
      }
    #endif
    #ifdef _GFL3D_
      default :
        y = curheight - y;
        _CLIC_POS_.x = x;
        _CLIC_POS_.y = y;
        _CLIC_POS_.z = 0.;
        moving = true+button;
        /* recupere les coord. spherique de la camera */
        theta_cam_init  = _main_cam_.theta;
        phi_cam_init    = _main_cam_.phi;
        dist_cam_init   = _main_cam_.dist;
          /* recupere les coord. spherique de la lumiere */
        theta_spot_init = _main_spot_.theta;
        phi_spot_init   = _main_spot_.phi;
        dist_spot_init  = _main_spot_.dist;
        _main_cam_.dx   = _main_cam_.dy =0.;
        break;
    #endif
  }
  return;
}

/*====================================================================*/
/*=  Gestion des mouvements de la souris  (clic gauche)              =*/
/*====================================================================*/
#ifdef _GFL3D_
  /* pilotes mouvements de la souris : camera ou lumiere */
  #define _CLIC_CONTROL_CAM_SPHR_  0
  #define _CLIC_CONTROL_SPOT_SPHR_ 1
  /* valeur par defaut : camera */
  static int _CLIC_CONTROLLER_ = _CLIC_CONTROL_CAM_SPHR_;
#endif

static double _mouse_speed_ = 0.005;
extern double gfl_GetMouseSpeed() { return _mouse_speed_; }
extern void   gfl_SetMouseSpeed(double speed) { _mouse_speed_=speed; }

static __inline__ void gfl_DrawMouseMove(int x, int y)
{
  glutSetWindow(drawwin);
  #ifdef _GFL2D_
    switch (moving)
    { 
      case true+MOUSE_MIDDLE :
      { /* le panscan */
        Xoffset = XcurrOffset + (+Xpixsize*(x-startx));
        Yoffset = YcurrOffset + (-Ypixsize*(y-starty));
        glutPostWindowRedisplay(drawwin);
        break;
      }
      case true+MOUSE_LEFT :
      { /* les points de controle */
        y = curheight - y;
        if (nbcpt!=0 && CPT)
        {
          CPT->x = GFLpixtowinX1(x);
          CPT->y = GFLpixtowinY1(y);
          CPT->x = CLIP(Xwmin,CPT->x,Xwmax);
          CPT->y = CLIP(Ywmin,CPT->y,Ywmax);
        }
        glutPostWindowRedisplay(drawwin);
        break;
      }
    }
  #endif
  #ifdef _GFL3D_
    double dx,dy;
    switch (_CLIC_CONTROLLER_)
    { /* fonctionnalite mouvements attache aux mouvements souris */
      case _CLIC_CONTROL_SPOT_SPHR_ :  /* Contrôle la Lumiere */
         switch (moving)
          {
            case true+GLUT_LEFT_BUTTON   :
             _main_spot_.theta = theta_spot_init + 0.005*(x-startx);
             _main_spot_.phi   = phi_spot_init   - 0.005*(y-starty);
             /* mise a jour des coord. cartesiennes de la lumiere */
             _main_spot_.pos.x = _main_spot_.dist*cos(_main_spot_.theta)*cos(_main_spot_.phi);
             _main_spot_.pos.y = _main_spot_.dist*sin(_main_spot_.theta)*cos(_main_spot_.phi);
             _main_spot_.pos.z = _main_spot_.dist*sin(_main_spot_.phi);
          break;
          case true+GLUT_MIDDLE_BUTTON  :
             _main_spot_.dist = dist_spot_init+0.05*(y-starty);
             /* mise a jour des coord. cartesiennes de la lumiere */
             _main_spot_.pos.x = _main_spot_.dist*cos(_main_spot_.theta)*cos(_main_spot_.phi);
             _main_spot_.pos.y = _main_spot_.dist*sin(_main_spot_.theta)*cos(_main_spot_.phi);
             _main_spot_.pos.z = _main_spot_.dist*sin(_main_spot_.phi);
          break;
        }
      break;
      case _CLIC_CONTROL_CAM_SPHR_ : /* Contrôle la Camera    */
        switch (moving)
        {
          case true+GLUT_LEFT_BUTTON   :
            gfl_SetCameraSpheric(theta_cam_init -_mouse_speed_*(x-startx),
                                 phi_cam_init   +_mouse_speed_*(y-starty),
                                 _main_cam_.dist);
          break;
          case true+GLUT_MIDDLE_BUTTON  : // pan scan
            dx = _mouse_speed_*(x-startx);
            dy = _mouse_speed_*(y-starty);
            dy*= (fabs(_main_cam_.phi)>(PI/4)?1.0:-1.0);
            gfl_PanScan(&_main_cam_,dx,dy);
            startx=x;
            starty=y;
          break;
        }
      break;
    }
  #endif

  _MOUSE_POS_.x=x;
  _MOUSE_POS_.y=y;
  glutPostRedisplay();
}

/*= recupere la position de la souris =*/
extern GFLpoint* gfl_GetMousePosition(void) { return &_MOUSE_POS_; }

#ifdef _GFL2D_
  /*= renvoie FALSE si la souris n'est pas dans le fenêtre(- une marge) =*/
  /*! pas très convivial  -- à revoir                                   !*/
  /* -------------------
   * |ooooo false ooooo|
   * |ooooooooooooooooo|
   * |oo             oo|
   * |oo             oo|
   * |oo     true    oo|
   * |oo             oo|
   * |ooooooooooooooooo|
   * |ooooooooooooooooo|
   * ------------------- */
  extern bool gfl_MouseInWindow(double margin)
  {
    if (_MOUSE_POS_.x <= gfl_GetXMin()*(1.+margin) || _MOUSE_POS_.x >= gfl_GetXMax()*(1.-margin) ||
        _MOUSE_POS_.y <= gfl_GetYMin()*(1.+margin) || _MOUSE_POS_.y >= gfl_GetYMax()*(1.-margin) )
        return false;
    return true;
  }
#endif

extern void     gfl_SetMouseMoveAction(void (*action)(GFLpoint))
{ MouseMoveAction = action;}

/*=   PASSIVE MOUSE FUNCTIONS =*/
static __inline__ void gfl_DrawPassiveMouseMove(int x, int y)
{
  glutSetWindow(drawwin);
  y = curheight - y;
  #ifdef _GFL2D_
    _MOUSE_POS_.x = GFLpixtowinX1(x);
    _MOUSE_POS_.y = GFLpixtowinY1(y);
  #endif
  #ifdef _GFL3D_
    _MOUSE_POS_.x = (double)x;
    _MOUSE_POS_.y = (double)y;
    _MOUSE_POS_.z = 0.;
  #endif
  if(MouseMoveAction) (*MouseMoveAction)(_MOUSE_POS_);
}

/*=   MOVE MOUSE FUNCTIONS =*/
static __inline__ void gfl_XDialMouseMove(int x, int y)
{
  glutSetWindow(xdialwin);
  y = curheight - y;
  switch (moving)
  {
    case true+MOUSE_LEFT   :
      if (TheScrollh)
      {
        TheScrollh->xcurs = CLIP(Xscrollstart,x,Xscrollend);
        TheScrollh->cursor= (double)(TheScrollh->xcurs-Xscrollstart)/(double)Xscrollrange;
        if (TheScrollh->dprm)
           *TheScrollh->dprm   = (double)(TheScrollh->min + TheScrollh->cursor*(TheScrollh->max-TheScrollh->min));
        else
        if (TheScrollh->iprm)
           *TheScrollh->iprm   = (   int)(TheScrollh->min + TheScrollh->cursor*(TheScrollh->max-TheScrollh->min));
      }
      glutPostRedisplay();
      glutPostWindowRedisplay(drawwin);
  }
}

static __inline__ void gfl_YDialMouseMove(int x, int y)
{
  glutSetWindow(ydialwin);
  y = curheight - y;
  switch (moving)
  {
    case true+MOUSE_LEFT   :
      if (TheScrollv)
      {
        TheScrollv->ycurs = CLIP(Yscrollstart,y,Yscrollend);
        TheScrollv->cursor= (double)(TheScrollv->ycurs-Yscrollstart)/(double)Yscrollrange;
        if (TheScrollv->dprm)
           *TheScrollv->dprm   = (double)(TheScrollv->min + TheScrollv->cursor*(TheScrollv->max-TheScrollv->min));
        else
        if (TheScrollv->iprm)
           *TheScrollv->iprm   = (   int)(TheScrollv->min + TheScrollv->cursor*(TheScrollv->max-TheScrollv->min));
      }
      glutPostRedisplay();
      glutPostWindowRedisplay(drawwin);
  }
}

/*====================================================================*/
/*=  fonction associee aux evenements de menu.                       =*/
/*=  - item : code associe au menu selectionne                       =*/
/*====================================================================*/
#define _MENU_CLIC_ 200
#define _MENU_FORM_ 300
#define _MENU_MPEG_ 400
#define _MENU_EXIT_ 500

/*====================================================================*/
/*=  Gestion des menus de la souris (clic droit)                     =*/
/*====================================================================*/
#ifdef _GFL3D_
  static const char* _RIGHT_BUT_[] = {"cam","light",NULL};
#endif
static const char* _IMG_CODEC_[] = {"jpg","pnm","png","gif","eps","bmp","tif","ras",NULL};
static const char* _VID_CODEC_[] = {"mp4","flv","x264","mpg2","ffmpeg",NULL};

static void gfl_MainMenu(int item)
{
#ifdef _GFL3D_
  if (item<_MENU_FORM_)            /* souris : camera ou lumiere (spécifique 3D) */
  {
    const char **f=_RIGHT_BUT_;
    item-=_MENU_CLIC_;
    while (item--) f++;
    _CLIC_CONTROLLER_ = (int)(f-_RIGHT_BUT_);
    return;
  }
#endif  
  if (item<_MENU_MPEG_)            /* choix format snapshot */
  {
    const char **f=_IMG_CODEC_;
    item-=_MENU_FORM_;
    while (item--) f++;
    /*if (!_GFL_FULLSCREEN_)*/ gfl_Snapshot(*f,_WINDNAME_,curwidth,curheight);
    return;
  }
  if (item <_MENU_EXIT_)           /* choix format video    */
  {
    const char **f=_VID_CODEC_;
    item-=_MENU_MPEG_;
    _GFL_VIDEO_ON_++;
    while (item--) { f++; _GFL_VIDEO_ON_++; }
    gfl_PlugCapture(_WINDNAME_,0,0,curwidth,curheight);
    return;
  }
  if (item==_MENU_EXIT_) return gfl_Quit(); /* exit */
}

static void gfl_SubMenu(void)
{
  /* CONSTRUCTION DU MENU SOURIS */
  const char **f=NULL;
#ifdef _GFL3D_
  int   mm=_MENU_CLIC_ ,submenum;
#endif

  int   mf=_MENU_FORM_ ,submenuf=0;
  int   mv=_MENU_MPEG_ ,submenuv=0;

#ifdef _GFL3D_
  submenum=glutCreateMenu(gfl_MainMenu);
  glutAddMenuEntry("visee",mm);
  /* fonctionalites clic droit (camera ou lumiere) */
  submenum=glutCreateMenu(gfl_MainMenu);
  f=_RIGHT_BUT_;
  while (*f) glutAddMenuEntry(*f++,mm++);
#endif

  /* fonctionalites clic droit (camera ou lumiere) */
  submenuf=glutCreateMenu(gfl_MainMenu);
  f=_IMG_CODEC_;
  while (*f) glutAddMenuEntry(*f++,mf++);

  /* choix format et lancement video */
  if (_GFL_ANIM_)
  {
    submenuv=glutCreateMenu(gfl_MainMenu);
    f=_VID_CODEC_;
    while (*f) glutAddMenuEntry(*f++,mv++);
  }

  glutCreateMenu(gfl_MainMenu);
#ifdef _GFL3D_
  glutAddSubMenu("mouse",submenum);
#endif
  glutAddSubMenu("photo",submenuf);
  if (_GFL_ANIM_)  glutAddSubMenu("video",submenuv);
  glutAddMenuEntry("exit ",_MENU_EXIT_);
  glutAttachMenu(GLUT_RIGHT_BUTTON);
}

/* utilitaire => devra disparaître */
extern void gfl_GetMouseStatut(void)
{
  fprintf(stderr, "<-- Mouse Info -->\n");
  fprintf(stderr, "clic status : %s\n", _LEFT_CLIC_?"true":"false");
  if(_LEFT_CLIC_)
  {
    fprintf(stderr, "\tclic_position = (%f, %f)\n", _CLIC_POS_.x, _CLIC_POS_.y);
  }
  fprintf(stderr, "mouse pos = (%f, %f)\n", _MOUSE_POS_.x, _MOUSE_POS_.y);
  fprintf(stderr, "stratx = %d, starty = %d\n", startx, starty);
  fprintf(stderr, "<-- ---------- -->\n");

}
