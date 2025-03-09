/**@file    gfl_pnm.h
 * @author  Universite Gustave Eiffel
 * @author  E.Incerti - eric.incerti@univ-eiffel.fr
 * @brief   Base public control functions
 * @version 6.e
 * @date    Aug.2022 (doc generation)
 */
/**@cond SHOW_SECURITY_DEFINE */

#ifdef __cplusplus
  extern "C" {
#endif

  #ifndef _GFL_PIXMAP_
    #define _GFL_PIXMAP_
  /**@endcond                   */
  
    #include <gfl_types.h>
  
    /* Le type "pixmap" se base sur le format "brut" PNM (Netpnm Project)
     * [cf. https://fr.wikipedia.org/wiki/Portable_pixmap
     *      https://netpbm.sourceforge.net/doc/pnm.html
     *      http://www.jchr.be/python/pnm-pam.htm         ]
     * ATTENTION : l'utilisation de ce format nécessite l'installation de la lib. <netpnm>
     * [apt-get install netpnm]
     * une image de format autre (bmp,gif,jpeg,png,tiff) est d'abord convertie
     * au format PNM adéquat (PBM : binaire 0/1 | PGM : niveau de gris (greymap) | PPM :couleurs RGB)
     * puis chargée dans un GFLpixmap
     * */
    typedef struct
    {
      int     id;           /* un identificateur (texture) */
      char    mode;         /* P[1-6]                      */
      int     width,height; /* largeur, hauteur            */
      int     layer,depth;  /* nbre de plans, profondeur   */
      uchar  *umap,*uend;     /* debut et fin du pixmap      */
      float  *fmap,*fend;   /* le pixmap en float [0.,1.]  */
    } GFLpixmap;
  
    /* avec _GFL_NO_RELOAD_ : lorsqu'une image est chargee, son nom est stocke dans un  *
     * tableau, avec le pointeur de pixmap correspondant. Si le meme nom est rappele    *
     * l'image n'est pas rechargee : on redirige vers le pixmap correspondant           *
     * C'est utile surtout en 3D pour les images de texture qui sont utilisees          *
     * plusieurs fois. --- RISQUE DE BUG A LA LIBERATION ????????                       */
    #define _GFL_RELOAD_        true
    #define _GFL_NO_RELOAD_     false
  
    /* Libere *pix et le met a NULL */
    bool gfl_PixmapFree(GFLpixmap** img);
  
    /* Si *pix=NULL, on alloue, sinon c'est qu'il existe deja, donc on ne fait rien   *
     * Le principe est donc de ne declarer que des GFLpixmap* initialises a NULL      *
     * paramètres :                                                                   *
     *  - *pix : le pixmap a creer                                                    *
     *  - width/height : dimensions en pixels                                         *
     *  - layer : nombre de plans (1 ou 3)                                            *
     *  - depth : niveau max (1 ou 255)                                               */
    bool gfl_PixmapAlloc(GFLpixmap** img, int width, int height, int layer, int depth);
  
    /* crée une copie (dst) de la source (src) */
    bool gfl_PixmapCpy(GFLpixmap** dst, GFLpixmap* src);
  
    /* extrait le plan <layer> de <src> (RGB) vers <dst> (GRAYSCALE) */
    bool gfl_PixmapLayer(GFLpixmap** dst, GFLpixmap* src, int layer);
  
    /* extrait les 3 plans de <src> (RGB) vers <dst[0,1,2]> (GRAYSCALE) */
    bool gfl_PixmapSplit_1x3(GFLpixmap** dst[3], GFLpixmap* src);
    bool gfl_PixmapSplit_3x1(GFLpixmap** A, GFLpixmap** B, GFLpixmap** C, GFLpixmap* src);
    /* recompose une image RGB à partir de 3 plans  <GRAYSCALE> */
    bool gfl_PixmapMerge_3x1(GFLpixmap** dst, GFLpixmap* A, GFLpixmap* B, GFLpixmap* C);
  
    /* Charge (alloue, si necessaire) *pix a partir d'un fichier image au format PNM                      *
     * ATTENTION : quel que soit le type du fichier original (pbm,pgm,ppm), on creera un pixmap 24bits    *
     * celà permet, dans le cas d'image binaires ou en niveaux de gris, de "colorer" les pixels à volonté *
     * ---------------------------------------------------------------------------------------------------*
     * ATTENTION : pour des raisons de compatibilité avec OpenGl (passage par une 'texture') les tailles  *
     * du pixmap doivent être des multiples de 8.                                                         *
     * Si nécessaire, l'image d'entrée est retaillée [>system("pnmcut...")] : bords droit/inférieur       */
    bool gfl_PnmLoad(GFLpixmap** img, char *filename);
  
    /* Charge une image dans un format quelconque (BMP/GIF/JPEG/PNG/PNM/TIFF) *
     * L'image est d'abord convertie au format PNM (dans /tmp) puis chargée   *
     * via la fonction <gfl_PnmLoad>                                          */
    bool gfl_AnyToPixmap(GFLpixmap** img, char* filename);
  
    /* <DEPRECATED> => utiliser plutôt <gfl_AnyToPixmap>              *
     * Idem, mais a partir d'un format d'image quelconque             *
     * L'image est transformee en fichier PNM (pnmlib), puis chargee  *
     * le fichier PNM intermediaire est detruit immediatement         */
    bool gfl_ImageLoad(GFLpixmap** img, char *filename, bool RELOAD);
  
    /* Affiche (stderr) quelques infos sur le pixmap */
    void gfl_PixmapInfo(GFLpixmap *img);

    /* renvoie la composante "lay" (\in{0|1|2}) du pixel ("line",""col") *
     * exp : uchar  green = *gfl_GetPixel(pix,l,c,1) -- composante verte *
     *       uchar *rgb = gfl_GetPixel(pix,l,c,0)                        *
     *           => rgb[0]:rouge, rgb[1]:vert, rgb[2]:bleu               */
    uchar*   gfl_GetPixel(GFLpixmap* pix, int line, int col, int lay);
  
    /* Ecrit les donnee pix dans un fichier PNM au format souhaite (pbm,pgm,ppm) */
    bool     gfl_PnmWrite(GFLpixmap*  img, char *filename, char mode, char* comment);
  
    /** AFFICHAGE - 2D UNIQUEMENT **/
    #ifdef _GFL2D_
      /* affichage d'une image non bufferisé                            *
       * => moins performant mais adapté à des traitements dynamiques.  *
       * le flag <PIXGRID> permet d'afficher les contours des pixels    *
       * lorsque le facteur de zoom est suffisant (8x)                  */
      void gfl_PixmapShow(GFLpixmap *img, bool PIXGRID);
    
      /* ces 2 fonctions permettent d'être un peu plus efficace          *
       * en pré-chargeant l'image (<gfl_PixmapPreload>) dans une texture *
       * puis en l'affichant (<gfl_PixmapRecall>) par simple rappel      *
       * => Le pixmpap NE peut PAS être modifié dynamiquement            */
      void gfl_PixmapPreload(GFLpixmap *img);
      void gfl_PixmapRecall(GFLpixmap *img, bool PIXGRID);

      /** PASSAGE COORDONNEES PIXEL ENTIERES (col|line) <=> COORDONNEES 'FENETRE' réelles (x|y) **/
      /* renvoie la coordonnée <y> (repère réel) de la ligne <line> de l'image */
      double   gfl_PixLineToY(GFLpixmap *img, int line);
    
      /* renvoie la coordonnée <x> (repère réel) de la colonne <col> de l'image */
      double   gfl_PixColToX (GFLpixmap *img, int col );
    
      /* renvoie les coordonnées <(y|x)> (repère réel) du pixel (line|col) *
       * de l'image sous forme de GFLpoint                                 */
      GFLpoint gfl_PixToPoint(GFLpixmap *pix, int line, int col);
    
      /* renvoie les coordonnées (line|col) (repère image)   *
       * du point <pos> (coord. pixel)                       */
      void     gfl_PointToPix(GFLpoint *pos, GFLpixmap *pix, int *line, int *col);    
    #endif         
    /********************************/
  
  #endif

#ifdef __cplusplus
  }
#endif
