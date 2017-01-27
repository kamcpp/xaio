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

%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <string>
#include <vector>
#include <sstream>
#include <stack>
#include <iostream>

#include <stdint.h>

#include "xidl.h"
#include "decl.h"
#include "interface.h"
#include "operation.h"
#include "arg.h"
#include "enum.h"

#include "fasthash.h"

#include "cc/cc_compiler.h"

void yyerror(char *);

extern "C" {
  int yylex(void);
}

unsigned int line_counter = 1;
std::string last_type;
::std::stack<::std::string> stack;
::xaio::xaio_xidl *xidl;
::xaio::xaio_module *module ;
::xaio::xaio_enum *enumm;
::xaio::xaio_struct *structt;
::xaio::xaio_interface *interface;
::xaio::xaio_operation *operation;

%}
%union {
  char *string;
}
%token MODULE
%token STRUCT
%token ENUM
%token <string> LIST
%token <string> SET
%token <string> MAP
%token STATELESS
%token STATEFUL
%token INTERFACE
%token <string> ORD
%token <string> COMMENT
%token <string> NUMBER
%token <string> IDENTIFIER
%token <string> TYPE
%start xidl
%type<string> type
%%

xidl:           modules {
                }
                ;

modules:        modules module
                |
                ;

module:         {
                  if (not xidl) {
                    xidl = new ::xaio::xaio_xidl;
                  }
                  if (not module) {
                    module = new ::xaio::xaio_module;
                    xidl->modules.push_back(module);
                    while (not stack.empty()) {
                      stack.pop();
                    }
                  } else {
                    fprintf(stdout, "SYNTAX ERROR: Modules can't be nested.\n");
                    exit(1);
                  }
                } MODULE package '{' module_body '}' ';' {
                  ::std::string package = "";
                  while (!stack.empty()) {
                    package += stack.top() + ".";
                    stack.pop();
                  }
                  module->package = package.substr(0, package.size() - 1);
                  module = nullptr;
                }
                ;

package:        IDENTIFIER package { 
                  stack.push($1);
                }
                | IDENTIFIER '.' package { 
                  stack.push($1); 
                }
                |
                ;

module_body:    items
                ;

items:          items item
                |
                ;

item:           {
                  if (not structt) {
                    structt = new ::xaio::xaio_struct(module);
                    module->structs.push_back(structt);
                  } else {
                    fprintf(stdout, "Line %d: Structs can't be nested.\n", line_counter);
                    exit(1);
                  }
                } STRUCT IDENTIFIER '{' struct_body '}' ';' {
                  structt->name = $3;
                  structt = nullptr;
                }
                | {
                    if (not interface) {
                      interface = new ::xaio::xaio_interface;
                      interface->type = ::xaio::xaio_interface::STATELESS;
                      interface->module = module;
                      modules->interfaces.push_back(interface);
                    } else {
                      fprintf(stdout, "SYNTAX ERROR: Interfaces can't be nested.\n");
                      exit(1);
                    }
                } STATELESS SERVICE IDENTIFIER '{' service_body '}' ';' {
                  interface->name = $4;
                  interface = nullptr;
                }
                | {
                    if (not interface) {
                      interface = new ::xaio::xaio_interface;
                      interface->type = ::xaio::xaio_interface::STATEFUL;
                      interface->module = module;
                      module->interfaces.push_back(interface);
                    } else {
                      fprintf(stdout, "SYNTAX ERROR: Interfaces can't be nested.\n");
                      exit(1);
                    } 
                } STATEFUL SERVICE IDENTIFIER '{' service_body '}' ';' {
                  interface->name = $4;
                  interface = nullptr;
                }
                | {
                    if (not enumm) {
                      enumm = new ::xaio::xaio_enum;
                      enumm->module = module;
                      module->enums.push_back(enumm);
                    } else {
                      fprintf(stdout, "SYNTAX ERROR: Enums can't be nested.\n");
                      exit(1);
                    }
                } ENUM IDENTIFIER '{' enum_body '}' ';' {
                  enumm->name = $3;
                  enumm = nullptr;
                }

enum_body:      enum_items;

enum_items:     enum_item
                | enum_items enum_item
                ;

enum_item:      NUMBER '=' IDENTIFIER ';' {
                  enumm->add_item($3, atol($1));
                }
                ;

struct_body:    declarations;

declarations:   declaration
                | declarations declaration
                ;

declaration:    type IDENTIFIER ORD ';' {
                  auto decl = new ::xaio::xaio_decl($1, $2, $3);
                  structt->decls.push_back(decl);
                }
                ;

service_body:   methods
                ;

methods:        methods method
                |
                ;

method:         {
                  if (not operation) {
                    operation = new ::xaio::xaio_operation;
                    operation->interface = interface;
                    interface->operations.push_back(operation);
                  }
                } type IDENTIFIER '(' arguments ')' ';' {
                  operation->return_type = $2;
                  operation->name = $3;
                  operation = nullptr;
                }
                ;

arguments:      argument
                | arguments ',' argument
                |
                ;

argument:       type IDENTIFIER {
                  auto arg = new ::xaio::xaio_arg;
                  arg->type = $1;
                  arg->variable = $2;
                  operation->args.push_back(arg);
                }
                ;

type:           LIST '<' type '>' {
                  $$ = (char*)malloc(strlen($1) + strlen($3) + 10);
                  strcpy($$, $1);
                  strcat($$, "<");
                  strcat($$, $3);
                  strcat($$, ">");
                }               
                | SET '<' type '>' {
                  $$ = (char*)malloc(strlen($1) + strlen($3) + 10);
                  strcpy($$, $1);
                  strcat($$, "<");
                  strcat($$, $3);
                  strcat($$, ">");
                }                
                | MAP '<' type ',' type '>' {
                  $$ = (char*)malloc(strlen($1) + strlen($3) + strlen($5) + 10);
                  strcpy($$, $1);
                  strcat($$, "<");
                  strcat($$, $3);
                  strcat($$, ",");
                  strcat($$, $5);
                  strcat($$, ">");
                }                
                | TYPE  {
                  $$ = (char *)malloc(strlen($1) + 2);
                  strcpy($$, $1);
                }
                | IDENTIFIER {
                  $$ = (char *)malloc(strlen($1) + 2);
                  strcpy($$, $1);
                }
                ;

%%

void yyerror(char *s) {
  fprintf(stderr, "Line %d: ERROR: %s\n", lineCounter, s);
}

int yywrap(void) {
  return 1;
}

extern FILE *yyin;

