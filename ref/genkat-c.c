/*
   BLAKE2 reference source code package - reference C implementations
  
   Copyright 2012, Samuel Neves <sneves@dei.uc.pt>.  You may use this under the
   terms of the CC0, the OpenSSL Licence, or the Apache Public License 2.0, at
   your option.  The terms of these licenses can be found at:
  
   - CC0 1.0 Universal : http://creativecommons.org/publicdomain/zero/1.0
   - OpenSSL license   : https://www.openssl.org/source/license.html
   - Apache 2.0        : http://www.apache.org/licenses/LICENSE-2.0
  
   More information about the BLAKE2 hash function can be found at
   https://blake2.net.
*/

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "blake2.h"

#define STR_(x) #x
#define STR(x) STR_(x)

#define LENGTH 256

#define MAKE_KAT(name,size_prefix) \
do  \
{ \
  printf( "static const uint8_t " #name "_kat[BLAKE2_KAT_LENGTH][" #size_prefix "_OUTBYTES] = \n{\n" ); \
   \
  for( size_t i = 0; i < LENGTH; ++i ) \
  { \
    name( hash, in, NULL, size_prefix ## _OUTBYTES, i, 0 ); \
    printf( "\t{\n\t\t" ); \
 \
  for( int j = 0; j < size_prefix ## _OUTBYTES; ++j ) \
      printf( "0x%02X%s", hash[j], ( j + 1 ) == size_prefix ## _OUTBYTES ? "\n" : j && !( ( j + 1 ) % 8 ) ? ",\n\t\t" : ", " ); \
      \
    printf( "\t},\n" ); \
  } \
   \
  printf( "};\n\n\n\n\n" ); \
  \
} while (0)

#define MAKE_KEYED_KAT(name,size_prefix) \
do  \
{ \
  printf( "static const uint8_t " #name "_keyed_kat[BLAKE2_KAT_LENGTH][" #size_prefix "_OUTBYTES] = \n{\n" ); \
  \
  for( size_t i = 0; i < LENGTH; ++i ) \
  { \
  name( hash, in, key, size_prefix ## _OUTBYTES, i, size_prefix ## _KEYBYTES ); \
  printf( "\t{\n\t\t" ); \
  \
  for( int j = 0; j < size_prefix ## _OUTBYTES; ++j ) \
  printf( "0x%02X%s", hash[j], ( j + 1 ) == size_prefix ## _OUTBYTES ? "\n" : j && !( ( j + 1 ) % 8 ) ? ",\n\t\t" : ", " ); \
  \
  printf( "\t},\n" ); \
  } \
  \
  printf( "};\n\n\n\n\n" ); \
  \
} while (0)


int main( int argc, char **argv )
{
  uint8_t key[64] = {0};
  uint8_t in[LENGTH] = {0};
  uint8_t hash[64] = {0};

  for( size_t i = 0; i < sizeof( in ); ++i )
    in[i] = i;

  for( size_t i = 0; i < sizeof( key ); ++i )
    key[i] = i;

  puts( "#ifndef BLAKE2_KAT_H\n"
        "#define BLAKE2_KAT_H\n\n\n"
        "#include <stdint.h>\n\n"
        "#define BLAKE2_KAT_LENGTH " STR( LENGTH ) "\n\n\n" );
  MAKE_KAT( blake2s, BLAKE2S );
  MAKE_KEYED_KAT( blake2s, BLAKE2S );
  MAKE_KAT( blake2b, BLAKE2B );
  MAKE_KEYED_KAT( blake2b, BLAKE2B );
  MAKE_KAT( blake2sp, BLAKE2S );
  MAKE_KEYED_KAT( blake2sp, BLAKE2S );
  MAKE_KAT( blake2bp, BLAKE2B );
  MAKE_KEYED_KAT( blake2bp, BLAKE2B );
  puts( "#endif" );
  return 0;
}
