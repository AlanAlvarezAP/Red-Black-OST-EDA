# Red-Black-OST-EDA
Perteneciente al proyecto final del curso de Estructura de datos Avanzadas donde se simulara el **flujo dinamico de la llegadas de noticias**

**Realizado por:**  Alan Alvarez  
**Profesor:**  Erick Gomez Nieto  
**Semestre:**  2025-II

# Flujo
Llegada de noticia -> Preprocesamiento (Extracción topico) -> Inserción Estructura -> Query respecto a una ventana m y top k  

# Archivos incluidos
Se incluyen los siguientes archivos en el repositorio:  

- **`Red-Black-OST-EDA.cpp`**:  Main donde se pedira los datos para realizar la simulación , ejecución 
- **`RB_OST.cpp\.h`**:  Archivos que tienen la implementación del arbol Red-Black OST con sus respectivos metodos
- **`Preprocesamiento.cpp\.h`**:  Todos los pasos necesarios para preprocesar recibiendo un archivo Limpieza->Tokenizar->Stop_words->Porter_Stemming->Conteo
- **`Porter.cpp\.h`**:  Implementación del Porter Stemming manual
- **`MiArray.h`**:  Implementación desde 0 de un Vector
- **`EstructurasAuxiliares.cpp\.h`**:  Implementación de estructuras auxiliares como la HashTable

Además considerar que en la entrega respectiva se añadio los archivos `topk.csv` donde se guardara los tops ejecutados y `stop_words.txt` que contiene las stopwords respectivas con `script_wordcloud.py` para la visualización de la Wordcloud

# Arquitectura
![Arquitectura clases](https://github.com/AlanAlvarezAP/Red-Black-OST-EDA/blob/main/Arquitectura%20RB_OST.png?raw=true)

# Funcionalidades
El proyecto permite realizar la siguiente funcionalidades:  
1. **`Procesamiento de un directorio`**:  
Utilizando la librería `windows.h`, se puede leer un directorio completo que solo contenga archivos `.txt`.  
Para definir qué directorio leer, modifica la línea:  
auto archivos = leer_archivos_carpeta("EDA input");  
Cambiando `"EDA input"` por el nombre del directorio que debe estar ubicado donde se encuentre el resto de archivos.

2. **`Query últimos m momentos`**:  
Permite, bajo la definición de un top k, calcular los tópicos más reincidentes dentro de la ventana actual y m momentos antes, generando su WordCloud.

3. **`Query rango personalizado`**:  
Permite calcular los top k más reincidentes pero bajo un rango **[x1;x2]**, con su respectiva WordCloud.

4. **`Query con configuración modificable`**:  
Permite definir una configuración inicial, realizar las configuraciones y queries previamente descritas para la siguiente inserción y modificarlas según sea necesario.

5. **`Query anteriores inserción por inserción`**:  
Se puede realizar todo lo anterior definiendo si se analiza después de procesar todo el directorio o inserción por inserción, logrando la simulación buscada.

## Demostración ejecución

Para el punto **2**, el flujo de respuestas al ejecutar el programa sería algo así:

```console
 EMPEZANDO SIMULACION ...
┐Desea query de los ultimos k (S) o rango personalizado (N)?
S
Dame los ultimos cuantos deseas
2000
Ingrese top m a mostrar en la wordcloud (m > 0):
1800
```
Para el punto **3**, el flujo es:
```console
 EMPEZANDO SIMULACION ...
┐Desea query de los ultimos k (S) o rango personalizado (N)?
N
Dame el inicio de tu rango
100
Dame el fin de tu rango
500
```
Para el punto **4** se le preguntara lo anterior y luego se le consultara:
```console
┐Desea que esta configuracion continue o se modifique? (S/N)
S/N
```

Para el punto **5** se le preguntara lo anterior y luego se le consultara:
```console
┐Desea que se realice su consulta al final del procesado (S/N)?
S/N
```



# Dependencias
Para la instalación se necesitara las siguientes dependencias:  
  -Realizar la descarga o ```git clone https://github.com/AlanAlvarezAP/Red-Black-OST-EDA.git``` para tener los archivos correspondientes y realizar los siguientes pasos  
  -Compilador para C++ en este caso **gcc(15.2)** donde su compilación del codigo será ```g++ -std=c++17 -O2 *.cpp -o program```  
  -Contar con versión reciente de **Python ultima versión (3.11.9)** la cual se necesitara las siguientes pip install matplotlib wordcloud   
  -Dataset a utilizar incluido en el repositorio o descargarlo desde https://vicg.icmc.usp.br/vicg/tool/7/data-sets-from-papers

# Nota 
Tomar en cuenta que para el Red-Black-OST-EDA.cpp tomar en cuenta que se utiliza `#include <windows.h>` por ende su compilación entre otros sistemas operativos fallara 
# Ejecución
Habiendo realizado los pasos de la dependencia simplemente llamar dependiendo del comando g++ en este caso **programa** se llamaria ./program
