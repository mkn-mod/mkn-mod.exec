/**
Copyright (c) 2013, Philip Deegan.
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are
met:

    * Redistributions of source code must retain the above copyright
notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above
copyright notice, this list of conditions and the following disclaimer
in the documentation and/or other materials provided with the
distribution.
    * Neither the name of Philip Deegan nor the names of its
contributors may be used to endorse or promote products derived from
this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/
#include "maiken/module/init.hpp"

#ifndef _MKN_MOD_EXEC_UNSUPPORTED_TYPE_
#define _MKN_MOD_EXEC_UNSUPPORTED_TYPE_ \
  "Error: mkn-mod.exec is an unsupported type"
#endif  // _MKN_MOD_EXEC_UNSUPPORTED_TYPE_

#ifndef _MKN_MOD_EXEC_EMPTY_STRING
#define _MKN_MOD_EXEC_EMPTY_STRING \
  "Warning: empty string provided to mkn-mod.exec"
#endif  //_MKN_MOD_EXEC_EMPTY_STRING

namespace mkn {
namespace mod {
namespace exec {
class MaikenModule : public maiken::Module {
 private:
  void exec(const std::string& s) KTHROW(std::exception) {
    for (const auto& line : kul::String::LINES(s)) {
      std::vector<std::string> args = kul::cli::asArgs(line);
      if (!args.empty()) {
        kul::Process p(args[0]);
        for (size_t i = 1; i < args.size(); i++) p << args[i];
        KLOG(DBG) << "mkn-mod.exec LAUNCHING: " << p;
        p.start();
        KLOG(DBG) << "mkn-mod.exec EXIT CODE: " << p.exitCode();
      } else
        KERR << _MKN_MOD_EXEC_EMPTY_STRING;
    }
  }
  void go(const YAML::Node& node) {
    if (node.IsScalar())
      exec(node.Scalar());
    else if (node.IsSequence()) {
      for (const auto& p : node) {
        if (!p.IsScalar())
          KEXCEPT(maiken::ModuleException, _MKN_MOD_EXEC_UNSUPPORTED_TYPE_);
        exec(p.Scalar());
      }
    } else if (!node.IsNull())
      KEXCEPT(maiken::ModuleException, _MKN_MOD_EXEC_UNSUPPORTED_TYPE_);
  }

 public:
  void init(maiken::Application& a, const YAML::Node& node)
      KTHROW(std::exception) override {
    go(node);
  }
  void compile(maiken::Application& a, const YAML::Node& node)
      KTHROW(std::exception) override {
    go(node);
  }
  void link(maiken::Application& a, const YAML::Node& node)
      KTHROW(std::exception) override {
    go(node);
  }
  void pack(maiken::Application& a, const YAML::Node& node)
      KTHROW(std::exception) override {
    go(node);
  }
};
}  // namespace exec
}  // namespace mod
}  // namespace mkn

extern "C" KUL_PUBLISH maiken::Module* maiken_module_construct() {
  return new mkn ::mod ::exec ::MaikenModule;
}

extern "C" KUL_PUBLISH void maiken_module_destruct(maiken::Module* p) {
  delete p;
}
