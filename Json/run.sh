#!/bin/bash

# Verificar si Handlebars está instalado
if node -e "require('handlebars')" 2>/dev/null; then
    echo "✅ Handlebars ya está instalado."
else
    echo "⚠️  Handlebars no está instalado. Instalando..."
    
    # Verificar si hay un package.json, si no lo hay, crear uno básico
    if [ ! -f "package.json" ]; then
        echo "{}" > package.json
    fi

    # Instalar handlebars localmente
    npm install handlebars

    if [ $? -ne 0 ]; then
        echo "❌ Falló la instalación de Handlebars. Abortando."
        exit 1
    fi
fi

# Ejecutar el script de generación
echo "🚀 Ejecutando generateHtml.js..."
node generateHtml.js
