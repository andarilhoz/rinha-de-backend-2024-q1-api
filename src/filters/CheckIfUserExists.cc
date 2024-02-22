/**
 *
 *  CheckIfUserExists.cc
 *
 */

#include "CheckIfUserExists.h"


void CheckIfUserExists::doFilter(const HttpRequestPtr &req,
                         FilterCallback &&fcb,
                         FilterChainCallback &&fccb)
{
    auto param = req->getRoutingParameters();
    std::string id = param[0];
    Clientes::PrimaryKeyType client_id = std::stoul(id);

    auto db = drogon::app().getFastDbClient();
    Mapper<Clientes> clientMapper(db);

    clientMapper.findByPrimaryKey(
        client_id,
        [fccb](const Clientes &client) mutable {
            fccb();
        }, 
        [fcb](const DrogonDbException &e){
            LOG_ERROR << "Erro ao buscar cliente: " << e.base().what();
            auto res = HttpResponse::newHttpResponse();
            res->setStatusCode(k404NotFound);
            res->setContentTypeCode(ContentType::CT_APPLICATION_JSON);
            res->setBody("{\"erro\": \"Cliente não encontrado\"}");
            fcb(res);
        }
    );
}
