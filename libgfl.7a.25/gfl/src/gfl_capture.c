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

#include <gfl.h>

#define _PPMTOMPEG_ 0
#define _MENCODER_  1

static int      _r_=0;
static bool     _PLUGGED_=false;
static int      _MAXIMAGES_=10000;

static GLint    pixwidth=0,pixheight=0;
static GLint    downleftx,downlefty,uprightx,uprighty;
static int      pixlength=0;
static GLubyte* pixmap=NULL;

static const char *pnm_name[] = {"pnm","ppm","pgm","pbm","PNM","PPM","PGM","PBM",NULL};
static const char *bmp_name[] = {"bmp","BMP",NULL};
static const char *jpg_name[] = {"jpeg","jpg","JPG","JPEG",NULL};
static const char *png_name[] = {"png","PNG",NULL};
static const char *gif_name[] = {"gif","GIF",NULL};
static const char *tif_name[] = {"tif","tiff","TIF","TIFF",NULL};
static const char *ras_name[] = {"rast","ras","raster","RAS",NULL};
static const char *eps_name[] = {"ps","eps","PS","EPS",NULL};

static char  capturename[256]="";
static char  tempdirname[512]="";
static char  tempsnapname[256]="";
static char  command[2048]="";
static FILE* framefile=NULL;
static FILE* parameterfile=NULL;
static int   framenum=1;
static int   first=1;
static int   pid=-1;
// paramètres vidéo
static int   fps    =25;
static int   bitrate=9600;
static int   TIMESTAMP=CLOCKS_PER_SEC/30; // petit écart pour compenser
static double clocktosec=1./CLOCKS_PER_SEC;


extern void  gfl_SetMaxImage(int m    ) { _MAXIMAGES_=m;      }
extern int   gfl_GetMaxImage(void     ) { return _MAXIMAGES_; }
extern void  gfl_SetFrameNum(int frame) { framenum=frame;  }
extern int   gfl_GetFrameNum(void     ) { return framenum; }
extern void  gfl_SetBitRate(int bit_rate) { bitrate=bit_rate; }
extern int   gfl_GetBitRate(void        ) { return bitrate;   }
extern void  gfl_SetFrameRate(int frame_per_sec) { fps=frame_per_sec; }
extern int   gfl_GetFrameRate(void             ) { return fps;        }
extern void  gfl_SetPid(int force_pid) { pid=force_pid; }
extern int   gfl_GetPid(void         ) { return pid;    }

static bool match_string(const char* str, const char* choices[])
{
  const char **p_choice=choices;
  while (*p_choice!=NULL)
    if (strcmp(*p_choice++,str)==0)
      return true;
  return false;
}

extern void gfl_UnplugCapture(void)
{
  if (!_PLUGGED_) return;
  fflush(framefile);
  fclose(framefile);
  snprintf(command,1023,"rm -f %s",tempsnapname);
  _r_=system(command);
  snprintf(command,1023,"rm -rf %s/",tempdirname);
  _r_=system(command);
  if (pixmap) free(pixmap);
  _PLUGGED_=false;
}

extern bool gfl_PlugCapture(const char *basename, int dlx, int dly, int urx, int ury)
{
  if (_PLUGGED_) return true;
  _PLUGGED_=true;
  char num[64]="";
  time_t date=time(NULL);
  struct tm d;
  d = *localtime(&date);
  sprintf(num,"-%02d%02d%02d-%02d%02d",d.tm_year-100,d.tm_mon+1,d.tm_mday,d.tm_hour,d.tm_min);
  if (!strlen(basename)) sprintf(capturename,"gx_capture%s" ,num);
  else                   sprintf(capturename,"%s%s",basename,num);
  sprintf(tempsnapname,"/tmp/tmp.%05d" ,getpid());
  sprintf(tempdirname ,"%s" ,capturename);
  downleftx  =(GLint)dlx;
  downlefty  =(GLint)dly;
  uprightx   =(GLint)urx;
  uprighty   =(GLint)ury;
  pixwidth   =(GLint)((uprightx-downleftx)/16)*16;
  pixheight  =(GLint)((uprighty-downlefty)/16)*16;
  pixlength  =(int  )(3*pixwidth*pixheight);
  if (!(pixmap=(GLubyte*)calloc(pixlength,sizeof(GLubyte)))) gfl_allocfail_(sizeof(GLubyte));
  if (!(framefile=fopen(tempsnapname,"w")))                  gfl_fopenfail_(tempsnapname,"w");
  return true;
}

