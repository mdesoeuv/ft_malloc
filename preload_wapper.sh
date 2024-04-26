HOSTTYPE=$(uname -m)_$(uname -s)
LD_PRELOAD="libft_malloc_$HOSTTYPE.so" LD_LIBRARY_PATH=$(pwd) $@