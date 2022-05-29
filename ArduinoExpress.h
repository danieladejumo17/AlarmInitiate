#ifndef ARDUINO_EXPRESS_HEADER
#define ARDUINO_EXPRESS_HEADER

#include <ESP8266WiFi.h>
#include "HTTP_Request.h"
#include "HTTP_Response.h"
#include <Vector.h>

// Req is an alias for HTTP_Request, Res is an alias for HTTP_Response
using Req = HTTP_Request;
using Res = HTTP_Response;

// Next is an alias for void(*)(), which is a function that takes no arguments and returns void
using Next = std::function<void()>;

/* Inherited by RouteCallback, MiddlewarCallback and the ArduinoExpressRouter which implement 
   * the pure virtual execute() method.
   *
   * method: execute(route, req, res, next)
   *  - route: the route this callback is called on. A callback will respond to one or more routes.
   *  - req: a reference to the HTTP_Request object
   *  - res: a reference to this request's response, an HTTP_Response object that can be populated 
   *    with data passed to each callback
   *  - next: next() will call the next callback in the callback chain handled by the ArduinoExpressRouter
   *  
   *  calling res.send() will send the response to the client, and terminate the callback chain
   * */
struct Callback{
  virtual void execute(const String&, Req&, Res&, Next ) = 0;
};


struct RouteCallback: public Callback{
  private:
    String _path;
    HTTP_Method _method;
    void *(*_middleware)(Req&, Res&) = nullptr;
    void *(*_callback)(Req&, Res&) = nullptr;
  
  public:
    RouteCallback() {}
    RouteCallback(HTTP_Method method, const String& path, 
                  void *(*middleware)(Req&, Res&), 
                  void *(*callback)(Req&, Res&))
      : _method{method}, _path{path}, _middleware{middleware}, _callback{callback} {}
    
    // void get(path, middleware, callback)
    // creates a callback that responds to a GET request to the given path
    void get(const String&, void *(*)(Req&, Res&), void *(*)(Req&, Res&));
    
    // void post(path, middleware, callback)
    // creates a callback that responds to a POST request to the given path
    void post(const String&, void *(*)(Req&, Res&), void *(*)(Req&, Res&));
    
    // void inspect(path, middleware, callback)
    // creates a callback that responds to an INSPECT request to the given path
    void inspect(const String&, void *(*)(Req&, Res&), void *(*)(Req&, Res&));

    // void put(const String&, void *(*)(Req&, Res&), void *(*)(Req&, Res&));
    // void deleteMethod(const String&, void *(*)(Req&, Res&), void *(*)(Req&, Res&));
    

    /* returns the path this callback is created on. 
      * This can be different from the complete HTTP request route if a router with a prefix is used.
      * */
    const String& path() const {return this->_path;}

    /* executes all the middlewares and callbacks on this route */
    void executeCallbacks(Req&, Res& );

    /* Returns true if the request matches this route */
    bool match(const String&, const Req& ) const;

    /* void execute(prefix, req, res, next)
      * - prefix: the prefix used by the router that this callback belongs to
      * executes the callback, if the request matches this route 
      * */
    void execute(const String&, Req&, Res&, Next);
};


struct MiddlewareCallback: public Callback{
  private:
    String _path;
    void *(*_callback)(HTTP_Request&, HTTP_Response&, Next) = nullptr;

  public:
    MiddlewareCallback() {}
    MiddlewareCallback(const String& path, void *(*callback)(HTTP_Request&, HTTP_Response&, Next)): _path{path}, _callback{callback} {}
    const String& path() const {return this->_path;}
    void executeCallbacks(HTTP_Request &req, HTTP_Response &res, Next next) {this->_callback(req, res, next);}
    bool match(const String &prefix, const HTTP_Request &req) const {return req.route.startsWith(prefix + this->_path) || (prefix + this->_path).isEmpty();}
    void execute(const String &prefix, HTTP_Request &req, HTTP_Response &res, Next next) 
    {
      if (match(prefix, req)){
        executeCallbacks(req, res, next);
      }
    }
  
};


struct ArduinoExpressRouter: public Callback{
  protected:
    String _routePrefix = "";
    
    const static int MAX_ROUTECALLBACKS_COUNT = ArduinoExpressConfig::MAX_ROUTECALLBACKS_COUNT;
    RouteCallback _routeCallbacksStorageArray[MAX_ROUTECALLBACKS_COUNT];
    Vector<RouteCallback> _routeCallbacks{_routeCallbacksStorageArray};

    const static int MAX_MIDDLEWARECALLBACKS_COUNT = ArduinoExpressConfig::MAX_MIDDLEWARECALLBACKS_COUNT;
    MiddlewareCallback _middlewareCallbacksStorageArray[MAX_MIDDLEWARECALLBACKS_COUNT];
    Vector<MiddlewareCallback> _middlewareCallbacks{_middlewareCallbacksStorageArray};

    const static int MAX_CALLBACKS_COUNT = ArduinoExpressConfig::MAX_CALLBACKS_COUNT;
    Callback* _callbacksStorageArray[MAX_CALLBACKS_COUNT];
    Vector<Callback*> _allCallbacks{_callbacksStorageArray};

    int _currentCallback = 0;

    void addRouteCallback(const RouteCallback& );
    void addAllCallback(Callback* );

    bool match(const String&, const HTTP_Request&) const;
    virtual void executeNext(const String&, HTTP_Request&, HTTP_Response&);
    virtual void execute(const String&, HTTP_Request&, HTTP_Response&, Next); // if route prefix matches (or is empty), execute middleswares, callbacks

  public:
    void get(const String&, void *(*)(HTTP_Request&, HTTP_Response&));
    void get(const String&, void *(*)(HTTP_Request&, HTTP_Response&), void *(*)(HTTP_Request&, HTTP_Response&));
    void post(const String&, void *(*)(HTTP_Request&, HTTP_Response&));
    void post(const String&, void *(*)(HTTP_Request&, HTTP_Response&), void *(*)(HTTP_Request&, HTTP_Response&));

    void setRoutePrefix(const String& prefix) {this->_routePrefix = prefix;}
    virtual void use(const String&, void *(*)(HTTP_Request&, HTTP_Response&, Next));
    virtual void use(void *(*)(HTTP_Request&, HTTP_Response&, Next));
};


struct ArduinoExpress : public ArduinoExpressRouter
{
  private:
    WiFiClient* _client = nullptr;
    WiFiServer _server{80};

    HTTP_Request _req;
    HTTP_Response _res{nullptr};

    void parseRequest();
    void execute();

    const static int MAX_ROUTERS_COUNT = ArduinoExpressConfig::MAX_ROUTERS_COUNT;
    ArduinoExpressRouter* _routersStorageArray[MAX_ROUTERS_COUNT];
    Vector<ArduinoExpressRouter*> _routers{_routersStorageArray};
    

  public:
    // Pass a callback function to perform tasks at the end of each ArduinoExpress pass
    void listen(int port, std::function<void()> callback);
    void use(const String&, ArduinoExpressRouter* );
    void use(ArduinoExpressRouter* );
    
    void use(const String &path, void *(*callback)(HTTP_Request&, HTTP_Response&, Next)) {ArduinoExpressRouter::use(path, callback);}
    void use(void *(*callback)(HTTP_Request&, HTTP_Response&, Next)) {ArduinoExpressRouter::use(callback);}
    
    static ArduinoExpressRouter Router() {return ArduinoExpressRouter();}
  
};

#endif
