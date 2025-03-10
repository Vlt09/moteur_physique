/*!=================================================================!*/
/*!= E.Incerti - eric.incerti@univ-eiffel.fr                       =!*/
/*!= Université Gustave Eiffel                                     =!*/
/*!= Code "squelette" pour prototypage avec libgfl.7e              =!*/
/*!=================================================================!*/

#include <string>
#include <ostream>
#include <sstream>
#include <iostream>
#include <vector>
#include <memory>

/* le seul #include nécessaire a priori
 * contient les libs C standards et OpenGl */
#include "include/PMat.hpp"
#include "include/Link.hpp"

/* tailles initiales de la fenêtre graphique (en pixels) */
#define WWIDTH 720
#define WHEIGHT 540

static unsigned int nbParticle = 10;
static double partRadius = 0.1;

/* variables de contrôle */
static int Fe; // Echantillonage
static int Fa = 1;
static int tempo = 1000;
static float h; // 1./Fe

static GFLvector g = (GFLvector){0.0f, 0.0f};
static std::vector<std::shared_ptr<PMat>> pmats;
static std::vector<Link> links;


void moveParticle(void){
  GFLvector t = (GFLvector){-9.5f, 0.0f};
  pmats[0]->addForce(t);
}

void initFlag(float k){
  for (int i = 0; i < 10; i++){
    for (int j = 0; j < 10; j++){
      if (j == 0){
        auto pmat = std::make_shared<PMat>(PMat(1., (GFLpoint){-5., 5. - (i * 0.5)}, (GFLvector){0.f, 0.f}, PMat::Model::LEAP_FROG));
        pmats.emplace_back(pmat);
        links.emplace_back(Link::Extern_Force(*pmat, &g));
      }
      else{
        auto pmat = std::make_shared<PMat>(PMat((GFLpoint){-4.5 + (j * 0.5), 5. - (i * 0.5)}));
        pmats.emplace_back(pmat);
        links.emplace_back(Link::Hook_Spring(*pmats[j - 1], *pmats[j], k, 0));        
      }
    }
  }
}


/* -----------------------------------------------------------------------
 * ici, en général pas mal de variables GLOBALES
 * - les variables de données globales (points, vecteurs....)
 * - les FLAGS de dialogues
 * - les paramètres de dialogue
 * - ......
 * Pas trop le choix, puisque TOUT passe par des fonctions <void f(void)>
 * ----------------------------------------------------------------------- */

/* la fonction d'initialisation : appelée 1 seule fois, au début     */
static void init(void)
{
  Fe = 1000;
  h = 1. / (float)Fe;

  float k = 0.000001 * SQR(Fe); // on suppose que m = 1
  float z = 0.00005 * (float)Fe;

  int step = 2;

  // for (int i = 0; i < nbParticle; i++)
  // {
  //   if (i == 0 || i == nbParticle - 1){
  //     auto pmat = std::make_shared<PMat>(PMat((GFLpoint){-2.5 + (i * 1.5), 0.}));
  //     pmats.emplace_back(pmat);
  //     links.emplace_back(Link::Extern_Force(*pmat, &g));
  //   }
  //   else{
  //     auto pmat = std::make_shared<PMat>(PMat(1.f, (GFLpoint){-2.5 + (i * 1.5), 0.}, (GFLvector){0.f, 0.f}, PMat::Model::LEAP_FROG));
  //     pmats.emplace_back(pmat);
  //     links.emplace_back(Link::Extern_Force(*pmat, &g));

  //   }

  // }

  // for (int i = 0; i < nbParticle - 1; i++)
  // {
  //   links.emplace_back(Link::Hook_Spring(*pmats[i], *pmats[i + 1], k, 0));
  // }

  initFlag(k);

  gfl_SetKeyAction('a', moveParticle, nullptr);
}

