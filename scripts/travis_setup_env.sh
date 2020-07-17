# script to be sourced in travis yml
# setup all enviroment variables

export CACHE_PREFIX=${HOME}/.cache/usr
export PATH=${HOME}/.local/bin:${PATH}
export PATH=${PATH}:${CACHE_PREFIX}/bin
export CPLUS_INCLUDE_PATH=${CPLUS_INCLUDE_PATH}:${CACHE_PREFIX}/include
export C_INCLUDE_PATH=${C_INCLUDE_PATH}:${CACHE_PREFIX}/include
export LIBRARY_PATH=${LIBRARY_PATH}:${CACHE_PREFIX}/lib
export LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:${CACHE_PREFIX}/lib
export DYLD_LIBRARY_PATH=${DYLD_LIBRARY_PATH}:${CACHE_PREFIX}/lib

alias make="make -j$(nproc)"

# setup the cache prefix folder
if [ ! -d ${HOME}/.cache ]; then
    mkdir ${HOME}/.cache
fi

if [ ! -d ${CACHE_PREFIX} ]; then
    mkdir ${CACHE_PREFIX}
fi
if [ ! -d ${CACHE_PREFIX}/include ]; then
    mkdir ${CACHE_PREFIX}/include
fi
if [ ! -d ${CACHE_PREFIX}/lib ]; then
    mkdir ${CACHE_PREFIX}/lib
fi
if [ ! -d ${CACHE_PREFIX}/bin ]; then
    mkdir ${CACHE_PREFIX}/bin
fi
