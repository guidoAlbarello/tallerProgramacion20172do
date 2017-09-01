#pragma once
#ifndef PARSERXML_H
#define PARSERXML_H

#include <iostream>
#include "ServerConfig.h"
#include "ClientConfig.h"

class ParserXml {

public:
	ParserXml();
	ServerConfig* readServerConfigFile(std::string path);
	ClientConfig* readClientConfigFile(std::string path);
};
#endif