#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Shaders.h"
#include "Model.h"
#include "Texture.h"
/*Este proyecto consiste en la visualización de un escenario libre, este escenario libre se trata de una cocina,
 * por la noche, donde unicamente se ha programado un microondas con un pollo dentro, con las teclas que se ven en el pdf se podra
 * manipular en su mayoria, se ha programado la iluminacion normal y dinamica, transparencias y un sistema de particulas.
 * La iluminacion reacciona con el boton de arriba si la giramos a la derecha o a la izquierda, al igual que con el boton de abajo
 * pero para el sistema de particulas.
 *ALUMNO: David Arévalo Rey
 */

//Funciones basicas para la programación en OPENGL
void configScene();
void renderScene();
void setLights (glm::mat4 P, glm::mat4 V);                                                  
void drawObjectMat(Model &model, Material material, glm::mat4 P, glm::mat4 V, glm::mat4 M);
void drawObjectTex(Model &model, Textures textures, glm::mat4 P, glm::mat4 V, glm::mat4 M);

//Funciones de interacciones (callbacks)
void funFramebufferSize(GLFWwindow* window, int width, int height);
void funKey            (GLFWwindow* window, int key  , int scancode, int action, int mods);
void funTimer          (double seconds, double &t0);
void funScroll         (GLFWwindow* window, double xoffset, double yoffset);                
void funCursorPos      (GLFWwindow* window, double xpos, double ypos);                      
float limite(float value, float min, float max);

//funciones para skybox
void initSkybox();
void renderSkybox(glm::mat4 P, glm::mat4 V);
unsigned int createNightCubeMap();


// Shaders
   Shaders shaders;

   Shaders skyboxShaders;
   unsigned int skyboxVAO = 0;
   unsigned int skyboxVBO = 0;
   unsigned int skyboxCubemap = 0;

// Modelo del microondas
   Model cuerpo; //Carcasa del microondas
   Model puerta; //Puerta movil
   Model plato; //Plato giratorio
   Model botonUp; // Boton superior
   Model botonDown; //Boton inferior
// Modelo de la cocina pared y encimera
   Model suelo;
   Model pared;
// Modelo del pollo
   Model chicken;
// Modelo lampara articulada
   Model lampBase;
   Model lampArm1;
   Model lampArm2;
   Model lampJoint;
   Model lampHead;
//Modelo licuadora
   Model blender;



// Imagenes (texturas)

// Textura microondas
   Texture imgMicro;
   Textures texturaMicro;
// Textura suelo o encimera
   Texture imgSuelo;
   Textures texturesSuelo;
   Texture imgSueloNormal;
// Textura del pollo
   Texture imgChicken;
   Textures texturesChicken;




// Luces y materiales
   #define   NLD 1          
   #define   NLP 1          
   #define   NLF 2
   Light     lightG;         //Luz global
   Light     lightD[NLD];   //Luz direccional del techo
   Light     lightP[NLP];   //Luz posicional
   Light     lightF[NLF];   //Luz focal
   Material  mluz;
   Material crema;

   Material metalLamp;
   Material lampBaseMat;
   Material lampArmMat;
   Material lampHeadMat;
   Material lampJointMat;
   Material blenderMat;


// Viewport
   int w = 500;
   int h = 500;

// variables de estado que se emplean para las animaciones

   bool cocinando = false; //permite saber si se esta cocinando
   bool puertaAbierta = false; //si la puerta esta abierta
   bool camaraInterior = false; //permite ver desde dentro del microondas
   bool focoTecho = false; //permite acitvar y desactivar el foco del techo
   bool lampOn = false; //enciende y apaga la lampara
   bool camaraPresentacion = false; //nueva camara frontal

// Variables de angulos y tiempos que necesitamos
   float angleDoor = 0.0f;//angulo actual de la puerta
   float anglePlate = 0.0f; //angulo actual del plato
// Movimiento de la luz
   float lightMove = 0.0f; //animación de luces
// Posicion para rotar los botones
   float rotBotonUp = 0.0f; //angulo rueda superior
   float rotBotonDown = 0.0f; //angulo rueda ingerior
// Limites y avances de rotaciones
   const float ROT_MIN = -160.0f;
   const float ROT_MAX = 135.0f;
   const float ROT_STEP = 5.0f;


//velocidad de animaciones
   const float speedDoor = 2.0f;
   const float speedPlate = 5.0f;


// Movimiento de camara y camara orbital
   float fovy   = 60.0; 
   float alphaX =  0.0; 
   float alphaY =  0.0;

//Angulos de lampara articulada
   float lampBaseAngle = -90.0f;
   float lampArm1Angle = 48.0f;
   float lampArm2Angle = -70.0f;
   float lampHeadAngle = -20.0f;



