#!/bin/bash
# config. dans ~/.bashrc
printf '#---------------------------------------------------------------------------------\n# ' cat >> $HOME/.bashrc;
date | cat >> $HOME/.bashrc;
echo "# Variables d'environnement pour la lib. graphique <gfl> - Version 7a - 2024/2025
#---------------------------------------------------------------------------------
# libGFL2d - installee comme une lib. locale (repertoire d'installation $PWD)
export GFL_VERSION='7a-25'
export GL_PATH='/usr/lib/x86_64-linux-gnu/'
export GFL_PATH=$PWD/gfl
export libGFL2='-lm -L\$(GL_PATH) -lGL -lGLU -lglut -L\$(GFL_PATH)/bin -lgfl2d'
export incGFL2='-I\$(GFL_PATH)/include -D_GFL2D_'
export libGFL3='-lm -L\$(GL_PATH) -lGL -lGLU -lglut -L\$(GFL_PATH)/bin -lgfl3d'
export incGFL3='-I\$(GFL_PATH)/include -D_GFL3D_'
export LD_LIBRARY_PATH=\$LD_LIBRARY_PATH:\$GFL_PATH/bin
#---------------------------------------------------------------------------------
"| cat >> $HOME/.bashrc;
# compilation de la lib
cd gfl/
make cleanall;
make DIM=2 GDB=0 CPP=0; make clean;      # compilation 'C'   en mode optimisé -> libgfl2d.{so,a}
make DIM=2 GDB=1 CPP=0; make clean;      # compilation 'C'   en mode debug    -> libgfl2d.gdb.{so,a}
make DIM=2 GDB=0 CPP=1; make clean;      # compilation 'C++' en mode optimisé -> libgfl2d++.{so,a}
make DIM=2 GDB=1 CPP=1; make clean;      # compilation 'C++' en mode debug    -> libgfl2d++.gdb.{so,a}
make DIM=3 GDB=0 CPP=0; make clean;      # compilation 'C'   en mode optimisé -> libgfl3d.{so,a}
make DIM=3 GDB=1 CPP=0; make clean;      # compilation 'C'   en mode debug    -> libgfl3d.gdb.{so,a}
make DIM=3 GDB=0 CPP=1; make clean;      # compilation 'C++' en mode optimisé -> libgfl3d++.{so,a}
make DIM=3 GDB=1 CPP=1; make clean;      # compilation 'C++' en mode debug    -> libgfl3d++.gdb.{so,a}
cd ../
# mise a jour bash
bash;
