/* ===============================================================
  E.Incerti - Université Gustave Eiffel - eric.incerti@univ-eiffel.fr
  ============================================================= */
#include <gfl.h>

/* tailles initiales de la fenêtre graphique (en pixels)     */
#define WWIDTH  720 
#define WHEIGHT 540

/* des couleurs prédéfinies */
static GFLcolor rouge  ={1.0,0.0,0.0,1.0};
static GFLcolor jaune  ={1.0,1.0,0.0,1.0};
static GFLcolor vert   ={0.0,1.0,0.0,1.0};
static GFLcolor cyan   ={0.0,1.0,1.0,1.0};
static GFLcolor bleu   ={0.0,0.0,1.0,1.0};
static GFLcolor magenta={1.0,0.0,1.0,1.0};
/* paramètres géométriques */
static double angle= 0.00;
static double rayon= 0.66;
/* paramètres de lumière/matière */
static double alpha= 0.5;
static double ambi = 0.2;
static double diff = 0.3;
static double spec = 0.4;
static double shin = 0.5;

/* une color-map à 25 teintes */
#define MAXCOL 25
static GFLcolor colmap[MAXCOL];

static GFLvector W={1.,2.,3.};
static double    b=0.1;


/* flag d'affichag/masquage */
static bool FLAG_TEAPOT=true;
static bool FLAG_TORUS =true;
static bool FLAG_CONE  =true;
static bool FLAG_ICOS  =true;


/*= action : variation de couleur =*/
static void action1(void)
{
  rouge.r-=0.01;  rouge.g+=0.01;
  vert.g -=0.01;  vert.b +=0.01;
  bleu.b -=0.01;  bleu.r +=0.01;
}
/*= action : variation de couleur =*/
static void action2(void)
{
  rouge.r+=0.01;  rouge.g-=0.01;
  vert.g +=0.01;  vert.b -=0.01;
  bleu.b +=0.01;  bleu.r -=0.01;
}

/*= action : affiche les parametre camera dans le terminal =*/
static void camera_info(void)
{
  GFLcamera* cam=gfl_GetCamera();
  fprintf(stderr,"position (x:%lf,y:%lf,z:%lf)\n",(*cam->pos).x,(*cam->pos).y,(*cam->pos).z);
  fprintf(stderr,"cible    (x:%lf,y:%lf,z:%lf)\n",(*cam->tar).x,(*cam->tar).y,(*cam->tar).z);
  fprintf(stderr,"coord. spheriques (d:%lf,theta:%lf,phi:%lf)\n",cam->dist,cam->theta,cam->phi);
}

/*------------------------------------*/
/*     LES 6 HANDLERS PRINCIPAUX      */

/* 1°) fonction d'initialisation (loader) */
static void init(void)
{
  /* paramètres caméra */
  /* param. géométrique de la caméra. cf. gluLookAt(...) */
  gfl_SetPerspective(40.,100.,1.);
  /* position, orientation de la caméra */
  gfl_SetCameraSpheric(0.25*PI,+0.25*PI,6.);

  /* fixe les param. colorimétriques du spot lumineux */
  /* lumiere blanche (c'est les valeurs par defaut)   */
  gfl_SetLightAmbient (1.,1.,1.);
  gfl_SetLightDiffuse (1.,1.,1.);
  gfl_SetLightSpecular(1.,1.,1.);

  /* fixe la position et la direction du spot lumineux */
  /* (c'est les valeurs par defaut)                    */
  gfl_SetLightPosition (10.,10.,10.);

  /* initialisation d'une carte de couleurs */
  gfl_ColorMapHSV360(colmap,MAXCOL);
}

/* 2°) fonction de contrôle */
static void ctrl(void)
{
  gfl_CreateSwitch("teapot",&FLAG_TEAPOT,"affiche/masques la theiere  ");
  gfl_CreateSwitch("torus ",&FLAG_TORUS ,"affiche/masques le tore     ");
  gfl_CreateSwitch("cone  ",&FLAG_CONE  ,"affiche/masques le cone     ");
  gfl_CreateSwitch("icos  ",&FLAG_ICOS  ,"affiche/masques l'icosahedre");

  gfl_SetScrollWidth(6);
  gfl_CreateScrollv_d("ray",&rayon,0.1,  2.0,"rayon sphere     ");
  gfl_CreateScrollv_d("ang",&angle,0.0,360.0,"angle rotation   ");
  gfl_CreateScrollv_d("alf",&alpha,0.0,  1.0,"transparence cube");

  gfl_SetScrollWidth(4);
  gfl_CreateScrollh_d("shin",&shin,0.0,1.0,"intensite lumiere brillance ");
  gfl_CreateScrollh_d("spec",&spec,0.0,1.0,"intensite lumiere speculaire");
  gfl_CreateScrollh_d("diff",&diff,0.0,1.0,"intensite lumiere diffuse   ");
  gfl_CreateScrollh_d("ambi",&ambi,0.0,1.0,"intensite lumiere ambiante  ");

  /* action attachées à des touches */
  gfl_SetKeyAction('g',action1,"variation de couleur");
  gfl_SetKeyAction('G',action2,"variation de couleur");
  gfl_SetKeyAction('c',camera_info,"pos./dir. de la camera => terminal");
}

