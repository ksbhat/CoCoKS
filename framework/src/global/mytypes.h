#ifndef _CIVCC_MYTYPES_H_
#define _CIVCC_MYTYPES_H_


/*
 * This enumeration defines all monops
 */
typedef enum { MO_not, MO_neg, MO_unknown } monop;

/*
 * This enumeration defines all binops
 */
typedef enum { BO_add, BO_sub, BO_mul, BO_div, BO_mod, 
               BO_lt, BO_le, BO_gt, BO_ge, BO_eq, BO_ne, 
               BO_and, BO_or, BO_unknown } binop;

typedef enum { Key_extern, Key_export, Key_L_FL_Bracket, Key_R_FL_Bracket,
	       Key_L_Bracket, Key_R_Bracket, Key_if, Key_else, Key_for,
	       Key_while, Key_do, Key_return,
	       Key_void, Key_int, Key_float, Key_bool, Key_equals } keyword;

#endif  /* _CIVCC_MYTYPES_H_ */
