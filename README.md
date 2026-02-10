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
Since SHA-224/256 and SHA-384/512 share implementation machinery, enabling one usually enables the pair. You can use single-algorithm macros for convenience:
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