int main() {

 // Inicializamos GLFW
    if(!glfwInit()) return -1;
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

 // Creamos la ventana
    GLFWwindow* window;
    window = glfwCreateWindow(w, h, "Practica: David Arévalo Rey", NULL, NULL);
    if(!window) {
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

 // Inicializamos GLEW
    glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    if(GLEW_OK != err) {
        std::cout << "Error: " << glewGetErrorString(err) << std::endl;
        return -1;
    }
    std::cout << "Status: Using GLEW " << glewGetString(GLEW_VERSION) << std::endl;
    const GLubyte *oglVersion = glGetString(GL_VERSION);
    std::cout <<"This system supports OpenGL Version: " << oglVersion << std::endl;

 // Configuramos los CallBacks
    glfwSetFramebufferSizeCallback(window, funFramebufferSize);
    glfwSetKeyCallback(window, funKey);
    glfwSetScrollCallback   (window, funScroll);    
    glfwSetCursorPosCallback(window, funCursorPos); 

 // Entramos en el bucle de renderizado
    configScene();
    double t0 = glfwGetTime();
    while(!glfwWindowShouldClose(window)) {
        renderScene();
        glfwSwapBuffers(window);
        glfwPollEvents();
        funTimer(0.01,t0);
    }
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}

void configScene() {

 // Test de profundidad, habilitamos z buffer
    glEnable(GL_DEPTH_TEST);

 // Transparencias, habilitamos mezcla de colores para permitir la transparencia del humo
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

 // Shaders
    shaders.initShaders("resources/shaders/vshader.glsl","resources/shaders/fshader.glsl");

   skyboxShaders.initShaders("resources/shaders/skybox_vshader.glsl","resources/shaders/skybox_fshader.glsl");

   initSkybox();
   glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

 // Modelos
   pared.initModel("resources/models/cube.obj");
   cuerpo.initModel("resources/models/cuerpo.obj");
   plato.initModel("resources/models/platos.obj");
   puerta.initModel("resources/models/puertas.obj");
   botonDown.initModel("resources/models/RuedaDown.obj");
   botonUp.initModel("resources/models/RuedaUp.obj");
   suelo.initModel("resources/models/cube.obj");
   chicken.initModel("resources/models/chicken.obj");

   lampBase.initModel("resources/models/cylinder.obj");
   lampArm1.initModel("resources/models/cube.obj");
   lampArm2.initModel("resources/models/cube.obj");
   lampJoint.initModel("resources/models/sphere.obj");
   lampHead.initModel("resources/models/cylinder.obj");

   blender.initModel("resources/models/blender.obj");


 // Imagenes (texturas) y su respectiva configuración
   blenderMat.ambient = glm::vec4(0.08, 0.08, 0.08, 1.0);
   blenderMat.diffuse = glm::vec4(0.20, 0.20, 0.22, 1.0);
   blenderMat.specular = glm::vec4(0.45, 0.45, 0.50, 1.0);
   blenderMat.emissive = glm::vec4(0.0, 0.0, 0.0, 1.0);
   blenderMat.shininess = 40.0f;

   imgMicro.initTexture("resources/textures/microwave_col.jpg");

   texturaMicro.diffuse = imgMicro.getTexture();
   texturaMicro.specular = 0;
   texturaMicro.emissive = 0;
   texturaMicro.normal = 0;
   texturaMicro.shininess = 50.0;

   imgChicken.initTexture("resources/textures/chicken.jpg");

   texturesChicken.diffuse = imgChicken.getTexture();
   texturesChicken.specular = 0;
   texturesChicken.normal = 0;
   texturesChicken.emissive = 0.0;
   texturesChicken.normal = 0;

// Configuracion de las luces
 // Luz ambiental global
    lightG.ambient = glm::vec3(0.1, 0.1, 0.1); //La ponemos oscura para resaltar el microondas

 // Luces direccionales, va a ser la luz del techo
    lightD[0].direction = glm::vec3(0.0, -1.0, 0.0);
    lightD[0].ambient   = glm::vec3(0.05,  0.05, 0.05);
    lightD[0].diffuse   = glm::vec3(0.2,  0.2, 0.2);
    lightD[0].specular  = glm::vec3(0.3,  0.3, 0.3);

 // Luces posicionales, la bombilla de dentro
    lightP[0].position    = glm::vec3(0.0, 0.4, 0.0);
    lightP[0].ambient     = glm::vec3(0.0, 0.0, 0.0);
    lightP[0].diffuse     = glm::vec3(0.0, 0.0, 0.0);
    lightP[0].specular    = glm::vec3(0.0, 0.0, 0.0);
    lightP[0].c0          = 1.00;
    lightP[0].c1          = 0.1;
    lightP[0].c2          = 0.1;

 // Luces focales
    lightF[0].position    = glm::vec3( 0.0,  4.5,  1.5);
    lightF[0].direction   = glm::normalize(glm::vec3(0.0, -1.0, -0.3));
    lightF[0].ambient     = glm::vec3( 0.2,  0.2,  0.2);
    lightF[0].diffuse     = glm::vec3( 1.0,  1.0,  0.9);
    lightF[0].specular    = glm::vec3( 1.0);
    lightF[0].innerCutOff = 18.0;
    lightF[0].outerCutOff = 28.0;
    lightF[0].c0          = 1.000;
    lightF[0].c1          = 0.090;
    lightF[0].c2          = 0.032;


   //luz focal de la lampara articulada
   lightF[1].position    = glm::vec3(0.0, 0.0, 0.0);
   lightF[1].direction   = glm::normalize(glm::vec3(-1.0, -0.2, 0.0));
   lightF[1].ambient     = glm::vec3(0.0, 0.0, 0.0);
   lightF[1].diffuse     = glm::vec3(0.0, 0.0, 0.0);
   lightF[1].specular    = glm::vec3(0.0, 0.0, 0.0);
   lightF[1].innerCutOff = 16.0;
   lightF[1].outerCutOff = 24.0;
   lightF[1].c0          = 1.000;
   lightF[1].c1          = 0.090;
   lightF[1].c2          = 0.032;


 // Materiales base
    mluz.ambient   = glm::vec4(0.0, 0.0, 0.0, 1.0);
    mluz.diffuse   = glm::vec4(0.0, 0.0, 0.0, 1.0);
    mluz.specular  = glm::vec4(0.0, 0.0, 0.0, 1.0);
    mluz.emissive  = glm::vec4(1.0, 1.0, 1.0, 1.0);
    mluz.shininess = 1.0;

   crema.ambient = glm::vec4(0.4, 0.4, 0.35, 1.0);
   crema.diffuse  = glm::vec4(0.95, 0.92, 0.85, 1.0);
   crema.specular = glm::vec4(0.1, 0.1, 0.1, 1.0);
   crema.emissive = glm::vec4(0.0, 0.0, 0.0, 1.0);
   crema.shininess = 10.0;

   metalLamp.ambient = glm::vec4(0.25, 0.25, 0.28, 1.0);
   metalLamp.diffuse = glm::vec4(0.7, 0.70, 0.75, 1.0);
   metalLamp.specular = glm::vec4(0.9, 0.9, 0.95, 1.0);
   metalLamp.emissive = glm::vec4(0.0, 0.0, 0.0, 1.0);
   metalLamp.shininess = 80.0;

   lampBaseMat.ambient = glm::vec4(0.05, 0.05, 0.05, 1.0);
   lampBaseMat.diffuse = glm::vec4(0.12, 0.12, 0.12, 1.0);
   lampBaseMat.specular = glm::vec4(0.25, 0.25, 0.25, 1.0);
   lampBaseMat.emissive = glm::vec4(0.0, 0.0, 0.0, 1.0);
   lampBaseMat.shininess = 40.0;

   lampArmMat.ambient = glm::vec4(0.10, 0.10, 0.12, 1.0);
   lampArmMat.diffuse = glm::vec4(0.22, 0.55, 0.60, 1.0);
   lampArmMat.specular = glm::vec4(0.45, 0.45, 0.50, 1.0);
   lampArmMat.emissive = glm::vec4(0.0, 0.0, 0.0, 1.0);
   lampArmMat.shininess = 60.0;

   lampJointMat.ambient = glm::vec4(0.18, 0.18, 0.20, 1.0);
   lampJointMat.diffuse = glm::vec4(0.55, 0.55, 0.6, 1.0);
   lampJointMat.specular = glm::vec4(0.85, 0.85, 0.90, 1.0);
   lampJointMat.emissive = glm::vec4(0, 0, 0, 1.0);
   lampJointMat.shininess = 90.0;

   lampHeadMat.ambient = glm::vec4(0.12, 0.12, 0.13, 1.0);
   lampHeadMat.diffuse = glm::vec4(0.35, 0.35, 0.38, 1.0);
   lampHeadMat.specular = glm::vec4(0.85, 0.85, 0.90, 1.0);
   lampHeadMat.emissive = glm::vec4(0, 0, 0, 1.0);
   lampHeadMat.shininess = 100.0;


   imgSuelo.initTexture("resources/textures/marmolBlanco.jpg");
   imgSueloNormal.initTexture("resources/textures/marmolBlanco_normal.png");

   texturesSuelo.diffuse   = imgSuelo.getTexture();
   texturesSuelo.specular  = 0;
   texturesSuelo.emissive  = 0;
   texturesSuelo.normal    = imgSueloNormal.getTexture();
   texturesSuelo.shininess = 25.0f;

   texturaMicro.normal   = 0;
   texturesChicken.normal = 0;


}

void renderScene() {

 // Borramos el buffer de color
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

 // Indicamos los shaders a utilizar
    shaders.useShaders();

 // Matriz P                                                         
    float nplane =  0.1;
    float fplane = 50.0;
    float aspect = (float)w/(float)h;
    glm::mat4 P = glm::perspective(glm::radians(fovy), aspect, nplane, fplane);

 // Matriz V

    glm::vec3 eye;
    glm::vec3 center;
    glm::vec3 up    (0.0, 1.0,  0.0);
//Esta sera la logica para alternar entre la camara interior y la exterior
   if (camaraPresentacion) {
      eye = glm::vec3(0.0, 3.0, 11.5);
      center = glm::vec3(0.0, 1.0, 0.5);

   }else if (camaraInterior){
      //camara fija en el interior del microondas en una esquina
      eye = glm::vec3(-1.8f, 1.0f, 1.5f);
      center = glm::vec3(-0.3f, 0.1f, -0.3f);

   }else {
      //camara orbital controlada por el raton
      float x = 10.0f*glm::cos(glm::radians(alphaY))*glm::sin(glm::radians(alphaX));
      float y = 10.0f*glm::sin(glm::radians(alphaY));
      float z = 10.0f*glm::cos(glm::radians(alphaY))*glm::cos(glm::radians(alphaX));
      eye = glm::vec3(x, y, z);
      center = glm::vec3(0, 0, 0);

   }

    glm::mat4 V = glm::lookAt(eye, center, up);
    shaders.setVec3("ueye",eye);

   //Calculo del control
   //Se ha convertido el angulo de los botones en un valor entre 0 y 1.
   //El factor potencia controla la intensidad de la luz y pertenece al boton de arriba
   float factorPotencia = 1.0f - ((rotBotonUp- ROT_MIN) / (ROT_MAX - ROT_MIN));
   factorPotencia = limite(factorPotencia, 0.0f, 1.0f);
   //El factor humo controla las particulas del humo y pertenece al boton de abajo
   float factorHumo = 1.0f - ((rotBotonDown - ROT_MIN) / (ROT_MAX - ROT_MIN));
   factorHumo = limite(factorHumo, 0.0f, 1.0f);

   //Luz al cocinar, es la logica de la ilumicacion con animaciones principal

   if (cocinando || puertaAbierta) {
      float maxIntensidad = 5.0f; // la intensidad depende de cuanto giremos la rueda de arriba

      float intensidad = maxIntensidad * factorPotencia;
      if (cocinando) {
         //si esta cocinando es el efecto de parpadeo
         float parpadeo = 0.7f + 0.3f * sin(glfwGetTime() * 1.0f);
         intensidad *= parpadeo;
      }
      //aplicamos un color calido con la intensidad
      lightP[0].diffuse = glm::vec3(1.0f, 0.95f, 0.8f) * intensidad;
      lightP[0].specular = glm::vec3(1.0f, 0.95f, 0.8f) * intensidad;
      lightP[0].ambient = glm::vec3(0.4f, 0.35f, 0.2f) * intensidad;
   }else {
      //luz apagada
      lightP[0].diffuse = glm::vec3(0.0, 0.0, 0.0);
      lightP[0].ambient = glm::vec3(0.0, 0.0, 0.0);
      lightP[0].specular = glm::vec3(0.0, 0.0, 0.0);
   }

   //Control de la luz del techo
   if (focoTecho) {
      lightF[0].diffuse = glm::vec3(1.0f, 1.0f, 0.9f);
      lightF[0].specular = glm::vec3(1.0f, 1.0f, 1.0f);
      lightF[0].innerCutOff= 10.0f;
      lightF[0].outerCutOff = 15.0f;

   }else {
      lightF[0].diffuse = glm::vec3(0.0f, 0.0f, 0.0f);
      lightF[0].specular = glm::vec3(0.0f, 0.0f, 0.0f);
   }



 // Dibujamos la escena
   //Son los pivotes para los botones, para evitar que cuando se giren se vayan de la escena y del lugar de hecho
   glm::vec3 hingeBotonUp = glm::vec3(1.90f, 1.40f, 1.65f);
   glm::vec3 hingeBotonDown = glm::vec3(1.92f, 0.66f, 1.70f);


 //Dibujamos la encimera

   glm::mat4 Msuelo = glm::translate(glm::mat4(1.0F), glm::vec3(0.0f, -1.0f, 0.0f));
   Msuelo = glm::scale(Msuelo, glm::vec3(10.0f, 1.0f, 10.0f));
   drawObjectTex(suelo, texturesSuelo, P, V, Msuelo);

   //medidas de la lampara
   float baseRadius = 0.42f;
   float baseHalfH = 0.15;
   float jointR = 0.09f;
   float armThickness = 0.08f;
   float arm1Half = 0.68f;
   float arm2Half = 0.56f;

   float headRadius = 0.14f;
   float headHalfLen = 0.34f;

   //posicion del centro de la base
   glm::vec3 lampPos = glm::vec3(4.25f, baseHalfH, 1.45f);
   //raiz base + giro de la base
   glm::mat4 MbaseRoot = glm::mat4(1.0f);
   MbaseRoot = glm::translate(MbaseRoot, lampPos);
   MbaseRoot = glm::rotate(MbaseRoot, glm::radians(lampBaseAngle), glm::vec3(0.0f, 1.0f, 0.0f));
   //base
   glm::mat4 MLampBase = MbaseRoot;
   MLampBase = glm::scale(MLampBase, glm::vec3(baseRadius, baseHalfH, baseRadius));
   drawObjectMat(lampBase, lampBaseMat, P, V, MLampBase);
   //pivote superior de la base
   glm::mat4 Mpivot1 = MbaseRoot;
   Mpivot1 = glm::translate(Mpivot1, glm::vec3(0.0f, baseHalfH, 0.0f));

   //Union
   glm::mat4 Mjoint1 = Mpivot1;
   Mjoint1 = glm::scale(Mjoint1, glm::vec3(jointR, jointR, jointR));
   drawObjectMat(lampJoint, lampJointMat, P, V, Mjoint1);
   //brazo1
   glm::mat4 Marm1Pivot = Mpivot1;
   Marm1Pivot = glm::rotate(Marm1Pivot, glm::radians(lampArm1Angle), glm::vec3(0.0f, 0.0f, 1.0f));

   glm::mat4 MlampArm1 = Marm1Pivot;
   MlampArm1 = glm::translate(MlampArm1, glm::vec3(0.0f, arm1Half, 0.0f));
   MlampArm1 = glm::scale(MlampArm1, glm::vec3(armThickness, arm1Half, armThickness));
   drawObjectMat(lampArm1, lampArmMat, P, V, MlampArm1);

   //pivote al final del brazo 1
   glm::mat4 Mpivot2 = Marm1Pivot;
   Mpivot2 = glm::translate(Mpivot2, glm::vec3(0.0f, 2.0f * arm1Half, 0.0f));
   //union 2
   glm::mat4 Mjoint2 = Mpivot2;
   Mjoint2 = glm::scale(Mjoint2, glm::vec3(jointR, jointR, jointR));
   drawObjectMat(lampJoint, lampJointMat, P, V, Mjoint2);

   //brazo 2
   glm::mat4 Marm2Pivot = Mpivot2;
   Marm2Pivot = glm::rotate(Marm2Pivot, glm::radians(lampArm2Angle), glm::vec3(0.0f, 0.0f, 1.0f));

   glm::mat4 MlampArm2 = Marm2Pivot;
   MlampArm2 = glm::translate(MlampArm2, glm::vec3(0.0f, arm2Half, 0.0f));
   MlampArm2 = glm::scale(MlampArm2, glm::vec3(armThickness, arm2Half, armThickness));
   drawObjectMat(lampArm2, lampArmMat, P, V, MlampArm2);

   //pivote al final del brazo 2
   glm::mat4 Mpivot3 = Marm2Pivot;
   Mpivot3 = glm::translate(Mpivot3, glm::vec3(0.0f, 2.0f * arm2Half, 0.0f));

   //union3

   glm::mat4 Mjoint3 = Mpivot3;
   Mjoint3 = glm::scale(Mjoint3, glm::vec3(0.045f, 0.045f, 0.045f));
   drawObjectMat(lampJoint, lampJointMat, P, V, Mjoint3);
   //Foco
   glm::mat4 MHeadPivot = Mpivot3;
   MHeadPivot = glm::rotate(MHeadPivot, glm::radians(lampHeadAngle), glm::vec3(0.0f, 0.0f, 1.0f));

   glm::mat4 MlampHead = MHeadPivot;
   MlampHead = glm::translate(MlampHead, glm::vec3(headHalfLen, 0.0f, 0.0f));
   MlampHead = glm::rotate(MlampHead, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
   MlampHead = glm::scale(MlampHead, glm::vec3(headRadius, headHalfLen, headRadius));

   glm::vec3 lampBack = glm::vec3(MlampHead * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
   glm::vec3 lampFront = glm::vec3(MlampHead * glm::vec4(0.0f, -1.0f, 0.0f, 1.0f));

   glm::vec3 lampDir = glm::normalize(lampFront - lampBack);

   glm::vec3 lampSpotPos =lampFront;

   lightF[1].position = lampSpotPos;
   lightF[1].direction = lampDir;

   if (lampOn) {
      lightF[1].ambient = glm::vec3(0.03f, 0.03f, 0.02f);
      lightF[1].diffuse = glm::vec3(1.0f, 0.95f, 0.85f) * 3.0f;
      lightF[1].specular = glm::vec3(1.0f, 0.98f, 0.95f) * 2.2f;
   }else {
      lightF[1].ambient = glm::vec3(0, 0, 0);
      lightF[1].diffuse = glm::vec3(0, 0, 0);
      lightF[1].specular = glm::vec3(0, 0, 0);
   }

   setLights(P, V);

   Material metalLampOn = lampHeadMat;


   if (lampOn) {
      metalLampOn.emissive = glm::vec4(0.2f, 0.18f, 0.1f, 1.0f);
      drawObjectMat(lampHead, metalLampOn, P, V, MlampHead);

   }else {
      drawObjectMat(lampHead, lampHeadMat, P, V, MlampHead);
   }
// Dibujamos el cuerpo del microondas
   glm::mat4 M = I;
   drawObjectTex(cuerpo, texturaMicro, P, V, M);

// Dibujamos la pared de al fondo

   glm::mat4 Mpared = glm::mat4(1.0F);
   Mpared = glm::translate(Mpared, glm::vec3(0.0f, 4.0f, -5.0f));
   Mpared = glm::scale(Mpared, glm::vec3(10.0f, 10.0f, 0.5f));
   drawObjectMat(pared, crema, P, V, Mpared);

   //dibujamos el blender
   glm::mat4 Mblender = glm::mat4(1.0f);
   Mblender = glm::translate(Mblender, glm::vec3(-4.3f, -2.0f, 1.2f));
   Mblender = glm::rotate(Mblender, glm::radians(90.0f),glm::vec3(1.0f, 0.0f, 0.0f));
   Mblender = glm::rotate(Mblender, glm::radians(180.0f),glm::vec3(0.0f, 1.0f, 0.0f));
   Mblender = glm::scale(Mblender, glm::vec3(0.1f));

   drawObjectMat(blender, blenderMat, P, V, Mblender);


   //Dibujamos el plato giratorio, animado con angleplate

   glm::mat4 Mplato = glm::mat4(1.0f);

   Mplato = glm::translate(Mplato, glm::vec3(-0.3f, 0.3f, 0.0f));

   Mplato = glm::rotate(Mplato, glm::radians(anglePlate), glm::vec3(0, 1, 0));

   drawObjectTex(plato, texturaMicro, P, V, Mplato);

   //Dibujamos el pollo y lo situamos donde el plato

   glm::mat4 Mchicken = Mplato;
   Mchicken = glm::translate(Mchicken, glm::vec3(-0.25f, 0.1f, 0.0f));
   Mchicken = glm::rotate(Mchicken, glm::radians(-90.0f), glm::vec3(0, 0, 1));
   Mchicken = glm::scale(Mchicken, glm::vec3(0.005f));
   drawObjectTex(chicken, texturesChicken, P, V, Mchicken);


   //Dibujamos los botones

   glm::mat4 MbotonUp = glm::mat4(1.0f);
   MbotonUp = glm::translate(MbotonUp, hingeBotonUp);
   MbotonUp = glm::rotate(MbotonUp, glm::radians(rotBotonUp), glm::vec3(0, 0, 1));
   drawObjectTex(botonUp, texturaMicro, P, V, MbotonUp);

   glm::mat4 MbotonDown = glm::mat4(1.0f);
   MbotonDown = glm::translate(MbotonDown, hingeBotonDown);
   MbotonDown = glm::rotate(MbotonDown, glm::radians(rotBotonDown), glm::vec3(0, 0, 1));
   drawObjectTex(botonDown, texturaMicro, P, V, MbotonDown);

   //Dibujamos la puerta y la animamos con angleDoor
   glm::mat4 Mdoor = glm::mat4(1.0);

   glm::vec3 hingePos = glm::vec3(-2.25f, 0.0f, 1.70f);

   Mdoor = glm::translate(Mdoor, hingePos);

   Mdoor = glm::rotate(Mdoor, glm::radians(-angleDoor), glm::vec3(0, 1, 0));

   Mdoor = glm::translate(Mdoor, -hingePos);

   drawObjectTex(puerta, texturaMicro, P, V, Mdoor);

   //Este es el sistema de particulas y solo se ve si el microondas esta en funcionamiento

   if (cocinando) {
      Material matVapor; //creamos un material casi transparente
      matVapor.ambient = glm::vec4(1.0, 1.0, 1.0, 0.3);
      matVapor.diffuse = glm::vec4(1.0, 1.0, 1.0, 0.3);
      matVapor.specular = glm::vec4(0.0, 0.0, 0.0, 0.0);
      matVapor.emissive = glm::vec4(0.5, 0.5, 0.5, 1.0); //este material brilla en la oscuridad
      matVapor.shininess = 1.0;

      glDepthMask(GL_FALSE); //Desactivamos para que se transparente
      //cantidad de humo controlada por la rueda de abajo
      int maxParticulas = 30;
      int numParticulas = (int)(maxParticulas * factorHumo);
      for (int i = 0; i < numParticulas; i++) {
         float tiempo = glfwGetTime();
         //calculamos la subida ciclica con el resto
         float altura = fmod(tiempo * 1.5f + i * 0.2f, 2.0f );
         //calculamso la oscilacion
         float oscilacion = sin(tiempo * 3.0f + i) * 0.05f;
         //Las particulas suben del plato y suben hasta el techo del microondas
         glm::mat4 Mvapor = Mplato;
         Mvapor = glm::translate(Mvapor, glm::vec3(-0.25f +oscilacion, altura + 0.1f, 0.0f));
         Mvapor = glm::rotate(Mvapor, glm::radians(tiempo * 60.0f + i * 20.0f), glm::vec3(1, 1, 1));
         //Con esta variable crecen a medida que suban
         float scala = 0.03f + (altura * 0.02f);
         Mvapor = glm::scale(Mvapor, glm::vec3(scala));
         //Se dibuja practicamente un cubo para que suba
         drawObjectMat(pared, matVapor, P, V, Mvapor);
      }
      glDepthMask(GL_TRUE); //Restaurameos el buffer
   }

   renderSkybox(P, V);

}

void setLights(glm::mat4 P, glm::mat4 V) {  

    shaders.setLight("ulightG",lightG);
    for(int i=0; i<NLD; i++) shaders.setLight("ulightD["+toString(i)+"]",lightD[i]);
    for(int i=0; i<NLP; i++) shaders.setLight("ulightP["+toString(i)+"]",lightP[i]);
    for(int i=0; i<NLF; i++) shaders.setLight("ulightF["+toString(i)+"]",lightF[i]);

}

void drawObjectMat(Model &model, Material material, glm::mat4 P, glm::mat4 V, glm::mat4 M) {  

    shaders.setMat4("uN"  ,glm::transpose(glm::inverse(M)));
    shaders.setMat4("uM"  ,M);
    shaders.setMat4("uPVM",P*V*M);
    shaders.setBool("uWithMaterials",true);
    shaders.setMaterial("umaterial",material);
    model.renderModel(GL_FILL);

}

void drawObjectTex(Model &model, Textures textures, glm::mat4 P, glm::mat4 V, glm::mat4 M) {

    shaders.setMat4("uN"  ,glm::transpose(glm::inverse(M)));
    shaders.setMat4("uM"  ,M);
    shaders.setMat4("uPVM",P*V*M);
    shaders.setBool("uWithMaterials",false);
    shaders.setTextures("utextures",textures);
    if(textures.normal!=0) shaders.setBool("uWithNormals",true);
    else                   shaders.setBool("uWithNormals",false);
    model.renderModel(GL_FILL);

}


void funFramebufferSize(GLFWwindow* window, int width, int height) {

 // Configuracion del Viewport
    glViewport(0, 0, width, height);

 // Actualizacion de w y h
    w = width;
    h = height;

}

void initSkybox() {

   float skyboxVertices[] = {
      -1.0f,  1.0f, -1.0f,
      -1.0f, -1.0f, -1.0f,
       1.0f, -1.0f, -1.0f,
       1.0f, -1.0f, -1.0f,
       1.0f,  1.0f, -1.0f,
      -1.0f,  1.0f, -1.0f,

      -1.0f, -1.0f,  1.0f,
      -1.0f, -1.0f, -1.0f,
      -1.0f,  1.0f, -1.0f,
      -1.0f,  1.0f, -1.0f,
      -1.0f,  1.0f,  1.0f,
      -1.0f, -1.0f,  1.0f,

       1.0f, -1.0f, -1.0f,
       1.0f, -1.0f,  1.0f,
       1.0f,  1.0f,  1.0f,
       1.0f,  1.0f,  1.0f,
       1.0f,  1.0f, -1.0f,
       1.0f, -1.0f, -1.0f,

      -1.0f, -1.0f,  1.0f,
      -1.0f,  1.0f,  1.0f,
       1.0f,  1.0f,  1.0f,
       1.0f,  1.0f,  1.0f,
       1.0f, -1.0f,  1.0f,
      -1.0f, -1.0f,  1.0f,

      -1.0f,  1.0f, -1.0f,
       1.0f,  1.0f, -1.0f,
       1.0f,  1.0f,  1.0f,
       1.0f,  1.0f,  1.0f,
      -1.0f,  1.0f,  1.0f,
      -1.0f,  1.0f, -1.0f,

      -1.0f, -1.0f, -1.0f,
      -1.0f, -1.0f,  1.0f,
       1.0f, -1.0f, -1.0f,
       1.0f, -1.0f, -1.0f,
      -1.0f, -1.0f,  1.0f,
       1.0f, -1.0f,  1.0f
  };

   glGenVertexArrays(1, &skyboxVAO);
   glGenBuffers(1, &skyboxVBO);

   glBindVertexArray(skyboxVAO);
   glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
   glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), skyboxVertices, GL_STATIC_DRAW);

   glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
   glEnableVertexAttribArray(0);

   glBindVertexArray(0);

   skyboxCubemap = createNightCubeMap();
}

unsigned int createNightCubeMap() {

   unsigned int textureID;
   glGenTextures(1, &textureID);
   glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

   unsigned char posX[3] = { 12, 16, 30 };
   unsigned char negX[3] = { 10, 14, 26 };
   unsigned char posY[3] = { 20, 28, 55 };
   unsigned char negY[3] = { 18, 18, 24 };
   unsigned char posZ[3] = { 11, 15, 28 };
   unsigned char negZ[3] = {  9, 12, 22 };

   glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGB, 1, 1, 0, GL_RGB, GL_UNSIGNED_BYTE, posX);
   glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_RGB, 1, 1, 0, GL_RGB, GL_UNSIGNED_BYTE, negX);
   glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_RGB, 1, 1, 0, GL_RGB, GL_UNSIGNED_BYTE, posY);
   glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_RGB, 1, 1, 0, GL_RGB, GL_UNSIGNED_BYTE, negY);
   glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_RGB, 1, 1, 0, GL_RGB, GL_UNSIGNED_BYTE, posZ);
   glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_RGB, 1, 1, 0, GL_RGB, GL_UNSIGNED_BYTE, negZ);

   glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
   glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
   glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
   glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
   glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

   return textureID;
}

