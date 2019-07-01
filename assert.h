
#include <stdio.h>
#include <types.h>

#ifndef __ASSERT_H__
#define __ASSERT_H__

NK_CPP_EXTERN_BEGIN

/**
 * 条件逻辑测试。
 * 测试条件中的逻辑是否为真，当逻辑不为真时，根据 @ref __verbose 标识输出日志。
 */
#define NK_TEST_(__condition, __verbose) \
    ((__condition) ? NK_True : \
        ((__verbose) ? \
            (printf("\"%s() @ %s:%d\" Expect Condition ( \"%s\" ).\n", __PRETTY_FUNCTION__, __FILE__, __LINE__, #__condition), NK_False) : NK_False))

/**
 * 条件预设期望。
 * 测试条件中的逻辑非真时会输出提示日志。
 */
#define NK_EXPECT(__condition) \
    NK_TEST_(__condition, NK_True)

/**
 * 条件预设期望。
 * 在方法中通过判断一个预设条件，针对无返回值的方法当条件不为真时跳出方法，跳出过程中没有日志输出提示。
 */
#define NK_EXPECT_RETURN(__condition) \
    do{\
        if (!NK_TEST_(__condition, NK_False)){\
            return;\
        }\
    }while(0)

/**
 * 条件预设期望。
 * 见 @ref NK_EXPECT_RETURN()，针对带返回值的方法。
 */
#define NK_EXPECT_RETURN_VAL(__condition, __ret) \
    do{\
        if (!NK_TEST_(__condition, NK_False)){\
            return(__ret);\
        }\
    }while(0)


#define NK_EXPECT_JUMP(__condition, __location) \
    do{\
        if (!NK_TEST_(__condition, NK_False)){\
            goto __location;\
        }\
    }while(0)

#define NK_EXPECT_JMP NK_EXPECT_JUMP

#define NK_EXPECT_BREAK(__condition) \
    if (!NK_TEST_(__condition, NK_False)){\
        break;\
    }\

#define NK_EXPECT_BRK NK_EXPECT_BREAK


#define NK_EXPECT_CONTINUE(__condition) \
    if (!NK_TEST_(__condition, NK_False)){\
        continue;\
    }\

#define NK_EXPECT_CONT NK_EXPECT_CONTINUE

/**
 * 条件预设期望。
 * @ref NK_EXPECT_RETURN()，跳出过程中带有日志输出提示。
 */
#define NK_EXPECT_VERBOSE_RETURN(__condition) \
    do{\
        if (!NK_TEST_(__condition, NK_True)){\
            return;\
        }\
    }while(0)

/**
 * 条件预设期望。
 * @ref NK_EXPECT_RETURN()，跳出过程中带有日志输出提示，针对带返回值的方法。
 */
#define NK_EXPECT_VERBOSE_RETURN_VAL(__condition, __ret) \
    do{\
        if (!NK_TEST_(__condition, NK_True)){\
            return(__ret);\
        }\
    }while(0)

#define NK_EXPECT_VERBOSE_JUMP(__condition, __location) \
    do{\
        if (!NK_TEST_(__condition, NK_True)){\
            goto __location;\
        }\
    }while(0)

#define NK_EXPECT_VERBOSE_JMP NK_EXPECT_VERBOSE_JUMP


#define NK_EXPECT_VERBOSE_BREAK(__condition) \
    if (!NK_TEST_(__condition, NK_True)){\
        break;\
    }\


#define NK_EXPECT_VERBOSE_BRK NK_EXPECT_VERBOSE_BREAK


#define NK_EXPECT_VERBOSE_CONTINUE(__condition) \
    if (!NK_TEST_(__condition, NK_True)){\
        continue;\
    }\


#define NK_EXPECT_VERBOSE_CONT NK_EXPECT_VERBOSE_CONTINUE

NK_CPP_EXTERN_END
#endif /* __ASSERT_H__ */
