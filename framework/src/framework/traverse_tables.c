
/**
 * @file traverse_tables.c
 *
 * This file defines the function tables for traversal.
 * 
 * THIS FILE HAS BEEN GENERATED USING 
 * $Id: traverse_tables.c.xsl 14593 2006-01-31 17:09:55Z cg $.
 * DO NOT EDIT THIS FILE AS MIGHT BE CHANGED IN A LATER VERSION.
 *
 * ALL CHANGES MADE TO THIS FILE WILL BE OVERWRITTEN!
 *
 */

#include "traverse_tables.h"
#include "traverse_helper.h"
#include "print.h"
#include "copy_node.h"
#include "free_node.h"
#include "check.h"


travtables_t travtables = {
  /* TR_undefined */
  {&TRAVerror, &TRAVerror, &TRAVerror, &TRAVerror, &TRAVerror, &TRAVerror,
   &TRAVerror, &TRAVerror, &TRAVerror, &TRAVerror, &TRAVerror, &TRAVerror,
   &TRAVerror, &TRAVerror, &TRAVerror, &TRAVerror, &TRAVerror, &TRAVerror,
   &TRAVerror, &TRAVerror, &TRAVerror, &TRAVerror, &TRAVerror, &TRAVerror,
   &TRAVerror, &TRAVerror, &TRAVerror, &TRAVerror, &TRAVerror, &TRAVerror,
   &TRAVerror, &TRAVerror, &TRAVerror, &TRAVerror, &TRAVerror, &TRAVerror,
   &TRAVerror, &TRAVerror, &TRAVerror, &TRAVerror, &TRAVerror, &TRAVerror,
   &TRAVerror, &TRAVerror, &TRAVerror, &TRAVerror, &TRAVerror, &TRAVerror,
   &TRAVerror, &TRAVerror, &TRAVerror, &TRAVerror, &TRAVerror, &TRAVerror,
   &TRAVerror, &TRAVerror}

  /* TR_prt */
  , {&TRAVerror, &PRTconst, &PRTstatement, &PRTprogram, &PRTvardec, &PRTparam,
     &PRTtype, &PRTextern, &PRTexport, &PRTfor, &PRTequals, &PRTifkeyword,
     &PRTl_fl_bracket, &PRTr_fl_bracket, &PRTl_bracket, &PRTr_bracket,
     &PRTkey_void, &PRTkey_int, &PRTkey_float, &PRTkey_bool, &PRTfundec,
     &PRTfunheader, &PRTrettype, &PRTparamlist, &PRTfundef, &PRTfunbody,
     &PRTglobaldef, &PRTglobaldec, &PRTvardeclist, &PRTreturn,
     &PRTreturnkeyword, &PRTstatementlist, &PRTdowhilestat, &PRTdo, &PRTwhile,
     &PRTwhilestat, &PRTforstat, &PRTfordec, &PRTforvarinit, &PRTfuncall,
     &PRTexprlist, &PRTifstat, &PRTelseblock, &PRTelse, &PRTenclosedblock,
     &PRTassign, &PRTbinop, &PRTmonop, &PRTcast, &PRTenclosedexpr, &PRTvarlet,
     &PRTvar, &PRTnum, &PRTfloat, &PRTbool, &PRTerror}

  /* TR_copy */
  , {&TRAVerror, &COPYconst, &COPYstatement, &COPYprogram, &COPYvardec,
     &COPYparam, &COPYtype, &COPYextern, &COPYexport, &COPYfor, &COPYequals,
     &COPYifkeyword, &COPYl_fl_bracket, &COPYr_fl_bracket, &COPYl_bracket,
     &COPYr_bracket, &COPYkey_void, &COPYkey_int, &COPYkey_float,
     &COPYkey_bool, &COPYfundec, &COPYfunheader, &COPYrettype, &COPYparamlist,
     &COPYfundef, &COPYfunbody, &COPYglobaldef, &COPYglobaldec,
     &COPYvardeclist, &COPYreturn, &COPYreturnkeyword, &COPYstatementlist,
     &COPYdowhilestat, &COPYdo, &COPYwhile, &COPYwhilestat, &COPYforstat,
     &COPYfordec, &COPYforvarinit, &COPYfuncall, &COPYexprlist, &COPYifstat,
     &COPYelseblock, &COPYelse, &COPYenclosedblock, &COPYassign, &COPYbinop,
     &COPYmonop, &COPYcast, &COPYenclosedexpr, &COPYvarlet, &COPYvar,
     &COPYnum, &COPYfloat, &COPYbool, &COPYerror}

  /* TR_free */
  , {&TRAVerror, &FREEconst, &FREEstatement, &FREEprogram, &FREEvardec,
     &FREEparam, &FREEtype, &FREEextern, &FREEexport, &FREEfor, &FREEequals,
     &FREEifkeyword, &FREEl_fl_bracket, &FREEr_fl_bracket, &FREEl_bracket,
     &FREEr_bracket, &FREEkey_void, &FREEkey_int, &FREEkey_float,
     &FREEkey_bool, &FREEfundec, &FREEfunheader, &FREErettype, &FREEparamlist,
     &FREEfundef, &FREEfunbody, &FREEglobaldef, &FREEglobaldec,
     &FREEvardeclist, &FREEreturn, &FREEreturnkeyword, &FREEstatementlist,
     &FREEdowhilestat, &FREEdo, &FREEwhile, &FREEwhilestat, &FREEforstat,
     &FREEfordec, &FREEforvarinit, &FREEfuncall, &FREEexprlist, &FREEifstat,
     &FREEelseblock, &FREEelse, &FREEenclosedblock, &FREEassign, &FREEbinop,
     &FREEmonop, &FREEcast, &FREEenclosedexpr, &FREEvarlet, &FREEvar,
     &FREEnum, &FREEfloat, &FREEbool, &FREEerror}

  /* TR_chk */
  , {&TRAVerror, &CHKconst, &CHKstatement, &CHKprogram, &CHKvardec, &CHKparam,
     &CHKtype, &CHKextern, &CHKexport, &CHKfor, &CHKequals, &CHKifkeyword,
     &CHKl_fl_bracket, &CHKr_fl_bracket, &CHKl_bracket, &CHKr_bracket,
     &CHKkey_void, &CHKkey_int, &CHKkey_float, &CHKkey_bool, &CHKfundec,
     &CHKfunheader, &CHKrettype, &CHKparamlist, &CHKfundef, &CHKfunbody,
     &CHKglobaldef, &CHKglobaldec, &CHKvardeclist, &CHKreturn,
     &CHKreturnkeyword, &CHKstatementlist, &CHKdowhilestat, &CHKdo, &CHKwhile,
     &CHKwhilestat, &CHKforstat, &CHKfordec, &CHKforvarinit, &CHKfuncall,
     &CHKexprlist, &CHKifstat, &CHKelseblock, &CHKelse, &CHKenclosedblock,
     &CHKassign, &CHKbinop, &CHKmonop, &CHKcast, &CHKenclosedexpr, &CHKvarlet,
     &CHKvar, &CHKnum, &CHKfloat, &CHKbool, &CHKerror}
};

preposttable_t pretable = {
  NULL, NULL, NULL, NULL, NULL
};

preposttable_t posttable = {
  NULL, NULL, NULL, NULL, NULL
};

const char *travnames[5] = {
  "unknown", "prt", "copy", "free", "chk"
};
