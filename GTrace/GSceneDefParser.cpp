#include"stdafx.h"
#include<boost/spirit/include/qi.hpp>

#include"GSceneDefParser.h"

using namespace boost::spirit;

template <typename Iterator>
struct scenedef_parser : qi::grammar<Iterator, int(), ascii::space_type>
{
	scenedef_parser() : scenedef_parser::base_type(start)
	{
		using qi::int_;
		using qi::lit;
		using qi::double_;
		using qi::lexeme;
		using ascii::char_;

		quoted_string %= lexeme['"' >> +(char_ - '"') >> '"'];

		start %= int_ ;
	}

	qi::rule<Iterator, std::string(), ascii::space_type> quoted_string;
	qi::rule<Iterator, int(), ascii::space_type> start;
};

int parser_test() 
{
	scenedef_parser<std::string::iterator> parser;
	std::string def("346");
	int value;
	boost::spirit::qi::phrase_parse(def.begin(), def.end(), parser, boost::spirit::ascii::space, value);
	
	return value;
}


