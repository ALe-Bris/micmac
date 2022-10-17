#!/usr/bin/env bash

export OLD_DIR=`pwd`
cd "$(dirname "${BASH_SOURCE[0]}")"
CURRENT_DIR=`pwd`

. ./setenvconda.sh

######### SET-UP
mm3dBin=$(which MMVII)
if [ "mm3dBin" == "" ]; then
    echo "MMVII not found. Exiting"
fi

DenseMDir="$(dirname "$mm3dBin")"
#DenseMDir="$(dirname "$DenseMDir")"
#DenseMDir=$DenseMDir"/src/DenseMatch/"
DenseMDir=$CURRENT_DIR/
echo DenseMDir=$DenseMDir
CodeDir=${DenseMDir}"PSMNet/"

#virtual env psmnet
if [ "$EnvName" == "" ]; then
    EnvName=$CONDA_ENV
fi

MODELPATH=${MODELPATH}
if [ "$MODELPATH" == "" ]; then
    MODELPATH=$DEFAULTMODELPATH
fi

if [ ! -e "$MODELPATH" ]; then
    echo current dir: `pwd`
    echo model "$MODELPATH" does not exist on disk. Exiting
    exit 1
fi

# disparity scale of the trained model
DISPSCALE=-256.0
########


# enter virtual env
# source ${DenseMDir}python_env/${EnvName}/bin/activate
echo python: `which python`
echo PYTHONHOME: $PYTHONHOME
source $CONDA_SHELL
conda activate $CONDA_ENV

cd $OLD_DIR

# run image matching
echo python3 ${CodeDir}Test_img.py --loadmodel ${MODELPATH} "$@"
python3 ${CodeDir}Test_img.py --loadmodel ${MODELPATH} "$@"

echo image matching done


# de-normalise disparities
echo de-normalise disparities ${6}
if [ ! -e "${6}" ]; then
    echo "cannot find ${6}. Exiting"
    exit 1
else
    mv ${6} ${6}_unnorm.tif
fi
mm3d Nikrup "/ ${6}_unnorm.tif ${DISPSCALE}" ${6}

# quit virtual env
conda deactivate
