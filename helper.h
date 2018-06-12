/*
*  Author: LW
*  Source: https://github.com/Mambabosso/C-Tools/blob/master/helper.h
*  License: https://github.com/Mambabosso/C-Tools/blob/master/LICENSE
*/


#define ENDOFSTRING '\0'

#ifdef ENDOFSTRING

#include <stdlib.h>
#include <stdarg.h>
#include <stdbool.h>


int strlength(const char *str);
int strccount(const char *str, const char c);
char * strsplit(const char *str, const char delimiter, const int offset, int *pos);
char ** strarrsplit(const char *str, const char delimiter, int *arrlen);
char * strfmt(const char *str, ...);
char * strinn(const char *str, const int pos, const int length);
char * strins(const char *str, const int pos, const char *s);
char * strdel(const char *str, const int pos, const int length);
char * strcmb(const char *str, const char *s);
int filesize(const char *path);
char * filecontent(const char *path);
bool filewrite(const char *path, const char *content);
bool fileappend(const char *path, const int pos, const char *content);

const char nullstr[1] = { '\0' };


int strlength(const char *str)
{
    int result = 0;
    while ((str[result] != ENDOFSTRING))
    {
        result++;
    }
    return result;
}

int strccount(const char *str, const char c)
{
    int result = 0;
    int i = 0;
    while ((str[i] != ENDOFSTRING))
    {
        if (str[i] == c)
        {
            result++;
        }
        i++;
    }
    return result;
}

char * strsplit(const char *str, const char delimiter, const int offset, int *pos)
{
    char *result;
    int i = offset;
    while ((str[i] != ENDOFSTRING) && (str[i] != delimiter))
    {
        i++;
    }
    result = (char*)malloc(sizeof(char) * (i - offset) + sizeof(char));
    i = offset;
    while ((str[i] != ENDOFSTRING) && (str[i] != delimiter))
    {
        result[i - offset] = str[i];
        i++;
    }
    result[i - offset] = ENDOFSTRING;
    *pos = ((i >= strlength(str)) ? -1 : ++i);
    return result;
}

char ** strarrsplit(const char *str, const char delimiter, int *arrlen)
{
    char **result;
    int ccount = strccount(str, delimiter) + 1;
    result = (char**)malloc(sizeof(char*) * ccount);
    int i = 0;
    char *ptr;
    int len = 0;
    int pos = 0;
    for (i = 0; i < ccount; i++)
    {
        ptr = strsplit(str, delimiter, pos, &pos);
        len = strlength(ptr);
        result[i] = (char*)malloc(sizeof(char) * len + sizeof(char));
        result[i] = ptr;
        result[i][len] = ENDOFSTRING;
    }
    *arrlen = ccount;
    return result;
}

char * strfmt(const char *str, ...)
{
    char *result;
    int len = 0;
    va_list valist;
    va_start(valist, str);
    len = vsnprintf(NULL, 0, str, valist);
    result = (char*)malloc(sizeof(char) * len + sizeof(char));
    vsnprintf(result, len + 1, str, valist);
    va_end(valist);
    result[len] = ENDOFSTRING;
    return result;
}

char * strinn(const char *str, const int pos, const int length)
{
    char *result = (char*)malloc(sizeof(char) * length + sizeof(char));
    int i = 0;
    for (i = pos; i < pos + length; i++)
    {
        result[i - pos] = str[i];
    }
    result[pos + length] = ENDOFSTRING;
    return result;
}

char * strins(const char *str, const int pos, const char *s)
{
    char *result;
    int slen = strlength(str);
    int ilen = strlength(s);
    int i = 0;
    result = (char*)malloc(sizeof(char) * slen + sizeof(char) * ilen + sizeof(char));
    for (i = 0; i < pos; i++)
    {
        result[i] = str[i];
    }
    for (i = 0; i < ilen; i++)
    {
        result[i + pos] = s[i];
    }
    for (i = pos; i < slen; i++)
    {
        result[i + ilen] = str[i];
    }
    result[slen + ilen] = ENDOFSTRING;
    return result;
}

char * strdel(const char *str, const int pos, const int length)
{
    char *result;
    int slen = strlength(str);
    int i = 0;
    if (length <= -1)
    {
        result = (char*)malloc(sizeof(char) * pos + sizeof(char));
        for (i = 0; i < pos; i++)
        {
            result[i] = str[i];
        }
        result[pos] = ENDOFSTRING;
    }
    else
    {
        result = (char*)malloc(sizeof(char) * slen - sizeof(char) * length + sizeof(char));
        for (i = 0; i < pos; i++)
        {
            result[i] = str[i];
        }
        for (i = pos + length; i < slen; i++)
        {
            result[i - length] = str[i];
        }
        result[slen - length] = ENDOFSTRING;
    }
    return result;
}

char * strcmb(const char *str, const char *s)
{
    char *result;
    int slen1 = strlength(str);
    int slen2 = strlength(s);
    int i = 0;
    result = (char*)malloc(sizeof(char) * slen1 + sizeof(char) * slen2 + sizeof(char));
    for (i = 0; i < slen1; i++)
    {
        result[i] = str[i];
    }
    for (i = 0; i < slen2; i++)
    {
        result[i + slen1] = s[i];
    }
    result[slen1 + slen2] = ENDOFSTRING;
    return result;
}

int filesize(const char *path)
{
    int result = -1;
    FILE *file = fopen(path, "r");
    if (file)
    {
        result = 0;
        while (fgetc(file) != -1)
        {
            result++;
        }
        fclose(file);
    }
    return result;
}

char * filecontent(const char *path)
{
    char *result;
    FILE *file = fopen(path, "r");
    int fsize = filesize(path);
    if (file && fsize > -1)
    {
        result = (char*)malloc(sizeof(char) * fsize + sizeof(char));
        if (result)
        {
            fread(result, sizeof(char), fsize, file);
            result[fsize] = ENDOFSTRING;
            fclose(file);
            return result;
        }
    }
    return NULL;
}

bool filewrite(const char *path, const char *content)
{
    bool result = false;
    FILE *file = fopen(path, "w");
    if (file)
    {
        fprintf(file, "%s", content);
        fclose(file);
        result = true;
    }
    return result;
}

bool fileappend(const char *path, const int pos, const char *content)
{
    bool result = false;
    char *c = filecontent(path);
    FILE *file;
    if (c)
    {
        file = fopen(path, "w");
        if (file)
        {
            if (pos <= -1)
            {
                c = strcmb(c, content);
                fprintf(file, "%s", c);
                fclose(file);
                result = true;
            }
            else
            {
                c = strins(c, pos, content);
                fprintf(file, "%s", c);
                fclose(file);
                result = true;
            }
        }
        free(c);
    }
    return result;
}


#endif // ENDOFSTRING

