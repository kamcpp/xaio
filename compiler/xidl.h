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

#pragma once

#include <vector>
#include <iostream>

#include <stdio.h>
#include <stdint.h>

#include "module.h"
#include "struct.h"
#include "interface.h"
#include "decl.h"
#include "operation.h"
#include "arg.h"
#include "enum.h"


namespace xaio {
namespace compiler {
  struct xaio_module;
  class xaio_xidl {
    ::std::vector<xaio_module *> modules;
    ::std::string to_str() {
      ::std::stringstream ss;
      ss << "xidl {\n";
      for (auto module : modules) {
        ss << "   module " << module->package << " {\n";
        for (auto enumm : modules->enums) {
          ss << "      enum " <<  enumm->name << " {\n";
          for (auto rev_item : enumm->rev_items) {
            ss << "         ";
            ss << rev_item->first << " -> " << rev_item->second;
            ss << ::std::endl;
          }
          ss << "      }\n";
        }
        for (auto structt : module->structs) {
          ss << "      struct " <<  structt->name << " {\n";
          for (auto decl : struct->decls) {
            ss << "         ";
            ss << decl->to_str();
            ss << ::std::endl;
          }
          ss << "      }\n";
        }
        for (auto interface : module->interfaces) {
          ss << "      ";
          ss << (interface->service_type == xaio_interface::STATELESS ? "stateless" : "stateful");
          ss << " interface " <<  interface->name << ::std::endl;
          ss << "      fqname: '" << interface->get_fq_name() << "'" << ::std::endl;
          ss << "      hash: '" << interface->get_hash() << "'";
          ss << " {\n";
          for (auto operation : interface->operations) {
            ss << "         ";
            ss << operatio->to_str();
            ss << ::std::endl;
          }
          ss << "      }\n";
        }
        ss << "   }\n";
      }
      ss << "}\n";
      return ss.str();
    }
  };
}
}
