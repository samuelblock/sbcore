#ifndef _SAMUEL_BLOCK_CORE_H
#define _SAMUEL_BLOCK_CORE_H

#define _SBCORE_OVERLOAD_3ARGS(a, b, c, name, ...) name

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

#ifndef SBCORE_NO_STRING
	typedef char* str_t;
	
	enum SbStringHeaderField
	{
		_SBCORE_STRING_FIELD_LENGTH,
		_SBCORE_STRING_FIELD_CAPACITY,
		_SBCORE_STRING_FIELD_COUNT,
	};

	str_t str_make(char* c_string, ulen_t n);
	str_t str_make_empty(ulen_t n);
	
	#define str_header(string)   ((ulen_t*)((string) - sizeof(ulen_t)*_SBCORE_STRING_FIELD_COUNT))
	#define str_length(string)   (str_header(string)[_SBCORE_STRING_FIELD_LENGTH])
	#define str_capacity(string) (str_header(string)[_SBCORE_STRING_FIELD_CAPACITY])
	#define str_free(string)     free(str_header(string))

	str_t str_resize(str_t string, ulen_t n);
	#define str_fit(string) str_resize((string), str_length(string))
	str_t str_append_char(str_t string, char ch);
	str_t str_append_string(str_t string, char* appendage, ulen_t n);
	#define str_concat(string, appendage) str_append_string((string), str_length(appendage))
	#ifndef SBCORE_DONT_OVERLOAD
		#define str_append(...) \
			_SBCORE_OVERLOAD_3ARGS(__VA_ARGS__, str_append_string, str_append_char)(__VA_ARGS__)
	#endif
	char str_pop(str_t string);

	#ifdef SBCORE_IMPLEMENT_STRING
		#include <stdlib.h>
	
		str_t str_make(char* c_string, ulen_t n)
		{
			str_t string = malloc(sizeof(ulen_t)*_SBCORE_STRING_FIELD_COUNT + n + 1);
			
			// Fill in header fields		
			((ulen_t*)string)[_SBCORE_STRING_FIELD_LENGTH] = n;
			((ulen_t*)string)[_SBCORE_STRING_FIELD_CAPACITY] = n;

			// Offset and terminate string
			string += sizeof(ulen_t)*_SBCORE_STRING_FIELD_COUNT;
			string[n] = 0;

			// Fill string
			do { n--; string[n] = c_string[n]; } while (n > 0);

			return string;
		}
		
		str_t str_make_empty(ulen_t n)
		{
			str_t string = malloc(sizeof(ulen_t)*_SBCORE_STRING_FIELD_COUNT + n + 1);
			
			// Fill in header fields
			((ulen_t*)string)[_SBCORE_STRING_FIELD_LENGTH] = 0;
			((ulen_t*)string)[_SBCORE_STRING_FIELD_CAPACITY] = n;

			// Offset and terminate string
			string += sizeof(ulen_t)*_SBCORE_STRING_FIELD_COUNT;
			string[0] = 0;

			return string;
		}

		// Assumes str_length(string) < n
		str_t str_resize(str_t string, ulen_t n)
		{
			string = realloc(str_header(string), sizeof(ulen_t)*_SBCORE_STRING_FIELD_COUNT + n + 1);
			
			// Update capacity and reoffset string (not using the macro saves additions)
			((ulen_t*)string)[_SBCORE_STRING_FIELD_CAPACITY] = n;
			string += sizeof(ulen_t)*_SBCORE_STRING_FIELD_COUNT;

			return string;
		}

		// Assumes str_capacity(string) > 0
		str_t str_append_char(str_t string, char ch)
		{
			string[str_length(string)] = ch; // Replace null terminator
			
			// Double size if needed
			if (++str_length(string) > str_capacity(string))
				{ string = str_resize(string, str_capacity(string)<<1); }
			
			string[str_length(string)] = 0; // Terminate string

			return string;
		}
	
		// Assumes str_capacity(string) > 0
		str_t str_append_string(str_t string, char* appendage, ulen_t n)
		{
			ulen_t old_length = str_length(string);
			str_length(string) += n;

			// Double size if needed
			while (str_length(string) > str_capacity(string))
				{ string = str_resize(string, str_capacity(string)<<1); }

			// Fill and terminate string
			string[str_length(string)] = 0;
			do { n--; string[old_length+n] = appendage[n]; } while (n > 0);

			return string;
		}

		char str_pop(str_t string)
		{
			if (str_length(string) > 0)
			{
				char ch = string[--str_length(string)];
				string[str_length(string)] = 0;
				return ch;
			}

			return 0;
		}
	#endif
#endif

#ifndef SBCORE_NO_CONTROL_FLOW
	#define loop for(;;)
	#define until(condition) while(!(condition))
	// The variable definition will probably get optimized out anyways
	// TODO: Check if this is true
	#define defer(...) for (char $=1; $; $=0,__VA_ARGS__)
#endif

#endif
