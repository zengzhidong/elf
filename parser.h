
#include <types.h>

#ifndef __ELF_PARSER_H__
#define __ELF_PARSER_H__

NK_CPP_EXTERN_BEGIN

#pragma pack(push, 4)

typedef struct NK_Parser {
#define NK_This struct NK_Parser *const

    NK_Int
    (*parse)(NK_This);

    /**
     * @brief
     *  dump header
     *
     * @param[in] 
     *  
     *
     * @retval 0
     *  成功。
     *
     * @retval -1
     *  失败。
     */
    NK_Int
    (*header)(NK_This);

    /**
     * @brief
     *  dump section tab
     *
     * @param[in] 
     *  
     *
     * @retval 0
     *  成功。
     *
     * @retval -1
     *  失败。
     */
    NK_Int
    (*section)(NK_This);

    /**
     * @brief
     *  dump section tab
     *
     * @param[in] 
     *  
     *
     * @retval 0
     *  成功。
     *
     * @retval -1
     *  失败。
     */
    NK_Int
    (*symtab)(NK_This);

#undef NK_This
} NK_Parser;

#pragma pack(pop)


/**
 * 创建 ELF 解析器。
 */
NK_API NK_Parser *
NK_Parse_Create(const NK_PChar elf);

/**
 * 销毁 ELF 解析器。
 */
NK_API NK_Int
NK_Parse_Free(NK_Parser **parser);

NK_CPP_EXTERN_END
#endif /* __ELF_PARSER_H__ */

