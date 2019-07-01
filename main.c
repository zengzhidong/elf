#include <parser.h>

int main(int argc, char **argv)
{
    NK_Parser *parser = NK_Parse_Create(argv[1]);
    parser->parse(parser);
    parser->symtab(parser);

    return NK_Parse_Free(&parser);
}
