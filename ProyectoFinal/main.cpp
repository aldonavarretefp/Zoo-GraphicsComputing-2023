#include <iostream>
#include <cmath>
#include <array>
#include <random>
#include <algorithm>

//GLEW
#include <GL/glew.h>

//GLFW
#include <GLFW/glfw3.h>

//Other Libs
#include "stb_image.h"

//GLM Mathematics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//Load Models
#include "SOIL2/SOIL2.h"

//Other includes
#include "Shader.h"
#include "Camera.h"
#include "Model.h"
#include "Texture.h"

//Function prototypes
void KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mode);
void MouseCallback(GLFWwindow *window, double xPos, double yPos);
void DoMovement();
void animacion();
void animacionA();
void animateFish();
void animateJarron();

//Window dimensions
const GLuint WIDTH = 800, HEIGHT = 600;
int SCREEN_WIDTH, SCREEN_HEIGHT;

//Camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
GLfloat lastX = WIDTH / 2.0;
GLfloat lastY = HEIGHT / 2.0;
bool keys[1024];
bool firstMouse = true;

//Variables Anfibios
//AnimaciÛn bandera
float timeFlags = 0;
float speed = 0;
//AnimaciÛn puerta
bool animacionPuerta = false;
float rotDoor = 0;
bool dirDoor = true;
constexpr float VELOCIDAD_DOOR = 40.0;
//AnimaciÛn microscopio
bool animacionMicro = false;
float rotMicro = 0;
bool dirMicro = true;
constexpr float VELOCIDAD_MICRO = 100.0;
//animaciÛn ajolote
bool animacionAjolote = false;
float rotLeg = -20;
bool dirLegs = true;
float rotTail = 0;
bool dirTail = true;
//ruta ajolote
enum Direccion {
    ADELANTE,
    ATRAS,
    MOVIMIENTO_CIRCULAR_IDA,
    MOVIMIENTO_CIRCULAR_REGRESO
};
Direccion direccionAjolote = ADELANTE;
float movX = 0;
float movZ = 0;
float rotAjo = 0;
float xc = 0, zc = 0;
float angCir = 0;
//Constantes ajolote
constexpr auto VELOCIDAD_GIRO_AJOLOTE = 80;
constexpr auto VELOCIDAD_NADO_AJOLOTE = 2;
constexpr float radio = 1;
constexpr float VELOCIDAD_PATAS_AJOLOTE = 160;
constexpr float VELOCIDAD_COLA_AJOLOTE = 160;
//animacion tragaluz
bool animacionTragaluz = false;
bool dirTraga = true;
float movXTraga = 0;
//constantes traga
constexpr float VELOCIDAD_TRAGALUZ = 10;

//Variables acuario
float range = 0.0f;
float rot = 0.0f;
float movCamera = 0.0f;
float posDeltaBur1 = 0.0f, posDeltaBur2 = 0.0f;

float alphaVariation = 10.0f;

// Animaciones sencillas
float   tiempo            = 0,
        r                = 0.2f,
        posSillas        = 0,
        posZPezNaranja  = 0,
        posXPezNaranja  = 0,
        rotFish            = 0,
        rotFishLeft        = 0,
        rotSilla        = 0;

bool    changeDirectionFish   = false,
        openDoors = true,
        changeDirectionSillas = false;

float rotPuertaDer = 0.0f, rotPuertaIzq = 0.0f;


//AnimaciÛn del coche
float movKitX = 0.0;
float movKitZ = 0.0;
float movKitY = 0.0f;
float rotKitZ = 0, rotKitX = 0;
bool iniciaJarron = false;
bool recorrido1 = true;
bool recorrido2 = false;
bool recorrido3 = false;
bool recorrido4 = false;
bool recorrido5 = false;


//Light attributes
glm::vec3 lightPos(0.0f, 0.0f, 0.0f);

//Positions of the point lights
glm::vec3 pointLightPositions[] = {
//    glm::vec3(-2.836f,6.442f, 7.108f),
//    glm::vec3(-6.654f ,6.442f, -2.277f),
//    glm::vec3(3.133f,6.442f,-7.495f),
//    glm::vec3(7.633f,6.442f, 3.823f)
    glm::vec3(0,0,0),
    glm::vec3(0,0,0),
    glm::vec3(0,0,0),
    glm::vec3(0,0,0)
};

//Posicion Bancas
glm::vec3 bancasPositions[] = {
    glm::vec3(-5.748,0,29.927),
    glm::vec3(22.066,0,30.025),
    glm::vec3(-5.748,0,67.179),
    glm::vec3(22.403,0,67.003),
    glm::vec3(36.037,0,41.527),
    glm::vec3(34.608,0,54.846),
    glm::vec3(56.055,0,41.787),
    glm::vec3(56.155,0,54.708),
    glm::vec3(85.769,0,12.914),
    glm::vec3(102.153,0,12.981),
    glm::vec3(85.881,0,-37.694),
    glm::vec3(102.153,0,-37.5),
    glm::vec3(62.454,0,-84.961),
    glm::vec3(62.355,0,-67.867),
    glm::vec3(29.847,0,-84.961),
    glm::vec3(29.821,0,-67.867),
    glm::vec3(-1.882,0,-84.961),
    glm::vec3(-1.616,0,-67.867),
    glm::vec3(-33.708,0,-84.961),
    glm::vec3(-33.64,0,-67.867),
    glm::vec3(-96.05,0,-23.209),
    glm::vec3(-79.527,0,-23.705),
    glm::vec3(-96.05,0,-1.448),
    glm::vec3(-79.527,0,-1.511),
    glm::vec3(-42.004,0,42.099),
    glm::vec3(-41.498,0,55.156),
    glm::vec3(-19.889,0,41.686),
    glm::vec3(-19.842,0,54.949)
};
//Rotacion Bancas
float rotacionBancas[] = {0.0f,180.0f,0.0f,180.0f,-90.0f,90.0f,-90.0f,90.0f, 0.0f,180.0f,0.0f,180.0f,-90.0f,90.0f,-90.0f,90.0f,-90.0f,90.0f,-90.0f,90.0f,0.0f,180.0f,0.0f, 180.0f, -90.0f, 90.0f, -90.0f, 90.0f};

//Posicion Postes
glm::vec3 postesPositions[] = {
    glm::vec3(-6.422,0,21.326),
    glm::vec3(22.617,0,22.191),
    glm::vec3(-6.441,0,37.685),
    glm::vec3(22.617,0,38.579),
    glm::vec3(-6.422,0,58.643),
    glm::vec3(22.617,0,59.266),
    glm::vec3(26.395,0,41.972),
    glm::vec3(25.626,0,55.6),
    glm::vec3(46.546,0,41.972),
    glm::vec3(45.464,0,55.644),
    glm::vec3(66.651,0,41.972),
    glm::vec3(65.603,0,55.644),
    glm::vec3(85.54,0,20.81),
    glm::vec3(102.686,0,21.566),
    glm::vec3(85.828,0,3.961),
    glm::vec3(102.64,0,4.655),
    glm::vec3(85.828,0,-30.013),
    glm::vec3(102.64,0,-28.976),
    glm::vec3(85.828,0,-46.808),
    glm::vec3(102.64,0,-45.92),
    glm::vec3(45.464,0,-68.079),
    glm::vec3(46.231,0,-85.479),
    glm::vec3(13.723,0,-68.124),
    glm::vec3(14.491,0,-85.523),
    glm::vec3(-18.129,0,-68.079),
    glm::vec3(-17.361,0,-85.479),
    glm::vec3(-96.473,0,-12.762),
    glm::vec3(-79.894,0,-12.398),
    glm::vec3(-29.416,0,41.972),
    glm::vec3(-30.259,0,55.6),
    glm::vec3(-9.266,0,41.972),
    glm::vec3(-10.421,0,55.644)
};
//Rotacion Bancas
float rotacionPostes[] = {180,0,180,0,180,0,90,-90,90,-90,90,-90,180,0,180,0,180,0,180,0,-90,90,-90,90,-90,90,180,0,90,-90,90,-90};

//Posicion Arboles
glm::vec3 arbolesPositions[] = {
    glm::vec3(20.834,3.828,40.816),
    glm::vec3(20.834,3.828,73.596),
    glm::vec3(41.046,3.828,40.816),
    glm::vec3(42.418,3.828,73.596),
    glm::vec3(96.036,3.828,38.654),
    glm::vec3(96.036,3.828,71.433),
    glm::vec3(116.248,3.828,38.654),
    glm::vec3(117.62,3.828,71.433),
    glm::vec3(137.093,3.828,21.06),
    glm::vec3(175.923,3.828,19.75),
    glm::vec3(137.945,3.828,-28.05),
    glm::vec3(175.923,3.828,-28.05),
    glm::vec3(125.115,3.828,-90.384),
    glm::vec3(91.911,3.828,-90.384),
    glm::vec3(60.155,3.828,-90.384),
    glm::vec3(28.786,3.828,-90.384),
    glm::vec3(-50.452,3.828,-12.802),
    glm::vec3(-2.644,3.828,-13.783),
    glm::vec3(-49.98,3.828,9.219),
    glm::vec3(-2.679,3.828,6.471)
};

//Keyframes
glm::vec3 PosIni(0.0f, 0.0f, 0.0f);
float posX =PosIni.x, posY = PosIni.y, posZ = PosIni.z, rotRodIzq = 0, rotRodDer=0, rotBrazoIzq = 0, rotBrazoDer = 0, posYPanda = 0.0f;

#define MAX_FRAMES 9
int i_max_steps = 50;
int i_curr_steps = 0;
typedef struct _frame {
    //Variables para GUARDAR Key Frames
    float posX;        //Variable para PosicionX
    float posY;        //Variable para PosicionY
    float posZ;        //Variable para PosicionZ
    float incX;        //Variable para IncrementoX
    float incY;        //Variable para IncrementoY
    float incZ;        //Variable para IncrementoZ
    float rotRodIzq;
    float rotRodDer;
    float rotInc;
    float rotIncRD;
    float rotBrazoIzq;
    float rotIncBrazoIzq;
    float rotBrazoDer;
    float rotIncBrazoDer;
    float posYPanda;
    float incPosYPanda;
}FRAME;

