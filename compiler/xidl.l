/*  The MIT License (MIT)
 *
 *  Copyright (c) 2015 LabCrypto Org.
 *
 *  Permission is hereby granted, free of charge, to any person obtaining a copy
 *  of this software and associated documentation files (the "Software"), to deal
 *  in the Software without restriction, including without limitation the rights
 *  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *  copies of the Software, and to permit persons to whom the Software is
 *  furnished to do so, subject to the following conditions:
 *  
 *  The above copyright notice and this permission notice shall be included in all
 *  copies or substantial portions of the Software.
 *  
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 *  SOFTWARE.
 */

%option noyywrap
%option never-interactive
%{
#include <stdio.h>

#include "xidl.tab.h"

extern uint32_t line_counter;

extern "C" int yylex();
%}
%x SINGLELINE_COMMENT MULTILINE_COMMENT
%%

"//" {
  BEGIN(SINGLELINE_COMMENT);
}
<SINGLELINE_COMMENT>. {
}
<SINGLELINE_COMMENT>"\r\n" {
  line_counter++;
  BEGIN(INITIAL);
}
<SINGLELINE_COMMENT>"\n" {
  line_counter++;
  BEGIN(INITIAL);
}

"/*" {
  BEGIN(MULTILINE_COMMENT);
}
<MULTILINE_COMMENT>. {
}
<MULTILINE_COMMENT>"\r\n" {
  line_counter++;
}
<MULTILINE_COMMENT>"\n" {
  line_counter++;
}
<MULTILINE_COMMENT>"*/" {
  BEGIN(INITIAL); 
}

ord\([1-9][0-9]*\) {
  yylval.string = (char *)malloc(yyleng + 1); 
  sprintf(yylval.string, "%s", yytext);
  return ORD;
}
stateless {
  return STATELESS;
}
stateful {
  return STATEFUL;
}
interface {
  return INTERFACE;
}
list {
  yylval.string = (char *)malloc(yyleng + 1); 
  sprintf(yylval.string, "%s", yytext);
  return LIST;
}
set {
  yylval.string = (char *)malloc(yyleng + 1); 
  sprintf(yylval.string, "%s", yytext);
  return SET;
}
map {
  yylval.string = (char *)malloc(yyleng + 1); 
  sprintf(yylval.string, "%s", yytext);
  return MAP;
}
int {
  yylval.string = (char *)malloc(yyleng + 1); 
  sprintf(yylval.string, "%s", yytext);
  return TYPE;
}
double {
  yylval.string = (char *)malloc(yyleng + 1); 
  sprintf(yylval.string, "%s", yytext);
  return TYPE;
}
string {
  yylval.string = (char *)malloc(yyleng + 1); 
  sprintf(yylval.string, "%s", yytext);
  return TYPE;
}
bool {
  yylval.string = (char *)malloc(yyleng + 1); 
  sprintf(yylval.string, "%s", yytext);
  return TYPE;
}
date {
  yylval.string = (char *)malloc(yyleng + 1); 
  sprintf(yylval.string, "%s", yytext);
  return TYPE;
}
module {
  return MODULE;
}
struct {
  return STRUCT;
}
enum {
  return ENUM;
}
[0-9]([0-9])* {
  yylval.string = (char *)malloc(yyleng + 1); 
  sprintf(yylval.string, "%s", yytext);
  return NUMBER;
}
[a-zA-Z_]([a-zA-Z0-9_])* {
  yylval.string = (char *)malloc(yyleng + 1); 
  sprintf(yylval.string, "%s", yytext);
  return IDENTIFIER;
}
[\{\}\;\.><,()=] { 
  return *yytext; 
}
\n {
  line_counter++;
}
[ \t\r] ;
. { 
}
%%
