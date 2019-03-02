#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//used to simulate a clause as a set of literals
//where literals are ints
typedef struct int_ll
{
    int literal;
    struct int_ll* next_literal;
    struct int_ll* previous_literal;
}int_ll;

//used to simulate a set of clauses 
typedef struct clause_ll 
{
    struct int_ll* clause;
    struct clause_ll* previous_clause;
    struct clause_ll* next_clause;
}clause_ll;

//inserts a literal in a hiven clause
int_ll* insert_literal(int_ll* clause,int literal)
{
    int_ll* new_clause = (int_ll*)malloc(sizeof(int_ll));
    new_clause->literal = literal;  
    new_clause->next_literal = clause;
    new_clause->previous_literal = NULL;
    if(clause != NULL)
        clause->previous_literal = new_clause;
    return new_clause; 
}

//inserts a clause in a clauses set
clause_ll* insert_clause(clause_ll* clauses_set, int_ll* clause)
{
    clause_ll* new_clause_set = (clause_ll*)malloc(sizeof(clause_ll));
    new_clause_set->clause = clause;
    new_clause_set->next_clause = clauses_set;
    new_clause_set->previous_clause = NULL;
    if(clauses_set != NULL)
        clauses_set->previous_clause = new_clause_set;
    return new_clause_set;
}

//malloc a clone of the clause given
int_ll* copy_of_clause(int_ll* clause)
{
    int_ll* copy = (int_ll*)malloc(sizeof(int_ll));
    copy->previous_literal = NULL;
    int_ll* start_copy = copy;
    while(clause != NULL)
    {
        if(clause->next_literal != NULL)
        {
            copy->next_literal = (int_ll*)malloc(sizeof(int_ll));
            copy->next_literal->previous_literal = copy;
        }
        else
            copy->next_literal = NULL;
        copy->literal = clause->literal;
        copy = copy->next_literal;
        clause = clause->next_literal;
    }
    copy = start_copy;
    return copy; 
}

//malloc a clone of the clauses set
clause_ll* copy_of_clauses_set(clause_ll* clauses_set)
{
    clause_ll* copy = (clause_ll*)malloc(sizeof(clause_ll));
    copy->previous_clause = NULL;
    clause_ll* start_copy = copy;
    while(clauses_set != NULL)
    {
        if(clauses_set->next_clause != NULL)
        {
            copy->next_clause = (clause_ll*)malloc(sizeof(clause_ll));
            copy->next_clause->previous_clause = copy;
        }
        else
            copy->next_clause = NULL;
        copy->clause = copy_of_clause(clauses_set->clause);
        copy = copy->next_clause;
        clauses_set = clauses_set->next_clause;
    }
    copy = start_copy;
    return copy; 
}

//developer flag, helps with debuging 
#ifdef DEVELOPER
//print a given clause to stdout
    void print_clause(int_ll* clause)
    {
        int_ll* start_clause = clause;
        if(clause == NULL)
        {
            printf("satisfied clause\n");
            return;
        }
        if(clause->previous_literal == NULL)
            printf("NULL ");
        else
            printf("NO_START_CLAUSE ");
        if(clause->literal == 0  && clause->next_literal == NULL)
        {
                printf("empty clause");
                if(clause->next_literal == NULL)
                    printf(" NULL\n");
                else
                    printf(" NO_END_CLAUSE\n");
                return;
        }
        while(clause != NULL)
        {       
            printf("%d ",clause->literal);
            clause = clause->next_literal;
        }
        printf(" NULL");
        printf("\n");
        clause = start_clause;
    }
//print a given clauses set to stdout
    void print_clauses_set(clause_ll* clauses_set)
    {
        clause_ll* start_clauses_set = clauses_set;
        if(clauses_set == NULL)
        {
            printf("Empty set\n\n");
            return;
        }
        if(clauses_set->previous_clause == NULL)
            printf("NULL\n");
        else
            printf("NO_START_SET\n");
        while(clauses_set != NULL)
        {
            print_clause(clauses_set->clause);
            clauses_set = clauses_set->next_clause;
        }
        printf("NULL\n\n");
        clauses_set = start_clauses_set;
    }

