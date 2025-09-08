#pragma once

#ifndef __has_builtin
    #define __has_builtin(x)
#endif

#ifndef __has_attribute
    #define __has_attribute(x)
#endif

#ifndef unlikely
    #if __has_builtin(__builtin_expect)
        #define unlikely(x) __builtin_expect(!!(x), 0)
    #else
        #define unlikely(x)
    #endif
#endif

#ifndef likely
    #if __has_builtin(__builtin_expect)
        #define likely(x) __builtin_expect(!!(x), 1)
    #else
        #define likely(x)
    #endif
#endif

#ifndef trap
    #if __has_builtin(__builtin_verbose_trap)
        #define trap(s) __builtin_verbose_trap(s, s)
    #else
        #define trap(s) __builtin_trap()
    #endif
#endif

#ifndef assume
    #if __has_builtin(__builtin_assume)
        #define assume(x) __builtin_assume(!!(x))
    #else
        #define assume(x) \
            do { \
                if (unlikely(!(x))) { \
                    __builtin_unreachable(); \
                } \
            } while (0)
    #endif
#endif

// Expands to nothing
#define NOTHING()

#pragma region STR

#define _STR(...) #__VA_ARGS__

// Preprocess args into a string literal
#define STR(...) _STR(__VA_ARGS__)

#pragma endregion

#pragma region GLUE

#define _GLUE2(a_, b_) a_##b_

// Glue arguments together
#define GLUE2(a_, b_) _GLUE2(a_, b_)

#pragma endregion

#pragma region FIRST & SECOND

// Strip everything but the first argument
#define FIRST(a, ...) a

// Strip everything but the second argument
#define SECOND(a, b, ...) b

#pragma endregion

#pragma region ISZERO & NONZERO

#define _ISPROBE(...) SECOND(__VA_ARGS__, 0)
#define _PROBE()      $, 1
#define _ISZERO_0     _PROBE()

// Expands to `1` if `a` is `0`, otherwise expands to `0`.
#define ISZERO(a) _ISPROBE(GLUE2(_ISZERO_, a))

// Expands to `0` if `a` is `0`, otherwise expands to `1`.
#define NONZERO(a_) ISZERO(ISZERO(a_))

#pragma endregion

#pragma region NONEMPTY & ISEMPTY

#define _EOA() 0

// Expands to `1` if if macro is passed any arguments, otherwise expands to `0`
#define NONEMPTY(...) NONZERO(FIRST(_EOA __VA_ARGS__)())

// Expands to `0` if if macro is passed any arguments, otherwise expands to `1`
#define ISEMPTY(...) ISZERO(NONEMPTY(__VA_ARGS__))

#pragma endregion

#pragma region IFELSE

#define _IF_0(...) _IF_0_ELSE
#define _IF_1(...) __VA_ARGS__ _IF_1_ELSE

#define _IF_0_ELSE(...) __VA_ARGS__
#define _IF_1_ELSE(...)

#define _IFELSE(cond) GLUE2(_IF_, cond)

// `IFELSE(cond)(if)(else)` expands to `else` if `cond` is `0`, otherwise expands to `if`
#define IFELSE(cond) _IFELSE(NONZERO(cond))

#pragma endregion

#pragma region EVAL

#define EVAL1(...)    __VA_ARGS__
#define EVAL2(...)    EVAL1(EVAL1(__VA_ARGS__))
#define EVAL4(...)    EVAL2(EVAL2(__VA_ARGS__))
#define EVAL8(...)    EVAL4(EVAL4(__VA_ARGS__))
#define EVAL16(...)   EVAL8(EVAL8(__VA_ARGS__))
#define EVAL32(...)   EVAL16(EVAL16(__VA_ARGS__))
#define EVAL64(...)   EVAL32(EVAL32(__VA_ARGS__))
#define EVAL128(...)  EVAL64(EVAL64(__VA_ARGS__))
#define EVAL256(...)  EVAL128(EVAL128(__VA_ARGS__))
#define EVAL512(...)  EVAL256(EVAL256(__VA_ARGS__))
#define EVAL1024(...) EVAL512(EVAL512(__VA_ARGS__))

#define EVAL(...) EVAL1024(__VA_ARGS__)

#pragma endregion

#pragma region DEFER

#define DEFER1(a) a NOTHING()
#define DEFER2(a) a NOTHING NOTHING()()

#pragma endregion

#pragma region MAP

#define __MAP() _MAP
#define _MAP(m, a, ...) \
    m(a) IFELSE(NONEMPTY(__VA_ARGS__))(, DEFER2(__MAP)()(m, __VA_ARGS__))()

// Iterate another macro across arguments
#define MAP(m, ...) \
    IFELSE(NONEMPTY(__VA_ARGS__))(EVAL(_MAP(m, __VA_ARGS__)))()


#define __DOUBLE_MAP() _DOUBLE_MAP
#define _DOUBLE_MAP(m, a, b, ...) \
    m(a, b) IFELSE(NONEMPTY(__VA_ARGS__))(, DEFER2(__DOUBLE_MAP)()(m, __VA_ARGS__))()

// Iterate another macro across a pair of arguments
#define DOUBLE_MAP(m, ...) \
    IFELSE(NONEMPTY(__VA_ARGS__))(EVAL(_DOUBLE_MAP(_, __VA_ARGS__)))()

#pragma endregion
#pragma region JOIN

#define __JOIN() _JOIN

#define _JOIN(d, args, ...) \
    args IFELSE(NONEMPTY(__VA_ARGS__))(d DEFER2(__JOIN)()(d, __VA_ARGS__))()


// Separate every argument with a delimiter `d`
#define JOIN(d, ...) \
    IFELSE(NONEMPTY(__VA_ARGS__))(EVAL(_JOIN(d, __VA_ARGS__)))()

#pragma endregion
