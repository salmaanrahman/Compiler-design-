#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STATES 20
#define MAX_SYMBOLS 10

int nfa[MAX_STATES][MAX_SYMBOLS][MAX_STATES]; 
int nfa_states, nfa_symbols;
char symbols[MAX_SYMBOLS];


int dfa_states[100][MAX_SYMBOLS];
int dfa_final[100];
int dfa_state_count = 0;


int compare_states(int *a, int *b, int size) {
    for (int i = 0; i < size; i++)
        if (a[i] != b[i]) return 0;
    return 1;
}

int state_exists(int dfa[][MAX_STATES], int total, int *state) {
    for (int i = 0; i < total; i++) {
        if (compare_states(dfa[i], state, MAX_STATES))
            return i;
    }
    return -1;
}

void print_state(int *state) {
    printf("{ ");
    for (int i = 0; i < MAX_STATES; i++) {
        if (state[i]) printf("q%d ", i);
    }
    printf("}");
}

void nfa_to_dfa(int start_state, int final_state) {
    int dfa_states_list[100][MAX_STATES] = {0};
    int queue[100], front = 0, rear = 0;
    int visited[100] = {0};

    int start_set[MAX_STATES] = {0};
    start_set[start_state] = 1;

    memcpy(dfa_states_list[0], start_set, sizeof(start_set));
    queue[rear++] = 0;
    dfa_state_count = 1;

    while (front < rear) {
        int current = queue[front++];
        visited[current] = 1;

        for (int a = 0; a < nfa_symbols; a++) {
            int new_state[MAX_STATES] = {0};

            
            for (int i = 0; i < nfa_states; i++) {
                if (dfa_states_list[current][i]) {
                    for (int j = 0; j < MAX_STATES; j++) {
                        if (nfa[i][a][j]) {
                            new_state[j] = 1;
                        }
                    }
                }
            }

            int index = state_exists(dfa_states_list, dfa_state_count, new_state);
            if (index == -1) {
                memcpy(dfa_states_list[dfa_state_count], new_state, sizeof(new_state));
                index = dfa_state_count;
                queue[rear++] = dfa_state_count;
                dfa_state_count++;
            }

            dfa_states[current][a] = index;
        }
    }

    
    for (int i = 0; i < dfa_state_count; i++) {
        if (dfa_states_list[i][final_state]) {
            dfa_final[i] = 1;
        }
    }

   
    printf("\nDFA Transitions:\n");
    for (int i = 0; i < dfa_state_count; i++) {
        print_state(dfa_states_list[i]);
        for (int a = 0; a < nfa_symbols; a++) {
            printf(" --%c--> ", symbols[a]);
            print_state(dfa_states_list[dfa_states[i][a]]);
        }
        if (dfa_final[i]) printf(" [FINAL]");
        printf("\n");
    }
}

int main() {
    printf("Enter number of states in NFA: ");
    scanf("%d", &nfa_states);

    printf("Enter number of input symbols: ");
    scanf("%d", &nfa_symbols);

    printf("Enter the symbols (space separated): ");
    for (int i = 0; i < nfa_symbols; i++) {
        scanf(" %c", &symbols[i]);
    }

    printf("Enter transitions (state symbol target_state), type -1 to end:\n");
    while (1) {
        int from, to;
        char symbol;
        scanf("%d", &from);
        if (from == -1) break;
        scanf(" %c %d", &symbol, &to);
        for (int i = 0; i < nfa_symbols; i++) {
            if (symbols[i] == symbol) {
                nfa[from][i][to] = 1;
                break;
            }
        }
    }

    int start, final;
    printf("Enter start state: ");
    scanf("%d", &start);
    printf("Enter final (accepting) state: ");
    scanf("%d", &final);

    nfa_to_dfa(start, final);

    return 0;
}
