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


    #Binarization
    ocropus-nlbin -n $input_dir/*.png
    #Segmentation
    ocropus-gpageseg -n --minscale 8 $input_dir/*.bin.png 
    
    ocropus-rpred -m Apex.pyrnn.gz $input_dir/*/*.bin.png


# Registra el tiempo de finalización
end_time=$(date +%s%3N)  # Tiempo en milisegundos

# Calcula el tiempo transcurrido
elapsed_time=$((end_time - start_time))

# Muestra el tiempo transcurrido
echo "Tiempo transcurrido: ${elapsed_time} ms"
