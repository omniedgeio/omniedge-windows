#include "googleoauth.h"

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
    const QUrl authUri(settingsObject["auth_uri"].toString());
    const auto clientId = settingsObject["client_id"].toString();
    const QUrl tokenUri(settingsObject["token_uri"].toString());
    const auto clientSecret(settingsObject["client_secret"].toString());

    const auto redirectUris = settingsObject["redirect_uris"].toArray();
    const QUrl redirectUri(redirectUris[0].toString());
    const auto port = static_cast<quint16>(redirectUri.port());

    this->google->setAuthorizationUrl(authUri);
    this->google->setClientIdentifier(clientId);
    this->google->setAccessTokenUrl(tokenUri);
    this->google->setClientIdentifierSharedKey(clientSecret);

    auto replyHandler = new QOAuthHttpServerReplyHandler(port, this);
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
