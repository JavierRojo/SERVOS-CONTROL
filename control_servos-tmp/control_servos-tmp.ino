/*
 * CÓDIGO PARA CONTROLAR LA VELOCIDAD SIMULTÁNEA DE MÚLTIPLES SERVOS
 * 
 * JAVIER ROJO MUÑOZ. HACKLAB. ETSIDI. UPM. 02/10/2016
 */
#include <Servo.h>
#define n 2         //número de servos que deseamos coordinar. 

//estructura con los datos que necesitaremos de cada servo
struct motor{
  Servo servo;      //el servo asociado
  int pin;          //el pin al que está conectado el servo.
  int angMin;       //ángulo mínimo al que vas a mover el servo
  int angMax;       //ángulo máximo al que vas a mover el servo
  int pos;          //posición que tiene el servo.
  int wpos;         //posición que deseamos que adquiera el servo (wanted position).
  int mov;          //movimiento que tiene que realizar en esa iteración.
  int vel;          //un parámetro de velocidad. Indica cuántos grados se moverá por iteración.Cuanto más alto, más rápido irá, pero valores altos sacrifican la "simultaneidad".
  int per;          //Periodo. Indica cada cuántos milisegundos han de pasar hasta que vuelva a moverse el motor.
  long TiempoRef;   //tiempo de referencia para el ciclo.

  void ESTABLECER_MOTOR(int PIN, int ANGMIN,int ANGMAX,int POS, int WPOS, int MOV, int VEL, int PER){        //una función para dar valores(constructor)
    pin=PIN;    angMin=ANGMIN;    angMax=ANGMAX;    pos=POS;    wpos=WPOS;    mov=MOV;    vel=VEL;    per=PER;  TiempoRef=millis();      
  }
};

int potpin=A0;  
motor lista[n];                                      //declaramos un vector de tantos servos como queramos coordinar.


void setup() {
  pinMode(potpin,INPUT);                             //opcional. Es para controlarlos con un potenciómetro. 
  
  lista[0].ESTABLECER_MOTOR(9,1,179,0,0,0,1,1);
  lista[1].ESTABLECER_MOTOR(10,1,179,0,0,0,2,5);
  /*Traducción del segundo caso: quiere decir que estará en el pin 10, de 1 a 179 grados, posición inicial 0,
   * y en principio hasta que no reciba estímulos no queremos que se mueva.
   * Se moverá 2 grados cada vez que pasen 5 milisegundos.
   */
  for(int i=0;i<n;i++){
    lista[i].servo.attach(lista[i].pin);
    lista[i].servo.write(lista[i].pos);
  }
}


void loop(){
  unsigned long TiempoActual;                                                                                                            //esta variable acumulará el tiempo.         
  for(int i=0;i<n;i++){                                                                                                                  //recorremos todos nuestros motores.
    TiempoActual=millis();                                                                                                               //medimos en qué instante estamosy lo usaremos para nuestro servo.
    if(TiempoActual-lista[i].TiempoRef>lista[i].per){                                                                                    //Si ya han pasado los "per" milisegundos desde nuestro último tiempo de referencia entonces entra en el if.
      lista[i].wpos=LEER_INPUT(i);                                                                       //la señal de entrada en este caso es un potenciómetro.
      lista[i].mov= (lista[i].wpos>lista[i].pos)? lista[i].vel:-lista[i].vel;                                                            //identifica hacia qué sentido ha de moverse.
      lista[i].pos = ( abs(lista[i].wpos-lista[i].pos) < abs(lista[i].mov*lista[i].vel))? lista[i].wpos :lista[i].pos + lista[i].mov;    //si va a recorrer más ángulos de los que quedan va directamente a la posición final. Evita oscilaciones
      lista[i].servo.write(lista[i].pos);                                                                                                //enviamos la posición que queremos al servo (normalmente 1 ó 2 grados distinta de la actual.
      lista[i].TiempoRef=TiempoActual;                                                                                                   //actualizamos nuestro tiempo de referencia
    }
  }    
}

int LEER_INPUT(int j){
  /*esta es la función para transformar el input en el ángulo al que quieres que vaya el servo.
   * En este caso es inútil ya que es igual para ambos, pero así es genérico. Además facilita
   * la lectura del código principal
   * Utiliza el índice para poder reaccionar de forma distinta a cada motor, y los ángulos mínimo y máximo de ese motor.
   * 
   * 
   *Para este caso concreto: constrain sirve para asegurarse en cualquier caso de que el valor devuelto está dentro de nuestros límites.
   *si ocurre algo extraño (default) le haremos pensar que no queremos que se mueva o, mejor dicho, que se mueva a donde ya está.
   */
  switch(j){
    case 0:
      return constrain( map( analogRead(potpin),0,1023,lista[j].angMin,lista[j].angMax ), lista[j].angMin, lista[j].angMax );
      break;
    case 1:
      return constrain( map( analogRead(potpin),0,1023,lista[j].angMin,lista[j].angMax ), lista[j].angMin, lista[j].angMax );
      break;
    default:
      return lista[j].pos;
      break;
  }
}