//Arreglos de Frames
float aPosYPanda[9] = {0,0,0,0,0,0.0,1.5,1.5,0};
float aPosX[9] = {0,-1.5,-3,-4.5,-5.55,-5.55,-5.55,-5.55,-5.55};
float aPosY[9] = {0,0,0,0,0,0,0,0,0};
float aPosZ[9] = {0,0,0,0,0,0,0,0,0};
float aRotRodIzq[9] = {0,-15,15,-15,0,0,0,0,0};
float aRotRodDer[9] = {0,15,-15,15,0,0,0,0,0};
float aRotBrazoDer[9] = {0,-15,15,-15,0,50,90,90,0};
float aRotBrazoIzq[9] = {0,15,-15,15,0,50,90,90,0};

FRAME KeyFrame[MAX_FRAMES];
int FrameIndex = 9;
bool play = false;
int playIndex = 0;

void resetElements(void){
    posYPanda = KeyFrame[0].posYPanda;
    
    posX = KeyFrame[0].posX;
    posY = KeyFrame[0].posY;
    posZ = KeyFrame[0].posZ;

    rotRodIzq = KeyFrame[0].rotRodIzq;
    rotRodDer = KeyFrame[0].rotRodDer;

    rotBrazoIzq = KeyFrame[0].rotBrazoIzq;
    rotBrazoDer = KeyFrame[0].rotBrazoDer;
}

void interpolation(void){
    KeyFrame[playIndex].incX = (KeyFrame[playIndex + 1].posX - KeyFrame[playIndex].posX) / i_max_steps;
    KeyFrame[playIndex].incY = (KeyFrame[playIndex + 1].posY - KeyFrame[playIndex].posY) / i_max_steps;
    KeyFrame[playIndex].incZ = (KeyFrame[playIndex + 1].posZ - KeyFrame[playIndex].posZ) / i_max_steps;
    
    KeyFrame[playIndex].rotInc = (KeyFrame[playIndex + 1].rotRodIzq - KeyFrame[playIndex].rotRodIzq) / i_max_steps;
    KeyFrame[playIndex].rotIncRD = (KeyFrame[playIndex + 1].rotRodDer - KeyFrame[playIndex].rotRodDer) / i_max_steps;

    KeyFrame[playIndex].rotIncBrazoIzq = (KeyFrame[playIndex + 1].rotBrazoIzq - KeyFrame[playIndex].rotBrazoIzq) / i_max_steps;
    KeyFrame[playIndex].rotIncBrazoDer = (KeyFrame[playIndex + 1].rotBrazoDer - KeyFrame[playIndex].rotBrazoDer) / i_max_steps;
    
    KeyFrame[playIndex].incPosYPanda = (KeyFrame[playIndex + 1].posYPanda - KeyFrame[playIndex].posYPanda) / i_max_steps;
}

float vertices[] = {
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
     0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
    
    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
     0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
        
    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
     0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
};

glm::vec3 Light1 = glm::vec3(0);

// Deltatime
GLfloat deltaTime = 0.0f;    // Time between current frame and last frame
GLfloat lastFrame = 0.0f;      // Time of last frame

//Variables animaciones Tienda de Regalos
bool active;

bool rotacionHelice = false;
float velocidadRotacionHelices = 0.0f;

bool cajaAbierta = false;
float aperturaCajaZ = 1.575f;

bool perezoso = false;
bool sentido = true;
float rotacionPerezoso = 0.0f;
float rotacionBrazos = 0.0f;

bool carrito = false;
float orientacionCarrito = 0.0f;
float rotCarrito = 0.0f;
float traslacionXCarrito = 4.0f;
float traslacionZCarrito = 4.0f;

