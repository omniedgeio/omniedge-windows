#include "menuflow.h"

MenuFlow::MenuFlow(QQmlApplicationEngine* engine)
{
    qmlEngine = engine;
    oauth = new OAuth();
    proxy = new OmniProxy();
    n2n = new N2NWorkerWrapper();
}
MenuFlow::~MenuFlow()
{

}
bool MenuFlow::checkToken()
{
    QSettings settings;
    if(settings.contains("idToken") &&
    settings.contains("accessToken") &&
    settings.contains("refreshToken")){
        qDebug() << "User have token.";
        Response response = proxy->refreshToken();
		if(response.status == ResponseStatus::Success){
            emit loginStatus(true);
			return true;
		}
		emit showMessage("Session Expired", "Please login again.");
    }
    qDebug() << "User dont have token.";
    emit loginStatus(false);
	return false;
}

void MenuFlow::getVirtualNetworks()
{
	if(this->checkToken())
    {
        virtualNetworksID.clear();
		Response res = proxy->getVirtualNetworks();
        if (res.status == ResponseStatus::Success) {
            qmlEngine->rootContext()->setContextProperty("virtualNetworks", res.data["items"].toList());
            virtualNetworksID = res.data["items"].toList().first().toMap().value("id").toString();
		} else {
            emit showMessage("Unknown Error", "We apologise for the error. Please send us an email to report.");
		}
	}
}
void MenuFlow::joinVirtualNetwork()
{
    if(this->checkToken())
    {
        Response res = proxy->joinVirtualNetwork(virtualNetworksID);
        if (res.status == ResponseStatus::Success) {
            qmlEngine->rootContext()->setContextProperty("myVirtualIP", res.data["virtualIP"].toString());
            qDebug()<<res.data<<"virtualIP"<<res.data["virtualIP"].toString();
        } else {
            emit showMessage("Unknown Error", "We apologise for the error. Please send us an email to report.");
        }
    }
}
void MenuFlow::getUserInfo()
{
	if(this->checkToken())
	{
		Response res = proxy->getUserInfo();
        if (res.status == ResponseStatus::Success) {
			qmlEngine->rootContext()->setContextProperty("user", res.data);
		} else {
            emit showMessage("Unknown Error", "We apologise for the error. Please send us an email to report.");
		}
	}
}

void MenuFlow::authenticate()
{
    oauth->grant();
    connect(oauth, &OAuth::granted, [=](){
        qmlEngine->rootContext()->setContextProperty("loading", true);
        getUserInfo();
        getVirtualNetworks();
        joinVirtualNetwork();
        qmlEngine->rootContext()->setContextProperty("loading", false);
    });
}

void MenuFlow::logout()
{
    QSettings settings;
    settings.clear();
    n2n->stopEdge();
    emit loginStatus(false);
}

void MenuFlow::connectSN()
{
    n2n->setVirtualIp("10.254.1.22","twofish");
    n2n->startEdge("omniedge","66YRd88kyYdhzk");
}

void MenuFlow::disconnectSN()
{
    n2n->stopEdge();
}
