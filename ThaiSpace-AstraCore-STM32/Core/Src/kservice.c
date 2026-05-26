/*
 * kservice.c
 *
 *  Created on: 11 May 2025
 *      Author: Pongpot
 */

#include <cmsis_os.h>
#include <kservice.h>

/* use precision */
#define TSC_PRINTF_PRECISION
/* use long long */
#define TSC_PRINTF_LONGLONG

/**
 * @addtogroup KernelService
 */

/**@{*/

#ifndef TSC_KSERVICE_USING_STDLIB_MEMSET
/**
 * This function will set the content of memory to specified value.
 *
 * @param  s is the address of source memory, point to the memory block to be filled.
 *
 * @param  c is the value to be set. The value is passed in int form, but the function
 *         uses the unsigned character form of the value when filling the memory block.
 *
 * @param  count number of bytes to be set.
 *
 * @return The address of source memory.
 */
__weak void *tsc_memset(void *s, int c, unsigned long count)
{
#ifdef TSC_KSERVICE_USING_TINY_SIZE
    char *xs = (char *)s;

    while (count--)
        *xs++ = c;

    return s;
#else
#define LBLOCKSIZE      (sizeof(long))
#define UNALIGNED(X)    ((long)X & (LBLOCKSIZE - 1))
#define TOO_SMALL(LEN)  ((LEN) < LBLOCKSIZE)

    unsigned int i;
    char *m = (char *)s;
    unsigned long buffer;
    unsigned long *aligned_addr;
    unsigned int d = c & 0xff;  /* To avoid sign extension, copy C to an
                                unsigned variable.  */

    if (!TOO_SMALL(count) && !UNALIGNED(s))
    {
        /* If we get this far, we know that count is large and s is word-aligned. */
        aligned_addr = (unsigned long *)s;

        /* Store d into each char sized location in buffer so that
         * we can set large blocks quickly.
         */
        if (LBLOCKSIZE == 4)
        {
            buffer = (d << 8) | d;
            buffer |= (buffer << 16);
        }
        else
        {
            buffer = 0;
            for (i = 0; i < LBLOCKSIZE; i ++)
                buffer = (buffer << 8) | d;
        }

        while (count >= LBLOCKSIZE * 4)
        {
            *aligned_addr++ = buffer;
            *aligned_addr++ = buffer;
            *aligned_addr++ = buffer;
            *aligned_addr++ = buffer;
            count -= 4 * LBLOCKSIZE;
        }

        while (count >= LBLOCKSIZE)
        {
            *aligned_addr++ = buffer;
            count -= LBLOCKSIZE;
        }

        /* Pick up the remainder with a bytewise loop. */
        m = (char *)aligned_addr;
    }

    while (count--)
    {
        *m++ = (char)d;
    }

    return s;

#undef LBLOCKSIZE
#undef UNALIGNED
#undef TOO_SMALL
#endif /* TSC_KSERVICE_USING_TINY_SIZE */
}

#endif /* TSC_KSERVICE_USING_STDLIB_MEMSET */

#ifndef TSC_KSERVICE_USING_STDLIB_MEMCPY
/**
 * This function will copy memory content from source address to destination address.
 *
 * @param  dst is the address of destination memory, points to the copied content.
 *
 * @param  src  is the address of source memory, pointing to the data source to be copied.
 *
 * @param  count is the copied length.
 *
 * @return The address of destination memory
 */
__weak void *tsc_memcpy(void *dst, const void *src, unsigned long count)
{
#ifdef TSC_KSERVICE_USING_TINY_SIZE
    char *tmp = (char *)dst, *s = (char *)src;
    unsigned long len;

    if (tmp <= s || tmp > (s + count))
    {
        while (count--)
            *tmp ++ = *s ++;
    }
    else
    {
        for (len = count; len > 0; len --)
            tmp[len - 1] = s[len - 1];
    }

    return dst;
#else

#define UNALIGNED(X, Y) \
    (((long)X & (sizeof (long) - 1)) | ((long)Y & (sizeof (long) - 1)))
#define BIGBLOCKSIZE    (sizeof (long) << 2)
#define LITTLEBLOCKSIZE (sizeof (long))
#define TOO_SMALL(LEN)  ((LEN) < BIGBLOCKSIZE)

    char *dst_ptr = (char *)dst;
    char *src_ptr = (char *)src;
    long *aligned_dst;
    long *aligned_src;
    int len = count;

    /* If the size is small, or either SRC or DST is unaligned,
    then punt into the byte copy loop.  This should be rare. */
    if (!TOO_SMALL(len) && !UNALIGNED(src_ptr, dst_ptr))
    {
        aligned_dst = (long *)dst_ptr;
        aligned_src = (long *)src_ptr;

        /* Copy 4X long words at a time if possible. */
        while (len >= BIGBLOCKSIZE)
        {
            *aligned_dst++ = *aligned_src++;
            *aligned_dst++ = *aligned_src++;
            *aligned_dst++ = *aligned_src++;
            *aligned_dst++ = *aligned_src++;
            len -= BIGBLOCKSIZE;
        }

        /* Copy one long word at a time if possible. */
        while (len >= LITTLEBLOCKSIZE)
        {
            *aligned_dst++ = *aligned_src++;
            len -= LITTLEBLOCKSIZE;
        }

        /* Pick up any residual with a byte copier. */
        dst_ptr = (char *)aligned_dst;
        src_ptr = (char *)aligned_src;
    }

    while (len--)
        *dst_ptr++ = *src_ptr++;

    return dst;
#undef UNALIGNED
#undef BIGBLOCKSIZE
#undef LITTLEBLOCKSIZE
#undef TOO_SMALL
#endif /* TSC_KSERVICE_USING_TINY_SIZE */
}