/* 3°) fonction de gestion des événements */
static void evts(void)
{
  /* rien à faire dans cet exempl */
}

/* 4°) fonction de dessin : du pur OpenGl ou presque */
static void draw(void)
{
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
    gfl_Material(rouge,ambi,diff,spec,shin,1.); 
    /* une sphere */
    glutSolidSphere(rayon,40,40);
    /* un cube transparent */
    /*--------------LIMITES DE LA TRANSPARENCE Gl ---------*/
    /* -> TESTER :  DEFINIR LE CUBE TRANSP. AVANT LA SPHERE */
    /* -> TESTER :  RENDRE LA SPHERE TRANSPARENTE           */
    gfl_Material(vert,ambi,diff,spec,shin,alpha);
    glutSolidCube(1.);
  glDisable(GL_BLEND);

  if (FLAG_TEAPOT)
  {
    glPushMatrix();
      glTranslatef(0.,0.,1.);
      glRotatef(angle,0.,0.,1.);
      glRotatef(90,1.,0.,0.);
      gfl_Material(bleu,ambi,diff,spec,shin,0.3);
    glDisable(GL_CULL_FACE);
      glutSolidTeapot(.5);
    glEnable(GL_CULL_FACE);
    glPopMatrix();
  }
  if (FLAG_TORUS)
  {
    glPushMatrix();
      glTranslatef(0.,0.,-1.);
      glRotatef(-angle,0.,0.,1.);
      glRotatef(45.,1.,0.,0.);
      glScalef(0.5,0.5,0.5);
      gfl_Material(jaune,ambi,diff,spec,shin,1.);
      glutSolidTorus(.2,1.,20,40);
    glPopMatrix();
  }
  if (FLAG_CONE)
  {
    glPushMatrix();
      glRotatef(angle,0.,0.,1.);
       glTranslatef(-2.,0.,0.);
       glScalef(0.5,0.5,0.5);
       gfl_Material(cyan,ambi,diff,spec,shin,1.);
      glutSolidCone(1.,1.,20,20);
    glPopMatrix();
  }
  if (FLAG_ICOS)
  {
    glPushMatrix();
      glTranslatef(2.,0.,0.);
      glRotatef(angle,0.,0.,1.);
      glScalef(0.5,0.5,0.5);
      gfl_Material(magenta,ambi,diff,spec,shin,1.);
      glutSolidIcosahedron();
    glPopMatrix();
  }

  /* les cubes animes -> cf fonction <Anim> */
  int i=0;
  double a=360./MAXCOL;
  glPushMatrix();
    glRotatef(-2.*b,0.,0.,W.z);
    while (i<MAXCOL)
    {
      gfl_Material(colmap[i],ambi,diff,spec,shin,1.);
      glRotatef(a,0.,0.,1.);
      glPushMatrix();
        glTranslatef(1.,0.,0.);
        glRotatef(i*a,1.,0.,0.);
        glRotatef(20.*b,W.x,W.y,W.z);
        glutSolidCube(1.25*PI/MAXCOL);
       glPopMatrix();
      i++;
    }
   glPopMatrix();
}

/* 5°) fonction d'animation */
static void anim(void)
{
  static double pas=0.1;
  b+=pas;
  W.x+=pas;
  W.y-=pas;
  W.z+=pas;
  if (W.x<-10. || W.x>+10.) pas=-pas;
}

/* 6°) fonction de sortie */
static void quit(void)
{
  fprintf(stdout,"\nbye !\n");
}


/***************************************************************************/
/* La fonction principale : NE CHANGE (presque) JAMAIS                     */
/***************************************************************************/
int main(int argc, char **argv)
{
  /* 1°) creation de la fenetre - titre et tailles (pixels) */
  gfl_InitWindow(*argv,WWIDTH,WHEIGHT);

  /* 2°) association des handlers */
  gfl_SetInitFunction(init); /* fonction d'initialisation */
  gfl_SetCtrlFunction(ctrl); /* fonction de contrôle      */
  gfl_SetEvtsFunction(evts); /* fonction d'événements     */
  gfl_SetDrawFunction(draw); /* fonction de dessin        */
  gfl_SetAnimFunction(anim); /* fonction d'animation      */
  gfl_SetExitFunction(quit); /* fonction de sortie        */

  /* 3°) lancement de la boucle principale */
  return gfl_MainStart();
  /* RIEN APRES CA */
}
