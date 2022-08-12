#include <stdio.h>      // printf
#include <stdlib.h>     // exit
#include <string.h>     // memcpy, memset
#include <stdint.h>     // uint8_t

void setup() {
  Serial.begin(9600);

  uint8_t plain[16] = {0x0, 0x0, 0x0, 0x1,
                       0x0, 0x2, 0x0, 0x3,
                       0x0, 0x4, 0x0, 0x5,
                       0x0, 0x6, 0x0, 0x7};
  uint8_t inp_key[32] = {0x0, 0x1, 0x0, 0x2,
                     0x0, 0x4, 0x0, 0x5,
                     0x0, 0x6, 0x0, 0x7,
                     0x0, 0x8, 0x0, 0x9,
                     0x1, 0x0, 0x1, 0x1,
                     0x1, 0x2, 0x1, 0x3,
                     0x1, 0x4, 0x1, 0x5};

  Serial.println("Plain Text: ");
  for(int i=0; i<16; i++) {
    Serial.print(plain[i], HEX);
    Serial.print(" ");
    if((i+1)%4 == 0)
      Serial.println();
  }

  Serial.println();
  Serial.println();

  uint8_t key[32];
  memcpy(key,inp_key,32);

  uint8_t arrkey[10][32];
  keyExpansion(key,arrkey);
  
  encrypt(plain, arrkey);
  decrypt(plain, arrkey);


}

void loop() {
  
}

// for debugging
#define DEBUG_KEY_FLAG 0
#define DEBUG_CIPHER_STATE_FLAG 0

#define DEBUG_KEY(a,b,c,d,e,f) do {         \
    if (DEBUG_KEY_FLAG) {                   \
        Serial.print(a);                    \
        Serial.print(b);                    \
        Serial.print(" -- ");               \
        Serial.print(c, HEX);               \
        Serial.print(d, HEX);               \
        Serial.print(e, HEX);               \
        Serial.println(f, HEX);             \
    }                                       \
} while(0)

#define DEBUG_CIPHER_STATE(a) do {          \
    if (DEBUG_CIPHER_STATE_FLAG) {          \
        for (int r=0; r<4; r++) {           \
          Serial.print(a);                  \
          Serial.print(state[r][0], HEX);   \
          Serial.print(state[r][1], HEX);   \
          Serial.print(state[r][2], HEX);   \
          Serial.println(state[r][3], HEX); \
        }                                   \
    }                                       \
} while(0)

uint8_t xtime(uint8_t x)
 {
     return ((x<<1) ^ (((x>>7) & 1) * 0x1b));
 }
 
 #define MULTIPLY_AS_A_FUNCTION 0
 // Multiply is used to multiply numbers in the field GF(2^8)
 #if MULTIPLY_AS_A_FUNCTION
 uint8_t Multiply(uint8_t x, uint8_t y)
 {
   return (((y & 1) * x) ^
        ((y>>1 & 1) * xtime(x)) ^
        ((y>>2 & 1) * xtime(xtime(x))) ^
        ((y>>3 & 1) * xtime(xtime(xtime(x)))) ^
        ((y>>4 & 1) * xtime(xtime(xtime(xtime(x))))));
 }      
 #else  
 #define Multiply(x, y)                                \
       (  ((y & 1) * x) ^                              \
       ((y>>1 & 1) * xtime(x)) ^                       \
       ((y>>2 & 1) * xtime(xtime(x))) ^                \
       ((y>>3 & 1) * xtime(xtime(xtime(x)))) ^         \
       ((y>>4 & 1) * xtime(xtime(xtime(xtime(x))))))   \
 
 #endif

const uint8_t s_box[16]= {0xC,0x5,0x6,0xB,0x9,0x0,0xA,0xD,0x3,0xE,0xF,0x8,0x4,0x7,0x1,0x2};
const uint8_t inv_s_box[16]= {0x5,0xE,0xF,0x8,0xC,0x1,0x2,0xD,0xB,0x4,0x6,0x3,0x0,0x7,0x9,0xA};

void addRoundKey(uint8_t state[16], uint8_t key[32]) {
  for(int i=0; i<16; i++) {
    state[15-i] = state[15-i] ^ key[31-i];
  }
}

void SubByte(uint8_t state[16]) {
  for(int i=0; i<16; i++) {
    state[i] = s_box[state[i]];
  }
}

void InvSubByte(uint8_t state[16])
{
    for(int i=0; i<16; i++) {
    state[i] = inv_s_box[state[i]];
  }
}

void shiftRows(uint8_t state[16]) {
  uint8_t temp;

  temp = state[4];
  state[4] = state[5];
  state[5] = state[6];
  state[6] = state[7];
  state[7] = temp;

  temp = state[8];
  state[8] = state[10];
  state[10] = temp;
  temp = state[9];
  state[9] = state[11];
  state[11] = temp;

  temp = state[12];
  state[12] = state[15];
  state[15] = state[14];
  state[14] = state[13];
  state[13] = temp;
}

void InvShiftRows(uint8_t state[16])  
{  
    uint8_t temp;
    temp = state[7];
    state[7] = state[6];
    state[6] = state[5];
    state[5] = state[4];
    
    state[4] = temp;

    temp = state[8];
    state[8] = state[10];
    state[10] = temp;
    temp = state[9];
    state[9] = state[11];
    state[11] = temp;

    temp = state[12];

    state[12] = state[13];
    state[13] = state[14];
    state[14] = state[15];
    
    state[15] = temp;

}

