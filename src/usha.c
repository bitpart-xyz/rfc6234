/**************************** usha.c ***************************/
/***************** See RFC 6234 for details. *******************/
/* Copyright (c) 2011 IETF Trust and the persons identified as */
/* authors of the code.  All rights reserved.                  */
/* See sha.h for terms of use and redistribution.              */

/*
 *  Description:
 *     This file implements a unified interface to the SHA algorithms.
 */

#include "sha.h"

/*
 *  USHAReset
 *
 *  Description:
 *      This function will initialize the SHA Context in preparation
 *      for computing a new SHA message digest.
 *
 *  Parameters:
 *      context: [in/out]
 *          The context to reset.
 *      whichSha: [in]
 *          Selects which SHA reset to call
 *
 *  Returns:
 *      sha Error Code.
 *
 */
int USHAReset(USHAContext *context, enum SHAversion whichSha)
{
  if (!context) return shaNull;
  context->whichSha = whichSha;
  switch (whichSha) {
#if RFC6234_ENABLE_SHA1
    case SHA1:   return SHA1Reset((SHA1Context*)&context->ctx);
#endif
#if RFC6234_ENABLE_SHA224_256
    case SHA224: return SHA224Reset((SHA224Context*)&context->ctx);
    case SHA256: return SHA256Reset((SHA256Context*)&context->ctx);
#endif
#if RFC6234_ENABLE_SHA384_512
    case SHA384: return SHA384Reset((SHA384Context*)&context->ctx);
    case SHA512: return SHA512Reset((SHA512Context*)&context->ctx);
#endif
    default: return shaBadParam;
  }
}
/*
 *  USHAInput
 *
 *  Description:
 *      This function accepts an array of octets as the next portion
 *      of the message.
 *
 *  Parameters:
 *      context: [in/out]
 *          The SHA context to update.
 *      message_array: [in]
 *          An array of octets representing the next portion of
 *          the message.
 *      length: [in]
 *          The length of the message in message_array.
 *
 *  Returns:
 *      sha Error Code.
 *
 */
int USHAInput(USHAContext *context,
              const uint8_t *bytes, unsigned int bytecount)
{
  if (!context) return shaNull;
  switch (context->whichSha) {
#if RFC6234_ENABLE_SHA1
    case SHA1:
      return SHA1Input((SHA1Context*)&context->ctx, bytes,
                       bytecount);
#endif
#if RFC6234_ENABLE_SHA224_256
    case SHA224:
      return SHA224Input((SHA224Context*)&context->ctx, bytes,
          bytecount);
    case SHA256:
      return SHA256Input((SHA256Context*)&context->ctx, bytes,
          bytecount);
#endif
#if RFC6234_ENABLE_SHA384_512
    case SHA384:
      return SHA384Input((SHA384Context*)&context->ctx, bytes,
          bytecount);
    case SHA512:
      return SHA512Input((SHA512Context*)&context->ctx, bytes,
          bytecount);
#endif
    default: return shaBadParam;
  }
}




/*
 * USHAFinalBits
 *
 * Description:
 *   This function will add in any final bits of the message.
 *
 * Parameters:
 *   context: [in/out]
 *     The SHA context to update.
 *   message_bits: [in]
 *     The final bits of the message, in the upper portion of the
 *     byte.  (Use 0b###00000 instead of 0b00000### to input the
 *     three bits ###.)
 *   length: [in]
 *     The number of bits in message_bits, between 1 and 7.
 *
 * Returns:
 *   sha Error Code.
 */
int USHAFinalBits(USHAContext *context,
                  uint8_t bits, unsigned int bit_count)
{
  if (!context) return shaNull;
  switch (context->whichSha) {
#if RFC6234_ENABLE_SHA1
    case SHA1:
      return SHA1FinalBits((SHA1Context*)&context->ctx, bits,
                           bit_count);
#endif
#if RFC6234_ENABLE_SHA224_256
    case SHA224:
      return SHA224FinalBits((SHA224Context*)&context->ctx, bits,
          bit_count);
    case SHA256:
      return SHA256FinalBits((SHA256Context*)&context->ctx, bits,
          bit_count);
#endif
#if RFC6234_ENABLE_SHA384_512
    case SHA384:
      return SHA384FinalBits((SHA384Context*)&context->ctx, bits,
          bit_count);
    case SHA512:
      return SHA512FinalBits((SHA512Context*)&context->ctx, bits,
          bit_count);
#endif
    default: return shaBadParam;
  }
}