int main() {
    // Init GLFW
    glfwInit();
    // Set all the required options for GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    // Create a GLFWwindow object that we can use for GLFW's functions
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Proyecto Final", nullptr, nullptr);

    if (nullptr == window){
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return EXIT_FAILURE;
    }

    glfwMakeContextCurrent(window);
    glfwGetFramebufferSize(window, &SCREEN_WIDTH, &SCREEN_HEIGHT);

    //Set the required callback functions
    glfwSetKeyCallback(window, KeyCallback);
    glfwSetCursorPosCallback(window, MouseCallback);

    //Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
    glewExperimental = GL_TRUE;
    
    //Initialize GLEW to setup the OpenGL Function pointers
    if (GLEW_OK != glewInit()){
        std::cout << "Failed to initialize GLEW" << std::endl;
        return EXIT_FAILURE;
    }

    //Define the viewport dimensions
    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    
    //numeros aleatorios
    int minRan = 0;
    int maxRan = 1;
    std::random_device randomDevice; // Only used once to initialise (seed) engine
    std::mt19937 randomEngine(randomDevice()); // Random-number engine used (Mersenne-Twister in this case)
    std::uniform_int_distribution<int> uniformDisGenerator(minRan, maxRan); // Guaranteed unbiased
    array<int, 7> randomOffset;
    for (int i = 0; i < randomOffset.size(); i++) {
        randomOffset[i] = uniformDisGenerator(randomDevice);
    }

    //Shaders
    Shader lightingShader("Shaders/lighting.vs", "Shaders/lighting.frag");
    Shader lampShader("Shaders/lamp.vs", "Shaders/lamp.frag");
    Shader AnimShader("Shaders/anim.vs", "Shaders/anim.frag");
    Shader SkyBoxshader("Shaders/SkyBox.vs", "Shaders/SkyBox.frag");
    //Exterior
        Model Banca((char*)"Models/Exterior/Banca/banca.obj");
        Model Camino((char*)"Models/Exterior/Camino/camino.obj");
        Model Cerca((char*)"Models/Exterior/Cerca/cerca.obj");
        Model LetreroW((char*)"Models/Exterior/Letrero/letrero.obj");
        Model Pasto((char*)"Models/Exterior/Pasto/pasto.obj");
        Model Piscina((char*)"Models/Exterior/Piscina/piscina.obj");
        Model Poste((char*)"Models/Exterior/Poste/poste.obj");
        Model Reja((char*)"Models/Exterior/Reja/reja.obj");
        Model Arbol((char*)"Models/Exterior/Arbol/arbol.obj");
    
    //Tienda de regalos
        //Edificio
        Model Columnas((char*)"Models/TiendaDeRegalos/Edificio/Columnas/columnas.obj");
        Model ParedesInteriores((char*)"Models/TiendaDeRegalos/Edificio/Interior/paredesInteriores.obj");
        Model Letras((char*)"Models/TiendaDeRegalos/Edificio/Letras/letras.obj");
        Model Letrero((char*)"Models/TiendaDeRegalos/Edificio/Letrero/letrero.obj");
        Model Paredes((char*)"Models/TiendaDeRegalos/Edificio/Paredes/paredes.obj");
        Model Pavimento((char*)"Models/TiendaDeRegalos/Edificio/Pavimento/pavimento.obj");
        Model Siluetas((char*)"Models/TiendaDeRegalos/Edificio/Siluetas/siluetas.obj");
        Model Suelo((char*)"Models/TiendaDeRegalos/Edificio/Suelo/suelo.obj");
        Model Techo((char*)"Models/TiendaDeRegalos/Edificio/Techo/techo.obj");
        Model Techo2((char*)"Models/TiendaDeRegalos/Edificio/Techo2/techo2.obj");
        Model Ventanas((char*)"Models/TiendaDeRegalos/Edificio/Ventanas/ventanas.obj");
        //Lamparas
        Model Lamparas((char*)"Models/TiendaDeRegalos/Lamparas/lamparas.obj");
        Model Helices((char*)"Models/TiendaDeRegalos/Lamparas/helices.obj");
        //Macetas
        Model Macetas((char*)"Models/TiendaDeRegalos/Macetas/macetas.obj");
        Model Flores((char*)"Models/TiendaDeRegalos/Macetas/flores.obj");
        //Jirafa
        Model Jirafa((char*)"Models/TiendaDeRegalos/Jirafa/jirafa.obj");
        //Caja Regiostradora
        Model Escritorio((char*)"Models/TiendaDeRegalos/Escritorio/escritorio.obj");
        //Estante
        Model Estante((char*)"Models/TiendaDeRegalos/Estante/estante.obj");
        Model Peresozo((char*)"Models/TiendaDeRegalos/Estante/osoperesozo.obj");
        Model PeresozoBrazo1((char*)"Models/TiendaDeRegalos/Estante/peresozoBrazo1.obj");
        Model PeresozoBrazo2((char*)"Models/TiendaDeRegalos/Estante/peresozoBrazo2.obj");
        //Peluches
        Model Peluches((char*)"Models/TiendaDeRegalos/Peluches/peluches.obj");
        Model PeluchesVidrio((char*)"Models/TiendaDeRegalos/Peluches/peluchesVidrio.obj");
        Model Panda((char*)"Models/TiendaDeRegalos/Peluches/panda.obj");
        //Turistas
        Model Torso((char*)"Models/TiendaDeRegalos/Turistas/turista.obj");
        Model PiernaDer((char*)"Models/TiendaDeRegalos/Turistas/piernaDerecha.obj");
        Model PiernaIzq((char*)"Models/TiendaDeRegalos/Turistas/piernaIzquierda.obj");
        Model BrazoIzq((char*)"Models/TiendaDeRegalos/Turistas/brazoIzquierdo.obj");
        Model BrazoDer((char*)"Models/TiendaDeRegalos/Turistas/brazoDerecho.obj");
        //Caja registradora
        Model CajaRegistradora((char*)"Models/TiendaDeRegalos/CajaRegistradora/cajaRegistradora.obj");
        Model Dinero((char*)"Models/TiendaDeRegalos/CajaRegistradora/dinero.obj");
        //Carrito
        Model Carrito((char*)"Models/TiendaDeRegalos/Carrito/carrito.obj");
    
    //Acuario
        Model AcuarioPared((char*)"Models/Acuario/Pared/modeloPared.obj");
        Model AcuarioPiso((char*)"Models/Acuario/Piso/piso.obj");
        Model AcuarioPeceraCuerpo((char*)"Models/Acuario/PeceraCuerpo/peceraCuerpo.obj");
        Model AcuarioPezNaranja((char*)"Models/Acuario/PezNaranja/pezNaranja.obj");
        Model AcuarioPezNaranja2((char*)"Models/Acuario/PezNaranja2/pezNaranja.obj");
        Model AcuarioPeceraVidrio((char*)"Models/Acuario/PeceraVidrio/pecera.obj");
        Model AcuarioTecho((char*)"Models/Acuario/Techo/techo.obj");
        Model AcuarioTexto((char*)"Models/Acuario/Texto/texto.obj");
        Model AcuarioLampara1((char*)"Models/Acuario/Lampara1/lampara1.obj");
        Model AcuarioLampara2((char*)"Models/Acuario/Lampara2/lampara2.obj");
        Model AcuarioContornoParedes((char*)"Models/Acuario/ContornoParedes/contornoParedes.obj");
        Model AcuarioBurbuja1((char*)"Models/Acuario/Burbuja1/burbuja1.obj");
        Model AcuarioBurbuja2((char*)"Models/Acuario/Burbuja2/burbuja2.obj");
        Model AcuarioEdificioVidrio((char*)"Models/Acuario/VidrioEdificio/vidrioEdificio.obj");
        Model AcuarioPuertaDerecha((char*)"Models/Acuario/PuertaDerecha/puertaDerecha.obj");
        Model AcuarioPuertaIzquierda((char*)"Models/Acuario/PuertaIzquierda/puertaIzquierda.obj");
        Model AcuarioPisoMundo((char*)"Models/Acuario/PisoMundo/pisoMundo.obj");
        Model AcuarioMesasAmbientacion((char*)"Models/Acuario/MesasAmbientacion/mesasAmbientacion.obj");
        Model AcuarioJarrones((char*)"Models/Acuario/Jarrones/jarrones.obj");
        Model AcuarioSillaMovible((char*)"Models/Acuario/SillaMovible/sillaMovible.obj");
        Model AcuarioJarronMovimiento((char*)"Models/Acuario/JarronMovimiento/jarronMovimiento.obj");
        Model AcuarioPulpos((char*)"Models/Acuario/Pulpos/pulpos.obj");
        Model AcuarioSilla1Mov((char*)"Models/Acuario/Silla1Mov/silla1Mov.obj");
        Model AcuarioSilla2Mov((char*)"Models/Acuario/Silla2Mov/silla2Mov.obj");
        Model AcuarioSilla3Mov((char*)"Models/Acuario/Silla3Mov/silla3Mov.obj");
        Model AcuarioTortuga((char*)"Models/Acuario/Tortuga/tortuga.obj");
    
    //Anfibios
        Model AnfibiosVigas((char*)"Models/MuseoDelAjolote/Edificio/vigas.obj");
        Model AnfibiosVentanas((char*)"Models/MuseoDelAjolote/Edificio/ventanas.obj");
        Model AnfibiosPuerta((char*)"Models/MuseoDelAjolote/Edificio/puerta.obj");
        Model AnfibiosMuros((char*)"Models/MuseoDelAjolote/Edificio/muros.obj");
        Model AnfibiosPilares((char*)"Models/MuseoDelAjolote/Edificio/pilares.obj");
        Model AnfibiossueloPlantaBaja((char*)"Models/MuseoDelAjolote/Edificio/sueloPlantaBaja.obj");
        Model AnfibiossueloPrimeraPlanta((char*)"Models/MuseoDelAjolote/Edificio/sueloPrimeraPlanta.obj");
        Model AnfibiosbarandalesCentro((char*)"Models/MuseoDelAjolote/Edificio/barandalesCentro.obj");
        Model AnfibiosTecho((char*)"Models/MuseoDelAjolote/Edificio/techo.obj");
        Model AnfibiosEscalera((char*)"Models/MuseoDelAjolote/Edificio/escalera.obj");
        Model AnfibiosExtintor((char*)"Models/MuseoDelAjolote/Extintor/extintor.obj");
        Model AnfibiosDivisionesVentanas((char*)"Models/MuseoDelAjolote/Edificio/divisiones-ventanas.obj");
        Model AnfibiosLetreroEntrada((char*)"Models/MuseoDelAjolote/Edificio/letrero-entrada.obj");
        Model AnfibiosCuerpoHabitat((char*)"Models/MuseoDelAjolote/Habitat/cuerpoHabitat.obj");
        Model AnfibiosAgua((char*)"Models/MuseoDelAjolote/Habitat/agua.obj");
        Model AnfibiosVentanasHabitat((char*)"Models/MuseoDelAjolote/Habitat/ventanas.obj");
        Model AnfibiosPasto((char*)"Models/MuseoDelAjolote/Habitat/pasto.obj");
        Model AnfibiosHojasFlotantes((char*)"Models/MuseoDelAjolote/Habitat/hojasFlotantes.obj");
        Model AnfibiosAjolotesEstaticos((char*)"Models/MuseoDelAjolote/Ajolote/ajolotesEstaticos.obj");
        Model AnfibiosAdornoCentral((char*)"Models/MuseoDelAjolote/Adorno-central/adorno-central.obj");
        Model AnfibiosSinks((char*)"Models/MuseoDelAjolote/Sinks/sinks.obj");
        Model AnfibiosMesa((char*)"Models/MuseoDelAjolote/Microscopio/table.obj");
        Model AnfibiosBandera1((char*)"Models/MuseoDelAjolote/Banderas/bandera1.obj");
        Model AnfibiosBandera2((char*)"Models/MuseoDelAjolote/Banderas/bandera2.obj");
        Model AnfibiosBandera3((char*)"Models/MuseoDelAjolote/Banderas/bandera3.obj");
        Model AnfibiosBandera4((char*)"Models/MuseoDelAjolote/Banderas/bandera4.obj");
        Model AnfibiosBandera5((char*)"Models/MuseoDelAjolote/Banderas/bandera5.obj");
        Model AnfibiosBandera6((char*)"Models/MuseoDelAjolote/Banderas/bandera6.obj");
        Model AnfibiosBandera7((char*)"Models/MuseoDelAjolote/Banderas/bandera7.obj");
        Model AnfibiosCuerpoAjoloteAnim((char*)"Models/MuseoDelAjolote/Ajolote/cuerpoDinamico.obj");
        Model AnfibiosDelanteraDerechaAnim((char*)"Models/MuseoDelAjolote/Ajolote/delateraDerechaDinamico.obj");
        Model AnfibiosTraceraDerechaAnim((char*)"Models/MuseoDelAjolote/Ajolote/traceraDerechaDinamico.obj");
        Model AnfibiosDelanteraIzquierdaAnim((char*)"Models/MuseoDelAjolote/Ajolote/delanteraIzquierdaDinamico.obj");
        Model AnfibiosTraceraIzquierdaAnim((char*)"Models/MuseoDelAjolote/Ajolote/traceraIzquierdaDinamico.obj");
        Model AnfibiosColaAnim((char*)"Models/MuseoDelAjolote/Ajolote/colaDinamico.obj");
        Model AnfibiosCristalTragaluz((char*)"Models/MuseoDelAjolote/Edificio/cristal-tragaluz.obj");
        Model AnfibiosMarcoTragaluz((char*)"Models/MuseoDelAjolote/Edificio/marco-tragaluz.obj");
        Model AnfibiosCuerpoMicro((char*)"Models/MuseoDelAjolote/Microscopio/cuerpo-micro.obj");
        Model AnfibiosRuedaIzqueirdaMicro((char*)"Models/MuseoDelAjolote/Microscopio/rueda-izquierda.obj");
        Model AnfibiosRuedaDerechaMicro((char*)"Models/MuseoDelAjolote/Microscopio/rueda-derecha.obj");
    
    //Inicializacion de KeyFrames
    for(int i=0; i<MAX_FRAMES; i++){
        KeyFrame[i].posX = aPosX[i];
        KeyFrame[i].posY = aPosY[i];
        KeyFrame[i].posZ = aPosZ[i];
        KeyFrame[i].incX = 0;
        KeyFrame[i].incY = 0;
        KeyFrame[i].incZ = 0;
        KeyFrame[i].rotRodIzq = aRotRodIzq[i];
        KeyFrame[i].rotInc = 0;
        KeyFrame[i].rotRodDer = aRotRodDer[i];
        KeyFrame[i].rotIncRD = 0;
        KeyFrame[i].rotBrazoIzq = aRotBrazoIzq[i];
        KeyFrame[i].rotIncBrazoIzq = 0;
        KeyFrame[i].rotBrazoDer = aRotBrazoDer[i];
        KeyFrame[i].rotIncBrazoDer = 0;
        KeyFrame[i].posYPanda = aPosYPanda[i];
        KeyFrame[i].incPosYPanda = 0;
    }
    
    GLfloat skyboxVertices[] = {
        // Positions
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
    
    // First, set the container's VAO (and VBO)
    GLuint VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    // normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Set texture units
    lightingShader.Use();
    glUniform1i(glGetUniformLocation(lightingShader.Program, "material.diffuse"), 0);
    glUniform1i(glGetUniformLocation(lightingShader.Program, "material.specular"),1);

    //SkyBox
    GLuint skyboxVBO, skyboxVAO;
    glGenVertexArrays(1, &skyboxVAO);
    glGenBuffers(1,&skyboxVBO);
    glBindVertexArray(skyboxVAO);
    glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices),&skyboxVertices,GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT,GL_FALSE, 3 * sizeof(GLfloat), (GLvoid *)0);
    
    // Load textures
    vector<const GLchar*> faces;
    faces.push_back("SkyBox/right.tga");
    faces.push_back("SkyBox/left.tga");
    faces.push_back("SkyBox/top.tga");
    faces.push_back("SkyBox/bottom.tga");
    faces.push_back("SkyBox/back.tga");
    faces.push_back("SkyBox/front.tga");
    
    GLuint cubemapTexture = TextureLoading::LoadCubemap(faces);
    
    glm::mat4 projection = glm::perspective(camera.GetZoom(), (GLfloat)SCREEN_WIDTH / (GLfloat)SCREEN_HEIGHT, 0.1f, 100.0f);

    // Game loop
    while (!glfwWindowShouldClose(window)){
        // Calculate deltatime of current frame
        GLfloat currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
        glfwPollEvents();
        DoMovement();
        animacion();
        animacionA();
        animateJarron();
        
        // Clear the colorbuffer
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
       
        // OpenGL options
        glEnable(GL_DEPTH_TEST);
        
        //Load Model

        // Use cooresponding shader when setting uniforms/drawing objects
        lightingShader.Use();
        GLint viewPosLoc = glGetUniformLocation(lightingShader.Program, "viewPos");
        glUniform3f(viewPosLoc, camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);

        // Directional light
        glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.direction"), -0.2f, -1.0f, -0.3f); //direccion
        glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.ambient"),0.3f,0.3f,0.3f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.diffuse"), 0.3f, 0.3f, 0.3f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.specular"),0.3f, 0.3f, 0.3f);

        // Point light 1
        glm::vec3 lightColor;
        lightColor.x= Light1.x;
        lightColor.y= Light1.y;
        lightColor.z= Light1.z;
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].position"), pointLightPositions[0].x, pointLightPositions[0].y, pointLightPositions[0].z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].ambient"), lightColor.x,lightColor.y, lightColor.z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].diffuse"), lightColor.x,lightColor.y,lightColor.z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].specular"), 0.0f, 0.0f, 0.0f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].constant"), 1.0f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].linear"), 0.22f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].quadratic"),0.2f);

        // Point light 2
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].position"), pointLightPositions[1].x, pointLightPositions[1].y, pointLightPositions[1].z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].ambient"), lightColor.x,lightColor.y, lightColor.z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].diffuse"), lightColor.x,lightColor.y, lightColor.z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].specular"), 0.0f, 0.0f, 0.0f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[1].constant"), 1.0f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[1].linear"), 0.22f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[1].quadratic"), 0.2f);

        // Point light 3
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[2].position"), pointLightPositions[2].x, pointLightPositions[2].y, pointLightPositions[2].z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[2].ambient"), lightColor.x,lightColor.y, lightColor.z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[2].diffuse"), lightColor.x,lightColor.y, lightColor.z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[2].specular"), 0.0f, 0.0f, 0.0f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[2].constant"), 1.0f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[2].linear"), 0.22f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[2].quadratic"), 0.2f);

        // Point light 4
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[3].position"), pointLightPositions[3].x, pointLightPositions[3].y, pointLightPositions[3].z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[3].ambient"), lightColor.x,lightColor.y, lightColor.z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[3].diffuse"), lightColor.x,lightColor.y, lightColor.z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[3].specular"), 0.0f, 0.0f, 0.0f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[3].constant"), 1.0f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[3].linear"), 0.22f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[3].quadratic"), 0.2f);

        // SpotLight
        glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.position"), camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);

        glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.direction"), camera.GetFront().x, camera.GetFront().y, camera.GetFront().z);

        glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.ambient"), 0.0f, 0.0f, 0.0f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.diffuse"), 0.0f, 0.0f, 0.0f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.specular"),0.0f, 0.0f, 0.0f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.constant"), 1.0f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.linear"), 0.35f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.quadratic"), 0.44f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.cutOff"), glm::cos(glm::radians(12.5f)));
        glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.outerCutOff"), glm::cos(glm::radians(15.0f)));

        // Set material properties
        glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 16.0f);

        // Create camera transformations
        glm::mat4 view;
        view = camera.GetViewMatrix();

        // Get the uniform locations
        GLint modelLoc = glGetUniformLocation(lightingShader.Program, "model");
        GLint viewLoc = glGetUniformLocation(lightingShader.Program, "view");
        GLint projLoc = glGetUniformLocation(lightingShader.Program, "projection");

        // Pass the matrices to the shader
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

        glm::mat4 model(1);
