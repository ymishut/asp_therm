#ifndef _CORE__COMMON__XML_READER_H_
#define _CORE__COMMON__XML_READER_H_

#define MAX_TAG_LEN 30

#include "common.h"

#include <map>
#include <memory>
#include <string>
#include <vector>


class XMLReader {
  // обычная пара тэг-значение
  struct xml_pair {
    std::string tag;
    std::string value;

    xml_pair(std::string tag, std::string value)
      : tag(tag), value(value) {}

    xml_pair()
      : tag(std::string()), value(std::string()) {}
  };
  // для однозначной индификации пары тэг-значение
  class xml_node {
  public:
    // name of node
    std::string name;
    // номер первой строки ЭТОГО узла в документе
    int line_number;
    // номер ПОСЛЕДНЕЙ строки ЭТОГО узла в документе
    int last_line_number;
    // указатели на начала строк в xml
    const std::vector<char *> &content;
    // копии извлеченныx данных с тэгами с ключем
    //   - порядковым номером в векторе "content"
    std::map<int, xml_pair> data;
    // внутренние узлы xml_node
    std::vector<std::unique_ptr<xml_node>> inner_nodes;
    // требуется ли перекодирование из utf8 в cp1251
    bool utf8toCP1251;
    // установим в true если формат документа не правильный
    bool is_ill_formed;

  public:
    xml_node(int line_number,
         const std::vector<char *> &content, bool utf8toCP1251);
    // return nullptr if nodename not in inner_root
    xml_node *GetNode(const std::string &nodename) const;
    // return "" if tag not in data
    std::string GetValue(const std::string &tag) const;

  private:
    bool init();
  };

  // указатели на начала строк в xml
  std::vector<char *> content_;
  std::unique_ptr<xml_node> root_node;
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
  // init by file
  static XMLReader *Init(char *filename, bool utf8toCP1251);
  // inti by char array
  static XMLReader *Init(char *begin, char *end, bool utf8toCP1251);

  ~XMLReader();

  char *GetBuffer();
  int   GetBufferSize();
  /// get pointer to line with this tag
  std::string Find_first_value(const std::string &tag);
  std::string Find_last_value(const std::string &tag);
  /// get first tag in str
};

#endif // ! _CORE__COMMON__XML_READER_H_
