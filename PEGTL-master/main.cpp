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
  bool show {false};
  //std::ifstream ifs("/home/clin99/Software/spirit/aes_cipher_top.spef");
  std::ifstream ifs("./simple.spef");
  show = true;

  ifs.seekg(0, std::ios::end);
  size_t size = ifs.tellg();
  std::string buffer(size, ' ');
  ifs.seekg(0);
  ifs.read(&buffer[0], size); 
  ifs.close();

	for(size_t i=0; i<buffer.size(); i++){
		if(buffer[i] == '/' && i+1 < buffer.size() && buffer[i+1] == '/') {
			buffer[i] = buffer[i+1] = ' ';
			for(i=i+2; i<buffer.size(); ++i) {
				if(buffer[i] == '\n' || buffer[i] == '\r') {
					break;
				}
				else buffer[i] = ' ';
			}
		}
	}

  buffer.append(1, EOF);
  //std::cout << buffer << '\n';
  //exit(1);


  {
    spef::Data data;
    //tao::pegtl::memory_input<> in("*abacda 1234  \nQQ AAB", "");
    tao::pegtl::memory_input<> in("7 b\na 1", "");
    //tao::pegtl::memory_input<> in("//\n", "");
    try{
      tao::pegtl::parse<spef::rule_qq, spef::action, spef::my_control>(in, data);
    }
    //catch(const std::exception& e){
    catch(const tao::pegtl::parse_error& e){
      std::cout << "Exception here:\n";
      std::cout << e.what() << std::endl;

      const auto p = e.positions.front();
      std::cout << in.line_as_string( p ) << std::endl;
      std::cout << std::string(p.byte_in_line, ' ') << '^' << std::endl;
      std::cout << "At line : " << p.line << "  byte = " << p.byte_in_line << '\n';
      std::cout << "Byte : " << p.byte << '\n';

      //std::cout << e.error_message << '\n';
    }

    return 0;
  }

  { 
    spef::Data data;
    tao::pegtl::memory_input<> in(buffer, "");
    try{
      tao::pegtl::parse<spef::rule_spef, spef::action>(in, data);
      std::cout << "\n\n\n";
      if(show){
        std::cout << data.dump() << '\n';
      }
    }
    catch(const std::exception& e){
      std::cout << "PARSE FAILED WITH EXCEPTION: " << e.what() << std::endl;
    }

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
}
