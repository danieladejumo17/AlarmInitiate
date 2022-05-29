#ifndef AUTHENTICATE_AUTHORIZE_HEADER
#define AUTHENTICATE_AUTHORIZE_HEADER

#include "ArduinoExpress.h"

bool authenticate(Req &req, Res &res)
{
   //Authentication Logic goes here

   // If authentication fails respond with status 401

   // return true/false
   return true;

}

void* authenticate_users(Req &req, Res &res, Next next)
{
    if(authenticate(req, res))
        next();
}

void* authorize_admin(Req &req, Res &res, Next next)
{
    // authenticate_users(req, res, ...)//this should not call a next. We are not done!
    if(authenticate(req, res)){
        // AUTHORIZATION LOGIC GOES HERE

        next();
    }
}


#endif