/*---------------------------------------------------------------------------------------------------*/
        
/*---------------------------------------------------------------------------------------------------*/
        //Carga de modelos: Tienda de regalos
        view = camera.GetViewMatrix();
        model = glm::mat4(1);
        model = glm::scale(model, glm::vec3(0.3,0.3,0.3));
        model = glm::translate(model, glm::vec3(92.688,0,48.127));
        model = glm::scale(model, glm::vec3(1.5,1.5,1.5));
        model = glm::rotate(model, glm::radians(-65.0f), glm::vec3(0.0f, 1.0f, 0.0f));

        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glUniform1i(glGetUniformLocation(lightingShader.Program,"activaTransparencia"),0);
        Columnas.Draw(lightingShader);
        ParedesInteriores.Draw(lightingShader);
        Letras.Draw(lightingShader);
        Letrero.Draw(lightingShader);
        Paredes.Draw(lightingShader);
        Pavimento.Draw(lightingShader);
        Siluetas.Draw(lightingShader);
        Suelo.Draw(lightingShader);
        Techo.Draw(lightingShader);
        Techo2.Draw(lightingShader);
        Lamparas.Draw(lightingShader);
        Jirafa.Draw(lightingShader);
        Escritorio.Draw(lightingShader);
        Estante.Draw(lightingShader);
        Peluches.Draw(lightingShader);
        Macetas.Draw(lightingShader);
        CajaRegistradora.Draw(lightingShader);
        
        //Carrito
        model = glm::mat4(1);
        model = glm::scale(model, glm::vec3(0.3,0.3,0.3));
        model = glm::translate(model, glm::vec3(92.688,0,48.127));
        model = glm::scale(model, glm::vec3(1.5,1.5,1.5));
        model = glm::rotate(model, glm::radians(-65.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::translate(model, glm::vec3(traslacionXCarrito, 0.0f, traslacionZCarrito));
        model = glm::rotate(model, glm::radians(orientacionCarrito), glm::vec3(0.0f, 1.0f, 0.0f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        Carrito.Draw(lightingShader);
        
        //Peluche panda
        model = glm::mat4(1);
        model = glm::scale(model, glm::vec3(0.3,0.3,0.3));
        model = glm::translate(model, glm::vec3(92.688,0,48.127));
        model = glm::scale(model, glm::vec3(1.5,1.5,1.5));
        model = glm::rotate(model, glm::radians(-65.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::translate(model, glm::vec3(-7.322f,1.177,-2.399f));
        model = glm::translate(model,glm::vec3(0.0f,posYPanda,0.0f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        Panda.Draw(lightingShader);
        
        //Turista
        model = glm::mat4(1);
        model = glm::scale(model, glm::vec3(0.3,0.3,0.3));
        model = glm::translate(model, glm::vec3(92.688,0,48.127));
        model = glm::scale(model, glm::vec3(1.5,1.5,1.5));
        model = glm::rotate(model, glm::radians(-65.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::translate(model, glm::vec3(0.0f,0.0f,-2.009f));
        model = glm::translate(model,glm::vec3(posX,posY,posZ));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        Torso.Draw(lightingShader);
        
        model = glm::mat4(1);
        model = glm::scale(model, glm::vec3(0.3,0.3,0.3));
        model = glm::translate(model, glm::vec3(92.688,0,48.127));
        model = glm::scale(model, glm::vec3(1.5,1.5,1.5));
        model = glm::rotate(model, glm::radians(-65.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::translate(model, glm::vec3(0.255f,1.554f,-2.216f));
        model = glm::translate(model, glm::vec3(posX, posY, posZ));
        model = glm::rotate(model, glm::radians(-rotRodDer), glm::vec3(0.0f, 0.0f, 1.0f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        PiernaDer.Draw(lightingShader);
        
        model = glm::mat4(1);
        model = glm::scale(model, glm::vec3(0.3,0.3,0.3));
        model = glm::translate(model, glm::vec3(92.688,0,48.127));
        model = glm::scale(model, glm::vec3(1.5,1.5,1.5));
        model = glm::rotate(model, glm::radians(-65.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::translate(model, glm::vec3(0.264f,1.572f,-1.789f));
        model = glm::translate(model, glm::vec3(posX, posY, posZ));
        model = glm::rotate(model, glm::radians(-rotRodIzq), glm::vec3(0.0f, 0.0f, 1.0f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        PiernaIzq.Draw(lightingShader);
        
        model = glm::mat4(1);
        model = glm::scale(model, glm::vec3(0.3,0.3,0.3));
        model = glm::translate(model, glm::vec3(92.688,0,48.127));
        model = glm::scale(model, glm::vec3(1.5,1.5,1.5));
        model = glm::rotate(model, glm::radians(-65.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::translate(model, glm::vec3(posX, posY, posZ));
        model = glm::translate(model, glm::vec3(0.061f,3.038f,-2.311f));
        model = glm::rotate(model, glm::radians(-rotBrazoDer), glm::vec3(0.0f, 0.0f, 1.0f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        BrazoDer.Draw(lightingShader);
        
        model = glm::mat4(1);
        model = glm::scale(model, glm::vec3(0.3,0.3,0.3));
        model = glm::translate(model, glm::vec3(92.688,0,48.127));
        model = glm::scale(model, glm::vec3(1.5,1.5,1.5));
        model = glm::rotate(model, glm::radians(-65.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::translate(model, glm::vec3(posX, posY, posZ));
        model = glm::translate(model, glm::vec3(0.125,3.038,-1.736));
        model = glm::rotate(model, glm::radians(-rotBrazoIzq), glm::vec3(0.0f, 0.0f, 1.0f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        BrazoIzq.Draw(lightingShader);
        
        //Animacion Caja Registradora
        model = glm::mat4(1);
        model = glm::scale(model, glm::vec3(0.3,0.3,0.3));
        model = glm::translate(model, glm::vec3(92.688,0,48.127));
        model = glm::scale(model, glm::vec3(1.5,1.5,1.5));
        model = glm::rotate(model, glm::radians(-65.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::translate(model, glm::vec3(8.395,1.602f,aperturaCajaZ));
        model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f,1.0f,0.0f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        Dinero.Draw(lightingShader);
        
        //Animacion Juguete Perezoso
        model = glm::mat4(1);
        model = glm::scale(model, glm::vec3(0.3,0.3,0.3));
        model = glm::translate(model, glm::vec3(92.688,0,48.127));
        model = glm::scale(model, glm::vec3(1.5,1.5,1.5));
        model = glm::rotate(model, glm::radians(-65.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::translate(model, glm::vec3(3.22f,2.005f,-8.78f));
        model = glm::rotate(model, glm::radians((float)glfwGetTime()*rotacionPerezoso), glm::vec3(0.0f,1.0f,0.0f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        Peresozo.Draw(lightingShader);
        
        model = glm::mat4(1);
        model = glm::scale(model, glm::vec3(0.3,0.3,0.3));
        model = glm::translate(model, glm::vec3(92.688,0,48.127));
        model = glm::scale(model, glm::vec3(1.5,1.5,1.5));
        model = glm::rotate(model, glm::radians(-65.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::translate(model, glm::vec3(3.232f,2.012f,-8.78f));
        model = glm::rotate(model, glm::radians((float)glfwGetTime()*rotacionPerezoso), glm::vec3(0.0f,1.0f,0.0f));
        model = glm::rotate(model, glm::radians(rotacionBrazos), glm::vec3(0.0f,0.0f,1.0f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        PeresozoBrazo1.Draw(lightingShader);
        
        model = glm::mat4(1);
        model = glm::scale(model, glm::vec3(0.3,0.3,0.3));
        model = glm::translate(model, glm::vec3(92.688,0,48.127));
        model = glm::scale(model, glm::vec3(1.5,1.5,1.5));
        model = glm::rotate(model, glm::radians(-65.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::translate(model, glm::vec3(3.22f,2.022f,-8.78f));
        model = glm::rotate(model, glm::radians((float)glfwGetTime()*rotacionPerezoso), glm::vec3(0.0f,1.0f,0.0f));
        model = glm::rotate(model, glm::radians(-rotacionBrazos), glm::vec3(0.0f,0.0f,1.0f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        PeresozoBrazo2.Draw(lightingShader);
        
        //Animacion de Helices
        model = glm::mat4(1);
        model = glm::scale(model, glm::vec3(0.3,0.3,0.3));
        model = glm::translate(model, glm::vec3(92.688,0,48.127));
        model = glm::scale(model, glm::vec3(1.5,1.5,1.5));
        model = glm::rotate(model, glm::radians(-65.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::translate(model, glm::vec3(-2.834f,6.502f,7.075f));
        model = glm::rotate(model, glm::radians((float)glfwGetTime()*velocidadRotacionHelices), glm::vec3(0.0f,1.0f,0.0f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        Helices.Draw(lightingShader);
        
        model = glm::mat4(1);
        model = glm::scale(model, glm::vec3(0.3,0.3,0.3));
        model = glm::translate(model, glm::vec3(92.688,0,48.127));
        model = glm::scale(model, glm::vec3(1.5,1.5,1.5));
        model = glm::rotate(model, glm::radians(-65.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::translate(model, glm::vec3(7.643,6.502f,3.803f));
        model = glm::rotate(model, glm::radians((float)glfwGetTime()*velocidadRotacionHelices), glm::vec3(0.0f,1.0f,0.0f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        Helices.Draw(lightingShader);
        
        model = glm::mat4(1);
        model = glm::scale(model, glm::vec3(0.3,0.3,0.3));
        model = glm::translate(model, glm::vec3(92.688,0,48.127));
        model = glm::scale(model, glm::vec3(1.5,1.5,1.5));
        model = glm::rotate(model, glm::radians(-65.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::translate(model, glm::vec3(-6.683,6.502f,-2.268));
        model = glm::rotate(model, glm::radians((float)glfwGetTime()*velocidadRotacionHelices), glm::vec3(0.0f,1.0f,0.0f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        Helices.Draw(lightingShader);
        
        model = glm::mat4(1);
        model = glm::scale(model, glm::vec3(0.3,0.3,0.3));
        model = glm::translate(model, glm::vec3(92.688,0,48.127));
        model = glm::scale(model, glm::vec3(1.5,1.5,1.5));
        model = glm::rotate(model, glm::radians(-65.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::translate(model, glm::vec3(3.14f,6.502f,-7.459f));
        model = glm::rotate(model, glm::radians((float)glfwGetTime()*velocidadRotacionHelices), glm::vec3(0.0f,1.0f,0.0f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        Helices.Draw(lightingShader);
                
        glEnable(GL_BLEND);//Activa la funcionalidad para trabajar el canal alfa
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        model = glm::mat4(1);
        model = glm::scale(model, glm::vec3(0.3,0.3,0.3));
        model = glm::translate(model, glm::vec3(92.688,0,48.127));
        model = glm::scale(model, glm::vec3(1.5,1.5,1.5));
        model = glm::rotate(model, glm::radians(-65.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        
        //Ventanas
        glUniform4f(glGetUniformLocation(lightingShader.Program,"colorAlpha"),1.0,1.0,1.0,0.3);
        Ventanas.Draw(lightingShader);
        
        //Vidrio mostrador de peluches
        glUniform4f(glGetUniformLocation(lightingShader.Program,"colorAlpha"),1.0,1.0,1.0,0.5);
        PeluchesVidrio.Draw(lightingShader);
                
        glDisable(GL_BLEND);
        
        //Flores
        glUniform4f(glGetUniformLocation(lightingShader.Program,"colorAlpha"),1.0,1.0,1.0,1.0);
        glUniform1i(glGetUniformLocation(lightingShader.Program,"activaTransparencia"),1);
        Flores.Draw(lightingShader);
        glUniform1i(glGetUniformLocation(lightingShader.Program,"activaTransparencia"),0);
        
        glBindVertexArray(0);
/*---------------------------------------------------------------------------------------------------*/
        //Carga de modelos: Acuario
        view = camera.GetViewMatrix();
        model = glm::mat4(1);
        model = glm::scale(model, glm::vec3(0.3,0.3,0.3));
        model = glm::translate(model,glm::vec3(91.512,0,-75.644));
        model = glm::scale(model, glm::vec3(7.5,7.5,7.5));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glUniform1f(glGetUniformLocation(lightingShader.Program, "transparencia"), 0.0);
        AcuarioPared.Draw(lightingShader);

        model = glm::mat4(1);
        model = glm::scale(model, glm::vec3(0.3,0.3,0.3));
        model = glm::translate(model,glm::vec3(91.512,0,-75.644));
        model = glm::scale(model, glm::vec3(7.5,7.5,7.5));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        AcuarioPiso.Draw(lightingShader);

        model = glm::mat4(1);
        model = glm::scale(model, glm::vec3(0.3,0.3,0.3));
        model = glm::translate(model,glm::vec3(91.512,0,-75.644));
        model = glm::scale(model, glm::vec3(7.5,7.5,7.5));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        AcuarioPeceraCuerpo.Draw(lightingShader);

        model = glm::mat4(1);
        model = glm::scale(model, glm::vec3(0.3,0.3,0.3));
        model = glm::translate(model,glm::vec3(91.512,0,-75.644));
        model = glm::scale(model, glm::vec3(7.5,7.5,7.5));
        model = glm::translate(model, glm::vec3(- 1.2f + r * cos(glm::radians(rotFish)), 0.487,-0.638 + r * sin(glm::radians(rotFish))));
        model = glm::rotate(model, glm::radians(rotFishLeft), glm::vec3(0.0f, 1.0f, 0.0f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        AcuarioPezNaranja.Draw(lightingShader);

        model = glm::mat4(1);
        model = glm::scale(model, glm::vec3(0.3,0.3,0.3));
        model = glm::translate(model,glm::vec3(91.512,0,-75.644));
        model = glm::scale(model, glm::vec3(7.5,7.5,7.5));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        AcuarioPezNaranja2.Draw(lightingShader);

        model = glm::mat4(1);
        model = glm::scale(model, glm::vec3(0.3,0.3,0.3));
        model = glm::translate(model,glm::vec3(91.512,0,-75.644));
        model = glm::scale(model, glm::vec3(7.5,7.5,7.5));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        AcuarioTecho.Draw(lightingShader);

        model = glm::mat4(1);
        model = glm::scale(model, glm::vec3(0.3,0.3,0.3));
        model = glm::translate(model,glm::vec3(91.512,0,-75.644));
        model = glm::scale(model, glm::vec3(7.5,7.5,7.5));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        AcuarioTexto.Draw(lightingShader);

        model = glm::mat4(1);
        model = glm::scale(model, glm::vec3(0.3,0.3,0.3));
        model = glm::translate(model,glm::vec3(91.512,0,-75.644));
        model = glm::scale(model, glm::vec3(7.5,7.5,7.5));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        AcuarioLampara1.Draw(lightingShader);

        model = glm::mat4(1);
        model = glm::scale(model, glm::vec3(0.3,0.3,0.3));
        model = glm::translate(model,glm::vec3(91.512,0,-75.644));
        model = glm::scale(model, glm::vec3(7.5,7.5,7.5));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        AcuarioLampara2.Draw(lightingShader);

        model = glm::mat4(1);
        model = glm::scale(model, glm::vec3(0.3,0.3,0.3));
        model = glm::translate(model,glm::vec3(91.512,0,-75.644));
        model = glm::scale(model, glm::vec3(7.5,7.5,7.5));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        AcuarioContornoParedes.Draw(lightingShader);

        model = glm::mat4(1);
        model = glm::scale(model, glm::vec3(0.3,0.3,0.3));
        model = glm::translate(model,glm::vec3(91.512,0,-75.644));
        model = glm::scale(model, glm::vec3(7.5,7.5,7.5));
        model = glm::translate(model, glm::vec3(0.5,0.509,2.813));
        model = glm::rotate(model, glm::radians(rotPuertaDer), glm::vec3(0.0f, 1.0f, 0.0f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        AcuarioPuertaDerecha.Draw(lightingShader);

        model = glm::mat4(1);
        model = glm::scale(model, glm::vec3(0.3,0.3,0.3));
        model = glm::translate(model,glm::vec3(91.512,0,-75.644));
        model = glm::scale(model, glm::vec3(7.5,7.5,7.5));
        model = glm::translate(model, glm::vec3(-0.64,0.509,2.813));
        model = glm::rotate(model, glm::radians(rotPuertaIzq), glm::vec3(0.0f, 1.0f, 0.0f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        AcuarioPuertaIzquierda.Draw(lightingShader);

        model = glm::mat4(1);
        model = glm::scale(model, glm::vec3(0.3,0.3,0.3));
        model = glm::translate(model,glm::vec3(91.512,0,-75.644));
        model = glm::scale(model, glm::vec3(7.5,7.5,7.5));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        AcuarioMesasAmbientacion.Draw(lightingShader);

        model = glm::mat4(1);
        model = glm::scale(model, glm::vec3(0.3,0.3,0.3));
        model = glm::translate(model,glm::vec3(91.512,0,-75.644));
        model = glm::scale(model, glm::vec3(7.5,7.5,7.5));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        AcuarioJarrones.Draw(lightingShader);

        model = glm::mat4(1);
        model = glm::scale(model, glm::vec3(0.3,0.3,0.3));
        model = glm::translate(model,glm::vec3(91.512,0,-75.644));
        model = glm::scale(model, glm::vec3(7.5,7.5,7.5));
        model = glm::translate(model, glm::vec3(-1.19f,0.450f,2.228f));
        model = glm::translate(model, glm::vec3(movKitX, movKitY, movKitZ));
        model = glm::rotate(model, glm::radians(rotKitZ), glm::vec3(1.0f, 0.0f, 0.0f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        AcuarioJarronMovimiento.Draw(lightingShader);

        model = glm::mat4(1);
        model = glm::scale(model, glm::vec3(0.3,0.3,0.3));
        model = glm::translate(model,glm::vec3(91.512,0,-75.644));
        model = glm::scale(model, glm::vec3(7.5,7.5,7.5));
        model = glm::translate(model, glm::vec3(1.457,0.3,2.503));
        model = glm::rotate(model, glm::radians(rotSilla), glm::vec3(0.0f, 1.0f, 0.0f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        AcuarioSillaMovible.Draw(lightingShader);

        model = glm::mat4(1);
        model = glm::scale(model, glm::vec3(0.3,0.3,0.3));
        model = glm::translate(model,glm::vec3(91.512,0,-75.644));
        model = glm::scale(model, glm::vec3(7.5,7.5,7.5));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        AcuarioPulpos.Draw(lightingShader);

        model = glm::mat4(1);
        model = glm::scale(model, glm::vec3(0.3,0.3,0.3));
        model = glm::translate(model,glm::vec3(91.512,0,-75.644));
        model = glm::scale(model, glm::vec3(7.5,7.5,7.5));
        model = glm::translate(model, glm::vec3(-posSillas, 0, 0));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        AcuarioSilla1Mov.Draw(lightingShader);

        model = glm::mat4(1);
        model = glm::scale(model, glm::vec3(0.3,0.3,0.3));
        model = glm::translate(model,glm::vec3(91.512,0,-75.644));
        model = glm::scale(model, glm::vec3(7.5,7.5,7.5));
        model = glm::translate(model, glm::vec3(posSillas, 0, 0));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        AcuarioSilla2Mov.Draw(lightingShader);

        model = glm::mat4(1);
        model = glm::scale(model, glm::vec3(0.3,0.3,0.3));
        model = glm::translate(model,glm::vec3(91.512,0,-75.644));
        model = glm::scale(model, glm::vec3(7.5,7.5,7.5));
        model = glm::translate(model, glm::vec3(0, 0, -posSillas));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        AcuarioSilla3Mov.Draw(lightingShader);
        
        model = glm::mat4(1);
        model = glm::scale(model, glm::vec3(0.3,0.3,0.3));
        model = glm::translate(model,glm::vec3(91.512,0,-75.644));
        model = glm::scale(model, glm::vec3(7.5,7.5,7.5));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        AcuarioTortuga.Draw(lightingShader);
        
        glUniform3f(glGetUniformLocation(lightingShader.Program, "material.ambient"), 0.0f, 0.0f, 1.0f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "material.diffuse"), 0.0f, 0.0f, 0.4f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "material.specular"), 0.5f, 0.5f, 0.5f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 10.0f);

        model = glm::mat4(1);
        model = glm::scale(model, glm::vec3(0.3,0.3,0.3));
        model = glm::translate(model,glm::vec3(91.512,0,-75.644));
        model = glm::scale(model, glm::vec3(7.5,7.5,7.5));
        model = glm::translate(model, glm::vec3(0,posDeltaBur1,0));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        AcuarioBurbuja1.Draw(lightingShader);

        model = glm::mat4(1);
        model = glm::scale(model, glm::vec3(0.3,0.3,0.3));
        model = glm::translate(model,glm::vec3(91.512,0,-75.644));
        model = glm::scale(model, glm::vec3(7.5,7.5,7.5));
        model = glm::translate(model, glm::vec3(0, posDeltaBur2, 0));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        AcuarioBurbuja2.Draw(lightingShader);

        //Traslucidez
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // activa alpha
        model = glm::mat4(1);
        model = glm::scale(model, glm::vec3(0.3,0.3,0.3));
        model = glm::translate(model,glm::vec3(91.512,0,-75.644));
        model = glm::scale(model, glm::vec3(7.5,7.5,7.5));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
        glUniform4f(glGetUniformLocation(lightingShader.Program, "colorAlpha"), 0.0f, 0.5f, 0.5f, 0.5f);
        AcuarioPeceraVidrio.Draw(lightingShader);

        model = glm::mat4(1);
        model = glm::scale(model, glm::vec3(0.3,0.3,0.3));
        model = glm::translate(model,glm::vec3(91.512,0,-75.644));
        model = glm::scale(model, glm::vec3(7.5,7.5,7.5));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glUniform4f(glGetUniformLocation(lightingShader.Program, "colorAlpha"), 0.5f, 0.5f, 0.5f, 0.1f);
        AcuarioEdificioVidrio.Draw(lightingShader);

        glDisable(GL_BLEND);
        glEnable(GL_DEPTH_TEST);
        glBindVertexArray(0);
/*---------------------------------------------------------------------------------------------------*/
        //Carga de modelos: Anfibios
        view = camera.GetViewMatrix();
        model = glm::mat4(1);
        model = glm::scale(model, glm::vec3(0.3,0.3,0.3));
        model = glm::translate(model,glm::vec3(8.441,0,-16.343));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glUniform1i(glGetUniformLocation(lightingShader.Program, "enableTransparency"), 0);
        AnfibiosVigas.Draw(lightingShader);
        
        model = glm::translate(model,glm::vec3(33.79, 5.4, -4.65));
        model = glm::rotate(model, (float)glm::radians(rotDoor), glm::vec3(0, 1, 0));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        AnfibiosPuerta.Draw(lightingShader);
        
        model = glm::mat4(1);
        model = glm::scale(model, glm::vec3(0.3,0.3,0.3));
        model = glm::translate(model,glm::vec3(8.441,0,-16.343));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        AnfibiosPilares.Draw(lightingShader);
        AnfibiossueloPlantaBaja.Draw(lightingShader);
        AnfibiossueloPrimeraPlanta.Draw(lightingShader);
        AnfibiosTecho.Draw(lightingShader);
        AnfibiosEscalera.Draw(lightingShader);
        AnfibiosExtintor.Draw(lightingShader);
        AnfibiosDivisionesVentanas.Draw(lightingShader);
        AnfibiosMuros.Draw(lightingShader);
        AnfibiosCuerpoHabitat.Draw(lightingShader);
        AnfibiosAjolotesEstaticos.Draw(lightingShader);
        AnfibiosSinks.Draw(lightingShader);
        
        //Modelado jer·rquico ajolote
        model = glm::mat4(1);
        model = glm::scale(model, glm::vec3(0.3,0.3,0.3));
        model = glm::translate(model,glm::vec3(8.441,0,-16.343));
        model = glm::translate(model, glm::vec3(-3.8811 + movX, 3.4185, -3.9202 + movZ));
        model = glm::rotate(model, glm::radians(angCir), glm::vec3(0, 1, 0));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        AnfibiosCuerpoAjoloteAnim.Draw(lightingShader);
        
        glm::mat4 modelTmp = model;
        model = glm::translate(model, glm::vec3(-0.1252, -0.08, 0.0351));
        model = glm::rotate(model, glm::radians(rotLeg), glm::vec3(0, 1, 0));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        AnfibiosDelanteraDerechaAnim.Draw(lightingShader);
        
        model = modelTmp;
        model = glm::translate(model, glm::vec3(-0.124, -0.0976, -0.2251));
        model = glm::rotate(model, glm::radians(-rotLeg), glm::vec3(0, 1, 0));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        AnfibiosTraceraDerechaAnim.Draw(lightingShader);
        
        model = modelTmp;
        model = glm::translate(model, glm::vec3(0.1131, -0.0752, 0.038));
        model = glm::rotate(model, glm::radians(rotLeg), glm::vec3(0, 1, 0));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        AnfibiosDelanteraIzquierdaAnim.Draw(lightingShader);
        
        model = modelTmp;
        model = glm::translate(model, glm::vec3(0.1019, -0.0733, -0.2158));
        model = glm::rotate(model, glm::radians(-rotLeg), glm::vec3(0, 1, 0));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        AnfibiosTraceraIzquierdaAnim.Draw(lightingShader);
        
        model = modelTmp;
        model = glm::translate(model, glm::vec3(0.0027, -0.0162, -0.3856));
        model = glm::rotate(model, glm::radians(rotTail), glm::vec3(0, 1, 0));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        AnfibiosColaAnim.Draw(lightingShader);
        
        //tragaluz
        model = glm::mat4(1);
        model = glm::scale(model, glm::vec3(0.3,0.3,0.3));
        model = glm::translate(model,glm::vec3(8.441,0,-16.343));
        model = glm::translate(model, glm::vec3(-0.6837, 23.6333 , 0.6244));
        model = glm::translate(model, glm::vec3(movXTraga, 0, 0));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        AnfibiosMarcoTragaluz.Draw(lightingShader);
        
        glm::mat4 tmpTragaluz = model;
        glUniform4f(glGetUniformLocation(lightingShader.Program, "colorAlpha"), 0.0, 0.0, 0.0, 0.8);
        glEnable(GL_BLEND);//Activa la funcionalidad para trabajar el canal alfa
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        model = tmpTragaluz;
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        AnfibiosCristalTragaluz.Draw(lightingShader);
        
        model = glm::mat4(1);
        model = glm::scale(model, glm::vec3(0.3,0.3,0.3));
        model = glm::translate(model,glm::vec3(8.441,0,-16.343));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        
        model = glm::mat4(1);
        model = glm::scale(model, glm::vec3(0.3,0.3,0.3));
        model = glm::translate(model,glm::vec3(8.441,0,-16.343));
        AnfibiosVentanas.Draw(lightingShader);
        AnfibiosVentanasHabitat.Draw(lightingShader);
        
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glUniform4f(glGetUniformLocation(lightingShader.Program, "colorAlpha"), 1.0, 1.0, 1.0, 2.0);
        AnfibiosAgua.Draw(lightingShader);
        glDisable(GL_BLEND);  //Desactiva el canal alfa
        
        glUniform4f(glGetUniformLocation(lightingShader.Program, "colorAlpha"), 1.0, 1.0, 1.0, 1.0);
        glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 1);
        AnfibiosbarandalesCentro.Draw(lightingShader);
        AnfibiosLetreroEntrada.Draw(lightingShader);
        AnfibiosPasto.Draw(lightingShader);
        AnfibiosHojasFlotantes.Draw(lightingShader);
        AnfibiosAdornoCentral.Draw(lightingShader);
        glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
        glBindVertexArray(0);
        
        //Banderas
        model = glm::mat4(1);
        model = glm::scale(model, glm::vec3(0.3,0.3,0.3));
        model = glm::translate(model,glm::vec3(8.441,0,-16.343));
        model = glm::translate(model, glm::vec3(-38.2, 18.61, 34));
        model = glm::rotate(model, (float)glm::radians(90.0), glm::vec3(0, 1, 0));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        AnfibiosBandera1.Draw(lightingShader);
        
        model = glm::mat4(1);
        model = glm::scale(model, glm::vec3(0.3,0.3,0.3));
        model = glm::translate(model,glm::vec3(8.441,0,-16.343));
        model = glm::translate(model, glm::vec3(-14.5, 18.61, 34));
        model = glm::rotate(model, (float)glm::radians(90.0), glm::vec3(0, 1, 0));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        AnfibiosBandera2.Draw(lightingShader);

        model = glm::mat4(1);
        model = glm::scale(model, glm::vec3(0.3,0.3,0.3));
        model = glm::translate(model,glm::vec3(8.441,0,-16.343));
        model = glm::translate(model, glm::vec3(14.2, 18.61, 35));
        model = glm::rotate(model, (float)glm::radians(90.0), glm::vec3(0, 1, 0));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        AnfibiosBandera3.Draw(lightingShader);

        model = glm::mat4(1);
        model = glm::scale(model, glm::vec3(0.3,0.3,0.3));
        model = glm::translate(model,glm::vec3(8.441,0,-16.343));
        model = glm::translate(model, glm::vec3(40.6, 18.61, 32.7));
        model = glm::rotate(model, (float)glm::radians(-45.0), glm::vec3(0, 1, 0));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        AnfibiosBandera4.Draw(lightingShader);
        
        model = glm::mat4(1);
        model = glm::scale(model, glm::vec3(0.3,0.3,0.3));
        model = glm::translate(model,glm::vec3(8.441,0,-16.343));
        model = glm::translate(model, glm::vec3(42.06, 18.61, 10.49));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        AnfibiosBandera5.Draw(lightingShader);
        
        model = glm::mat4(1);
        model = glm::scale(model, glm::vec3(0.3,0.3,0.3));
        model = glm::translate(model,glm::vec3(8.441,0,-16.343));
        model = glm::translate(model, glm::vec3(42.06, 18.61, -10.03));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        AnfibiosBandera6.Draw(lightingShader);
        
        model = glm::mat4(1);
        model = glm::scale(model, glm::vec3(0.3,0.3,0.3));
        model = glm::translate(model,glm::vec3(8.441,0,-16.343));
        model = glm::translate(model, glm::vec3(40.84, 18.61, -32.66));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        AnfibiosBandera7.Draw(lightingShader);

        glBindVertexArray(0);
        
/*---------------------------------------------------------------------------------------------------*/
        //Carga de modelos: Exterior
        view = camera.GetViewMatrix();
        model = glm::mat4(1);
        model = glm::scale(model, glm::vec3(0.3,0.3,0.3));
        model = glm::translate(model, glm::vec3(0,-0.1,0));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glUniform1i(glGetUniformLocation(lightingShader.Program,"activaTransparencia"),0);
        
        Camino.Draw(lightingShader);
        Cerca.Draw(lightingShader);
        LetreroW.Draw(lightingShader);
        Pasto.Draw(lightingShader);
        Piscina.Draw(lightingShader);
        Reja.Draw(lightingShader);
        
        //Bancas
        for(int i=0; i<28; i++){
            model = glm::mat4(1);
            model = glm::scale(model, glm::vec3(0.3,0.3,0.3));
            model = glm::translate(model, bancasPositions[i]);
            model = glm::rotate(model, glm::radians(rotacionBancas[i]), glm::vec3(0.0f, 1.0f, 0.0f));
            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
            Banca.Draw(lightingShader);
        }
        
        //Postes
        for(int i=0; i<32; i++){
            model = glm::mat4(1);
            model = glm::scale(model, glm::vec3(0.3,0.3,0.3));
            model = glm::translate(model, postesPositions[i]);
            model = glm::rotate(model, glm::radians(rotacionPostes[i]), glm::vec3(0.0f, 1.0f, 0.0f));
            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
            Poste.Draw(lightingShader);
        }
        
        //Arboles
        glUniform4f(glGetUniformLocation(lightingShader.Program,"colorAlpha"),1.0,1.0,1.0,1.0);
        glUniform1i(glGetUniformLocation(lightingShader.Program,"activaTransparencia"),1);
        model = glm::mat4(1);
        for(int i=0; i<20; i++){
            model = glm::mat4(1);
            model = glm::scale(model, glm::vec3(0.3,0.3,0.3));
            model = glm::translate(model, arbolesPositions[i]);
            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
            Arbol.Draw(lightingShader);
        }
        glUniform1i(glGetUniformLocation(lightingShader.Program,"activaTransparencia"),0);
        
        glBindVertexArray(0);

        // Also draw the lamp object, again binding the appropriate shader
        lampShader.Use();
        // Get location objects for the matrices on the lamp shader (these could be different on a different shader)
        modelLoc = glGetUniformLocation(lampShader.Program, "model");
        viewLoc = glGetUniformLocation(lampShader.Program, "view");
        projLoc = glGetUniformLocation(lampShader.Program, "projection");

        // Set matrices
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
        model = glm::mat4(1);
        model = glm::translate(model, lightPos);
        model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        // Draw the light object (using light's vertex attributes)
        for (GLuint i = 0; i < 4; i++){
            model = glm::mat4(1);
            model = glm::translate(model, pointLightPositions[i]);
            model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
            glBindVertexArray(VAO);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
        glBindVertexArray(0);
        
/*---------------------------------------------------------------------------------------------------*/
        
        // Draw skybox as last
        glDepthFunc(GL_LEQUAL);  // Change depth function so depth test passes when values are equal to depth buffer's content
        SkyBoxshader.Use();
        view = glm::mat4(glm::mat3(camera.GetViewMatrix()));    // Remove any translation component of the view matrix
        glUniformMatrix4fv(glGetUniformLocation(SkyBoxshader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(glGetUniformLocation(SkyBoxshader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

        // skybox cube
        glBindVertexArray(skyboxVAO);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);
        glDepthFunc(GL_LESS); // Set depth function back to default
        
        // Swap the screen buffers
        glfwSwapBuffers(window);
    }
    
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteVertexArrays(1, &skyboxVAO);
    glDeleteBuffers(1, &skyboxVBO);
    
    // Terminate GLFW, clearing any resources allocated by GLFW.
    glfwTerminate();
    return 0;
}
inline void animateSillas() {
    if (posSillas > 0.4f) {
        changeDirectionSillas = true;
    }
    else if (posSillas < 0.0f) {
        changeDirectionSillas = false;
    }

    if (!changeDirectionSillas) {
        posSillas += 0.001;
    }
    else {
        posSillas -= 0.001;
    }
}

void animateJarron() {
    //Movimiento del coche
    if (iniciaJarron) {
        if (recorrido1) {
            movKitX += 0.001f;
            rotKitZ += 0.1f;
            if (movKitX > 0.05f) {
                recorrido1 = false;
                recorrido2 = true;
            }
        }
        if (recorrido2) {
            printf("%f %f %f\n", movKitX, movKitY, movKitZ);
            movKitX += 0.0015f;
            movKitY -= 4.0f * alphaVariation * pow(0.001, 2);
            alphaVariation += 4.5f;
            rotKitZ += 0.4f;
            if (movKitY < -0.40f) {
                recorrido2 = false;
                recorrido3 = true;
            }
        }
        if (recorrido3) {
            printf("----\n");
            printf("%f %f %f\n", movKitX, movKitY, movKitZ);
            movKitZ -= 0.00015f;
            alphaVariation += 2.0f;
            rotKitX += 0.1f;
            if (movKitZ < -0.10f) {
                recorrido3 = false;
                recorrido4 = true;
            }
        }
        if (recorrido4) {
            printf("----!!!\n");
            printf("%f %f %f\n", movKitX, movKitY, movKitZ);
            movKitZ += 0.00015f;
            rotKitX -= 0.1f;
            if (movKitZ > 0.10f) {
                recorrido4 = false;
            }
        }
    }
}

void animacionA() {
        //Movimiento del personaje
        if (play) {
            if (i_curr_steps >= i_max_steps) {//end of animation between frames?
                playIndex++;
                if (playIndex>FrameIndex - 2)    //end of total animation?
                {
                    printf("termina anim\n");
                    playIndex = 0;
                    play = false;
                }
                else //Next frame interpolations
                {
                    i_curr_steps = 0; //Reset counter
                                      //Interpolation
                    interpolation();
                }
            }
            else
            {
                //Draw animation
                posX += KeyFrame[playIndex].incX;
                posY += KeyFrame[playIndex].incY;
                posZ += KeyFrame[playIndex].incZ;
                rotRodIzq += KeyFrame[playIndex].rotInc;
                i_curr_steps++;
            }
        }
}

//inline void turnOnLights() {
//    LightP1 = LightP2 = LightP3 = glm::vec3(0.2f, 0.2f, 0.2f);
//}
//
//inline void turnOffLights() {
//    LightP1 = LightP2 = LightP3 = glm::vec3(0.0f, 0.0f, 0.0f);
//}

inline void animateFish() {
    rotFish += 0.5;
    rotFishLeft -= 0.5;
}

inline void animateDoors() {
    if (openDoors) {
        rotPuertaDer -= 0.3;
        rotPuertaIzq += 0.3;
    } else {
        rotPuertaDer += 0.3;
        rotPuertaIzq -= 0.3;
    }
    if (rotPuertaDer < -90) {
        openDoors = false;
    }
    if (rotPuertaDer > 0)
        openDoors = true;
}

inline void animateChair() {
    rotSilla += 1.0f;
}

inline void animateBubbles() {
    float currBubblePos = posY + posDeltaBur1;
    if (currBubblePos > 1.73) {
        posDeltaBur1 = 0;
        posDeltaBur2 = 0;
    }
    else {
        posDeltaBur1 += 0.001;
        posDeltaBur2 += 0.002;
    }
}



void animacion(){
    //Movimiento del personaje
    if (play){
        if (i_curr_steps >= i_max_steps){ //end of animation between frames?
            playIndex++;
            if (playIndex>FrameIndex - 2){    //end of total animation?
                printf("termina anim\n");
                playIndex = 0;
                play = false;
            } else { //Next frame interpolations
                i_curr_steps = 0; //Reset counter
                interpolation();//Interpolation
            }
        } else {
            //Draw animation
            posX += KeyFrame[playIndex].incX;
            posY += KeyFrame[playIndex].incY;
            posZ += KeyFrame[playIndex].incZ;
            rotRodIzq += KeyFrame[playIndex].rotInc;
            rotRodDer += KeyFrame[playIndex].rotIncRD;
            rotBrazoIzq += KeyFrame[playIndex].rotIncBrazoIzq;
            rotBrazoDer += KeyFrame[playIndex].rotIncBrazoDer;
            posYPanda += KeyFrame[playIndex].incPosYPanda;
            i_curr_steps++;
        }
    }
}

// Moves/alters the camera positions based on user input
void DoMovement(){
    // Camera controls
    if (keys[GLFW_KEY_W] || keys[GLFW_KEY_UP]){
        camera.ProcessKeyboard(FORWARD, deltaTime);
    }
    
    if (keys[GLFW_KEY_S] || keys[GLFW_KEY_DOWN]){
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    }
    
    if (keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT]){
        camera.ProcessKeyboard(LEFT, deltaTime);
    }
    
    if (keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT]){
        camera.ProcessKeyboard(RIGHT, deltaTime);
    }
    
    if (keys[GLFW_KEY_B]) {
        animateBubbles();
    }
    
    if (keys[GLFW_KEY_M]) {
        animateFish();
    }
    
    if (keys[GLFW_KEY_P]) {
        animateDoors();
    }
    
    if (keys[GLFW_KEY_N]) {
        animateChair();
    }
    
    if (keys[GLFW_KEY_X]) {
        animateSillas();
    }
    
    if(cajaAbierta){
        if(aperturaCajaZ<2.299){
            aperturaCajaZ += 0.01;
        }
    } else {
        if(aperturaCajaZ>1.575){
            aperturaCajaZ -= 0.01;
        }
    }
    
    if(perezoso){
        if(sentido){
            rotacionBrazos += 0.5f;
            if(rotacionBrazos==45.0f) sentido = false;
        } else {
            rotacionBrazos -= 0.5f;
            if(rotacionBrazos==0) sentido = true;
        }
    }
    
    if(carrito) {
        rotCarrito += 0.6;
        orientacionCarrito -= 0.6;
    }
    traslacionXCarrito = 4 * cos(glm::radians(rotCarrito));
    traslacionZCarrito = 4 * sin(glm::radians(rotCarrito));
        
    if (animacionTragaluz) {
        if (movXTraga < 8.5 && dirTraga) {
            movXTraga += min(VELOCIDAD_TRAGALUZ * deltaTime, 8.5f-movXTraga);
        }
        else if (movXTraga > 0 && !dirTraga) {
            movXTraga -= min(VELOCIDAD_TRAGALUZ * deltaTime, movXTraga);
        }
        else {
            if (movXTraga < 0)
            animacionTragaluz = false;
        }
    }
    
    if (animacionPuerta) {
        if (rotDoor < 90 && dirDoor) {
            rotDoor += min(VELOCIDAD_DOOR*deltaTime, 90-rotDoor);
        }
        else if (rotDoor > 0 && !dirDoor) {
            rotDoor -= min(VELOCIDAD_DOOR*deltaTime, rotDoor);
        }
        else {
             animacionPuerta = false;
        }
    }

    if (animacionMicro) {
        if (rotMicro < 90 && dirMicro) {
            rotMicro += min(VELOCIDAD_MICRO*deltaTime, 90 - rotMicro);
        }
        else if (rotMicro > 0 && !dirMicro) {
            rotMicro -= min(VELOCIDAD_MICRO*deltaTime, rotMicro);
        }
        else {
            animacionMicro = false;
        }
    }

    if (animacionAjolote) {
        if (rotLeg < 25 && dirLegs) {
            rotLeg += VELOCIDAD_PATAS_AJOLOTE * deltaTime;
        }
        else if (rotLeg > -25 && !dirLegs) {
            rotLeg -= VELOCIDAD_PATAS_AJOLOTE * deltaTime;
        }
        else {
            dirLegs = !dirLegs;
        }

        if (rotTail < 20 && dirTail) {
            rotTail += VELOCIDAD_COLA_AJOLOTE * deltaTime;
        }
        else if (rotTail > -20 && !dirTail) {
            rotTail -= VELOCIDAD_COLA_AJOLOTE * deltaTime;
        }
        else {
            dirTail = !dirTail;
        }

        
        switch (direccionAjolote) {
            case ADELANTE:
                if (movZ < 8) {
                    movZ += VELOCIDAD_NADO_AJOLOTE*deltaTime;
                }else {
                    direccionAjolote = MOVIMIENTO_CIRCULAR_IDA;
                    xc = movX;
                    zc = movZ;
                }
                break;
            case MOVIMIENTO_CIRCULAR_IDA:
                if (angCir < 180) {
                    angCir += VELOCIDAD_GIRO_AJOLOTE * deltaTime;
                    movX = xc + cos(glm::radians(angCir + 180)) + 1;
                    movZ = zc + sin(glm::radians(angCir));
                }
                else {
                    direccionAjolote = ATRAS;
                }
                break;
            case ATRAS:
                if (movZ > 0) {
                    movZ -= VELOCIDAD_NADO_AJOLOTE * deltaTime;
                }
                else {
                    direccionAjolote = MOVIMIENTO_CIRCULAR_REGRESO;
                    xc = movX;
                    zc = movZ;
                }
                break;
            case MOVIMIENTO_CIRCULAR_REGRESO:
                if (angCir < 360) {
                    angCir += VELOCIDAD_GIRO_AJOLOTE * deltaTime;
                    movX = xc + cos(glm::radians(angCir + 180)) - 1;
                    movZ = zc + sin(glm::radians(angCir));
                }
                else {
                    direccionAjolote = ADELANTE;
                    angCir = 0;
                }
                break;

        }
        
    }
}

// Is called whenever a key is pressed/released via GLFW
void KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mode) {
    if (GLFW_KEY_ESCAPE == key && GLFW_PRESS == action){
        glfwSetWindowShouldClose(window, GL_TRUE);
    }

    if (key >= 0 && key < 1024){
        if (action == GLFW_PRESS){
            keys[key] = true;
        }
        else if (action == GLFW_RELEASE){
            keys[key] = false;
        }
    }

    if (keys[GLFW_KEY_SPACE]){
        active = !active;
        if (active){
            Light1 = glm::vec3(1.0f, 1.0f, 1.0f);
        }
        else{
            Light1 = glm::vec3(0);//Cuado es solo un valor en los 3 vectores pueden dejar solo una componente
        }
    }
    
    if(keys[GLFW_KEY_H]){
        cajaAbierta = !cajaAbierta;
    }
    
    if(keys[GLFW_KEY_G]){
        rotacionHelice = !rotacionHelice;
        if (rotacionHelice){
            velocidadRotacionHelices = 150.0f;
        }
        else{
            velocidadRotacionHelices = 0.0f;
        }
        
    }
    
    if(keys[GLFW_KEY_J]){
        perezoso = !perezoso;
        if(perezoso) {
            rotacionPerezoso = 80.0f;
        } else {
            rotacionPerezoso = 0.0f;
        }
    }
    
    if (keys[GLFW_KEY_L]){
        if (play == false && (FrameIndex > 1)){
            resetElements();
            //First Interpolation
            interpolation();
            play = true;
            playIndex = 0;
            i_curr_steps = 0;
        } else {
            play = false;
        }
    }
    
    if (keys[GLFW_KEY_K]){
        carrito = !carrito;
    }
    
    if (keys[GLFW_KEY_I]) {
        iniciaJarron = true;
    }
    if (keys[GLFW_KEY_U]) {
        iniciaJarron = false;
    }
    
    if (keys[GLFW_KEY_P]) {
        animacionPuerta = true;
        dirDoor = !dirDoor;
    }

    if (keys[GLFW_KEY_M]) {
        animacionMicro = true;
        dirMicro = !dirMicro;
    }

    if (keys[GLFW_KEY_F]) {
        if (speed != 0) {
            speed = 0;
        }
        else {
            speed = 3;
        }
    }

    if (keys[GLFW_KEY_L]) {
        animacionAjolote = !animacionAjolote;
    }

    if (keys[GLFW_KEY_T]) {
        animacionTragaluz = true;
        dirTraga = !dirTraga;
    }

}

void MouseCallback(GLFWwindow *window, double xPos, double yPos){
    if (firstMouse){
        lastX = xPos;
        lastY = yPos;
        firstMouse = false;
    }

    GLfloat xOffset = xPos - lastX;
    GLfloat yOffset = lastY - yPos;  // Reversed since y-coordinates go from bottom to left

    lastX = xPos;
    lastY = yPos;

    camera.ProcessMouseMovement(xOffset, yOffset);
}
