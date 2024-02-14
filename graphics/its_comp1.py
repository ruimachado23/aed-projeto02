import numpy as np
import matplotlib.pyplot as plt

# DADOS
# tamanho
tamanho = np.array([63, 77, 84, 195, 375])

iteracoes_alg1 = np.array([51, 53, 54, 132, 224])

its_alg1_T = np.array([49, 49, 49, 169, 225])

# Plotar os gráficos
plt.plot(tamanho, iteracoes_alg1, label='Experimental', color='blue', linewidth=2)
plt.plot(tamanho, its_alg1_T , label='Teórico', color='red', linewidth=2)
plt.xlabel('Arestas x Vértices')
plt.ylabel('Nº de Iterações')
plt.title('Comparação das iterações experimentais e teóricas (Alg 1)')
plt.legend()
plt.grid(True)

plt.show()