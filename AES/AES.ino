#include <AES.h>
#include <AES_config.h>
#include <printf.h>

AES aes;

//2b7e151628aed2a6abf7158809cf4f3c
byte key[] = {
  0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15
};

//6bc1bee22e409f96e93d7e117393172a
byte plain[] = {
  0x30 , 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37
};

int plainLength = sizeof(plain);  // don't count the trailing /0 of the string !
int padedLength = plainLength + N_BLOCK - plainLength % N_BLOCK;

//real iv = iv x2 ex: 01234567 = 0123456701234567
unsigned long long int my_iv = 01234567;

void setup()
{
  Serial.begin(9600);
  while (!Serial) {
    yield();
  }
  printf_begin();
  delay(500);

   prekey_test();
  
}

void loop()
{

}

void prekey(int bits)
{
  //aes.iv_inc();
  byte iv[N_BLOCK];
  byte plain_p[16];
  byte cipher[16];
  byte check[16];
  unsigned long ms = micros();
  aes.set_IV(my_iv);
  aes.get_IV(iv);
  
  aes.do_aes_encrypt(plain, plainLength, cipher, key, bits, iv);
  ms = micros() - ms;
  Serial.print("Encryption took: ");
  Serial.println(ms);
  
  ms = micros();
  aes.set_IV(my_iv);
  aes.get_IV(iv);
  aes.do_aes_decrypt(cipher, padedLength, check, key, bits, iv);
  Serial.print("Decryption took: ");
  Serial.println(micros() - ms);

  Serial.print("KEY :");
  aes.printArray(key, 16);
  Serial.println();
  
  Serial.print("PLAIN :");
  aes.printArray(plain, (bool)true);
  Serial.println();
  
  Serial.print("CIPHER:");
  aes.printArray(cipher, (bool)false);
  Serial.println();
  
  Serial.print("CHECK :");
  aes.printArray(check, (bool)true);
  Serial.println();
  
  Serial.print("\nIV    :");
  aes.printArray(iv, 16);
  Serial.println();
  Serial.println();
}

void prekey_test()
{
  prekey(128);
}
