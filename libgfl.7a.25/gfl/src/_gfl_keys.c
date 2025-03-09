/*!===============================================================
  E.Incerti - Universite Gustave Eiffel - eric.incerti@univ-eiffel.fr
       - Librairie GFL - Fonctions de base d'acces public -
                    version 6e - aout 2022
  ================================================================!*/

/*===============================================*/
/*=  ASSOCIER DES FONCTIONALITES A DES TOUCHES  =*/
/*===============================================*/

/**@brief   associates an action to a key (ASCII).
 * @warning some keys are pre-defined and thus should not be used to avoid conflict with core functionalities.
 *          |SPACE|ESC|q|Q|Ctrl+f|Ctrl+r|Ctrl+w|+|-|=|?| */
typedef struct
{
  char  key;             /**@brief   key value (ASCII)
                          * @warning see struct GFLkey documentation for pre-defined behaviour  */
  void (*action)(void);  /**@brief   action to be done at keypressed                            */
  char  info[_GFL_INFOSIZE_+1];/**@brief text for the info pannel (key '?')                           */
} GFLkey;

static int keynumb=0;
static int keyblocnumb=0;
static GFLkey* _KEY_=NULL;

/*= cree une GFLkey =*/
extern bool gfl_SetKeyAction(char key, void (*action)(void), const char *info)
{
  if (keyblocnumb==_GFL_MAXBLOC_) gfl_error_(" %d actions : maximum atteint\e[0m\n",_GFL_MAXBLOC_*_GFL_BLOCSIZE_);

  if (keynumb%_GFL_BLOCSIZE_==0)
  { /* si toutes les GLXkey ont ete attribuees on ajoute un bloc */
    if (!(_KEY_=(GFLkey*)realloc(_KEY_,(keyblocnumb+1)*_GFL_BLOCSIZE_*sizeof(GFLkey)))) return false;
    memset(_KEY_+keyblocnumb*_GFL_BLOCSIZE_,0,_GFL_BLOCSIZE_*sizeof(GFLkey));
    keyblocnumb++;
  }
  _KEY_[keynumb].key    = key;
  _KEY_[keynumb].action = action;
  if (info) snprintf(_KEY_[keynumb].info,_GFL_INFOSIZE_,"%s",info);
  keynumb++;
  return true;
}

/*= Execute l'action associee au car. c =*/
static __inline__ bool gfl_ExecuteKeyAction(char c)
{
  GFLkey *k;
  for (k=_KEY_; k<_KEY_+keynumb; k++)
    if (c==k->key) { k->action(); return true; }
  return false;
}

/*========================================================*/
/*=  ASSOCIER DES FONCTIONALITES A DES TOUCHES SPECIALES =*/
/*========================================================*/
static int    skeynumb=0;
static int    skeyblocnumb=0;
static GFLkey* _SKEY_=NULL;

extern bool gfl_SetSpecialKeyAction(char key, void (*action)(void), const char *info)
{
  if (skeyblocnumb==_GFL_MAXBLOC_) gfl_error_(" %d actions : maximum atteint\e[0m\n",_GFL_MAXBLOC_*_GFL_BLOCSIZE_);
  if (skeynumb%_GFL_BLOCSIZE_==0)
  { /* si toutes les GLXkey ont ete attribuees on ajoute un bloc */
    if (!(_SKEY_=(GFLkey*)realloc(_SKEY_,(skeyblocnumb+1)*_GFL_BLOCSIZE_*sizeof(GFLkey)))) return false;
    memset(_SKEY_+skeyblocnumb*_GFL_BLOCSIZE_,0,_GFL_BLOCSIZE_*sizeof(GFLkey));
    skeyblocnumb++;
  }
  _SKEY_[skeynumb].key    = key;
  _SKEY_[skeynumb].action = action;
  if (info) snprintf(_SKEY_[skeynumb].info,_GFL_INFOSIZE_,"%s",info);
  skeynumb++;
  return true;
}

/*= Execute l'action associee au car. c =*/
static __inline__ bool gfl_ExecuteSpecialKeyAction(char c)
{
  GFLkey *k;
  for (k=_SKEY_; k<_SKEY_+skeynumb; k++)
    if (c==k->key) { k->action(); return true; }

  return false;
}

/*= Libere les KeyActions =*/
/* obsolète -> garbage collector
static __inline__ void gfl_FreeKeyAction(void)
{
  if (_KEY_) free(_KEY_);
  _KEY_=NULL;
  keyblocnumb=0;
  keynumb=0;
  if (_SKEY_) free(_SKEY_);
  _SKEY_=NULL;
  skeyblocnumb=0;
  skeynumb=0;
}*/

/*====================================================
  fonction associee aux touches 'spéciales'
  F1 à F12, pavé fléché...
  parametres :
  - c : caractere saisi
  - x,y : coordonnee du curseur dans la fenetre
  ==================================================*/

