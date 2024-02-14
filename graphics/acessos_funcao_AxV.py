import numpy as np
import matplotlib.pyplot as plt

# DADOS
# tamanho
tamanho = np.array([63, 77, 84, 195, 375])

# acessos a memoria
acessos_alg1 = np.array([84, 84, 84, 183, 224])
acessos_alg2 = np.array([129, 133, 135, 303, 393])
acessos_alg3 = np.array([66, 69, 70, 108, 131])

# Plotar os gráficos
plt.plot(tamanho, acessos_alg1, label='Alg 1', color='blue', linewidth=2)
plt.plot(tamanho, acessos_alg2, label='Alg 2', color='red', linewidth=2)
plt.plot(tamanho, acessos_alg3, label='Alg 3', color='green', linewidth=2)
plt.xlabel('Arestas x Vértices')
plt.ylabel('Nº de Acessos a Memória')
plt.title('Acessos à memória em função Arestas x Vértices')
plt.legend()
plt.grid(True)

plt.show()