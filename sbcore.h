#ifndef _SAMUEL_BLOCK_CORE_H
#define _SAMUEL_BLOCK_CORE_H

#ifndef SBCORE_NO_INTEGERS
	#include <stdint.h>
	#include <stddef.h>
	
	typedef uint8_t   u8_t;
	typedef uint16_t  u16_t;
	typedef uint32_t  u32_t;
	typedef uint64_t  u64_t;
	typedef uintptr_t uptr_t;
	typedef size_t    ulen_t;
	
	typedef int8_t   i8_t;
	typedef int16_t  i16_t;
	typedef int32_t  i32_t;
	typedef int64_t  i64_t;
	typedef intptr_t iptr_t;
	// TODO: typedef ssize_t    ulen_t;
#endif

#ifndef SBCORE_NO_FLOATS
	#ifndef SBCORE_F32_TYPE
		#define SBCORE_F32_TYPE float
	#endif
	#ifndef SBCORE_F64_TYPE
		#define SBCORE_F64_TYPE double
	#endif

	typedef SBCORE_F32_TYPE f32_t;
	typedef SBCORE_F64_TYPE f64_t;
#endif

#ifndef SBCORE_NO_BOOLEAN
	typedef char bool_t;
	#define TRUE  ((bool_t)1)
	#define FLASE ((bool_t)0)
#endif

#ifndef SBCORE_NO_CONTROL_FLOW
	#define loop for(;;)
	#define until(condition) while(!(condition))
	// The variable definition will probably get optimized out anyways
	// TODO: Check if this is true
	#define defer(...) for (char $=1; $; $=0,__VA_ARGS__)
#endif

#endif
