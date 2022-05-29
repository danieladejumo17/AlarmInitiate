#ifndef ANY_CLIENT_ENDPOINTS_ROUTER
#define ANY_CLIENT_ENDPOINTS_ROUTER
#include "ArduinoExpress.h"

namespace routers{
  static auto router_any = []()->ArduinoExpressRouter&{
    static int count = 0;
    static auto router = ArduinoExpress::Router();
    count = count + 1;

    if(count > 1) return router;
    
    router.get("/ping", [](Req &req, Res &res)->void*{
      res.send(200, "text/plain", "ALARM_INITIATE");  
    });
    
    router.get("/state", [](Req &req, Res &res)->void*{
      res.json(200, R"==(
        {
          "emergency": false,
          "details": null
        }
      )==");
    });
        
    router.get("/history", [](Req &req, Res &res)->void*{
      res.json(200, "[]");
    });

    
    return router;
  };

  static auto &any = router_any();
}

#endif