extern bool gfl_Snapshot(const char *format, const char *basename, int w, int h)
{
  static uint view=0;
  static char snapname[512];

  fprintf(stderr,"%s\t,%s\t,(%d,%d)\n",format,basename,w,h);

  pixwidth   =(GLint)(8*(int)(w/8));
  pixheight  =(GLint)(8*(int)(h/8));

  pixlength  =(int)(3*pixwidth*pixheight);
  if (!(pixmap=(GLubyte*)realloc(pixmap,pixlength*sizeof(GLubyte)))) gfl_allocfail_(sizeof(GLubyte));

  glReadPixels(0,0,pixwidth,pixheight,GL_RGB,GL_UNSIGNED_BYTE,pixmap);

  char num[64]="";
  time_t date=time(NULL);
  struct tm d;
  d = *localtime(&date);
  sprintf(num,"-%02d%02d%02d-%02d%02d",d.tm_year-100,d.tm_mon+1,d.tm_mday,d.tm_hour,d.tm_min);
  if (!strlen(basename)) sprintf(capturename,"gx_capture%s" ,num);
  else                   sprintf(capturename,"%s%s",basename,num);

  sprintf(tempsnapname,"/tmp/tmp.%05d" ,getpid());

  if (!(framefile=fopen(tempsnapname,"w")))  gfl_fopenfail_(tempsnapname,"w");

  fprintf(framefile,"P6\n%d %d 255\n",pixwidth,pixheight);
  _r_=fwrite(pixmap,sizeof(GLubyte),pixlength,framefile);

  fflush(framefile);
  fclose(framefile);

  if (strlen(format)==0) format=*pnm_name;
  sprintf(snapname,"%s.%02d.%s",capturename,view,format);
  fprintf(stderr,"snapshot : %s\n",snapname);
  view++;
  if (view==100) gfl_warning_(" nombre maximal de snapshot atteint (00->99)\e[0m\n");

       if (match_string(format,pnm_name)) snprintf(command,1023,"pnmflip -tb %s > %s",tempsnapname,snapname);
  else if (match_string(format,bmp_name)) snprintf(command,1023,"cat %s | pnmflip -tb | ppmtobmp > %s",tempsnapname,snapname);
  else if (match_string(format,jpg_name)) snprintf(command,1023,"cat %s | pnmflip -tb | pnmtojpeg -quality=100 -dct=float> %s",tempsnapname,snapname);
  else if (match_string(format,gif_name)) snprintf(command,1023,"cat %s | pnmflip -tb | pnmquant -fs 256 | ppmtogif > %s",tempsnapname,snapname);
  else if (match_string(format,tif_name)) snprintf(command,1023,"cat %s | pnmflip -tb | pnmtotiff -packbits > %s",tempsnapname,snapname);
  else if (match_string(format,ras_name)) snprintf(command,1023,"cat %s | pnmflip -tb | pnmtorast > %s",tempsnapname,snapname);
  else if (match_string(format,png_name)) snprintf(command,1023,"cat %s | pnmflip -tb | pnmtopng -compression 9 -transparent black > %s",tempsnapname,snapname);
  else if (match_string(format,eps_name)) snprintf(command,1023,"cat %s | pnmflip -tb | pnmtops -noturn -rle -dpi 600 > %s",tempsnapname,snapname);
  else { gfl_error_(" format <%s> non pris en compte\e[0m\n",format); return false; }
  _r_=system(command);
  return true;
}

