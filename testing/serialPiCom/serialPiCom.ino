void setup() {
	Serial.begin(115200);
}
int incoming[3];
void loop() {
	while(Serial.available() < 4);
	// fill array
	for (int i = 0; i < 4; i++){
	  incoming[i] = Serial.read();
	}
	Serial.print(incoming[0]);
	Serial.print(", ");
	Serial.print(incoming[1]);
	Serial.print(", ");
	Serial.println(incoming[2]);
}	