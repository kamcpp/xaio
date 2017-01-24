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

#include <stdexcept>

#include "string_helper.h" 
#include "runtime.h"

namespace xaio {
namespace compiler {
  class xaio_type_helper {
  public:
    static bool is_udt(::std::string type) {
      if (type == "int8") {
        return false;
      } else if (type == "int16") {
        return false;
      } else if (type == "int32") {
        return false;
      } else if (type == "int64") {
        return false;
      } else if (type == "uint8") {
        return false;
      } else if (type == "uint16") {
        return false;
      } else if (type == "uint32") {
        return false;
      } else if (type == "uint64") {
        return false;
      } else if (type == "bool") {
        return false;
      } else if (type == "string") {
        return false;
      } else if (type == "char") {
        return false;
      } else if (type == "data") {
        return false;
      } else if (type == "date") {
        return false;
      } else if (type == "double") {
        return false;
      } else if (type == "void") {
        return false;
      } else if (type == "long") {
        return false;
      }
      return true;
    }
    static ::std::string get_cc_type(::std::string type) {
      if (type == "int8") {
        return "int8_t";
      } else if (type == "int16") {
        return "int16_t";
      } else if (type == "int32") {
        return "int32_t";
      } else if (type == "int64") {
        return "int64_t";
      } else if (type == "uint8") {
        return "uint8_t";
      } else if (type == "uint16") {
        return "uint16_t";
      } else if (type == "uint32") {
        return "uint32_t";
      } else if (type == "uint64") {
        return "uint64_t";
      } else if (type == "bool") {
        return "bool";
      } else if (type == "string") {
        return "std::string";
      } else if (type == "char") {
        return "char";
      } else if (type == "data") {
        return "unsigned char *";
      } else if (type == "date") {
        return "uint64_t";
      } else if (type == "double") {
        return "double";
      } else if (type == "void") {
        return "void";
      } else if (type == "long") {
        return "long";
      }
      return type;
    }
    static bool is_list_type(::std::string type) {
      if (type.find("list") != ::std::string::npos) {
        return true;
      }
      return false;
    }
    static ::std::string fetch_type_of_list(::std::string type) {
      ::std::string output = xaio_string_helper::replace(type , "list<" , "");
      xaio_string_helper::replace(output , ">" , "" , 1);
      return output;
    }  
    static size_t get_type_length(::std::string type){
      if (is_enum(type)) {
        return 2;
      } else if (type.compare("int8") == 0 or type.compare("uint8") == 0) {
        return 1;
      } else if (type.compare("int16") == 0 or type.compare("uint16") == 0) {
        return 2;
      } else if (type.compare("int32") == 0 or type.compare("uint32") == 0) {
        return 4;
      } else if (type.compare("int64") == 0 or type.compare("uint64") == 0) {
        return 8;
      } else if (type.compare("long") == 0 or type.compare("long") == 0) {
        return 8;
      } else if (type.compare("bool") == 0) {
        return 1;
      }
      return -1;
    }
    static bool is_void(::std::string type) {
      return xaio_string_helper::make_lower_case(type) == "void";
    }
    static bool is_boolean(::std::string type) {
      return xaio_string_helper::make_lower_case(type) == "bool";
    }
    static bool is_enum(::std::string type) {
      for (auto e : xaio_runtime::enums.size()) {
        if (e->name == type) {
          return true;
        }
      }
      return false;
    }
  };
}
}
