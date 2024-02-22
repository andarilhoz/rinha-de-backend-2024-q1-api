/**
 *
 *  TransacoesValidator.cc
 *
 */

#include "TransacoesValidator.h"

using namespace drogon;

void TransacoesValidator::doFilter(const HttpRequestPtr &req,
                         FilterCallback &&fcb,
                         FilterChainCallback &&fccb)
{
    auto param = req->getRoutingParameters();
    auto id = param[0];

    if(id.empty()){
        auto res = drogon::HttpResponse::newHttpResponse();
        res->setStatusCode(k422UnprocessableEntity);
        res->setContentTypeCode(ContentType::CT_APPLICATION_JSON);
        res->setBody("{\"erro\": \"ID do cliente deve estar presente na URL\"}");
        fcb(res);
        return;
    }

    //validate if id is int
    if(!std::all_of(id.begin(), id.end(), ::isdigit)){
        auto res = drogon::HttpResponse::newHttpResponse();
        res->setStatusCode(k422UnprocessableEntity);
        res->setContentTypeCode(ContentType::CT_APPLICATION_JSON);
        res->setBody("{\"erro\": \"ID do cliente deve ser um inteiro positivo\"}");
        fcb(res);
        return;
    }


    auto json = req->getJsonObject();
    if(json){
        if(!(*json)["valor"].isInt() || (*json)["valor"].asInt() <= 0){
            auto res = drogon::HttpResponse::newHttpResponse();
            res->setStatusCode(k422UnprocessableEntity);
            res->setContentTypeCode(ContentType::CT_APPLICATION_JSON);
            res->setBody("{\"erro\": \"Valor deve ser um inteiro positivo e deve estar presente no corpo da requisição em formato JSON\"}");
            fcb(res);
            return;
        }

        if(!(*json)["tipo"].isString() || !(*json)["descricao"].isString()){
            auto res = drogon::HttpResponse::newHttpResponse();
            res->setStatusCode(k422UnprocessableEntity);
            res->setContentTypeCode(ContentType::CT_APPLICATION_JSON);
            res->setBody("{\"erro\": \"Tipo e descrição devem estar presentes no corpo da requisição em formato JSON\"}");
            fcb(res);
            return;
        }

        auto tipo = (*json)["tipo"].asString();
        if(tipo != "c" && tipo != "d"){
            auto res = drogon::HttpResponse::newHttpResponse();
            res->setStatusCode(k422UnprocessableEntity);
            res->setContentTypeCode(ContentType::CT_APPLICATION_JSON);
            res->setBody("{\"erro\": \"Tipo deve ser 'c' para crédito ou 'd' para débito\"}");
            fcb(res);
            return;
        }

        auto descricao = (*json)["descricao"].asString();
        if(descricao.size() > 10 || descricao.size() < 1){
            auto res = drogon::HttpResponse::newHttpResponse();
            res->setStatusCode(k422UnprocessableEntity);
            res->setContentTypeCode(ContentType::CT_APPLICATION_JSON);
            res->setBody("{\"erro\": \"Descrição deve ter entre 1 e 10 caracteres\"}");
            fcb(res);
            return;
        }
        fccb();
    }
    else{
        auto res = drogon::HttpResponse::newHttpResponse();
        res->setStatusCode(k422UnprocessableEntity);
        res->setContentTypeCode(ContentType::CT_APPLICATION_JSON);
        res->setBody("{\"erro\": \"Corpo da requisição deve estar em formato JSON\"}");
        fcb(res);
        return;
    }
}