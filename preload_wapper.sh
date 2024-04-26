HOSTTYPE=$(uname -m)_$(uname -s)
FT_MALLOC_LOG_LEVEL=1 LD_PRELOAD="libft_malloc_$HOSTTYPE.so" LD_LIBRARY_PATH=$(pwd) $@