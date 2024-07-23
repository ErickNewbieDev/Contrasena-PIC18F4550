//Creado por ErickNewbieDev
//https://github.com/ErickNewbieDev
//Contacto directo: ericknewbiedev@protonmail.com
//BLOQUE DEL LCD
// Pin de selección del registro (RS)
sbit LCD_RS at LATE0_bit;
// Pin de habilitación (EN)
sbit LCD_EN at LATE1_bit;
// Pins de datos D4 a D7
sbit LCD_D4 at LATC0_bit;
sbit LCD_D5 at LATC1_bit;
sbit LCD_D6 at LATC6_bit;
sbit LCD_D7 at LATC7_bit;
// Dirección del pin RS
sbit LCD_RS_Direction at TRISE0_bit;
// Dirección del pin EN
sbit LCD_EN_Direction at TRISE1_bit;
// Dirección de los pines de datos
sbit LCD_D4_Direction at TRISC0_bit;
sbit LCD_D5_Direction at TRISC1_bit;
sbit LCD_D6_Direction at TRISC6_bit;
sbit LCD_D7_Direction at TRISC7_bit;
////////////////////////////////////
//Bloque del teclado
char keypadPort at PORTD;
unsigned short tecla;
unsigned short teclaReset;
////////////////////////////////////
//Bloque de metodos
void ASCII();
void validarTamano();
void validarContrasena();
void accesoConcedido();
void accesoDenegado();
void mensajeInicial();
void limpiarContrasena();
////////////////////////////////////
//Bloque de variables
unsigned int x = 0;//Variable para llevar el conteo de caracteres ingresados por el usuario
char teclaChar;//Variable que almacena el valor de la tecla pulsada pero a modo de caracter ('1','2','3',...)
char contrasena[6] = "159DA";//Contraseña correcta fija
char inputUsuario[6];//Contraseña que va siendo ingresada por el usuario
int esCorrecta = 0;//Varible "boleana" para llevar control de si la contraseña ingresada fue correcta o no
////////////////////////////////////
//Bloque principal de codigo
void main(){
//Configuraciones generales
ADCON0 = 0X00;
ADCON1 = 0X0F;
CMCON = 0X07;
//////////////////////
//Configuracion de puertos a usar
TRISA = 0X0F;//Entrada
TRISB = 0x00;//Salida
TRISD = 0X00;//Teclado
//////////////////////
//Limpiamos los puertos a usar
PORTA = 0x00;
PORTB = 0x00;
PORTD = 0x00;
/////////////
//Inicializamos el Teclado y el LCD
Keypad_init(); // Inicializar el Teclado
Lcd_Init();    // Inicializar LCD
Lcd_Cmd(_LCD_CLEAR); // Limpiar display
Lcd_Cmd(_LCD_CURSOR_OFF); // Apagar cursor
//////////////////////////////////////////
mensajeInicial();//Se llama al metodo que imprime el mensaje inicial
delay_ms(100);//Retardo de 100 ms
//BUCLE PRINCIPAL DE CODIGO
do{
tecla = 0; // Reiniciar el valor de la tecla para poder leer una nueva cada que se pulse
//Condicional para saber si el usuario pulso el boton para leer la contraseña
if(PORTA.F1==1&&x>=4){//Si, el boton fue pulsado y ademas la contraseña ya tiene una longitud valida...
validarContrasena();//Se llama al metodo que valida la contraseña
}//Termina condicional if
else{//De lo contrario, si aun no se solicita leer la contraseña, se procede con el programa de lectura y almacenado de caracteres
do{//Inicia un bucle que espera hasta que se lea un valor del teclado
      tecla = Keypad_Key_Click(); // Almacena el valor de la tecla
      //Condicional para saber si el usuario pulso el boton para leer la contraseña
      if(PORTA.F1==1&&x>=4){//Si, el boton fue pulsado y ademas la contraseña ya tiene una longitud valida...
      validarContrasena();//Se llama al metodo que valida la contraseña
      }//Termina condicional if
      //Condicional para saber si es la primera vez que se usa el teclado, esto porque asi es solicitado por el profesor en el ejercicio
      if(tecla&&x<=0){//Si se pulso alguna tecla y es la primera tecla pulsada
             Lcd_Cmd(_LCD_CLEAR); // Limpiar display ya que el programa pide que se elimine el mensaje en cuanto se oprima el primer valor
      }//Termina condicional if
}while (!tecla);//Termina la lectura del teclado
ASCII();//Se llama al metodo que asigna un valor ASCII a la tecla pulsada
Lcd_Chr(2, (x+1), inputUsuario[x]); // Visualizar el valor ASCII del la tecla en el segundo renglon en la columna x+1 (esto ya que comienza en el indice 0)
validarTamano();//Se llama al metodo que valida si la contraseña ingresada ya supera los 5 caracteres
}//Termina condicional else
} while (1);//Termina bucle principal
}//Termina metodo main

