'''
20230325
Lee, YunTae
2023 IMMC
Using Land: A valuable resource

finding area section maximize P.
using values and synergy.
'''

from decimal import Decimal, getcontext
import itertools

getcontext().prec = 10

# if problem 3: True
considerFactory = False

E = ['A', 'G', 'S', 'L']

n = 4
rho = [[0, -0.3, -0.2, -0.2],
        [0, 0, 0, 0.1],
        [0.1, 0, 0, 0.2],
        [-0.2, 0, 0, 0]]
rho_F = [-0.4, 0, 0, 0.4]
beta = [9.09, 319.55, 623.64, 0]
gamma = [8.81, 16.87, 14.75, 0]

rho = [[Decimal(str(i)) for i in sub] for sub in rho]
rho_F = [Decimal(str(i)) for i in rho_F]
gamma = [Decimal(str(i)) for i in gamma]
beta = [Decimal(str(i)) for i in beta]

B_F = Decimal('9e8')

iteration = itertools.combinations_with_replacement(range(n+1), 30)

result = []
for case in iteration:
    a = [Decimal(str(case.count(i) / 10)) for i in range(n+1)]
    N_density = a[-1] / 3
    
    B = [beta[i] * a[i] * 1000000 for i in range(n)]
    V = [gamma[i] * a[i] * 1000000 for i in range(n)]

    # change B_L, V_L
    if considerFactory:
        X = min(Decimal('1787.5'), a[3] / 90 * 1000000)
    else:
        X = min(Decimal('1414'), a[3] / 90 * 1000000)
    
    B[-1] = 6600 * X
    V[-1] = 5940 * X
    
    P = 0
    
    for i in range(n):
        P += V[i]

    for i in range(n):
        for j in range(n):
            P += rho[i][j] * pow(pow(B[i], 1 + abs(rho[i][j])) * pow(B[j], 1 - abs(rho[i][j])), Decimal('0.5'))

    for i in range(n):
        # Nature doesn't influence electric power generation.
        if i != 1:
            P += N_density * B[i]
        
    if considerFactory:
        for i in range(n):
            P += rho_F[i] * pow(pow(B_F, 1 + rho_F[i]) * pow(B[i], 1 - rho_F[i]), Decimal('0.5'))
    
    result.append((P, a))
    
result.sort(reverse=True)
for i in range(10):
    print(result[i])
