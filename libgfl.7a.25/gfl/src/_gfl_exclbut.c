/*!===============================================================
  E.Incerti - Universite Gustave Eiffel - eric.incerti@univ-eiffel.fr
       - Librairie GFL - Fonctions de base d'acces public -
                    version 6e - aout 2022
            création Hugo PETIOT (stage M1) Mai 2022
  ================================================================!*/

/*===================================*/
/*=  GESTION DES BOUTONS POUSSOIRS  =*/
/*===================================*/
/**@brief describe button mutual exclusive
 * All button must be ascociated to a group. maximum 16 groups for a maximum of 32 buttons.*/
typedef struct
{ void (*action)(void);  /**@brief the action associated to the button
                          * will only be run when calling @see applyExclusiveButtonGroupe
                          * using this button group's id as parameter       */
  int exclusiveGrp;      /**@brief id of the exclusive group it belongs     */
  bool on;               /**@brief wether the button is active or not
                          * automatically switched to <false> when an other
                          * button of the same group switches to true.      */
  int  num;              /**@brief the button number                       */
  int  x,y;              /**@brief button coordinates on the display       */
  int  len;              /**@brief button label length                     */
  char name[_GFL_NAMESIZE_+1]; /**@brief the button name                         */
  char info[_GFL_INFOSIZE_+1]; /**@brief text for information pannel (key '?')   */
}GFLExclBut;

/*                                                                        */
/**@brief Group of exclusive button
 * Such group can contain up to 32 buttons if all button are in the same group.
 * Maximum number of group is BLOCKSIZE / 2 as there should be at least 2 button per group.
 * @see GFLExclBut                                                        */
typedef struct
{
  GFLExclBut* active;    /**@brief pointer to the currently active member */
  char name[_GFL_NAMESIZE_+1]; /**@brief The group's name
                          * @todo implement display                       */
  int len;               /**@brief name length                            */
  GFLExclBut* members;   /**@brief a table of member buttons.             */
  int nbMembers;         /**@brief the number of members.                 */
} GFLexclusiveGrp;

/*========================================================================*/

// static GFLExclBut    *_EXCL_BUTTON_=NULL,*TheButton=NULL;
static int GROUP_NUMBER = _GFL_BLOCSIZE_ / 2;
static GFLexclusiveGrp* _EXCL_GRPS_ = NULL;
static GFLExclBut** _EXCL_BUTTON_=NULL;
static int       exclbutnum=0;
static int       grpnum = -1;

static int _excl_button_id_on_, _excl_button_id_off_ = 0;

extern bool gfl_CreateExclusiveButtonGroup(const char *name, int* grpId)
{
  if(!_EXCL_GRPS_)
  {
    if (!(_EXCL_GRPS_=(GFLexclusiveGrp*)calloc(GROUP_NUMBER,sizeof(GFLexclusiveGrp)))) return -1;
  }
  grpnum++;
  GFLexclusiveGrp grp = _EXCL_GRPS_[grpnum];
  grp.active = NULL;
  strncpy(grp.name, name, _GFL_NAMESIZE_);
  for (int i = 0; grp.name[i]!= '\0'; i++)
  {
   grp.len += glutBitmapWidth(GLUT_BITMAP_HELVETICA_10,grp.name[i]);
  }
  grp.nbMembers = 0;
 *grpId = grpnum;
  grp.active = NULL;

  return true;
}

/*=  Attribue un numero et un texte au bouton  =*/
extern int gfl_CreateExclusiveButton(const char *name, void (*action)(void), const char *info, int grpId)
{
  int memberNumber;
  GFLexclusiveGrp* targetGrp;
  GFLExclBut* but;
  if(grpnum < grpId)
  {
    fprintf(stderr, "Couldn't create exclusive button <%s>, belonging to group n°%d: inexistant group\n", name, grpId);
    return false;
  }
  if (!_EXCL_BUTTON_)
  {
    if (!(_EXCL_BUTTON_=(GFLExclBut**)calloc(_GFL_BLOCSIZE_,sizeof(GFLExclBut*)))) return false;
  }
  if(!_EXCL_GRPS_[grpId].members){
    if (!(_EXCL_GRPS_[grpId].members=(GFLExclBut*)calloc(_GFL_BLOCSIZE_,sizeof(GFLExclBut)))) return false;
  }
  targetGrp = &_EXCL_GRPS_[grpId];
  memberNumber = targetGrp->nbMembers;
  but = &(targetGrp->members[memberNumber]);

  strncpy(but->name,name,_GFL_NAMESIZE_);
  if (info) strncpy(but->info,info,127);
  but->len = 0;
  for(int i = 0; but->name[i]; i++){
    but->len+=glutBitmapWidth(GLUT_BITMAP_HELVETICA_10,but->name[i]);
  }
  Ydialwidth  = MAX(Ydialwidth,but->len+8);
  Xbutpos     = Ydialwidth/2;
  Xbutw       = Ydialwidth/2-2;
  _EXCL_BUTTON_[exclbutnum++] = but;
  but->num  = (targetGrp->nbMembers)++;
  but->x    = Xbutpos;
  but->y    = Ybutpos; Ybutpos+=22;
  but->on   = false;
  but->action = action;
  but->exclusiveGrp = grpId;

  return true;
}