static int _mkdir_win_linux_(const char *pathname)
{
  #ifdef WIN
    return mkdir(tempdirname);
  #else
    return mkdir(tempdirname,S_IRWXU);
  #endif
}

extern bool gfl_FilmFrame(void)
{
  if (framenum>=_MAXIMAGES_) { gfl_error_(" nombre maximal d'images (%d) atteint. Arret automatique\e[0m\n",_MAXIMAGES_); return false; }
  else if (framenum==1)
  {
    if (-1==_mkdir_win_linux_(tempdirname)) { perror("mkdir"); return false; }
    fprintf(stderr,"\e[0;36mvideo: frame #\n");
  }
  fprintf(stderr,"\e[F\e[1;36m\t\t\b\b%04d\e[0;36m\e[0m                    \r",framenum);

  glReadPixels(downleftx,downlefty,pixwidth,pixheight,GL_RGB,GL_UNSIGNED_BYTE,pixmap);
  rewind(framefile);
  fprintf(framefile,"P6\n%d %d 255\n",pixwidth,pixheight);
  _r_=fwrite(pixmap,sizeof(GLubyte),pixlength,framefile);
  fflush(framefile);
  /* flip vertical du ./tmpppm et encodage jpg    */
  /*
  snprintf(command,1023,"cat %s | pnmflip -tb | pnmtojpeg -quality=90 -dct=int> %s/frame.%04d.jpg"
                  ,tempsnapname,tempdirname,framenum);
  */
  snprintf(command,1023,"cat %s | pnmflip -tb | pnmtopng -compression 5 > %s/frame.%04d.png" ,tempsnapname,tempdirname,framenum);
  _r_=system(command);
  framenum++;
  return true;
}

extern bool gfl_RealTimeFrame(void)
{
  static clock_t start=0,current=0;
  if (start==0) start = clock(); //démarrage chrono

  current = clock(); //démarrage chrono
  if ((current-start)<TIMESTAMP) return true;

  start=current;
  if (framenum>=_MAXIMAGES_) { gfl_error_(" nombre maximal d'images (%d) atteint. Arret automatique\e[0m\n",_MAXIMAGES_); return false; }
  else if (framenum==1)
  {
    if (-1==_mkdir_win_linux_(tempdirname)) { perror("mkdir"); return false; }
    fprintf(stderr,"\e[0;36mvideo: frame #\n");
  }
  fprintf(stderr,"\e[1m\t\t[%04d] %.2lf sec\e[0m \r",framenum,current*clocktosec);

  glReadPixels(downleftx,downlefty,pixwidth,pixheight,GL_RGB,GL_UNSIGNED_BYTE,pixmap);
  rewind(framefile);
  fprintf(framefile,"P6\n%d %d 255\n",pixwidth,pixheight);
  _r_=fwrite(pixmap,sizeof(GLubyte),pixlength,framefile);
  fflush(framefile);
  /* flip vertical du ./tmpppm et encodage jpg    */
  /*
  snprintf(command,1023,"cat %s | pnmflip -tb | pnmtojpeg -quality=90 -dct=int> %s/frame.%04d.jpg"
                  ,tempsnapname,tempdirname,framenum);
  */
  snprintf(command,1023,"cat %s | pnmflip -tb | pnmtopng -quiet -compression 0 > %s/frame.%04d.png ",tempsnapname,tempdirname,framenum);
   _r_=system(command);
  framenum++;
  return true;
}

