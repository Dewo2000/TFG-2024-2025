https://devblogs.microsoft.com/cppblog/build-c-applications-in-a-linux-docker-container-with-visual-studio/


# 🐳 Guía de Uso de `localizationtests` en Docker

## 🔧 Construcción de la Imagen

Para crear una imagen a partir del `Dockerfile`:

```bash
docker build -t localizationtests .
```

---

## 🚀 Ejecución del Contenedor

### ▶️ Conexión por puerto 5000 (puerto 22 del contenedor):

```bash
docker run -p 5000:22 -i -t localizationtests /bin/bash
```

### 💾 Crear un contenedor con volumen compartido:

```bash
docker run -v %cd%/volumen:/home/localizationtests/volumen --name testing -p 5000:22 -i -t localizationtests /bin/bash
```

---

## 🖥️ Iniciar el Servidor SSH en el Contenedor

Una vez dentro del contenedor, ejecutar:

```bash
service ssh start
```

**Nota:** Es necesario definir una contraseña la primera vez:

```bash
passwd localizationtests
```

---

## 🔤 Añadir Fuentes para Entrenamiento

### Opción A: Al construir la imagen

1. Colocar las fuentes en la carpeta `fonts`.
2. Ejecutar nuevamente el `docker build`.

### Opción B: Con contenedor ya creado

1. Colocar las fuentes en `fonts` dentro del volumen compartido.
2. Ejecutar dentro del contenedor:

```bash
./copyFonts.sh
```

---

## 🧪 Uso de la Herramienta

### 1. Descargar el ZIP

Descargar `localizationtest.zip` desde:

https://drive.google.com/drive/folders/1bRqojLCUeeF7kCh4h9LcKQL6H1HBa2CX?usp=drive_link

### 2. Descomprimir el archivo

```bash
unzip localizationtest.zip
```

### 3. Cargar la imagen `.tar`

```bash
docker load -i localizationtest.tar
```

### 4. Ejecutar el contenedor

```bash
docker run --name nombre_contenedor -p 5000:22 -i -t node:build /bin/bash
```

---

## 📁 Archivos de Ejemplo

Archivos de ejemplo disponibles en:

```bash
/home/localizationtests/sample
```

---

## 🧪 Ejecución para Testing

```bash
./localizationtests/bin/LocalizationTests.out --test -c configfile.json
```

Esto generará un archivo `result.json` en la carpeta de salida.

---

## 🏋️ Entrenamiento del Modelo

```bash
./localizationtests/bin/LocalizationTests.out --train -f font -l lan
```

---

## 📊 Generar Informe HTML

1. Copiar `result.json` a:

```bash
/home/localizationtests/Json
```

2. Ejecutar el script:

```bash
./run.sh
```

---

✅ Ahora tendrás tu informe de resultados generado en HTML.