#include "googleoauth.h"
#include "myoauthhttpserverreplyhandler.h"

GoogleOAuth::GoogleOAuth(QObject *parent) : QObject(parent)
{
    this->google = new QOAuth2AuthorizationCodeFlow(this);
    this->google->setScope("email");


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

    connect(this->google , &QOAuth2AuthorizationCodeFlow::authorizeWithBrowser, [=](QUrl url) {
        QUrlQuery query(url);

        query.addQueryItem("prompt", "consent");      // Param required to get data everytime
        query.addQueryItem("access_type", "offline"); // Needed for Refresh Token (as AccessToken expires shortly)
        url.setQuery(query);
        QDesktopServices::openUrl(url);
    });

    QByteArray val;
    QFile file;
    file.setFileName(":/google_auth.json");
    if(file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        val = file.readAll();
        file.close();
    }
    QJsonDocument document = QJsonDocument::fromJson(val);
    QJsonObject object = document.object();
    const auto settingsObject = object["web"].toObject();
    const QUrl authUri("https://omniedge-dev.auth.us-east-2.amazoncognito.com/login");
    const auto clientId = "7cvqu5n6n1c8pi327684a56k96";
    const QUrl tokenUri("https://omniedge-dev.auth.us-east-2.amazoncognito.com/oauth2/token");
    const auto clientSecret = "1vciunmp54piospic9tq5d0tr19cc0fgsk1fqqvrmqlfc2hspnmi";
    const QUrl redirectUri("http://localhost:8080/");
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
    this->google->setReplyHandler(replyHandler);

    connect(this->google, &QOAuth2AuthorizationCodeFlow::granted, [=](){
        qDebug() << this->google->token();
        qDebug() << this->google->refreshToken();
    });

}

void GoogleOAuth::grant()
{
    this->google->grant();
}