extern void applyExclusiveButtonGroupe(int grpId){
  if(_EXCL_GRPS_[grpId].active != NULL)
  {
    _EXCL_GRPS_[grpId].active->action();
  }
}

/*= Si un bouton est selectionne, son numero =*/
/*= est mis dans la var. glob. <TheButton>   =*/
static __inline__ bool gfl_SelectExclButton(int x, int y)
{
  if (!_EXCL_BUTTON_){
    return false;
  }
  GFLExclBut **but = _EXCL_BUTTON_;
  for(int i = 0; but[i] != _EXCL_BUTTON_[exclbutnum]; i++){
    if(abs(but[i]->x-x)<2*Xbutw && abs(but[i]->y-y)<10){
      pushmode=GLUT_DOWN;

      int num = but[i]->num;
      GFLexclusiveGrp* grp = &(_EXCL_GRPS_[(but[i])->exclusiveGrp]);

      if(but[i]->on){
        but[i]->on = false;
        grp->active = NULL;
        return true;
      }
      for (int j = 0; j < grp->nbMembers; j++){
        grp->members[j].on = false;
      }
      grp->members[num].on = true;
      grp->active = &(grp->members[num]);
      return true;
    }
  }
  return false;
}

/*= libere les bouttons        =*/
/* obsolète -> garbage collector
static __inline__ void gfl_FreeExclButtons()
{
  if(_EXCL_GRPS_)
  {
    for(int i = 0; i < grpnum; i++)
    {
      free(_EXCL_GRPS_[i].members);
    }
    free(_EXCL_GRPS_);
  }
  if (_EXCL_BUTTON_)
  {
    free(_EXCL_BUTTON_);
  }
}*/

static __inline__ void gfl_InitExclButOn()
{
  _excl_button_id_on_ = glGenLists(GROUP_NUMBER);
  float hue_graduation = 1 / (float)GROUP_NUMBER;
  for (int i = 0; i < GROUP_NUMBER; i++){
    glNewList(_excl_button_id_on_+i, GL_COMPILE);
      glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
      glPolygonMode(GL_BACK, GL_LINE);
      glBegin(GL_TRIANGLE_FAN);
        gfl_Color4fv(gfl_hsva_rgba_4f(hue_graduation * i, 0.8, 0.75, 1));

        glVertex2i(0,0);
        gfl_Color4fv(gfl_hsva_rgba_4f(hue_graduation * i, 0.8, 0.75, 1));

        glVertex2i(-Xbutw,-8);
        glVertex2i(+Xbutw,-8);
        glVertex2i(+Xbutw,+8);
        glVertex2i(-Xbutw,+8);
        glVertex2i(-Xbutw,-8);
      glEnd();
      glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glEndList();
  }
}

static __inline__ void gfl_InitExclButOff()
{
  float hue_graduation = 1 / (float)GROUP_NUMBER;

  _excl_button_id_off_ = glGenLists(GROUP_NUMBER);
  for(int i = 0; i < GROUP_NUMBER; i++){
    glNewList(_excl_button_id_off_+i, GL_COMPILE);
      glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
      glPolygonMode(GL_BACK, GL_LINE);// permet de faire apparaitre les bugs d'orientation.
      glBegin(GL_TRIANGLE_FAN);
        gfl_Color4fv(gfl_hsva_rgba_4f(hue_graduation * i, 0.5, 0.75, 1));
        glVertex2i(0,0);
        gfl_Color4fv(gfl_hsva_rgba_4f(hue_graduation * i, 0.5, 0.75, 1));
        glVertex2i(-Xbutw,-8);
        glVertex2i(+Xbutw,-8);
        glVertex2i(+Xbutw,+8);
        glVertex2i(-Xbutw,+8);
        glVertex2i(-Xbutw,-8);
      glEnd();
      glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glEndList();
  }
}

/*=  dessinne tous les boutons  =*/
static __inline__ void gfl_DrawExclusiveButtons(void)
{
  GFLExclBut  **but=_EXCL_BUTTON_;
  GFLcolor col;
  char *c;

  while (but<_EXCL_BUTTON_+exclbutnum)
  {
    int grp = (*but)->exclusiveGrp;
    glPushMatrix();
      glTranslatef(Xbutpos,curheight-(*but)->y,0.);
      glCallList((*but)->on?_excl_button_id_on_+grp:_excl_button_id_off_+grp);
      glRasterPos2i(12,-4);
      col   = GFLk;
      col.a = 1.-col.a;
      gfl_Color4fv(col);
      glRasterPos2i(-(*but)->len/2,-4);
      for (c=(*but)->name; *c!='\0'; c++) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10,*c);
      glRasterPos2i(1-(*but)->len/2,-4);
      for (c=(*but)->name; *c!='\0'; c++) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10,*c);
    glPopMatrix();
    but++;
  }
}
