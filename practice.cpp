
//#include <boost/spirit.hpp>
#include <boost/spirit/home/x3.hpp>

#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>

//#include <boost/spirit/include/classic.hpp>
#include <boost/spirit/include/qi.hpp> 
#include <iostream>
#include <cstring>
#include <algorithm>
#include <utility>
#include <tuple>
#include <unordered_map>
#include <map>
#include <vector>

#include <typeinfo>

struct output : public boost::static_visitor<>
{
  void operator()(int d) const { std::cout << "Int = " << d << '\n'; }
  void operator()(double d) const { std::cout << "Double = " << d << '\n'; }
  void operator()(char c) const { std::cout << "Char = " << c << '\n'; }
  void operator()(std::string s) const { std::cout << s << '\n'; }
};

int main(){
  using boost::spirit::x3::int_;
  using boost::spirit::x3::double_;
  using boost::spirit::x3::char_;
  using boost::spirit::x3::_attr;
  using boost::spirit::x3::phrase_parse;
  using boost::spirit::x3::ascii::space;

  using boost::spirit::x3::ascii::alpha;
  using boost::spirit::x3::ascii::alnum;
  {

    //const auto r {"*A" >> *('*' >> rule_name >> boost::spirit::x3::char_("IOB") >> 
    //  *(("*C" >> boost::spirit::x3::double_ >> boost::spirit::x3::double_)  | 
    //    ("*L" >> boost::spirit::x3::double_) 
    //  ))
    //};

    //const auto r {"*A" >> *('*' >> rule_name >> boost::spirit::x3::char_("IOB") >> 
    //  *('*' >> boost::spirit::x3::char_("CL") >> +(boost::spirit::x3::double_) ))
    //};


    auto f = [](auto& t) {
      //auto& v = boost::spirit::x3::_attr(t);
    };

    //constexpr auto rule_name = boost::spirit::x3::lexeme[+(boost::spirit::x3::char_("a-zA-Z0-9[]/:_"))];

    std::string input("*A 123");

    //const auto rule {"*PORTS" >> *('*' >> int_ >> boost::spirit::x3::char_("IOB") >> 
    //  (*boost::spirit::x3::omit['*' >> boost::spirit::x3::char_("CL") >> double_ ]))
    //};


    const auto rule = "*PORTS" >> *(int_ >> alpha >> *('*' >> double_ >> alpha));


		auto iter = input.begin();
		auto iter_end = input.end();
    phrase_parse(iter, iter_end, rule, space);
    //phrase_parse(iter, iter_end, rule[f], space);

    return 0;
  }

  {
    auto f = [](auto &t){
      auto& v = boost::spirit::x3::_attr(t);
      for(const auto&g : v){
        boost::apply_visitor(
          output{}, g
        );
      }
      //std::cout << typeid(v).name() << '\n';
      //std::cout << v << '\n';
      //if(v){}
      //for(const auto&g : v){
      //  //boost::apply_visitor(
      //  //  //output{}, v
      //  //  [](char a){},
      //  //  [](double a){},
      //  //  t
      //  //);
      //}
      //std::cout << boost::fusion::at_c<0>(v) << '\n';
    };
    std::string input("*A -");
    //auto r = "*A" >> *boost::spirit::x3::lexeme[(double_ | '-')];

    double d4;
    char c3;
    //auto r = "*A" >> *boost::spirit::x3::lexeme[double_[boost::phoenix::ref(d4)=boost::spirit::qi::_1]];

		//auto iter = input.begin();
		//auto iter_end = input.end();
    //phrase_parse(iter, iter_end, r[f], space);
    return 1;
  }

	{
		std::string input( "foo: bar ,"
                    		"gorp: smart ,"
                				"falcou : \"crazy frenchman\" " );
		auto iter = input.begin();
		auto iter_end = input.end();
		auto name = boost::spirit::x3::rule<class name, std::string>()
			= alpha >> *alnum;
		auto quote = boost::spirit::x3::rule<class quote, std::string>()
			= '"' >> boost::spirit::x3::lexeme[ *(~char_('"')) ]	>> '"';
		auto item = boost::spirit::x3::rule<class item, std::pair<std::string, std::string>>() =
			name >> ':' >> ( quote | name );
		std::map<std::string, std::string> key_value_map;
    std::pair<std::string, std::string> p3;
    std::pair<int, double> p4;
    int ii;
    double dd;

		std::map<int, double> kv2;
    auto R5 = boost::spirit::x3::rule<class TAG, std::map<int, double>> () = int_ >> double_;
    //phrase_parse(iter, iter_end, R5, space, kv2);
		//phrase_parse( iter, iter_end,
		//		//item % ',',
    //    item,
		//		space,
    //    //p3);
		//		key_value_map);
    return 0;
	}

  {
    //boost::spirit::x3::rule<class RULE, std::unordered_map<std::string, std::string>> r1;

    auto key = boost::spirit::x3::rule<class name1, std::string>() = alpha >> *alnum;
    auto value = boost::spirit::x3::rule<class name2, std::string>() = alpha >> *alnum;
    std::string str("abc Z123");

    auto last_rule = boost::spirit::x3::rule<class item, std::pair<std::string, std::string>>()
        = key >> ' ' >> value;

    auto b = str.begin();
    auto e = str.end();
    std::map<std::string, std::string> kv;
    std::pair<std::string, std::string> p3;
    //phrase_parse(b, e, last_rule % ',', space, kv);
    return 0;


    //auto f = [&](auto &t){
    //  //auto &a = _attr(t);
    //  //kv.insert(a);
    //};

  }
  {
    struct ones_ : boost::spirit::x3::symbols<double>
		{
			ones_()
			{
				add("A" , 1)("B" , 2)("C" , 3)
					("D" , 4)("E" , 5)("F" , 6)("G" , 7)("H" , 8)("I" , 9);
			}
		} ones;

    auto f = [](auto &ctx){
      auto a = _attr(ctx);
      std::cout << a << '\n';
    };
    std::string str("1 B");
    //boost::spirit::x3::symbols<char, double> table;
    boost::spirit::x3::symbols<double> table;
    table.add("A", 1.0)("B", 2.0)("C", 3.0);
    const auto r2 = double_ >> table[f];
    phrase_parse(str.begin(), str.end(), r2, space);
    return 0;
  }

  {
    //boost::spirit::x3::rule<std::string::iterator, std::pair<double, char>, boost::spirit::x3::ascii::space> r1;

    std::string str("1 R");
    //const auto r2 = boost::spirit::x3::rule<class QuotedString, std::tuple<double, char>>() 
      const auto r2                     = double_ >> char_;
    //const auto r2 = boost::spirit::x3::rule<class QuotedString>() 
    //                      = double_ >> char_;


    std::pair<double, char> pp;
    auto f = [&](auto &ctx){ 
      std::cout << "FUCK\n";
      using boost::fusion::at_c;
      auto& attr = boost::spirit::x3::_attr(ctx);
      std::cout << at_c<0>(attr) << "\n";
      std::cout << at_c<1>(attr) << "\n";
      pp = std::make_pair(at_c<0>(attr), at_c<1>(attr));
    };
    std::cout << std::boolalpha << phrase_parse(str.begin(), str.end(), r2[f], space) << '\n';
    return 1;
  }

  double A, B; 
  auto get_A = [&](auto &ctx){ A = _attr(ctx); };
  auto get_B = [&](auto &ctx){ B = _attr(ctx); };

  std::string str("(10, 123.456)");
  //boost::spirit::x3::phrase_parse(str.begin(), str.end(),
  //    (
  //    '(' >> double_[get_A] >> -(',' >> double_[get_B]) >> ')' | 
  //     double_[get_A]
  //    ),
  //    space
  //  );

  //auto name = +char_("a-z") >> *('.' >> +char_("a-z"));
  //auto quote = '"' >> *( char_('"') ) >> '"';
  //boost::spirit::x3::phrase_parse(str.begin(), str.end(),
  //      (name >> ':' >> (quote | name ) ) % ','
  //    ,
  //    space
  //  );


  auto quoted_string = '"' >> boost::spirit::x3::lexeme[*(char_ - '"')] >> '"' ;
  str = "123.45";
  auto iter = str.begin();

  double dd;
  boost::spirit::x3::parse(str.begin(), str.end(), double_, dd);

  std::cout << std::distance(str.begin(), iter) << '\n';
  if(iter == str.end()){
    std::cout << "This is an end\n";
  }
  //auto _quoted_string = '"' >> boost::spirit::x3::no_skip[*(boost::spirit::x3::ascii::char_ - '"')] >> '"'; 
//  boost::spirit::x3::rule<std::string::iterator, char> _quoted_string = '"' >> boost::spirit::x3::no_skip[*(boost::spirit::x3::ascii::char_ - '"')] >> '"';


  auto name = boost::spirit::x3::rule<class name, std::string>() = boost::spirit::x3::ascii::alpha >> boost::spirit::x3::ascii::alnum;

  auto _quoted_string = boost::spirit::x3::rule<class _quoted_string, std::string>() = '"' >> boost::spirit::x3::no_skip[*(boost::spirit::x3::char_ - '"')] >> '"'; 
  //boost::spirit::x3::rule<std::string::iterator, std::vector<int>()> qq = *boost::spirit::x3::int_ % ',';
  //boost::spirit::x3::rule<std::string, std::tuple<char, char>> qq = '"' >> boost::spirit::x3::ascii::char_;
  //boost::spirit::x3::rule<std::string, std::vector<char>> qs = '"' >> boost::spirit::x3::lexeme[*(char_ - '"')] >> '"';

  std::cout << A << " " << B << '\n';
  //std::string str("CX 10 123.456");
  //boost::spirit::classic::parse(str, *(boost::spirit::classic::str_p("Hello World") [ boost::spirit::classic::increment_a(count) ]
  //      |
  //      boost::spirit::classic::anychar_p)
  //    );
}
