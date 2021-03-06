// Copyright (c)2019 Black Sphere Studios
// For conditions of distribution and use, see copyright notice in innative.h

#ifndef __SCHEMA_H__IR__
#define __SCHEMA_H__IR__

#include "innative/innative.h"
#include "innative/khash.h"
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdio.h>
#include <assert.h>

#define kh_exist2(h, x) ((x < kh_end(h)) && kh_exist(h, x))

#ifdef  __cplusplus
#include <string>
extern "C" {
#endif

typedef uint32_t uint32;
typedef bool varuint1;
typedef uint8_t varuint7;
typedef uint32_t varuint32;
typedef uint64_t varuint64;
typedef int8_t varsint7;
typedef int32_t varsint32;
typedef int64_t varsint64;
typedef uint64_t varuptr;
typedef float float32;
typedef double float64;

// Maximum number of immediates used by any instruction
#define MAX_IMMEDIATES 2

enum WASM_TYPE_ENCODING
{
  TE_i32 = -0x01,
  TE_i64 = -0x02,
  TE_f32 = -0x03,
  TE_f64 = -0x04,
  TE_funcref = -0x10,
  TE_func = -0x20,
  TE_void = -0x40,

  TE_iPTR = 0x70, // Internal values, never encoded
  TE_NONE = 0x71,
  TE_POLY = 0x72,
};

enum WASM_LIMIT_FLAGS
{
  WASM_LIMIT_HAS_MAXIMUM = 0x01,
};

enum WASM_SECTION_OPCODE
{
  WASM_SECTION_CUSTOM   = 0x00,
  WASM_SECTION_TYPE     = 0x01,
  WASM_SECTION_IMPORT   = 0x02,
  WASM_SECTION_FUNCTION = 0x03,
  WASM_SECTION_TABLE    = 0x04,
  WASM_SECTION_MEMORY   = 0x05,
  WASM_SECTION_GLOBAL   = 0x06,
  WASM_SECTION_EXPORT   = 0x07,
  WASM_SECTION_START    = 0x08,
  WASM_SECTION_ELEMENT  = 0x09,
  WASM_SECTION_CODE     = 0x0A,
  WASM_SECTION_DATA     = 0x0B
};

enum WASM_KIND
{
  WASM_KIND_FUNCTION = 0,
  WASM_KIND_TABLE    = 1,
  WASM_KIND_MEMORY   = 2,
  WASM_KIND_GLOBAL   = 3
};

enum WASM_INSTRUCTION_OPCODES
{
  OP_unreachable = 0x00,
  OP_nop = 0x01,
  OP_block = 0x02,
  OP_loop = 0x03,
  OP_if = 0x04,
  OP_else = 0x05,
  OP_end = 0x0b,
  OP_br = 0x0c,
  OP_br_if = 0x0d,
  OP_br_table = 0x0e,
  OP_return = 0x0f,

  // Call operators
  OP_call = 0x10,
  OP_call_indirect = 0x11,

  // Parametric operators
  OP_drop = 0x1a,
  OP_select = 0x1b,

  // Variable access
  OP_local_get = 0x20,
  OP_local_set = 0x21,
  OP_local_tee = 0x22,
  OP_global_get = 0x23,
  OP_global_set = 0x24,

  // Memory-related operator
  OP_i32_load = 0x28,
  OP_i64_load = 0x29,
  OP_f32_load = 0x2a,
  OP_f64_load = 0x2b,
  OP_i32_load8_s = 0x2c,
  OP_i32_load8_u = 0x2d,
  OP_i32_load16_s = 0x2e,
  OP_i32_load16_u = 0x2f,
  OP_i64_load8_s = 0x30,
  OP_i64_load8_u = 0x31,
  OP_i64_load16_s = 0x32,
  OP_i64_load16_u = 0x33,
  OP_i64_load32_s = 0x34,
  OP_i64_load32_u = 0x35,
  OP_i32_store = 0x36,
  OP_i64_store = 0x37,
  OP_f32_store = 0x38,
  OP_f64_store = 0x39,
  OP_i32_store8 = 0x3a,
  OP_i32_store16 = 0x3b,
  OP_i64_store8 = 0x3c,
  OP_i64_store16 = 0x3d,
  OP_i64_store32 = 0x3e,
  OP_memory_size = 0x3f,
  OP_memory_grow = 0x40,

  // Constants
  OP_i32_const = 0x41,
  OP_i64_const = 0x42,
  OP_f32_const = 0x43,
  OP_f64_const = 0x44,

  // Comparison operators
  OP_i32_eqz = 0x45,
  OP_i32_eq = 0x46,
  OP_i32_ne = 0x47,
  OP_i32_lt_s = 0x48,
  OP_i32_lt_u = 0x49,
  OP_i32_gt_s = 0x4a,
  OP_i32_gt_u = 0x4b,
  OP_i32_le_s = 0x4c,
  OP_i32_le_u = 0x4d,
  OP_i32_ge_s = 0x4e,
  OP_i32_ge_u = 0x4f,
  OP_i64_eqz = 0x50,
  OP_i64_eq = 0x51,
  OP_i64_ne = 0x52,
  OP_i64_lt_s = 0x53,
  OP_i64_lt_u = 0x54,
  OP_i64_gt_s = 0x55,
  OP_i64_gt_u = 0x56,
  OP_i64_le_s = 0x57,
  OP_i64_le_u = 0x58,
  OP_i64_ge_s = 0x59,
  OP_i64_ge_u = 0x5a,
  OP_f32_eq = 0x5b,
  OP_f32_ne = 0x5c,
  OP_f32_lt = 0x5d,
  OP_f32_gt = 0x5e,
  OP_f32_le = 0x5f,
  OP_f32_ge = 0x60,
  OP_f64_eq = 0x61,
  OP_f64_ne = 0x62,
  OP_f64_lt = 0x63,
  OP_f64_gt = 0x64,
  OP_f64_le = 0x65,
  OP_f64_ge = 0x66,

  // Numeric operators
  OP_i32_clz = 0x67,
  OP_i32_ctz = 0x68,
  OP_i32_popcnt = 0x69,
  OP_i32_add = 0x6a,
  OP_i32_sub = 0x6b,
  OP_i32_mul = 0x6c,
  OP_i32_div_s = 0x6d,
  OP_i32_div_u = 0x6e,
  OP_i32_rem_s = 0x6f,
  OP_i32_rem_u = 0x70,
  OP_i32_and = 0x71,
  OP_i32_or = 0x72,
  OP_i32_xor = 0x73,
  OP_i32_shl = 0x74,
  OP_i32_shr_s = 0x75,
  OP_i32_shr_u = 0x76,
  OP_i32_rotl = 0x77,
  OP_i32_rotr = 0x78,
  OP_i64_clz = 0x79,
  OP_i64_ctz = 0x7a,
  OP_i64_popcnt = 0x7b,
  OP_i64_add = 0x7c,
  OP_i64_sub = 0x7d,
  OP_i64_mul = 0x7e,
  OP_i64_div_s = 0x7f,
  OP_i64_div_u = 0x80,
  OP_i64_rem_s = 0x81,
  OP_i64_rem_u = 0x82,
  OP_i64_and = 0x83,
  OP_i64_or = 0x84,
  OP_i64_xor = 0x85,
  OP_i64_shl = 0x86,
  OP_i64_shr_s = 0x87,
  OP_i64_shr_u = 0x88,
  OP_i64_rotl = 0x89,
  OP_i64_rotr = 0x8a,
  OP_f32_abs = 0x8b,
  OP_f32_neg = 0x8c,
  OP_f32_ceil = 0x8d,
  OP_f32_floor = 0x8e,
  OP_f32_trunc = 0x8f,
  OP_f32_nearest = 0x90,
  OP_f32_sqrt = 0x91,
  OP_f32_add = 0x92,
  OP_f32_sub = 0x93,
  OP_f32_mul = 0x94,
  OP_f32_div = 0x95,
  OP_f32_min = 0x96,
  OP_f32_max = 0x97,
  OP_f32_copysign = 0x98,
  OP_f64_abs = 0x99,
  OP_f64_neg = 0x9a,
  OP_f64_ceil = 0x9b,
  OP_f64_floor = 0x9c,
  OP_f64_trunc = 0x9d,
  OP_f64_nearest = 0x9e,
  OP_f64_sqrt = 0x9f,
  OP_f64_add = 0xa0,
  OP_f64_sub = 0xa1,
  OP_f64_mul = 0xa2,
  OP_f64_div = 0xa3,
  OP_f64_min = 0xa4,
  OP_f64_max = 0xa5,
  OP_f64_copysign = 0xa6,

  // Conversions
  OP_i32_wrap_i64 = 0xa7,
  OP_i32_trunc_f32_s = 0xa8,
  OP_i32_trunc_f32_u = 0xa9,
  OP_i32_trunc_f64_s = 0xaa,
  OP_i32_trunc_f64_u = 0xab,
  OP_i64_extend_i32_s = 0xac,
  OP_i64_extend_i32_u = 0xad,
  OP_i64_trunc_f32_s = 0xae,
  OP_i64_trunc_f32_u = 0xaf,
  OP_i64_trunc_f64_s = 0xb0,
  OP_i64_trunc_f64_u = 0xb1,
  OP_f32_convert_i32_s = 0xb2,
  OP_f32_convert_i32_u = 0xb3,
  OP_f32_convert_i64_s = 0xb4,
  OP_f32_convert_i64_u = 0xb5,
  OP_f32_demote_f64 = 0xb6,
  OP_f64_convert_i32_s = 0xb7,
  OP_f64_convert_i32_u = 0xb8,
  OP_f64_convert_i64_s = 0xb9,
  OP_f64_convert_i64_u = 0xba,
  OP_f64_promote_f32 = 0xbb,

  // Reinterpretations
  OP_i32_reinterpret_f32 = 0xbc,
  OP_i64_reinterpret_f64 = 0xbd,
  OP_f32_reinterpret_i32 = 0xbe,
  OP_f64_reinterpret_i64 = 0xbf,
};

enum IR_ERROR
{
  ERR_SUCCESS = 0,

  // Parse errors that immediately terminate parsing
  ERR_PARSE_UNEXPECTED_EOF = -0x0F,
  ERR_PARSE_INVALID_MAGIC_COOKIE,
  ERR_PARSE_INVALID_VERSION,
  ERR_PARSE_INVALID_FILE_LENGTH,
  ERR_PARSE_INVALID_NAME,

  // Fatal errors that prevent properly parsing the module
  ERR_FATAL_INVALID_WASM_SECTION_ORDER = -0xFF,
  ERR_FATAL_INVALID_MODULE,
  ERR_FATAL_INVALID_ENCODING,
  ERR_FATAL_OVERLONG_ENCODING,
  ERR_FATAL_UNKNOWN_KIND,
  ERR_FATAL_UNKNOWN_INSTRUCTION,
  ERR_FATAL_UNKNOWN_SECTION,
  ERR_FATAL_UNKNOWN_FUNCTION_SIGNATURE,
  ERR_FATAL_UNKNOWN_TARGET,
  ERR_FATAL_EXPECTED_END_INSTRUCTION,
  ERR_FATAL_NULL_POINTER,
  ERR_FATAL_BAD_ELEMENT_TYPE,
  ERR_FATAL_BAD_HASH,
  ERR_FATAL_DUPLICATE_EXPORT,
  ERR_FATAL_DUPLICATE_MODULE_NAME,
  ERR_FATAL_FILE_ERROR,
  ERR_FATAL_LINK_ERROR,
  ERR_FATAL_TOO_MANY_LOCALS,
  ERR_FATAL_OUT_OF_MEMORY,

  // Validation errors that prevent compiling the module
  ERR_VALIDATION_ERROR = -0xFFF,
  ERR_INVALID_FUNCTION_SIG,
  ERR_INVALID_FUNCTION_INDEX,
  ERR_INVALID_TABLE_INDEX,
  ERR_INVALID_MEMORY_INDEX,
  ERR_INVALID_GLOBAL_INDEX,
  ERR_INVALID_IMPORT_MEMORY_MINIMUM,
  ERR_INVALID_IMPORT_MEMORY_MAXIMUM,
  ERR_INVALID_IMPORT_TABLE_MINIMUM,
  ERR_INVALID_IMPORT_TABLE_MAXIMUM,
  ERR_INVALID_IDENTIFIER,
  ERR_INVALID_TABLE_ELEMENT_TYPE,
  ERR_INVALID_LIMITS,
  ERR_INVALID_INITIALIZER,
  ERR_INVALID_GLOBAL_INITIALIZER,
  ERR_INVALID_INITIALIZER_TYPE,
  ERR_INVALID_GLOBAL_TYPE,
  ERR_INVALID_GLOBAL_IMPORT_TYPE,
  ERR_INVALID_TABLE_TYPE,
  ERR_INVALID_MEMORY_TYPE,
  ERR_INVALID_START_FUNCTION,
  ERR_INVALID_TABLE_OFFSET,
  ERR_INVALID_MEMORY_OFFSET,
  ERR_INVALID_FUNCTION_BODY,
  ERR_INVALID_FUNCTION_IMPORT_TYPE,
  ERR_INVALID_VALUE_STACK,
  ERR_EMPTY_VALUE_STACK,
  ERR_INVALID_TYPE,
  ERR_INVALID_TYPE_INDEX,
  ERR_INVALID_BRANCH_DEPTH,
  ERR_INVALID_LOCAL_INDEX,
  ERR_INVALID_ARGUMENT_TYPE,
  ERR_INVALID_BLOCK_SIGNATURE,
  ERR_INVALID_MEMORY_ALIGNMENT,
  ERR_INVALID_RESERVED_VALUE,
  ERR_INVALID_UTF8_ENCODING,
  ERR_INVALID_DATA_SEGMENT,
  ERR_INVALID_MUTABILITY,
  ERR_INVALID_EMBEDDING,
  ERR_IMMUTABLE_GLOBAL,
  ERR_UNKNOWN_SIGNATURE_TYPE,
  ERR_UNKNOWN_MODULE,
  ERR_UNKNOWN_EXPORT,
  ERR_UNKNOWN_BLANK_IMPORT,
  ERR_EMPTY_IMPORT,
  ERR_MULTIPLE_RETURN_VALUES,
  ERR_MULTIPLE_TABLES,
  ERR_MULTIPLE_MEMORIES,
  ERR_IMPORT_EXPORT_MISMATCH,
  ERR_IMPORT_EXPORT_TYPE_MISMATCH,
  ERR_FUNCTION_BODY_MISMATCH,
  ERR_MEMORY_MINIMUM_TOO_LARGE,
  ERR_MEMORY_MAXIMUM_TOO_LARGE,
  ERR_IF_ELSE_MISMATCH,
  ERR_END_MISMATCH,
  ERR_SIGNATURE_MISMATCH,
  ERR_EXPECTED_ELSE_INSTRUCTION,
  ERR_ILLEGAL_C_IMPORT,

  // Compilation errors when parsing WAT
  ERR_WAT_INTERNAL_ERROR = -0xFFFFF,
  ERR_WAT_EXPECTED_OPEN,
  ERR_WAT_EXPECTED_CLOSE,
  ERR_WAT_EXPECTED_TOKEN,
  ERR_WAT_EXPECTED_NAME,
  ERR_WAT_EXPECTED_STRING,
  ERR_WAT_EXPECTED_VALUE,
  ERR_WAT_EXPECTED_NUMBER,
  ERR_WAT_EXPECTED_TYPE,
  ERR_WAT_EXPECTED_VAR,
  ERR_WAT_EXPECTED_VALTYPE,
  ERR_WAT_EXPECTED_FUNC,
  ERR_WAT_EXPECTED_OPERATOR,
  ERR_WAT_EXPECTED_INTEGER,
  ERR_WAT_EXPECTED_FLOAT,
  ERR_WAT_EXPECTED_RESULT,
  ERR_WAT_EXPECTED_THEN,
  ERR_WAT_EXPECTED_ELSE,
  ERR_WAT_EXPECTED_END,
  ERR_WAT_EXPECTED_LOCAL,
  ERR_WAT_EXPECTED_FUNCREF,
  ERR_WAT_EXPECTED_MUT,
  ERR_WAT_EXPECTED_MODULE,
  ERR_WAT_EXPECTED_ELEM,
  ERR_WAT_EXPECTED_KIND,
  ERR_WAT_EXPECTED_EXPORT,
  ERR_WAT_EXPECTED_IMPORT,
  ERR_WAT_EXPECTED_BINARY,
  ERR_WAT_EXPECTED_QUOTE,
  ERR_WAT_INVALID_TOKEN,
  ERR_WAT_INVALID_NUMBER,
  ERR_WAT_INVALID_IMPORT_ORDER,
  ERR_WAT_INVALID_ALIGNMENT,
  ERR_WAT_INVALID_NAME,
  ERR_WAT_INVALID_VAR,
  ERR_WAT_INVALID_TYPE,
  ERR_WAT_INVALID_LOCAL,
  ERR_WAT_UNKNOWN_TYPE,
  ERR_WAT_UNEXPECTED_NAME,
  ERR_WAT_TYPE_MISMATCH,
  ERR_WAT_LABEL_MISMATCH,
  ERR_WAT_OUT_OF_RANGE,
  ERR_WAT_BAD_ESCAPE,
  ERR_WAT_DUPLICATE_NAME,
  ERR_WAT_PARAM_AFTER_RESULT,

  // Runtime errors
  ERR_RUNTIME_INIT_ERROR = -0xFFFFFF,
  ERR_RUNTIME_TRAP,
  ERR_RUNTIME_ASSERT_FAILURE,
};

enum WASM_ENVIRONMENT_FLAGS
{
  ENV_MULTITHREADED = (1 << 0), // Compiles each module in parallel
  ENV_DEBUG = (1 << 1), // Enables debug information
  ENV_LIBRARY = (1 << 2), // Builds a dynamic library instead of an executable.
  ENV_WHITELIST = (1 << 3), // Enables the C import whitelist (if the whitelist is empty, no C imports will be allowed)
  ENV_ENABLE_WAT = (1 << 4), // Enables compiling .wat and .wast files
  ENV_EMIT_LLVM = (1 << 5), // Emits intermediate LLVM IR files for debugging
  ENV_HOMOGENIZE_FUNCTIONS = (1 << 6), // Converts all exported functions to i64 types for testing
  ENV_NO_INIT = (1 << 7), // Disables automatic initialization in DLLs, requiring you to manually call IR_INIT_FUNCTION and IR_EXIT_FUNCTION
  ENV_CHECK_STACK_OVERFLOW = (1 << 10),
  ENV_CHECK_FLOAT_TRUNC = (1 << 11),
  ENV_CHECK_MEMORY_ACCESS = (1 << 12),
  ENV_CHECK_INDIRECT_CALL = (1 << 13),
  ENV_CHECK_INT_DIVISION = (1 << 14),
  ENV_DISABLE_TAIL_CALL = (1 << 15),

  // Strictly adheres to the standard, provided the optimization level does not exceed ENV_OPTIMIZE_STRICT
  ENV_STRICT = ENV_CHECK_STACK_OVERFLOW | ENV_CHECK_FLOAT_TRUNC | ENV_CHECK_MEMORY_ACCESS | ENV_CHECK_INDIRECT_CALL | ENV_DISABLE_TAIL_CALL | ENV_CHECK_INT_DIVISION | ENV_WHITELIST,
  // Only inserts checks required to maintain a sandbox
  ENV_SANDBOX = ENV_CHECK_STACK_OVERFLOW | ENV_CHECK_MEMORY_ACCESS | ENV_CHECK_INDIRECT_CALL | ENV_WHITELIST,
};

enum WASM_OPTIMIZE_FLAGS
{
  ENV_OPTIMIZE_FAST_MATH_NNAN = (1 << 0), // Assume no NaN
  ENV_OPTIMIZE_FAST_MATH_NINF = (1 << 1), // Assume no Inf
  ENV_OPTIMIZE_FAST_MATH_NSZ = (1 << 2),
  ENV_OPTIMIZE_FAST_MATH_ARCP = (1 << 3),
  ENV_OPTIMIZE_FAST_MATH_CONTRACT = (1 << 4),
  ENV_OPTIMIZE_FAST_MATH_AFN = (1 << 5),
  ENV_OPTIMIZE_FAST_MATH_REASSOC = (1 << 6),
  ENV_OPTIMIZE_FAST_MATH = ENV_OPTIMIZE_FAST_MATH_NNAN | ENV_OPTIMIZE_FAST_MATH_NINF | ENV_OPTIMIZE_FAST_MATH_NSZ | ENV_OPTIMIZE_FAST_MATH_ARCP | ENV_OPTIMIZE_FAST_MATH_CONTRACT | ENV_OPTIMIZE_FAST_MATH_AFN | ENV_OPTIMIZE_FAST_MATH_REASSOC,
  ENV_OPTIMIZE_O0 = 0, // no optimization
  ENV_OPTIMIZE_O1 = (1 << 8), // some optimization
  ENV_OPTIMIZE_O2 = (2 << 8), // more optimization
  ENV_OPTIMIZE_O3 = (3 << 8), // all the optimization
  ENV_OPTIMIZE_Os = (4 << 8), // minimize code size
  ENV_OPTIMIZE_OMASK = (7 << 8), 
  ENV_OPTIMIZE_STRICT = ENV_OPTIMIZE_O3, // Only performs optimizations that cannot invalidate the standard
  ENV_OPTIMIZE_ALL = ENV_OPTIMIZE_O3 | ENV_OPTIMIZE_FAST_MATH, // Performs all optimizations, but will never compromise the sandbox.
};

enum WASM_FEATURE_FLAGS
{
  ENV_FEATURE_MUTABLE_GLOBALS = (1 << 0),
  ENV_FEATURE_ALL = ~0,
};

struct __WASM_ENVIRONMENT;

typedef struct __WASM_BYTE_ARRAY
{
#ifdef  __cplusplus
  __WASM_BYTE_ARRAY() : bytes(nullptr), n_bytes(0) {}
  __WASM_BYTE_ARRAY(uint8_t* b, varuint32 n) : bytes(b), n_bytes(n) {}
  inline uint8_t* get() { return bytes; }
  inline const uint8_t* get() const { return bytes; }
  inline const char* str() const { return (char*)bytes; }
  inline varuint32 size() const { return n_bytes; }
  void resize(varuint32 sz, bool terminator, const struct __WASM_ENVIRONMENT& env);
  void discard(varuint32 sz, bool terminator);

  bool operator==(const __WASM_BYTE_ARRAY& r) const;
  inline bool operator!=(const __WASM_BYTE_ARRAY& r) const { return !operator==(r); }
  inline const uint8_t& operator[](varuint32 i) const { assert(i < n_bytes); return bytes[i]; }
  inline uint8_t& operator[](varuint32 i) { assert(i < n_bytes); return bytes[i]; }

protected:
#endif
  varuint32 n_bytes;
  uint8_t* bytes;
} ByteArray;

typedef ByteArray Identifier;

KHASH_DECLARE(exports, Identifier, varuint32);
KHASH_DECLARE(cimport, Identifier, char);
KHASH_DECLARE(modules, Identifier, size_t);

typedef struct __WASM_DEBUGINFO
{
  unsigned int line;
  unsigned int column;
  Identifier name; // Stored debug name, if applicable
} DebugInfo;

typedef union __WASM_IMMEDIATE
{
  uint32 _uint32;
  varuint1 _varuint1;
  varuint7 _varuint7;
  varuint32 _varuint32;
  varuint64 _varuint64;
  varsint7 _varsint7;
  varsint32 _varsint32;
  varsint64 _varsint64;
  float32 _float32;
  float64 _float64;
  varuptr _varuptr;
  struct { varuint32 n_table; varuint32* table; };
} Immediate;

typedef struct __WASM_INSTRUCTION
{
  uint8_t opcode;
  Immediate immediates[MAX_IMMEDIATES];
  unsigned int line; // To keep the size small, we ONLY store line/column on instructions
  unsigned int column;
} Instruction;

typedef struct __WASM_FUNCTION_TYPE
{
  varsint7 form;
  varsint7* params;
  varuint32 n_params;
  varsint7* returns;
  varuint32 n_returns;
} FunctionType;

typedef struct __WASM_RESIZABLE_LIMITS
{
  varuint32 flags;
  varuint32 minimum;
  varuint32 maximum;
} ResizableLimits;

typedef struct __WASM_MEMORY_DESC
{
  ResizableLimits limits;
  DebugInfo debug;
} MemoryDesc;

typedef struct __WASM_TABLE_DESC
{
  varsint7 element_type;
  ResizableLimits resizable;
  DebugInfo debug;
} TableDesc;

typedef struct __WASM_GLOBAL_DESC
{
  varsint7 type;
  varuint1 mutability;
  DebugInfo debug;
} GlobalDesc;

typedef struct __WASM_GLOBAL_DECL
{
  GlobalDesc desc;
  Instruction init;
} GlobalDecl;

typedef struct __WASM_FUNCTION_DESC
{
  varuint32 type_index;
  DebugInfo debug;
  DebugInfo* param_names; // Always the size of n_params from the signature
} FunctionDesc;

typedef struct __WASM_IMPORT
{
  Identifier module_name;
  Identifier export_name;
  varuint7 kind; // WASM_KIND
  union
  {
    FunctionDesc func_desc;
    TableDesc table_desc;
    MemoryDesc mem_desc;
    GlobalDesc global_desc;
  };
} Import;

typedef struct __WASM_EXPORT
{
  Identifier name;
  varuint7 kind; // WASM_KIND
  varuint32 index;
} Export;

typedef struct __WASM_TABLE_INIT
{
  varuint32 index;
  Instruction offset;
  varuint32 n_elements;
  varuint32* elements;
} TableInit;

typedef struct __WASM_FUNCTION_BODY
{
  varuint32 body_size;
  varuint32 n_locals;
  varsint7* locals;
  Instruction* body;
  varuint32 n_body; // INTERNAL: track actual number of instructions
  DebugInfo* local_names; // INTERNAL: debug names of locals, always the size of n_locals or NULL if it doesn't exist
  DebugInfo* param_names; // INTERNAL: debug names of parameters, always the size of n_params or NULL if it doesn't exist
  DebugInfo debug;
} FunctionBody;

typedef struct __WASM_DATA_INIT
{
  varuint32 index;
  Instruction offset;
  ByteArray data;
} DataInit;

typedef struct __WASM_CUSTOM_SECTION
{
  varuint7 opcode;
  varuint32 payload;
  Identifier name; // The first thing in a custom section must always be a legal identifier
  uint8_t* data;
} CustomSection;

typedef struct __WASM_MODULE
{
  uint32 magic_cookie;
  uint32 version;
  uint32_t knownsections; // bit-index corresponds to that OPCODE section being loaded
  Identifier name; // Name of the module as determined by the environment or name section

  struct TypeSection
  {
    varuint32 n_functions;
    FunctionType* functions;
  } type;

  struct ImportSection
  {
    varuint32 functions;
    varuint32 tables;
    varuint32 memories;
    union
    {
      varuint32 globals;
      varuint32 n_import;
    };
    Import* imports;
  } importsection;

  struct FunctionSection
  {
    varuint32 n_funcdecl;
    varuint32* funcdecl;
  } function;

  struct TableSection
  {
    varuint32 n_tables;
    TableDesc* tables;
  } table;

  struct MemorySection
  {
    varuint32 n_memories;
    MemoryDesc* memories;
  } memory;

  struct GlobalSection
  {
    varuint32 n_globals;
    GlobalDecl* globals;
  } global;

  struct ExportSection
  {
    varuint32 n_exports;
    Export* exports;
  } exportsection;

  varuint32 start;

  struct ElementSection
  {
    varuint32 n_elements;
    TableInit* elements;
  } element;

  struct CodeSection
  {
    varuint32 n_funcbody;
    FunctionBody* funcbody;
  } code;

  struct DataSection
  {
    varuint32 n_data;
    DataInit* data;
  } data;

  size_t n_custom;
  CustomSection* custom;

  struct kh_exports_s* exports;
  const char* path; // For debugging purposes, store path to source .wat file, if it exists.
} Module;

typedef struct __WASM_VALIDATION_ERROR
{
  int code;
  char* error;
  ptrdiff_t m;
  struct __WASM_VALIDATION_ERROR* next;
} ValidationError;

typedef struct __WASM_EMBEDDING
{
  const void* data;
  uint64_t size; // If size is 0, data points to a null terminated UTF8 file path
  int tag; // defines the type of embedding data included, determined by the runtime. 0 is always a static library file for the current platform.
  struct __WASM_EMBEDDING* next;
} Embedding;

enum WASM_LOG_LEVEL
{
  LOG_NONE = -1, // Suppress all log output no matter what
  LOG_FATAL = 0,
  LOG_ERROR,
  LOG_WARNING, // Default setting
  LOG_NOTICE, // Verbose setting
  LOG_DEBUG, // Only useful for library developers
};

KHASH_DECLARE(modulepair, kh_cstr_t, FunctionType);

struct __WASM_ALLOCATOR;

typedef struct __WASM_ENVIRONMENT
{
  size_t n_modules; // number of completely loaded modules (for multithreading)
  size_t size; // Size of loaded or loading modules
  size_t capacity; // Capacity of the modules array
  Module* modules;
  Embedding* embeddings;
  ValidationError* errors; //A linked list of non-fatal validation errors that prevent proper execution.
  uint64_t flags;
  uint64_t features;
  uint64_t optimize;
  unsigned int maxthreads; // Max number of threads for any multithreaded action. If 0, there is no limit.
  const char* sdkpath; // Path to look for SDK components, which usually aren't in the working directory
  const char* linker; // If nonzero, attempts to execute this path as a linker instead of using the built-in LLD linker
  struct __WASM_ALLOCATOR* alloc; // Stores a pointer to the allocator
  int loglevel;
  FILE* log;
  void(*wasthook)(void*);

  struct kh_modules_s* modulemap;
  struct kh_modulepair_s* whitelist;
  struct kh_cimport_s* cimports;
} Environment;

#ifdef  __cplusplus
}
#endif

#endif