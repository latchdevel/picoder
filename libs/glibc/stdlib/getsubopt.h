/* 
   Assuming *OPTIONP is a comma separated list of elements of the form
   "token" or "token=value", getsubopt parses the first of these elements.

   If the first element refers to a "token" that is member of the given
   NULL-terminated array of tokens:
     - It replaces the comma with a NUL byte, updates *OPTIONP to point past
       the first option and the comma, sets *VALUEP to the value of the
       element (or NULL if it doesn't contain an "=" sign),
     - It returns the index of the "token" in the given array of tokens.

   Otherwise it returns -1, and *OPTIONP and *VALUEP are undefined.

   For more details see the POSIX:2001 specification.
   http://www.opengroup.org/onlinepubs/009695399/functions/getsubopt.html 

*/

#if HAVE_GETSUBOPT

/* Get getsubopt declaration.  */
#include <stdlib.h>

#else

#ifdef __cplusplus
extern "C" {
#endif //__cplusplus

int getsubopt (char **optionp, char *const *tokens, char **valuep);

#ifdef __cplusplus
}
#endif //__cplusplus

#endif