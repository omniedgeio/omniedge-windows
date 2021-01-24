#include "menuflow.h"

MenuFlow::MenuFlow(QQmlApplicationEngine* engine)
{
    qmlEngine = engine;
    oauth = new OAuth();
    proxy = new OmniProxy();
    n2n = new N2NWorkerWrapper();
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
		Response res = proxy->getVirtualNetworks();
        if (res.status == ResponseStatus::Success) {
            qmlEngine->rootContext()->setContextProperty("virtualNetworks", res.data["items"].toList());
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
        qmlEngine->rootContext()->setContextProperty("loading", false);
    });
}

void MenuFlow::logout()
{
    QSettings settings;
    settings.clear();
    emit loginStatus(false);
}
