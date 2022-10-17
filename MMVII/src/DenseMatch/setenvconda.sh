#!/usr/bin/env bash

# set here the path to your CONDAHOME
export CONDAHOME=/opt/anaconda3



export PYTHONHOME=$CONDAHOME
export CONDA_SHELL=$CONDAHOME/etc/profile.d/conda.sh

# extracting the version of python
#export PYTHON_VERSION=3.9
export PYTHON_VERSION=`python --version`
export PYTHON_PREFIX="Python "
export PYTHON_VERSION=${PYTHON_VERSION#"$PYTHON_PREFIX"}
export PYTHON_VERSION=`echo $PYTHON_VERSION | cut -c1-3`
echo Python: $PYTHON_VERSION


export CONDA_ENV=psmnet_env
export PYTHON_ENV=$CONDA_ENV

# source et modele PSMNET
PSMNET_ROOT=https://github.com/erupnik/PSMNet.git
PSMNET_HASH=9ba1e36903f3ba2c99e5be8f03d31d2751a2cb33
MODEL_FOLDER=PSMNet
DEFAULTMODELPATH=`pwd`/$MODEL_FOLDER/models/finetune_PSMnet.tar
MODEL_URL="https://drive.google.com/uc?id=1JzVwoUuCdXfKmB26rPyV3vISRqgOUZxj"

foo=${PYTHON_VERSION#"$PYTHON_PREFIX"}
