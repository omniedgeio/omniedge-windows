#include "oauth.h"
#include <QSettings>
#include <QJsonObject>
#include <QJsonDocument>

OAuth::OAuth(QObject *parent) : QObject(parent)
{
    this->oAuthFlow = new QOAuth2AuthorizationCodeFlow(this);
    this->oAuthFlow->setScope("email");
    this->oAuthFlow->setScope("openid");

    // Get configurations
    QByteArray val;
    QFile file;
    file.setFileName(":/oauth.json");
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        val = file.readAll();
        file.close();

        qDebug() << "OAuth: Opened configuration file.";

        QJsonDocument document = QJsonDocument::fromJson(val);
        QJsonObject object = document.object();
        QJsonObject settingsObject = object["dev"].toObject();

        // Client id and client secret
        QString clientId = settingsObject["client_id"].toString();
        QString clientSecret = settingsObject["client_secret"].toString();

        // Uris
        QUrl redirectUri(settingsObject["redirect_uri"].toString());
        QUrl authUri(settingsObject["cognito_uri"].toString() + "/login");
        QUrl tokenUri(settingsObject["cognito_uri"].toString() + "/oauth2/token");
        quint16 port = static_cast<quint16>(redirectUri.port());

        this->oAuthFlow->setAccessTokenUrl(tokenUri);
        this->oAuthFlow->setAuthorizationUrl(authUri);
        this->oAuthFlow->setClientIdentifier(clientId);
        this->oAuthFlow->setClientIdentifierSharedKey(clientSecret);

        qDebug() << "OAuth: Intialize settings successfully.";

        this->replyHandler = new OAuthReplyHandler(port, this);

        connect(this->oAuthFlow, &QOAuth2AuthorizationCodeFlow::granted, this, &OAuth::granted);
        connect(this->replyHandler, &OAuthReplyHandler::tokensReceived, this, &OAuth::tokenReceived);
    } else {
        qDebug() << "OAuth: Cannot open configuration file.";
    }
}

void OAuth::grant()
{
    this->oAuthFlow->grant();
}

void OAuth::tokenReceived(const QVariantMap &data)
{
    QSettings settings;
    settings.setValue("IdToken", data["id_token"]);
    settings.setValue("AccessToken", data["access_token"]);
    settings.setValue("RefreshToken", data["refresh_token"]);
    qDebug() << "OAuth: Token received.";
    qDebug() << "OAuth: ID TOKEN = " << settings.contains("idToken");
    qDebug() << "OAuth: ACCESS TOKEN = " << settings.contains("accessToken");
    qDebug() << "OAuth: REFRESH TOKEN = " << settings.contains("refreshToken");
}

OAuth::~OAuth()
{
    delete this->oAuthFlow;
}
