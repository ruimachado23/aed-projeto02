import numpy as np
import matplotlib.pyplot as plt

# DADOS
# tamanho
tamanho = np.array([63, 77, 84, 195, 375])

# iteracoes
iteracoes_alg1 = np.array([51, 53, 54, 132, 224])
iteracoes_alg2 = np.array([51, 53, 54, 132, 224])
iteracoes_alg3 = np.array([37, 39, 40, 67, 85])

# Plotar os gráficos
plt.plot(tamanho, iteracoes_alg1, label='Alg 1', color='blue', linewidth=2)
plt.plot(tamanho, iteracoes_alg2, label='Alg 2', color='red', linewidth=2)
plt.plot(tamanho, iteracoes_alg3, label='Alg 3', color='green', linewidth=2)
plt.xlabel('Arestas x Vértices')
plt.ylabel('Nº de Iterações')
plt.title('Iterações em função Arestas x Vértices')
plt.legend()
plt.grid(True)

plt.show()