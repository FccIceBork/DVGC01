/**********************************************************************/
/* lab 1 DVG C01 - Parser OBJECT                                      */
/**********************************************************************/

/**********************************************************************/
/* Include files                                                      */
/**********************************************************************/
#include <stdio.h>
#include <ctype.h>
#include <string.h>

/**********************************************************************/
/* Other OBJECT's METHODS (IMPORTED)                                  */
/**********************************************************************/
#include "keytoktab.h"               /* when the keytoktab is added   */
/* #include "lexer.h"       */       /* when the lexer     is added   */
/* #include "symtab.h"      */       /* when the symtab    is added   */
/* #include "optab.h"       */       /* when the optab     is added   */

/**********************************************************************/
/* OBJECT ATTRIBUTES FOR THIS OBJECT (C MODULE)                       */
/**********************************************************************/
#define DEBUG 1
static int  lookahead=0;
static int  is_parse_ok=1;

/**********************************************************************/
/* Simulate the token stream for a given program                      */
/**********************************************************************/
static int tokens[] = {program, id, '(', input, ',', output, ')', ';',
                     var, id, ',', id, ',', id, ':', integer, ';',
                     begin,
                     id, assign, id, '+', id, '*', number,
                     end, '.',
                     '$' };;

/**********************************************************************/
/*  Simulate the lexer -- get the next token from the buffer          */
/**********************************************************************/
static int pget_token()
{  
   static int i=0;
   if (tokens[i] != '$') return tokens[i++]; else return '$';
   }

/**********************************************************************/
/*  PRIVATE METHODS for this OBJECT  (using "static" in C)            */
/**********************************************************************/
static void in_print(char * status)
{
   printf("\n *** In %s", status);
   }
static void out_print(char * status)
{
   printf("\n *** Out %s", status);
   }
/**********************************************************************/
/* The Parser functions                                               */
/**********************************************************************/
static void match(int t)
{
   if(DEBUG) printf("\n *** In match\t\texpected: %-7s, found: %-7s",
                    tok2lex(t), tok2lex(lookahead));
   if (lookahead == t) lookahead = pget_token();
   else {
      is_parse_ok=0;
      printf("\n *** Unexpected Token:\t\texpected: %4d found: %4d (in match)",
              t, lookahead);
      }
   }

/**********************************************************************/
/* The grammar functions                                              */
/**********************************************************************/
/* Program header                                                     */
/**********************************************************************/
static void program_header()
{
   if (DEBUG) 
   {
      printf("\n");
      in_print("program_header");
      match(program); match(id); match('('); match(input);
      match(','); match(output); match(')'); match(';');
      out_print("program_header");
      }
   }

/**********************************************************************/
/* Var part                                                           */
/**********************************************************************/
static void type()
{
   in_print("type");
   switch (lookahead)
   {
   case integer:
      match(integer);
      break;
   case boolean:
      match(boolean);
      break;
   case real:
      match(real);
      break;
   default:
      break;
   }
   out_print("type");
   }
static void id_list()
{
   in_print("id_list");
   match(id);
   if(lookahead == ',') { match(','); id_list(); }
   out_print("id_list");
   }
static void var_dec()
{
   in_print("var_dec");
   id_list();
   match(':');
   type();
   match(';');
   out_print("var_dec");
   }
static void var_dec_list()
{
   in_print("var_dec_list");
   var_dec();
   if (lookahead == id) var_dec_list();
   out_print("var_dec_list");
   }
static void var_part()
{
   if (DEBUG) 
   {
      printf("\n");
      in_print("var_part");
      match(var); 
      var_dec_list();
      out_print("var_part");
      }
   }

/**********************************************************************/
/* Stat part                                                          */
/**********************************************************************/
static void operand()
{
   in_print("operand");
   if (lookahead == id) match(id);
   if (lookahead == number) match(number);
   out_print("operand");
}
static void factor()
{
   in_print("factor");
   operand();
   out_print("factor");
   }
static void term()
{
   in_print("term");
   factor();
   if (lookahead == '*') { match('*'); term(); }
   out_print("term");
   }
static void expr()
{
   in_print("expr");
   term();
   if (lookahead == '+') { match('+'); expr(); }
   out_print("expr");
   }
static void assign_stat()
{
   in_print( "assign_stat" );
   match(id);
   match(assign);
   expr();
   out_print("assign_stat");
   }
static void stat()
{
   in_print("stat");
   assign_stat();
   out_print("stat");
   }
static void stat_list()
{
   in_print("stat_list");
   stat();
   if (lookahead == ';') { match(';'); stat_list(); }
   out_print("stat_list");
   }
static void stat_part()
{
   if (DEBUG)
   {  
      printf("\n");
      in_print("stat_part");
      match(begin);
      stat_list();
      match(end); 
      match('.');
      out_print("stat_part");
      }
   } 

/**********************************************************************/
/*  PUBLIC METHODS for this OBJECT  (EXPORTED)                        */
/**********************************************************************/

int parser()
{
   if (DEBUG) printf("\n *** In  program");
   lookahead = pget_token();       // get the first token
   program_header();               // call the first grammar rule
   var_part();                     // call the second grammar rule
   stat_part();                    // call the tird grammar rule
   return is_parse_ok;             // status indicator
   }

/**********************************************************************/
/* End of code                                                        */
/**********************************************************************/