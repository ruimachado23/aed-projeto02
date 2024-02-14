import numpy as np
import matplotlib.pyplot as plt

# DADOS
# tamanho
tamanho = np.array([63, 77, 84, 195, 375])

iteracoes_alg3 = np.array([37, 39, 40, 67, 85])

its_alg3_T = np.array([16, 18, 19, 28, 40])

# Plotar os gráficos
plt.plot(tamanho, iteracoes_alg3, label='Experimental', color='blue', linewidth=2)
plt.plot(tamanho, its_alg3_T , label='Teórico', color='red', linewidth=2)
plt.xlabel('Arestas x Vértices')
plt.ylabel('Nº de Iterações')
plt.title('Comparação das iterações experimentais e teóricas (Alg 3)')
plt.legend()
plt.grid(True)

plt.show()