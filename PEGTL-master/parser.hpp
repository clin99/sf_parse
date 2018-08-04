#ifndef SPEF_HPP_
#define SPEF_HPP_

//#include <boost/spirit/include/classic.hpp>
//#include <boost/spirit/include/qi.hpp> 
//#include <boost/spirit/home/x3.hpp>
#include <tao/pegtl.hpp>
#include <iostream>
#include <cstring>
#include <algorithm>
#include <utility>
#include <cassert>
//#include <optional>
#include <vector>
#include <unordered_map>
#include <tuple>
#include <array>
#include <map>

namespace spef{


// ------------------------------------------------------------------------------------------------ 
/*
enum class ConnectionType {
  INTERNAL,
  EXTERNAL
};

std::string to_string(ConnectionType);

enum class ConnectionDirection {
  INPUT,
  OUTPUT,
  INOUT
};

std::string to_string(ConnectionDirection);

// Struct: Connection
struct Connection {

  std::string name;
  ConnectionType type;
  ConnectionDirection direction;

  Connection(const std::string&, ConnectionType, ConnectionDirection);

  Connection() = default;
  Connection(Connection&&) = default;

  Connection& operator = (Connection&&) = default;
};

// ------------------------------------------------------------------------------------------------

// Struct: Net
struct Net {

  std::string name;
  float lcap;
  std::vector<Connection> connections;
  std::vector<std::tuple<std::string, float>> caps;
  std::vector<std::tuple<std::string, std::string, float>> ress;

  void scale_capacitance(float);
  void scale_resistance(float);

  Net() = default;
  Net(Net&&) = default;

  Net& operator = (Net&&) = default;
};

// Operator: <<
std::ostream& operator << (std::ostream&, const Net&);

// ------------------------------------------------------------------------------------------------

// Struct: Spef 
struct Spef {
  
  std::optional<char> divider;
  std::optional<char> delimiter;
  std::optional<TimeUnit> time_unit;
  std::optional<CapacitanceUnit> capacitance_unit;
  std::optional<ResistanceUnit> resistance_unit;

  std::unordered_map<std::string, std::string> name_map;

  std::vector<Net> nets;


  void to_capacitance_unit(const CapacitanceUnit&);
  void to_resistance_unit(const ResistanceUnit&);

  Spef() = default;
  Spef(Spef&&) = default;

  Spef& operator = (Spef&&) = default;
};
*/

//  end of spef data structure. --------------------------------------------------------------------


/*
 *  A typical SPEF file will have 4 main sections:
 *  
 *    a header section,
 *    a name map section,
 *    a top level port section, and
 *    the main parasitic description section.
 */



/*
const inline auto rule_comment = boost::spirit::x3::space | 
  boost::spirit::x3::lexeme[ "//" >> *(boost::spirit::x3::char_ - boost::spirit::x3::eol) >> boost::spirit::x3::eol];

const inline auto rule_quoted_string = boost::spirit::x3::rule<class QuotedString, std::string>() 
                                     = '"' >> boost::spirit::x3::no_skip[*(boost::spirit::x3::char_ - '"')] >> '"';

// Header section
const inline auto rule_standard     {"*SPEF"        >> rule_quoted_string};
const inline auto rule_design_name  {"*DESIGN"      >> rule_quoted_string};
const inline auto rule_data         {"*DATE"        >> rule_quoted_string};
const inline auto rule_vendor       {"*VENDOR"      >> rule_quoted_string};
const inline auto rule_program      {"*PROGRAM"     >> rule_quoted_string};
const inline auto rule_version      {"*VERSION"     >> rule_quoted_string};
const inline auto rule_design_flow  {"*DESIGN_FLOW" >> rule_quoted_string};
const inline auto rule_divider      {"*DIVIDER"     >> boost::spirit::x3::char_};
const inline auto rule_delimiter    {"*DELIMITER"   >> boost::spirit::x3::char_};
const inline auto rule_bus_delimiter{"*BUS_DELIMITER" >> boost::spirit::x3::char_ >> boost::spirit::x3::char_};
const inline auto rule_t_unit       {"*T_UNIT"      >> boost::spirit::x3::double_ >> Units >> 'S'};
const inline auto rule_c_unit       {"*C_UNIT"      >> boost::spirit::x3::double_ >> Units >> 'F'};
const inline auto rule_r_unit       {"*R_UNIT"      >> boost::spirit::x3::double_ >> Units >> "OHM"};
const inline auto rule_l_unit       {"*L_UNIT"      >> boost::spirit::x3::double_ >> Units >> 'H'};


// Name map
//const inline auto rule_net_name {boost::spirit::x3::lexeme[+(boost::spirit::x3::char_ - boost::spirit::x3::eol)]};
const inline auto rule_name {boost::spirit::x3::lexeme[+(boost::spirit::x3::char_("a-zA-Z0-9[]/:_"))]};
const inline auto rule_name_map {"*NAME_MAP" >> *('*' >> boost::spirit::x3::int_ >> rule_name)};

// Port 
const inline auto rule_port {"*PORTS" >> *('*' >> rule_name >> boost::spirit::x3::char_("IOB") >> 
  *(("*C" >> boost::spirit::x3::double_ >> boost::spirit::x3::double_)  | 
    ("*L" >> boost::spirit::x3::double_) |  
    ("*S" >> boost::spirit::x3::double_ >> boost::spirit::x3::double_))
  )
};
*/


class SpefParser{

