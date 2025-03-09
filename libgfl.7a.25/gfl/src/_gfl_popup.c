/*!===============================================================
  E.Incerti - Universite Gustave Eiffel - eric.incerti@univ-eiffel.fr
       - Librairie GFL - Fonctions de base d'acces public -
                    version 6e - aout 2022
  ================================================================!*/

/*===================================*/
/*=  GESTION DES BOUTONS POUSSOIRS  =*/
/*===================================*/
/**@brief single action button.
 * Just like a popup on internet, the action linked to this button is operated imediatly      */
typedef struct
{
  void (*action)(void);  /**@brief the linked action as a function pointer */
  bool on;               /**@brief wether the button is active or not      */
  int  num;              /**@brief the button number                       */
  int  x,y;              /**@brief button coordinates on the display       */
  int  len;              /**@brief button label length                     */
  char name[_GFL_NAMESIZE_+1]; /**@brief the button name                         */
  char info[_GFL_INFOSIZE_+1]; /**@brief text for information pannel (key '?')   */
} GFLpopup;

static GFLpopup *_POPUP_=NULL,*ThePopUp=NULL;
static int       popnum=0;

static int _popup_id_on_=0,_popup_id_off_=0;

/*=  Attribue un numero et un texte au bouton  =*/
extern bool gfl_CreatePopUp(const char *name, void (*action)(void), const char *info)
{
  if (!_POPUP_)
    if (!(_POPUP_=(GFLpopup*)calloc(_GFL_BLOCSIZE_,sizeof(GFLpopup)))) return false;

  GFLpopup *pop = _POPUP_+popnum;

  strncpy(pop->name,name,_GFL_NAMESIZE_);
  if (info) strncpy(pop->info,info,127);
  pop->len=glutBitmapLength(GLUT_BITMAP_HELVETICA_10,(const uchar*)name);

  Ydialwidth  = MAX(Ydialwidth,pop->len+8);
  Xbutpos     = Ydialwidth/2;
  Xbutw       = Ydialwidth/2-2;

  pop->num    = popnum;
  pop->action = action;
  pop->on     = false;
  pop->x      = Xbutpos;
  pop->y      = Ybutpos; Ybutpos+=22;
  popnum++;
  return true;
}

/*=  Renvoie le numero du bouton selectionne =*/
extern int gfl_GetPopUp(void) { return ThePopUp?(int)ThePopUp->num:-1; }


/*= Si un bouton est selectionne, son numero =*/
/*= est mis dans la var. glob. <ThePopUp>   =*/
static __inline__ bool gfl_SelectPopUp(int x, int y)
{
  GFLpopup *pop=_POPUP_;
  while (pop<_POPUP_+popnum)
  {
    if (abs(pop->x-x)<2*Xbutw && abs(pop->y-y)<10)
    {
      pushmode=GLUT_DOWN;
      ThePopUp=pop;
      ThePopUp->on=true;
      return true;
    }
    pop++;
  }
  return false;
}

/*= libere les bouttons        =*/
/* obsolète -> garbage collector
static __inline__ void gfl_FreePopUps()
{
  if (_POPUP_)
  {
    free(_POPUP_);
    ThePopUp=_POPUP_=NULL;
  }
}*/

static __inline__ void gfl_InitPopOn()
{
  _popup_id_on_ = glGenLists(1);
  glNewList(_popup_id_on_, GL_COMPILE);
    glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
    glBegin(GL_TRIANGLE_FAN);
      gfl_Color4fv(GFLg_b);
      glVertex2i(0,0);
      gfl_Color4fv(GFLk_b);
      glVertex2i(-Xbutw,-8);
      glVertex2i(+Xbutw,-8);
      glVertex2i(+Xbutw,+8);
      glVertex2i(-Xbutw,+8);
      glVertex2i(-Xbutw,-8);
    glEnd();
    glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
  glEndList();
}

static __inline__ void gfl_InitPopOff()
{
  _popup_id_off_ = glGenLists(1);
  glNewList(_popup_id_off_, GL_COMPILE);
    glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
    glBegin(GL_TRIANGLE_FAN);
      gfl_Color4fv(GFLb_b);
      glVertex2i(0,0);
      gfl_Color4fv(GFLk_c);
      glVertex2i(-Xbutw,-8);
      glVertex2i(+Xbutw,-8);
      glVertex2i(+Xbutw,+8);
      glVertex2i(-Xbutw,+8);
      glVertex2i(-Xbutw,-8);
    glEnd();
    glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
  glEndList();
}


/*=  dessinne tous les boutons  =*/
static __inline__ void gfl_DrawPopUps(void)
{
  GFLpopup  *pop=_POPUP_;
  GFLcolor   col;
  while (pop<_POPUP_+popnum)
  {
    glPushMatrix();
      glTranslatef(Xbutpos,curheight-pop->y,0.);
      glCallList(pop->on?_popup_id_on_:_popup_id_off_);
      glRasterPos2i(12,-4);
      col = pop->on?GFLk:GFLw;
      col.a = 1.-col.a;
      gfl_Color4fv(col);
      glRasterPos2i(-pop->len/2,-4);
      glutBitmapString(GLUT_BITMAP_HELVETICA_10,(const uchar*)pop->name);
      glRasterPos2i(1-pop->len/2,-4);
      glutBitmapString(GLUT_BITMAP_HELVETICA_10,(const uchar*)pop->name);
    glPopMatrix();
    pop++;
  }
}
