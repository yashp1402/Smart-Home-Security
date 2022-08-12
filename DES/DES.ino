#include "DES.h"

DES des;

void setup() {
  Serial.begin(9600);
  
  desTest();
  tdesTest();
  delay(2000);
}

void loop() {

}

void desTest()
{
  byte out[8];
  byte in[] = { 0, 1, 2, 3, 4, 5, 6, 7 };
  byte key[] = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07 };
  
  Serial.println();
  Serial.println("========= DES test ==========");
  
  //encrypt
  Serial.print("Encrypt...");
  unsigned long time = micros();
  des.encrypt(out, in, key);
  time = micros() - time;
  Serial.print("done. (");
  Serial.print(time);
  Serial.println(" micros)");
  printArray(out);
  
  //decrypt
  for (int i = 0; i < 8; i++)
  {
    in[i] = out[i];
  }
  Serial.print("Decrypt...");
  time = micros();
  des.decrypt(out, in, key);
  time = micros() - time;
  Serial.print("done. (");
  Serial.print(time);
  Serial.println(" micros)");
  printArray(out);
}

void tdesTest()
{
  byte out[8];
  byte in[] = { 1, 2, 3, 4, 5, 6, 7, 8 };
  byte key[] = { 
                  0x3b, 0x38, 0x98, 0x37, 0x15, 0x20, 0xf7, 0x5e, // key A
                  0x92, 0x2f, 0xb5, 0x10, 0xc7, 0x1f, 0x43, 0x6e, // key B
                  0x3b, 0x38, 0x98, 0x37, 0x15, 0x20, 0xf7, 0x5e, // key C (in this case A)
                };
  
  Serial.println();
  Serial.println("====== Triple-DES test ======");
  
  //encrypt
  Serial.print("Encrypt...");
  unsigned long time = micros();
  des.tripleEncrypt(out, in, key);
  time = micros() - time;
  Serial.print("done. (");
  Serial.print(time);
  Serial.println(" micros)");
  printArray(out);
  
  //decrypt
  for (int i = 0; i < 8; i++)
  {
    in[i] = out[i];
  }
  Serial.print("Decrypt...");
  time = micros();
  des.tripleDecrypt(out, in, key);
  time = micros() - time;
  Serial.print("done. (");
  Serial.print(time);
  Serial.println(" micros)");
  printArray(out);
}

void printArray(byte output[])
{
  for (int i = 0; i < 8; i++)
  {
    if (output[i] < 0x10)
    {
      Serial.print("0");
    }
    Serial.print(output[i], HEX);
    Serial.print(" ");
  }
  Serial.println();
}