#else

    void print_clause(int_ll* clause)
    {
        int_ll* start_clause = clause;
        if(clause == NULL)
        {
            printf("satisfied clause\n");
            return;
        }
        if(clause->literal == 0 && clause->next_literal == NULL)
        {
                printf("empty clause\n");
                return;
        }
        while(clause != NULL)
        {       
            printf("%d ",clause->literal);
            clause = clause->next_literal;
        }
        printf("\n");
        clause = start_clause;
    }

    void print_clauses_set(clause_ll* clauses_set)
    {
        clause_ll* start_clauses_set = clauses_set;
        if(clauses_set == NULL)
        {
            printf("Empty set\n\n");
            return;
        }
        while(clauses_set != NULL)
        {
            print_clause(clauses_set->clause);
            clauses_set = clauses_set->next_clause;
        }
        printf("\n\n");
        clauses_set = start_clauses_set;
    }
#endif
//frees a given clause that was previously malloc'd
void free_clause(int_ll* clause)
{
    int_ll* temp;
    while(clause != NULL)
    {
        temp = clause->next_literal;
        free(clause);
        clause = temp;
    }
    return;
}

//frees a given clauses set that was previously malloc'd
void free_clauses_set(clause_ll* clauses_set)
{
    clause_ll* temp;
    while(clauses_set != NULL)
    {
        free_clause(clauses_set->clause);
        temp = clauses_set->next_clause;
        free(clauses_set);
        clauses_set = temp;
    }
    return;
}

// given a clause and a literal this will return the result of propagating
//literal on clause
int_ll* propagate_literal_on_clause(int literal,int_ll* clause)
{
    int_ll* start_clause = clause;
    int clauseChanged = 0;
    while(clause != NULL)
    {
        clauseChanged = 0;
        if(literal == clause->literal)
        {
            clause = start_clause;
            free_clause(clause);
            return NULL;
        }
        else if( -literal == clause->literal )
        {

            if(clause->next_literal == NULL && 
                clause->previous_literal == NULL)
            {
                clause->literal = 0;
                return clause;
            }

            if (clause->next_literal != NULL && 
                clause->previous_literal == NULL)
            {
                clause->next_literal->previous_literal = NULL;
                start_clause = clause->next_literal;
                free(clause);
                clause = start_clause;
                clauseChanged = 1;
            }
            else if(clause->next_literal == NULL && 
                    clause->previous_literal != NULL)
            {
                clause->previous_literal->next_literal = NULL;
                free(clause);
                clause = NULL;
                clauseChanged = 1;
            }
            else
            {
                clause->next_literal->previous_literal = clause->previous_literal;
                clause->previous_literal->next_literal = clause->next_literal;
                int_ll* temp = clause->next_literal;
                free(clause);
                clause = temp;
                clauseChanged = 1;
            }

        }
        if(!clauseChanged)
            clause = clause->next_literal;
    }
    clause = start_clause;
    return clause;
}

//propagates literal over the clauses set
clause_ll* propagate_literal(int literal,clause_ll* clauses_set)
{
    clause_ll* start_clauses_set = clauses_set;
    int_ll* new_clause;
    while(clauses_set != NULL)
    {
        clauses_set->clause = propagate_literal_on_clause(literal,
                                                clauses_set->clause);
        
        if (clauses_set->clause == NULL)
        {
            if(clauses_set->next_clause == NULL && \
           clauses_set->previous_clause == NULL)
            {
                free(clauses_set);
                return NULL;
            }
            if (clauses_set->next_clause != NULL && 
                clauses_set->previous_clause == NULL)
            {
                clauses_set->next_clause->previous_clause = NULL;
                clause_ll* temp = clauses_set->next_clause;
                free(clauses_set);
                clauses_set = temp;
                start_clauses_set = clauses_set;
                
            }
            else if (clauses_set->next_clause == NULL && 
                clauses_set->previous_clause != NULL)
            {
                clauses_set->previous_clause->next_clause = NULL;
                free(clauses_set);
                clauses_set = NULL;
            }
            else
            {
                clauses_set->next_clause->previous_clause = \
                                        clauses_set->previous_clause;
                clauses_set->previous_clause->next_clause = \
                                        clauses_set->next_clause;
                clause_ll* temp = clauses_set->next_clause;
                free(clauses_set);
                clauses_set = temp;
            }
        }
        else if(clauses_set->clause->literal == 0)
        {
            clauses_set = start_clauses_set;
            free_clauses_set(clauses_set);
            clauses_set = (clause_ll*)malloc(sizeof(clause_ll));
            int_ll* empty_clause = (int_ll*)malloc(sizeof(int_ll));
            empty_clause->literal = 0;
            empty_clause->next_literal = NULL;
            empty_clause->previous_literal = NULL;
            clauses_set->clause = empty_clause;
            clauses_set->next_clause = NULL;
            clauses_set->previous_clause = NULL;
            return clauses_set;
        }
        else        
            clauses_set = clauses_set->next_clause;
    }
    clauses_set = start_clauses_set;
    return clauses_set;
}

