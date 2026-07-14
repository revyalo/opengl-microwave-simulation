# OpenGL Microwave Simulation

Simulación 3D interactiva de una cocina nocturna desarrollada en **C++** con **OpenGL 3.3**. El elemento principal es un microondas funcional con puerta animada, plato giratorio, iluminación dinámica, cámaras interactivas y un sistema de partículas semitransparentes para representar vapor.

El proyecto fue desarrollado como trabajo individual para la asignatura de **Informática Gráfica** del Grado en Ingeniería Informática de la Universidad Rey Juan Carlos.

## Características principales

- Apertura y cierre animado de la puerta del microondas.
- Lógica de funcionamiento: el microondas solo cocina con la puerta cerrada y se detiene al abrirla.
- Plato y pollo giratorios durante la cocción.
- Dos mandos interactivos para regular la intensidad de la luz interior y la cantidad de vapor.
- Sistema de partículas con transparencias y solape.
- Cámara orbital controlada con el ratón.
- Cámara interior para observar la escena desde dentro del microondas.
- Cámara frontal de presentación.
- Lámpara articulada construida mediante transformaciones jerárquicas.
- Iluminación global, direccional, posicional y focal.
- Foco de techo activable por teclado.
- Sombras dinámicas mediante **shadow mapping**.
- **Normal mapping** aplicado a la pared para simular relieve.
- Skybox nocturno implementado mediante **cube maps**.
- Modelos 3D externos en formato OBJ, materiales y texturas.

## Tecnologías utilizadas

- C++17
- OpenGL 3.3 Core Profile
- GLSL
- CMake 3.22+
- CLion
- GLFW
- GLEW
- GLM
- Assimp
- FreeImage

## Controles

| Entrada | Acción |
|---|---|
| `O` | Abrir o cerrar la puerta. Al abrirla, se detiene la cocción. |
| `S` | Iniciar o detener el microondas si la puerta está cerrada. |
| `C` | Activar o desactivar la cámara interior. |
| `V` | Activar o desactivar la cámara de presentación. |
| `U / J` | Girar el mando superior y modificar la intensidad de la luz interior. |
| `I / K` | Girar el mando inferior y modificar la cantidad de vapor. |
| `F` | Encender o apagar el foco del techo. |
| `L` | Encender o apagar la lámpara articulada. |
| `1 / 2` | Rotar la base de la lámpara. |
| `3 / 4` | Mover el primer brazo de la lámpara. |
| `5 / 6` | Mover el segundo brazo de la lámpara. |
| `7 / 8` | Mover el cabezal de la lámpara. |
| Ratón | Controlar la cámara orbital. |
| Rueda del ratón | Acercar o alejar la cámara. |
| `Esc` | Cerrar la aplicación. |

## Estructura del repositorio

```text
opengl-microwave-simulation/
├── CMakeLists.txt                 # Configuración de compilación con CMake
├── src/                           # Código fuente principal
├── binary/
│   └── resources/
│       ├── models/                # Modelos 3D en formato OBJ
│       ├── textures/              # Texturas y mapas normales
│       └── shaders/               # Shaders GLSL
└── lib/                           # Dependencias y cabeceras incluidas
```

## Entorno de desarrollo original

El proyecto fue desarrollado y probado con:

- **CLion**
- **MacBook Pro 2017 con procesador Intel**
- **macOS Ventura 13.7.8**
- **Homebrew** instalado en `/usr/local`
- Estándar **C++17**

La configuración de compilación se encuentra en `CMakeLists.txt`. El ejecutable generado se llama `practica_micro` y se guarda en la carpeta `binary/`.

## Instalación y ejecución en macOS

### 1. Instalar las dependencias

```bash
brew install cmake glew glfw glm assimp freeimage
```

### 2. Abrir el proyecto en CLion

1. Clona el repositorio.
2. Abre en CLion la carpeta que contiene `CMakeLists.txt`.
3. Espera a que CLion configure el proyecto mediante CMake.
4. Selecciona la configuración de ejecución `practica_micro`.
5. Configura el directorio de trabajo como la carpeta `binary/` del proyecto.
6. Compila y ejecuta el proyecto.

La aplicación carga los recursos mediante rutas relativas como `resources/models`, `resources/textures` y `resources/shaders`. Por ello, el directorio de trabajo debe ser `binary/` para que el programa pueda encontrar correctamente los modelos, texturas y shaders.

### Rutas de Homebrew

El `CMakeLists.txt` original contiene rutas correspondientes a las versiones de Homebrew utilizadas durante el desarrollo:

- GLEW `2.2.0_1`
- GLFW `3.4`
- GLM `1.0.1`
- Assimp `6.0.2`
- FreeImage `3.18.0`

Si Homebrew instala versiones distintas, será necesario actualizar esas rutas en `CMakeLists.txt`. Las rutas actuales de cada dependencia pueden consultarse con:

```bash
brew --prefix glew
brew --prefix glfw
brew --prefix glm
brew --prefix assimp
brew --prefix freeimage
```

## Conceptos gráficos implementados

El proyecto aplica varios conceptos de programación gráfica en tiempo real:

- Transformaciones de modelado con matrices de traslación, rotación y escala.
- Objetos jerárquicos para la lámpara articulada.
- Materiales, texturas y mapas de normales.
- Iluminación con distintos tipos de fuentes de luz.
- Transparencias mediante `GL_BLEND`.
- Control del Z-buffer durante el renderizado del vapor.
- Animaciones temporizadas con GLFW.
- Shadow mapping.
- Cube maps y skybox.
- Cámaras orbital, interior y de presentación.

## Autor

**David Arévalo Rey**  
Grado en Ingeniería Informática — Universidad Rey Juan Carlos