//Este metodo se encarga de determinar que caracter es el indicada dependiendo de la tecla presionada en el teclado
void ASCII(){//Inicia metodo ASCII
switch (tecla){//Inicia switch teniendo como parametro a la tecla pulsada
        default:
            tecla += 48;
        case 1:
            tecla = 49;
            break; //'1'
        case 2:
            tecla = 50;
            break; //'2'
        case 3:
            tecla = 51;
            break; //'3'
        case 4:
            tecla = 65;
            break; //'A'
        case 5:
            tecla = 52;
            break; //'4'
        case 6:
            tecla = 53;
            break; //'5'
        case 7:
            tecla = 54;
            break; //'6'
        case 8:
            tecla = 66;
            break; //'B'
        case 9:
            tecla = 55;
            break; //'7'
        case 10:
            tecla = 56;
            break; //'8'
        case 11:
            tecla = 57;
            break; //'9'
        case 12:
            tecla = 67;
            break; //'C'
        case 13:
            tecla = 42;
            break; //'*'
        case 14:
            tecla = 48;
            break; //'0'
        case 15:
            tecla = 35;
            break; //'#'
        case 16:
            tecla = 68;
            break; //'D'
        }//Termina switch
teclaChar = (char)tecla;//Almacenamos en un caracter el valor de la tecla presionada
inputUsuario[x] = teclaChar;//Almacenamos el valor anterior en el arreglo en el indice x el cual incrementa cada que se pulsa una tecla
}//Termina metodo ASCII
//Este metodo se encarga de validar que la contraseña ya haya alcanzado los 5 caracteres, pues en ese momento debe comenzar a leer una contraseña nueva
void validarTamano(){//Inicia metodo validarTamano
     if((x+1)>5){//Si el valor de x ya sobrepaso el valor admitido (cinco)
        x = 0;//x se retorna a 0
        Lcd_Cmd(_LCD_CLEAR);//Y se limpia el display
     }//Termina condicional if
     else{//Por el contrario, si la contraseña aun admite valores...
         inputUsuario[x] = (char)tecla;//Se ingresa al arreglo el valor de la tecla pulsada
         x++;//Se incrementa en 1 el valor del conteo
     }//Termina else
}//Termina metodo validarTamano
//Este metodo es el importante pues se encarga de revisar que la contraseña ingresada y la contraseña fija sean iguales
void validarContrasena(){//Inicia metodo validarContrasena
int checkSum = 0;//Esta variable se encarga de almacenar la cantidad de caracteres iguales entre ambas contraseñas
int i = 0;//Variable indice para el ciclo for
for(i=0; i<5; i++){//Ciclo for que recorre el arreglo que contiene la contraseña ingresada por el usuario
            if(inputUsuario[i]==contrasena[i]){//Si el caracter en indice i de ambas contraseñas es igual...
                    checkSum++;//Se suma en uno al conteo de caracteres iguales
            }//Termina condicional if
            if(i>=4&&checkSum<5){//Si ya se llegó al final del bucle i y no todos los caracteres fueron iguales...
                esCorrecta = 0;//La contraseña es incorrecta
            }//Termina condicinal if
            else if(i>=4&&checkSum>4){//Por el contrario si ya se llego al final del bucle y todos los caracteres fueron iguales...
                esCorrecta = 1;;//La contraseña es correcta
            }//Termina condicional else
}//Termina for
if(esCorrecta==1){//Si, la contraseña es correcta...
accesoConcedido();//Se llama al metodo que realiza las acciones correspondientes al acceso concedido
}//Termina condicinal if
else{//De lo contrario, si la contraseña es incorrecta..
accesoDenegado();//Se llama al metodo que realiza las acciones correspondientes al acceso denegado
}//Termina condicional else
}//Termina metodo validarContrasena
//Este metodo se encarga de todas las acciones al conceder el acceso
void accesoConcedido(){//Inicia metodo accesoConcedido
PORTB.F0=1;//Se enciende el puerto B0 (destinado a un LED)
Lcd_Cmd(_LCD_CLEAR);//Limpiamos el display
//Se imprime el mensaje de bienvenida al laboratorio
Lcd_Out(1, 1, "BIENVENIDO A LAB");
Lcd_Out(2, 1, "DE DISP PROGRAM");
do{//Inicia un bucle que espera hasta que se lea un valor del teclado y este sea el "#"
   teclaReset = Keypad_Key_Click();//Guardamos el valor de la tecla pulsada en la variable teclaReset
}while(teclaReset!=15);//Se termina el bucle hasta que el valor sea el signo de gato "#"
Lcd_Cmd(_LCD_CLEAR); // Limpiar display
mensajeInicial();//Se llama al metodo mensajeInicial que ademas de imprimir el mensaje inicial, restaura el valor de algunas variables
}//Termina metodo accesoConcedido
//Este metodo se encarga de todas las acciones al denegar el acceso
void accesoDenegado(){//Inicia metodo accesoDenegado
PORTB.F1=1;//Se enciende el puerto B1 (destinado a un LED)
Lcd_Cmd(_LCD_CLEAR); // Limpiar display
//Se imprime el mensaje de acceso denegado
Lcd_Out(1, 5, "ACCESO");
Lcd_Out(2, 5, "DENEGADO");
do{//Inicia un bucle que espera hasta que se lea un valor del teclado y este sea el "#"
   teclaReset = Keypad_Key_Click();//Guardamos el valor de la tecla pulsada en la variable teclaReset
}while(teclaReset!=15);//Se termina el bucle hasta que el valor sea el signo de gato "#"
Lcd_Cmd(_LCD_CLEAR); // Limpiar display
mensajeInicial();//Se llama al metodo mensajeInicial que ademas de imprimir el mensaje inicial, restaura el valor de algunas variables
}//Termina metodo accesoDenegado
//Este metodo se encarga de vaciar el arreglo con la contraseña ingresada por el usuario
void limpiarContrasena(){//Inicia metodo limpiarContrasena
int i = 0;//Variable indice para el ciclo for
for(i=0; i<5; i++){//Ciclo for que recorre todo el arreglo de la contraseña
inputUsuario[i] = '\0';//Para igualar cada indice a un caracter vacio
}//Termina for
}//Termina metodo limpiarContrasena
//Este metodo se encarga de imprimir el mensaje inicial y ademas restaura el valor de algunas variables
void mensajeInicial(){//Inicia metodo mensajeInicial
//Se imprime el mensaje inicial
Lcd_Out(1, 1, "INTRODUZCA");
Lcd_Out(2, 1, "CONTRASENA");
PORTB.F0=0;//Se apaga el puerto B0 (destinado a un LED)
PORTB.F1=0;//Se apaga el puerto B1 (destinado a un LED)
limpiarContrasena();//Se llama al metodo limpiarContrasena
esCorrecta = 0;//Se iguala a 0 la varaible "boleana" que determina si la contraseña es correcta
teclaReset = 0;//Se iguala a 0 el valor de la tecla para reset
x = 0;//Igualamos el contador x a 0
}//Termina metodo mensajeInicial
