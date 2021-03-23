#include "maininterface.h"
#include "ui_maininterface.h"

#include<QJsonDocument>
#include<QDebug>
#include<QJsonArray>

MainInterface::MainInterface(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainInterface)
{
    ui->setupUi(this);
    //创建对象
    m_httpNetworkRequest = new HttpNetworkRequest;
//    testPost();
    testJsonData();
    //处理post返回的结果
    connect(m_httpNetworkRequest, &HttpNetworkRequest::sendDealDataResult,
            this, [&](int result, int type){

    });
}

MainInterface::~MainInterface()
{
    //销毁对象
    delete  m_httpNetworkRequest;
    delete ui;
}
//发送post请求
void MainInterface::testPost()
{
    //要发送的json数据
    QJsonObject obj;
    obj.insert("accountId", "1");
    QJsonDocument jsonDoc(obj);//QJsonObject转QJsonDocument
    QString str = QString(jsonDoc.toJson());//QJsonDocument转QString
    QByteArray content = str.toUtf8();
    //发送post请求
    m_httpNetworkRequest->receivePostRequestType(ExecutePostType::TestOperate, content);
}

void MainInterface::testJsonData()
{
    //要发送的json数据
    QJsonObject sendJsonData;
    sendJsonData.insert("code", 200);
    //加入array
    QJsonArray arry;
    QJsonObject tempObject1;
    tempObject1.insert("name", "小明");
    tempObject1.insert("height", 180);
    tempObject1.insert("weight", 70.3);
    arry.append(tempObject1);
    QJsonObject tempObject2;
    tempObject2.insert("name", "小花");
    tempObject2.insert("height", 165);
    tempObject2.insert("weight", 45.8);
    arry.append(tempObject2);
    sendJsonData.insert("people", QJsonValue(arry));
    //加入object
    QJsonObject tempObject;
    tempObject.insert("school", "Carnegie Mellon University");
    tempObject.insert("location", "PIT");
    sendJsonData.insert("affiliation", QJsonValue(tempObject));

    m_jsonData = sendJsonData;
    qDebug()<<"sendJsonData:"<<sendJsonData;
    //解析数据
    analysisJsonData();

}

void MainInterface::analysisJsonData()
{
    //解析code
    if(m_jsonData.contains("code")){
        qDebug()<<"code:"<<m_jsonData.value("code").toInt();
    }
    //解析array
    if(m_jsonData.contains("people")){
      QJsonValue peopleObject = m_jsonData.take("people");
      if(peopleObject.isArray()){
          QJsonArray peopleArray = peopleObject.toArray();
          for(int i = 0;i < peopleArray.size();++i){
              QJsonValue value = peopleArray.at(i);
              qDebug()<<"name:"<<value["name"].toString();
              qDebug()<<"height:"<<value["height"].toInt();
              qDebug()<<"weight:"<<value["weight"].toDouble();
          }
      }
    }
    //解析obect
    if(m_jsonData.contains("affiliation")){
        QJsonValue affiliationValue = m_jsonData.value("affiliation");
        QJsonObject affiliationObject = affiliationValue.toObject();
        if(affiliationObject.contains("school")){
            qDebug()<<"school:"<<affiliationObject.value("school").toString();
            qDebug()<<"location:"<<affiliationObject.value("location").toString();
        }
    }

}