uint8_t multByTwo(uint8_t x) {
    return ((x<<1) ^ (((x>>7) & 1) * 0x1b));
}

void PSub(uint8_t state[16]) 
{
  uint8_t temp[16];
  for(int i=0; i<15; i++) 
    temp[i*4 % 15] = state[i];
  temp[15] = state[15];
  state = temp; 
}

void InvPSub(uint8_t state[16])
{
  uint8_t temp[16];
  for(int i=0; i<15; i++) 
    temp[i] = state[i*4 % 15];
  temp[15] = state[15];
  state = temp; 
}


void mixColumns(uint8_t state[16]) {
  
  uint8_t temp[4];
  for(int c=0; c<4; c++) {
    temp[0] = state[4*0 + c];
    temp[1] = state[4*1 + c];
    temp[2] = state[4*2 + c];
    temp[3] = state[4*3 + c];

    state[4*0 + c] = multByTwo(temp[0]) ^ temp[1] ^ multByTwo(temp[1]) ^ temp[2] ^ temp[3];
    state[4*1 + c] = temp[0] ^ multByTwo(temp[1]) ^ temp[2] ^ multByTwo(temp[2]) ^ temp[3];
    state[4*2 + c] = temp[0] ^ temp[1] ^ multByTwo(temp[2]) ^ temp[3] ^ multByTwo(temp[3]);
    state[4*3 + c] = temp[0] ^ multByTwo(temp[0]) ^ temp[1] ^ temp[2] ^ multByTwo(temp[3]);    
  }
}

void InvMixColumns(uint8_t state[16])  
{  
  int i;
  uint8_t a,b,c,d;
  for(i=0;i<4;++i)
  {
    a = state[4*0 + i];
    b = state[4*1 + i];
    c = state[4*2 + i];
    d = state[4*3 + i];
 
    state[4*0 + i] = Multiply(a, 0x0e) ^ Multiply(b, 0x0b) ^ Multiply(c, 0x0d) ^ Multiply(d, 0x09);
    state[4*1 + i] = Multiply(a, 0x09) ^ Multiply(b, 0x0e) ^ Multiply(c, 0x0b) ^ Multiply(d, 0x0d);
    state[4*2 + i] = Multiply(a, 0x0d) ^ Multiply(b, 0x09) ^ Multiply(c, 0x0e) ^ Multiply(d, 0x0b);
    state[4*3 + i] = Multiply(a, 0x0b) ^ Multiply(b, 0x0d) ^ Multiply(c, 0x09) ^ Multiply(d, 0x0e);
   }
} 

void keyUpdate(uint8_t state[32]) {
    //First half left shifting
    for(int i=15; i>=2; i--) {
      state[i] = state[i-2];
    }
    state[0] = 0;
    state[1] = 0;

    //Addition & XOR
    for(int i=0; i<16; i++) {
      state[i] = state[i] + state[i+16];
      state[i] = state[i] ^ 0;
    }

    //Second half shifting
    for(int i=16; i<31; i++) {
      state[i] = state[i+1];
    }
    state[32] = 0;

    for(int i=16; i<32; i++) {
      state[i] = state[i] + state[i-16];
    }
}

void keyExpansion(uint8_t key[32], uint8_t arrkey[10][32]) {
  for(int i=0; i<11; i++) {
    memset(arrkey[i],key,32);
    keyUpdate(key);    //SPECK
  }
}

void encrypt(uint8_t state[16], uint8_t key[10][32]) {

  unsigned long t = micros();
  addRoundKey(state, key[0]); //PRESENT
  for(int RC=1; RC<11; RC++) {
       mixColumns(state);
       addRoundKey(state, key[RC]); //PRESENT
       shiftRows(state);        //AES
       PSub(state);             //PRESENT
  }
  t = micros() - t;
  Serial.print("Encryption time taken: ");  
  Serial.println(t);
  Serial.println();

  /*------------------Printing--------------------*/
  Serial.println("Cipher Text: ");
  for(int i=0; i<16; i++) {
    Serial.print(state[i], HEX);
    Serial.print(" ");
    if((i+1)%4 == 0)
      Serial.println();
  }
  Serial.println();
  /*---------------End of Printing----------------*/
  
}

void decrypt(uint8_t state[16], uint8_t key[10][32]) {
  
  unsigned long t1 = micros();
  for(int RC=10; RC>=1; RC--) {
     InvPSub(state);            //PRESENT
     InvShiftRows(state);       //AES
     addRoundKey(state, key[RC]);   //PRESENT
     InvMixColumns(state);      //AES
  }
  
  addRoundKey(state, key[0]);   //PRESENT
  unsigned long t2 = micros();
  
  Serial.print("Decryption time taken: ");  
  Serial.println(t2-t1);
  Serial.println();
  
  Serial.println("Deciphered Text: ");
  for(int i=0; i<16; i++) {
    Serial.print(state[i], HEX);
    Serial.print(" ");
    if((i+1)%4 == 0)
      Serial.println();
  }

  Serial.println();
}
