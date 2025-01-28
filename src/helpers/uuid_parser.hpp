#include <boost/uuid/string_generator.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_io.hpp>

namespace userver::storages::postgres::io {

// Парсер для UUID
template <>
struct BufferParser<boost::uuids::uuid> {
  static boost::uuids::uuid Parse(const FieldBuffer& buffer) {
    return boost::uuids::string_generator{}(buffer.ToString());
  }
};

// Форматтер для UUID
template <>
struct BufferFormatter<boost::uuids::uuid> {
  static void Format(const boost::uuids::uuid& value, FieldBuffer& buffer) {
    std::string str = boost::uuids::to_string(value);
    buffer.length = str.size();
    buffer.buffer = reinterpret_cast<const std::uint8_t*>(str.data());
  }
};

}  // namespace userver::storages::postgres::io