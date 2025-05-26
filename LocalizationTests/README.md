https://devblogs.microsoft.com/cppblog/build-c-applications-in-a-linux-docker-container-with-visual-studio/

### Para crear una imagen con el dockerFile
docker build -t localizationtests .
### Para crear un contenendor con la imagen en un cierto puerto para conectarse.
docker run -p 5000:22 -i -t localizationtests /bin/bash

###          crea un volumen en el directorio actual /volu     nombre del contenedor 
docker run -v %cd%/volumen:/home/localizationtests/volumen --name testing -p 5000:22 -i -t localizationtests /bin/bash

### Arrancar el servidor para conectarse a docker
service ssh start 
### Es necesario definir una contreseña la primera vez
passwd localizationtests


### Añadir fuente para el entreno:
Poner la fuente en la carpeta fonts y volver a buildear la imagen en docker.
Si ya esta el contenedor creado, poner la fuentes en la carpeta fonts del volumen y ejecutar copyFonts.sh