#endif /* TSC_KSERVICE_USING_STDLIB_MEMCPY */

#ifndef TSC_KSERVICE_USING_STDLIB

/**
 * This function will move memory content from source address to destination
 * address. If the destination memory does not overlap with the source memory,
 * the function is the same as memcpy().
 *
 * @param  dest is the address of destination memory, points to the copied content.
 *
 * @param  src is the address of source memory, point to the data source to be copied.
 *
 * @param  n is the copied length.
 *
 * @return The address of destination memory.
 */
void *tsc_memmove(void *dest, const void *src, size_t n)
{
    char *tmp = (char *)dest, *s = (char *)src;

    if (s < tmp && tmp < s + n)
    {
        tmp += n;
        s += n;

        while (n--)
            *(--tmp) = *(--s);
    }
    else
    {
        while (n--)
            *tmp++ = *s++;
    }

    return dest;
}

/**
 * This function will compare two areas of memory.
 *
 * @param  cs is a block of memory.
 *
 * @param  ct is another block of memory.
 *
 * @param  count is the size of the area.
 *
 * @return Compare the results:
 *         If the result < 0, cs is smaller than ct.
 *         If the result > 0, cs is greater than ct.
 *         If the result = 0, cs is equal to ct.
 */
int32_t tsc_memcmp(const void *cs, const void *ct, size_t count)
{
    const unsigned char *su1, *su2;
    int res = 0;

    for (su1 = (const unsigned char *)cs, su2 = (const unsigned char *)ct; 0 < count; ++su1, ++su2, count--)
        if ((res = *su1 - *su2) != 0)
            break;

    return res;
}

/**
 * This function will return the length of a string, which terminate will
 * null character.
 *
 * @param  s is the string
 *
 * @return The length of string.
 */
size_t tsc_strlen(const char *s)
{
    const char *sc;

    for (sc = s; *sc != '\0'; ++sc) /* nothing */
        ;

    return sc - s;
}

/**
 * This function will return the first occurrence of a string, without the
 * terminator '\0'.
 *
 * @param  s1 is the source string.
 *
 * @param  s2 is the find string.
 *
 * @return The first occurrence of a s2 in s1, or NULL if no found.
 */
char *tsc_strstr(const char *s1, const char *s2)
{
    int l1, l2;

    l2 = tsc_strlen(s2);
    if (!l2)
        return (char *)s1;
    l1 = tsc_strlen(s1);
    while (l1 >= l2)
    {
        l1 --;
        if (!tsc_memcmp(s1, s2, l2))
            return (char *)s1;
        s1 ++;
    }

    return NULL;
}

/**
 * This function will compare two strings while ignoring differences in case
 *
 * @param  a is the string to be compared.
 *
 * @param  b is the string to be compared.
 *
 * @return Compare the results:
 *         If the result < 0, a is smaller than a.
 *         If the result > 0, a is greater than a.
 *         If the result = 0, a is equal to a.
 */
int32_t tsc_strcasecmp(const char *a, const char *b)
{
    int ca, cb;

    do
    {
        ca = *a++ & 0xff;
        cb = *b++ & 0xff;
        if (ca >= 'A' && ca <= 'Z')
            ca += 'a' - 'A';
        if (cb >= 'A' && cb <= 'Z')
            cb += 'a' - 'A';
    }
    while (ca == cb && ca != '\0');

    return ca - cb;
}

/**
 * This function will copy string no more than n bytes.
 *
 * @param  dst points to the address used to store the copied content.
 *
 * @param  src is the string to be copied.
 *
 * @param  n is the maximum copied length.
 *
 * @return The address where the copied content is stored.
 */
char *tsc_strncpy(char *dst, const char *src, size_t n)
{
    if (n != 0)
    {
        char *d = dst;
        const char *s = src;

        do
        {
            if ((*d++ = *s++) == 0)
            {
                /* NUL pad the remaining n-1 bytes */
                while (--n != 0)
                    *d++ = 0;
                break;
            }
        } while (--n != 0);
    }

    return (dst);
}

/**
 * This function will copy string.
 *
 * @param  dst points to the address used to store the copied content.
 *
 * @param  src is the string to be copied.
 *
 * @return The address where the copied content is stored.
 */
char *tsc_strcpy(char *dst, const char *src)
{
    char *dest = dst;

    while (*src != '\0')
    {
        *dst = *src;
        dst++;
        src++;
    }

    *dst = '\0';
    return dest;
}

/**
 * This function will compare two strings with specified maximum length.
 *
 * @param  cs is the string to be compared.
 *
 * @param  ct is the string to be compared.
 *
 * @param  count is the maximum compare length.
 *
 * @return Compare the results:
 *         If the result < 0, cs is smaller than ct.
 *         If the result > 0, cs is greater than ct.
 *         If the result = 0, cs is equal to ct.
 */
int32_t tsc_strncmp(const char *cs, const char *ct, size_t count)
{
    register signed char __res = 0;

    while (count)
    {
        if ((__res = *cs - *ct++) != 0 || !*cs++)
            break;
        count --;
    }

    return __res;
}

