// En Tinkercad: https://www.tinkercad.com/things/3BKK1VTuI19

/* A cuántos puntos va el partido */
			int const puntos_partido = 5;
/* Cantidad de ledes en fila (menos las puntas) */
			int const cant_leds=6;
/* Mapea pines de Arduino*/
			int const ledpins[] = {5,6,7,8,9,10};
/* pines de las puntas */
			int const punta_1_pin = 4;
			int const punta_2_pin = 11;
			int const intervalo_max=1000;
/* Se setea en CalcularIntervalo() */
			int intervalo;
/* Pines de los botones en Arduino */
			int const boton_1=12;
			int const boton_2=13;
/* Puntos de los jugadores */
			int puntos_jug1=0;
			int puntos_jug2=0;
/* Ángulo máximo de potenciometro */
			int const angulo_max_pot=179;
/* Pin del Arduino que lee potenciómetro */
			int const PinPot=A0;
/* Para calcular el ángulo que tira el potenciometro */
			int ValorPot;
			int Angulo;
/* Corrimiento de resultado potenciometro */
			int corrimiento;

/* Pin del arduino que recibe datos de buzzer */
			int const parlante = 3;

/* Frecuencia de salida de parlante */
			int const tono_parlante = 261;


/* Muestra puntajes */
			void MostrarPuntos()
			{
				Serial.print("Puntos Jug 1: ");
					Serial.println(puntos_jug1);

				Serial.print("Puntos Jug 2: ");
					Serial.println(puntos_jug2);
			}

/* Lee ángulo de potenciómetro.*/
			int leerAnguloPot(){
					ValorPot=analogRead(PinPot);
					Angulo=map(ValorPot,0,1023,0,179);
					return Angulo;
			}

/* Calculo el intervalo a partir de dato angulo de potenciometro */
			int CalcularIntervalo(int angulo)
			{
				int retval;
				retval=(angulo*intervalo_max)/angulo_max_pot;
				return retval;
			}

/* Para terminar el juego muestra puntajes, ganador y activa el buzzer */
			void game_over(int ganador)
			{
				Serial.println("************************");
				MostrarPuntos();
				Serial.println("************************");
				Serial.print("GANA JUGADOR ");
				Serial.println(ganador);
				tone(parlante,tono_parlante);
				delay(500);
				noTone(parlante);
				exit(0);
			}

/* Le da un punto al jugador id_jugador (1 o 2).  Si se llega a puntos_partido (seteado arriba) termina el juego */
			void punto(int id_jugador){
			  if( id_jugador==1 ){
			    puntos_jug1++;
			    Serial.println("Punto Jug 1");
			    MostrarPuntos();
					if( puntos_jug1>=puntos_partido)
						game_over(1);
			  }
			  else if( id_jugador==2 ){
			    puntos_jug2++;
			    Serial.println("Punto Jug 2");
			    MostrarPuntos();
					if( puntos_jug2>=puntos_partido)
						game_over(2);
			  }
			}

/* La pelota viaja de jugador 1 a jugador 2 */
			void ir()
			{
				for (int i=0; i < cant_leds; i++){
			  		digitalWrite(ledpins[i],HIGH);
			    	delay(intervalo);
			    	digitalWrite(ledpins[i],LOW);
			      	delay(intervalo);
				}
			}

/* La pelota viaja de jugador 2 a jugador 1 */
			void volver()
			{
			   	for (int i=cant_leds-1; i >=0 ; i--){
			  		digitalWrite(ledpins[i],HIGH);
			    	delay(intervalo);
			    	digitalWrite(ledpins[i],LOW);
			        delay(intervalo);
				}
			}

/* Devuelve true si el botón id_boton (1 o 2) fue presionado*/
			int boton_bien(int id_boton){
			  int dr;
			  int boton_pin;
				int punta_pin;
			  int bien=false;

			  if (id_boton==1){
			    boton_pin = boton_1;
			    punta_pin = punta_1_pin;
			  }
			  else if( id_boton==2){
			    boton_pin = boton_2;
			    punta_pin = punta_2_pin;
			  }
			  digitalWrite(punta_pin,HIGH);
			  for( int i=0 ; i<=10 ; i++ ){
					delay(100);
					dr=digitalRead(boton_pin);

			  	if( dr==HIGH ){
							digitalWrite(punta_pin,LOW);
			        return true;
			        }

			  }
				 digitalWrite(punta_pin,LOW);
			   return false;
			}

			void setup()
			{
			  Serial.begin(9600);
				/*Habilito los pins que iuminan fila de leds para salida (OUTPUT)*/
			  for (int i=0; i < cant_leds; i++){
			  	pinMode(ledpins[i], OUTPUT);
			  }
				/* Habilito los pins que se conectan con los botones*/
			  pinMode(boton_1, INPUT);
			  pinMode(boton_2, INPUT);
				/* Habilito los pines que están en los leds de las puntas.  Teóricamente, cuando se prende uno de estos, el jugador respectivo debe  apretar su botón */
				pinMode(punta_1_pin, OUTPUT);
				pinMode(punta_2_pin, OUTPUT);
				/* Calculo el intervalo (depende de la posición del potenciómetro), esto es la ctd. de milisegundos que se prende el led */
				intervalo =  CalcularIntervalo(leerAnguloPot()) + corrimiento;
				pinMode(parlante, OUTPUT);
			}

			void loop(){
				/*Los leds viajan de jug. 1 a jug. 2 */
			  ir();
				/*Si el jugador 2 no aprieta el botón, hay punto para el jugador 1*/
			  if( !boton_bien(2) ){
			    punto(1);
			  }
			  /*Los leds viajan de jug. 2 a jug. 1 */
			  volver();
				/*Si el jugador 1 no aprieta el botón, hay punto para el jugador 2*/
				if( !boton_bien(1) ){
			    punto(2);
			  }
			}
