#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_TERMINALS 10
#define MAX_NON_TERMINALS 10
#define MAX_PRODUCTIONS 10
#define MAX_STRING_LEN 100

// Define terminals, non-terminals, productions, and their counts
int no_of_terminals;
char terminals[MAX_TERMINALS][MAX_STRING_LEN];

int no_of_non_terminals;
char non_terminals[MAX_NON_TERMINALS][MAX_STRING_LEN];

char starting_symbol[MAX_STRING_LEN];

int no_of_productions;
char productions[MAX_PRODUCTIONS][MAX_STRING_LEN];

typedef struct {
    char non_terminal[MAX_STRING_LEN];
    char alternatives[MAX_PRODUCTIONS][MAX_STRING_LEN];
    int alt_count;
} Production;

Production productions_dict[MAX_NON_TERMINALS];

typedef struct {
    char set[MAX_STRING_LEN][MAX_STRING_LEN];
    int count;
} Set;

Set FIRST[MAX_NON_TERMINALS];
Set FOLLOW[MAX_NON_TERMINALS];

// Function to check if a string exists in a set
int exists_in_set(Set *set, char *str) {
    for (int i = 0; i < set->count; i++) {
        if (strcmp(set->set[i], str) == 0) {
            return 1;
        }
    }
    return 0;
}

// Function to add a string to a set
void add_to_set(Set *set, char *str) {
    if (!exists_in_set(set, str)) {
        strcpy(set->set[set->count], str);
        set->count++;
    }
}

// Function to perform the FIRST operation
void first(char *string, Set *result) {
    if (strlen(string) == 0 || strcmp(string, "@") == 0) {
        add_to_set(result, "@");
        return;
    }

    for (int i = 0; i < no_of_terminals; i++) {
        if (strcmp(string, terminals[i]) == 0) {
            add_to_set(result, string);
            return;
        }
    }

    for (int i = 0; i < no_of_non_terminals; i++) {
        if (strcmp(string, non_terminals[i]) == 0) {
            for (int j = 0; j < productions_dict[i].alt_count; j++) {
                first(productions_dict[i].alternatives[j], result);
            }
            return;
        }
    }

    char first_char[2] = {string[0], '\0'};
    Set temp_set = {0};

    first(first_char, &temp_set);

    if (exists_in_set(&temp_set, "@")) {
        int i = 1;
        while (exists_in_set(&temp_set, "@") && string[i] != '\0') {
            char next_char[2] = {string[i], '\0'};
            first(next_char, &temp_set);
            i++;
        }
    }

    for (int i = 0; i < temp_set.count; i++) {
        add_to_set(result, temp_set.set[i]);
    }
}

// Function to perform the FOLLOW operation
void follow(char *nT, Set *result) {
    if (strcmp(nT, starting_symbol) == 0) {
        add_to_set(result, "$");
    }

    for (int i = 0; i < no_of_non_terminals; i++) {
        for (int j = 0; j < productions_dict[i].alt_count; j++) {
            char *alt = productions_dict[i].alternatives[j];
            char *pos = strstr(alt, nT);
            if (pos) {
                char following_str[MAX_STRING_LEN];
                strcpy(following_str, pos + strlen(nT));

                if (strlen(following_str) == 0) {
                    if (strcmp(productions_dict[i].non_terminal, nT) != 0) {
                        follow(productions_dict[i].non_terminal, result);
                    }
                } else {
                    Set temp_set = {0};
                    first(following_str, &temp_set);
                    if (exists_in_set(&temp_set, "@")) {
                        add_to_set(result, "@");
                        follow(productions_dict[i].non_terminal, result);
                    }
                    for (int k = 0; k < temp_set.count; k++) {
                        add_to_set(result, temp_set.set[k]);
                    }
                }
            }
        }
    }
}

int main() {
    // Input terminals
    printf("Enter number of terminals: ");
    scanf("%d", &no_of_terminals);
    printf("Enter the terminals:\n");
    for (int i = 0; i < no_of_terminals; i++) {
        scanf("%s", terminals[i]);
    }

    // Input non-terminals
    printf("Enter number of non-terminals: ");
    scanf("%d", &no_of_non_terminals);
    printf("Enter the non-terminals:\n");
    for (int i = 0; i < no_of_non_terminals; i++) {
        scanf("%s", non_terminals[i]);
    }

    // Input starting symbol
    printf("Enter the starting symbol: ");
    scanf("%s", starting_symbol);

    // Input productions
    printf("Enter number of productions: ");
    scanf("%d", &no_of_productions);
    printf("Enter the productions (in the format NonTerminal->Production1/Production2/...):\n");
    for (int i = 0; i < no_of_productions; i++) {
        scanf(" %[^\n]", productions[i]);
    }

    // Initialize production dictionary
    for (int i = 0; i < no_of_non_terminals; i++) {
        strcpy(productions_dict[i].non_terminal, non_terminals[i]);
        productions_dict[i].alt_count = 0;
    }

    // Populate production dictionary
    for (int i = 0; i < no_of_productions; i++) {
        char nonterm_to_prod[MAX_STRING_LEN];
        strcpy(nonterm_to_prod, productions[i]);
        char *non_terminal = strtok(nonterm_to_prod, "->");
        char *alternatives = strtok(NULL, "->");
        int index = 0;
        for (int j = 0; j < no_of_non_terminals; j++) {
            if (strcmp(non_terminals[j], non_terminal) == 0) {
                index = j;
                break;
            }
        }
        char *alternative = strtok(alternatives, "/");
        while (alternative) {
            strcpy(productions_dict[index].alternatives[productions_dict[index].alt_count], alternative);
            productions_dict[index].alt_count++;
            alternative = strtok(NULL, "/");
        }
    }

    // Compute FIRST and FOLLOW sets
    for (int i = 0; i < no_of_non_terminals; i++) {
        first(non_terminals[i], &FIRST[i]);
    }

    for (int i = 0; i < no_of_non_terminals; i++) {
        follow(non_terminals[i], &FOLLOW[i]);
    }

    // Print FIRST and FOLLOW sets
    printf("%-20s%-20s%-20s\n", "Non Terminals", "First", "Follow");
    for (int i = 0; i < no_of_non_terminals; i++) {
        printf("%-20s", non_terminals[i]);

        printf("{ ");
        for (int j = 0; j < FIRST[i].count; j++) {
            printf("%s ", FIRST[i].set[j]);
        }
        printf("}\t");

        printf("{ ");
        for (int j = 0; j < FOLLOW[i].count; j++) {
            printf("%s ", FOLLOW[i].set[j]);
        }
        printf("}\n");
    }

    return 0;
}

