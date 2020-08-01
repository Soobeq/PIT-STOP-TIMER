

/**************************/
const int PIT_TIME = 1501; // 35 sek
int time_for_delay = 200;


/**************************/


const int pin_ON = 2;        // URUCHOMIENIE POMIARU
const int pin_CHECK = 3;     // SPRAWDZENIE POMIARU
const int pin_LIGHT = 4;
bool pin_ON_status = 0;    //status nacisnięcia przyciusku lub uruchomienia czujek
bool pin_CHECK_status = 0;
bool open_ON = 0;
bool open_CHECK = 0;

bool timing_status[8] = {0,0,0,0,0,0,0,0};  // tablica statusu dla timing
bool burned_status[8] = {0,0,0,0,0,0,0,0}; 
unsigned long timing[8] = {0,0,0,0,0,0,0,0};          // tablica z czasami 
String resoult;  // łancuch koncowy
unsigned long tmp=0;
bool tmp_status=0;
bool delay_time_status = 0; // status uruchomienia opóźnienia między wiazdami


/* FUNKCJE */
void check_burn();
void sort_timings();
void open_port();
void create_resoult();
void stop_timings();
void increment_timing();
void clear_burned_pit();
/* FUNKCJE END */


void setup() {
  
  Serial.begin(2000000);
  pinMode(pin_ON, INPUT);
  pinMode(pin_CHECK, INPUT);
  pinMode(pin_LIGHT, OUTPUT);
  digitalWrite(pin_LIGHT, LOW);
  
}

void loop() {
  
  open_port(); // otwarcie portu
  increment_timing();
  check_burn();
  clear_burned_pit();
 // stop_timings();
  sort_timings(timing, timing_status, 8);
  create_resoult(); // tworzenie wyniku
  
  
  Serial.println(resoult);
  
    if(timing[0] == PIT_TIME+1){
      tmp++;
    }
  
//delay(10);
//digitalWrite(pin_LIGHT, LOW);
delayMicroseconds(9500);
}
void increment_timing(){
  for ( int i=0;i<8;i++){
    if(timing_status[i] == 1 && timing[i]<PIT_TIME ){
    timing[i]++;   
  }
  
  }
}//void increment timing

void stop_timings(){

  for ( int i=0;i<8;i++){
    pin_CHECK_status = digitalRead(pin_CHECK);

    
    if(pin_CHECK_status && timing[i]==PIT_TIME && open_CHECK ==0 ){
        open_CHECK=1;
        timing[i]=0;
        timing_status[i]=0;
    }else if(!pin_CHECK_status){
      open_CHECK = 0;
    }
    
    /*if(timing[i] == PIT_TIME){
    timing[i]=0;
    timing_status[i]=0;   
  }*/
  }
}

void clear_burned_pit(){
  
  

         if( timing[0] == PIT_TIME+1 && tmp > 10){
          timing[0]=0;
          timing_status[0]=0;
          tmp=0;
          digitalWrite(pin_LIGHT, LOW);
          }

        
    
  
}

void check_burn(){
   pin_CHECK_status = digitalRead(pin_CHECK);
    if(pin_CHECK_status && open_CHECK == 0){
      open_CHECK = 1;

      
      if(timing[0] < PIT_TIME){
        timing[0] = PIT_TIME+1;
        tone(A0, 2500, 10);
        digitalWrite(pin_LIGHT, HIGH);
      }
      
      if( timing[0] == PIT_TIME ){
          timing[0]=0;
          timing_status[0]=0;
          }  
        
      
      
     
      
    }else if(!pin_CHECK_status){
      open_CHECK = 0;
    }
}


void create_resoult(){
  resoult="";
  for ( int i=0 ;i<8;i++){
     /*resoult+="T:";
     resoult+= timing[i];
     resoult+=" S:";
     resoult+= timing_status[i];
    resoult+= " B:";
    resoult+= timing_burned[i];
    resoult+= " - ";
    */
     resoult+= timing[i];
     resoult+= ";";
    }// for end
    /*
    resoult+="dTime: ";
    resoult+=delay_time;
    resoult+=" sON: ";
    resoult+=delay_time_status;
    */
}//void create resoult


void open_port(){
  pin_ON_status = digitalRead(pin_ON);
  
  if(pin_ON_status){
    
    //tone(A0, 2500, 1000);

       for (int i=0;i<8;i++){
            if(timing_status[i] != 1 && open_ON == 0)
            {
              open_ON = 1;
              timing_status[i]=1;
              delay_time_status = 1;
              break;
              
            }//if end
        } // for end   
  } else{
    open_ON = 0;
  } // else end
    
}// void open port END

void sort_timings(unsigned long timing[],bool timing_status[], int size) {
    for(int i=0; i<(size-1); i++) {
        for(int o=0; o<(size-(i+1)); o++) {
            if(timing[o] < timing[o+1]) {
               int t = timing[o];
               int t2 = timing_status[o];
                    
               timing[o] = timing[o+1];
               timing[o+1] = t;

               timing_status[o] = timing_status[o+1];
               timing_status[o+1] = t2;
                } // if end
        }//for end
    }//for end
}// void sort_timings end