void renderSkybox(glm::mat4 P, glm::mat4 V) {

   glDepthFunc(GL_LEQUAL);
   glDepthMask(GL_FALSE);

   skyboxShaders.useShaders();

   glm::mat4 Vsky = glm::mat4(glm::mat3(V));

   skyboxShaders.setMat4("uP", P);
   skyboxShaders.setMat4("uV", Vsky);
   skyboxShaders.setInt("uSkybox", 0);

   glBindVertexArray(skyboxVAO);
   glActiveTexture(GL_TEXTURE0);
   glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxCubemap);
   glDrawArrays(GL_TRIANGLES, 0, 36);
   glBindVertexArray(0);

   glDepthMask(GL_TRUE);
   glDepthFunc(GL_LESS);
}

void funKey(GLFWwindow* window, int key  , int scancode, int action, int mods) {

   //Deteccion de teclas

   if (action == GLFW_PRESS) {
      switch(key) {
         case GLFW_KEY_O:
            //O : Abrir y cerrar puertas, se apaga el microondas
            puertaAbierta = !puertaAbierta;
            if (puertaAbierta) cocinando = false;
            break;
         case GLFW_KEY_S:
            //S: Iniciar o parar, solo si la puerta esta cerrada
            if (!puertaAbierta) cocinando = !cocinando;
            break;
         case GLFW_KEY_ESCAPE:
            //ESC: Salir
            glfwSetWindowShouldClose(window, GLFW_TRUE);
            break;
         case GLFW_KEY_C:
            //C: Cambiar camara hacia dentro del microondas o hacia fuera
            if (camaraPresentacion) camaraPresentacion = false;
            camaraInterior = !camaraInterior;
            break;
         case GLFW_KEY_U:
            rotBotonUp += ROT_STEP;
            rotBotonUp = limite(rotBotonUp, ROT_MIN, ROT_MAX) ;
            break;
         case GLFW_KEY_J: // U/J Controlan la potencia
            rotBotonUp -= ROT_STEP;
            rotBotonUp = limite(rotBotonUp, ROT_MIN, ROT_MAX);
            break;
         case GLFW_KEY_I:
            rotBotonDown += ROT_STEP;
            rotBotonDown = limite(rotBotonDown, ROT_MIN, ROT_MAX);
            break;
         case GLFW_KEY_K: // I/K Controlan el humo
            rotBotonDown -= ROT_STEP;
            rotBotonDown = limite(rotBotonDown, ROT_MIN, ROT_MAX);
            break;
         case GLFW_KEY_F:
            focoTecho = !focoTecho;
            break;
         case GLFW_KEY_V:
            camaraPresentacion = !camaraPresentacion;
            if (camaraPresentacion) camaraInterior = false;
            break;
         case GLFW_KEY_1:
            lampBaseAngle += 5.0f;
            break;
         case GLFW_KEY_2:
            lampBaseAngle -= 5.0f;
            break;
         case GLFW_KEY_3:
            lampArm1Angle = limite(lampArm1Angle + 5.0f, 20.0f, 75);
            break;
         case GLFW_KEY_4:
            lampArm1Angle = limite(lampArm1Angle - 5.0f, 20.0f, 75.0f);
            break;
         case GLFW_KEY_5:
            lampArm2Angle = limite(lampArm2Angle + 5.0f, -85.0f, 10.0f);
            break;
         case GLFW_KEY_6:
            lampArm2Angle = limite(lampArm2Angle - 5.0f, -85.0f, 10.0f);
            break;
         case GLFW_KEY_7:
            lampHeadAngle = limite(lampHeadAngle + 5.0f, -70.0f, 25.0f);
            break;
         case GLFW_KEY_8:
            lampHeadAngle = limite(lampHeadAngle - 5.0f, -70.0f, 25.0f);
            break;
         case GLFW_KEY_L:
            lampOn = !lampOn;
            break;

         default:
            break;
      }

   }

}