/**
 * This function will compare two strings without specified length.
 *
 * @param  cs is the string to be compared.
 *
 * @param  ct is the string to be compared.
 *
 * @return Compare the results:
 *         If the result < 0, cs is smaller than ct.
 *         If the result > 0, cs is greater than ct.
 *         If the result = 0, cs is equal to ct.
 */
int32_t tsc_strcmp(const char *cs, const char *ct)
{
    while (*cs && *cs == *ct)
    {
        cs++;
        ct++;
    }

    return (*cs - *ct);
}

#endif /* TSC_KSERVICE_USING_STDLIB */

#if !defined(TSC_KSERVICE_USING_STDLIB) || defined(__ARMCC_VERSION)
/**
 * The  strnlen()  function  returns the number of characters in the
 * string pointed to by s, excluding the terminating null byte ('\0'),
 * but at most maxlen.  In doing this, strnlen() looks only at the
 * first maxlen characters in the string pointed to by s and never
 * beyond s+maxlen.
 *
 * @param  s is the string.
 *
 * @param  maxlen is the max size.
 *
 * @return The length of string.
 */
size_t tsc_strnlen(const char *s, unsigned long maxlen)
{
    const char *sc;

    for (sc = s; *sc != '\0' && (unsigned long)(sc - s) < maxlen; ++sc) /* nothing */
        ;

    return sc - s;
}

#ifdef __ARMCC_VERSION
size_t strnlen(const char *s, size_t maxlen) __attribute__((alias("tsc_strnlen")));
#endif /* __ARMCC_VERSION */
#endif /* !defined(TSC_KSERVICE_USING_STDLIB) || defined(__ARMCC_VERSION) */

#ifdef TSC_USING_HEAP
/**
 * This function will duplicate a string.
 *
 * @param  s is the string to be duplicated.
 *
 * @return The string address of the copy.
 */
char *tsc_strdup(const char *s)
{
    size_t len = tsc_strlen(s) + 1;
    char *tmp = (char *)tsc_malloc(len);

    if (!tmp)
        return NULL;

    tsc_memcpy(tmp, s, len);

    return tmp;
}
#ifdef __ARMCC_VERSION
char *strdup(const char *s) __attribute__((alias("tsc_strdup")));
#endif /* __ARMCC_VERSION */
#endif /* TSC_USING_HEAP */


/* private function */
#define _ISDIGIT(c)  ((unsigned)((c) - '0') < 10)

/**
 * This function will duplicate a string.
 *
 * @param  n is the string to be duplicated.
 *
 * @param  base is support divide instructions value.
 *
 * @return the duplicated string pointer.
 */
#ifdef TSC_PRINTF_LONGLONG
__STATIC_INLINE int divide(long long *n, int base)
#else
__STATIC_INLINE int divide(long *n, int base)
#endif /* TSC_PRINTF_LONGLONG */
{
    int res;

    /* optimized for processor which does not support divide instructions. */
    if (base == 10)
    {
#ifdef TSC_PRINTF_LONGLONG
        res = (int)(((unsigned long long)*n) % 10U);
        *n = (long long)(((unsigned long long)*n) / 10U);
#else
        res = (int)(((unsigned long)*n) % 10U);
        *n = (long)(((unsigned long)*n) / 10U);
#endif
    }
    else
    {
#ifdef TSC_PRINTF_LONGLONG
        res = (int)(((unsigned long long)*n) % 16U);
        *n = (long long)(((unsigned long long)*n) / 16U);
#else
        res = (int)(((unsigned long)*n) % 16U);
        *n = (long)(((unsigned long)*n) / 16U);
#endif
    }

    return res;
}

__STATIC_INLINE int skip_atoi(const char **s)
{
    register int i = 0;
    while (_ISDIGIT(**s))
        i = i * 10 + *((*s)++) - '0';

    return i;
}

#define ZEROPAD     (1 << 0)    /* pad with zero */
#define SIGN        (1 << 1)    /* unsigned/signed long */
#define PLUS        (1 << 2)    /* show plus */
#define SPACE       (1 << 3)    /* space if plus */
#define LEFT        (1 << 4)    /* left justified */
#define SPECIAL     (1 << 5)    /* 0x */
#define LARGE       (1 << 6)    /* use 'ABCDEF' instead of 'abcdef' */

