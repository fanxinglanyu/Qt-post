#ifndef HTTPNETWORKREQUEST_H
#define HTTPNETWORKREQUEST_H

#include<QNetworkAccessManager>
#include<QNetworkRequest>
#include<QString>
#include<QObject>
#include<QJsonObject>
#include<QByteArray>

class HttpNetworkRequest : public QObject
{
   Q_OBJECT
public:
    HttpNetworkRequest();
    ~HttpNetworkRequest();

private:
    //执行的post类型
    int m_executePostRequestType = 0;
    //解析的post类型
    int m_analysisPostRequestType = 0;

private:
    //post请求状态
    enum PostStatus{
        SucceedDeal = 0,//成功处理
        ServerMaintenanceError = 1,//服务器维护状态
        SeverDisconnectError = 2,//断网状态
        NoPermissionError = 3,//没有权限
        DataAbnormalError = 4,//json数据异常错误
        DealDatabaseError = 5//处理数据库异常错误
    };
    //请求的ip头
    QString m_requestIPHead = "https://baidu.com";
    //解析的post类型
    enum AnalysisPostType{
        TestRequest = 0,//工作模式
    };
    //执行的post类型
    enum ExecutePostType{
        TestOperate = 0,//进货操作
    };


    //得到消息头的token
    QString m_accessToken;
    //发送网络请求和接受应答
    QNetworkAccessManager* m_networkAccessManager;
    //请求的消息题
    QNetworkRequest m_httpRequest;
    //发送data数据
    QByteArray m_sendJsonData;

     /**************请求**********************/
    /**
     * @brief 测试test的post请求
     */
    void testHttpRequestPost();

    /**************处理**********************/

    /**
     * @brief 处理服务器状态请求
     * @param jsonObject
     */
    bool dealTestRequest(QJsonObject& jsonObject);
private slots:
    /**
     * @brief 处理post请求发回的数据的总接口
     * @param reply
     */
    void dealNetworkFinisheded(QNetworkReply *reply);

//调用唯一接口
public slots:

    /**
     * @brief 接收请求操作的post类型
     */
    void receivePostRequestType(int instruct, const QByteArray& content = "");
signals:
    /**
     * @brief 处理数据的结果
     * @param type:界面类型
     * 处理回传参数post，各种异常
     */
    void sendDealDataResult(int result, int type);


};

#endif // HTTPNETWORKREQUEST_H
