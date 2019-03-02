A DPLL SAT solver made in C with some optimisations.

Make it with the command DPLL for max performance and DPLLV
for a walktrough of the solution.

You can run it and provide input from standard input accordind to the format below, or redirect input from a file (e.g. ./dpll < file)

I provided a selection of sets of clauses to test the solver on.

It solves hard* problems anywhere between 1m to 3h,
but it's reliably fast for medium or easy ones.

*hard problems where randomly generated with the ratio of clauses to variables around the corssover point (4.25).

Format of set clauses:

c comment line

c this is a test problem

p cnf 3 4

1 0

-1 2 0

-1 -2 3 0

-2 -3 0

  Where lines starting with  "c" are comment lines.
  Line "p cnf 3 4" 
  indicates that there are 3 variables and 4 clauses in the problem.
  Literals are represented by numbers 1 is positive literal -1 negative literal   
  clauses are written as a sequence of literals separated by spaces, 
  each clause is written on one line terminated by "0".