/* la fonction de contrôle : appelée 1 seule fois, juste APRES <init> */
static void ctrl(void)
{
  /* un scrollbar vertical associé à la variable <tempo> : varie dans [0,5000] */
  gfl_CreateScrollv_i("tempo", &tempo, 0, 300, "temporisation (micro-sec.)");
  gfl_CreateScrollv_i("Fa", &Fa, 1, 20, "Fa");

  gfl_CreateScrollh_d("Gravity x", &(g.x), 0, 9, "contrainte ext en x");
  gfl_CreateScrollh_d("Gravity y", &(g.y), 0, 9, "contrainte ext en y");

}

/* la fonction de contrôle : appelée 1 seule fois, juste APRES <init> */
static void evts(void)
{
  /*! Interface de dialogue (partie dynamique) : les touches clavier, la souris ....
   *  Tout ce qu'il y a ici pourrait être directement écrit dans la fonction draw(),
   *  mais c'est plus 'propre' et plus pratique de séparer.
  !*/
  gfl_SetDelay(tempo); /* réglage de la temporisation (micro-secondes) */
  gfl_SetRefreshFreq(Fa);
}

/* la fonction de dessin : appelée en boucle (indispensable) */
static void draw(void)
{

  for (int i = 0; i < pmats.size(); i++)
  {
    gfl_DrawFillCircle(pmats[i]->position(), partRadius, GFLo);
  }

  for (int i = 0; i < links.size(); i++)
  {
    if (links[i]._type != Link::EXTERNAL_FORCE)
      gfl_DrawLine(links[i].getPmat1().position(), links[i].getPmat2().position(), GFLg, 1.f);
  }

  // gfl_Axes(); /* spécifique 2D : affiche le repère principal (O,x,y) */
}

/* la fonction d'animation : appelée en boucle draw/anim/draw/anim... (facultatif) */
static void anim(void)
{
  for (int i = 0; i < links.size(); i++)
  {
    links[i].update();
  }

  for (int i = 0; i < pmats.size(); i++)
  {
    pmats[i]->update(h);
  }
}

/* la fonction de sortie  (facultatif) */
static void quit(void)
{
  /*! Ici, les opérations à réaliser en sortie du programme
   *  - libération de la mémoire éventuellement alloueé dans <init()>
   *  - fermeture de fichiers ....
   *  - bilan et messages...
   *  Au final cette fonction est exécutée par un appel à <atexit()>
  !*/
}

/***************************************************************************/
/* La fonction principale : NE CHANGE (presque) JAMAIS                     */
/***************************************************************************/
int main(int argc, char **argv)
{
  /* 1°) creation de la fenetre - titre et tailles (pixels)  */
  gfl_InitWindow(*argv, WWIDTH, WHEIGHT);
  /* 2°) définition de la zone de travail en coord. réeelles *
   *     par défaut (pas d'initialisation) la zone 'réelle'  *
   *     est [(0.,0.),(WWIDTH,WHEIGHT)]                      */
  gfl_SetCenteredDrawZone(0., 0., 10., 0.);
  /* --- autre façon de faire ---
   *     ATTENTION : veiller à respecter les proportions
   *                 (wxmax-wxmin)/(wymax-wymin) = WWIDTH/WHEIGHT
   */
  // double wxmin = -10., wymin = -10.,
  // wxmax = +10., wymax = +10.;
  // gfl_SetWindowCoord(wxmin,wymin,wxmax,wymax);

  /* 3°) association des fonctions */
  gfl_SetInitFunction(init); /* fonction d'initialisation */
  gfl_SetCtrlFunction(ctrl); /* fonction de contrôle      */
  gfl_SetEvtsFunction(evts); /* fonction d'événements     */
  gfl_SetDrawFunction(draw); /* fonction de dessin        */
  gfl_SetAnimFunction(anim); /* fonction d'animation      */
  gfl_SetExitFunction(quit); /* fonction de sortie        */

  /* 4°) lancement de la boucle principale */
  return gfl_MainStart();
  /* RIEN APRES CA */
}