static char *print_number(char *buf,
                          char *end,
#ifdef TSC_PRINTF_LONGLONG
                          long long  num,
#else
                          long  num,
#endif /* TSC_PRINTF_LONGLONG */
                          int   base,
                          int   s,
#ifdef TSC_PRINTF_PRECISION
                          int   precision,
#endif /* TSC_PRINTF_PRECISION */
                          int   type)
{
    char c, sign;
#ifdef TSC_PRINTF_LONGLONG
    char tmp[32];
#else
    char tmp[16];
#endif /* TSC_PRINTF_LONGLONG */
    int precision_bak = precision;
    const char *digits;
    static const char small_digits[] = "0123456789abcdef";
    static const char large_digits[] = "0123456789ABCDEF";
    register int i;
    register int size;

    size = s;

    digits = (type & LARGE) ? large_digits : small_digits;
    if (type & LEFT)
        type &= ~ZEROPAD;

    c = (type & ZEROPAD) ? '0' : ' ';

    /* get sign */
    sign = 0;
    if (type & SIGN)
    {
        if (num < 0)
        {
            sign = '-';
            num = -num;
        }
        else if (type & PLUS)
            sign = '+';
        else if (type & SPACE)
            sign = ' ';
    }

#ifdef TSC_PRINTF_SPECIAL
    if (type & SPECIAL)
    {
        if (base == 16)
            size -= 2;
        else if (base == 8)
            size--;
    }
#endif /* TSC_PRINTF_SPECIAL */

    i = 0;
    if (num == 0)
        tmp[i++] = '0';
    else
    {
        while (num != 0)
            tmp[i++] = digits[divide(&num, base)];
    }

#ifdef TSC_PRINTF_PRECISION
    if (i > precision)
        precision = i;
    size -= precision;
#else
    size -= i;
#endif /* TSC_PRINTF_PRECISION */

    if (!(type & (ZEROPAD | LEFT)))
    {
        if ((sign) && (size > 0))
            size--;

        while (size-- > 0)
        {
            if (buf < end)
                *buf = ' ';
            ++ buf;
        }
    }

    if (sign)
    {
        if (buf < end)
        {
            *buf = sign;
        }
        -- size;
        ++ buf;
    }

#ifdef TSC_PRINTF_SPECIAL
    if (type & SPECIAL)
    {
        if (base == 8)
        {
            if (buf < end)
                *buf = '0';
            ++ buf;
        }
        else if (base == 16)
        {
            if (buf < end)
                *buf = '0';
            ++ buf;
            if (buf < end)
            {
                *buf = type & LARGE ? 'X' : 'x';
            }
            ++ buf;
        }
    }
#endif /* TSC_PRINTF_SPECIAL */

    /* no align to the left */
    if (!(type & LEFT))
    {
        while (size-- > 0)
        {
            if (buf < end)
                *buf = c;
            ++ buf;
        }
    }

#ifdef TSC_PRINTF_PRECISION
    while (i < precision--)
    {
        if (buf < end)
            *buf = '0';
        ++ buf;
    }
#endif /* TSC_PRINTF_PRECISION */

    /* put number in the temporary buffer */
    while (i-- > 0 && (precision_bak != 0))
    {
        if (buf < end)
            *buf = tmp[i];
        ++ buf;
    }

    while (size-- > 0)
    {
        if (buf < end)
            *buf = ' ';
        ++ buf;
    }

    return buf;
}

/**
 * This function will fill a formatted string to buffer.
 *
 * @param  buf is the buffer to save formatted string.
 *
 * @param  size is the size of buffer.
 *
 * @param  fmt is the format parameters.
 *
 * @param  args is a list of variable parameters.
 *
 * @return The number of characters actually written to buffer.
 */
