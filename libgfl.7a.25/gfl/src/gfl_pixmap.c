/*!===============================================================
  E.Incerti - Universite Gustave Eiffel - eric.incerti@univ-eiffel.fr
       - Librairie GFL - Fonctions de base d'acces public -
                    version 6e - aout 2022
  ================================================================!*/
#ifdef __cplusplus
  extern "C" {
#else
  #define _GNU_SOURCE
#endif

#include <gfl.h> /* le type GFLpixmap est dans <gfl_pixmap.h> */

/* 2023/09/10 - pour palier quelques 'bugs' de la commande <anytopnm> (format TIFF)  *
 * - 1°) utilise la commande systeme <file> pour identifier le type MIME de l'image  *
 * - 2°) cherche le type dans une liste predefinie de formats d'image classique      *
 * - 3°) utilise le convertisseur <***topnm> adapté, plutôt que <anytopnm>           *
 */
extern bool gfl_AnyToPixmap(GFLpixmap** img, char* filename)
{
  /* test préalable : est-ce que le fichier demandé existe ? */
  if (!fopen(filename,"r")) gfl_fopenfail_(filename,"r");
  /* ------------------------------------------------------- */
  static char FILE_FORMAT[8][32]={"bmp","gif","jpeg","png","tiff","x-portable_bitmap","x-portable-greymap","x-portable-pixmap"};
  static char FILE_CODEC[6][16] ={"bmptopnm","giftopnm","jpegtopnm","pngtopnm","tifftopnm",""};
  char command[1024]="";
  char buffer[2048]="",*bptr1=NULL,*bptr2=NULL;
  FILE *filetype=NULL;

  /* appel à la commande <file> : identification du type */
  sprintf(command,"file %s -ib 1> /tmp/_gflfiletype_",filename);
  if ( system(command)<0 ) gfl_callfail_(command);
  if (!(filetype=fopen("/tmp/_gflfiletype_","r")))  gfl_fopenfail_("/tmp/_gflfiletype_","r");
  if (!fgets(buffer,2047,filetype)) { gfl_error_(" erreur lecture /tmp/_gflfiletype_\n");     return false; }
  if (!(bptr1=strchr(buffer,'/')))  { gfl_error_(" erreur lecture buffer (1) <%s>\n",buffer); return false; }
  bptr1++;
  if (!(bptr2=strchr(bptr1,';')))   { gfl_error_(" erreur lecture buffer (2) <%s>\n",buffer); return false; }                
  *bptr2=0;
  /* identification du format d'image */
  int i;
  for (i=0;i<8;i++)
    if (0==strcmp(bptr1,FILE_FORMAT[i])) break;

  /* formats BMP/GIF/JPEG/PNG/TIFF => conversion en PNM (dans /tmp) */
  if (i<5)
  {
    sprintf(command,"%s %s > /tmp/_gfltmppnm0_",FILE_CODEC[i],filename);
    if ( system(command)<0 ) gfl_callfail_(command);
    return gfl_PnmLoad(img,"/tmp/_gfltmppnm0_");
  }
  // formats PBM/PGM/PPM
  if (i<8) return gfl_PnmLoad(img,filename);

  gfl_error_("Format <%s> non pris en charge\n",bptr1);
  return false;
}

/* Lecture image -- doit être au format PNM [cf. https://fr.wikipedia.org/wiki/Portable_pixmap] */
static size_t gfl_PnmPullRAW(GFLpixmap *img, FILE* pnm_file)
{
  uchar *umap=img->umap, *uend=img->uend;
  uchar *p,*q;
  int    i;
  size_t n=img->layer*img->height*img->width;

  switch (img->mode) /* bpm/pgm/ppm ASCII/RAW */
  { /* format ASCII */
    case '1' : /* PBM */
    case '2' : /* PGM */
    case '3' : /* PPM */ for (p=umap; !feof(pnm_file) && p<uend ;++p) n+=fscanf(pnm_file,"%hhu",p); break;
    /* format RAW */
    case '4' : /* PBM */ n/=8;
    case '5' : /* PGM */
    case '6' : /* PPM */ n=fread(umap,sizeof(uchar),n,pnm_file); break;
  }

  if (img->mode=='4') /* cas BINAIRE  : 1 bit -> 8 bits */
  {
    fprintf(stderr,"\e[0;33mFormat PBM : conversion 1 bit -> 8 bits (%d octets lus)->%d pixels\e[0m\n",(int)n,(int)(8*n));
    p=(uchar*)img->umap+n-1;
    q=(uchar*)img->uend-1;
    do
    {
      for (i=0;i<8;i++) {  q--; *q=(*p&0x1)?0xFF:0x00; *p>>=1;}
      p--;
    }
    while (p!=img->umap);
    n*=8;
    return n;
  }
  return n;
}

/* Ecriture image au format PNM */
static size_t gfl_PnmPushRAW(GFLpixmap *img, FILE* pnm_file)
{
  uchar *umap=img->umap, *uend=img->uend;
  uchar *p,*q,a;
  size_t n=img->layer*img->height*img->width,i=0;

  if (img->mode=='4')
  {
    fprintf(stderr,"\e[0;33mFormat PBM : conversion 8 bits -> 1 bit\e[0m\n");
    p=umap;
    i=0;
    do { fprintf(stderr,"\e[0;33m%hhu%s",*p++,(++i)%img->width?"":"\e[0m\n");} while (p!=uend);
    p=umap;
    q=umap;
    n=0;
    do
    {
      a = 0;
      a|=(*q++&1)<<7; a|=(*q++&1)<<6; a|=(*q++&1)<<5; a|=(*q++&1)<<4;
      a|=(*q++&1)<<3; a|=(*q++&1)<<2; a|=(*q++&1)<<1; a|=(*q++&1)<<0;
      p--;
     *p++=a;
      n++;
    }
    while (q!=uend);
    uend=p;
  }

  switch (img->mode)
  {
    case '1' :
    case '2' :
    case '3' : for (p=umap;p<uend;++p) n+=fprintf(pnm_file,"%hhu ",*p); break;
    case '4' :
    case '5' :
    case '6' : n=fwrite(umap,sizeof(uchar),n,pnm_file); break;
  }
  return n;
}


#define _MAX_IMG_NAME_ 32
#define _MAX_NAME_LEN_ 255
typedef struct
{
  GFLpixmap* img;
  char       name[_MAX_NAME_LEN_+1];
} GFLpixmapnames;

static int nbimages=0;
static GFLpixmapnames img_names[_MAX_IMG_NAME_];

/* Libere *pix et le met a NULL */
extern bool gfl_PixmapFree(GFLpixmap** img)
{
  if (*img==NULL) return false;

  if (nbimages)
  {
    int i=0;
    while (i<nbimages && *img!=img_names[i].img) i++;
    if (i<nbimages)
    {
      if (*img_names[i].name==0)  return true;
     *img_names[i].name=0;
    }
  }
  if ((*img)->umap) free((*img)->umap);
  if ((*img)->fmap) free((*img)->fmap);
  free(*img);
  *img=NULL;
  return true;
}

/* Si *pix=NULL, on alloue, sinon c'est qu'il existe deja, donc on ne fait rien   *
   * Le principe est donc de ne declarer que des GFLpixmap* initialises a NULL      *
   * paramètres :                                                                   *
   *  - *pix : le pixmap a creer                                                    *
   *  - width/height : dimensions en pixels                                         *
   *  - layer : nombre de plans (1 ou 3)                                            *
   *  - depth : niveau max (1 ou 255)                                               */
extern bool gfl_PixmapAlloc(GFLpixmap **img, int width, int height, int layer, int depth)
{
  /* recyclage si possible */
  if ((*img)!=NULL                               &&
      (*img)->umap!=NULL                          &&
      (*img)->height*(*img)->width>=height*width &&
      (*img)->layer>=layer                       &&
      (*img)->depth>=depth                        )
  {
    (*img)->height = height;
    (*img)->width  = width;
    (*img)->layer  = layer;
    (*img)->depth  = depth;
    (*img)->mode   = (layer==3 ? 6 : 3 );
    (*img)->uend=(uchar*)(*img)->umap + (layer*height*width);
    return true;
  }

  /* si pas de recyclage : on libère so nécessaire et on réalloue */
  if (*img!=NULL) gfl_PixmapFree(img);
  *img = (GFLpixmap*)calloc(1,sizeof(GFLpixmap));
  if (*img==NULL) gfl_allocfail_(sizeof(GFLpixmap));

  (*img)->height = height;
  (*img)->width  = width;
  (*img)->layer  = layer;
  (*img)->depth  = depth;
  (*img)->mode   = (layer==3 ? 6 : 3 );
  if (!((*img)->umap = (uchar*)calloc(layer*width*height,sizeof(uchar))))
  {
    free((void**)img);
    gfl_allocfail_((ulong)(layer*width*height));
  }
  (*img)->uend=(uchar*)(*img)->umap+(layer*height*width);
  return true;
}

/* Affiche (stderr) quelques infos sur le pixmap */
extern void gfl_PixmapInfo(GFLpixmap *img)
{
  fprintf(stderr,"mode P%c\n",img->mode);
  switch (img->mode)
  {
    case '1' :   fprintf(stderr,"\t<PBM> BitMap  (1 layer) |   1 bit /pix | ASCII data\n"); break;
    case '2' :   fprintf(stderr,"\t<PGM> GrayMap (1 layer) |   8 bits/pix | ASCII data\n"); break;
    case '3' :   fprintf(stderr,"\t<PPM> PixMap  (3 layers) | 3*8 bits/pix | ASCII data\n"); break;
    case '4' :   fprintf(stderr,"\t<PBM> BitMap  (1 layer) |   1 bit /pix | RAW data\n"); break;
    case '5' :   fprintf(stderr,"\t<PGM> GrayMap (1 layer) |   8 bits/pix | RAW data\n"); break;
    case '6' :   fprintf(stderr,"\t<PPM> PixMap  (3 layers) | 3*8 bits/pix | RAW data\n"); break;
    case '7' :   fprintf(stderr,"\t<PAM> AnyMap  (4 layers) | 4*8 bits/pix | RAW data\n"); break;
  }
  fprintf(stderr,"\tsize %d cols | %d lines\n",img->width,img->height);
  fprintf(stderr,"\tdyn. %d\n",img->depth);
  fprintf(stderr,"\tfirst 8x8 block :\n\t");
  for (int l=0; l<8; l++)
  {
    for (int c=0; c<8; c++)
    {
      for (int k=0; k<img->layer; k++)
        fprintf(stderr,"%3d ",img->umap[(l*img->width+c)*img->layer+k]);
      fprintf(stderr,"| ");
    }

    fprintf(stderr,"\n\t");
  }
  fprintf(stderr,"\n--------------------\n");
}

/*[jan. 2025] le pixmap en float [0.,1.] */
extern bool gfl_Pixmap255uTo1f(GFLpixmap *img)
{
  if (!(img && img->umap)) 
  {
    gfl_error_("image must be loaded before conversion to float");
    return false;
  }
  size_t length = (img->layer*img->width*img->height);
  if (!(img->fmap = (float*)calloc(length,sizeof(float))))
  {
    free((void**)img);
    gfl_allocfail_((size_t)length);
  }
  img->fend=(float*)(img->fmap+length);
  uchar *u = img->umap;
  float *f = img->fmap;
  while (u<img->uend) { (*f) = (*u)*RGB_to_01; u++; f++; }
  return true;
}

extern bool gfl_Pixmap1fTo255u(GFLpixmap *img)
{
  if (!(img && img->fmap)) 
  {
    gfl_error_("image float map doesn' exist - use <gfl_PixmapToFloat> to load");
    return false;
  }
  uchar *u = img->umap;
  float *f = img->fmap;
  float  x;
  while (u<img->uend) { x = (*f)*255.5; *u = (uchar)CAST_UCHAR(x); u++; f++; }
  return true;
}

/* crée une copie (dst) de la source (src) */
extern bool gfl_PixmapCpy(GFLpixmap** dst, GFLpixmap* src)
{
  if (!gfl_PixmapAlloc(dst,src->width,src->height,src->layer,src->depth)) return false;
  memcpy((*dst)->umap,src->umap,src->layer*src->width*src->height*sizeof(uchar));
  return true;
}

/* extrait le plan <layer> de <src> (RGB) verts <dst> (GRAYSCALE) */
extern bool gfl_PixmapLayer(GFLpixmap** dst, GFLpixmap* src, int layer)
{
  if (!gfl_PixmapAlloc(dst,src->width,src->height,1,src->depth)) return false;

  uchar *s,*d;
  for ( (s=src->umap+layer, d=(*dst)->umap); s<src->uend; s+=src->layer,d++)
    *d = *s;
  return true;
}

/* extrait les 3 plans de <src> (RGB) vers <dst[0,1,2]> (GRAYSCALE) */
extern bool gfl_PixmapSplit_1x3(GFLpixmap** dst[3], GFLpixmap* src)
{
  if (!gfl_PixmapLayer(dst[0],src,0)) return false;
  if (!gfl_PixmapLayer(dst[1],src,1)) return false;
  if (!gfl_PixmapLayer(dst[2],src,2)) return false;
  return true;
}

/* extrait les 3 plans de <src> (RGB) vers <dst[0,1,2]> (GRAYSCALE) */
extern bool gfl_PixmapSplit_3x1(GFLpixmap** A, GFLpixmap** B, GFLpixmap** C, GFLpixmap* src)
{  
  if (!gfl_PixmapLayer(A,src,0)) return false;
  if (!gfl_PixmapLayer(B,src,1)) return false;
  if (!gfl_PixmapLayer(C,src,2)) return false;
  return true;
}

/* extrait le plan <layer> de <src> (RGB) verts <dst> (GRAYSCALE) */
extern bool gfl_PixmapMerge_3x1(GFLpixmap** dst, GFLpixmap* A, GFLpixmap* B, GFLpixmap* C)
{
  if ( A->width !=B->width  || A->width !=C->width ||
       A->height!=B->height || A->height!=C->height ) gfl_error_("sizes not compatible A:(%d,%d) B(%d,%d) C(%d,%d)\n"
                                                                  ,A->width,A->height,B->width,B->height,C->width,C->height); 
  if ( A->layer>1 || B->layer>1 || B->layer>1 ) gfl_error_("all images must be single layer A:%d, B:%d, C:%d\n"
                                                                  ,A->layer,B->layer,C->layer); 
  
  if (!gfl_PixmapAlloc(dst,A->width,A->height,3,A->depth)) return false;

  uchar *a=A->umap,*b=B->umap,*c=C->umap;
  uchar *d=(*dst)->umap ;  
  while (d<(*dst)->uend)
  {
    *d = *a; a++; d++;
    *d = *b; b++; d++;
    *d = *c; c++; d++;
  }
  return true;
}


/* Charge (alloue, si necessaire) *pix a partir d'un fichier image au format PNM                      *
 * ATTENTION : quel que soit le type du fichier original (pbm,pgm,ppm), on creera un pixmap 24bits    *
 * celà permet, dans le cas d'image binaires ou en niveaux de gris, de "colorer" les pixels à volonté *
 * ---------------------------------------------------------------------------------------------------*
 * ATTENTION : pour des raisons de compatibilité avec OpenGl (passage par une 'texture') les tailles  *
 * du pixmap doivent être des multiples de 8.                                                         *
 * Si nécessaire, l'image d'entrée est retaillée [>system("pnmcut...")] : bords droit/inférieur       */
extern bool gfl_PnmLoad(GFLpixmap** img, char *filename)
{
  int     height,width,depth=1,layer;
  char    mode[3],c,comment[256];
  FILE*   pnm_file;
  size_t  leng;

  if (!(pnm_file=fopen(filename,"r"))) gfl_fopenfail_(filename,"r");

  /*------- le mode, les plans ---------*/
  if (0==fscanf(pnm_file,"%s\n",mode)) { gfl_error_("error reading magic number <%s>\n",mode); return false; }
  if (mode[0]!='P') return false;
  switch (mode[1])
  {
    case '1' : case '2' :
    case '4' : case '5' : layer=1; break; // PBM & PGM
    case '3' : case '6' : layer=3; break; // PPM
    case '7' :            layer=4; break; // PAM
    default  : return false;
  }

  /*------- les commentaires ----------*/
  while ((c=getc(pnm_file))=='#')
  { if (NULL==fgets(comment,256,pnm_file)) { gfl_error_("error reading comment line <%s>\n",comment);  return false; }
    fprintf(stderr,"\t# %s\n",comment);
  }
  ungetc(c,pnm_file);
  /*--------- les tailles -------------*/
  if (0==fscanf(pnm_file,"\n%d %d",&width,&height)) { gfl_error_("error reading image sizes <%d,%d>\n",width,height);  return false; }
  fprintf(stderr,"\tmode [%s] size [%dx%d]\n",mode,width,height);
  /*-------- la profondeur ------------*/
  if (mode[1]!='1' && mode[1]!='4') 
  { 
    if (0==fscanf(pnm_file,"%d\n",&depth)) { gfl_error_("error reading pixel depth <%d>\n",depth);  return false; }
    depth++; 
  }

  /* 2023/09/09 : retaillage pour alignement des tailles sur multiple de 8                 *
   * => Cette opération est nécessaire pour compatibilité avec gestion des textures OpenGl */
  if (width%8!=0 || height%8!=0)
  {
    fclose(pnm_file);
    width  -= width %8;
    height -= height%8;
    
    gfl_warning_("image must be resized to fit [WxH] modulo 8 => [%dx%d]\n",width,height);

    char command[512]="";
    sprintf(command,"pnmcut -width=%d -height=%d %s > /tmp/_gfltmppnm1_",width,height,filename);
    if (system(command)<0) gfl_callfail_(command);

    return gfl_PnmLoad(img,"/tmp/_gfltmppnm1_");
  }

  if (!gfl_PixmapAlloc(img,width,height,layer,depth)) { fclose(pnm_file); return false; }

  (*img)->mode=mode[1];

  leng=gfl_PnmPullRAW(*img,pnm_file);
  if (leng<(size_t)(layer*height*width))
    gfl_warning_(" truncated data -- %d octets read out of %d expected\n",(int)leng,(int)(layer*height*width));

  fclose(pnm_file);
  return true;
}

/* <DEPRECATED> => utiliser plutôt <gfl_AnyToPixmap>              *
 * Idem, mais a partir d'un format d'image quelconque             *
 * L'image est transformee en fichier PNM (pnmlib), puis chargee  *
 * le fichier PNM intermediaire est detruit immediatement         */
extern bool gfl_ImageLoad(GFLpixmap** img, char *filename, bool RELOAD)
{
  static char  command[256];
  bool   ok=true;

  if (!RELOAD)
  {
    int i=0;
    while (i<nbimages && strcmp(filename,img_names[i].name)) i++;
    if (i<nbimages)  { *img=img_names[i].img; return true; }
  }
  // on teste d'abord si c'est du format PNM
  if (!gfl_PnmLoad(img,filename))
  { // si c'est pas le cas, on converti en PNM (temporaire)
    sprintf(command,"anytopnm %s > /tmp/_gfltmppnm0_",filename);
    if ((system(command))) gfl_callfail_(command);

    // on charge le PNM temporaire
    ok=gfl_PnmLoad(img,"/tmp/_gfltmppnm0_");
  }
  char *sep=strrchr(filename,'/');
  sep = (sep ? sep+1 : filename);
  fprintf(stderr,"\e[1m<%s>\e[0m : Chargement image \e[0;35m%s\e[0;33m (recyclage:[\e[0;45m%s\e[0;33m])\e[0m\n"
                ,__func__,sep,RELOAD?"non":"oui");

  if (!RELOAD)
  {
    if (nbimages==_MAX_IMG_NAME_) gfl_warning_(" impossible de stocker le nom de l'image <%s> - tableau plein\e[0m\n",filename);
    strncpy(img_names[nbimages].name,filename,_MAX_NAME_LEN_);
    img_names[nbimages].img=*img;
    nbimages++;
  }
  return ok;
}

#ifdef _GFL2D_
  /* [2024/12/19] affichage grille et valeurs des pixels si facteur de zoom suffisant */
  static void gfl_PixmapGridAndValues(GFLpixmap *pixmap, bool GRID, bool VALUES)
  {
    int zoom=gfl_GetZoom();
    if (!GRID || zoom<8) return;
  
    int h = pixmap->height/2;
    int w = pixmap->width/2;
  
    /* tracé du cadre des pixel */
    for (int y=-h; y<+h; y++) gfl_Line(-w, y,+w, y,GFLwa_a,1);
    for (int x=-w; x<+w; x++) gfl_Line( x,-h, x,+h,GFLwa_a,1);
    
    if (!VALUES || zoom<30) return;
  
    /* affichage des valeurs de pixels */  
    if      (zoom>75) gfl_SetFontAttributes('L','n','c');
    else if (zoom>60) gfl_SetFontAttributes('l','n','c');
    else if (zoom>45) gfl_SetFontAttributes('m','n','c');
    else              gfl_SetFontAttributes('s','n','c');
  
    uchar *p=pixmap->umap;
    GFLcolor col;
    for (int y=h-1; y>=-h;y--)
    {
      if ( y+1.<gfl_GetYMin() || y-1.>gfl_GetYMax() ) { p += pixmap->layer*pixmap->width; continue; }
      for (int x=-w; x<+w; x++)
      {
        if (x+1.<gfl_GetXMin() || x-1.>gfl_GetXMax() ) { p += pixmap->layer; continue; }
        switch(pixmap->layer)
        {
          case 1 :  gfl_Print((x+0.5),(y+0.50),(*p>127?GFLk:GFLw),"%d",*p); break;
          case 3 :  col = ((*p+*(p+1)+*(p+2))>300 ? GFLk:GFLw);
                    gfl_Print((x+0.5),(y+0.75),col,"R:%3d",*p+0);
                    gfl_Print((x+0.5),(y+0.50),col,"G:%3d",*p+1);
                    gfl_Print((x+0.5),(y+0.25),col,"B:%3d",*p+2);
                    break;
        }
        p += pixmap->layer;
      }
    }
  }
  
  /* [2024/12/19] pixmap -> Texture OpenGl */
  static void gfl_PixmapToGlTex(GFLpixmap *pixmap, GLint tex_id)
  {
    glBindTexture(GL_TEXTURE_2D,tex_id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    switch (pixmap->layer)
    {
      case 1  : glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, pixmap->width, pixmap->height, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE,pixmap->umap); break;
      case 3  : glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8     , pixmap->width, pixmap->height, 0, GL_RGB      , GL_UNSIGNED_BYTE,pixmap->umap); break;
      default : gfl_error_("invalid number of layers [%d]\n",pixmap->layer); return;
    }
    glEnable(GL_TEXTURE_2D);
      glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
      glColor4f(1.,1.,1.,0.);
      glBegin(GL_QUADS);
        glTexCoord2d(0,1); glVertex2d(-0.5*pixmap->width,-0.5*pixmap->height);
        glTexCoord2d(1,1); glVertex2d(+0.5*pixmap->width,-0.5*pixmap->height);
        glTexCoord2d(1,0); glVertex2d(+0.5*pixmap->width,+0.5*pixmap->height);
        glTexCoord2d(0,0); glVertex2d(-0.5*pixmap->width,+0.5*pixmap->height);
      glEnd();
      glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
    glDisable(GL_TEXTURE_2D);
  }
  
  /* Chargement du pixamp dans une texture               *
   * Par défaut, la texture est un rectangle à la taille *
   * de l'image qui sera redimensionné à l'affichage     */
  extern void gfl_PixmapPreload(GFLpixmap *pixmap)
  {
    pixmap->id = glGenLists(1);
    glNewList(pixmap->id, GL_COMPILE);
      uint tex_id;
      glGenTextures(1,&tex_id);
      gfl_PixmapToGlTex(pixmap,tex_id);
      glDeleteTextures(1, &tex_id);
    glEndList();
  }
  
  /* affichage d'une image non bufferisé                           *
   * => moins performant mais adapté à des traitements dynamiques. *
   * le flag <PIXGRID> permet d'afficher les contours des pixels   *
   * lorsque le facteur de zoom est suffisant (8x)                 */
  extern void gfl_PixmapShow(GFLpixmap *pixmap, bool PIX_GRID)
  {
    uint tex_id;
    double dx = gfl_GetZoom()*gfl_GetXPixSize(); // facteur d'échelle en x
    double dy = gfl_GetZoom()*gfl_GetYPixSize(); // facteur d'échelle en y
    glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
    glPushMatrix();
      glScalef(dx,dy,1.);
      glGenTextures(1,&tex_id);
      gfl_PixmapToGlTex(pixmap,tex_id);
      gfl_PixmapGridAndValues(pixmap,PIX_GRID,PIX_GRID);
    glPopMatrix();
  }

  /* simple appel à l'objet Gl précompilé (bcp + économique)              *
   * on lui applique les conditions de zoom_value via les transfos OpenGl *
   * pour recaler la texture (rectangle réel) à la taille voulue          */
  extern void gfl_PixmapRecall(GFLpixmap *pixmap, bool PIX_GRID)
  {
    double dx = gfl_GetZoom()*gfl_GetXPixSize(); // facteur d'échelle en x
    double dy = gfl_GetZoom()*gfl_GetYPixSize(); // facteur d'échelle en y
    glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
    glPushMatrix();
      glScalef(dx,dy,1.);
      glCallList(pixmap->id);
      gfl_PixmapGridAndValues(pixmap,PIX_GRID,PIX_GRID);    
    glPopMatrix();
    glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
  }

  /** PASSAGE COORDONNEES PIXEL ENTIERES (col|line) <=> COORDONNEES 'FENETRE' réelles (x|y) **/
  /* renvoie la coordonnée <y> (repère réel) de la ligne <line> de l'image */
  extern double   gfl_PixLineToY(GFLpixmap *pix, int line)
  {
    return (-0.5*pix->height+line)/(gfl_GetXMax()-gfl_GetXMin());
  }
  
  /* renvoie la coordonnée <x> (repère réel) de la colonne <col> de l'image */
  extern double   gfl_PixColToX(GFLpixmap *pix, int col)
  {
    return (-0.5*pix->width+col)/(gfl_GetXMax()-gfl_GetXMin());
  }
  
  /* renvoie les coordonnées <(y|x)> (repère réel) du pixel (line|col) *
   * de l'image sous forme de GFLpoint                                 */
  extern GFLpoint gfl_PixToPoint(GFLpixmap *pix, int line, int col)
  {
    return (GFLpoint){gfl_PixColToX(pix,col),gfl_PixLineToY(pix,line)};
  }
  
  /* renvoie les coordonnées (line|col) (repère image)   *
   * du point <pos> (coord. pixel)                       */
  extern void     gfl_PointToPix(GFLpoint *pos, GFLpixmap *pix, int *line, int *col)
  {
    *col  = (int)(pos->x<0.?floor(pos->x)  :ceil(pos->x)-1)+pix->width/2;
    *line =-(int)(pos->y<0.?floor(pos->y)+1:ceil(pos->y)  )+pix->height/2;
  }
