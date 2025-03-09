#include <gfl.h>

/* des couleurs prédéfinies */
static GFLcolor rouge  ={1.0,0.0,0.0,1.0};
static GFLcolor jaune  ={1.0,1.0,0.0,1.0};
static GFLcolor vert   ={0.0,1.0,0.0,1.0};
static GFLcolor cyan   ={0.0,1.0,1.0,1.0};
static GFLcolor bleu   ={0.0,0.0,1.0,1.0};
static GFLcolor magenta={1.0,0.0,1.0,1.0};


GFLpoint  A,B,I,K;
GFLvector u,v;
bool      flag=false;

bool distdroites_2(GFLpoint A, GFLvector u, GFLpoint *I, GFLpoint B, GFLvector v, GFLpoint *K)
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


/*! fonction de dessin : passée à <gfl_SetDrawFunction> !*/
/*! ne fait aucun calcul, que du dessin                 !*/
void draw(void)
{
  glDisable(GL_LIGHTING);     /* <BALISE.GL>  "débranche" la lumière, pour permettre le tracé en mode point/ligne */

    /*! tracé de la "droite" [A,u) - En fait on trace le segment [A-10*u,A+10*u]                                 !*/
    gfl_Color4fv(bleu);       /* <VAR.ENV.GL> fonction gfl, emule la fonction glColor4fv(GLfloat [4])             */
    glLineWidth(1.0);         /* <VAR.ENV.GL> épaisseur de référence du trait (1.0 = val. par défaut)             */
    glBegin(GL_LINES);        /* <BALISE.GL>  début du bloc de tracé, en mode ligne                               */
      glVertex3d(A.x-10*u.x,A.y-10*u.y,A.z-10*u.z); /* 2 points, à relier par une ligne                           */
      glVertex3d(A.x+10*u.x,A.y+10*u.y,A.z+10*u.z);
    glEnd();                  /* <BALISE.GL>  fin du bloc                                                         */

    /*! tracé du segment [A,A+u] pour visualiser la postion de A et la direction de u                            !*/
    gfl_Color4fv(jaune);
    glPointSize(4.0);         /* <VAR.ENV.GL> taille de référence du point                                        */
    glBegin(GL_POINTS);       /* <BALISE.GL>  début du bloc de tracé, en mode "point"                             */
      gfl_Vertex3dv(A);       /*              fonction gfl, emule la fonction glVertex3dv(GLdouble [3])           */
    glEnd();
    glLineWidth(2.0);         /* <VAR.ENV.GL> on change l'épaisseur du trait,                                     */
    glBegin(GL_LINES);        /* <BALISE.GL>  début du bloc                                                       */
      glVertex3d(A.x,A.y,A.z);
      glVertex3d(A.x+u.x,A.y+u.y,A.z+u.z);
    glEnd();                  /* <BALISE.GL>  fin du bloc                                                         */

    /*! tracé de la "droite" [B,v)                                                                               !*/
    gfl_Color4fv(vert);
    glLineWidth(1.0);
    glBegin(GL_LINES);
      glVertex3d(B.x-10*v.x,B.y-10*v.y,B.z-10*v.z);
      glVertex3d(B.x+10*v.x,B.y+10*v.y,B.z+10*v.z);
    glEnd();
    /*! tracé du segment [B,B+v]                                                                                 !*/
    gfl_Color4fv(jaune);
    glPointSize(4.0);
    glBegin(GL_POINTS);
      gfl_Vertex3dv(A);
    glEnd();
    glLineWidth(2.0);
    glBegin(GL_LINES);
      glVertex3d(B.x,B.y,B.z);
      glVertex3d(B.x+v.x,B.y+v.y,B.z+v.z);
    glEnd();

  if (flag==true) /* si le calcul à donné un résultat : on trace [I,K] */
  {
    gfl_Color4fv(rouge);
    glPointSize(3.0);
    glBegin(GL_POINTS);
      gfl_Vertex3dv(I);
      gfl_Vertex3dv(K);
    glEnd();
    glLineWidth(1.);
    glBegin(GL_LINES);
      gfl_Vertex3dv(I);
      gfl_Vertex3dv(K);
    glEnd();
  }

  glEnable(GL_LIGHTING);      /* <BALISE.GL>  "rebranche" la lumière, pour permettre le tracé en mode "3D"      */
}


/*! fonction d'initialisation : passée à <gfl_SetInitFunction>      !*/
/*! ne dessine rien, fait éventuellement des calculs (1 seule fois) !*/
void init(void)
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

int main(int argc, char** argv)
{
  /* initialisation de la fenêtre graphique et paramétrage Gl */
  gfl_InitWindow(*argv,768,512);

  gfl_SetInitFunction(init);
  gfl_SetDrawFunction(draw);

  return gfl_MainStart();
}
