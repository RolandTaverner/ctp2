#pragma once

#include <stdexcept>
#include <string>
#include <vector>

#include <unicode/unistr.h>
#include <unicode/ustring.h>

namespace TileEngine::D3d {

/*
 Copy-pasted from http://www.cplusplus.com/forum/windows/48428/
*/

/** Converts a std::wstring into a std::string with UTF-8 encoding.
 */
template < typename StringT >
StringT utf8(std::wstring const & rc_string);

/** Converts a std::String with UTF-8 encoding into a std::wstring.
 */
template < typename StringT >
StringT utf8(std::string const & rc_string);

/** Nop specialization for std::string.
 */
template < >
inline std::string utf8(std::string const & rc_string) {
  return rc_string;
}

/** Nop specialization for std::wstring.
 */
template < >
inline std::wstring utf8(std::wstring const & rc_string) {
  return rc_string;
}

template < >
std::string utf8(std::wstring const & rc_string) {
  std::string result;
  if (rc_string.empty())
    return result;

  std::vector<UChar> buffer;

  result.resize(rc_string.size() * 3); // UTF-8 uses max 4 bytes per char
  buffer.resize(rc_string.size() * 2); // UTF-16 uses 2 code-points per char

  UErrorCode status = U_ZERO_ERROR;
  int32_t len = 0;

  u_strFromWCS(
    &buffer[0],
    buffer.size(),
    &len,
    &rc_string[0],
    rc_string.size(),
    &status
  );
  if (!U_SUCCESS(status)) {
    throw std::runtime_error("utf8: u_strFromWCS failed");
  }
  buffer.resize(len);

  u_strToUTF8(
    &result[0],
    result.size(),
    &len,
    &buffer[0],
    buffer.size(),
    &status
  );
  if (!U_SUCCESS(status)) {
    throw std::runtime_error("utf8: u_strToUTF8 failed");
  }
  result.resize(len);

  return result;
}/* end of utf8 ( ) */


template < >
std::wstring utf8(std::string const & rc_string) {
  std::wstring result;
  if (rc_string.empty())
    return result;

  std::vector<UChar> buffer;

  result.resize(rc_string.size());
  buffer.resize(rc_string.size());

  UErrorCode status = U_ZERO_ERROR;
  int32_t len = 0;

  u_strFromUTF8(
    &buffer[0],
    buffer.size(),
    &len,
    &rc_string[0],
    rc_string.size(),
    &status
  );
  if (!U_SUCCESS(status)) {
    throw std::runtime_error("utf8: u_strFromUTF8 failed");
  }
  buffer.resize(len);

  u_strToWCS(
    &result[0],
    result.size(),
    &len,
    &buffer[0],
    buffer.size(),
    &status
  );
  if (!U_SUCCESS(status)) {
    throw std::runtime_error("utf8: u_strToWCS failed");
  }
  result.resize(len);

  return result;
}/* end of utf8 ( ) */

} // namespace TileEngine::D3d