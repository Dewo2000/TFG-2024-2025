const fs = require('fs');
const Handlebars = require('handlebars');

// Leer la plantilla de Handlebars
fs.readFile('template.hbs', 'utf8', (err, templateData) => {
    if (err) {
        console.error('Error al leer la plantilla:', err);
        return;
    }

    // Leer el archivo JSON
    fs.readFile('result.json', 'utf8', (err, jsonData) => {
        if (err) {
            console.error('Error al leer el archivo JSON:', err);
            return;
        }

        // Convertir el JSON a un objeto
        const data = JSON.parse(jsonData);

        // Compilar la plantilla con Handlebars
        const template = Handlebars.compile(templateData);
        const resultHtml = template(data);

        // Guardar el HTML resultante en un archivo
        fs.writeFile('output.html', resultHtml, 'utf8', (err) => {
            if (err) {
                console.error('Error al guardar el archivo HTML:', err);
                return;
            }
            console.log('HTML generado correctamente en output.html');
        });
    });
});
