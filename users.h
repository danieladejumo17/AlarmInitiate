#ifndef USERS_ENDPOINTS_ROUTER
#define USERS_ENDPOINTS_ROUTER

#include "ArduinoExpress.h"
#include "authenticate.h"
#include "radio.h"


namespace routers{
  static auto router_users = []()->ArduinoExpressRouter&{
      static int count = 0;
      static auto router = ArduinoExpress::Router();
      count = count + 1;

      if(count > 1) return router;
      
      router.use(authenticate_users);

      router.get("/authenticate", [](Req &req, Res &res)->void*{
          res.json(200, R"==(
              {
                  "success": false,
                  "error": "This endpoint is yet to be implemented."
              }
          )==");
      });

      router.post("/roger", [](Req &req, Res &res)->void*{
          res.json(200, R"==(
              {
                  "success": false,
                  "error": "This endpoint is yet to be implemented."
              }
          )==");
      });

      router.post("/raise_alarm", [](Req &req, Res &res)->void*{
        char msg[120];
        // TODO: Compute the dynamic length of the whole message to set the array size
        // TODO: Compute the length of the message you are sending here and send it as part of the headers   
        snprintf_P(msg, sizeof(msg), "/raise_alarm\n%s\n\n{\"house\": %s,\"user\": %s,\"time\": %s}\n\n",
        ALARM_SERVER_KEY, "Hostel 1", "Martins M.K", "12-01-2022");

          radio.send(/*msg*/ "on", [](bool success, const char* msg){
            // If the message was directed back to you
            // Process your message
            // Call done 
          }, 3);


        res.json(200, R"==(
            {
                "success": true,
                "error": "This endpoint (/raise_alarm) is yet to be implemented."
            }
        )==");
      });

    // Only the persn who raise an alarm is allowed to use this endpoint, 1. there is an alarm 2. the right user
      router.post("/alarm_message", [](Req &req, Res &res)->void*{
          res.json(200, R"==(
              {
                  "success": false,
                  "error": "This endpoint is yet to be implemented."
              }
          )==");
      });

      router.post("/raise_alarm_for", [](Req &req, Res &res)->void*{
          res.json(200, R"==(
              {
                  "success": false,
                  "error": "This endpoint is yet to be implemented."
              }
          )==");
      });

      router.post("/off_alarm", [](Req &req, Res &res)->void*{
          res.json(200, R"==(
              {
                  "success": true,
                  "error": "This endpoint is yet to be implemented."
              }
          )==");
      });

      router.post("/update_user", [](Req &req, Res &res)->void*{
          res.json(200, R"==(
              {
                  "success": false,
                  "error": "This endpoint is yet to be implemented."
              }
          )==");
      });

  };

  static auto &users = router_users();
}

#endif
