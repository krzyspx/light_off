int timeon1 = 4 * 60; //czas zał 4 h
//int timeon2 = 9; //czas zał
//int timeoff = 12; //czas wył

byte flagaled = 0;

void save_data ();
void read_data ();
void loff_main();

void GSon() {
  // Serial.print("GS  on  ");   Serial.println(stan_programu);
  digitalWrite(led_green, HIGH); //przekaźnik on
  Blynk.setProperty(V19, "offLabel", "L_ON");
  Blynk.setProperty(V19, "color", _YELLOW);
  akcja.updateInterval(2, 500);

}
void GSoff() {
  //  Serial.print("GS  off  ");   Serial.println(stan_programu);
  digitalWrite(led_green, LOW);
  stan_programu = 0;
  Blynk.setProperty(V19, "color", _GREEN); //LED
  Blynk.setProperty(V19, "offLabel", "L_OFF");
  akcja.updateInterval(2, 5000);

}
void loff_setup1() {  //załączenie po podaniu napięcia na SONOFF
  read_data(); //odtworzenie czasu z pamięci
  stan_programu = 1; //załączenie przekaźnika po starcie napięcia zasilania
  loff_main();
}
void loff_setup()
{

  Blynk.virtualWrite(V22, timeon1);
  Blynk.setProperty(V22, "color", _YELLOW);
  //  Blynk.virtualWrite(V23, timeoff);
  //  Blynk.setProperty(V23, "color", _BLUE);
  //  Blynk.virtualWrite(V24, timeon2);
  //  Blynk.setProperty(V24, "color", _RED);
  //  Blynk.setProperty(V21, "color", _MAGENTA);
}

BLYNK_WRITE(V11) {    //vprzycis załączenia przekaźnika
  int i = param.asInt();
  if (i) GSon(); else GSoff;
}

BLYNK_WRITE(V12) {    // załączenia przekaźnika przy zbliżaniu się do domu
  int i = param.asInt();
  if (i) {
    stan_programu++;
    gstime = 0;
    loff_main();
  }
}
BLYNK_WRITE(V19) {    //vprzycis załączenia przekaźnika
  int i = param.asInt();
  if (i) {
    stan_programu++;
    gstime = 0;
    loff_main();
  }
}

BLYNK_WRITE(V22) {     //nowe wartości czasu
  timeon1 = param.asInt();
  save_data ();
}
/*
  BLYNK_WRITE(V23) {
  timeoff = param.asInt();
  save_data ();
  }
  BLYNK_WRITE(V24) {
  timeon2 = param.asInt();
  save_data ();
  }
*/
void loff_main() // główna procedura
{
  //  Serial.print("gstime  ");    Serial.println(gstime);
  Blynk.setProperty(V20, "offLabel", String(gstime));
  Blynk.virtualWrite(V21, gstime);

  switch (stan_programu) {
    case 1: {
        GSon();
        gstime++;
        if (gstime >= timeon1) {
          gstime = 0;
          stan_programu = 0;
        }
      } break;
    /*
        case 2: {
            GSoff();
            Blynk.setProperty(V19, "offLabel", "Wait");
            Blynk.setProperty(V19, "color", _BLUE);
            akcja.updateInterval(2, 1000);
            gstime++;
            if (gstime >= timeoff) {
              gstime = 0;
              stan_programu = 3;
            }
          } break;

        case 3: {
            GSon();
            Blynk.setProperty(V19, "offLabel", "Boil");
            Blynk.setProperty(V19, "color", _RED); //LED
            akcja.updateInterval(2, 2000);
            gstime++;
            if (gstime >= timeon2) {
              gstime = 0;
              stan_programu = 0;
            }
          } break;
    */
    default:
      {
        GSoff();
      } break;
  }
}


