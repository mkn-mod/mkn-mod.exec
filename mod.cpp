

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
        void compile(maiken::Application& a, const YAML::Node& node) throw (std::exception){
            if(check(node)) exec(node.Scalar());
        }
        void link(maiken::Application& a, const YAML::Node& node) throw (std::exception){
            if(check(node)) exec(node.Scalar());
        }
        void pack(maiken::Application& a, const YAML::Node& node) throw (std::exception){
            if(check(node)) exec(node.Scalar());
        }
};

extern "C" 
KUL_PUBLISH 
maiken::Module* maiken_module_construct() {
    return new MaikenModule;
}

extern "C" 
KUL_PUBLISH  
void maiken_module_destruct(maiken::Module* p) {
    delete p;
}
