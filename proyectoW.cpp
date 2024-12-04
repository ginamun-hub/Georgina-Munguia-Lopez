#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "stb_image.h" // Biblioteca para cargar texturas

// Variables globales
GLuint texture;
float altura_inicial = 2.0f; // Altura inicial de la caída libre
float y = altura_inicial;    // Altura actual
float t = 0.0f;              // Tiempo para la animación
float g = 9.8f;              // Gravedad
bool animando = false;       // Controla la animación

// Función para cargar texturas
void cargarTextura(const char *escudo_unam) {
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    int width, height, nrChannels;
    unsigned char *data = stbi_load(escudo_unam, &width, &height, &nrChannels, 0);
    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        std::cerr << "Error al cargar la textura: " << escudo_unam << std::endl;
    }
    stbi_image_free(data);
}

// Función para dibujar la letra "W"
void dibujarLetraW() {
    // Habilitar texturas
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture);

    // Lado izquierdo con textura
    glBegin(GL_TRIANGLES);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.5f, y, 0.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-0.3f, y - 0.8f, 0.0f);
    glTexCoord2f(0.5f, 1.0f); glVertex3f(-0.4f, y - 0.4f, 0.0f);
    glEnd();

    glDisable(GL_TEXTURE_2D);

    // Material para el lado derecho inferior
    GLfloat mat1[] = {0.0f, 1.0f, 0.0f, 1.0f}; // Verde
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, mat1);
    glBegin(GL_TRIANGLES);
    glVertex3f(-0.3f, y - 0.8f, 0.0f);
    glVertex3f(0.0f, y - 0.4f, 0.0f);
    glVertex3f(-0.1f, y - 0.8f, 0.0f);
    glEnd();

    // Material para el lado derecho superior
    GLfloat mat2[] = {1.0f, 0.0f, 0.0f, 1.0f}; // Rojo
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, mat2);
    glBegin(GL_TRIANGLES);
    glVertex3f(0.0f, y - 0.4f, 0.0f);
    glVertex3f(0.3f, y, 0.0f);
    glVertex3f(0.2f, y - 0.4f, 0.0f);
    glEnd();
}

// Función para animar la caída libre
void animarCaidaLibre() {
    if (animando) {
        t += 0.016f; // Simula 60 FPS
        y = altura_inicial - (0.5f * g * t * t);

        if (y <= 0.0f) { // Rebote
            t = 0.0f;
            altura_inicial *= 0.7f; // Reduce la altura por el rebote
            if (altura_inicial < 0.1f) { // Umbral mínimo para detener
                animando = false;
                altura_inicial = 2.0f; // Reinicia altura
                y = altura_inicial;
            }
        }
    }
}

// Función de renderizado
void render() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    animarCaidaLibre();
    dibujarLetraW();
}

// Función para manejar las teclas
void teclado(GLFWwindow *window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_SPACE && action == GLFW_PRESS) { // Barra espaciadora
        animando = true;
    }
}

// Configuración inicial de OpenGL
void inicializar() {
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    GLfloat luzDifusa[] = {1.0f, 1.0f, 1.0f, 1.0f};
    GLfloat posicionLuz[] = {0.0f, 5.0f, 5.0f, 1.0f};
    glLightfv(GL_LIGHT0, GL_DIFFUSE, luzDifusa);
    glLightfv(GL_LIGHT0, GL_POSITION, posicionLuz);

    cargarTextura("images/escudo_unam.jpg");
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
}

// Configuración de GLFW y GLEW
int main() {
    if (!glfwInit()) {
        std::cerr << "Error al inicializar GLFW" << std::endl;
        return -1;
    }

    GLFWwindow *window = glfwCreateWindow(800, 600, "Letra W con Textura, Material y Animación", NULL, NULL);
    if (!window) {
        std::cerr << "Error al crear la ventana GLFW" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        std::cerr << "Error al inicializar GLEW" << std::endl;
        return -1;
    }

    inicializar();
    glfwSetKeyCallback(window, teclado);

    while (!glfwWindowShouldClose(window)) {
        render();
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
