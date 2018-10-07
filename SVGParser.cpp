#include "stdafx.h"
#include "SVGParser.h"
#include <algorithm>
#include <iostream>
#include <fstream>
#include <sstream>

bool SVGParser::GetVerticesFromSvgFile( const std::string& filePath, std::vector<Point2f> &vertices )
{
	// Open the file
	std::ifstream svgStream( filePath.c_str( ) );

	if ( !svgStream )
	{
		std::cerr << "SVGParser::GetVerticesFromSvgFile(..), failed to load vertices from file " << filePath << std::endl;
		return false;
	}

	// Read the file
	std::string svgLine;
	std::string svgString;
	while ( !svgStream.eof() )
	{
		getline( svgStream, svgLine );
		svgString += svgLine;
	}

	// close the file
	svgStream.close( );

	// Cleanup
	RemoveSpaces( svgString );

	bool success = GetVerticesFromSvgString( svgString, vertices );
	if ( !success )
	{
		std::cerr << "SVGParser::GetVerticesFromSvgFile(..), malformed information in file " << filePath;
		return false;
	}

	std::string viewBoxValue;
	success = GetAttributeValue( svgString, "viewBox", viewBoxValue );
	if ( !success )
	{
		return false;
	}

	Rectf viewBox{};
	std::stringstream sstream{ viewBoxValue };
	sstream >> viewBox.left >> viewBox.bottom >> viewBox.width >> viewBox.height;

	// flip the y coordinate
	for ( Point2f& p : vertices )
	{
		p.y = viewBox.height - p.y;
	}

	return true;
}

void SVGParser::RemoveSpaces( std::string& svgString )
{
	// Remove spaces before and = chars
	size_t foundPos{};
	while ( ( foundPos = svgString.find( " =" ) ) != std::string::npos )
	{
		svgString.replace( foundPos, 2, "=" );
	}
	// Remove spaces after and = chars
	while ( ( foundPos = svgString.find( "= " ) ) != std::string::npos )
	{
		svgString.replace( foundPos, 2, "=" );
	}
	//std::cout << svgString.size( ) << "\n";
	
	// Remove spaces before and > chars
	while ( ( foundPos = svgString.find( " >" ) ) != std::string::npos )
	{
		svgString.replace( foundPos, 2, ">" );
	}
	// Remove spaces after and < chars
	while ( ( foundPos = svgString.find( "< " ) ) != std::string::npos )
	{
		svgString.replace( foundPos, 2, "<" );
	}
	//std::cout << svgString << "\n";

}

bool SVGParser::GetVerticesFromSvgString( std::string& svgString, std::vector<Point2f> &vertices )
{
	// Get path element
	std::string pathElementContent;
	bool success{ GetElementContent( svgString, "path", pathElementContent ) };
	if ( !success )
	{
		std::cerr << "SVGParser::GetVerticesFromSvgString(..), no path element found " ;
		return false;
	}

	// Get d attribute value
	std::string pathDataValue{};
	success = GetAttributeValue( pathElementContent, " d", pathDataValue );
	if ( !success )
	{
		std::cerr << "SVGParser::GetVerticesFromSvgString(..), path element doesn't contain a d-attrribute ";
		return false;
	}
	// Process the path data 
	return GetVerticesFromPathData( pathDataValue, vertices );
}

bool SVGParser::GetVerticesFromPathData( const std::string& pathData, std::vector<Point2f> &vertices )
{
	std::string pathCmdChars( ( "mMZzLlHhVvCcSsQqTtAa" ) );

	// Use streamstream for parsing
	std::stringstream ss( pathData );

	char cmd = 0;
	Point2f cursor{};
	Point2f startPoint;//At the end of the z command, the new current point is set to the initial point of the current subpath.

	bool isOpen = true;

	// http://www.w3.org/TR/SVG/paths.html#Introduction
	Point2f vertex;
	char pathCommand;
	ss >> pathCommand;
	while ( !ss.eof( ) )
	{
		if ( strchr( pathCmdChars.c_str( ), pathCommand ) != 0 )
		{
			cmd = pathCommand;
		}
		else
		{
			ss.putback( pathCommand );
		}

		switch ( cmd )
		{
		case ( 'Z' ):
		case ( 'z' ):
			isOpen = true;
			break;

		case ( 'M' ):
		case ( 'm' ):
			if ( isOpen )
			{
				cursor = FirstSvgPoint( ss, cursor, cmd, isOpen, true );
				startPoint = cursor;
				vertices.push_back( cursor );
				isOpen = false;
				break;
			}
			// Fallthrough when isOpen
		case ( 'L' )://lineto
		case ( 'l' ):
			vertex = NextSvgPoint( ss, cursor, cmd, isOpen, true );
			vertices.push_back( vertex );
			break;

		case ( 'h' ): // horizontal lineto
		case ( 'H' ):
			vertex = NextSvgCoordX( ss, cursor, cmd, isOpen );
			vertices.push_back( vertex );
			break;

		case ( 'v' ): // vertical lineto
		case ( 'V' ):
			vertex = NextSvgCoordY( ss, cursor, cmd, isOpen );
			vertices.push_back( vertex );
			break;

		case ( 'C' ):
		case ( 'c' ):
			std::cerr << "SVGParser::GetVerticesFromPathData,  beziers are not supported.\nHave another look at the guide, or select all nodes in inkscape and press shift + L\n";
			return false;
			break;

		default:
			std::cerr <<  "SVGParser::GetVerticesFromPathData, " << cmd << " is not a supported SVG command";
			return false;
			break;
		}
		// Next command
		ss >> pathCommand;

	}

	return true;
}

