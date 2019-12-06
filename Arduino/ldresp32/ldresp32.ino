float valorLDR;
int LDR;

void setup (){
    Serial.begin(9600);
    pinMode(13, INPUT); 
}

void loop (){
    valorLDR = analogRead(13);
    LDR = map(valorLDR, 600, 23, 0, 100);
    Serial.print(LDR);
    Serial.println("%");
    delay(1000);
}
