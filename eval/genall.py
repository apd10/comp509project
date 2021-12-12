N=150
K=6


for i in range(16):
    ratio =  0.4 * i + 3
    L = int(ratio * N)
    print("python3 generate_random_problem.py",N,L,K,100,"&>problem.N{}.L{}.K{}".format(N,L,K))
