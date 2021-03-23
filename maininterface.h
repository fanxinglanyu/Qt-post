#ifndef MAININTERFACE_H
#define MAININTERFACE_H

#include <QMainWindow>
#include"httpnetworkrequest.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainInterface; }
QT_END_NAMESPACE

class MainInterface : public QMainWindow
{
    Q_OBJECT

public:
    MainInterface(QWidget *parent = nullptr);
    ~MainInterface();

private:
    Ui::MainInterface *ui;

private:
    //执行的post类型
    enum ExecutePostType{
        TestOperate = 0,//进货操作
    };
    //post请求
    HttpNetworkRequest* m_httpNetworkRequest;

    QJsonObject m_jsonData;
    /**
     * @brief testPost：调用post方法
     */
    void testPost();
    /**
     * @brief testJsonData:测试json数据
     */
    void testJsonData();
    /**
     * @brief analysisJsonData：解析json数据
     */
    void analysisJsonData();

};
#endif // MAININTERFACE_H
