//*****************************************************************************
//    Copyright (C) 2014 ZenoTec LLC (http://www.zenotec.net)
//
//    File: zData.h
//    Description:
//
//*****************************************************************************

#ifndef __ZDATA_H__
#define __ZDATA_H__

#include <string>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/xml_parser.hpp>

#include <zutils/zLog.h>

namespace zUtils
{
namespace zData
{

class Data
{

  static const std::string ROOT;
  static const std::string KEY;

public:

  Data(const std::string &key_ = "");

  Data(Data &other_)
  {
    this->_pt = other_._pt;
  }

  Data(const Data &other_)
  {
    this->_pt = other_._pt;
  }

  virtual
  ~Data();

  Data &
  operator=(Data &other_)
  {
    this->_pt = other_._pt;
    return (*this);
  }

  Data &
  operator=(const Data &other_)
  {
    this->_pt = other_._pt;
    return (*this);
  }

  bool
  operator ==(const Data &other_) const
      {
    return (this->_pt == other_._pt);
  }

  bool
  operator !=(const Data &other_) const
      {
    return (this->_pt != other_._pt);
  }

  std::string
  GetKey() const;

  std::string
  GetValue(const std::string &key_) const;
  bool
  GetValue(const std::string &key_, std::string &value_);
  bool
  GetValue(const std::string &key_, int &value_);
  bool
  GetValue(const std::string &key_, unsigned int &value_);

  bool
  SetValue(const std::string &key_, const std::string &value_);
  bool
  SetValue(const std::string &key_, const int &value_);
  bool
  SetValue(const std::string &key_, const unsigned int &value_);

  bool
  AddValue(const std::string &key_, const std::string &value_);
  bool
  AddValue(const std::string &key_, int &value_);

  bool
  GetChild(const std::string &key_, Data &child_) const;
  bool
  PutChild(const std::string &key_, const Data &child_);
  bool
  AddChild(const std::string &key_, const Data &child_);

  std::string
  GetJson() const;
  bool
  SetJson(const std::string &json_);

  std::string
  GetXml() const;
  bool
  SetXml(const std::string &xml_);

protected:

  std::string
  _getKey() const;

  bool
  _setKey(const std::string &key_);

  boost::property_tree::ptree
  _getValue(const std::string &key_) const;

  bool
  _setValue(const std::string &key_, const boost::property_tree::ptree &pt_);

  boost::property_tree::ptree _pt;

private:

  friend class iterator;

};

}
}

#endif /* __ZDATA_H__ */
