#ifndef _CORE__COMMON__XML_READER_H_
#define _CORE__COMMON__XML_READER_H_

#define MAX_TAG_LEN 30

#include "common.h"

#include <map>
#include <string>
#include <vector>

class XMLReader {
  struct xml_pair {
    std::string tag;
    std::string value;

    xml_pair(std::string tag, std::string value)
      : tag(tag), value(value) {}

    xml_pair()
      : tag(std::string()), value(std::string()) {}
  };
  // указатели на начала строк в xml
  std::vector<char *> content_;
  // копии извлеченныx данных с тэгами с ключем 
  //   - порядковым номером в векторе "content_"
  std::map<int, xml_pair> data_;
  // начало буффера данных
  char *buffer_begin_;
  char *buffer_end_;
  // количество записанных строк 
  int  strs_count_;
  // размер буффера в байтах
  int  buffer_size_;
  // требуется ли перекодирование из utf8 в cp1251
  bool utf8toCP1251_;
  // если файл считан объектом XMLReader
  //   требуется очистка
  bool freeing_;

private:
  XMLReader(char *begin, char *end, bool utf8toCP1251);
  XMLReader(char *filename, bool utf8toCP1251);

  /// fill vector content_ with data
  void init_data();
  /// fill map data_ with data
  void fill_parameters();
  std::string get_tag(char *str);
  xml_pair get_pair(char *data);

#ifdef _OS_WINDOWS
  std::string cp1251_encode(const std::wstring &wstr);
  std::wstring utf8_decode(const std::string &str);
#endif  // _OS_WINDOWS
  std::string setEncodingCP1251(const std::string &utf8_str);

public:
  static XMLReader *Init(char *filename, bool utf8toCP1251);
  static XMLReader *Init(char *begin, char *end, bool utf8toCP1251);

  ~XMLReader();

  char *GetBuffer();
  int   GetBufferSize();
  /// get pointer to line with this tag
  std::string Find_first_value(const std::string  &tag);
  std::string Find_last_value(const std::string  &tag);
  /// get first tag in str
};

#endif // ! _CORE__COMMON__XML_READER_H_
