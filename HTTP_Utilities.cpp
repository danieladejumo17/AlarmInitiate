#include "HTTP_Utilities.h"

String toText(const HTTP_Method& method)
{
  switch(method)
  {
    case HTTP_Method::GET: return "GET";
    case HTTP_Method::POST: return "POST";
    case HTTP_Method::PUT: return "PUT";
    case HTTP_Method::DELETE: return "DELETE";
    case HTTP_Method::INSPECT: return "INSPECT";
  }
  return "";
}

// JsonObject& textToJSON(const char* text, int size)
// {
//   const size_t capacity = JSON_ARRAY_SIZE(2) + JSON_OBJECT_SIZE(3) + size;
//   DynamicJsonBuffer jsonBuffer(capacity);

//   //  const JsonObject& json = jsonBuffer.parseObject(text);
//   //  if (!json.success()) throw "Parse Failed";
//   //  return json;

//   return jsonBuffer.parseObject(text);
// }


String HTTPStatusText(int status)
{
  if(status == 200) return "OK";
  else if(status == 400) return "Bad Request";
  else if(status == 401) return "Unauthorized";
  else if(status == 403) return "Forbidden";
  else if(status == 404) return "Not Found";
  else if(status == 405) return "Method Not Allowed";
  else if(status == 500) return "Internal Server Error";
  return "Unknown";
}

// void logErr(const String& err)
// {
//   Serial.println(String("ERR: ") + err);
// }
// void logWarn(const String& warning)
// {
//   Serial.println(String("WARN: ") + warning);
// }
// void debug(const String& debugString)
// {
//   Serial.println(String("DEBUG: ") + debugString);
// }
// void logInfo(const String& info)
// {
//   Serial.println(String("INFO: ") + info);
// }
