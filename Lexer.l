%%

[ \t\n]                                                                     { /* ignorar espacios */ }

":="                                                                        { return ASSIGN_SHORT; }
"=="                                                                        { return OP_EQ; }
"!="                                                                        { return OP_NEQ; }
"<="                                                                        { return OP_LEQ; }
">="                                                                        { return OP_GEQ; }
"||"                                                                        { return OP_OR; }
"&&"                                                                        { return OP_AND; }

"+"|"-"|"*"|"/"|"%"|";"|","|"("|")"|"{"|"}"|"<"|">"|"="|"!"|"&"             { return SINGLE_OP; }

[0-9]+                                                                      { return INT_LIT; }
\"([^\\\"]|\\.)*\"                                                          { return STRING_LIT; }
[a-zA-Z_][a-zA-Z0-9_]*                                                      { return IDENTIFIER; }

.                                                                           { return UNK; }

%%