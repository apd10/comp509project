import sys
import numpy  as np


N = int(sys.argv[1]) # N nuber of variables
L = int(sys.argv[2]) # number of clauses
K = int(sys.argv[3]) # distinct literals per clause
n = int(sys.argv[4]) # number of problems


for i in range(n):
    np.random.seed(i)
    print("c This is sample number", i + 1, "for config", N,L,K);
    print("p cnf",N,L)
    for j in range(L):
        literals = np.random.randint(1,N+1,(K))
        while( len(np.unique(literals)) != K):
            literals = np.random.randint(1,N+1,(K))
        signs = np.random.randint(0,2,(K)) * 2 - 1
        clause = signs * literals
        print(*clause, 0)

