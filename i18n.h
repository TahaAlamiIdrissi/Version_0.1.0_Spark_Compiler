// @auteur Karim Baïna, ENSIAS, Décembre 2018

#ifndef I18N_H
#define I18N_H

#define Line 43
#define MessageDeBienvenueC 44
#define MessageDeBienvenueI 45

#define LANGUAGES 4
#define DICTIONARY_CARDINAL (MessageDeBienvenueI+1)

typedef enum { English=0, French, Spanish, German } Langue;

void set_LANGUE_COURANTE(Langue l);
Langue get_LANGUE_COURANTE();

static char *dictionary[ DICTIONARY_CARDINAL ][ LANGUAGES ] =
{
	// IdfExpected=0 
        {
            "expected identificator", 		/* English */
            "identificateur attendu", 	/* French */
	    "identificador esperado", /* Spanish */
            "erwarteter identifikator" /* German */
        },

	// BeginExpected=1,
	{
            "expected \"begin\"",			/* English */
            "\"begin\" attendu",			/* French */
"esperado \"begin\"", /* Spanish */
"erwarteter \"begin\"" /* German */
        },

	//DoExpected=2,
	{
            "expected \"do\"",			/* English */
            "\"do\" attendu",			/* French */
"esperado \"do\"", /* Spanish */
"erwartet \"do\""/* German */
        },


	//InumberExpected=3,
	{
            "expected integer number",			/* English */
            "nombre entier attendu",			/* French */
	    "número entero esperado",/* Spanish */
"erwartete ganze Zahl"/* German */

        },

	//ToExpected=4, 
	{
            "expected \"to\"",			/* English */
            "\"to\" attendu",			/* French */
	"esperado \"to\"",/* Spanish */
"erwartet \"to\""/* German */

        },

	//EqExpected=5,
	{
            "expected '='",			/* English */
            "'=' attendu",			/* French */
"esperado '='",/* Spanish */
"erwartet '='"/* German */

        },

	//PvirgExpected=6,
	{
            "expected ';'",			/* English */
            "';' attendu",			/* French */
"esperado ';'",/* Spanish */
"erwartet ';'"/* German */

        },

	//EndforExpected=7,
	{
            "expected \"endfor\"",			/* English */
            "\"endfor\" attendu",			/* French */
"esperado \"endfor\"",/* Spanish */
"erwartetes \"endfor\""/* German */
        },

	//ThenExpected=8,
	{
            "expected \"then\"",			/* English */
            "\"then\" attendu",			/* French */
"esperado \"then\"",/* Spanish */
"erwartet \"then\""/* German */
        },
          
	//PcloseExpected=9,
	{
            "expected ')'",			/* English */
            "')' attendu",			/* French */
"esperado ')'",/* Spanish */
"erwartet ')'"/* German */
        },

	//EqeqExpected=10,
	{
            "expected \"==\"",			/* English */
            "\"==\" attendu",			/* French */
"esperado \"==\"",/* Spanish */
"erwartet \"==\""/* German */
        },

	//PopenExpected=11,
	{
            "expected '('",			/* English */
            "'(' attendu",			/* French */
"esperado '('",/* Spanish */
"erwartet '('"/* German */
        },
          

	//EndifExpected=12,
	{
            "expected \"endif\"",			/* English */
            "\"endif\" attendu",			/* French */
"esperado \"endif\"",/* Spanish */
"erwartet \"endif\""/* German */
        },
          

	//EndiforelseExpected=13,
	{
            "expected \"endif\" or \"else\"",			/* English */
            "\"endif\" ou \"else\" attendu",			/* French */
"esperado \"endif\" o \"else\"",/* Spanish */
"erwartet \"endif\" oder \"else\""/* German */
        },
          

	//TypeExpected=14,
	{
            "expected \"int\" or \"double\" or \"bool\" or \"string\"",			/* English */
            "\"int\" ou \"double\" ou \"bool\" ou \"string\" attendu",			/* French */
"esperado \"int\" o \"double\" o \"bool\" o \"string\"",/* Spanish */
"erwartet \"int\" oder \"double\" oder \"bool\" oder \"string\""/* German */
        },
          

	//ConstExpected=15,
	{
            "expected integer number or real number or \"true\" or \"false\"",			/* English */
            "nombre entier ou nombre réel ou \"true\" ou \"false\" attendu",			/* French */
"número entero esperado o número real o \"true\" o \"false\"",/* Spanish */
"erwartete ganze Zahl oder reelle Zahl oder \"true\" oder \"false\""/* German */
        },
          

	//AddorsubExpected=16,
	{
            "expected '+' or '-'",			/* English */
            "'+' ou '-' attendu",			/* French */
"esperado '+' o '-'",/* Spanish */
"erwartet '+' oder '-'"/* German */
        },
          

	//MultordivExpected=17,
	{
            "expected '*' or '/' or '+' or '-' or ')' or ';'",			/* English */
            "'*' ou '/' ou '+' ou '-' ou ')' ou ';' attendu",			/* French */
"esperado '*' o '/' o '+' o '-' o ')' o ';'",/* Spanish */
"erwartet '*' oder '/' oder '+' oder '-' oder ')' oder ';'"/* German */
        },
          

	//TrueorFalseExpected=18,
	{
            "expected \"true\" or \"false\"",			/* English */
            "\"true\" ou \"false\" attendu",			/* French */
"esperado \"true\" o \"false\"",/* Spanish */
"erwartet \"true\" oder \"false\"" /* German */
        },

	//IdforinumberordnumberorpopenExpected=19,
	{
            "expected identificator or integer number or real number or '('",			/* English */
            "identificateur ou nombre entier ou nombre réel ou '(' attendu",			/* French */
"identificador esperado o número entero o número real o '('",/* Spanish */
"erwarteter identificador oder ganze Zahl oder reelle Zahl oder '('"/* German */
        },
          

	//InstructionExpected=20,
	{
            "expected instruction",			/* English */
            "instruction attendue",			/* French */
"instrucción esperada",/* Spanish */
"erwarteter Unterricht"/* German */

        },
	  

	//EndExpected=21,
	{
            "expected \"end\"",			/* English */
            "\"end\" attendu",			/* French */
"esperado \"end\"",/* Spanish */
"erwartetes \"end\""/* German */
        },
	  
	//CaseExpected=22,
	{
            "expected \"case\"",			/* English */
            "\"case\" attendu",			/* French */
"esperado \"case\"",/* Spanish */
"erwartetes \"case\""/* German */
        },
	
	//EndswitchExpected=23,
	{
            "expected \"endswitch\"",			/* English */
            "\"endswitch\" attendu",			/* French */
"esperado \"endswitch\"",/* Spanish */
"erwartetes \"endswitch\""/* German */
        },

	//BreakExpected=24,
	{
            "expected \"break\"",			/* English */
            "\"break\" attendu",			/* French */
"esperado \"break\"",/* Spanish */
"erwartetes \"break\""/* German */
        },
	
	//DeuxpointExpected=25,
	{
            "expected \':\'",			/* English */
            "\':\' attendus",			/* French */
"esperado \':\'",/* Spanish */
"erwartetes \':\'"/* German */
        },
	

	//DefaultExpected=26,
{
            "expected \"default\"",			/* English */
            "\"default\" attendu",			/* French */
"esperado \"default\"",/* Spanish */
"erwartetes \"default\""/* German */
        },

//PragmaExpected=27
{
            "pragma expected",			/* English */
            "pragma attendu",			/* French */
"esperado pragma",/* Spanish */
"erwartet pragma" /* German */
        },
//CStringExpected=28
{
            "constant string expected",			/* English */
            "chaîne de caractères attendu",			/* French */
"esperado constante string",/* Spanish */
"erwartet konstant string" /* German */
        },
	//NonCategorized=29
	{
            "syntactic error encountred",			/* English */
            "erreur syntaxique rencontrée",			/* French */
"error sintáctico incluido",/* Spanish */
"syntaktischer Fehler"/* German */
        },


	//NotDeclared=30,
	{
            "variable non declared",			/* English */
            "variable non declarée",			/* French */
"variable no declarada",/* Spanish */
"variable nicht deklariert"/* German */
        },

	//AlreadyDeclared=31,
	{
            "variable already declared",			/* English */
            "variable déjà declarée",			/* French */
"variable ya declarada",/* Spanish */
"variable bereits deklariert"/* German */
        },

	//NotInitialized=32,
	{
            "variable used without initialization",			/* English */
            "variable utilisée sans initialisation",			/* French */
"variable utilizada sin inicialización",/* Spanish */
"variable ohne Initialisierung verwendet"/* German */
        },

	//BadlyInitialised=33,
	{
            "variable badly initialised",			/* English */
            "variable mal initialisée",			/* French */
"variable mal iniciada",/* Spanish */
"variable schlecht initialisiert"/* German */
        },

	//IncompatibleAssignType=34,
	{
            "incompatible with assignment value",			/* English */
            "incompatible avec la valeur d'affectation",			/* French */
"incompatible con el valor de asignación",/* Spanish */
"mit Zuordnungswert nicht kompatibel"/* German */
        },

	//IncompatibleCompType=35,
	{
            "incompatible with comparison value",			/* English */
            "incompatible avec la valeur de comparaison",			/* French */
"incompatible con el valor de comparación",/* Spanish */
"nicht kompatibel mit Vergleichswert"/* German */
        },

	//IncompatibleOperationType=36
	{
            "variable incompatible with operation",			/* English */
            "variable incompatible avec l'opération",			/* French */
"variable incompatible con la operación",/* Spanish */
"variable nicht kompatibel mit Operation"/* German */
        },
	//IncompatibleForIndexType=37
	{
            "variable with bad index type",			/* English */
            "variable avec mauvais type d'index",			/* French */
"variable incompatible con \"for\"",/* Spanish */
"variable nicht kompatibel mit \"for\""/* German */
        },
	//      IncompatibleForSwitch=38
	{
            "variable incompatible for \"switch\"",			/* English */
            "variable incompatible pour le \"switch\"",			/* French */
"variable incompatible con \"switch\"",/* Spanish */
"variable nicht kompatibel mit \"switch\""/* German */
        },
//MultipleValueForSwitch=39
	{
            "variable with duplicated value in a \"switch\"",			/* English */
            "variable avec valeur en double dans un \"switch\"",			/* French */
"variable con valor duplicado en una \"switch\"",/* Spanish */
"variable mit doppeltem Wert in a \"switch\""/* German */
        },
//	      DivisionbyZero=40
	{
            "Division by zero",			/* English */
            "Division par zéro",			/* French */
"División por cero",/* Spanish */
"Division durch null"/* German */
        },
//SyntacticError=41
	{
            "syntactic error",			/* English */
            "erreur syntaxique",			/* French */
"error sintáctico",/* Spanish */
"syntaktischer Fehler"/* German */
        },
	//SemanticError = 42
	{
            "semantic error",			/* English */
            "erreur sémantique",			/* French */
"error semántico",/* Spanish */
"semantischer fehler"/* German */
        },
	//Line=43
	{
            "line",			/* English */
            "ligne",			/* French */
	"línea",/* Spanish */
	"Linie"/* German */
        },
	//MessageDeBienvenueC 44
	{"Welcome to HortEnsias Language compiler", /* English */
	 "Bienvenue au Compilateur du langage HortEnsias", /*French*/
	 "Bienvenido al comilador del ligua HortEnsias", /*Spanish*/
	 "Wielcomen to compilador de sprakher HortEnsias" /*German*/
	},
	//MessageDeBienvenueI 45
	{"Welcome to HortEnsias Language interpreter", /* English */
	 "Bienvenue à l'Interpréteur du langage HortEnsias", /*French*/
	 "Bienvenido al intérprete del ligua HortEnsias", /*Spanish*/
	 "Wielcomen to Dolmetscher de sprakher HortEnsias" /*German*/
	}

};

#endif
