# Proyecto de Senales (Entrega 1, 29 de mayo) Thomas Little

## Resumen

TODO: explicar en tus palabras que hace el proyecto.

Idea general:

- Cargar un archivo WAV.
- Convertirlo a mono.
- Partir la senal en bloques.
- Aplicar FFT a cada bloque.
- Exportar CSVs con datos de frecuencia.
- Hacer la inversa de Fourier por bloque.
- Reconstruir un WAV final sumando/pegando los bloques de vuelta.
- Generar graficos con matplotlib desde los CSV exportados.

## Como correr

Compilar y ejecutar con:

```bat
run.bat
```

O manualmente:

```bash
g++ -DLOCAL -std=c++20 -Wall -Wextra -Wshadow -Wconversion -Wno-cpp main.cpp -o main.exe
main.exe
```

El programa pide el nombre del archivo WAV por consola.

## Archivos de salida

Los resultados quedan en la carpeta `export/`.

- `fourier_0.csv`, `fourier_1.csv`, etc: espectro de cada bloque.
- `normal_0.csv`, `normal_1.csv`, etc: bloques despues de aplicar la inversa.
- `reconstructed.wav`: audio reconstruido desde los bloques inverseados.
- `spectrogram.png`: imagen tipo espectrograma con todos los bloques.
- `spectra_by_block.png`: grafico separado de cada bloque.

## Graficos

`main.cpp` intenta llamar automaticamente:

```bash
python plot_spectrograms.py --no-show
```

Si Python o matplotlib no estan instalados, puedes correrlo manualmente despues de instalar dependencias:

```bash
pip install matplotlib
python plot_spectrograms.py
```

## Disclaimer

`dr_wav.h` fue descargado de internet como libreria, los creditos estan dentro del mismo archivo.

Algunos comentarios explicativos agregados al codigo y el archivo `plot_spectrograms.py` fueron escritos con ayuda de Codex, lo otro fue hecho por mi (thomas). Se uso codex al final para que le pegase una pasada y arreglase cualquier oversight que tuve y no atrape con testing.cpp. El script de matplotlib se llama desde `main.cpp` para crear automaticamente las imagenes del espectrograma a partir de los CSV `fourier_*.csv`.

