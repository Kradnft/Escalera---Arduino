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
void term(){
  digitalWrite(12, HIGH);
  digitalWrite(11, HIGH);
  delay(2000);
  exit(0);
}


/**
 * Se ponen los pines del 2 al 8 como entradas
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
      }else {
        arr[x].estado = 0;
      }
      Serial.print(arr[x].estado);
      
  }
  for(int x=0; x<7; x++){
      Serial.print(arr[x].direcc);    
  }
  Serial.println("");
}
void updateAnt(){
  for(int x=0; x<7; x++){
      //Nota tener cuidado de que si se pueda hacer así
      arrAnt[x] = arr[x];    
  }
}

//Se enciende el led verde en caso de que pueda mover, ya que se ve que se tiene guardado el anterior estado.
bool permiso(){
  bool perm = true;
  for(int x=0; x<7; x++){
    //En caso que haya cambio se debe actualizar, y esperar autorización para mover
    if(arr[x].estado != arrAnt[x].estado){
      perm = false;
      ledR();
      delay(5000);
      // break();
    }
  }
  if (perm){
    ledV();
  }
  return perm;
  
}


void validarMovimiento(){
  //Validacion que no hay movido mas de una ficha
  
  if (Hamming() > 2){
    Serial.print("Se movieron más de 2 fichas");
    term();
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
  if (r==y){
    Serial.println("Solo movio una ficha");
    term();
  }
  int dif = abs(r-y);
  if (dif > 2){
    Serial.print("Movimiento muy largo");
    term();
  }
  
  String izq = arrAnt[y].direcc;
  String der = arrAnt[r].direcc;
  if(izq.equals("L")){
    Serial.print("MOVIO UNA IZQUIERDA A LA DERECHA");
    term();
  }else if(der.equals("R")){
    Serial.print("MOVIO UNA DERECHA A LA IZQUIERDA");
    term();
  } else {
    Serial.println("Se hara cambio");
    arr[y].direcc = arrAnt[r].direcc;
    arr[r].direcc = arrAnt[y].direcc;
  }

}


int Hamming(){
  cont = 0;
  for(int x=0; x<7; x++){
    //En caso que haya cambio se debe actualizar, y esperar autorización para mover
    if(arr[x].estado != arrAnt[x].estado){
      cont++;
    }
  
  }
  return cont;
}


String aunPuede[] = {"RRXRLLL","RRLRXLL","RRLRLXL", "RRLXLRL","RXLRLRL","XRLRLRL","LRXRLRL", "LRLRXRL", "LRLRLRX", 
"LRLRLXR", "LRLXLRR", "LXLRLRR", "LLXRLRR", "LLLRXRR"};
String perdio[] = {"XRRRLLL","RRLRLLX","RRLLLXR","LRLLXRR", "LXRRLRL", "RLLXRRL", "RLLLXRR", "LXRRLRL", "RRLLLXR", "LXRRRLL", "LXRRRLL", "LLXRRRL"};
String casiPerdio[] = {"RXRRLLL","RRLLXRL","RRLLLRX", "RLXRLRL", "RLLRXRL", "XLRRLRL", "RLLRLRX", "RLLXLRR", "RLLRLXR", "RRLXRLL", "XLRRLRL", "RRLLRXL", 
"RRLLRLX", "RRLLXLR", "RXLRRLL", "XRLRRLL", "LRXRRLL", "RLXRRLL", "XLRRRLL", "LRLXRRL", "LXLRRRL"};


String valTab(){
  String cadena ="";
  for (int x=0; x<7; x++){
    cadena +=arr[x].direcc;
  }
  if (cadena.equals("LLLXRRR")){
    return("Gano :)");
  }
  for (int x = 0; x<sizeof(casiPerdio); x++){
    if (casiPerdio[x].equals(cadena)){
      return ("Sin opciones de ganar, aun puede mover");
    }
  }

  for (int x = 0; x<sizeof(aunPuede); x++){
    if (aunPuede[x].equals(cadena)){
      return ("Con opciones de ganar");
    }
  }
  for (int x = 0; x<sizeof(perdio); x++){
    if (perdio[x].equals(cadena)){
      return ("Sin movimientos posibles, perdio");
    }
  }
  if(cadena.indexOf("RRLL") != -1){
    return("Sin opciones de ganar");
  } else {
    return("Movimiento no reconocido");
  }
 
}


void setup() {
  Serial.begin(9600);
  inicializar();
  ledV();
}

void loop() {
  delay(1000); //Espera de 1 segundo
  updateArr();
  valTab();
  if (permiso()){
    Serial.println("Puede mover");
    Serial.println(valTab());
  }else{
    validarMovimiento();
  }
  updateAnt();


}
