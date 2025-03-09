#include <gfl.h>

/* tailles initiales de la fenêtre graphique (en pixels)     */
#define WWIDTH  720 
#define WHEIGHT 540

/* des couleurs prédéfinies */
static GFLcolor rouge  ={1.0,0.0,0.0,1.0};
static GFLcolor jaune  ={1.0,1.0,0.0,1.0};
static GFLcolor vert   ={0.0,1.0,0.0,1.0};
static GFLcolor bleu   ={0.0,0.0,1.0,1.0};

GFLpoint  A,B,I,K;
GFLvector u,v;
bool      flag=false;

static bool distdroites_2(GFLpoint A, GFLvector u, GFLpoint *I, GFLpoint B, GFLvector v, GFLpoint *K)
{
  double c = gfl_ProdScal(u,v);
  double d = (1.-c*c);
  if (fabs(d)<1.E-8) return false;
  d = 1./d;
  GFLvector AB=gfl_Vector2p(A,B);
  double a =+gfl_ProdScal(AB,u);
  double b =-gfl_ProdScal(AB,v);
  double ta = (a+b*c)*d;
  double tb = (b+a*c)*d;
  I->x = A.x+ta*u.x;
  I->y = A.y+ta*u.y;
  I->z = A.z+ta*u.z;
  K->x = B.x+tb*v.x;
  K->y = B.y+tb*v.y;
  K->z = B.z+tb*v.z;
  return true;
}


/*------------------------------------*/
/*      LES HANDLERS PRINCIPAUX       */

/* 1°) fonction d'initialisation (loader) */
static void init(void)
{
  A=(GFLpoint ){-1.0,-1.0,-1.0};
  u=(GFLvector){+0.5,+0.4,+0.2};
  gfl_Normalize(&u);
  B=(GFLpoint ){+1.0,+1.0,+1.0};
  v=(GFLvector){-0.2,-0.4,-0.5};
  gfl_Normalize(&v);
  /* l'algo. qui détermine les points I,K */
  flag = distdroites_2(A,u,&I,B,v,&K);
}


/* 4°) fonction de dessin : du pur OpenGl ou presque */
static void draw(void)
{
  glDisable(GL_LIGHTING);    /* <BALISE.GL>  "débranche" la lumière, pour permettre le tracé en mode point/ligne */
    glPointSize(3.);         /* <VAR.ENV.GL> taille de référence du point                                        */
    gfl_Color4fv(jaune);     /* fonction gfl, emule la fonction glColor4fv(GLfloat [4])                          */
    glBegin(GL_POINTS);      /* <BALISE.GL>  début du bloc de tracé, en mode "point"                             */
      gfl_Vertex3dv(A);      /* fonction gfl, emule la fonction glVertex3dv(GLdouble [3])                        */
      gfl_Vertex3dv(B);
    glEnd();                 /* <BALISE.GL>  fin du bloc                                                         */

    gfl_Color4fv(jaune);
    glBegin(GL_LINES);
      glVertex3d(A.x,A.y,A.z);
      glVertex3d(A.x+u.x,A.y+u.y,A.z+u.z);
    glEnd();

    glBegin(GL_LINES);
      glVertex3d(B.x,B.y,B.z);
      glVertex3d(B.x+v.x,B.y+v.y,B.z+v.z);
    glEnd();

    gfl_Color4fv(bleu);
    glLineWidth(2.);          /* taille de référence du point (en pixel)                             */
    glBegin(GL_LINES);        /* début du bloc de tracé, en mode ligne                               */
      glVertex3d(A.x-10*u.x,A.y-10*u.y,A.z-10*u.z); /* les points, à relier par des lignes           */
      glVertex3d(A.x+10*u.x,A.y+10*u.y,A.z+10*u.z);
    glEnd();                  /* fin du bloc de tracé                                                */

    gfl_Color4fv(vert);
    glLineWidth(2.);
    glBegin(GL_LINES);
      glVertex3d(B.x-10*v.x,B.y-10*v.y,B.z-10*v.z);
      glVertex3d(B.x+10*v.x,B.y+10*v.y,B.z+10*v.z);
    glEnd();

  if (!flag) { glDisable(GL_LIGHTING); return; }

    gfl_Color4fv(rouge);
    glPointSize(3.);
    glBegin(GL_POINTS);
      gfl_Vertex3dv(I);
      gfl_Vertex3dv(K);
    glEnd();
    glLineWidth(2.);
    glBegin(GL_LINES);
      glVertex3d(I.x,I.y,I.z);
      glVertex3d(K.x,K.y,K.z);
    glEnd();

  glEnable(GL_LIGHTING);
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
  gfl_SetCtrlFunction(NULL); /* fonction de contrôle      */
  gfl_SetEvtsFunction(NULL); /* fonction d'événements     */
  gfl_SetDrawFunction(draw); /* fonction de dessin        */
  gfl_SetAnimFunction(NULL); /* fonction d'animation      */
  gfl_SetExitFunction(NULL); /* fonction de sortie        */

  /* 3°) lancement de la boucle principale */
  return gfl_MainStart();
  /* RIEN APRES CA */
}
