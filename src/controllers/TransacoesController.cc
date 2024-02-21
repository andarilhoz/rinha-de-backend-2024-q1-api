/**
 *
 *  TransacoesController.cc
 *  This file is generated by drogon_ctl
 *
 */

#include "TransacoesController.h"
#include <string>


void TransacoesController::create(const HttpRequestPtr &req,
                                  std::function<void(const HttpResponsePtr &)> &&callback,
                                  int &&id,
                                  TransacaoRequest &&transacao)
{

    LOG_INFO << "Iniciando a criação de transação para o cliente ID: " << id;
    
    auto callbackPtr = std::make_shared<std::function<void(const HttpResponsePtr &)>>(std::move(callback));
    auto dbClientPtr = drogon::app().getDbClient();
    if(!dbClientPtr){
        LOG_ERROR << "DB not initialized";
    }

    const auto transPtr = dbClientPtr->newTransaction();

    if(transacao.tipo == "d"){
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
            if(transacao.tipo == "c" && saldo - transacao.valor < limite * -1){
                LOG_ERROR << "Limite de crédito excedido";
                auto resp = HttpResponse::newHttpResponse();
                resp->setStatusCode(k422UnprocessableEntity);
                transPtr->rollback();
                (*callbackPtr)(resp);
                return;
            }
            
            auto respBody = Json::Value();
            respBody["limite"] = limite;
            respBody["saldo"] = saldo;
            
            auto resp = HttpResponse::newHttpResponse();
            resp->setStatusCode(k200OK);
            LOG_INFO << "Transação concluída com sucesso";
            (*callbackPtr)(resp);
        },
        [callbackPtr](const drogon::orm::DrogonDbException &e){
            LOG_ERROR << "Erro ao executar SQL: " << e.base().what();
            auto resp = HttpResponse::newHttpResponse();
            resp->setStatusCode(HttpStatusCode::k500InternalServerError);
            (*callbackPtr)(resp);
        }, id);
}

void TransacoesController::updateSaldo(int id, int valor, const DbClientPtr &dbClient)
{
    std::string sqlUpdate = "UPDATE saldos SET valor = valor + $2 WHERE cliente_id = $1";
    dbClient->execSqlAsync(sqlUpdate,
    [=](const drogon::orm::Result &result){

    },
    [=](const drogon::orm::DrogonDbException &e){
        LOG_ERROR << e.base().what();

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
            LOG_ERROR << e.base().what();
        }
    );
}
