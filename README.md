# RFC 6234 Secure Hash Algorithms (SHA)

This repository contains a C implementation of the US Secure Hash Algorithms as specified in [RFC 6234](https://tools.ietf.org/html/rfc6234). It includes support for SHA-1, SHA-224, SHA-256, SHA-384, and SHA-512, along with HMAC (Keyed-Hashing for Message Authentication) and HKDF (HMAC-based Extract-and-Expand Key Derivation Function).

## Project Structure

- `include/`: Public headers (e.g., `sha.h`).
- `src/`: Core implementation files.
- `reference/`: Canonical code extracts directly from the RFC text.

## Compile-Time Options

The library supports several compile-time macros to control which algorithms are included, optimizing for binary size and memory usage.

### Algorithm Gating

By default, all algorithms are enabled. If you define **any** of the following macros, the library enters "opt-in" mode: only the algorithms you explicitly enable will be compiled.

| Macro | Description |
|-------|-------------|
| `RFC6234_SHA1` | Enable SHA-1 |
| `RFC6234_SHA224_SHA256` | Enable the SHA-224 and SHA-256 pair |
| `RFC6234_SHA384_SHA512` | Enable the SHA-384 and SHA-512 pair |

#### Pair Toggles
Since SHA-224/256 and SHA-384/512 share implementation machinery, enabling one enables the pair. You can use single-algorithm macros for convenience:
- `RFC6234_SHA224` or `RFC6234_SHA256` → enables `RFC6234_SHA224_SHA256`
- `RFC6234_SHA384` or `RFC6234_SHA512` → enables `RFC6234_SHA384_SHA512`

#### Resource Optimization
When larger hashes (SHA-384/512) are disabled, the library automatically shrinks internal buffers in `HMACContext` and `HKDFContext` to match the largest remaining algorithm, saving stack and heap space.

### Optimization & Portability Macros

| Macro | Description |
|-------|-------------|
| `USE_32BIT_ONLY` | Use 32-bit arithmetic only. Required for systems without 64-bit integer support. |
| `USE_MODIFIED_MACROS` | Use alternate forms of `SHA_Ch()` and `SHA_Maj()` that are equivalent but potentially faster on some architectures. |

## Usage Example

To build the library with only SHA-256 support:
```bash
cc -DRFC6234_SHA256 -Iinclude src/usha.c src/sha224-256.c src/hmac.c src/hkdf.c
```

To build with everything except SHA-1:
```bash
cc -DRFC6234_SHA224_SHA256 -DRFC6234_SHA384_SHA512 -Iinclude ...
```

## Runtime Behavior
If an algorithm is disabled at compile time, the Unified SHA (USHA) and HMAC/HKDF APIs will return `shaBadParam` if that specific algorithm is requested via the `SHAversion` enum.

# RFC 6234 License

   Copyright (c) 2011 IETF Trust and the persons identified as
   authors of the code.  All rights reserved.

   Redistribution and use in source and binary forms, with or
   without modification, are permitted provided that the following
   conditions are met:

   - Redistributions of source code must retain the above
     copyright notice, this list of conditions and
     the following disclaimer.


   - Redistributions in binary form must reproduce the above
     copyright notice, this list of conditions and the following
     disclaimer in the documentation and/or other materials provided
     with the distribution.

   - Neither the name of Internet Society, IETF or IETF Trust, nor
     the names of specific contributors, may be used to endorse or
     promote products derived from this software without specific
     prior written permission.

   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND
   CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
   INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
   MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
   DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR
   CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
   SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
   NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
   LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
   HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
   CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
   OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
   EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.


## RFC 6234 Author's Address
```
   Donald Eastlake
   Huawei
   155 Beaver Street
   Milford, MA 01757 USA

   Telephone:   +1-508-333-2270
   EMail:       d3e3e3@gmail.com
```
```
   Tony Hansen
   AT&T Laboratories
   200 Laurel Ave.
   Middletown, NJ 07748 USA

   Telephone:   +1-732-420-8934
   EMail:       tony+shs@maillennium.att.com
```
