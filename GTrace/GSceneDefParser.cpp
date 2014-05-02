#include"stdafx.h"
#include<boost/spirit/include/qi.hpp>
#include<boost/any.hpp>
#include<map>
#include<utility>
#include<string>
#include <boost/fusion/include/std_pair.hpp>
#include"GSceneDefParser.h"
#include<boost/fusion/include/adapt_struct.hpp>

/* Scene definition grammar

  Scene: Object+
  ObjectType: "Light" | "Plane" | "Sphere" | "Camera" | "Material"
  Object: type=ObjecType name=Identifier Map
  Map: "{" KeyValue ("," KeyValue)* "}"
  KeyValue: key=Identifier ":" value=Value
  Value: String | Double | Map | List
  List: "[" Value ("," Value)* "]"
  String: "\"" [^"]+ "\""
  Double: [1-9]+[0-9]*("."[0-9]+)?

*/

using namespace boost::spirit;

enum ObjectType {
	Light, Plane, Sphere, Camera
};

struct ObjectDefinition {
	ObjectType type;
	std::string name;
	std::map<std::string, std::string> attributes;
};
typedef std::map<std::string, std::string> attr_t;

BOOST_FUSION_ADAPT_STRUCT(
	ObjectDefinition,
	(ObjectType, type)
	(std::string, name)
	(attr_t, attributes)
)


struct object_type : qi::symbols<char, ObjectType> {
	object_type() {
		add("Light", Light);
		add("Plane", Plane);
		add("Sphere", Sphere);
		add("Camera", Camera);
	}
}object_type;

template <typename Iterator>
struct scenedef_parser : qi::grammar<Iterator,  ObjectDefinition(), ascii::space_type>
{
	scenedef_parser() : scenedef_parser::base_type(start)
	{
		using qi::int_;
		using qi::lit;
		using qi::double_;
		using qi::lexeme;
		using ascii::char_;
		using qi::alpha;
		using qi::alnum;

		object = object_type >> identifier >> map;
		quoted_string %= lexeme['"' >> +(char_ - '"') >> '"'];
		identifier %= +alpha >> *alnum;
		keyValue = identifier >> ':' >> quoted_string;
		map = '{' >> (keyValue % ',') >> '}';
		start = object;
	}

	qi::rule<Iterator, std::map<std::string, std::string>(), ascii::space_type> map;
	qi::rule<Iterator, std::pair<std::string, std::string>(), ascii::space_type> keyValue;
	qi::rule<Iterator, std::string(), ascii::space_type> quoted_string;
	qi::rule<Iterator, std::string(), ascii::space_type> identifier;
	qi::rule<Iterator, ObjectDefinition(), ascii::space_type> object;
	qi::rule<Iterator, ObjectDefinition(), ascii::space_type> start;
};

int parser_test() 
{
	scenedef_parser<std::string::iterator> parser;
	std::string def("Camera cam01 { pos:\"xyz\", orientation:\"342\"}");
	boost::any value;
	boost::spirit::qi::phrase_parse(def.begin(), def.end(), parser, boost::spirit::ascii::space, value);
	
	return 0;
}


