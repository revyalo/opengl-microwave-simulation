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

- C++
- OpenGL 3.3 Core Profile
- GLSL
- GLFW
- GLEW
- GLM
- Assimp

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
├── src/                         # Código fuente principal
├── binary/
│   └── resources/
│       ├── models/              # Modelos 3D en formato OBJ
│       ├── textures/            # Texturas y mapas normales
│       └── shaders/             # Shaders GLSL
└── lib/                         # Dependencias incluidas en el proyecto
```

## Ejecución

La aplicación carga los recursos mediante rutas relativas como `resources/models`, `resources/textures` y `resources/shaders`. Por ello, el directorio de trabajo del ejecutable debe ser `binary/` o debe conservar una estructura equivalente en la que la carpeta `resources` quede junto al ejecutable.

### Requisitos

- Compilador compatible con C++.
- Tarjeta gráfica y controladores compatibles con OpenGL 3.3.
- GLFW, GLEW, GLM y Assimp correctamente incluidos y enlazados.

### Compilación

Este repositorio conserva el código fuente, las bibliotecas y los recursos del proyecto, pero actualmente no incluye un sistema de compilación automatizado como CMake. Para compilarlo desde otro entorno es necesario:

1. Crear o abrir un proyecto de C++.
2. Añadir los archivos de `src/`.
3. Configurar las rutas de inclusión y enlazado de las dependencias de `lib/`.
4. Copiar o mantener `binary/resources/` junto al ejecutable.
5. Ejecutar la aplicación usando `binary/` como directorio de trabajo.

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
