def gerar_clusters_artificiais(nome_arquivo, num_clusters=5, pontos_por_cluster=4000, dimensao=2, distancia=100):
    import random
    random.seed(42)
    with open(nome_arquivo, "w") as f:
        id_counter = 1
        for c in range(num_clusters):
            centro = [c * distancia for _ in range(dimensao)]
            for _ in range(pontos_por_cluster):
                coords = [round(random.gauss(centro[i], 10), 2) for i in range(dimensao)]
                id_ponto = f"P{id_counter:05d}"
                linha = f"{id_ponto}," + ",".join(map(str, coords)) + "\n"
                f.write(linha)
                id_counter += 1
    print(f"Arquivo '{nome_arquivo}' gerado com {num_clusters * pontos_por_cluster} pontos.")

gerar_clusters_artificiais("teste.txt", num_clusters=10, pontos_por_cluster=1500, dimensao=2)
