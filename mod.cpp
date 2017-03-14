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

#include <maiken.hpp>
#define KUL_EXPORT
#undef  _KUL_DEFS_HPP_
#include <kul/defs.hpp>

#ifndef _MKN_MOD_EXEC_NOT_STRING
#define _MKN_MOD_EXEC_NOT_STRING "Error: mkn-mod.exec expects a String as argument"
#endif// _MKN_MOD_EXEC_NOT_STRING

#ifndef _MKN_MOD_EXEC_EMPTY_STRING
#define _MKN_MOD_EXEC_EMPTY_STRING "Warning: empty string provided to mkn-mod.exec"
#endif//_MKN_MOD_EXEC_EMPTY_STRING

namespace mkn { namespace mod { namespace exec {
class MaikenModule : public maiken::Module{
    private:
        bool check(const YAML::Node& node) throw (std::exception){
            if(node.IsNull()) return false;
            if(!node.IsScalar()) 
                KEXCEPT(maiken::ModuleException, _MKN_MOD_EXEC_NOT_STRING);
            return true;
        }
        void exec(const std::string& s) throw (std::exception){
            for(const auto& line : kul::String::LINES(s)){
                std::vector<std::string> args = kul::cli::asArgs(line);
                if(!args.empty()){
                    kul::Process p(args[0]);
                    for(size_t i = 1; i < args.size(); i++) p << args[i];
                    p.start();
                }else
                    KERR << _MKN_MOD_EXEC_EMPTY_STRING;
            }
        }
    public:
        void compile(maiken::Application& a, const YAML::Node& node) throw (std::exception) override{
            if(check(node)) exec(node.Scalar());
        }
        void link(maiken::Application& a, const YAML::Node& node) throw (std::exception) override{
            if(check(node)) exec(node.Scalar());
        }
        void pack(maiken::Application& a, const YAML::Node& node) throw (std::exception) override{
            if(check(node)) exec(node.Scalar());
        }
};
}}}

extern "C" 
KUL_PUBLISH 
maiken::Module* maiken_module_construct() {
    return new mkn :: mod :: exec :: MaikenModule;
}

extern "C" 
KUL_PUBLISH  
void maiken_module_destruct(maiken::Module* p) {
    delete p;
}
