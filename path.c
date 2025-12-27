#include "shell.h"

/**
 * _strlen - obtiene la longitud de una cadena
 * @s: cadena
 *
 * Return: longitud
 */
size_t _strlen(const char *s)
{
    const char *p = s;
    while (*p)
        p++;
    return ((size_t)(p - s));
}

/**
 * _strcmp - compara dos cadenas
 * @s1: cadena 1
 * @s2: cadena 2
 *
 * Return: 0 si iguales, distinto si no
 */
int _strcmp(const char *s1, const char *s2)
{
    while (*s1 && *s2 && *s1 == *s2)
    {
        s1++;
        s2++;
    }
    return ((unsigned char)*s1 - (unsigned char)*s2);
}

/**
 * _strchr - busca caracter en cadena
 * @s: cadena
 * @c: caracter
 *
 * Return: puntero en s o NULL
 */
char *_strchr(const char *s, int c)
{
    while (*s)
    {
        if (*s == (char)c)
            return ((char *)s);
        s++;
    }
    if (c == '\0')
        return ((char *)s);
    return (NULL);
}

/**
 * _memcpy - copia memoria
 * @dest: destino
 * @src: origen
 * @n: bytes
 *
 * Return: dest
 */
void *_memcpy(void *dest, const void *src, size_t n)
{
    size_t i;
    unsigned char *d = (unsigned char *)dest;
    const unsigned char *s = (const unsigned char *)src;

    for (i = 0; i < n; i++)
        d[i] = s[i];
    return (dest);
}

/**
 * _strcpy - copia cadena
 * @dest: destino
 * @src: origen
 *
 * Return: dest
 */
char *_strcpy(char *dest, const char *src)
{
    char *d = dest;
    while (*src)
    {
        *d++ = *src++;
    }
    *d = '\0';
    return (dest);
}

/**
 * get_env_value - obtiene el valor de una variable de entorno
 * @name: nombre de variable
 *
 * Return: puntero al valor o NULL
 */
char *get_env_value(const char *name)
{
    size_t nlen;
    int i;

    if (!name)
        return (NULL);
    nlen = _strlen(name);

    for (i = 0; environ && environ[i]; i++)
    {
        if (!strncmp(environ[i], name, nlen) && environ[i][nlen] == '=')
            return (environ[i] + nlen + 1);
    }
    return (NULL);
}

/**
 * join_path - construye "dir/cmd" en nueva cadena
 * @dir: directorio
 * @cmd: comando
 *
 * Return: cadena nueva o NULL
 */
static char *join_path(const char *dir, const char *cmd)
{
    size_t ld, lc;
    char *out;

    ld = _strlen(dir);
    lc = _strlen(cmd);
    out = malloc(ld + 1 + lc + 1);
    if (!out)
        return (NULL);
    _memcpy(out, dir, ld);
    out[ld] = '/';
    _memcpy(out + ld + 1, cmd, lc);
    out[ld + 1 + lc] = '\0';
    return (out);
}

/**
 * resolve_command - resuelve comando por PATH o ruta directa
 * @cmd: nombre del comando
 *
 * Return: ruta completa nueva o NULL
 */
char *resolve_command(const char *cmd)
{
    const char *path_env;
    char *copy, *p, *start, *candidate;
    struct stat st;

    if (!cmd || *cmd == '\0')
        return (NULL);

    if (_strchr(cmd, '/'))
    {
        if (stat(cmd, &st) == 0 && access(cmd, X_OK) == 0)
        {
            char *ret = malloc(_strlen(cmd) + 1);
            if (ret)
                _strcpy(ret, cmd);
            return (ret);
        }
        return (NULL);
    }

    path_env = get_env_value("PATH");
    if (!path_env || *path_env == '\0')
        return (NULL);

    copy = malloc(_strlen(path_env) + 1);
    if (!copy)
        return (NULL);
    _strcpy(copy, path_env);

    p = copy;
    while (*p)
    {
        start = p;
        while (*p && *p != ':')
            p++;
        if (*p == ':')
            *p++ = '\0';

        if (*start == '\0')
            start = ".";

        candidate = join_path(start, cmd);
        if (candidate)
        {
            if (stat(candidate, &st) == 0 && access(candidate, X_OK) == 0)
            {
                free(copy);
                return (candidate);
            }
            free(candidate);
        }
    }

    free(copy);
    return (NULL);
}
