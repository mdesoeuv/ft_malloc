HOSTTYPE=$(uname -m)_$(uname -s)
M_LOGLEVEL="TRACE" LD_PRELOAD="libft_malloc_$HOSTTYPE.so" LD_LIBRARY_PATH=$(pwd) $@