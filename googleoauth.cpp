#include "googleoauth.h"
#include "myoauthhttpserverreplyhandler.h"
#include <QSettings>
GoogleOAuth::GoogleOAuth(QObject *parent) : QObject(parent)
{
    this->google = new QOAuth2AuthorizationCodeFlow(this);
    this->google->setScope("email");
    this->google->setScope("openid");

    // Google one-time code couldn't be process
    // So need this function to transform the code
    this->google->setModifyParametersFunction([](QAbstractOAuth::Stage stage,
                                              QVariantMap* parameters)
    {
        if(stage == QAbstractOAuth::Stage::RequestingAccessToken){
            QByteArray code = parameters->value("code").toByteArray();
            (*parameters)["code"] = QUrl::fromPercentEncoding(code);
        }
    });

    connect(this->google, &QOAuth2AuthorizationCodeFlow::authorizeWithBrowser, [=](QUrl url) {
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
    const QUrl authUri(settingsObject["auth_uri"].toString());
    const auto clientId = settingsObject["client_id"].toString();
    const QUrl tokenUri(settingsObject["token_uri"].toString());
    const auto clientSecret = settingsObject["client_secret"].toString();
    const QUrl redirectUri(settingsObject["redirect_uri"].toString());
    const auto port = static_cast<quint16>(redirectUri.port());

    this->google->setAuthorizationUrl(authUri);
    this->google->setClientIdentifier(clientId);
    this->google->setAccessTokenUrl(tokenUri);
    this->google->setClientIdentifierSharedKey(clientSecret);

    this->google->setModifyParametersFunction([](QAbstractOAuth::Stage stage, QVariantMap* parameters) {
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
        emit this->loginToGetVirtualNetworks();
    });
    this->google->setReplyHandler(replyHandler);
}
GoogleOAuth::~GoogleOAuth()
{
    delete this->google;
}

void GoogleOAuth::grant()
{
    this->google->grant();
}

void GoogleOAuth::clearToken()
{
    QSettings settings;
    settings.setValue("idToken", "");
    settings.setValue("accessToken","");
    settings.setValue("refreshToken","");
}