__weak int tsc_vsnprintf(char *buf, size_t size, const char *fmt, va_list args)
{
#ifdef TSC_PRINTF_LONGLONG
    unsigned long long num;
#else
    uint32_t num;
#endif /* TSC_PRINTF_LONGLONG */
    int i, len;
    char *str, *end, c;
    const char *s;

    uint8_t base;            /* the base of number */
    uint8_t flags;           /* flags to print number */
    uint8_t qualifier;       /* 'h', 'l', or 'L' for integer fields */
    int32_t field_width;     /* width of output field */

#ifdef TSC_PRINTF_PRECISION
    int precision;      /* min. # of digits for integers and max for a string */
#endif /* TSC_PRINTF_PRECISION */

    str = buf;
    end = buf + size;

    /* Make sure end is always >= buf */
    if (end < buf)
    {
        end  = ((char *) - 1);
        size = end - buf;
    }

    for (; *fmt ; ++fmt)
    {
        if (*fmt != '%')
        {
            if (str < end)
                *str = *fmt;
            ++ str;
            continue;
        }

        /* process flags */
        flags = 0;

        while (1)
        {
            /* skips the first '%' also */
            ++ fmt;
            if (*fmt == '-') flags |= LEFT;
            else if (*fmt == '+') flags |= PLUS;
            else if (*fmt == ' ') flags |= SPACE;
            else if (*fmt == '#') flags |= SPECIAL;
            else if (*fmt == '0') flags |= ZEROPAD;
            else break;
        }

        /* get field width */
        field_width = -1;
        if (_ISDIGIT(*fmt)) field_width = skip_atoi(&fmt);
        else if (*fmt == '*')
        {
            ++ fmt;
            /* it's the next argument */
            field_width = va_arg(args, int);
            if (field_width < 0)
            {
                field_width = -field_width;
                flags |= LEFT;
            }
        }

#ifdef TSC_PRINTF_PRECISION
        /* get the precision */
        precision = -1;
        if (*fmt == '.')
        {
            ++ fmt;
            if (_ISDIGIT(*fmt)) precision = skip_atoi(&fmt);
            else if (*fmt == '*')
            {
                ++ fmt;
                /* it's the next argument */
                precision = va_arg(args, int);
            }
            if (precision < 0) precision = 0;
        }
#endif /* TSC_PRINTF_PRECISION */
        /* get the conversion qualifier */
        qualifier = 0;
#ifdef TSC_PRINTF_LONGLONG
        if (*fmt == 'h' || *fmt == 'l' || *fmt == 'L')
#else
        if (*fmt == 'h' || *fmt == 'l')
#endif /* TSC_PRINTF_LONGLONG */
        {
            qualifier = *fmt;
            ++ fmt;
#ifdef TSC_PRINTF_LONGLONG
            if (qualifier == 'l' && *fmt == 'l')
            {
                qualifier = 'L';
                ++ fmt;
            }
#endif /* TSC_PRINTF_LONGLONG */
        }

        /* the default base */
        base = 10;

        switch (*fmt)
        {
        case 'c':
            if (!(flags & LEFT))
            {
                while (--field_width > 0)
                {
                    if (str < end) *str = ' ';
                    ++ str;
                }
            }

            /* get character */
            c = (uint8_t)va_arg(args, int);
            if (str < end) *str = c;
            ++ str;

            /* put width */
            while (--field_width > 0)
            {
                if (str < end) *str = ' ';
                ++ str;
            }
            continue;

        case 's':
            s = va_arg(args, char *);
            if (!s) s = "(NULL)";

            for (len = 0; (len != field_width) && (s[len] != '\0'); len++);
#ifdef TSC_PRINTF_PRECISION
            if (precision > 0 && len > precision) len = precision;
#endif /* TSC_PRINTF_PRECISION */

            if (!(flags & LEFT))
            {
                while (len < field_width--)
                {
                    if (str < end) *str = ' ';
                    ++ str;
                }
            }

            for (i = 0; i < len; ++i)
            {
                if (str < end) *str = *s;
                ++ str;
                ++ s;
            }

            while (len < field_width--)
            {
                if (str < end) *str = ' ';
                ++ str;
            }
            continue;

        case 'p':
            if (field_width == -1)
            {
                field_width = sizeof(void *) << 1;
                flags |= ZEROPAD;
            }
#ifdef TSC_PRINTF_PRECISION
            str = print_number(str, end,
                               (long)va_arg(args, void *),
                               16, field_width, precision, flags);
#else
            str = print_number(str, end,
                               (long)va_arg(args, void *),
                               16, field_width, flags);
#endif /* TSC_PRINTF_PRECISION */
            continue;

        case '%':
            if (str < end) *str = '%';
            ++ str;
            continue;

        /* integer number formats - set up the flags and "break" */
        case 'o':
            base = 8;
            break;

        case 'X':
            flags |= LARGE;
        case 'x':
            base = 16;
            break;

        case 'd':
        case 'i':
            flags |= SIGN;
        case 'u':
            break;

        default:
            if (str < end) *str = '%';
            ++ str;

            if (*fmt)
            {
                if (str < end) *str = *fmt;
                ++ str;
            }
            else
            {
                -- fmt;
            }
            continue;
        }

#ifdef TSC_PRINTF_LONGLONG
        if (qualifier == 'L') num = va_arg(args, long long);
        else if (qualifier == 'l')
#else
        if (qualifier == 'l')
#endif /* TSC_PRINTF_LONGLONG */
        {
            num = va_arg(args, uint32_t);
            if (flags & SIGN) num = (int32_t)num;
        }
        else if (qualifier == 'h')
        {
            num = (uint16_t)va_arg(args, int32_t);
            if (flags & SIGN) num = (int16_t)num;
        }
        else
        {
            num = va_arg(args, uint32_t);
            if (flags & SIGN) num = (int32_t)num;
        }
#ifdef TSC_PRINTF_PRECISION
        str = print_number(str, end, num, base, field_width, precision, flags);
#else
        str = print_number(str, end, num, base, field_width, flags);
#endif /* TSC_PRINTF_PRECISION */
    }

    if (size > 0)
    {
        if (str < end) *str = '\0';
        else
        {
            end[-1] = '\0';
        }
    }

    /* the trailing null byte doesn't count towards the total
    * ++str;
    */
    return str - buf;
}

/**
 * This function will fill a formatted string to buffer.
 *
 * @param  buf is the buffer to save formatted string.
 *
 * @param  size is the size of buffer.
 *
 * @param  fmt is the format parameters.
 *
 * @return The number of characters actually written to buffer.
 */
int tsc_snprintf(char *buf, size_t size, const char *fmt, ...)
{
    int32_t n;
    va_list args;

    va_start(args, fmt);
    n = tsc_vsnprintf(buf, size, fmt, args);
    va_end(args);

    return n;
}

/**
 * This function will fill a formatted string to buffer.
 *
 * @param  buf is the buffer to save formatted string.
 *
 * @param  format is the format parameters.
 *
 * @param  arg_ptr is a list of variable parameters.
 *
 * @return The number of characters actually written to buffer.
 */
int tsc_vsprintf(char *buf, const char *format, va_list arg_ptr)
{
    return tsc_vsnprintf(buf, (size_t) - 1, format, arg_ptr);
}

/**
 * This function will fill a formatted string to buffer
 *
 * @param  buf the buffer to save formatted string.
 *
 * @param  format is the format parameters.
 *
 * @return The number of characters actually written to buffer.
 */
int tsc_sprintf(char *buf, const char *format, ...)
{
    int32_t n;
    va_list arg_ptr;

    va_start(arg_ptr, format);
    n = tsc_vsprintf(buf, format, arg_ptr);
    va_end(arg_ptr);

    return n;
}

#ifdef TSC_USING_CONSOLE

__weak void tsc_hw_console_output(const char *str)
{
    /* empty console output */
}

