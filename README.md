# ProyectoCGIHC

# Proyecto Final: Vida Sustentable en el Espacio
**Equipo 3 - CGIHC**

## Descripción General
Este proyecto consiste en una simulación interactiva desarrollada en **C++** y **OpenGL**, que explora un entorno espacial habitable. La aplicación integra técnicas avanzadas de computación gráfica como iluminación dinámica, animación por huesos (Rigging), cinemática de partículas y shaders de animación procedimental para representar la vida y operación en una base espacial.

## Características Técnicas
El sistema implementa una arquitectura basada en shaders (GLSL 3.3 core) para lograr diversos efectos visuales:

* **Iluminación Avanzada**: Uso del modelo de iluminación de Phong con soporte para múltiples fuentes de luz (ambientales, puntuales y focales/spotlights).[cite: 15, 18, 19, 26, 28].
* **Animación de Personajes**: Implementación de *Vertex Skinning* para el movimiento del astronauta, permitiendo animaciones fluidas mediante el uso de matrices de transformación de huesos (`gBones`).
* **Animación Procedimental**:
    * **Movimiento Orbital**: Simulación de sondas espaciales (Juno y Voyager) con trayectorias calculadas en tiempo real.
    * **Efectos de Ondas**: Animación de la bandera mediante funciones sinusoidales en el Vertex Shader.
    * **Transformaciones en Espacio de Cámara**: Los cálculos de reflexión y brillo especular se realizan dinámicamente para asegurar una iluminación coherente desde la perspectiva del usuario.
* **Entorno Inmersivo**: Implementación de un `Skybox` mediante un Cubemap que proyecta una textura de 360° del espacio profundo.
## Requisitos del Sistema
* **Lenguaje**: C++.
* **Gráficos**: OpenGL 3.3+ con GLAD.
* **Bibliotecas**:
    * **GLFW**: Manejo de ventanas y eventos.
    * **GLM**: Operaciones matemáticas de matrices y vectores.
    * **Assimp**: Carga de modelos 3D complejos (FBX).
    * **irrKlang**: Motor de audio para efectos sonoros espaciales.

## Instrucciones de Control
La interacción con la escena se realiza mediante los siguientes periféricos:

### Navegación y Cámara
* **Flechas Direccionales**: Controlan el desplazamiento (Arriba/Abajo) y la rotación (Izquierda/Derecha) del astronauta.
* **Teclas W, A, S, D**: Movimiento libre de la cámara en el espacio.
* **Barra Espaciadora / Shift**: Ascender y descender en la escena.
* **F1 / F2**: Alternar entre cámara en tercera persona y cámara libre.

### Interacción con Objetos
* **Tecla K**: Disparo de láser desde los marcianos (dirección y color aleatorio).
* **Teclas Y, U**: Rotación manual de la excavadora.
* **Teclas H, J**: Control de rotación de la supernova.
* **Tecla L**: Control dinámico de la caída y tamaño del meteorito.
* **M, N, B**: Cambio de modo de renderizado (Líneas, Sólido, Puntos).

## Créditos y Recursos
* **Modelos 3D**: Formatos FBX procesados para optimización de vértices.
* **Texturas**: Mapas de difusión y Cubemap espacial procesado para proyección equirectangular.


---
*Este proyecto fue desarrollado con fines académicos para la asignatura de Computación Gráfica e Interacción Humano-Computadora.*.