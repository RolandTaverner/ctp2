//----------------------------------------------------------------------------
//
// Project      : Call To Power 2
// File type    : C/C++ header
// Description  : Slic cmd definitions, to make slic cmd parser different from
//                the other slic parser that is generated by the same tool.
//                and contains the same global symbols.
//
//----------------------------------------------------------------------------
//
// Disclaimer
//
// THIS FILE IS NOT GENERATED OR SUPPORTED BY ACTIVISION.
//
// This material has been developed at apolyton.net by the Apolyton CtP2
// Source Code Project. Contact the authors at ctp2source@apolyton.net.
//
//----------------------------------------------------------------------------
//
// Compiler flags
//
// - None
//
//----------------------------------------------------------------------------
//
// Modifications from the original Activision code:
//
// - Added more yy* symbols to be replaced by sc* symbols so that there are
//   less problems with the other parser charing the same global symbols that
//   caused usage of invalid memory. - May 28th 2005 Martin Gühmann
//
//----------------------------------------------------------------------------

#ifndef __SCYACCDEFS_H__
#define __SCYACCDEFS_H__

#define yyact scact
#define yychar scchar
#define yychk scchk
#define yydebug scdebug
#define yydef scdef
#define yyerrflag scerrflag
#define yyerror scerror
#define yyexca scexca
#define yylex sclex
#define yylval sclval
#define yynerrs scnerrs
#define yypact scpact
#define yyparse scparse
#define yypgo scpgo
#define yyps scps
#define yypv scpv
#define yyr1 scr1
#define yyr2 scr2
#define yyreds screds
#define yys scs
#define yystate scstate
#define yytmp sctmp
#define yytoks sctoks
#define yyv scv
#define yyval scval
// Added by Martin Gühmann to fix external linkage
// issures in the sliccmd parser
#define yyssp scssp
#define yyvsp scvsp
#define yyss scss
#define yysslim scsslim
#define yyvs scvs
#define yystacksize scstacksize

#define yygrowstack scgrowstack
#define yyrule scrule
#define yyname scname
//
#define yycheck sccheck
#define yytable sctable
#define yygindex scgindex
#define yyrindex scrindex
#define yysindex scsindex
#define yydgoto scdgoto
#define yydefred scdefred
#define yylen sclen
#define yylhs sclhs

#endif