/**
 * This function will put string to the console.
 *
 * @param str is the string output to the console.
 */
void tsc_kputs(const char *str)
{
    if (!str) return;

    tsc_hw_console_output(str);
}

/**
 * This function will print a formatted string on system console.
 *
 * @param fmt is the format parameters.
 *
 * @return The number of characters actually written to buffer.
 */
static char tsc_log_buf[TSC_CONSOLEBUF_SIZE];
__weak int tsc_kprintf(const char *fmt, ...)
{
    va_list args;
    size_t length;

    va_start(args, fmt);
    /* the return value of vsnprintf is the number of bytes that would be
     * written to buffer had if the size of the buffer been sufficiently
     * large excluding the terminating null byte. If the output string
     * would be larger than the tsc_log_buf, we have to adjust the output
     * length. */
    length = tsc_vsnprintf(tsc_log_buf, sizeof(tsc_log_buf) - 1, fmt, args);
    if (length > TSC_CONSOLEBUF_SIZE - 1)
        length = TSC_CONSOLEBUF_SIZE - 1;

    tsc_hw_console_output(tsc_log_buf);

    va_end(args);

    return length;
}
#endif /* TSC_USING_CONSOLE */

#if defined(TSC_USING_HEAP) && !defined(TSC_USING_USERHEAP)
#ifdef TSC_USING_HOOK
static void (*tsc_malloc_hook)(void *ptr, size_t size);
static void (*tsc_free_hook)(void *ptr);

/**
 * @addtogroup Hook
 */

/**@{*/

/**
 * @brief This function will set a hook function, which will be invoked when a memory
 *        block is allocated from heap memory.
 *
 * @param hook the hook function.
 */
void tsc_malloc_sethook(void (*hook)(void *ptr, size_t size))
{
    tsc_malloc_hook = hook;
}

/**
 * @brief This function will set a hook function, which will be invoked when a memory
 *        block is released to heap memory.
 *
 * @param hook the hook function
 */
void tsc_free_sethook(void (*hook)(void *ptr))
{
    tsc_free_hook = hook;
}

/**@}*/

#endif /* TSC_USING_HOOK */

#if defined(TSC_USING_HEAP_ISR)
#elif defined(TSC_USING_MUTEX)
static struct tsc_mutex _lock;
#endif

__STATIC_INLINE void _heap_lock_init(void)
{
#if defined(TSC_USING_HEAP_ISR)
#elif defined(TSC_USING_MUTEX)
    tsc_mutex_init(&_lock, "heap", TSC_IPC_FLAG_PRIO);
#endif
}

__STATIC_INLINE long _heap_lock(void)
{
#if defined(TSC_USING_HEAP_ISR)
    return tsc_hw_interrupt_disable();
#elif defined(TSC_USING_MUTEX)
    if (tsc_thread_self())
        return tsc_mutex_take(&_lock, TSC_WAITING_FOREVER);
    else
        return TSC_EOK;
#else
    taskENTER_CRITICAL();
    return TSC_EOK;
#endif
}

__STATIC_INLINE void _heap_unlock(long level)
{
#if defined(TSC_USING_HEAP_ISR)
    tsc_hw_interrupt_enable(level);
#elif defined(TSC_USING_MUTEX)
    TSC_ASSERT(level == TSC_EOK);
    if (tsc_thread_self())
        tsc_mutex_release(&_lock);
#else
    taskEXIT_CRITICAL();
#endif
}

#if defined(TSC_USING_SMALL_MEM_AS_HEAP)
static tsc_smem_t system_heap;
__STATIC_INLINE void _smem_info(size_t *total,
    size_t *used, size_t *max_used)
{
    if (total)
        *total = system_heap->total;
    if (used)
        *used = system_heap->used;
    if (max_used)
        *max_used = system_heap->max;
}
#define _MEM_INIT(_name, _start, _size) \
    system_heap = tsc_smem_init(_name, _start, _size)
#define _MEM_MALLOC(_size)  \
    tsc_smem_alloc(system_heap, _size)
#define _MEM_REALLOC(_ptr, _newsize)\
    tsc_smem_realloc(system_heap, _ptr, _newsize)
#define _MEM_FREE(_ptr) \
    tsc_smem_free(_ptr)
#define _MEM_INFO(_total, _used, _max)  \
    _smem_info(_total, _used, _max)
#elif defined(TSC_USING_MEMHEAP_AS_HEAP)
static struct tsc_memheap system_heap;
void *_memheap_alloc(struct tsc_memheap *heap, size_t size);
void _memheap_free(void *rmem);
void *_memheap_realloc(struct tsc_memheap *heap, void *rmem, size_t newsize);
#define _MEM_INIT(_name, _start, _size) \
    tsc_memheap_init(&system_heap, _name, _start, _size)
#define _MEM_MALLOC(_size)  \
    _memheap_alloc(&system_heap, _size)
#define _MEM_REALLOC(_ptr, _newsize)    \
    _memheap_realloc(&system_heap, _ptr, _newsize)
#define _MEM_FREE(_ptr)   \
    _memheap_free(_ptr)
#define _MEM_INFO(_total, _used, _max)   \
    tsc_memheap_info(&system_heap, _total, _used, _max)
