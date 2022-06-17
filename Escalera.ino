class ficha {
  public:
    String direcc;
    int pin;
    int estado; //0 apagdo - 1 encendido
  
     ficha(){
     }
     ficha(int x){
      estado = 1;
      if (x<3){
        direcc = "R";
        pin = x+2;
      }
      else if (x>3){
        direcc = "L";
        pin = x+2;
      }
      else {
        direcc = "X";
        pin = x+2;
        estado = 0;
      }
    }
    void pri(){
      Serial.print(direcc);
    }
  
};

//Variables globales
int nPines = 7;
int pines[] = {2,3,4,5,6,7,8};
ficha arr[7];
ficha arrAnt[7];

int cont = 0;

//Encender led rojo
void ledR(){
  digitalWrite(12, HIGH);
  digitalWrite(11, LOW);

}
//Encender led verde
void ledV(){
  digitalWrite(12, LOW);
  digitalWrite(11, HIGH);
}


/**
 * Se ponen los pines del 2 al 9 como entradas
 * Mientras que el 11 y 12 serán salidas
 */
void inicializar(){
  
  
  for(int x=0; x<nPines; x++){
    pinMode(pines[x], INPUT);
  }
  
  pinMode(11,OUTPUT); //VERDE
  pinMode(12,OUTPUT); //ROJO
  
  //Se inicializacn todas las fichas
  for (int x=0; x<7; x++){
    arr[x] = ficha(x);
    arrAnt[x] = ficha(x);
  }

  ledR();
}




void updateArr(){
  for(int x=0; x<7; x++){
    int value = digitalRead(arr[x].pin);
      if(value == HIGH){
        arr[x].estado = 1;
      }
      else {
        arr[x].estado = 0;
      }
      Serial.print(arr[x].estado);
      Serial.print(arr[x].direcc);
  }
  Serial.println("");
}
void updateAnt(){
  for(int x=0; x<7; x++){
      //Nota tener cuidado de que si se pueda hacer así
      arrAnt[x] = arr[x];    
  }
  Serial.println("Actualizada");
}

//Se eciende el led verde en caso de que pueda mover, ya que se ve que se tiene guardado el anterior estado.
bool permiso(){
  bool perm = true;
  for(int x=0; x<7; x++){
    //En caso que haya cambio se debe actualizar, y esperar autorización para mover
    if(arr[x].estado != arrAnt[x].estado){
      Serial.println("Cambio");
      perm = false;
      ledR();
      // break();
    }
  }
  Serial.println(perm);
  if (perm){
    ledV();
  }
  return perm;
  
}


void validarMovimiento(){
  //Validacion que no hay movido mas de una ficha
  if (Hamming() > 2){
    Serial.print("Se movieron más de 2 fichas");
    exit(0);
  }else {
    validarCambio();
  }
  
  
}

void validarCambio(){
  int y = -1;
  int r = 0;
  for(int x=0; x<7; x++){
    //En caso que haya cambio se debe actualizar, y esperar autorización para mover
    if(arr[x].estado != arrAnt[x].estado){
      if (y == -1)
        y = x;
      ledR();
      r = x;
    }
  }

  String izq = arrAnt[y].direcc;
  String der = arrAnt[r].direcc;
  if(izq.compareTo("L")){
    Serial.print("MOVIO UNA IZQUIERDA A LA DERECHA");
    exit(0);
  }else if(der.compareTo("R")){
    Serial.print("MOVIO UNA DERECHA A LA IZQUIERDA");
    exit(0);
  } else {
    Serial.print("Se hara cambio");

    arr[y] = arrAnt[r];
    arr[r] = arrAnt[y];
  }

}


int Hamming(){
  cont = 0;
  for(int x=0; x<7; x++){
    //En caso que haya cambio se debe actualizar, y esperar autorización para mover
    if(arr[x].estado != arrAnt[x].estado){
      cont++;
    }
  return cont;
  
}}


void setup() {
  Serial.begin(9600);
  inicializar();
}

void loop() {
  delay(10000); //Espera de 5 segundos
  updateArr();
  if (permiso()){
    Serial.print("Hola");
  }else{
    validarMovimiento();
  }
  updateAnt();


}
