/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vchevill <vchevill@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/05 11:37:18 by mdesoeuv          #+#    #+#             */
/*   Updated: 2022/02/14 11:03:18 by vchevill         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBFT_H

# define LIBFT_H

# include <stdlib.h>
# include <unistd.h>
# include <stdio.h>

typedef struct s_list
{
	void			*content;
	struct s_list	*next;
}							t_list;

void			*ft_memalloc(size_t size);
void			ft_memdel(void **ap);
char			*ft_strnew(size_t size);
void			ft_strdel(char **as);
void			ft_strclr(char *s);
void			ft_striter(char *s, void (*f)(char *));
void			ft_striteri(char *s, void (*f)(unsigned int, char *));
char			*ft_strmap(char const *s, char (*f)(char));
char			*ft_strmapi(char const *s, char (*f)(unsigned int, char));
int				ft_strequ(char const *s1, char const *s2);
int				ft_strnequ(char const *s1, char const *s2, size_t n);
char			*ft_substr(char const *s, unsigned int start, size_t len);
char			*ft_strjoin(char const *s1, char const *s2);
char			*ft_strtrim(char const *s, char const *set);
char			**ft_split(char const *s, char c);
char			*ft_itoa(int n);
void			ft_putchar(char c);
void			ft_putstr(char const *s);
void			ft_putendl(char const *s);
void			ft_putnbr(int n);
void			ft_putchar_fd(char c, int fd);
void			ft_putstr_fd(char *s, int fd);
void			ft_putendl_fd(char *s, int fd);
void			ft_putnbr_fd(int n, int fd);
void			*ft_memset(void *b, int c, size_t len);
void			ft_bzero(void *s, size_t n);
void			*ft_memcpy(void *dest, const void *src, size_t size);
void			*ft_memccpy(void *dest, const void *src, int ch,
					size_t maxSize);
void			*ft_memmove(void *dest, const void *src, size_t size);
void			*ft_memchr(const void *memoryBlock, int searchedChar,
					size_t size);
int				ft_memcmp(const void *s1, const void *s2, size_t n);
int				ft_strlen(const char *s);
char			*ft_strdup(const char *s);
char			*ft_strcpy(char *dst, const char *src);
char			*ft_strncpy(char *dst, const char *src, size_t len);
char			*ft_strcat(char *dst, const char *src);
char			*ft_strncat(char *dst, const char *src, size_t n);
int				ft_strlcat(char *dst, const char *src, size_t size);
char			*ft_strchr(const char *s, int c);
char			*ft_strrchr(const char *s, int c);
char			*ft_strstr(const char *haystack, const char *needle);
char			*ft_strnstr(const char *haystack, const char *needle,
					size_t len);
int				ft_strcmp(const char *s1, const char *s2);
int				ft_strncmp(const char *s1, const char *s2, size_t n);
size_t			ft_strlcpy(char *dst, const char *src, size_t dstsize);
int				ft_atoi(char const *str);
int				ft_isalpha(int c);
int				ft_isdigit(int c);
int				ft_isalnum(int c);
int				ft_isascii(int c);
int				ft_isprint(int c);
int				ft_is_charset(char c, char *charset);
int				ft_toupper(int c);
int				ft_tolower(int c);
void			*ft_calloc(size_t count, size_t size);
t_list			*ft_lstnew(void *content);
void			ft_lstdelone(t_list *alst, void (*del)(void *));
void			ft_lstclear(t_list **alst, void (*del)(void *));
void			ft_lstadd_front(t_list **alst, t_list *new_elem);
void			ft_lstadd_back(t_list **alst, t_list *new_elem);
void			ft_lstiter(t_list *lst, void (*f)(void *));
t_list			*ft_lstmap(t_list *lst, void *(*f)(void *),
					void (*del)(void *));
int				ft_lstsize(t_list *list);
t_list			*ft_lstlast(t_list *lst);
int				ft_isspace(int c);
int				ft_isblank(int c);
char			*ft_strndup(const char *s1, size_t len);
void			ft_swap(int			*a, int *b);
void			ft_swap_p(void **a, void **b);
void			ft_putnbr_base(int nbr, char *base);
char			*ft_strjoin_free_s1(char *s1, char *s2);
char			*ft_strjoin_free_s2(char *s1, char *s2);
char			*ft_strjoin_free(char *s1, char *s2);
char			*ft_strndup(const char *s1, size_t len);
int				ft_min(int a, int b);
int				ft_max(int a, int b);
int				ft_abs(int n);
int				ft_if(int condition, int true, int false);
char			**free_return_null(char **tab, int j);
char			**malloc_return(char ***tab, char const *s, char c);
char			*ft_split_strdup(char const *s, int i, int len);
long int		ft_atol(char const *str);
void			ft_free_split(char **split);
#endif
