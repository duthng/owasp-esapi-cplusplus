/*
* Configuration.cpp
*
*  Created on: Mar 21, 2012
*/

#include "EsapiCommon.h"
#include "errors/IllegalArgumentException.h"
#include "errors/NoSuchPropertyException.h"
#include "errors/ParseException.h"
#include "reference/Configuration.h"
#include "util/TextConvert.h"

#include <sstream>

namespace esapi {

  Configuration::Configuration() {
  }

  //Configuration::Configuration(const hash_map<String, String> map): map_(map) {
  //}

  Configuration::Configuration(const unordered_map<String, String> map): map_(map) {
  }

  Configuration::~Configuration() {
  }

  bool Configuration::hasProperty(const String &key) const {
    return map_.count(key) > 0;
  }

  bool Configuration::getUnparsedString(const String &key, String &value) const {
    // hash_map<String, String>::const_iterator iterator = map_.find(key);
    unordered_map<String, String>::const_iterator iterator = map_.find(key);
    if (iterator != map_.end()) {
      value = iterator->second;
      return true;
    }
    else
      return false;
  }

  String Configuration::getString(const String &key) const {
    String value;
    if (!getUnparsedString(key, value))
      throw NoSuchPropertyException("Property not found: " + key);

    return value;
  }

  String Configuration::getString(const String &key, const String &defaultValue) const {
    String value;
    if (getUnparsedString(key, value))
      ;
    else
      value = defaultValue;

    return value;
  }

  int Configuration::getInt(const String &key) const {
    String value;
    int intValue;
    if (getUnparsedString(key, value))
      intValue = parseInt(value);
    else
      throw NoSuchPropertyException("Property not found: " + key);

    return intValue;
  }

  int Configuration::getInt(const String &key, int defaultValue) const {
    String value;
    if (getUnparsedString(key, value))
      return parseInt(value);
    return defaultValue;
  }

  bool Configuration::getBool(const String &key) const {
    String value;
    bool boolValue;
    if (getUnparsedString(key, value))
      boolValue = parseBool(value);
    else
      throw NoSuchPropertyException("Property not found: " + key);

    return boolValue;
  }

  bool Configuration::getBool(const String &key, const bool defaultValue) const {
    String value;
    bool boolValue;
    if (getUnparsedString(key, value))
      boolValue = parseBool(value);
    else
      value = defaultValue;

    return boolValue;
  }


  bool Configuration::parseBool(const String &s) const {
    String lower(s);
    std::transform(lower.begin(), lower.end(), lower.begin(), ::tolower);

    if (s == "0" || s == "false" || s == "off" || s == "no")
      return false;
    else if (s == "1" || s == "true" || s == "on" || s == "yes")
      return true;
    else
      throw ParseException("Cannot parse as boolean: " + s);
  }

  int Configuration::parseInt(const String &s) const {

    int n = -1;
    try
    {
      n = stoi(s);
    }
    catch (...)
    {
      throw ParseException("Cannot parse as int: " + s);
    }

    // FIXME: Maybe better than the above because it does not require C++0X:
    //	int n = -1;
    //	try
    //	{
    //		int n = boost::lexical_cast<int>(s);
    //	}
    //	catch (boost::bad_lexical_cast &e)
    //	{
    //		throw ParseException("Cannot parse as int: " + s);
    //	}

    // FIXME: This does not seem to work:
    //	std::wistringstream ss(s);
    //	int n;
    //	wchar_t c;
    //	ss >> n;
    //	if (ss.fail() || ss.get(c)) {
    //		throw ParseException("Cannot parse as int: " + s);
    //	}

    return n;
  }

  StringList Configuration::getStringList(const String &key) const
  {
    StringList value;

    String unparsed;
    if (getUnparsedString(key, unparsed))
    {
      splitString(unparsed, value, ", ", true);
    }
    else
      throw NoSuchPropertyException("Property not found: " + key);

    return value;
  }

  StringList Configuration::getStringList(const String &key, const StringList &defaultValue) const
  {
    StringList value;

    String unparsed;
    if (getUnparsedString(key, unparsed))
    {
      splitString(unparsed, value, ",", true);
    }
    else
      value = defaultValue;

    return value;
  }

  void Configuration::splitString(String &input, StringList &output, const String &delimiters = " ", const bool trimEmpty = false) const
  {
    if (delimiters.empty())
      throw IllegalArgumentException("Cannot split using empty set of delimiters");

    String::size_type offset = 0;
    while (true)
    {
      String::size_type pos = input.find_first_of(delimiters, offset);
      if (pos == String::npos)
      {
        pos = input.length();

        if (pos != offset || !trimEmpty)
          output.push_back(input.substr(offset));
        break;
      }
      else
      {
        if (pos != offset || !trimEmpty)
          output.push_back(input.substr(offset, pos - offset));
        offset = pos + 1;
      }
    }
  }
}
