#include "parser.hpp"

#include <fstream>
#include <typeinfo>


namespace pegtl = tao::pegtl;
namespace hello
{
   // Parsing rule that matches a literal "Hello, ".

   struct prefix
      : pegtl::string< 'H', 'e', 'l', 'l', 'o', ',', ' ' >
   {};

   // Parsing rule that matches a non-empty sequence of
   // alphabetic ascii-characters with greedy-matching.

   struct name
      : pegtl::plus< pegtl::alpha >
   {};

   // Parsing rule that matches a sequence of the 'prefix'
   // rule, the 'name' rule, a literal "!", and 'eof'
   // (end-of-file/input), and that throws an exception
   // on failure.

   struct grammar
      : pegtl::must< prefix, name, pegtl::one< '!' >, pegtl::eof >
   {};

   // Class template for user-defined actions that does
   // nothing by default.

   template< typename Rule >
   struct action
      : pegtl::nothing< Rule >
   {};

   // Specialisation of the user-defined action to do
   // something when the 'name' rule succeeds; is called
   // with the portion of the input that matched the rule.

   template<>
   struct action< name >
   {
      template< typename Input >
      static void apply( const Input& in, std::string& v )
      {
         v = in.string();
      }
   };

}  // namespace hello



int main(int argc, char* argv[]){
  std::ifstream ifs("./simple.spef");
  ifs.seekg(0, std::ios::end);
  size_t size = ifs.tellg();
  std::string buffer(size, ' ');
  ifs.seekg(0);
  ifs.read(&buffer[0], size); 
  ifs.close();

  std::string name;
  std::string design;
  std::string date;
  std::string vendor;
  std::string program;
  std::string version;
  std::string design_flow;
  char divider;
  char delimiter;
  char suffix_delim;
  char postfix_delim;

  //{
  //  spef::Data data;
  //  tao::pegtl::memory_input<> in("abacda 12345\nQQ AAB", "");
  //  tao::pegtl::parse<spef::SpefBusDelimiter, spef::action>(in, data);
  //  return 0;
  //}

  { 
    spef::Data data;
    tao::pegtl::memory_input<> in(buffer, "");
    tao::pegtl::parse<spef::rule_spef, spef::action>(in, data);

    data.show();
  }


  //{
  //  tao::pegtl::memory_input<> in2("AB 123 CD", "");
  //  //tao::pegtl::argv_input<> in(argv, 1);
  //  //tao::pegtl::parse< hello::grammar, hello::action >(in, name );
  //  //tao::pegtl::parse<spef::Compound, spef::A2>(in2, name);
  //  tao::pegtl::parse<spef::All, spef::action>(in2, name);
  //  return 0;
  //}

  ////tao::pegtl::memory_input<> in2(buffer, "");
  //{
  //  tao::pegtl::memory_input<> in2("*SPEF \"Hello 123\"\nQQ AAB", "");
  //  //tao::pegtl::argv_input<> in(argv, 1);
  //  //tao::pegtl::parse< hello::grammar, hello::action >(in, name );
  //  tao::pegtl::parse<spef::Standard, spef::A1>(in2, name);
  //}
  




  return 0;


  //std::cout << "Parse success? " << std::boolalpha << ret << '\n' << '\n';

  std::cout << name << "\n";
  std::cout << design << "\n";
  std::cout << date << "\n";
  std::cout << vendor << "\n";
  std::cout << program << "\n";
  std::cout << version << "\n";
  std::cout << design_flow << "\n";
  std::cout << divider << "\n";
  std::cout << delimiter << "\n";
  std::cout << suffix_delim << postfix_delim << "\n";
  //std::cout << t_unit << "\n";
  //std::cout << c_unit << "\n";
  //std::cout << r_unit << "\n";
  //std::cout << l_unit << "\n";

  //for(const auto& [k, v]: name_map){
  //  std::cout << k << ' ' << v << '\n';
  //}


}