void funTimer(double seconds, double &t0) {
   //Control de animaciones por tiempo
   double t1 = glfwGetTime();
   if (t1 - t0 > seconds) {
      if (cocinando) {
         anglePlate += speedPlate; //si estamos cocinando giramos el plato

      }

      if (puertaAbierta && angleDoor < 90.0f) { //animacion suave de apertura de puerta
         angleDoor += speedDoor;
         if (angleDoor >= 90.0f) angleDoor = 90.0f;
      }else if (!puertaAbierta && angleDoor > 00.0f) {
         angleDoor -= speedDoor;
         if (angleDoor < 00.0f) angleDoor = 00.0f;
      }

      lightMove += 0.05f; //luz auxiliar
      lightF[0].direction.x = sin(lightMove);
      t0 = t1;
   }
}

void funScroll(GLFWwindow* window, double xoffset, double yoffset) {    
   //zoom de la camara
    if(yoffset>0) fovy -= fovy>10.0f ? 5.0f : 0.0f;
    if(yoffset<0) fovy += fovy<90.0f ? 5.0f : 0.0f;

}

void funCursorPos(GLFWwindow* window, double xpos, double ypos) {       
   //control de la camara orbital con el raton
   float limiteLateral = 28.0f;
   alphaX = limiteLateral * (2.0 * xpos/ float(w) - 1.0);
   float rawAlphaY= 90.0 * (1.0 - 2.0 * ypos / float(h));
   alphaY = limite(rawAlphaY, 8.0f, 32.0f);

}
//Esta funcion se usa para limitar valores con un minimo y un maximo
float limite(float value, float min, float max) {
   return std::max(min, std::min(max, value));
}