#elif defined(TSC_USING_SLAB_AS_HEAP)
static tsc_slab_t system_heap;
__STATIC_INLINE void _slab_info(size_t *total,
    size_t *used, size_t *max_used)
{
    if (total)
        *total = system_heap->total;
    if (used)
        *used = system_heap->used;
    if (max_used)
        *max_used = system_heap->max;
}
#define _MEM_INIT(_name, _start, _size) \
    system_heap = tsc_slab_init(_name, _start, _size)
#define _MEM_MALLOC(_size)  \
    tsc_slab_alloc(system_heap, _size)
#define _MEM_REALLOC(_ptr, _newsize)    \
    tsc_slab_realloc(system_heap, _ptr, _newsize)
#define _MEM_FREE(_ptr) \
    tsc_slab_free(system_heap, _ptr)
#define _MEM_INFO       _slab_info
#else
#define _MEM_INIT(...)
#define _MEM_MALLOC(_size)  pvPortMalloc(_size)
#define _MEM_REALLOC(...)   NULL
#define _MEM_FREE(_ptr)		vPortFree(_ptr)
#define _MEM_INFO(...)
#endif

/**
 * @brief This function will init system heap.
 *
 * @param begin_addr the beginning address of system page.
 *
 * @param end_addr the end address of system page.
 */
__weak void tsc_system_heap_init(void *begin_addr, void *end_addr)
{
    unsigned long begin_align = TSC_ALIGN((unsigned long)begin_addr, TSC_ALIGN_SIZE);
    unsigned long end_align   = TSC_ALIGN_DOWN((unsigned long)end_addr, TSC_ALIGN_SIZE);

    TSC_ASSERT(end_align > begin_align);

    /* Initialize system memory heap */
    _MEM_INIT("heap", begin_addr, end_align - begin_align);
    /* Initialize multi thread contention lock */
    _heap_lock_init();
}

/**
 * @brief Allocate a block of memory with a minimum of 'size' bytes.
 *
 * @param size is the minimum size of the requested block in bytes.
 *
 * @return the pointer to allocated memory or NULL if no free memory was found.
 */
__weak void *tsc_malloc(size_t size)
{
    long level;
    void *ptr;

    /* Enter critical zone */
    level = _heap_lock();
    /* allocate memory block from system heap */
    ptr = _MEM_MALLOC(size);
    /* Exit critical zone */
    _heap_unlock(level);
//    /* call 'tsc_malloc' hook */
//    TSC_OBJECT_HOOK_CALL(tsc_malloc_hook, (ptr, size));
    return ptr;
}

/**
 * @brief This function will change the size of previously allocated memory block.
 *
 * @param rmem is the pointer to memory allocated by tsc_malloc.
 *
 * @param newsize is the required new size.
 *
 * @return the changed memory block address.
 */
__weak void *tsc_realloc(void *rmem, size_t newsize)
{
    long level;
    void *nptr;

    /* Enter critical zone */
    level = _heap_lock();
    /* Change the size of previously allocated memory block */
    nptr = _MEM_REALLOC(rmem, newsize);
    /* Exit critical zone */
    _heap_unlock(level);
    return nptr;
}

/**
 * @brief  This function will contiguously allocate enough space for count objects
 *         that are size bytes of memory each and returns a pointer to the allocated
 *         memory.
 *
 * @note   The allocated memory is filled with bytes of value zero.
 *
 * @param  count is the number of objects to allocate.
 *
 * @param  size is the size of one object to allocate.
 *
 * @return pointer to allocated memory / NULL pointer if there is an error.
 */
__weak void *tsc_calloc(size_t count, size_t size)
{
    void *p;

    /* allocate 'count' objects of size 'size' */
    p = tsc_malloc(count * size);
    /* zero the memory */
    if (p)
    {
        tsc_memset(p, 0, count * size);
    }
    return p;
}

/**
 * @brief This function will release the previously allocated memory block by
 *        tsc_malloc. The released memory block is taken back to system heap.
 *
 * @param rmem the address of memory which will be released.
 */
__weak void tsc_free(void *rmem)
{
    long level;

//    /* call 'tsc_free' hook */
//    TSC_OBJECT_HOOK_CALL(tsc_free_hook, (rmem));
    /* Enter critical zone */
    level = _heap_lock();
    _MEM_FREE(rmem);
    /* Exit critical zone */
    _heap_unlock(level);
}

/**
* @brief This function will caculate the total memory, the used memory, and
*        the max used memory.
*
* @param total is a pointer to get the total size of the memory.
*
* @param used is a pointer to get the size of memory used.
*
* @param max_used is a pointer to get the maximum memory used.
*/
__weak void tsc_memory_info(size_t *total,
                            size_t *used,
                            size_t *max_used)
{
    long level;

    /* Enter critical zone */
    level = _heap_lock();
    _MEM_INFO(total, used, max_used);
    /* Exit critical zone */
    _heap_unlock(level);
}

/**
 * This function allocates a memory block, which address is aligned to the
 * specified alignment size.
 *
 * @param  size is the allocated memory block size.
 *
 * @param  align is the alignment size.
 *
 * @return The memory block address was returned successfully, otherwise it was
 *         returned empty NULL.
 */