/* version MPEG 1 - obsolete */
extern bool gfl_MakeMpeg(void)
{
  if (!(parameterfile=fopen("./.ppmtompeg_parameterfile","w")))
  { gfl_error_(" erreur de creation du fichier de parametrage\e[0m\n"); return false; }
  fprintf(parameterfile,"PATTERN IBBPBBIBBPBBIBBPBBIBBPBB\n");
  fprintf(parameterfile,"IQSCALE 2\n");
  fprintf(parameterfile,"PQSCALE 4\n");
  fprintf(parameterfile,"BQSCALE 8\n");
  fprintf(parameterfile,"SLICES_PER_FRAME 1\n");
  fprintf(parameterfile,"PIXEL HALF\n");
  fprintf(parameterfile,"OUTPUT %s.mpeg\n",capturename);
  fprintf(parameterfile,"INPUT_DIR ./%s\n",tempdirname);
  fprintf(parameterfile,"INPUT\n");
  fprintf(parameterfile,"frame.*.jpg [%04d-%04d]\n",first,framenum-1);
  fprintf(parameterfile,"END_INPUT\n");
  fprintf(parameterfile,"BASE_FILE_FORMAT JPG\n");
  fprintf(parameterfile,"INPUT_CONVERT*\n");
  fprintf(parameterfile,"GOP_SIZE %d\n",fps);
  fprintf(parameterfile,"RANGE 2\n");
  fprintf(parameterfile,"PSEARCH_ALG EXHAUSTIVE\n");
  fprintf(parameterfile,"BSEARCH_ALG CROSS2\n");
  fprintf(parameterfile,"REFERENCE_FRAME DECODED\n");
  fprintf(parameterfile,"framerate %d\n",fps);
  fflush(parameterfile);
  fclose(parameterfile);
  _r_=system("ppmtompeg -float_dct -realquiet -no_frame_summary .ppmtompeg_parameterfile");
  _r_=system("rm -rf ./.ppmtompeg_parameterfile");
  return true;
}


extern bool gfl_MakeAvi(void)
{
  fprintf(stderr,"\e[1mCREATION VIDEO [%s.avi]\e[0m\n",capturename);
  snprintf(command,2047,"\
mencoder -quiet mf://%s/*.png -mf type=png:w=%d:h=%d:fps=%d\
                              -oac copy -o %s.avi -ovc x264\
                              -x264encopts \
                               autoaspect=1:subq=6:partitions=all:\
                               me=umh:frameref=5:bframes=3:b_pyramid:weight_b:\
                               threads=12"
                              ,tempdirname,pixwidth,pixheight,fps,capturename);
  _r_=system(command);
  return true;
}

extern bool gfl_MakeMpeg4(void)
{
  fprintf(stderr,"\e[1mCREATION VIDEO [%s.mp4]\e[0m\n",capturename);
  snprintf(command,2047,"\
mencoder -msglevel all=0 mf://%s/*.png -mf fps=%d:type=png:w=%d:h=%d -o %s.mp4 -ovc lavc -lavcopts \
vcodec=mpeg4:autoaspect=1:vbitrate=%d:mbd=2:o=mpv_flags=+mv0:trell:v4mv:\
o=mpv_flags=+cbp_rd:last_pred=3:predia=2:dia=2:vmax_b_frames=2:vb_strategy=1:\
precmp=2:cmp=2:subcmp=2:preme=2:o=quantizer_noise_shaping=2:threads=4  >/dev/null"
                       ,tempdirname,fps,(pixwidth/16)*16,(pixheight/16)*16,capturename,bitrate);
  _r_=system(command);
  return true;
}

extern bool gfl_MakeFlv(void)
{
  fprintf(stderr,"\e[1mCREATION VIDEO [%s.flv]\e[0m\n",capturename);
  snprintf(command,2047,"\
mencoder mf://%s/*.png -mf fps=%d:type=png:w=%d:h=%d -of lavf -ovc lavc\
-lavcopts vcodec=flv:vbitrate=%d:mbd=2:mv0:trell:v4mv:cbp:last_pred=3 -o %s.flv"
                       ,tempdirname,fps,pixwidth,pixheight,bitrate,capturename);
  _r_=system(command);
  return true;
}



extern bool gfl_FFmpeg(void)
{
  snprintf(command,2047,"ffmpeg -r %d -b %d -i %s/* %s.mp4"
                       ,fps,bitrate,tempdirname,capturename);
  _r_=system(command);
  return true;
}

#ifdef __cplusplus
  }
#endif