  public:
  private: 

    void test();


};



namespace double_
{
   using namespace tao::TAO_PEGTL_NAMESPACE;  // NOLINT

   struct plus_minus : opt< one< '+', '-' > > {}; 
   struct dot : one< '.' > {}; 

   struct inf : seq< istring< 'i', 'n', 'f' >,
                     opt< istring< 'i', 'n', 'i', 't', 'y' > > > {}; 

   struct nan : seq< istring< 'n', 'a', 'n' >,
                     opt< one< '(' >,
                          plus< alnum >,
                          one< ')' > > > {}; 

   template< typename D > 
   struct number : if_then_else< dot,
                                 plus< D >,
                                 seq< plus< D >, opt< dot, star< D > > > > {}; 

   struct e : one< 'e', 'E' > {}; 
   struct p : one< 'p', 'P' > {}; 
   struct exponent : seq< plus_minus, plus< digit > > {}; 

   struct decimal : seq< number< digit >, opt< e, exponent > > {}; 
   struct hexadecimal : seq< one< '0' >, one< 'x', 'X' >, number< xdigit >, opt< p, exponent > > {}; 

   struct grammar : seq< plus_minus, sor< hexadecimal, decimal, inf, nan > > {}; 
};





namespace pegtl = tao::TAO_PEGTL_NAMESPACE;

struct Quote: pegtl::string<'"'>
{};

struct QuotedString: pegtl::must<Quote, pegtl::plus<pegtl::not_at<Quote>, pegtl::any>, Quote>
//struct QuotedString: pegtl::must<Quote, pegtl::plus<pegtl::alpha>, Quote>
{};

struct Standard: pegtl::must<TAO_PEGTL_KEYWORD("*SPEF"), pegtl::plus<pegtl::space>, QuotedString, pegtl::eol>
{};


template<typename QuotedString>
struct A1: pegtl::nothing<QuotedString>
{};

template<>
struct A1<QuotedString>
{
  template <typename Input>
  static void apply(const Input& in, std::string& v){
    //std::cout << "Hello\n";
    v = in.string();
    std::cout << "Get = " << v << '\n';
  };
};



struct Compound: pegtl::must<
          pegtl::sor<pegtl::plus<pegtl::alpha>, pegtl::space, pegtl::plus<pegtl::digit>>>
{};


template<typename Compound>
struct A2: pegtl::nothing<Compound>
{};


template<>
struct A2<Compound>
{
  template <typename Input>
  static void apply(const Input& in, std::string& v){
    //std::cout << "Hello\n";
    v = in.string();
    std::cout << "Get = " << v << '\n';
  };
};


struct AB: pegtl::must<TAO_PEGTL_KEYWORD("AB")>
{};

//struct NUM: pegtl::must<TAO_PEGTL_KEYWORD("123")>
struct NUM: pegtl::must<double_::grammar>
//struct NUM: pegtl::must<pegtl::alpha>
{};

struct All: pegtl::must<AB, pegtl::space, NUM>
{};



template<typename T>
struct action: pegtl::nothing<T>
{};


template<>
struct action<AB>
{
  template <typename Input>
  static void apply(const Input& in, std::string& v){
    auto q = in.string();
    std::cout << "This is AB : " << q << "\n";
  };
};


template<>
struct action<NUM>
{
  template <typename Input>
  static void apply(const Input& in, std::string& v){
    auto q = in.string();
    std::cout << "This is NUM : " << q << "\n";
  };
};





};    // end of namespace spef. --------------------------------------------------------------------

#endif
