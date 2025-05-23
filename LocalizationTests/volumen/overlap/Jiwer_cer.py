import jiwer
import glob
import os
import json
import argparse

# Configuración de argparse para recibir argumentos
parser = argparse.ArgumentParser(description='Calcular el CER entre archivos de referencia y de hipótesis.')
parser.add_argument('hypothesis_directory', type=str, 
                    help='Ruta al directorio que contiene los archivos de hipótesis.')
args = parser.parse_args()

# Directorios donde están los archivos
reference_directory = "./"  
hypothesis_directory = args.hypothesis_directory

# Obtener listas de archivos de referencia y de hipótesis
reference_files = sorted(glob.glob(os.path.join(reference_directory, "*-gt.txt")))
hypothesis_files = sorted(glob.glob(os.path.join(hypothesis_directory, "*.txt")))

# Diccionario para almacenar el CER de cada par
cer_results = {}
total_cer = 0
file_count = 0

# Emparejar archivos según el número
for ref_file in reference_files:
    # Obtener el número del archivo de referencia
    file_number = os.path.basename(ref_file).split("-gt.txt")[0]
    # Construir la ruta del archivo de hipótesis correspondiente
    hyp_file = os.path.join(hypothesis_directory, f"{file_number}.txt")

    if hyp_file in hypothesis_files:
        # Leer los contenidos de los archivos
        with open(ref_file, "r", encoding="utf-8") as ref:
            reference = ref.read()

        with open(hyp_file, "r", encoding="utf-8") as hyp:
            hypothesis_lines = hyp.readlines()

        # Quitar lineas en blaco
        hypothesis = [line.strip() for line in hypothesis_lines if line.strip()]
        hypothesis_string = " ".join(hypothesis)
        # Calcular el CER
        cer = jiwer.cer(reference, hypothesis_string)
        cer_results[file_number] = cer
        total_cer += cer
        file_count += 1
        print(f"CER para el archivo {file_number}: {cer}")
    else:
        print(f"No se encontró archivo de hipótesis para {file_number}")

# Calcular el CER medio
cer_average = total_cer / file_count if file_count > 0 else 0
cer_results["CER medio"] = cer_average

# Guardar los resultados en un archivo JSON
json_file_path = args.hypothesis_directory+"/cer_results.json"  # Nombre del archivo JSON donde se guardarán los resultados
with open(json_file_path, "w", encoding="utf-8") as json_file:
    json.dump(cer_results, json_file, indent=4)

print(f"Resultados CER guardados en {json_file_path}")
