/*
 * test.c
 *
 *  Created on: 26.07.2019
 *      Author: localo
 */
#include "ice.h"
#include <stdio.h>

void ice_decrypt(unsigned char* src, unsigned char* dst, int len, const unsigned char *key)
{
	int bytes_left;
	int block_size;

	ICE_KEY* ik = ice_key_create(2);

	ice_key_set(ik, key);

	block_size = ice_key_block_size(ik);


	bytes_left = len;
	while(bytes_left >= block_size){
		ice_key_decrypt(ik, src, dst);
		src+=block_size;
		dst+=block_size;
		bytes_left-=block_size;
	}
	for(int i = 0; i< bytes_left;i++){
		dst[i]=src[i];
	}

	dst-=len;
	ice_key_destroy(ik);
}

void ice_encrypt(unsigned char* src,unsigned char* dst, int len, const unsigned char *key)
{
	int bytes_left;
	int block_size;

	ICE_KEY* ik = ice_key_create(2);

	ice_key_set(ik, key);

	block_size = ice_key_block_size(ik);

	bytes_left = len;
	while(bytes_left >= block_size){
		ice_key_encrypt(ik, src, dst);
		src+=block_size;
		dst+=block_size;
		bytes_left-=block_size;
	}
	for(int i = 0; i< bytes_left;i++){
		dst[i]=src[i];
	}

	dst-=len;
	ice_key_destroy(ik);
}

int main(int argc , char ** argv){
	const unsigned char key[] = "1234567890123456";
	unsigned char test[] = "Hello World\n";

	unsigned char buf[13];
	unsigned char out[13];

	ice_encrypt(test, buf, 13, key);
	for(int i = 0; i< 13;i++){
		printf("%c",buf[i]);
	}

	ice_decrypt(buf, out, 13, key);
	for(int i = 0; i< 13;i++){
		printf("%c",out[i]);
	}
}
