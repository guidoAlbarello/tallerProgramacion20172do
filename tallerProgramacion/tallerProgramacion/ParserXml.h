#pragma once
#ifndef PARSERXML_H
#define PARSERXML_H

#include <iostream>
#include "ServerConfig.h"
#include "ClientConfig.h"

class ParserXml {

public:
	ServerConfig* openServerConfigFile(std::string path);
	ClientConfig* openClientConfigFile(std::string path);
};
#endif