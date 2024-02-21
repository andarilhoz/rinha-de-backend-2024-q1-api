#include <drogon/drogon.h>

using namespace drogon;

class TransacaoRequest {
    public:
        int valor;
        std::string tipo;
        std::string descricao;
};

namespace drogon
{
    template<>
    inline TransacaoRequest fromRequest(const HttpRequest &req){
        auto json = req.getJsonObject();
        TransacaoRequest transacaoRequest;
        if(json){
            transacaoRequest.valor = (*json)["valor"].asInt();
            transacaoRequest.tipo = (*json)["tipo"].asString();
            transacaoRequest.descricao = (*json)["descricao"].asString();
        }
        return transacaoRequest;
    }
}