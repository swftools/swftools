#ifndef __parser_h__
#define __parser_h__

enum type_t {
    TWIP=0, 
    NUMBER=1,
    COMMAND=2,
    STRING=3,
    ASSIGNMENT=4,
    IDENTIFIER=5,
    LABEL=6,
    END=7
};

extern char*type_names[];

struct token_t {
    enum type_t type;
    char* text;
    int line;
    int column;
};

extern struct token_t* generateTokens(char*filename);
extern void freeTokens(struct token_t*);

#endif
