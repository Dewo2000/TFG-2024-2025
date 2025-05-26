#!/bin/bash
if [ "$#" -ne 1 ]; then
    echo "Uso: $0 <directorio_de_imagenes>"
    exit 1
fi

# Define el directorio de entrada y salida
input_dir="$1"
output_dir="$1result"
tessdata_dir="trainingFont/trainedModel"

# Crea el directorio de salida si no existe
mkdir -p "$output_dir"

start_time=$(date +%s%3N)

# Procesa todos los archivos .png en el directorio de entrada
for file in "$input_dir"/*.png; do
    # Extrae el nombre del archivo sin la extensión
    filename=$(basename "$file" .png)
    # Ejecuta Tesseract
    tesseract "$file" "$output_dir/$filename" --tessdata-dir "$tessdata_dir" -l Arial
done

# Registra el tiempo de finalización
end_time=$(date +%s%3N)  # Tiempo en milisegundos

# Calcula el tiempo transcurrido
elapsed_time=$((end_time - start_time))

# Muestra el tiempo transcurrido
echo "Tiempo transcurrido: ${elapsed_time} ms"