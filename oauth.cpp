#include "oauth.h"
#include "myoauthhttpserverreplyhandler.h"
#include <QSettings>

OAuth::OAuth(QObject *parent) : QObject(parent)
{
    this->oauth = new QOAuth2AuthorizationCodeFlow(this);
    this->oauth->setScope("email");
    this->oauth->setScope("openid");

    // Google one-time code couldn't be process
    // So need this function to transform the code
    /*this->oauth->setModifyParametersFunction([](QAbstractOAuth::Stage stage,
                                              QVariantMap* parameters)
    {
        if(stage == QAbstractOAuth::Stage::RequestingAccessToken){
            QByteArray code = parameters->value("code").toByteArray();
            (*parameters)["code"] = QUrl::fromPercentEncoding(code);
        }
    });*/

    connect(this->oauth, &QOAuth2AuthorizationCodeFlow::authorizeWithBrowser, [=](QUrl url) {
        QUrlQuery query(url);

        query.addQueryItem("prompt", "consent");      // Param required to get data everytime
        query.addQueryItem("access_type", "offline"); // Needed for Refresh Token (as AccessToken expires shortly)
        url.setQuery(query);
        QDesktopServices::openUrl(url);
    });

    QByteArray val;
    QFile file;
    file.setFileName(":/oauth.json");
    if(file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        val = file.readAll();
        file.close();
    }

    QJsonDocument document = QJsonDocument::fromJson(val);
    QJsonObject object = document.object();
    const auto settingsObject = object["dev"].toObject();
    const QUrl authUri(settingsObject["cognito_uri"].toString() + "/login");
    const auto clientId = settingsObject["client_id"].toString();
    const QUrl tokenUri(settingsObject["cognito_uri"].toString() + "/oauth2/token");
    const auto clientSecret = settingsObject["client_secret"].toString();
    const QUrl redirectUri(settingsObject["redirect_uri"].toString());
    const auto port = static_cast<quint16>(redirectUri.port());

    this->oauth->setAuthorizationUrl(authUri);
    this->oauth->setClientIdentifier(clientId);
    this->oauth->setAccessTokenUrl(tokenUri);
    this->oauth->setClientIdentifierSharedKey(clientSecret);

    this->oauth->setModifyParametersFunction([](QAbstractOAuth::Stage stage, QVariantMap* parameters) {
          // Percent-decode the "code" parameter so Google can match it
          if (stage == QAbstractOAuth::Stage::RequestingAccessToken) {
              QByteArray code = parameters->value("code").toByteArray();
              (*parameters)["code"] = QUrl::fromPercentEncoding(code);
          }
      });
    auto replyHandler = new MyOAuthHttpServerReplyHandler(port,this);

    connect(replyHandler, &MyOAuthHttpServerReplyHandler::tokensReceived, [=](const QVariantMap &data){
        QSettings settings;
        settings.setValue("idToken", data["id_token"]);
        settings.setValue("accessToken", data["access_token"]);
        settings.setValue("refreshToken", data["refresh_token"]);
        qDebug() << "ID TOKEN " << settings.contains("idToken");
        qDebug() << "ACCESS TOKEN " << settings.contains("accessToken");
        qDebug() << "REFRESH TOKEN " << settings.contains("refreshToken");
        emit granted();
    });
    this->oauth->setReplyHandler(replyHandler);
}

OAuth::~OAuth()
{
    delete this->oauth;
}

void OAuth::grant()
{
    this->oauth->grant();
}

void OAuth::clearToken()
{
    QSettings settings;
    settings.setValue("idToken", "");
    settings.setValue("accessToken","");
    settings.setValue("refreshToken","");
}
