#include <stdio.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string.h>
#include <vector>
#include <set>
#include <chrono>
#include <assert.h>
#include <unordered_map>


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
      enum splitting_protocol_enum
      {   
          ENUM_FIRST,
          ENUM_RANDOM,
          ENUM_OCCUR2,
          ENUM_MAX
      };
      splitting_protocol_enum splitting_protocol;

      // for stats
      int num_splitting_applications;
      int num_random_splits;
      int num_single_applications;
      double time_taken;

      Problem(vector<int> * _values, int * _idx, int _n, int _m, splitting_protocol_enum sp) 
          : values(_values), idx(_idx), m(_m), n(_n), splitting_protocol(sp){ 
          clause_evaluations = new int[m];
          active_counts = new int[m];
          proposed_proposition = new int[m];
          truth_assignment = new int[n+1];
          unassigned_propositions = new vector<int>();
          num_splitting_applications = 0;
          num_random_splits = 0;

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
          srand(1);
          num_splitting_applications = 0;
          num_random_splits = 0;
          num_single_applications = 0;
          auto t_start  = std::chrono::high_resolution_clock::now();
          bool res = dpll();
          auto t_end  = std::chrono::high_resolution_clock::now();
          time_taken = chrono::duration<double, std::milli>(t_end - t_start).count();

          if ( res) {

                  printf("SAT time: %f ms, num_single: %d num_split: %d num_rand: %d\n", time_taken, num_single_applications, num_splitting_applications, num_random_splits);
                  /*      
                  printf("0 : false, 1: true, -1: don't care \n");
                  for(int i=1; i < n+1; i++) {
                          //if(truth_assignment[i] != -2)
                              printf(" %d %d \n", i, truth_assignment[i]);
                  }*/
          } else  {

                  printf("UNSAT time: %f ms, num_single %d num_split: %d num_rand: %d\n", time_taken, num_single_applications, num_splitting_applications, num_random_splits);
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
                    ////printf("clause failed %d\n", i);
                    negative ++;
                } else {
                    incomplete ++;
                }
            }

        //printf("clause: %d evaluated to : %d\n", i, clause_evaluations[i]);
        }
        ////printf("pos: %d, neg:%d, inc:%d m:%d\n",positive, negative, incomplete, m);
        if (negative > 0) {
          return 0;
        } else if ( incomplete > 0) {
          return -1;
        } else {
          return 1;
        }
      }

      int get_maxoccur(int desired_count) {
          std::unordered_map<int, int> counts;
          int max_count = 0;
          int p;
          for (int i=0; i < m; i++ ){
              if( active_counts[i] == desired_count) {
                ////printf("clause:%d has 2 active\n", i);
                 for(int j = idx[i]; j < idx[i+1];j++) {
                    p= abs((*values)[j]);
                    if(truth_assignment[p] == -1) {
                        if(counts.find(p) == counts.end()) {
                            counts[p] = 1;
                        }else {
                            counts[p] ++;
                        }
                        max_count = max(max_count, counts[p]);
                        ////printf("updated %d - %d\n", p, counts[p]);
                    }
                } 
              }
          }
          vector<int> props ;
          for(auto itr = counts.begin(); itr!=counts.end(); itr++) {
              ////printf("1. %d %d\n", itr->first, itr->second);
              if(itr->second == max_count){
              ////printf(" -- selected\n");
                  props.push_back(itr->first);
              }       
           }

          if (props.size() > 0) {
              //char c; //printf(">>\n");scanf("%c", &c);
              return props[rand() % props.size()];
          }
          else {
              return -1;
          }
      }
      
      int get_random() {
              int temp = rand() % unassigned_propositions->size();
              int prop = (*unassigned_propositions)[temp];
              return prop;
      }


      bool safeexit(bool returnvalue, vector<int> &propositions_set) {
              if ( not returnvalue) {
                      for(int i = 0 ; i < propositions_set.size(); i++) {
                              truth_assignment[propositions_set[i]] = -1;
                      }
              }
              return returnvalue;
      }

      bool dpll() {
          //print_ta();
          //char c;
          int eval = evaluate_cat();
          if (eval == 0) {
              //printf("exit1\n");
              return false;
          } else if (eval == 1) {

              //printf("exit2\n");
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
                                  num_single_applications += 1;
                                  lt = proposed_proposition[i];
                                  p = (lt < 0) ? -lt: lt;
                                  truth = (lt > 0) ? 1 : 0;

                                  //printf("single : setting %d for %d in accordance with clause %d \n", truth, p, i); 
                                  if ( truth_assignment[p] != -1 and truth_assignment[p] != truth) {
                                          ////printf("Issue in assignment 1 : %d \n", p);
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
                                  //printf("exit3\n");
                                  return safeexit(false, propositions_set);
                          } else if (eval == 1) {

                                  //printf("exit4\n");
                                  return safeexit(true, propositions_set);
                          }
                  }
          }
          if ( not allok ) {
              // reset the propositions set and return
              //printf("exit5\n");
              return safeexit(false, propositions_set);
          }
          
          eval = evaluate_cat();
          if (eval == 0) {
              //printf("exit6\n");
              return safeexit(false, propositions_set);;
          } else if (eval == 1) {

              //printf("exit7\n");
              return safeexit(true, propositions_set);

          }

          //print_problem(); scanf("%c", &c);
          num_splitting_applications += 1;
          int prop = (*unassigned_propositions)[0]; 
          int temp = 0;
          switch(splitting_protocol) {
              case ENUM_FIRST:
                  prop =( *unassigned_propositions)[0];
                  break;
              case ENUM_RANDOM:
                  prop = get_random();
                  break;
              case ENUM_OCCUR2:
                  // select the proposition that appears most in 2-occur
                  // this logic is not implemented in the evaluate_cat because we should not
                  // penalise other methods for this strategy
                  prop = get_maxoccur(2);
                  if (prop == -1) {
                      prop = get_maxoccur(3);
                      if (prop == -1) {
                        num_random_splits += 1;
                        prop = get_random();
                      }
                  }
                  break;
          
              default:  
                  assert(false);
                  break;
          }
          //printf("choosing, %d 1\n", prop);
          truth_assignment[prop] = 1;
          bool val;
          val = dpll();
          if (val) {

            //printf("exit8\n");
            return safeexit(true, propositions_set);
          }

          truth_assignment[prop] = 0;

          //printf("choosing, %d 0\n", prop);
          val = dpll();
          if (val) {
            //printf("exit9\n");
            return safeexit(true, propositions_set);
          }
        
          // reset everything
          truth_assignment[prop] = -1;
          //printf("unsetting %d\n", prop);
          //printf("exit10\n");
          return safeexit(false, propositions_set);
      }
};



int main(int argc, char ** argv) {
    if (argc < 3) {
        printf("usage : <commandline> <file.in> <protocol>\n");
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
    Problem::splitting_protocol_enum splitting_protocol;
    ifstream infile (argv[1]);
    int enidx = atoi(argv[2]);
    switch(enidx) {
            case 0:
                    splitting_protocol = Problem::ENUM_FIRST;
                    break;
            case 1:
                    splitting_protocol = Problem::ENUM_RANDOM;
                    break;
            case 2:
                    splitting_protocol = Problem::ENUM_OCCUR2;
                    break;
            default:  
                    printf("Protocol not found\n");
                    return  0;
    }

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
              //printf(" The problem configuration is %c : %s : %d : %d\n", p, problem_type, n, m);
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
          problem = new Problem(values_vector, idx, n, m, splitting_protocol);
          problem->solve();
       } else {
          printf("unrecognized line : %s\n", line.c_str() );
       }
    }
}
