/**
 * @file    gfl_colors.h
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

#ifndef _GFL_COLORS_H
  #define _GFL_COLORS_H
/**@endcond                   */

  #include <gfl_types.h>

  /**
   * @struct GFLcolor, GFLfcolor
   * @brief Couleur RGB.A (Red|Green|Blue|Alpha) sous forme d'une structure à 4 champs réels (float)
   * >>> Les 4 valeurs réelles doivent être dans l'intervalle [0,1]
   * Composition par Modèle Additif [https://fr.wikipedia.org/wiki/Synthèse_additive]
   * Peut aussi être utilisé au format HSV.A [https://fr.wikipedia.org/wiki/Teinte_Saturation_Valeur]
   * Attention OpenGl gere les couleurs via des tableaux <float rgba[4]> et non des structures
   * La libgfl propose quelques fonctions/macros de conversion
  **/
  typedef struct
  {
    float  r,g,b,a;
  } GFLcolor,GFLfcolor; // 2023.02.07 : alias GFLfcolor pour mixage avec SDL/libMLV

  /**
   * @struct GFLucolor (pour compatibilité libMLV/SDL)
   * @brief Couleur RGB.A (Red|Green|Blue|Alpha) sous forme d'un entier 32bits (4 octets 0xRRGGBBAA)
   * >>> chaque composante est codée sur 8 bits [0,255]
   * >>> format Hexa : 0xRRGGBBAA
   * Composition par Modèle Additif [https://fr.wikipedia.org/wiki/Synthèse_additive]
   * Peut aussi être utilisé au format HSV.A [https://fr.wikipedia.org/wiki/Teinte_Saturation_Valeur]
  **/
  typedef uint GFLucolor; // 2023.02.07 : alias GFLfcolor pour compatibilité avec libMLV

  GFLfcolor gfl_uColor_to_fColor(GFLucolor col);
  GFLucolor gfl_fColor_to_uColor(GFLfcolor col);

  /**
   * @brief pour compatibilité OpenGl - équivalents glColor3fv et glColor4fv
   * Existe en version fonctions et macros
  **/
  #define  GFLcolor3fv(col) glColor3f((col).r,(col).g,(col).b)
   void    gfl_Color3v(GFLcolor c);
  #define  GFLcolor4fv(col) glColor4f((col).r,(col).g,(col).b,(col).a)
   void    gfl_Color4fv(GFLcolor c);

  #ifdef _GFL3D_
    /*! pour compatibilité OpenGl - équivalent glMaterialfv!*/
    void gfl_Materialfv(GLenum face,GLenum pname, GFLcolor col);
  
    void gfl_Enable_Blend(void);
  
    /*! @brief ALTERNATIVE a la gestion de couleurs OpenGl
     *  mise en forme des parametres de couleur/matiere des objets
     *  tous les parametres dans [0,1]
     *  col    : teinte RGB
     *  ambi   : reaction a la lumiere ambiante (blanche par defaut)
     *  diff   : reaction a la lumiere diffuse (blanche par defaut)
     *  spec   : reaction a la lumiere de speculosite (blanche par defaut)
     *  shine  : coeff. de speculosite (max par defaut -- 256.    )
     *  transp : valeur de transparence (0.: objet totalement transp. / 1.:opaque)
    !*/
    void gfl_Material(GFLcolor col, float ambi, float diff, float spec, float shine, float alpha);
  #endif
  
  /**
   * @brief négatif RGB, RGBA : inverse les composantes (x -> (1.-x))
   * @fn GFLcolor gfl_Color_NegRGB(GFLcolor col)
   * @fn GFLcolor gfl_Color_NegRGBA(GFLcolor col)
  **/
  GFLcolor gfl_Color_NegRGB(GFLcolor col);
  GFLcolor gfl_Color_NegRGBA(GFLcolor col);

  /**
   * @brief mapping Couleur
   * toutes les valeurs sont automatiquement recalées dans l'intervalle [0,1]
   * @fn GFLcolor gfl_mapscal4(GFLcolor C, float    m)
   * renvoie la couleur [m*C.r|m*C.g|m*C.b|m*C.a]
   * @fn GFLcolor gfl_mapscal4(GFLcolor C, GFLcolor m)
   * renvoie la couleur [m.r*C.r|m.g*C.g|m.b*C.b|m.a*C.a]
  **/
  GFLcolor gfl_mapscal4(GFLcolor C, float    m);
  GFLcolor gfl_mapvect4(GFLcolor C, GFLcolor m);

  /**
   * @brief conversions RGBA<->HSVA :
   * toutes les valeurs (entrées/sorties) restent dans [0.,1.]
   * @fn GFLcolor gfl_hsva_rgba_4f(float h, float s, float v, float a);
   * @fn GFLcolor gfl_rgba_hsva_4fv(GFLcolor rgba);
   * @fn GFLcolor gfl_h110_rgba_1f(float h);
   * @fn GFLcolor gfl_hsva_rgba_4fv(GFLcolor hsva);
  **/
  GFLcolor gfl_hsva_rgba_4f(float h, float s, float v, float a);
  GFLcolor gfl_rgba_hsva_4fv(GFLcolor rgba);
  GFLcolor gfl_h110_rgba_1f(float h);
  GFLcolor gfl_hsva_rgba_4fv(GFLcolor hsva);

  /**
   * @brief Cartes de couleurs
   * rempli un tableau de n teintes etalonnees du rouge (0°) au rouge (360°)
   * la définition (allocation/libération) du tableau <umap> est à la charge de l'utilisateur
   * @fn void   gfl_ColorMapHSV360   (GFLcolor *umap, int n);
   * @fn void   gfl_ColorMapHSV360_va(GFLcolor *umap, int n, float v, float a);
   * @fn void   gfl_ColorMapHSV360_sa(GFLcolor *umap, int n, float s, float a);
   * @fn void   gfl_ColorMapHSV360_interv(GFLcolor *umap, int n, float min, float max);
  **/
  void   gfl_ColorMapHSV360   (GFLcolor *umap, int n);
  void   gfl_ColorMapHSV360_va(GFLcolor *umap, int n, float v, float a);
  void   gfl_ColorMapHSV360_sa(GFLcolor *umap, int n, float s, float a);
  void   gfl_ColorMapHSV360_interv(GFLcolor *umap, int n, float min, float max);
  /*! change la couleur n°<idx> de la table <umap> !*/
  void   gfl_SetIndexColor(GFLcolor* umap, int idx, GFLcolor col);
  /*! distance euclidienne entre 2 couleurs (champs r,g,b seulement) !*/
  double gfl_ColorDist(GFLcolor c1, GFLcolor c2);

  /* DES COULEURS PREDEFINIES                                */
  /* TRANSPARENCE 0.00                                       */
  /* intensite 100%                                          */
  #define GFLw    (GFLcolor){1.00,1.00,1.00,0.00} /* blanc   */
  #define GFLr    (GFLcolor){1.00,0.00,0.00,0.00} /* rouge   */
  #define GFLo    (GFLcolor){1.00,0.50,0.25,0.00} /* orange  */
  #define GFLy    (GFLcolor){1.00,1.00,0.00,0.00} /* jaune   */
  #define GFLg    (GFLcolor){0.00,1.00,0.00,0.00} /* vert    */
  #define GFLc    (GFLcolor){0.00,1.00,1.00,0.00} /* cyan    */
  #define GFLb    (GFLcolor){0.00,0.00,1.00,0.00} /* bleu    */
  #define GFLm    (GFLcolor){1.00,0.00,1.00,0.00} /* magenta */
  #define GFLk    (GFLcolor){0.00,0.00,0.00,0.00} /* noir    */
  /* intensite  75%                                          */
  #define GFLwa   (GFLcolor){0.75,0.75,0.75,0.00} /*         */
  #define GFLra   (GFLcolor){0.75,0.00,0.00,0.00} /*         */
  #define GFLya   (GFLcolor){0.75,0.75,0.00,0.00} /*         */
  #define GFLga   (GFLcolor){0.00,0.75,0.00,0.00} /*         */
  #define GFLca   (GFLcolor){0.00,0.75,0.75,0.00} /*         */
  #define GFLba   (GFLcolor){0.00,0.00,0.75,0.00} /*         */
  #define GFLma   (GFLcolor){0.75,0.00,0.75,0.00} /*         */
  /* intensite  50%                                          */
  #define GFLwb   (GFLcolor){0.50,0.50,0.50,0.00} /*         */
  #define GFLrb   (GFLcolor){0.50,0.12,0.12,0.00} /*         */
  #define GFLyb   (GFLcolor){0.50,0.50,0.12,0.00} /*         */
  #define GFLgb   (GFLcolor){0.12,0.50,0.12,0.00} /*         */
  #define GFLcb   (GFLcolor){0.12,0.50,0.50,0.00} /*         */
  #define GFLbb   (GFLcolor){0.12,0.12,0.50,0.00} /*         */
  #define GFLmb   (GFLcolor){0.50,0.12,0.50,0.00} /*         */
  /* intensite  25%                                          */
  #define GFLwc   (GFLcolor){0.25,0.25,0.25,0.00} /*         */
  #define GFLrc   (GFLcolor){0.25,0.12,0.12,0.00} /*         */
  #define GFLyc   (GFLcolor){0.25,0.25,0.12,0.00} /*         */
  #define GFLgc   (GFLcolor){0.12,0.25,0.12,0.00} /*         */
  #define GFLcc   (GFLcolor){0.12,0.25,0.25,0.00} /*         */
  #define GFLbc   (GFLcolor){0.12,0.12,0.25,0.00} /*         */
  #define GFLmc   (GFLcolor){0.25,0.12,0.25,0.00} /*         */
  /* intensite  12.5%                                        */
  #define GFLwd   (GFLcolor){0.12,0.12,0.12,0.00} /*         */
  #define GFLrd   (GFLcolor){0.12,0.06,0.06,0.00} /*         */
  #define GFLyd   (GFLcolor){0.12,0.12,0.06,0.00} /*         */
  #define GFLgd   (GFLcolor){0.06,0.12,0.06,0.00} /*         */
  #define GFLcd   (GFLcolor){0.06,0.12,0.12,0.00} /*         */
  #define GFLbd   (GFLcolor){0.06,0.06,0.12,0.00} /*         */
  #define GFLmd   (GFLcolor){0.12,0.06,0.12,0.00} /*         */

  /* TRANSPARENCE 0.25                                       */
  /* intensite 100%                                          */
  #define GFLw_a  (GFLcolor){1.00,1.00,1.00,0.75} /* blanc   */
  #define GFLr_a  (GFLcolor){1.00,0.00,0.00,0.75} /* rouge   */
  #define GFLo_a  (GFLcolor){1.00,0.50,0.00,0.75} /* orange  */
  #define GFLy_a  (GFLcolor){1.00,1.00,0.00,0.75} /* jaune   */
  #define GFLg_a  (GFLcolor){0.00,1.00,0.00,0.75} /* vert    */
  #define GFLc_a  (GFLcolor){0.00,1.00,1.00,0.75} /* cyan    */
  #define GFLb_a  (GFLcolor){0.00,0.00,1.00,0.75} /* bleu    */
  #define GFLm_a  (GFLcolor){1.00,0.00,1.00,0.75} /* magenta */
  #define GFLk_a  (GFLcolor){0.00,0.00,0.00,0.75} /* noir    */
  /* intensite  75%                                          */
  #define GFLwa_a (GFLcolor){0.75,0.75,0.75,0.75} /*         */
  #define GFLra_a (GFLcolor){0.75,0.00,0.00,0.75} /*         */
  #define GFLya_a (GFLcolor){0.75,0.75,0.00,0.75} /*         */
  #define GFLga_a (GFLcolor){0.00,0.75,0.00,0.75} /*         */
  #define GFLca_a (GFLcolor){0.00,0.75,0.75,0.75} /*         */
  #define GFLba_a (GFLcolor){0.00,0.00,0.75,0.75} /*         */
  #define GFLma_a (GFLcolor){0.75,0.00,0.75,0.75} /*         */
  /* intensite  50%                                          */
  #define GFLwb_a (GFLcolor){0.50,0.50,0.50,0.75} /*         */
  #define GFLrb_a (GFLcolor){0.50,0.12,0.12,0.75} /*         */
  #define GFLyb_a (GFLcolor){0.50,0.50,0.12,0.75} /*         */
  #define GFLgb_a (GFLcolor){0.12,0.50,0.12,0.75} /*         */
  #define GFLcb_a (GFLcolor){0.12,0.50,0.50,0.75} /*         */
  #define GFLbb_a (GFLcolor){0.12,0.12,0.50,0.75} /*         */
  #define GFLmb_a (GFLcolor){0.50,0.12,0.50,0.75} /*         */
  /* intensite  25%                                          */
  #define GFLwc_a (GFLcolor){0.25,0.25,0.25,0.75} /*         */
  #define GFLrc_a (GFLcolor){0.25,0.12,0.12,0.75} /*         */
  #define GFLyc_a (GFLcolor){0.25,0.25,0.12,0.75} /*         */
  #define GFLgc_a (GFLcolor){0.12,0.25,0.12,0.75} /*         */
  #define GFLcc_a (GFLcolor){0.12,0.25,0.25,0.75} /*         */
  #define GFLbc_a (GFLcolor){0.12,0.12,0.25,0.75} /*         */
  #define GFLmc_a (GFLcolor){0.25,0.12,0.25,0.75} /*         */
  /* intensite  12.5%                                        */
  #define GFLwd_a (GFLcolor){0.12,0.12,0.12,0.75} /*         */
  #define GFLrd_a (GFLcolor){0.12,0.06,0.06,0.75} /*         */
  #define GFLyd_a (GFLcolor){0.12,0.12,0.06,0.75} /*         */
  #define GFLgd_a (GFLcolor){0.06,0.12,0.06,0.75} /*         */
  #define GFLcd_a (GFLcolor){0.06,0.12,0.12,0.75} /*         */
  #define GFLbd_a (GFLcolor){0.06,0.06,0.12,0.75} /*         */
  #define GFLmd_a (GFLcolor){0.12,0.06,0.12,0.75} /*         */

  /* TRANSPARENCE 0.50                                       */
  /* intensite 100%                                          */
  #define GFLw_b  (GFLcolor){1.00,1.00,1.00,0.50} /* blanc   */
  #define GFLr_b  (GFLcolor){1.00,0.00,0.00,0.50} /* rouge   */
  #define GFLo_b  (GFLcolor){1.00,0.50,0.00,0.50} /* orange  */
  #define GFLy_b  (GFLcolor){1.00,1.00,0.00,0.50} /* jaune   */
  #define GFLg_b  (GFLcolor){0.00,1.00,0.00,0.50} /* vert    */
  #define GFLc_b  (GFLcolor){0.00,1.00,1.00,0.50} /* cyan    */
  #define GFLb_b  (GFLcolor){0.00,0.00,1.00,0.50} /* bleu    */
  #define GFLm_b  (GFLcolor){1.00,0.00,1.00,0.50} /* magenta */
  #define GFLk_b  (GFLcolor){0.00,0.00,0.00,0.50} /* noir    */
  /* intensite  75%                                          */
  #define GFLwa_b (GFLcolor){0.75,0.75,0.75,0.50} /*         */
  #define GFLra_b (GFLcolor){0.75,0.00,0.00,0.50} /*         */
  #define GFLya_b (GFLcolor){0.75,0.75,0.00,0.50} /*         */
  #define GFLga_b (GFLcolor){0.00,0.75,0.00,0.50} /*         */
  #define GFLca_b (GFLcolor){0.00,0.75,0.75,0.50} /*         */
  #define GFLba_b (GFLcolor){0.00,0.00,0.75,0.50} /*         */
  #define GFLma_b (GFLcolor){0.75,0.00,0.75,0.50} /*         */
  /* intensite  50%                                          */
  #define GFLwb_b (GFLcolor){0.50,0.50,0.50,0.50} /*         */
  #define GFLrb_b (GFLcolor){0.50,0.12,0.12,0.50} /*         */
  #define GFLyb_b (GFLcolor){0.50,0.50,0.12,0.50} /*         */
  #define GFLgb_b (GFLcolor){0.12,0.50,0.12,0.50} /*         */
  #define GFLcb_b (GFLcolor){0.12,0.50,0.50,0.50} /*         */
  #define GFLbb_b (GFLcolor){0.12,0.12,0.50,0.50} /*         */
  #define GFLmb_b (GFLcolor){0.50,0.12,0.50,0.50} /*         */
  /* intensite  25%                                          */
  #define GFLwc_b (GFLcolor){0.25,0.25,0.25,0.50} /*         */
  #define GFLrc_b (GFLcolor){0.25,0.12,0.12,0.50} /*         */
  #define GFLyc_b (GFLcolor){0.25,0.25,0.12,0.50} /*         */
  #define GFLgc_b (GFLcolor){0.12,0.25,0.12,0.50} /*         */
  #define GFLcc_b (GFLcolor){0.12,0.25,0.25,0.50} /*         */
  #define GFLbc_b (GFLcolor){0.12,0.12,0.25,0.50} /*         */
  #define GFLmc_b (GFLcolor){0.25,0.12,0.25,0.50} /*         */
  /* intensite  12.5%                                        */
  #define GFLwd_b (GFLcolor){0.12,0.12,0.12,0.50} /*         */
  #define GFLrd_b (GFLcolor){0.12,0.06,0.06,0.50} /*         */
  #define GFLyd_b (GFLcolor){0.12,0.12,0.06,0.50} /*         */
  #define GFLgd_b (GFLcolor){0.06,0.12,0.06,0.50} /*         */
  #define GFLcd_b (GFLcolor){0.06,0.12,0.12,0.50} /*         */
  #define GFLbd_b (GFLcolor){0.06,0.06,0.12,0.50} /*         */
  #define GFLmd_b (GFLcolor){0.12,0.06,0.12,0.50} /*         */

  /* TRANSPARENCE 0.75                                       */
  /* intensite 100%                                          */
  #define GFLw_c  (GFLcolor){1.00,1.00,1.00,0.25} /* blanc   */
  #define GFLr_c  (GFLcolor){1.00,0.00,0.00,0.25} /* rouge   */
  #define GFLo_c  (GFLcolor){1.00,0.50,0.00,0.25} /* orange  */
  #define GFLy_c  (GFLcolor){1.00,1.00,0.00,0.25} /* jaune   */
  #define GFLg_c  (GFLcolor){0.00,1.00,0.00,0.25} /* vert    */
  #define GFLc_c  (GFLcolor){0.00,1.00,1.00,0.25} /* cyan    */
  #define GFLb_c  (GFLcolor){0.00,0.00,1.00,0.25} /* bleu    */
  #define GFLm_c  (GFLcolor){1.00,0.00,1.00,0.25} /* magenta */
  #define GFLk_c  (GFLcolor){0.00,0.00,0.00,0.25} /* noir    */
  /* intensite  75%                                          */
  #define GFLwa_c (GFLcolor){0.75,0.75,0.75,0.25} /*         */
  #define GFLra_c (GFLcolor){0.75,0.00,0.00,0.25} /*         */
  #define GFLya_c (GFLcolor){0.75,0.75,0.00,0.25} /*         */
  #define GFLga_c (GFLcolor){0.00,0.75,0.00,0.25} /*         */
  #define GFLca_c (GFLcolor){0.00,0.75,0.75,0.25} /*         */
  #define GFLba_c (GFLcolor){0.00,0.00,0.75,0.25} /*         */
  #define GFLma_c (GFLcolor){0.75,0.00,0.75,0.25} /*         */
  /* intensite  50%                                          */
  #define GFLwb_c (GFLcolor){0.50,0.50,0.50,0.25} /*         */
  #define GFLrb_c (GFLcolor){0.50,0.12,0.12,0.25} /*         */
  #define GFLyb_c (GFLcolor){0.50,0.50,0.12,0.25} /*         */
  #define GFLgb_c (GFLcolor){0.12,0.50,0.12,0.25} /*         */
  #define GFLcb_c (GFLcolor){0.12,0.50,0.50,0.25} /*         */
  #define GFLbb_c (GFLcolor){0.12,0.12,0.50,0.25} /*         */
  #define GFLmb_c (GFLcolor){0.50,0.12,0.50,0.25} /*         */
  /* intensite  25%                                          */
  #define GFLwc_c (GFLcolor){0.25,0.25,0.25,0.25} /*         */
  #define GFLrc_c (GFLcolor){0.25,0.12,0.12,0.25} /*         */
  #define GFLyc_c (GFLcolor){0.25,0.25,0.12,0.25} /*         */
  #define GFLgc_c (GFLcolor){0.12,0.25,0.12,0.25} /*         */
  #define GFLcc_c (GFLcolor){0.12,0.25,0.25,0.25} /*         */
  #define GFLbc_c (GFLcolor){0.12,0.12,0.25,0.25} /*         */
  #define GFLmc_c (GFLcolor){0.25,0.12,0.25,0.25} /*         */
  /* intensite  12.5%                                        */
  #define GFLwd_c (GFLcolor){0.12,0.12,0.12,0.25} /*         */
  #define GFLrd_c (GFLcolor){0.12,0.06,0.06,0.25} /*         */
  #define GFLyd_c (GFLcolor){0.12,0.12,0.06,0.25} /*         */
  #define GFLgd_c (GFLcolor){0.06,0.12,0.06,0.25} /*         */
  #define GFLcd_c (GFLcolor){0.06,0.12,0.12,0.25} /*         */
  #define GFLbd_c (GFLcolor){0.06,0.06,0.12,0.25} /*         */
  #define GFLmd_c (GFLcolor){0.12,0.06,0.12,0.25} /*         */

#endif

#ifdef __cplusplus
  }
#endif
/*!===========================================================================!*/
