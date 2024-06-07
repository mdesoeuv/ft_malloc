HOSTTYPE=$(uname -m)_$(uname -s)
M_LOGLEVEL="DEBUG" LD_PRELOAD="libft_malloc_$HOSTTYPE.so" LD_LIBRARY_PATH=$(pwd) $@