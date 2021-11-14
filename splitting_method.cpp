#include <stdio.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string.h>
#include <vector>
#include <set>

using namespace std;

class Problem {
    public:
      // matrix in sparse row format
      vector<int>* values = NULL; // [m]
      int * idx = NULL; // [m+1]
      int m; // number of clauses
      int n; // number of propositions . 
      // this is dim of the matrix d1 x d2

      // meta information
      int * clause_evaluations; // this has the information of clause evaluation
      int * active_counts; // this has active counts of the propositions in each clause
      int * proposed_proposition;
      int * truth_assignment;
      vector<int> * unassigned_propositions;

      Problem(vector<int> * _values, int * _idx, int _n, int _m) : values(_values), idx(_idx), m(_m), n(_n){ 
          clause_evaluations = new int[m];
          active_counts = new int[m];
          proposed_proposition = new int[m];
          truth_assignment = new int[n+1];
          unassigned_propositions = new vector<int>();
          for ( int i=1 ; i < n + 1; i++ ) {
              truth_assignment[i] = -2; // unassigned
          }
          for ( int i=0 ; i < m; i++ ) {
              active_counts[i] = idx[i+1] - idx[i];
              proposed_proposition[i] = 1;
              clause_evaluations[i] = -1; // incomplete evaluation
          }

          for ( int i=0; i < m; i++) {
                  for(int j =idx[i]; j < idx[i+1]; j++) {
                          int lt = (*values)[j];
                          int p = lt > 0 ? lt : -lt;
                          truth_assignment[p] = -1;
                  }
          }

          for(int i=1; i < n+1; i++) {
                  if ( truth_assignment[i] == -1)
                          unassigned_propositions->push_back(i);
          }
      }

      ~Problem() {
          delete clause_evaluations;
          delete active_counts;
          delete proposed_proposition;
          delete truth_assignment;
          delete unassigned_propositions;
 
     }
      void print_ta() {
              printf("Truth Assignments : ");
              for (int i=1; i < n+1;i++) {
                      printf(" %d:%d", i, truth_assignment[i]);
              }
              printf("\n");
      }


      void print_problem() {
          printf("Solving the problem m=%d n=%d \n", m, n);
          for ( int i=0; i < m ; i++) {
              printf("clause: %d active: %d proposed: %d || ", clause_evaluations[i], active_counts[i], proposed_proposition[i]);
              for ( int j=0; j < idx[i+1] - idx[i] ; j++ ) {
                  printf("%d ", (*values)[idx[i] + j]);
              }
              printf("\n");
          }
          printf("Truth Assignments\n");
          for (int i=1; i < n+1;i++) {
              printf(" %d", truth_assignment[i]);
          }
          printf("\n");

          printf("Unassigned propositions %ld || ", unassigned_propositions->size());
          for (int i=0; i < unassigned_propositions->size(); i++) {
              printf(" %d", (*unassigned_propositions)[i]);
          }
          printf("\n");
      }

      void solve() {

          bool res = dpll();
          if ( res) {
                  printf("SAT\n");
                  printf("0 : false, 1: true, -1: don't care \n");
                  for(int i=1; i < n+1; i++) {
                          if(truth_assignment[i] != -2)
                              printf(" %d %d \n", i, truth_assignment[i]);
                  }
          } else  {
              printf("UNSAT\n");
          }
      }

      int evaluate_cat() {
        // repopulate clause_evaluations and active_counts w.r.t new current truth assignment
        int incomplete = 0;
        int positive = 0;
        int negative = 0;
        unassigned_propositions->clear();
        
        for(int i=1; i < n + 1; i++) {
            if ( truth_assignment[i] == -1)
                unassigned_propositions->push_back(i);
        }
        

        for ( int i=0; i < m; i++) {
            bool value = false;
            bool unassigned = false;
            active_counts[i] = 0;
            clause_evaluations[i] = -1;

            for(int j =idx[i]; j < idx[i+1]; j++) {
                int lt = (*values)[j];
                int p = lt > 0 ? lt : -lt;
                if ( truth_assignment[p] == -1) {
                    unassigned = true;
                    active_counts[i] ++;
                    proposed_proposition[i] = lt;
                } else {
                    if (((truth_assignment[p]==1) and lt > 0) or ((truth_assignment[p]==0) and (lt < 0))) {
                        value = true;
                        break;
                    }
                }
            }
            if (value) {
                clause_evaluations[i] = 1;
                active_counts[i] = 0;
                positive ++;
            } else {
                if (active_counts[i] == 0) {
                    clause_evaluations[i] = 0;
                    //printf("clause failed %d\n", i);
                    negative ++;
                } else {
                    incomplete ++;
                }
            }
        }
        //printf("pos: %d, neg:%d, inc:%d m:%d\n",positive, negative, incomplete, m);
        if (negative > 0) {
          return 0;
        } else if ( incomplete > 0) {
          return -1;
        } else {
          return 1;
        }
      }

