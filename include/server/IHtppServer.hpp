
class IHttpServer {
public:
    virtual void AddRoute() = 0;
    virtual void Listen() = 0;
     
};