static void __inline__ gfl_Special(int c, int x, int y)
{
  switch(c)
  { // <F11> plein écran -----------------------------------------
    case SKEY_F11 :_GFL_FULLSCREEN_ = !_GFL_FULLSCREEN_;
                    glutSetWindow(mainwin);
                    switch (_GFL_FULLSCREEN_)
                    {
                      case true :
                        fullwidth =curwidth;
                        fullheight=curheight;
                        glutFullScreen();
                        break;
                      default : glutReshapeWindow(fullwidth,fullheight);
                    }
                    return glutPostRedisplay();
    // info -----------------------------------------------------------
    case SKEY_F12 : _GFL_INFO_ =!_GFL_INFO_ ; 
                    glutSetWindow(mainwin);
                    return glutPostRedisplay();
    case SKEY_F7  : gfl_RealTimeVideo(); return glutPostRedisplay();
    default       : glutSetWindow(mainwin);
                    if (gfl_ExecuteSpecialKeyAction(c)) glutPostRedisplay();
                    //else gfl_warning_("SPECIAL KEY (%d) : nothing attached\t\r",(int)c);
  }
  glutPostWindowRedisplay(mainwin);
}

/*==============================================================================*/
/*= fonction associee aux interruptions clavier : quelques touches predefinies =*/
/*= parametres :                                                               =*/
/*= - c : caractere saisi                                                      =*/
/*= - x,y : coordonnee du curseur dans la fenetre                              =*/
/*==============================================================================*/
static void __inline__ gfl_Keyboard(uchar c, int x, int y)
{
  
  switch(c)
  { // <Ctrl+'f'> plein écran -----------------------------------------
    case 6  : _GFL_FULLSCREEN_ = !_GFL_FULLSCREEN_;
              glutSetWindow(mainwin);
              switch (_GFL_FULLSCREEN_)
              {
                case true :
                  
                  fullwidth=curwidth;
                  fullheight=curheight;
                  glutFullScreen();
                  break;
                default :
                  glutReshapeWindow(fullwidth,fullheight);
              }
              break;
    // <Ctrl+j>/<Ctrl+p> capture d'écran jpeg, png --------------------
    case 10 : gfl_Snapshot("jpg",_WINDNAME_,curwidth,curheight);  return;
    case 16 : gfl_Snapshot("png",_WINDNAME_,curwidth,curheight);  return;

    // <Ctrl+r> affichage/masque la grille (2D) ou le repère (3D) ------
    case 18 : glutSetWindow(mainwin); _GFL_AXES_=!_GFL_AXES_; glutPostWindowRedisplay(drawwin); break;
    // <Ctrl+v> lance l'enregistrement vidéo "temps-réel"
    case 22 : if (_GFL_ENABLE_RTVID_) { gfl_RealTimeVideo(); return glutPostRedisplay(); }
              break;
    // <Ctrl+w> inverser couleur de fond ------------------------------
    case 23 : glutSetWindow(mainwin); GFL_BKGD=1.-GFL_BKGD; break;
    // <Ctrl+'q'> ou <ESC> : sort du programme ------------------------
    case 27 : case 17  : gfl_Quit();
    // <ESPACE> stoppe/relance l'animation <SPACE> --------------------
    case 32 : 
      glutSetWindow(mainwin);
      #ifdef _USE_GL_IDLE_
         // mise a jour de la fonction d'anim
         _GFL_RUN_=!_GFL_RUN_;
         glutIdleFunc((_GFL_ANIM_&&_GFL_RUN_)?(_GFL_FAFF_>1?_anim_F_:_anim_0_):NULL);
      #else
        gfl_Stop(); 
      #endif
      break;
    // info -----------------------------------------------------------
    case '?': glutSetWindow(mainwin); _GFL_INFO_ =!_GFL_INFO_ ; break;
    #ifdef _GFL2D_
      // zoom_value (spécifique 2D) -------------------------------------------
      case '+': glutSetWindow(mainwin); zoom_value*=1.1;  break;
      case '-': glutSetWindow(mainwin); zoom_value/=1.1;  break;
      case '=': glutSetWindow(mainwin); zoom_value =1.0;
                gfl_ResetOffset();/* recentrage */
                break;
    #endif
    #ifdef _GFL3D_
      case '\t': _FILL_=!_FILL_; break;
      // déplacement du _main_spot_ lumineux (spécifique 3D) ------------------
      case 'X' : _main_spot_.pos.x+=_main_spot_.dx; break;
      case 'x' : _main_spot_.pos.x-=_main_spot_.dx; break;
      case 'Y' : _main_spot_.pos.y+=_main_spot_.dy; break;
      case 'y' : _main_spot_.pos.y-=_main_spot_.dy; break;
      case 'Z' : _main_spot_.pos.z+=_main_spot_.dz; break;
      case 'z' : _main_spot_.pos.z-=_main_spot_.dz; break;
    #endif
    // les autres actions liees a une touche, définies par l'utilisateur
    default  : glutSetWindow(mainwin); 
               if (gfl_ExecuteKeyAction(c)) glutPostRedisplay();
               else gfl_warning_("KEY '%c' (%d) : nothing attached\t\r",c,(int)c);
  }
  glutPostWindowRedisplay(mainwin);
}