bool SVGParser::GetElementContent( const std::string& svgText, const std::string& elementName, std::string& elementContent )
{
	size_t startContentPos;
	size_t endContenPos;
	// 2 possible formats
	// <ElementName> content <ElementName/>
	std::string startElement = "<" + elementName + ">";
	std::string endElement = "<" + elementName + "/>";
	if ( ( startContentPos = svgText.find( startElement )) != std::string::npos )
	{
		startContentPos += startElement.length( );
		if ( ( endContenPos = svgText.find( endElement ) ) != std::string::npos )
		{
			elementContent = svgText.substr( startContentPos, endContenPos - startContentPos );
			return true;
		}
		else
		{
			return false;
		}
	}
	// or
	// <ElementName content />
	startElement = "<" + elementName;
	endElement = "/>"; 
	if ( ( startContentPos = svgText.find( startElement ) ) != std::string::npos )
	{
		startContentPos += startElement.length( );
		if ( ( endContenPos = svgText.find( endElement ) ) != std::string::npos )
		{
			elementContent = svgText.substr( startContentPos, endContenPos - startContentPos );
			return true;
		}
	}
	return false;

}

bool SVGParser::GetAttributeValue( const std::string& svgText, const std::string& attributeName, std::string& attributeValue )
{
	std::string searchAttributeName{ attributeName  + "="};
	
	size_t attributePos =  svgText.find( searchAttributeName );
	if( attributePos == std::string::npos )
	{
		return false;
	}

	size_t openingDoubleQuotePos{ svgText.find( "\"", attributePos ) };
	if ( openingDoubleQuotePos == std::string::npos )
	{
		return false;
	}

	size_t closingDoubleQuotePos{ svgText.find( "\"", openingDoubleQuotePos + 1) };
	if ( closingDoubleQuotePos == std::string::npos )
	{
		return false;
	}

	attributeValue = svgText.substr( openingDoubleQuotePos + 1, closingDoubleQuotePos - openingDoubleQuotePos  - 1);
	//std::cout << attributeName << ":" << attributeValue << "\n";
	return true;
}


// Skips any optional commas in the stream
// SVG has a really funky format,
// not sure this code works for all cases.
// TODO: Test cases!
void SVGParser::SkipSvgComma( std::stringstream& svgStream, bool isRequired )
{
	while ( true )
	{
		char c = svgStream.get( );

		if ( svgStream.eof( ) )
		{
			if ( isRequired )
			{
				std::cerr << "SVGParser::SkipSvgComma, expected comma or whitespace\n";
			}
			break;
		}

		if ( c == ( ',' ) )
			return;

		if ( !isspace( c ) )
		{
			svgStream.unget( );
			return;
		}
	}
}

float SVGParser::ReadSvgValue( std::stringstream& svgStream, float defaultValue )
{
	float s;
	svgStream >> s;

	if ( svgStream.eof( ) )
	{
		s = defaultValue;
	}
	else
	{
		SkipSvgComma( svgStream, false );
	}

	return s;
}

float SVGParser::ReadSvgValue( std::stringstream& svgStream, bool separatorRequired )
{
	float s;
	svgStream >> s;
	SkipSvgComma( svgStream, separatorRequired );
	return s;
}

// Reads a single point
Point2f SVGParser::ReadSvgPoint( std::stringstream& svgStream )
{
	//std::cout << "ReadSvgPoint: "  << svgStream.str() << "\n";
	Point2f p;
	p.x = ReadSvgValue( svgStream, true );
	p.y = ReadSvgValue( svgStream, false );
	return p;
}

Point2f SVGParser::FirstSvgPoint( std::stringstream& svgStream, Point2f& cursor, char cmd, bool isOpen, bool advance )
{
	if ( !isOpen )
	{
		std::cerr << "SVGParser::FirstSvgPoint, expected 'Z' or 'z' command";
	}

	Point2f p = ReadSvgPoint( svgStream );

	if ( islower( cmd ) )
	{
		// Relative point
		p.x = cursor.x + p.x;
		p.y = cursor.y + p.y;
	}

	if ( advance )
	{
		cursor = p;
	}

	return p;
}
// Read the next point, 
// taking into account relative and absolute positioning.
// Advances the cursor if requested.
// Throws an exception if the figure is not open
Point2f SVGParser::NextSvgPoint( std::stringstream& svgStream, Point2f& cursor, char cmd, bool isOpen, bool advance )
{
	if ( isOpen )
	{
		std::cerr << "SVGParser::NextSvgPoint, expected 'M' or 'm' command\n";
	}

	Point2f p = ReadSvgPoint( svgStream );

	if ( islower( cmd ) )
	{
		// Relative point
		p.x = cursor.x + p.x;
		p.y = cursor.y + p.y;
	}

	if ( advance )
	{
		cursor = p;
	}

	return p;
}

// Reads next point, given only the new x coordinate 
Point2f SVGParser::NextSvgCoordX( std::stringstream& svgStream, Point2f& cursor, char cmd, bool isOpen )
{
	if ( isOpen )
	{
		std::cerr << "SVGParser::NextSvgCoordX, expected 'M' or 'm' command\n";
	}

	float c;
	svgStream >> c;

	if ( islower( cmd ) )
	{
		// Relative point
		cursor.x += c;
	}
	else
	{
		cursor.x = c;
	}

	return cursor;
}

// Reads next point, given only the new y coordinate 
Point2f SVGParser::NextSvgCoordY( std::stringstream& svgStream, Point2f& cursor, char cmd, bool isOpen )
{
	if ( isOpen )
	{
		std::cerr << "SVGParser::NextSvgCoordY, expected 'M' or 'm' command\n";
	}

	float c;
	svgStream >> c;

	if ( islower( cmd ) )
	{
		// Relative point
		cursor.y += c;
	}
	else
	{
		cursor.y = c;
	}

	return cursor;
}
