#ifndef ADMIN_ENDPOINTS_ROUTER
#define ADMIN_ENDPOINTS_ROUTER

#include "ArduinoExpress.h"
#include "authenticate.h"


namespace routers{
  static auto router_admin = []()->ArduinoExpressRouter&{
      static int count = 0;
      static auto router = ArduinoExpress::Router();
      count = count + 1;

      if(count > 1) return router;
      
      router.use(authorize_admin);

      router.post("/config_gateway", [](Req &req, Res &res)->void*{
          res.json(200, R"==(
              {
                  "success": false,
                  "error": "This endpoint is yet to be implemented."
              }
          )==");
      });

      router.post("/config_alarm_server", [](Req &req, Res &res)->void*{
          res.json(200, R"==(
              {
                  "success": false,
                  "error": "This endpoint is yet to be implemented."
              }
          )==");
      });

      router.get("/users", [](Req &req, Res &res)->void*{
          res.json(200, R"==(
              {
                  "success": false,
                  "error": "This endpoint is yet to be implemented."
              }
          )==");
      });

      router.post("/users/add-user", [](Req &req, Res &res)->void*{
          res.json(200, R"==(
              {
                  "success": false,
                  "error": "This endpoint is yet to be implemented."
              }
          )==");
      });

      router.post("/users/remove-user", [](Req &req, Res &res)->void*{
          res.json(200, R"==(
              {
                  "success": false,
                  "error": "This endpoint is yet to be implemented."
              }
          )==");
      });


      


  };

  static auto &admin = router_admin();
}

#endif