/**@file    gfl.h
 * @author  Universite Gustave Eiffel
 * @author  E.Incerti - eric.incerti@univ-eiffel.fr
 * @brief   Base public control functions
 * @version 6.e
 * @date    Aug.2022 (doc generation)
 */
/**@cond SHOW_SECURITY_DEFINE */
#ifdef __cplusplus
  extern "C" {
#else
  #define _GNU_SOURCE
#endif

#ifdef _GFL3D_
  #ifndef _GFL_CAMLIGHT_3D_H
    #define _GFL_CAMLIGHT_3D_H
  /**@endcond                   */
  
    #include <gfl_types.h>
    #include <gfl_colors.h>
    #include <gfl_transfo3d.h>
  
    /**================================================================*
      * Gestion de la Caméra et de la Lumière                          *
      *================================================================**/
  
    /**-----------------------------------------------*
      *                 GESTION CAMERA                *
      *-----------------------------------------------**/
  
    /** Le type Camera **/
    /** position et cible (tar) sont des pointeurs pour pouvoir "brancher"    *
      * la caméra sur n'importe quelle paire de points existant dans la scène *
      * => gfl_SetCameraTracking(GFLpoint *position, GFLpoint *target);       **/
    typedef struct
    {
      GFLpoint  *pos; /** point d'attache (point de la scene)    **/
      GFLpoint  *tar; /** point de visee  (point de la scene)    **/
      GFLvector  hrz,upv,vis; /**  repère local **/
      /** coord. spheriques dans le repere local                 **/
      double     theta,phi,dist,dx,dy;
      /** parametres d'ouverture (-> man gluLookAt)              **/
      double     near,far,open;
      /** matrices directe et inverse                            **/
      GFLhmat    Md,Mi;
    } GFLcamera;
  
    /**  parametre camera (cf. <gluLookat>)                      **/
    void gfl_SetPerspective(float near, float far, float open);
  
    /**  coord. spheriques  + point de visee                     **/
    void gfl_SetCameraTheta(double   theta);
    void gfl_SetCameraPhi  (double   phi  );
    void gfl_SetCameraDist (double   dist );
    void gfl_SetCameraTar  (GFLpoint target);
  
    /**  Mise à jour des coord. sphériques et des matrices Md/Mi **/
    void gfl_SetCameraSpheric(double theta, double phi, double dist);
  
    /**  meme chose en cartesien (peu utile....)                 **/
    void gfl_SetCameraCartesian(GFLpoint position, GFLpoint target);
  
    /** pour attacher la camera a une paire de points mobiles    *
      * de la scene. Si l'un des pointeurs est NULL, le point    *
      * associé n'est pas modifié                                *
      * => gfl_SetCameraTracking(NULL,&A) : la caméra s'oriente  *
      *    toujours vers A sans se déplacer                      **/
    void gfl_SetCameraTracking(GFLpoint *position, GFLpoint *target);
  
    /** recupere un pointeur sur la camera **/
    GFLcamera* gfl_GetCamera(void);
  
    /** recupere la distance de la camera a la cible **/
    double gfl_GetCameraDist(void);
  
    /**-----------------------------------------------**/
    /**               GESTION LUMIERE                 **/
    /**-----------------------------------------------**/
    /** le type Lumiere**/
    typedef struct
    {
      GFLpoint   pos;            /** position  cartesienne       **/
      double    theta,phi,dist;  /** coordonnees spheriques      **/
      /** couleurs de la lumiere                                 **/
      GFLcolor   ambi;           /** ambiante                    **/
      GFLcolor   diff;           /** diffuse                     **/
      GFLcolor   spec;           /** speculaire                  **/
      double     dx,dy,dz;       /** pas de deplacement clavier  **/
    } GFLlight;
  
  
    /** recupere un pointeur sur la lumière **/
    GFLlight* gfl_GetLight(void);
  
    /** fixe les param. colorimetriques du spot lumineux**/
    void gfl_SetLightAmbient  (float x, float y, float z);
    void gfl_SetLightDiffuse  (float x, float y, float z);
    void gfl_SetLightSpecular (float x, float y, float z);
    /** fixe la position du spot lumineux               **/
    void gfl_SetLightPosition (float x, float y, float z);
    /** fixe les pas de deplacement du spot ('x'/'X', 'y'/'Y', 'z'/'Z',)**/
    void gfl_SetSpotSteps     (float dx, float dy, float dz);
    /**  les 3 coord. spheriques + point de visee**/
    void gfl_SetLightSpheric(double theta, double phi, double dist);
    /**  meme chose en cartesien**/
    void gfl_SetLightCartesian(GFLpoint position) ;
  #endif
#endif

#ifdef __cplusplus
  }
#endif
