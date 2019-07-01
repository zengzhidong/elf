#include <parser.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <elf.h>
#include <utils.h>
#include <assert.h>

#define NKL_Debug   ("debug")
#define NKL_Info    ("info")
#define NKL_Waring  ("waring")
#define NKL_Error   ("error")
#define NKL_Alert   ("alert")

#define NKLOG(__log, __level, fmt, arg...) \
    (((__log) && (__level)) ? \
        printf("\033[36m[%s]\033[0m \033[31m[%s]\033[0m [%s:%d] "fmt"\n", (__log), (__level), __FUNCTION__, __LINE__, ##arg) \
            : printf(fmt, ##arg))

#define TRACE(fmt, arg...) printf(fmt, ##arg)

/**
 * Parser 模块私有句柄，句柄访问模块内部的私有成员。\n
 * 内存在 Parser 模块创建时统一分配。\n
 * 位置在句柄数据结构 @ref NK_Parser 上位，\n
 * 这样有效避免 @ref NK_Parser 内存空间被错误释放。\n
 * 如下图：\n
 *
 *  | NK_PrivatedParser
 * \|/
 *  +------------------------+
 *  |          |             |
 *  |          |             |
 *  +------------------------+
 *            /|\
 *             | NK_Parser
 *
 */
typedef struct NK_PrivatedParser {

    /// elf 源路径
    NK_Char Path[256];

    /// elf 源
    NK_PVoid Src;

} NK_PrivatedParser;

/**
 * 通过模块公有句柄获取私有句柄。
 */
static inline NK_PrivatedParser *
PRIVATED(NK_Parser *Public) {

    /// 偏移到私有句柄。
    return (NK_PrivatedParser *)Public - 1;
}

/**
 * 定义私有句柄。
 */
#define DECLARE_PRIVATED() NK_PrivatedParser *Privated = PRIVATED(Public)

/**
 * NK_This 指针定义，\n
 * 定义以下为模块 API 接口实现。
 */
#define NK_This NK_Parser * const Public

#define NK_Log ("Parser")

/**
 * parse start。
 */
static NK_Int
Elf_parse(NK_This) {

    /// 检测句柄异常。
    NK_EXPECT_VERBOSE_RETURN_VAL(NK_Nil != Public, -1);

    /// 获取私有句柄。
    DECLARE_PRIVATED();

    NK_EXPECT_VERBOSE_RETURN_VAL(0 < NK_ReadFile2Buffer(Privated->Path, (NK_PChar *)(&Privated->Src)), -1);
    NK_EXPECT_VERBOSE_RETURN_VAL(NK_Nil != Privated->Src, -1);

    return 0;
}

/**
 * dump header。
 */
static NK_Int
Elf_header(NK_This) {

    /// 检测句柄异常。
    NK_EXPECT_VERBOSE_RETURN_VAL(NK_Nil != Public, -1);

    /// 获取私有句柄。
    DECLARE_PRIVATED();

    /// 数据源检查
    NK_EXPECT_VERBOSE_RETURN_VAL(NK_Nil != Privated->Src, -1);

    NKLOG(NK_Log, NKL_Alert, "ELF header begin");

    NK_Int i;
    Elf32_Ehdr *Ehdr = (Elf32_Ehdr *)(Privated->Src);

    TRACE("ELF Headers:\n");

    /**
     * Magic number and other info
     */
    for (i = 0; i < EI_NIDENT; i++) {

        if (0 == i)
            TRACE("  Magic:\t%02hhx ", Ehdr->e_ident[i]);
        else if ((i + 1) == EI_NIDENT)
            TRACE("%02hhx\r\n", Ehdr->e_ident[i]);
        else
            TRACE("%02hhx ", Ehdr->e_ident[i]);
    }

    if (memcmp(&Ehdr->e_ident[EI_MAG0], ELFMAG, SELFMAG)) {
        return -1;
    }

    switch (Ehdr->e_ident[EI_CLASS])
    {
    case ELFCLASS32:
        TRACE("  Class:\tELF32\r\n"); break;
    case ELFCLASS64:
        TRACE("  Class:\tELF64\r\n"); break;
    default:
        TRACE("  Class:\tInvalid class\r\n"); break;
    }

    switch (Ehdr->e_ident[EI_DATA])
    {
    case ELFDATA2LSB:
        TRACE("  Date:t\t1's complement, little endian\r\n"); break;
    case ELFDATA2MSB:
        TRACE("  Date:t\t2's complement, big endian\r\n"); break;
    default:
        TRACE("  Date:\t\tInvalid data encoding\r\n"); break;
    }

    TRACE("  Version:\t%d\r\n", Ehdr->e_ident[EI_VERSION]);

    switch (Ehdr->e_ident[EI_OSABI])
    {
    case ELFOSABI_NONE:
        TRACE("  OS/ABI:\tUNIX System V ABI\r\n"); break;
    default:
        TRACE("  OS/ABI:\tUnknow\r\n"); break;
    }

    TRACE("  ABI Version:\t%d\r\n", Ehdr->e_ident[EI_ABIVERSION]);

    /**
     * Object file type
     */
    switch (Ehdr->e_type)
    {
    case ET_REL:
        TRACE("  Type:\t\tREL (Relocatable file)\r\n"); break;
    case ET_EXEC:
        TRACE("  Type:\t\tEXEC (Executable file)\r\n"); break;
    case ET_DYN:
        TRACE("  Type:\t\tDYN (Shared object file)\r\n"); break;
    case ET_CORE:
        TRACE("  Type:\t\tCORE (Core file)\r\n"); break;
    default:
        TRACE("  Type:\t\tNONE (No file type)\r\n"); break;
    }

    /**
     * Architecture
     */
    TRACE("  Machine:\t%d\r\n", Ehdr->e_machine);

    /**
     * Object file version
     */
    TRACE("  Version:\t%d\r\n", Ehdr->e_version);

    /**
     * Entry point virtual address
     */
    TRACE("  Entry Point address:\t0x%x\r\n", Ehdr->e_entry);

    /**
     * Program header table file offset
     */
    TRACE("  Start of program headers\t%d (bytes into file)\r\n", Ehdr->e_phoff);

    /**
     * Section header table file offset
     */
    TRACE("  Start of section headers:\t%d (bytes into file)\r\n", Ehdr->e_shoff);

    /**
     * Processor-specific flags
     */
    TRACE("  Flags:\t0x%x\r\n", Ehdr->e_flags);

    /**
     * ELF header size in bytes
     */
    TRACE("  Size of this headers:\t%d (bytes)\r\n", Ehdr->e_ehsize);

    /**
     * Program header table entry size
     */
    TRACE("  Size of this program headers:\t%d (bytes)\r\n", Ehdr->e_phentsize);

    /**
     * Program header table entry count
     */
    TRACE("  Number of program headers:\t%d\r\n", Ehdr->e_phnum);

    /**
     * Section header table entry size
     */
    TRACE("  Size of section headers:\t%d (bytes)\r\n", Ehdr->e_shentsize);

    /**
     * Section header table entry count
     */
    TRACE("  Number of section headers:\t%d\r\n", Ehdr->e_shnum);

    /**
     * Section header string table index
     */
    TRACE("  Section header string table index:\t%d\r\n", Ehdr->e_shstrndx);

    return 0;
}

static NK_Int
Elf_section(NK_This) {

    /// 检测句柄异常。
    NK_EXPECT_VERBOSE_RETURN_VAL(NK_Nil != Public, -1);

    /// 获取私有句柄。
    DECLARE_PRIVATED();

    /// 数据源检查
    NK_EXPECT_VERBOSE_RETURN_VAL(NK_Nil != Privated->Src, -1);

    NKLOG(NK_Log, NKL_Alert, "ELF section begin");

    NK_Int i;
    Elf32_Ehdr *Ehdr = (Elf32_Ehdr *)(Privated->Src);
    Elf32_Shdr *Shdr = (Elf32_Shdr *)(Privated->Src + Ehdr->e_shoff);

    TRACE("There are %d section headers, starting at offset 0x%x\n\n", Ehdr->e_shnum, Ehdr->e_shoff);
    TRACE("Section Headers:\n");
    TRACE("  [Nr] Name              Type            Addr     Off      Size     ES       Flg Lk       Inf      Al       \n");

    for (i = 0; i < Ehdr->e_shnum; i++) {

        /// 从"段表字符串表"找出段名
        NK_Char *Name = (NK_Char *)(Privated->Src + Shdr[Ehdr->e_shstrndx].sh_offset + Shdr[i].sh_name);

        NK_Char Type[16] = {""};
        switch (Shdr[i].sh_type)
        {
        case SHT_NULL:
            snprintf(Type, sizeof(Type), "NULL"); break;
        case SHT_PROGBITS:
            snprintf(Type, sizeof(Type), "PROGBITS"); break;
        case SHT_SYMTAB:
            snprintf(Type, sizeof(Type), "SYMTAB"); break;
        case SHT_STRTAB:
            snprintf(Type, sizeof(Type), "STRTAB"); break;
        case SHT_RELA:
            snprintf(Type, sizeof(Type), "RELA"); break;
        case SHT_HASH:
            snprintf(Type, sizeof(Type), "HASH"); break;
        case SHT_DYNAMIC:
            snprintf(Type, sizeof(Type), "DYNAMIC"); break;
        case SHT_NOTE:
            snprintf(Type, sizeof(Type), "NOTE"); break;
        case SHT_NOBITS:
            snprintf(Type, sizeof(Type), "NOBITS"); break;
        case SHT_REL:
            snprintf(Type, sizeof(Type), "REL"); break;
        case SHT_SHLIB:
            snprintf(Type, sizeof(Type), "SHLIB"); break;
        case SHT_DYNSYM:
            snprintf(Type, sizeof(Type), "DYNSYM"); break;
        case SHT_INIT_ARRAY:
            snprintf(Type, sizeof(Type), "INIT_ARRAY"); break;
        case SHT_FINI_ARRAY:
            snprintf(Type, sizeof(Type), "FINI_ARRAY"); break;
        case SHT_PREINIT_ARRAY:
            snprintf(Type, sizeof(Type), "PREINIT_ARRAY"); break;
        case SHT_GROUP:
            snprintf(Type, sizeof(Type), "GROUP"); break;
        case SHT_SYMTAB_SHNDX:
            snprintf(Type, sizeof(Type), "SYMTAB_SHNDX"); break;
        default:
            break;
        }

        NK_Char Flags[16] = {""};
        if (Shdr[i].sh_flags & SHF_WRITE)               strcat(Flags, "W");
        if (Shdr[i].sh_flags & SHF_ALLOC)               strcat(Flags, "A");
        if (Shdr[i].sh_flags & SHF_EXECINSTR)           strcat(Flags, "X");
        if (Shdr[i].sh_flags & SHF_MERGE)               strcat(Flags, "M");
        if (Shdr[i].sh_flags & SHF_STRINGS)             strcat(Flags, "S");
        if (Shdr[i].sh_flags & SHF_INFO_LINK)           strcat(Flags, "I");
        if (Shdr[i].sh_flags & SHF_LINK_ORDER)          strcat(Flags, "L");
        if (Shdr[i].sh_flags & SHF_OS_NONCONFORMING)    strcat(Flags, "O");
        if (Shdr[i].sh_flags & SHF_GROUP)               strcat(Flags, "G");
        if (Shdr[i].sh_flags & SHF_TLS)                 strcat(Flags, "T");
        if (Shdr[i].sh_flags & SHF_COMPRESSED)          strcat(Flags, "C");

        TRACE("  [%2d] %-17s %-15s %08x %08x %08x %08x %-3s %08x %08x %08x\n"
            , i, Name, Type, Shdr[i].sh_addr, Shdr[i].sh_offset, Shdr[i].sh_size, Shdr[i].sh_entsize
            , Flags, Shdr[i].sh_link, Shdr[i].sh_info, Shdr[i].sh_addralign);
    }

    TRACE("Key to Flags:\n");
    TRACE("  W (write), A (alloc), X (execute), M (merge), S (strings), I (info),\n");
    TRACE("  L (link order), O (extra OS processing required), G (group), T (TLS), C (compressed)\n");

    return 0;
}

static NK_Int
Elf_symtab(NK_This) {

    /// 检测句柄异常。
    NK_EXPECT_VERBOSE_RETURN_VAL(NK_Nil != Public, -1);

    /// 获取私有句柄。
    DECLARE_PRIVATED();

    /// 数据源检查
    NK_EXPECT_VERBOSE_RETURN_VAL(NK_Nil != Privated->Src, -1);

    NKLOG(NK_Log, NKL_Alert, "ELF symtab begin");

    NK_Int i;
    NK_Int ii;
    Elf32_Ehdr *Ehdr = (Elf32_Ehdr *)(Privated->Src);
    Elf32_Shdr *Shdr = (Elf32_Shdr *)(Privated->Src + Ehdr->e_shoff);

    for (i = 0; i < Ehdr->e_shnum; i++) {

        /// 不是符号表
        if (SHT_SYMTAB != Shdr[i].sh_type && SHT_DYNSYM != Shdr[i].sh_type) {
            continue;
        }

        /// 从"段表字符串表"找出段名
        NK_Char *Name = (NK_Char *)(Privated->Src + Shdr[Ehdr->e_shstrndx].sh_offset + Shdr[i].sh_name);

        /// 获取符号数
        NK_Int Cnt = Shdr[i].sh_size / Shdr[i].sh_entsize;

        TRACE("Symbol table '%s' contains %d entries:\n", Name, Cnt);
        TRACE("  [  Nr] Value    Size     Type     Bind     Vis       Ndx  Name\n");

        /// 获取符号表
        Elf32_Sym *Sym = (Elf32_Sym *)(Privated->Src + Shdr[i].sh_offset);

        for (ii = 0; ii < Cnt; ii++) {

            NK_Char Type[16] = {""};
            switch (ELF32_ST_TYPE(Sym[ii].st_info))
            {
            case STT_NOTYPE:
                snprintf(Type, sizeof(Type), "NOTYPE"); break;
            case STT_OBJECT:
                snprintf(Type, sizeof(Type), "OBJECT"); break;
            case STT_FUNC:
                snprintf(Type, sizeof(Type), "FUNC"); break;
            case STT_SECTION:
                snprintf(Type, sizeof(Type), "SECTION"); break;
            case STT_FILE:
                snprintf(Type, sizeof(Type), "FILE"); break;
            case STT_COMMON:
                snprintf(Type, sizeof(Type), "COMMON"); break;
            case STT_TLS:
                snprintf(Type, sizeof(Type), "TLS"); break;
            default:
                break;
            }

            NK_Char Bind[16] = {""};
            switch (ELF32_ST_BIND(Sym[ii].st_info))
            {
            case STB_LOCAL:
                snprintf(Bind, sizeof(Bind), "LOCAL"); break;
            case STB_GLOBAL:
                snprintf(Bind, sizeof(Bind), "GLOBAL"); break;
            case STB_WEAK:
                snprintf(Bind, sizeof(Bind), "WEAK"); break;
            default:
                break;
            }

            NK_Char Vis[16] = {""};
            switch (ELF32_ST_BIND(Sym[ii].st_other))
            {
            case STV_DEFAULT:
                snprintf(Vis, sizeof(Vis), "DEFAULT"); break;
            case STV_INTERNAL:
                snprintf(Vis, sizeof(Vis), "INTERNAL"); break;
            case STV_HIDDEN:
                snprintf(Vis, sizeof(Vis), "HIDDEN"); break;
            case STV_PROTECTED:
                snprintf(Vis, sizeof(Vis), "PROTECTED"); break;
            default:
                break;
            }

            /// 从"段表字符串表"找出段名
            NK_Char *Name = (NK_Char *)(Privated->Src + Shdr[Ehdr->e_shstrndx].sh_offset + Shdr[i].sh_name);

            TRACE("  [%4d] %08x %-8d %-8s %-8s %-9s %4d %s\n", ii, Sym[ii].st_value, Sym[ii].st_size, Type, Bind, Vis, Sym[ii].st_shndx, Name);
        }
    }

    return 0;
}

/**
 * NK_Log 定义解除。
 */
#undef NK_Log

/**
 * NK_This 指针定义解除，\n
 * 定义以上为模块 API 接口实现。
 */
#undef NK_This

NK_Parser *
NK_Parse_Create(const NK_PChar elf) {

    NK_PrivatedParser *Privated = NK_Nil;
    NK_Parser *Public = NK_Nil;

    /**
     * 参数检测。
     */
    NK_EXPECT_VERBOSE_RETURN_VAL(NK_Nil != elf, NK_Nil);

    /**
     * 初始化句柄。
     * 公有句柄的内存空间仅靠私有句柄的高位。
     * 有效防止模块公有句柄在外部被意外释放。
     */
    Privated = malloc(sizeof(NK_PrivatedParser) + sizeof(NK_Parser));
    NK_EXPECT_VERBOSE_RETURN_VAL(NK_Nil != Privated, NK_Nil);

    Public = (NK_Parser *)(Privated + 1);

    memset(Privated, 0, sizeof(NK_PrivatedParser) + sizeof(NK_Parser));

    /// 初始化模块私有句柄。
    memcpy(Privated->Path, elf, strlen(elf));
    Privated->Path[strlen(elf)] = '\0';

    /// 初始化模块公有句柄。
    Public->parse   = Elf_parse;
    Public->header  = Elf_header;
    Public->section = Elf_section;
    Public->symtab  = Elf_symtab;

    /// 返回模块公有句柄。
    return Public;
}

NK_Int
NK_Parse_Free(NK_Parser **parser) {

    NK_PrivatedParser *Privated = NK_Nil;
    NK_Parser *Public = NK_Nil;

    /**
     * 参数检测。
     */
    NK_EXPECT_VERBOSE_RETURN_VAL(NK_Nil != parser, -1);
    NK_EXPECT_VERBOSE_RETURN_VAL(NK_Nil != parser[0], -1);

    Public = parser[0];
    Privated = PRIVATED(Public);

    parser[0] = NK_Nil;

    /// 释放私有数据。
    if (Privated->Src)
        free(Privated->Src);

    /// 销毁私有句柄。
    free(Privated);

    return 0;
}
