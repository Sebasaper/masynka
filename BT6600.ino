//definiowanie wejść / wyjść: trudniej się pomylić

//maszynka
#define kr_1 3
#define kr_2 2
#define kr_3 1
#define kr_4 4
#define s_p 5
#define s_t 6
//#define s_pod 12
//#define s_cof 7
#define elektropodajnika A3
#define elektrotrzymaniagilzy A1
#define srt A0
const int stepPin = 9;

const int dirPin = 8;

const int enPin = 13;

const int homeSwitchPin = A2;

void setup()
{

  pinMode(kr_1, INPUT_PULLUP);// krańcówka blokady 1
  pinMode(kr_2, INPUT_PULLUP); //  krańcówka blokady 2
  pinMode(kr_3, INPUT_PULLUP);// maszynka nabity krańcówka 1
  pinMode(kr_4, INPUT_PULLUP); // maszynka otwarta krańcówka 2
  pinMode(s_p, OUTPUT);// sterowanie nabijanie
  pinMode(s_t, OUTPUT);// sterowanie cofania nabijania
  // pinMode(s_pod, OUTPUT);// blokada gilzy
  // pinMode(s_cof, OUTPUT);//  cofania blokady gilz
  pinMode(elektropodajnika, OUTPUT);// silnik wibracji podajnika gilz
  pinMode(elektrotrzymaniagilzy, OUTPUT);//  silnik wibracji podajnika tabaki
  pinMode(srt, INPUT_PULLUP);// wyzwalanie masą

  pinMode(stepPin, OUTPUT);

  pinMode(dirPin, OUTPUT);

  pinMode(homeSwitchPin , INPUT_PULLUP);

  pinMode(enPin, OUTPUT);

  digitalWrite(enPin, LOW);

  //digitalWrite(dirPin, HIGH);
  digitalWrite(dirPin, HIGH); // Enables the motor to move in a particular direction

}

//definiuję sobie zmienną która określa, jaką akcję właśnie wykonuje system
enum stan {czekam = 0, naprzod, cofanie, podawanie, cofaniepodajnika};
void loop()
{

  Motor();
  motorStep(1);

}

void Motor()
{
  int homeSw = digitalRead( homeSwitchPin);
  //tworzę zmienną typu 'stan', która będzie przechowywać informację o aktualnie wykonywanej czynności
  static stan aktualny = czekam; //na początku czekamy na wciśnięcie

  switch (aktualny) { //w zależności od aktualnego stanu, sprawdzamy różne warunki!
    //pierwszy przypadek: bezczynność
    case czekam:
      //czekamy, aż ktoś wciśnie przycisk
      if (digitalRead(srt) == LOW) {
        // tamten kod ma sie wykonać w tym miejscu
        if ( homeSw == LOW && (digitalRead(dirPin) == HIGH)) {
          motorStep(1);
        }
        else if ( homeSw == HIGH && (digitalRead(dirPin) == HIGH) ) {
          digitalWrite(dirPin, LOW);
          delay(200);
        }
        if ( (digitalRead(dirPin) == LOW) ) {
          motorStep(4800);
          digitalWrite(dirPin, HIGH);
          delay(200);
        }

        motorStep();// Run the function
        digitalWrite(elektrotrzymaniagilzy, LOW);
        digitalWrite(elektropodajnika, HIGH);

        //    motorStep( int MAX);// Run the function
        digitalWrite(s_p, HIGH);
        digitalWrite(s_t, LOW);
        //   motorStep();// Run the function
        //zmieniamy informację o aktualnym stanie
        aktualny = naprzod;
      }
      break;

    //drugi przypadek: jedzie naprzód
    case naprzod:
      //czekamy na pierwsza krańcówkę
      if (digitalRead(kr_1) == LOW) {
        digitalWrite(s_p, LOW);
        digitalWrite(s_t, HIGH);
        digitalWrite(elektrotrzymaniagilzy, HIGH);
        digitalWrite(elektropodajnika, HIGH);

        //zmieniamy informację o aktualnym stanie
        aktualny = cofanie;
      }
      break;

    //trzeci przypadek: cofanie
    case cofanie:
      //czekamy na pierwsza krańcówkę
      if (digitalRead(kr_2) == LOW) {
        digitalWrite(s_p, HIGH);
        digitalWrite(s_t, LOW);
        digitalWrite(elektrotrzymaniagilzy, HIGH);
        digitalWrite(elektropodajnika, HIGH);


        //zmieniamy informację o aktualnym stanie
        aktualny = podawanie;
      }
      break;

    //trzeci przypadek: cofanie
    case podawanie:
      //czekamy na pierwsza krańcówkę
      if (digitalRead(kr_3) == LOW) {
        digitalWrite(s_p, LOW);
        digitalWrite(s_t, HIGH);
        digitalWrite(elektrotrzymaniagilzy, HIGH);
        digitalWrite(elektropodajnika, HIGH);

        //zmieniamy informację o aktualnym stanie
        aktualny = cofaniepodajnika;
      }
      break;

    //trzeci przypadek: cofanie
    case cofaniepodajnika:
      //czekamy na pierwsza krańcówkę
      if (digitalRead(kr_4) == LOW) {
        digitalWrite(s_p, LOW);
        digitalWrite(s_t, LOW);
        digitalWrite(elektrotrzymaniagilzy, HIGH);
        digitalWrite(elektropodajnika, LOW);
        delay(100);
        digitalWrite(elektropodajnika, HIGH);
        //zmieniamy informację o aktualnym stanie
        aktualny = czekam;
      }
      break;

  }
}

void motorStep( int MAX) {


  for (int x = 0; x < MAX; x++) {

    digitalWrite(stepPin, LOW);

    delayMicroseconds(60);

    digitalWrite(stepPin, HIGH);

    delayMicroseconds(60);
  }

}
