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

#include <stdio.h>
#include <string.h>
#include <stdint.h>

#include <string>
#include <algorithm>
#include <functional> 
#include <cctype>
#include <locale>
#include <sstream>
#include <vector>
#include <stdexcept>

namespace xaio {
namespace compiler {
  class xaio_string_helper {
  public:
    static bool starts_with(::std::string &str, ::std::string &s) {
      const char *str_char_arr = str.c_str();
      const char *s_char_arr = s.c_str();
      return strncmp((char *)s_char_arr, (char *)str_char_arr, strlen(s_char_arr)) == 0;
    } 
    static ::std::string left_trim(::std::string &s) {
      s.erase(s.begin(), std::find_if(s.begin(), s.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
      return s;
    }
    static ::std::string right_trim(::std::string &s) {
      s.erase(std::find_if(s.rbegin(), s.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
      return s;
    }
    static std::string& trim(std::string &s) {
      return left_trim(right_trim(s));
    }
    static ::std::string replace (
      ::std::string s,
      const ::std::string to_replace,
      const ::std::string replace_with
    ) {
      ::std::string result = s;
      ::std::size_t index = result.find(to_replace);
      while (index != ::std::string::npos) {
        result = result.replace(index, to_replace.length(), replace_with);
        index = result.find(to_replace);
      }
      return result;
    }
    static void replace (
      ::std::string &s,
      const ::std::string to_replace,
      const ::std::string replace_with,
      uint32_t func_status
    ) {
      if (func_status == 1) {
        ::std::size_t index = s.find(to_replace);
        while (index != ::std::string::npos) {
          s.replace(index, to_replace.length(), replace_with);
          index = s.find(to_replace);
        }
      } else {
        throw std::runtime_error("xaio_string_helper::replace(...) : function status is not suppoerted.");
      }
    }
    static ::std::vector<::std::string> split (
      std::string str, 
      char delim
    ) {
      ::std::vector<::std::string> tokens;
      ::std::istringstream f(str);
      ::std::string s;    
      while (getline(f, s, delim)) {
        tokens.push_back(s);
      }
      return tokens;
    }
    static ::std::string concat (
      ::std::vector<::std::string> strs, 
      ::std::string delim
    ) {
      ::std::string result;
      ::std::string del = "";
      for (uint32_t i = 0; i < strs.size(); i++) {
        result += del + strs[i];
        del = delim;
      }
      return result;
    }
    static std::::vector<::std::string> explode_camel_case(::std::string str) {
      ::std::vector<::std::string> tokens;
      uint32_t start_index = 0;
      for (uint32_t i = 0; i < str.size(); i++) {
        if (i > 0 && str[i] >= 'A' && str[i] <= 'Z') {
          tokens.push_back(str.substr(start_index, i - start_index));
          start_index = i;
        }
      }
      tokens.push_back(str.substr(start_index, str.size() - start_index));
      return tokens;
    }
    static ::std::string make_lower_case(::std::string str) {
      ::std::string result(str);
      ::std::transform(str.begin(), str.end(), result.begin(), ::tolower);
      return result;
    }
    static std::string make_upper_case(::std::string str) {
      ::std::string result(str);
      ::std::transform(str.begin(), str.end(), result.begin(), ::toupper);
      return result;
    }
    static ::std::string make_first_capital(::std::string str) {
      if ((int)str[0] < 97 || (int)str[0] > 122) {
        return str;   
      }    
      std::string result = str;
      result[0] = ::std::toupper(result[0]);
      return result;         
    }
    static ::std::string make_first_lower_case(::std::string str) {
      if ((int)str[0] < 65 || (int)str[0] > 90) {
        return str;   
      }    
      std::string result = str;
      result[0] = ::std::tolower(result[0]);
      return result;         
    }
    static ::std::string make_camel_case (
      ::std::vector<::std::string> strs, 
      bool first_capital = true
    ) {
      ::std::stringstream ss;
      for (uint32_t i = 0; i < strs.size(); i++) {
        if (i == 0 and not first_capital) {
          ss << make_lower_case(strs[i]);
        } else {
          ss << make_first_capital(make_lower_case(strs[i]));
        }
      }
      return ss.str();
    }
    static ::std::string make_snake_case(::std::vector<::std::string> strs) {
      ::std::stringstream ss;
      ::std::string del = "";
      for (uint32_t i = 0; i < strs.size(); i++) {
        ss << del << make_lower_case(strs[i]);
        del = "_";
      }
      return ss.str();
    }
    static ::std::string make_screaming_snake_case(::std::vector<::std::string> strs) {
      ::std::stringstream ss;
      ::std::string del = "";
      for (uint32_t i = 0; i < strs.size(); i++) {
        ss << del << make_upper_case(strs[i]);
        del = "_";
      }
      return ss.str();
    }
    static ::std::string make_camel_case_first_capital (
      ::std::string str, 
      char del = ' '
    ) {
      return make_camel_case(split(make_snake_case_from_camel_case(str), '_'));
    }
    static ::std::string make_camel_case_first_small (
      ::std::string str, 
      char del = ' '
    ) {
      return make_camel_case(split(make_snake_case_from_camel_case(str), '_'), false);
    }
    static ::std::string make_snake_case (
      ::std::string str, 
      char del = ' '
    ) {
      return make_snake_case(split(str, del));
    }
    static ::std::string make_screaming_snake_case (
      ::std::string str, 
      char del = ' '
      ) {
      return make_screaming_snake_case(split(str, del));
    }
    static ::std::string make_camel_case_first_capital_from_snake_case(::std::string str) {
      return make_camel_case(split(str, '_'));
    }
    static ::std::string make_camel_case_first_capital_from_screaming_snake_case(::std::string str) {
      return make_camel_case(split(str, '_'));
    }
    static ::std::string make_snake_case_from_camel_case(::std::string str) {
      return make_snake_case(explode_camel_case(str));
    }
    static ::std::string make_snake_case_from_screaming_snake_case(::std::string str) {
      return make_snake_case(split(str, '_'));
    }
    static ::std::string make_screaming_snake_case_from_camel_case(::std::string str) {
      return make_screaming_snake_case(explode_camel_case(str));
    }
    static ::std::string make_screaming_snake_case_from_snake_case(::std::string str) {
      return make_screaming_snake_case(split(str, '_'));
    }
  };
}
}
