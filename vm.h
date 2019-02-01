#ifndef VM_H
#define VM_H

#include "pseudocode.h"

// @auteur Karim Baïna, ENSIAS, Décembre 2010, mis à jour Décembre 2018


// inintialise la machine abstraite
void inintialiser_machine_abstraite();

// visite une pseudo insctruction  avec deux modes (interpretationMode, optimisationMode)
// cas interpretationMode : effectue l'interprétation de la pseudo instruction
// case optimisationMode : simule l'exécution et effectue l'opération de la pseudo instruction avec les vraies valeur simulées en mémoire statique et en pile, effectue le marquage de la pseudo instruction, et ne fait pas d'affichage du résultat de la pseudo instruction
void visiter_pseudo_instruction(struct pseudoinstruction *ppi, char ** next_label_name, boolean optimisationMode);

// precondition pc <> NULL
// interprete un pseudocode
// le résultat est en tête de pile.
void interpreter_pseudo_code(pseudocode pc);

// precondition pc <> NULL
// precondition le pseudo-code se termine en un temps fini
// optimise un pseudocode en éliminant les dead paths
void optimiser_pseudo_code(pseudocode pc);

// precondition pc <> NULL
// visite un pseudocode selon deux modes (interpretationMode, optimisationMode)
// cas interpretationMode : effectue l'interprétation du pseudo code
// case optimisationMode : simule l'exécution et effectue les opérations avec les vraies valeur simulées en mémoire statique et en pile, effectue le marquage des pseudo instrutions visitees du pseudocode, et ne fait pas d'affichage des résultats
void visiter_pseudo_code(pseudocode pc, boolean optimiserMode);

#endif