#endif

/* écrit le contenu du pixmap <img> sur fichier <filename> au format PNM  *
 * mode    : \in {1|2|3|4|5|6} => magic number "P*"                       *
 * comment : texte de commentaires (metadata)                             */
extern bool gfl_PnmWrite(GFLpixmap* img, char *filename, char mode, char* comment)
{
  FILE  *pnm_file;
  time_t date=time(NULL);
  size_t leng;

  if (img==NULL) { gfl_error_("nothing to write on %s\e[0m\n",filename); return false; }
  if (!(pnm_file=fopen(filename,"w")))  gfl_fopenfail_(filename,"w");

  fprintf(pnm_file,"P%c\n",mode);
  fprintf(pnm_file,"#--------------------------------------\n");
  fprintf(pnm_file,"# %s - %s",filename,ctime(&date));
  if (strlen(comment)!=0) fprintf(pnm_file,"# %s\n",comment);
  fprintf(pnm_file,"#--------------------------------------\n");
  fprintf(pnm_file,"%d %d\n",img->width,img->height);

  if (mode!='1' && mode!='4') fprintf(pnm_file,"%d\n",img->depth-1);
  fflush(pnm_file);

  leng=gfl_PnmPushRAW(img,pnm_file);
  if (leng<(size_t)(img->layer*img->height*img->width)) gfl_warning_("truncated data -- %d octets written out of %d expected\n",
                                                                     (int)leng,(int)(img->layer*img->height*img->width));
  fflush(pnm_file);
  fclose(pnm_file);
  return true;
}


/* renvoie la composante "lay" (\in{0|1|2}) du pixel ("line",""col") *
 * exp : uchar  green = *gfl_GetPixel(pix,l,c,1) -- composante verte *
 *       uchar *rgb = gfl_GetPixel(pix,l,c,0)                        *
 *           => rgb[0]:rouge, rgb[1]:vert, rgb[2]:bleu               */
extern uchar*   gfl_GetPixel(GFLpixmap* pix, int line, int col, int lay)
{
  if (pix==NULL || pix->umap==NULL    ||
      lay <0    || lay >=pix->layer  ||
      col <0    || col >=pix->width  ||
      line<0    || line>=pix->height ) 
  { 
    gfl_error_("one of the three values (line|col|lay) is not correct \n"); 
    return NULL; 
  }
  return (pix->umap + pix->layer*(line*pix->width+col) + lay);
}

#ifdef __cplusplus
  }
#endif
