#ifndef DES_h
#define DES_h

#include "DES_config.h"
#define tdea_enc tdes_enc
#define tdea_dec tdes_dec

class DES
{
	public:
		
		void encrypt(void* out, const void* in, const void* key);
		void decrypt(void* out, const void* in, const uint8_t* key);
		void tripleEncrypt(void* out, void* in, const void* key);
		void tripleDecrypt(void* out, void* in, const uint8_t* key);
		
		DES();
		void init(const void* m_key,unsigned long long int IVCl);
		void init(const void* m_key);
		void change_key(const void* m_key);
		
		void set_IV(unsigned long long int IVCl);
		void get_IV(byte *out);
		unsigned long long int get_IV_int();
		
		void iv_inc();
		
		byte* get_key();
		
		int get_size();
		
		void set_size(int sizel);
		
		void calc_size_n_pad(int p_size);
		
		void padPlaintext(void* in,byte* out);
		
		bool CheckPad(byte* in,int size);
		
		void tdesCbcEncipher(byte* in,byte* out);
		
		void tdesCbcDecipher(byte* in,byte* out);
		
		void printArray(byte output[],bool p_pad = true);
		
		void printArray(byte output[],int sizel);
		
		void do_3des_encrypt(byte *plain,int size_p,byte *cipher,const void *key, bool inc = false);
	
		
	void do_3des_decrypt(byte *cipher,int size_c,byte *plain,const void *key, unsigned long long int ivl);
		#if defined(DES_LINUX)
			
			double millis();
		#endif
	private:
		void permute(const uint8_t *ptable, const uint8_t *in, uint8_t *out);
		
		void changeendian32(uint32_t * a);
		
		inline void shiftkey(uint8_t *key);
		
		inline void shiftkey_inv(uint8_t *key);
		
		inline uint64_t splitin6bitwords(uint64_t a);
		
		inline uint8_t substitute(uint8_t a, uint8_t * sbp);
		
		uint32_t des_f(uint32_t r, uint8_t* kr);
		byte key[24];									/**< holds the key for the encryption */
		unsigned long long int IVC;						/**< holds the initialization vector counter in numerical format. */
		byte iv[8];										/**< holds the initialization vector that will be used in the cipher. */
		int pad;										/**< holds the size of the padding. */
		int size;										/**< hold the size of the plaintext to be ciphered */
		#if defined(DES_LINUX)
			timeval tv;									/**< holds the time value on linux */
		#endif
			
		byte arr_pad[7];								/**< holds the hexadecimal padding values, initialisation in the constructor */
};

#endif
