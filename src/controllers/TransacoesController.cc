/**
 *
 *  TransacoesController.cc
 *  This file is generated by drogon_ctl
 *
 */

#include "TransacoesController.h"
#include <string>
#include <unordered_map>

void TransacoesController::create(const HttpRequestPtr &req,
                                  std::function<void(const HttpResponsePtr &)> &&callback,
                                  int &&id,
                                  TransacaoRequest &&transacao)
{

    LOG_DEBUG << "Iniciando a criação de transação para o cliente ID: " << id;
    auto callbackPtr = std::make_shared<std::function<void(const HttpResponsePtr &)>>(std::move(callback));

    bool client_exists = this->checkIfUserExists(id);
    if(!client_exists){
        LOG_ERROR << "Erro ao buscar cliente: " << id << " nao encontrado";
        auto res = HttpResponse::newHttpResponse();
        res->setStatusCode(k404NotFound);
        res->setContentTypeCode(ContentType::CT_APPLICATION_JSON);
        res->setBody("{\"erro\": \"Cliente não encontrado\"}");
        (*callbackPtr)(res);
        return;
    }
    
    auto dbClientPtr = drogon::app().getFastDbClient();
    if(!dbClientPtr){
        LOG_ERROR << "DB not initialized";
    }

    dbClientPtr->newTransactionAsync([transacao, id, this, callbackPtr](const std::shared_ptr<Transaction> &transPtr){
        LOG_DEBUG << "Transação iniciada";
        if(transacao.tipo == "c"){
            this->updateSaldo(id, transacao.valor, transPtr);
        }else{
            this->updateSaldo(id, transacao.valor * -1, transPtr);
        }

        this->addNewOperation(id, transacao, transPtr);

        std::string saldosELimites = "SELECT c.limite, s.valor AS saldo FROM clientes c JOIN saldos s ON c.id = s.cliente_id WHERE c.id = $1";
        transPtr->execSqlAsync(
            saldosELimites,
            [callbackPtr, transacao, transPtr](const drogon::orm::Result &result){
                if(result.empty()){
                    LOG_ERROR << "Cliente ou saldo não encontrado";
                    auto resp = HttpResponse::newHttpResponse();
                    resp->setStatusCode(k500InternalServerError);
                    (*callbackPtr)(resp);
                    return;
                }
                auto row = result[0];
                auto saldo = row["saldo"].as<int>();
                auto limite = row["limite"].as<int>();
                if(transacao.tipo == "d" && saldo < limite * -1){
                    LOG_DEBUG << "Limite de crédito excedido, saldo: " << saldo << " limite: " << limite << " valor: " << transacao.valor;
                    auto resp = HttpResponse::newHttpResponse();
                    resp->setStatusCode(k422UnprocessableEntity);
                    resp->setBody("Limite de crédito excedido");
                    transPtr->rollback();
                    (*callbackPtr)(resp);
                    return;
                }
                
                auto respBody = Json::Value();
                respBody["limite"] = limite;
                respBody["saldo"] = saldo;
                
                auto resp = HttpResponse::newHttpResponse();
                resp->setStatusCode(k200OK);
                resp->setBody(respBody.toStyledString());
                resp->setContentTypeCode(ContentType::CT_APPLICATION_JSON);
                LOG_DEBUG << "Transação concluída com sucesso";
                (*callbackPtr)(resp);
                return;
            },
            [callbackPtr](const drogon::orm::DrogonDbException &e){
                LOG_ERROR << "Erro ao executar SQL: " << e.base().what();
                auto resp = HttpResponse::newHttpResponse();
                resp->setStatusCode(HttpStatusCode::k500InternalServerError);
                (*callbackPtr)(resp);
                return;
            }, id);
    });   
}

void TransacoesController::updateSaldo(int id, int valor, const DbClientPtr &dbClient)
{
    std::string sqlUpdate = "UPDATE saldos SET valor = valor + $2 WHERE cliente_id = $1";
    dbClient->execSqlAsync(sqlUpdate,
    [=](const drogon::orm::Result &result){

    },
    [=](const drogon::orm::DrogonDbException &e){
        
        LOG_ERROR << "Erro ao atualizar saldo " << e.base().what();

    }, id, valor);
}

void TransacoesController::addNewOperation(int id, TransacaoRequest transacao, const DbClientPtr &dbClient)
{
    auto json = Json::Value();
    json["cliente_id"] = id;
    json["valor"] = transacao.valor;
    json["tipo"] = transacao.tipo;
    json["descricao"] = transacao.descricao;
    json["realizadaEm"] = trantor::Date::now().toFormattedString(false);

    Transacoes novaTransacao = Transacoes(json);

    Mapper<Transacoes> transacoesMapper = Mapper<Transacoes>(dbClient);
    transacoesMapper.insert(
        novaTransacao, 
        [=](const Transacoes &transacao){

        },
        [=](const DrogonDbException &e){

            LOG_ERROR << "Error adicionando nova operacao: " << e.base().what();
        }
    );
}

bool TransacoesController::checkIfUserExists(int id) {
    return id > 0 && id < 6;
}