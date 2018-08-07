#include "parser.hpp"

#include <fstream>
#include <typeinfo>
#include <experimental/filesystem>

namespace pegtl = tao::pegtl;

namespace std {
  namespace filesystem = experimental::filesystem;
};

int main(int argc, char* argv[]){
  if(argc != 2){
    std::cout << "Insufficient argument!\n";
    return 1;
  }
  if(not std::filesystem::exists(argv[1])){
    std::cout << "file not exists!\n";
    return 1;
  }

  bool show {false};
  //std::ifstream ifs("/home/clin99/Software/OpenTimer/benchmark/vga_lcd/vga_lcd.spef");
  //std::ifstream ifs("/home/clin99/Software/spirit/aes_cipher_top.spef");
  //std::ifstream ifs("./simple.spef");
  std::ifstream ifs(argv[1]);
  //show = true;

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


  //{
  //  spef::Data data;
  //  //tao::pegtl::memory_input<> in("*abacda 1234  \nQQ AAB", "");
  //  tao::pegtl::memory_input<> in("7 b\na 1", "");
  //  //tao::pegtl::memory_input<> in("//\n", "");
  //  try{
  //    tao::pegtl::parse<spef::rule_qq, spef::action, spef::my_control>(in, data);
  //  }
  //  //catch(const std::exception& e){
  //  catch(const tao::pegtl::parse_error& e){
  //    std::cout << "Exception here:\n";
  //    std::cout << e.what() << std::endl;

  //    const auto p = e.positions.front();
  //    std::cout << in.line_as_string( p ) << std::endl;
  //    std::cout << std::string(p.byte_in_line, ' ') << '^' << std::endl;
  //    std::cout << "At line : " << p.line << "  byte = " << p.byte_in_line << '\n';
  //    std::cout << "Byte : " << p.byte << '\n';
  //    //std::cout << e.error_message << '\n';
  //  }

  //  return 0;
  //}

  { 
    spef::Data data;
    tao::pegtl::memory_input<> in(buffer, "");
    try{
      tao::pegtl::parse<spef::rule_spef, spef::action>(in, data);
      if(show){
        std::cout << "\n\n\n";
        std::cout << data.dump() << '\n';
      }
    }
    catch(const std::exception& e){
      std::cout << "PARSE FAILED WITH EXCEPTION: " << e.what() << std::endl;
      return 1;
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
