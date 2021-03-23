#include "httpnetworkrequest.h"

#include<QJsonObject>
#include<QJsonDocument>
#include<QJsonArray>
#include<QNetworkReply>
#include<QDebug>
#include<QVector>
#include<QDateTime>


/*
消息同步逻辑：
   receivePostRequestType(接收执行消息类型)-->workModeQueryRequestPost（进行工作状态查询）【sendServerMaintainStatus信号】
    --->accessTokenHttpRequestPost--->dealTokenReques(根据m_executePostRequestType类型不同，执行不同业务操作)
*/

HttpNetworkRequest::HttpNetworkRequest()
{

    m_networkAccessManager = new QNetworkAccessManager();
    //处理所有post请求
    connect(m_networkAccessManager, &QNetworkAccessManager::finished,
            this, &HttpNetworkRequest::dealNetworkFinisheded);
}

HttpNetworkRequest::~HttpNetworkRequest()
{
    qDebug()<<"HttpNetworkRequest";
    delete m_networkAccessManager;
}
//发送post请求
void HttpNetworkRequest::testHttpRequestPost()
{
    qDebug()<<"testHttpRequestPost";
//    QJsonObject obj;
//    obj.insert("accountId", "1");
//    QJsonDocument jsonDoc(obj);//QJsonObject转QJsonDocument
//    QString str = QString(jsonDoc.toJson());//QJsonDocument转QString
//    QByteArray content = str.toUtf8();
//    int contentLength = content.length();


    m_httpRequest.setUrl(QUrl(m_requestIPHead + "/api/clientversion/client/working/mode/query"));
    m_httpRequest.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    m_httpRequest.setRawHeader("access_token", m_accessToken.toUtf8());
    m_httpRequest.setHeader(QNetworkRequest::ContentLengthHeader, m_sendJsonData.length());

    m_analysisPostRequestType = AnalysisPostType::TestRequest;
    // 发送参数// 将用户名和密码发送至web服务器进行验证
    m_networkAccessManager->post(m_httpRequest, m_sendJsonData);
}
//处理post请求
bool HttpNetworkRequest::dealTestRequest(QJsonObject &jsonObject)
{
    qDebug()<<"dealTestRequest:"<<jsonObject;
    if(jsonObject.contains("code")){
        if(jsonObject.value("code").toInt() == 200){
            //发送处理结果
            emit sendDealDataResult(PostStatus::SucceedDeal, m_executePostRequestType);
            return true;
        }else{
             emit sendDealDataResult(PostStatus::DataAbnormalError, m_executePostRequestType);
            return false;
        }
    }else{
        emit sendDealDataResult(PostStatus::DealDatabaseError, m_executePostRequestType);
        return false;
    }
}
//post处理总入口
void HttpNetworkRequest::dealNetworkFinisheded(QNetworkReply *reply)
{
    //    if(QSslSocket::supportsSsl() == true){
    //       qDebug()<<"支持SSL:"<<QSslSocket::sslLibraryBuildVersionString() + QSslSocket::sslLibraryVersionString();
    //    }else{
    //        qDebug()<<"不支持SSL:"<<QSslSocket::sslLibraryBuildVersionString() + QSslSocket::sslLibraryVersionString();
    //    }
        QVariant data = reply->readAll();//读取全部数据
        QJsonParseError jsonError;
        QJsonDocument document = QJsonDocument::fromJson(data.toByteArray(), &jsonError);
        if (!document.isNull() && (jsonError.error == QJsonParseError::NoError)) {
              qDebug()<<"解析未发生错误";
                qDebug()<<"JSON 文档为对象";
                  QJsonObject object = document.object();  // 转化为对象
                  if (document.isObject()) {
                      switch (m_analysisPostRequestType) {
                        case AnalysisPostType::TestRequest:
                          dealTestRequest(object);
                          break;
                      default:
                          break;
                      }
                  }else{
                      emit sendDealDataResult(PostStatus::DealDatabaseError, m_executePostRequestType);
                  }
        }else{
            if(document.isNull() == true){
                qDebug()<<"document.isNull:" + reply->errorString();
                emit sendDealDataResult(PostStatus::SeverDisconnectError, m_executePostRequestType);
            }else if(jsonError.error == QJsonParseError::NoError){
               qDebug()<<"jsonError.error:" + reply->errorString();
               emit sendDealDataResult(PostStatus::SeverDisconnectError, m_executePostRequestType);
            }
        }
        //删除对象
        reply->deleteLater();
}


//类接口
void HttpNetworkRequest::receivePostRequestType(int instruct, const QByteArray& content){
    //获得执行的指令
    m_executePostRequestType = instruct;
    //获得发送的内容
    m_sendJsonData = content;

    switch (m_executePostRequestType) {
        case ExecutePostType::TestOperate:
            testHttpRequestPost();
            break;
        default:
            break;
    }
}
