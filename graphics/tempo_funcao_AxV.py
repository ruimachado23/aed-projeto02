import numpy as np
import matplotlib.pyplot as plt

# DADOS
# tamanh
tamanho = np.array([63, 77, 84, 195, 375])

# tempos de execução
tempo_alg1 = np.array([0.000007, 0.000007, 0.000006, 0.000012, 0.000013])
tempo_alg2 = np.array([0.000001, 0.000002, 0.000002, 0.000002, 0.000003])
tempo_alg3 = np.array([0.000002, 0.000002, 0.000002, 0.000003, 0.000004])

# Plotar os gráficos
plt.plot(tamanho, tempo_alg1, label='Alg 1', color='blue', linewidth=2)
plt.plot(tamanho, tempo_alg2, label='Alg 2', color='red', linewidth=2)
plt.plot(tamanho, tempo_alg3, label='Alg 3', color='green', linewidth=2)
plt.xlabel('Arestas x Vértices')
plt.ylabel('Tempos de Execução')
plt.title('Tempo em função Arestas x Vértices')
plt.legend()
plt.grid(True)

plt.show()