//performs unit propagation as many times as possible on the clauses set
clause_ll* unit_propagation(clause_ll* clauses_set)
{
    int propagated = 0;
    clause_ll* start_clauses_set = clauses_set;
        while(clauses_set != NULL)
        {
            propagated = 0;
            if(clauses_set->clause->next_literal == NULL && 
                clauses_set->clause->literal != 0)
            {
                int removed_literal = clauses_set->clause->literal;
                start_clauses_set = propagate_literal(removed_literal,start_clauses_set);
                clauses_set = start_clauses_set;
                #ifdef SHOW_PROCESS
                    printf("removed literal: %d\n",removed_literal );
                    print_clauses_set(clauses_set);
                #endif
                propagated = 1;
            }
            if(!propagated)
                clauses_set = clauses_set->next_clause;
        }
    clauses_set = start_clauses_set;
    return clauses_set;
}

//uses the DPLL algorithm to determine the satifiability of the clauses set
char* dpll(clause_ll** clauses_set)
{
    #ifdef DEVELOPER
    printf("first elem %p pointer address %p\n",*clauses_set,clauses_set );
    #endif
    #ifdef SHOW_PROCESS
        print_clauses_set(*clauses_set);
    #endif
    *clauses_set = unit_propagation(*clauses_set);
    if(*clauses_set == NULL)
        return "SATISFIABLE";
    if((*clauses_set)->clause->literal == 0)
    {
        free_clauses_set(*clauses_set);
        return "UNSATISFIABLE";
    }
    else
    {
        int removed_literal = (*clauses_set)->clause->literal;

        clause_ll** pos_copy_ptr = &(*clauses_set);
        clause_ll* neg_copy = copy_of_clauses_set(*clauses_set);
        clause_ll** neg_copy_ptr = &neg_copy;
        *pos_copy_ptr = propagate_literal(removed_literal,*pos_copy_ptr);
        *neg_copy_ptr = propagate_literal(-removed_literal,*neg_copy_ptr);
        #ifdef SHOW_PROCESS
        printf("On branch: %d\n",removed_literal );
        #endif
        char* pos_clauses_set = dpll(pos_copy_ptr);
        #ifdef SHOW_PROCESS
        printf("On branch: %d\n",-removed_literal );
        #endif
        char* neg_clauses_set = dpll(neg_copy_ptr);
        if(strcmp(pos_clauses_set,"SATISFIABLE") == 0 || 
            strcmp(neg_clauses_set,"SATISFIABLE") == 0 )
            return "SATISFIABLE";
        else
            return "UNSATISFIABLE";
    }

}

int main(int argc, char const *argv[])
{
    //char const* const fileName = argv[1];
    //FILE* file = fopen(fileName, "r"); 
    char line[256];
    clause_ll* clauses_set = NULL;

    while(fgets(line, sizeof(line), stdin) != NULL )
    {
        if(line[0] == 'p' || line[0] == 'c');
        //do nothing
        else
        {
            int_ll* new_clause = NULL;
            char *nextInt = strtok(line," ");
            while(nextInt != NULL)
            {
                if(atoi(nextInt) != 0)
                {
                    new_clause = insert_literal(new_clause,atoi(nextInt));
                }
                nextInt =  strtok(NULL," ");
            }
            clauses_set = insert_clause(clauses_set,new_clause);
        }
    }
    printf("%s\n",dpll(&clauses_set) );
    //fclose(file);
    return 0;
}
