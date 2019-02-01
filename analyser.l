%{
    #include "analyser.h"
    #define debug false
    static int my_yylineno = 1;
%}

%option noyywrap

IDF                   [a-zA-Z_][a-zA-Z0-9_]*
CHARACTER             [A-Za-z]
DIGIT                 [0-9]+
FLOATVARIABLE         DIGIT"."DIGIT
STRING                \"[^\"]*\"
%%

"Integer" 			{return INTEGER;}
"Boolean" 			{return BOOLEAN;}
"character"         {return CHARACTER;}
"constant"          {return CONSTANT;}
"STRING"            {return STRING;}
"Float" 			{return FLOAT;}
"Natural" 			{return NATURAL;}
"record" 			{return RECORD;}
"array" 			{return ARRAY;}
"with"				{return WITH;}
"is"				{return IS;}
"procedure"			{return PROCEDURE;}
"body"				{return BODY;}
"function"			{return FUNCTION;}
"new"				{return NEW;}
"type"				{return TYPE;}
"and"				{return AND;}
"not"				{return NOT;}
"private"			{return PRIVATE;}
"project"			{return PROJECT;}
"package"			{return PACKAGE;}
"pragma"			{return PRAGMA;}
"put"               {return PUT;}
"put_line"          {return PUT_LINE;}
"range"				{return RANGE;}
":="				{return AFFECTATION;}
"of"			    {return OF;}
"begin"				{return BEG;}
"end"				{return END;}
"if"			    {return IF;}
"then"				{return THEN;}
"else"				{return ELSE;}
"elsif"				{return ELSIF;}
"in out"			{return IN_OUT;}
"in"			    {return IN;}
"out"				{return OUT;}
"Input"				{return INPUT;}
"Output"			{return OUTPUT;}
"In_Out"			{return IN_OUT_;}
"Proof_In"			{return PROOF_IN;}
"raise"				{return RAISE;}
"generic"			{return GENERIC;}
"return"			{return RETURN;}
"others"			{return OTHERS;}
"subtype"			{return SUBTYPE;}
"mod"				{return MOD;}
"all"				{return ALL;}
"some"				{return SOME;}
"False"				{return FALSE;}
"True"				{return TRUE;}
"or"			    {return OR;}
"loop"				{return LOOP;}
"for"               {return FOR;}
"Positive"			{return POSITIVE;}
"when"[ ]+"others"  {return WHEN_OTHERS;}
"when"				{return WHEN;}
"while"				{return WHILE;}
"tagged"			{return TAGGED;}
"overriding"		{return OVERRIDING;}
"task"				{return TASK;}
"declare"			{return DECLARE;}
"protected"			{return PROTECTED;}
"entry"				{return ENTRY;}
"abs"				{return ABS;}
"Element"			{return ELEMENT;}
"Index"				{return INDEX;}
"xor"				{return XOR;}
"exit"				{return EXIT;}
"limited"			{return LIMITED;}
"access"			{return ACCESS;}
";"					{return SEMI_COLON;}
","					{return COMMA;}
":"					{return COLON;}
"("					{return POPEN;}
")"					{return PCLOSE;}
"=>"				{return ARROW;}
"."					{return POINT;}
">"					{return GREATER;}
"=" 				{return EQUAL;}
"/="                {return DIFFERENT;}
"-"					{return MINUS;}
"+"					{return PLUS;}
"<"					{return LESS;}
"<="                {return LESS_OR_EQ;}
">="                {return GREATER_OR_EQ;}
".."				{return INTERVAL;}
"'"					{return APOSTROPHE;}
"|"					{return ORINRANGE;}
"*"					{return MULT;}
"/"					{return DIV;}
"**"				{return POWER;}
"&"					{return AMPERSAND;}
'{CHARACTER}'       {return CHAR;}
{STRING}            {return CSTR;}
{IDF} 	            {
                        if (debug) printf ("IDF(%s)\n",yytext);
			            set_idf_attributes(yytext, my_yylineno);
			            return IDF;
                    }
{FLOATVARIABLE}     {
                        if (debug) printf ("NUMBER(%s)\n",yytext);
			            set_number_attributes(atof(yytext));
                        return FNUMBER;
                    }
{DIGIT}		        {
                        if (debug) printf ("NUMBER(%s)\n",yytext);
			            set_number_attributes((double)atoi(yytext));
			            return INUMBER;
                    }
[-][-][^\n]*[\n]     /* comments */
[ \t]		        /* empty characters */
\n                  {my_yylineno++; set_token_attributes( my_yylineno );}
.	        { printf( "ERRORLEX(%s)\n ", yytext );}
%%
