import os
import re

# Configurações
pasta_projeto = './biblioteca/' # A pasta onde o script está rodando
arquivo_saida = 'Tinkercad_Final.ino'

includes_globais = set()
codigo_h = ""
codigo_cpp = ""
codigo_ino = ""

# Expressões regulares para identificar os tipos de #include
regex_include_global = re.compile(r'#include\s+<([^>]+)>') # Ex: #include <EEPROM.h>
regex_include_local = re.compile(r'#include\s+"([^"]+)"')  # Ex: #include "Jogador.h"

print("Iniciando a união dos arquivos...")

# Passa por todos os arquivos da pasta
for arquivo in os.listdir(pasta_projeto):
    # Pula o próprio arquivo de saída se ele já existir e o próprio script Python
    if arquivo == arquivo_saida or arquivo.endswith('.py'): 
        continue
    
    caminho = os.path.join(pasta_projeto, arquivo)
    if not os.path.isfile(caminho): 
        continue

    # Só processa arquivos do Arduino
    if arquivo.endswith(('.h', '.cpp', '.ino')):
        with open(caminho, 'r', encoding='utf-8') as f:
            linhas = f.readlines()
            
        conteudo_limpo = ""
        for linha in linhas:
            match_global = regex_include_global.search(linha)
            if match_global:
                nome_da_biblioteca = match_global.group(1)
                
                # Se começar com 'biblioteca/', é arquivo de vocês. Ignora a linha!
                if nome_da_biblioteca.startswith('biblioteca/'):
                    continue
                else:
                    # Se for biblioteca nativa (ex: <Wire.h>), guarda para pôr no topo
                    includes_globais.add(linha.strip())
                    continue
        
            # Se for include local ("..."), deleta a linha para não dar erro no Tinkercad
            if regex_include_local.search(linha):
                continue
            
            conteudo_limpo += linha

        # Separa o código nos blocos corretos de ordem
        cabecalho = f"\n// " + "="*40 + f"\n// ARQUIVO: {arquivo}\n// " + "="*40 + "\n"
        
        if arquivo.endswith('.h'):
            codigo_h += cabecalho + conteudo_limpo
        elif arquivo.endswith('.cpp'):
            codigo_cpp += cabecalho + conteudo_limpo
        elif arquivo.endswith('.ino'):
            codigo_ino += cabecalho + conteudo_limpo

# Escreve tudo no arquivo final na ordem obrigatória do C++
with open(arquivo_saida, 'w', encoding='utf-8') as f:
    f.write("// === BIBLIOTECAS NATIVAS ===\n")
    for inc in sorted(includes_globais):
        f.write(inc + "\n")
    
    f.write("\n\n// === 1. DECLARAÇÕES (.h) ===\n")
    f.write(codigo_h)
    
    f.write("\n\n// === 2. IMPLEMENTAÇÕES (.cpp) ===\n")
    f.write(codigo_cpp)
    
    f.write("\n\n// === 3. CÓDIGO PRINCIPAL (.ino) ===\n")
    f.write(codigo_ino)

print(f"Sucesso! Código unificado salvo em: {arquivo_saida}")