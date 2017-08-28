#pragma once
#ifndef PARSERXML_H
#define PARSERXML_H

#include <iostream>
#include "ServerConfig.h"

class ParserXml {

public:
	ServerConfig* openServerConfigFile(std::string path);
	void openClientConfigFile(std::string path);
};
#endif