/*
 * CÓDIGO PARA CONTROLAR LA VELOCIDAD SIMULTÁNEA DE MÚLTIPLES SERVOS DEPENDIENDO DE UN BUCLE
 * 
 * JAVIER ROJO MUÑOZ. HACKLAB. ETSIDI. UPM. 02/10/2016
 */
#include <Servo.h>

//estructura con los datos que necesitaremos de cada servo
struct motor{
  Servo servo;    //el servo asociado
  int pin;        //el pin al que está conectado el servo.
  int angMin;     //ángulo mínimo al que vas a mover el servo
  int angMax;     //ángulo máximo al que vas a mover el servo
  int pos;        //posición que tiene el servo.
  int wpos;       //posición que deseamos que adquiera el servo.
  int mov;        //movimiento que tiene que realizar en esa iteración
  int pR;         //un parámetro de velocidad. Indica cuántos grados se moverá por iteración.Cuanto más alto, más rápido irá, pero valores altos sacrifican la "simultaneidad".
  int pL;         //otro parámetro de velocidad. Indica cada cuántas iteraciones se ejecutará el código de moverse.

  void ESTABLECER_MOTOR(int PIN, int ANGMIN,int ANGMAX,int POS, int WPOS, int MOV, int PR, int PL){     //una función para dar valores.
    pin=PIN;
    angMin=ANGMIN;
    angMax=ANGMAX;
    pos=POS;
    wpos=WPOS;
    mov=MOV;
    pR=PR;
    pL=PL;        
  }
};

int cont;             //un contador para mantener control de las iteraciones del bucle.
int potpin=A0;
motor servoA;
motor servoB;


void setup() {
  pinMode(potpin,INPUT);//opcional. Es para controlarlos con un potenciómetro.  
  
  servoA.ESTABLECER_MOTOR(9,1,179,0,0,0,1,1);
  servoB.ESTABLECER_MOTOR(10,1,179,0,0,0,2,1);//quiere decir que estará en el pin 9, de 1 a 179 grados, posición inicial 0,
                                              //y en principio hasta que no reciba estímulos no queremos que se mueva.
                                              //Se moverá dos grados cada vez que el bucle se repita 1 veces.  
  servoA.servo.attach(servoA.pin);
  servoB.servo.attach(servoB.pin);
  servoA.servo.write(servoA.pos);
  servoB.servo.write(servoB.pos);
  cont=0;
}


void loop(){


  if(cont%servoA.pL==0){                                                                                      //cada pL iteraciones del ciclo entra en el if.
    servoA.wpos=LEER_INPUT(servoA.angMin,servoA.angMax);                                                                                   //la señal de entrada en este caso es un potenciómetro.
    servoA.mov= (servoA.wpos>servoA.pos)? servoA.pR:-servoA.pR;                                                           //identifica hacia qué sentido ha de moverse.
    servoA.pos=(abs(servoA.wpos-servoA.pos)<abs(servoA.mov*servoA.pR))? servoA.wpos:servoA.pos+servoA.mov;    //si va a recorrer más ángulos de los que quedan va directamente a la posición final. Evita oscilaciones
    servoA.servo.write(servoA.pos);
  }

  if(cont%servoB.pL==0){                       
    servoB.wpos=LEER_INPUT(servoB.angMin,servoB.angMax);                                                                                   
    servoB.mov= (servoB.wpos>servoB.pos)? servoB.pR:-servoB.pR;                                                            
    servoB.pos=(abs(servoB.wpos-servoB.pos)<abs(servoB.mov*servoB.pR))? servoB.wpos:servoB.pos+servoB.mov;    
    servoB.servo.write(servoB.pos);
  }
    
  cont++;
  delay(10);
}

int LEER_INPUT(int a, int b){
  return map(analogRead(potpin),0,1023,a,b);       //la función sirve principalmente para facilitar la comprensión del código.
}                                                  //Es en esta función donde hay que incluir la transformación input-grados que quieres que gire

