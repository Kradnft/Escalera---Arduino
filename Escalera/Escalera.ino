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
int cero = 0;
int ceroAnt =0;
int cont = 0;
// int vidas = 5;

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
    pinMode(pines[x], INPUT_PULLUP);
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
  ceroAnt = cero;
  cero = -1;
  for(int x=0; x<7; x++){
    int value = digitalRead(arr[x].pin);
      if(value == HIGH){
        arr[x].estado = 1;
      }else {
        arr[x].estado = 0;
        if (cero=-1){
          cero = x;
        } else{
          ceroAnt = x;
        }
        
      }      
  }
  for(int x=0; x<7; x++){
      Serial.print(arr[x].estado);    
  }
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
      Serial.println("Moviendo");
      delay(200);
      //A probar
      bool cambio = false;
      if(cero==ceroAnt){
        cero = -1;
        for(int x=0; x<7; x++){
          int value = digitalRead(arr[x].pin);
          if(value != HIGH){
            if (cero=-1){
              cero = x;
            } else{
              ceroAnt = x;
            }
          }
        }
      }
      while(!cambio){
        int value = digitalRead(arr[cero].pin);
        int value2 = digitalRead(arr[ceroAnt].pin);
        if (value == HIGH || value2 == HIGH){
          cambio= true;
        }
      }


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
  int cont = 0;
  int r = 0;
  for(int x=0; x<7; x++){
    //En caso que haya cambio se debe actualizar, y esperar autorización para mover
    if(arr[x].estado != arrAnt[x].estado){
      if (y == -1)
        y = x;
      ledR();
      r = x;
      cont = cont + 1;
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
  if (cont != 0){
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
    delay(500);
  }
  }
  else {
    Serial.println("Misma posicion");
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



String aunPuede[] = {"RXRLRLL", "RRXRLLL","RRLRXLL","RRLRLXL", "RRLXLRL","RXLRLRL","XRLRLRL","LRXRLRL", "LRLRXRL", "LRLRLRX", "LRLRLXR", "LRLXLRR", "LXLRLRR", "LLXRLRR", "LLLRXRR", "RXRLRLL", "RLRXRLL", "RLRLRXL", "RLRLRLX", "RLRLXLR", "RLXLRLR", "XLRLRLR", "LXRLRLR", "LLRXRLR","LLRLRXR", "LLRLXRR", "LLXLRRR"};
String perdio[] = {"RRRLLLX","XRRRLLL", "RRLRLLX", "RRLLLXR","LRLLXRR", "LXRRLRL",  "RLLLXRR", "LXRRLRL", "RRLLLXR","LLXRRRL","RRRLLLX", "XRRLRLL", "LXRRRLL", "LLXRRLR", "RLRLLXR", "RLLXRRL"};
String casiPerdio[] = {"RXLRRLL","XRLRRLL","LRXRRLL","XLRLRRL", "XLRRRLL","RLXRRLL","RRRLLXL","RLXRLRL", "RLLRXRL", "XLRRLRL", "RLLRLRX", "RLLXLRR", "RLLRLXR", "RRLXRLL", "XLRRLRL", "LRLXRRL", "LXLRRRL", "RLLXRLR", "RLLLRXR","RLRLXRL", "RLXLRRL", "RLRLLRX"};
String valTab(){
  String cadena ="";
  for (int x=0; x<7; x++){
    cadena +=arr[x].direcc;
    }
  if (cadena == "LLLXRRR" ){
    return("Gano :)");
    Serial.println("Gano :");
  }
   if (cadena.equals("RRRXLLL")){
    return("Iniciando");
  } 

  // while (x<sizeof(aunPuede));
  for (int x = 0; x<(sizeof(aunPuede)/sizeof(aunPuede[0])); x++){
    if (aunPuede[x].equals(cadena)){
      return ("Con opciones de ganar");
    }
  }
  
  for (int x = 0; x<(sizeof(perdio)/sizeof(perdio[0])); x++){
    if (perdio[x] == cadena){
      return ("Sin movimientos posibles, perdio");
    }
  }
  for (int x = 0; x<(sizeof(casiPerdio)/sizeof(casiPerdio[0])); x++){
    if (perdio[x] == cadena){
      return ("Sin movimientos posibles, perdio");
    }
  }
  if(cadena.indexOf("RRLL") != -1){
    return("Sin opciones de ganar");
  }
  else{
    return ("Moviemiento no encontrado");
  }
}

void setup() {
  Serial.begin(9600);
  inicializar();
  ledV();
}

void loop() {

  delay(500); //Espera de 1/2 segundo
  updateArr();
  delay(100);
  if (permiso()){
    Serial.println(valTab());
  }else{
    updateArr();
    validarMovimiento();
  }
  updateAnt();


}
