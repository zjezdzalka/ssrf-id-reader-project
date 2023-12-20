#include <SoftwareSerial.h>

const int BUFFER_SIZE = 14; 
const int DATA_SIZE = 10; 
const int DATA_VERSION_SIZE = 2; 
const int DATA_TAG_SIZE = 8;
const int CHECKSUM_SIZE = 2;

SoftwareSerial ssrfid = SoftwareSerial(6,8); 


uint8_t buffer[BUFFER_SIZE];
int buffer_index = 0;

void setup() {
 Serial.begin(9600); 
 
 ssrfid.begin(9600);
 ssrfid.listen(); 
 digitalWrite(2, HIGH);
 Serial.println("Program finished loading!");
}
void loop() {
  bool armed = true;
  bool printed = false;
  if (armed == true){
    int cardIDLength = String(ssrfid.read()).length();
    if (cardIDLength > 5) { // aby sprawdzalo czy jest karta, caly ten system jest do poprawy bo póki co nie do końca działą, ani nie mieliśmy jak przetestować
      if(!printed){
        bool call_extract_tag = false;
        int ssvalue = ssrfid.read(); 
        if (ssvalue == -1) { 
          return;
        }

        if (ssvalue == 2) { 
          buffer_index = 0;
        } else if (ssvalue == 3) {       
          call_extract_tag = true; 
        }

        if (buffer_index >= BUFFER_SIZE) { 
          Serial.println("Error: Buffer overflow detected! ");
          return;
        }
        
        buffer[buffer_index++] = ssvalue; 
        unsigned tag;
        
        if (call_extract_tag == true) {
          if (buffer_index == BUFFER_SIZE) {
            tag = extract_tag();
          } else { 
            buffer_index = 0;
            return;
          }
        }
        bool temp;
        switch (tag){

          temp = 0;
          //tutaj są do poszczególnych id'ków
          default:
            digitalWrite(2, LOW);
            Serial.println("Scanned");
            printed = true;
            temp = 1;
            break;
        }   
        if(!temp==1){
          Serial.println("Something went wrong.");
          
        }
      }
    }
    
    }   
  }
  digitalWrite(2, HIGH);
}

unsigned extract_tag() {
    uint8_t msg_head = buffer[0];
    uint8_t *msg_data = buffer + 1; 
    uint8_t *msg_data_version = msg_data;
    uint8_t *msg_data_tag = msg_data + 2;
    uint8_t *msg_checksum = buffer + 11;
    uint8_t msg_tail = buffer[13];
    
    Serial.println("--------");
    
    for (int i = 0; i < DATA_TAG_SIZE; ++i) {
      Serial.print(char(msg_data_tag[i]));
    }
    Serial.println(" (tag)");

    

    Serial.println("--");

    long tag = hexstr_to_value(msg_data_tag, DATA_TAG_SIZE);
    Serial.print("Extracted Tag: ");
    Serial.println(tag);

    

    Serial.println("");
    Serial.println("--------");

    return tag;

}
long hexstr_to_value(char *str, unsigned int length) { 
  char* copy = malloc((sizeof(char) * length) + 1); 
  memcpy(copy, str, sizeof(char) * length);
  copy[length] = '\0'; 
  long value = strtol(copy, NULL, 16);  
  free(copy); 
  return value;
}