#!/usr/bin/env bash

#Introduction: https://www.youtube.com/watch?v=N5vscPTWKOk

export OLD_DIR=`pwd`
cd "$(dirname "${BASH_SOURCE[0]}")"

. ./setenvconda.sh
force_download=0
force_reinstall=0
SET_PYTHON_ENV=1


set PYTHON
if [[ "$SET_PYTHON_ENV" == 1 ]]; then
    if [ "$PYTHONHOME" != "" ]; then
        PATH=$PYTHONHOME/bin:$PATH
    fi
    echo PATH: $PATH
    PYTHON=`which python$PYTHON_VERSION`
    PIP=`which pip$PYTHON_VERSION`
    echo PYTHON=$PYTHON
    PYTHONHOME=
    PYTHONPATH="$(dirname "$PYTHON")"
    PYTHON_CONFIG=$PYTHONPATH/python$PYTHON_VERSION-config
    export PYTHONHOME=`$PYTHON_CONFIG --exec-prefix`
    echo PYTHONHOME reset to $PYTHONHOME
else
    PYTHON=`which python$PYTHON_VERSION`
    PIP=`which pip$PYTHON_VERSION`
fi

echo PYTHON=$PYTHON
echo PIP=$PIP
echo PATH=$PATH

echo "which PYTHON="`which python`
echo "which PIP="`which pip`
echo "which virtualenv="`which virtualenv`

# clone PSMNet
if [ ! -d PSMNet ]; then
    echo clone PSMNet
    echo "cloning PSMNET..."
    git clone $PSMNET_ROOT
    # create new branch 'test'
    git checkout -b test
fi

cd $MODEL_FOLDER

# reset to initial commit
git branch

if [[ "`git branch`" == *"test"* ]]; then
    git checkout  test
else
    git checkout -b test
fi

git reset --hard $PSMNET_HASH

#download a trained model
if [ "$force_reinstall" == 1 ]; then
    conda deactivate ${CONDA_ENV}
    conda create --name -p python$PYTHON_VERSION ${CONDA_ENV}
    if [ -d models ]; then
        rm -rf models
    fi
fi

if [ ! -e "$DEFAULTMODELPATH" ]; then
    echo get default trained model
    wget "$MODEL_URL" -O ${DEFAULTMODELPATH}
fi

#create a new virtualenv named psmnet_env
echo create/activate the environment conda $CONDA_ENV

if [ "$force_reinstall" == 1 ]; then
    conda remove --name $CONDA_ENV --all
fi

conda create --yes --name ${CONDA_ENV} python=$PYTHON_VERSION
conda activate ${CONDA_ENV}

echo PYTHONPATH=$PYTHONPATH
echo PYTHONHOME=$PYTHONHOME

cd ..

OLD_DIR=`pwd`
cd "$(dirname "${BASH_SOURCE[0]}")"
SCRIPTPATH="`pwd`"
PATH=$SCRIPTPATH:$PATH
cd $OLD_DIR

REQUIREMENTS=`which requirements.txt`
if [ ! -e "$REQUIREMENTS" ]; then
    echo cannot find requirements.txt. update environment variable PATH and relaunch
    exit 1
fi

#add modules to the env
echo install requirements

pip install -r "$REQUIREMENTS"  # > /dev/null 2>&1

if [ $? -eq 0 ]; then
    echo python extra modules installed
else
    echo pip install has failed. Exiting
    # echo Exiting
    #exit 1
fi

cd $OLD_DIR