      bool dpll() {
          //print_ta();
          //char c;
          int eval = evaluate_cat();
          if (eval == 0) {
              return false;
          } else if (eval == 1) {
              return true;
          }

          //print_problem();
          // UNIT PREFERENCE         
          int lt,p;
          int truth;
          vector <int> propositions_set ;
          bool allok = true;
          bool changed = true;
          while(changed) {
                  changed = false;
                  for (int i=0; i < m; i++ ){
                          if (active_counts[i] == 1) {
                                  lt = proposed_proposition[i];
                                  p = (lt < 0) ? -lt: lt;
                                  truth = (lt > 0) ? 1 : 0;

                                  //printf("single : setting %d for %d in accordance with clause %d \n", truth, p, i); 
                                  if ( truth_assignment[p] != -1 and truth_assignment[p] != truth) {
                                          //printf("Issue in assignment 1 : %d \n", p);
                                          allok = false;
                                          break;
                                  }
                                  truth_assignment[p] = truth;
                                  propositions_set.push_back(p);
                                  changed = true;
                          }
                  }
                  if( not allok) {
                      break;
                  }
                  if (changed) {
                          eval = evaluate_cat();
                          if (eval == 0) {
                                  return false;
                          } else if (eval == 1) {
                                  return true;
                          }
                  }
          }
          if ( not allok ) {
              // reset the propositions set and return
              for(int i = 0 ; i < propositions_set.size(); i++) {
                  truth_assignment[propositions_set[i]] = -1;
              }
              return false;
          }
          
          eval = evaluate_cat();
          if (eval == 0) {
              return false;
          } else if (eval == 1) {
              return true;
          }

          //print_problem(); scanf("%c", &c);
          
          int prop = (*unassigned_propositions)[0];
          truth_assignment[prop] = 1;
          bool val;
          val = dpll();
          if (val) {
            return true;
          }

          truth_assignment[prop] = 0;
          val = dpll();
          if (val) {
            return true;
          }
        
          // reset everything
          truth_assignment[prop] = -1;
          for(int i = 0 ; i < propositions_set.size(); i++) {
                  truth_assignment[propositions_set[i]] = -1;
          }
          return false;
      }
};



int main(int argc, char ** argv) {
    if (argc < 2) {
        printf("usage : <commandline> <file.in>\n");
        printf("use sat format in the file.in\n");
        printf("Output : <var, value>\n");
        printf("0 : false, 1: true, -1: don't care, -2 : does not appear");
        
        return 0;
    }
    string line;
    Problem * problem = NULL;
    int val;
    vector <int> * values_vector = NULL;
    int * idx = NULL;
    int n;
    int m;
    ifstream infile (argv[1]);

    if ( not infile.is_open() ) {
      printf("cannot open file %s\n", argv[1]);
      return 0;
    }

    while(getline(infile, line)) {
       if(line[0] == 'c') {
          continue;
       } else if (line[0] == 'p'){
          stringstream ss ;
          ss << line;
          char problem_type[5];
          char p;
          ss >> p;
          ss >> problem_type;
          ss >> n;
          ss >> m;
          if (strcmp(problem_type, "cnf") != 0) {
              printf("problem type not recognized\n");
          } {
              printf(" The problem configuration is %c : %s : %d : %d\n", p, problem_type, n, m);
          }

          // release previous variables
          if ( values_vector != NULL ) delete values_vector;
          if ( idx !=NULL ) delete idx;
          if ( problem !=NULL ) delete problem;

          idx = new int[m+1];
          values_vector = new vector<int>();

          int value_idx=0;
          idx[0] = 0;
          for ( int i=0;i < m;i++ ) {

              infile >> val;
              while (val) {
                  values_vector->push_back(val);
                  value_idx ++;
                  infile >> val;
              }
              idx[i+1] = value_idx;
          }
          // input rest of the line
          getline(infile, line);
          problem = new Problem(values_vector, idx, n, m);
          problem->solve();
       } else {
          printf("unrecognized line : %s\n", line.c_str() );
       }
    }
}