/*
 * USHAResult
 *
 * Description:
 *   This function will return the message digest of the appropriate
 *   bit size, as returned by USHAHashSizeBits(whichSHA) for the
 *   'whichSHA' value used in the preceeding call to USHAReset,
 *   into the Message_Digest array provided by the caller.
 *
 * Parameters:
 *   context: [in/out]
 *     The context to use to calculate the SHA-1 hash.
 *   Message_Digest: [out]
 *     Where the digest is returned.
 *
 * Returns:
 *   sha Error Code.
 *
 */
int USHAResult(USHAContext *context,
               uint8_t Message_Digest[USHAMaxHashSize])
{
  if (!context) return shaNull;
  switch (context->whichSha) {
#if RFC6234_ENABLE_SHA1
    case SHA1:
      return SHA1Result((SHA1Context*)&context->ctx, Message_Digest);
#endif
#if RFC6234_ENABLE_SHA224_256
    case SHA224:
      return SHA224Result((SHA224Context*)&context->ctx,
                          Message_Digest);
    case SHA256:
      return SHA256Result((SHA256Context*)&context->ctx,
                          Message_Digest);
#endif
#if RFC6234_ENABLE_SHA384_512
    case SHA384:
      return SHA384Result((SHA384Context*)&context->ctx,
                          Message_Digest);
    case SHA512:
      return SHA512Result((SHA512Context*)&context->ctx,
                          Message_Digest);
#endif
    default: return shaBadParam;
  }
}

/*
 * USHABlockSize
 *
 * Description:
 *   This function will return the blocksize for the given SHA
 *   algorithm.
 *
 * Parameters:
 *   whichSha:
 *     which SHA algorithm to query
 *
 * Returns:
 *   block size
 *
 */
int USHABlockSize(enum SHAversion whichSha)
{
  switch (whichSha) {
#if RFC6234_ENABLE_SHA1
    case SHA1:   return SHA1_Message_Block_Size;
#endif
#if RFC6234_ENABLE_SHA224_256
    case SHA224: return SHA224_Message_Block_Size;
    case SHA256: return SHA256_Message_Block_Size;
#endif
#if RFC6234_ENABLE_SHA384_512
    case SHA384: return SHA384_Message_Block_Size;
    case SHA512: return SHA512_Message_Block_Size;
#endif
    default: return shaBadParam;
  }
}

/*
 * USHAHashSize
 *
 * Description:
 *   This function will return the hashsize for the given SHA
 *   algorithm.
 *
 * Parameters:
 *   whichSha:
 *     which SHA algorithm to query
 *
 * Returns:
 *   hash size
 *
 */
int USHAHashSize(enum SHAversion whichSha)
{
  switch (whichSha) {
#if RFC6234_ENABLE_SHA1
    case SHA1:   return SHA1HashSize;
#endif
#if RFC6234_ENABLE_SHA224_256
    case SHA224: return SHA224HashSize;
    case SHA256: return SHA256HashSize;
#endif
#if RFC6234_ENABLE_SHA384_512
    case SHA384: return SHA384HashSize;
    case SHA512: return SHA512HashSize;
#endif
    default: return shaBadParam;
  }
}
/*
 * USHAHashSizeBits
 *
 * Description:
 *   This function will return the hashsize for the given SHA
 *   algorithm, expressed in bits.
 *
 * Parameters:
 *   whichSha:
 *     which SHA algorithm to query
 *
 * Returns:
 *   hash size in bits
 *
 */
int USHAHashSizeBits(enum SHAversion whichSha)
{
  switch (whichSha) {
#if RFC6234_ENABLE_SHA1
    case SHA1:   return SHA1HashSizeBits;
#endif
#if RFC6234_ENABLE_SHA224_256
    case SHA224: return SHA224HashSizeBits;
    case SHA256: return SHA256HashSizeBits;
#endif
#if RFC6234_ENABLE_SHA384_512
    case SHA384: return SHA384HashSizeBits;
    case SHA512: return SHA512HashSizeBits;
#endif
    default: return shaBadParam;
  }
}

/*
 * USHAHashName
 *
 * Description:
 *   This function will return the name of the given SHA algorithm
 *   as a string.
 *
 * Parameters:
 *   whichSha:
 *     which SHA algorithm to query
 *
 * Returns:
 *   character string with the name in it
 *
 */
const char *USHAHashName(enum SHAversion whichSha)
{
  switch (whichSha) {
#if RFC6234_ENABLE_SHA1
    case SHA1:   return "SHA1";
#endif
#if RFC6234_ENABLE_SHA224_256
    case SHA224: return "SHA224";
    case SHA256: return "SHA256";
#endif
#if RFC6234_ENABLE_SHA384_512
    case SHA384: return "SHA384";
    case SHA512: return "SHA512";
#endif
    default: return 0;
  }
}

