%%

[ \t\n]+                            { /* ignore whitespaces */ }
"//".*                              { /* ignore line comments */ }
"/*"([^*]|\*+[^*/])*"*/"            { /* ignore block comments */ }

[a-zA-Z_][a-zA-Z0-9_]*              { return IDENTIFIER; }
[0-9]+                              { return INT_LIT; }
"\""([^\\\"]|\\.)*"\""              { return STRING_LIT; }

"+"|"-"|"*"|"/"|"%"|";"|","|"("|")"|"{"|"}"|"<"|">"|"="|"&"|"["|"]"             { return SINGLE_OP; }

":="                                { return ASSIGN_SHORT; }
"=="                                { return OP_EQ; }
"!="                                { return OP_NEQ; }
"<="                                { return OP_LEQ; }
">="                                { return OP_GEQ; }
"||"                                { return OP_OR; }
"&&"                                { return OP_AND; }

.                                   { return UNK; }

%%