__weak void *tsc_malloc_align(size_t size, size_t align)
{
    void *ptr;
    void *align_ptr;
    int uintptr_size;
    size_t align_size;

    /* sizeof pointer */
    uintptr_size = sizeof(void*);
    uintptr_size -= 1;

    /* align the alignment size to uintptr size byte */
    align = ((align + uintptr_size) & ~uintptr_size);

    /* get total aligned size */
    align_size = ((size + uintptr_size) & ~uintptr_size) + align;
    /* allocate memory block from heap */
    ptr = tsc_malloc(align_size);
    if (ptr != NULL)
    {
        /* the allocated memory block is aligned */
        if (((unsigned long)ptr & (align - 1)) == 0)
        {
            align_ptr = (void *)((unsigned long)ptr + align);
        }
        else
        {
            align_ptr = (void *)(((unsigned long)ptr + (align - 1)) & ~(align - 1));
        }

        /* set the pointer before alignment pointer to the real pointer */
        *((unsigned long *)((unsigned long)align_ptr - sizeof(void *))) = (unsigned long)ptr;

        ptr = align_ptr;
    }

    return ptr;
}

/**
 * This function release the memory block, which is allocated by
 * tsc_malloc_align function and address is aligned.
 *
 * @param ptr is the memory block pointer.
 */
__weak void tsc_free_align(void *ptr)
{
    void *real_ptr;

    real_ptr = (void *) * (unsigned long *)((unsigned long)ptr - sizeof(void *));
    tsc_free(real_ptr);
}

#endif /* TSC_USING_HEAP */

#ifndef TSC_USING_CPU_FFS
#ifdef TSC_USING_TINY_FFS
const uint8_t __lowest_bit_bitmap[] =
{
    /*  0 - 7  */  0,  1,  2, 27,  3, 24, 28, 32,
    /*  8 - 15 */  4, 17, 25, 31, 29, 12, 32, 14,
    /* 16 - 23 */  5,  8, 18, 32, 26, 23, 32, 16,
    /* 24 - 31 */ 30, 11, 13,  7, 32, 22, 15, 10,
    /* 32 - 36 */  6, 21,  9, 20, 19
};

/**
 * This function finds the first bit set (beginning with the least significant bit)
 * in value and return the index of that bit.
 *
 * Bits are numbered starting at 1 (the least significant bit).  A return value of
 * zero from any of these functions means that the argument was zero.
 *
 * @return return the index of the first bit set. If value is 0, then this function
 * shall return 0.
 */
int __tsc_ffs(int value)
{
    return __lowest_bit_bitmap[(uint32_t)(value & (value - 1) ^ value) % 37];
}
#else
const uint8_t __lowest_bit_bitmap[] =
{
    /* 00 */ 0, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
    /* 10 */ 4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
    /* 20 */ 5, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
    /* 30 */ 4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
    /* 40 */ 6, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
    /* 50 */ 4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
    /* 60 */ 5, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
    /* 70 */ 4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
    /* 80 */ 7, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
    /* 90 */ 4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
    /* A0 */ 5, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
    /* B0 */ 4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
    /* C0 */ 6, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
    /* D0 */ 4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
    /* E0 */ 5, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
    /* F0 */ 4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0
};

/**
 * This function finds the first bit set (beginning with the least significant bit)
 * in value and return the index of that bit.
 *
 * Bits are numbered starting at 1 (the least significant bit).  A return value of
 * zero from any of these functions means that the argument was zero.
 *
 * @return Return the index of the first bit set. If value is 0, then this function
 *         shall return 0.
 */
int __tsc_ffs(int value)
{
    if (value == 0) return 0;

    if (value & 0xff)
        return __lowest_bit_bitmap[value & 0xff] + 1;

    if (value & 0xff00)
        return __lowest_bit_bitmap[(value & 0xff00) >> 8] + 9;

    if (value & 0xff0000)
        return __lowest_bit_bitmap[(value & 0xff0000) >> 16] + 17;

    return __lowest_bit_bitmap[(value & 0xff000000) >> 24] + 25;
}
#endif /* TSC_USING_TINY_FFS */
#endif /* TSC_USING_CPU_FFS */

#ifndef __on_tsc_assetsc_hook
    #define __on_tsc_assetsc_hook(ex, func, line)         __ON_HOOK_ARGS(tsc_assetsc_hook, (ex, func, line))
#endif

#ifdef TSC_DEBUG
/* TSC_ASSERT(EX)'s hook */

void (*tsc_assert_hook)(const char *ex, const char *func, size_t line);

/**
 * This function will set a hook function to TSC_ASSERT(EX). It will run when the expression is false.
 *
 * @param hook is the hook function.
 */
void tsc_assert_set_hook(void (*hook)(const char *ex, const char *func, size_t line))
{
    tsc_assert_hook = hook;
}

/**
 * The TSC_ASSERT function.
 *
 * @param ex_string is the assertion condition string.
 *
 * @param func is the function name when assertion.
 *
 * @param line is the file line number when assertion.
 */
void tsc_assert_handler(const char *ex_string, const char *func, size_t line)
{
    volatile char dummy = 0;

    if (tsc_assert_hook == NULL)
    {
#ifdef TSC_USING_MODULE
        if (dlmodule_self())
        {
            /* close assertion module */
            dlmodule_exit(-1);
        }
        else
#endif /*TSC_USING_MODULE*/
        {
            tsc_kprintf("(%s) assertion failed at function:%s, line number:%d \n", ex_string, func, line);
            while (dummy == 0);
        }
    }
    else
    {
    	tsc_assert_hook(ex_string, func, line);
    }
}
#endif /* TSC_DEBUG */

/**@}*/
